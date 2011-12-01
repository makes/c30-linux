/*
 * tc-pic30.c -- Assembler for the dsPIC30 architecture.
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 * Modified by John Elliott (john.elliott@microchip.com)
 *
 * This file is part of GAS, the GNU Assembler.
 *
 * GAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GAS; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <as.h>
#include "subsegs.h"
#include "struc-symbol.h"
#include "macro.h"
#include <opcode/pic30.h>
#include "opcodes/pic30-opc.h"
#include <coff-pic30.h>
#include "obstack.h"
#include "bfd.h"
#if defined (OBJ_ELF)
#include "obj-elf.h"
#include "dwarf2dbg.h"
#else
#include "obj-coff.h"
#endif
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "write.h"
#include "pic30-utils.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../c30_resource/src/c30/c30_flag_definitions.h"


/******************************************************************************
 * PROTOTYPES
 ******************************************************************************/
#if defined(OBJ_ELF)
static void pic30_emit_leb128_expr PARAMS ((expressionS *exp, int sign));
static int pic30_output_sleb128 PARAMS ((char *p, offsetT value));
static int pic30_output_uleb128 PARAMS ((char *p, valueT value));
#endif
extern unsigned int pic30_attribute_map PARAMS ((asection *));
extern unsigned int pic30_extended_attribute_map PARAMS ((asection *));
extern int pic30_set_implied_attributes PARAMS ((asection *, unsigned char ));
static int pic30_align_power PARAMS ((offsetT));
static int pic30_address_align_power PARAMS ((bfd_vma));
static  bfd_vma pic30_address PARAMS ((bfd_vma));
static  bfd_vma pic30_element_size PARAMS ((bfd_vma));
extern int pic30_is_valid_attributes PARAMS ((unsigned int, unsigned char));
extern unsigned int pic30_is_auxpsv PARAMS ((void));
int subseg_readonly PARAMS ((segT));
int subseg_psv PARAMS ((segT));
int subseg_eedata PARAMS ((segT));
int subseg_dma PARAMS ((segT));
int subseg_code PARAMS ((segT));
int subseg_auxflash PARAMS ((segT));
int subseg_memory PARAMS ((segT));
int subseg_heap PARAMS ((segT));
int subseg_stack PARAMS ((segT));
static unsigned int pic30_next_char_of_string PARAMS ((void));
static char * demand_copy_string PARAMS ((int *));
unsigned char pic30_do_org PARAMS ((register segT, PTR,
                                           long, unsigned char));
void pic30_update_section_info PARAMS ((void));
void pic30_set_global_info PARAMS ((void));
static int pic30_update_processor PARAMS ((char *));
void pic30_display_section_info PARAMS ((FILE *, asection *, unsigned char));
void pic30_display_code_section_info PARAMS ((bfd *, asection *, PTR));
void pic30_display_data_section_info PARAMS ((bfd *, asection *, PTR));
void print_expression PARAMS ((const expressionS *));
long pic30_apply_operator PARAMS ((bfd_reloc_code_real_type,
                                   long, unsigned char));
short pic30_parse_operands PARAMS ((expressionS []));
static unsigned char pic30_populate_operand_value PARAMS
  ((const expressionS *, struct pic30_operand_value *, unsigned long));
struct pic30_opcode * pic30_find_opcode_based_on_operands PARAMS
  ((struct pic30_opcode *, expressionS [], short, unsigned short *,
  unsigned short *, unsigned short *, char **));
unsigned char pic30_operands_encode_identically PARAMS
  ((struct pic30_opcode *, expressionS []));
void pic30_create_call_insn PARAMS ((struct pic30_opcode *, expressionS []));
void pic30_create_goto_insn PARAMS ((struct pic30_opcode *, expressionS []));
void pic30_create_do_insn PARAMS ((struct pic30_opcode *, expressionS []));
void pic30_create_2word_insn PARAMS ((struct pic30_opcode *, expressionS []));
void pic30_create_insn PARAMS ((struct pic30_opcode *, expressionS [], short));
void pic30_check_isa PARAMS ((struct pic30_opcode *, expressionS []));
int md_chars_to_number PARAMS ((unsigned char *, int));
static void pic30_record_do PARAMS ((fixS *));
void pic30_relax_call PARAMS ((fragS *));
void pic30_prepare_relax_scan PARAMS ((fragS *, unsigned long,
                                       long *, relax_substateT,
                                       PTR));

/******************************************************************************
 * ENUMERATED TYPES
 ******************************************************************************/

enum pic30_relax_state
{
   NO_RELAX_STATE = -1,
   DO_RELAX,
   DO_NOT_RELAX
};
enum pic30_isa_state
{
   NO_ISA_STATE = -1,
   ISA_REV_A,
   ISA_REV_B
};
enum pic30_secure_segment
{
   NO_SECURE_SEG = -1,
   BOOT_SEG,
   SECURE_SEG
};

/*
 * NOTE:  If you add a enumerator to this list, make sure that you update the
 *        directives_not_supported array that follows.
 */
enum directive_not_supported
{
   BALIGN,
   BALIGNL,
   BALIGNW,
   DCB,
   DCB_B,
   DCB_D,
   DCB_L,
   DCB_S,
   DCB_W,
   DCB_X,
   DS,
   DS_B,
   DS_D,
   DS_L,
   DS_P,
   DS_S,
   DS_W,
   DS_X,
   OCTA,
   P2ALIGN,
   P2ALIGNL,
   P2ALIGNW,
   QUAD,
   ZERO
};

/******************************************************************************
 * STRUCTURES
 ******************************************************************************/

/*
 * This information is required by data allocation pseudo-ops.  The
 * p_directive member states whether this is a program word directive.
 * The number_of_bytes member is the number of bytes that will be allocated
 * by this directive.  This information is passed to the pic30_cons function
 * to set up the global_p_directive_active global variable and to pass to the
 * cons function.
 */
typedef struct pic30_cons_structure {
   char * name;
   unsigned char p_directive;
   int number_of_bytes;
} pic30_consS;

/*
** Data structures for validating DO LOOP restrictions.
*/
struct pic30_do_fixup
{
  struct pic30_do_fixup *next; /* Next DO LOOP fixup.  */
  fixS *fixP;                  /* This fixup.  */
  long endaddress;             /* DO LOOP end address */
  segT seg;                    /* The section this fixup is in.  */
};

/*
** The list of DO LOOP relocs.
*/
static struct pic30_do_fixup *pic30_do_fixup_list;

/* A list of equated symbols */
static symbolS **pic30_equate_symbols;
static int pic30_symbol_count;
static int pic30_max_symbols;
#define PIC30_INIT_SYM_COUNT 20
#define PIC30_GROW_SYM_COUNT 10


/******************************************************************************
 * GLOBALS
 ******************************************************************************/

extern const bfd_arch_info_type * pic30_get_machine(const char * const);
extern const bfd_arch_info_type * global_PROCESSOR;
extern int pic30_proc_family(const bfd_arch_info_type *);
extern int pic30_is_dsp_machine(const bfd_arch_info_type *);
extern int pic30_is_eedata_machine(const bfd_arch_info_type *);
extern int pic30_is_dma_machine(const bfd_arch_info_type *);
extern int pic30_is_codeguard_machine(const bfd_arch_info_type *);
extern int pic30_is_pmp_machine(const bfd_arch_info_type *);
extern int pic30_is_epmp_machine(const bfd_arch_info_type *);
extern int pic30_is_eds_machine(const bfd_arch_info_type *);
extern int pic30_is_5V_machine(const bfd_arch_info_type *);

static struct hash_control * pic30_opcodes_hash;
static enum pic30_relax_state global_RELAX = NO_RELAX_STATE;
static enum pic30_isa_state global_ISA = ISA_REV_B;
static unsigned long global_current_opcode_flags = F_NONE;
static unsigned short global_current_operand_position = 0;
static unsigned char has_cpu_option = FALSE;
static unsigned char has_proc_option = FALSE;
enum pic30_debuginfo_state global_DEBUGINFO = NO_GEN_DEBUGINFO;
unsigned char global_fillupper_value = NOP_OPCODE;
unsigned char global_fillvalue_value = DEFAULT_FILL_VALUE;
unsigned char global_pfillvalue_value = DEFAULT_FILL_VALUE;
unsigned long global_current_location = 0;
unsigned char global_must_align_location_counter = FALSE;
unsigned char global_p_directive_active = FALSE;
unsigned char global_long_directive_active = FALSE;
segT pic30_last_code_section;

static segT previous_section;
struct section_stack
{
  struct section_stack *next;
  segT seg, prev_seg;
};

static struct section_stack *section_stack;


/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define PIC30_DO_CODE_ALIGNMENT()                          \
   do {                                                    \
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))  \
         record_alignment (now_seg, PIC30_CODE_ALIGNMENT); \
                                                           \
      global_must_align_location_counter = FALSE;          \
   } while (0);

#define PIC30_RESET_GLOBAL_FILL_VALUES()                   \
   do {                                                    \
      global_fillupper_value = NOP_OPCODE;                 \
      global_fillvalue_value = DEFAULT_FILL_VALUE;         \
      global_pfillvalue_value = DEFAULT_FILL_VALUE;        \
   } while (0);

/*
 * The following defines are the power of 2 alignment values for SEC_CODE vs.
 * SEC_DATA sections. They are given in "addressible units" which means bytes
 * for SEC_DATA sections and 16-bit words for SEC_CODE sections.
 */
#define PIC30_CODE_ALIGNMENT 1
#define PIC30_DATA_ALIGNMENT 1

/*
 * These defines are used to define the machine independent options accepted.
 */
#define PIC30_BASE_OPTION          (OPTION_MD_BASE)
#define PIC30_NO_RELAX             (PIC30_BASE_OPTION + 0)
#define PIC30_RELAX                (PIC30_BASE_OPTION + 1)
#define PIC30_CPU                  (PIC30_BASE_OPTION + 2)

/* Number of littlenums required to hold an extended precision number */
#define MAX_LITTLENUMS 6

/* The size of a .fixed */
#define PIC30_FIXED_SIZE 2

/******************************************************************************/

/*
 * This is a const char * which GAS adds to the machine independent string
 * passed to getopt.
 */
const char * md_shortopts = "p:";

/*
 * This is a struct option [] which gas adds to the machine independent long
 * options passed to getopt.  You may use OPTION_MD_BASE, defined in 'as.h',
 * as the start of a set of long option indices.
 */
struct option md_longopts[] =
{
   { "no-relax",     no_argument,       NULL, PIC30_NO_RELAX  },
   { "relax",        no_argument,       NULL, PIC30_RELAX     },
   { "processor",    required_argument, NULL, 'p'             },
   { "cpu",          required_argument, NULL, PIC30_CPU       },
   { "g",            no_argument,       NULL, 'g'             },
   { NULL,           no_argument,       NULL, 0               }
};

/*
 * This is a size_t holding the size of md_longopts.  NOTE:  This is not the
 * number of structures in this array.
 */
size_t md_longopts_size = (size_t)(sizeof (md_longopts));

/*
 * NOTE:  The order of these should directly correspond to the above enumerated
 *        type -- directive_not_supported.
 */
const char * const directives_not_supported[] =
{
   ".balign",
   ".balignl",
   ".balignw",
   ".dcb",
   ".dcb.b",
   ".dcb.d",
   ".dcb.l",
   ".dcb.s",
   ".dcb.w",
   ".dcb.x",
   ".ds",
   ".ds.b",
   ".ds.d",
   ".ds.l",
   ".ds.p",
   ".ds.s",
   ".ds.w",
   ".ds.x",
   ".octa",
   ".p2align",
   ".p2alignl",
   ".p2alignw",
   ".quad",
   ".zero"
};

const int number_of_directives_not_supported = 
   (sizeof(directives_not_supported) / sizeof (const char * const));

/* Table that contains information for data allocation pseudo-ops. */
const pic30_consS pic30_cons_table[] =
{
#define PIC30_CONS_BYTE     0
   { ".byte",  FALSE, 1 },
#define PIC30_CONS_DC       1
   { ".dc",    FALSE, 2 },
#define PIC30_CONS_DC_B     2
   { ".dc.b",  FALSE, 1 },
#define PIC30_CONS_DC_L     3
   { ".dc.l",  FALSE, 4 },
#define PIC30_CONS_DC_W     4
   { ".dc.w",  FALSE, 2 },
#define PIC30_CONS_HWORD    5
   { ".hword", FALSE, 2 },
#define PIC30_CONS_INT      6
   { ".int",   FALSE, 4 },
#define PIC30_CONS_LONG     7
   { ".long",  FALSE, 4 },
#define PIC30_CONS_PBYTE    8
   { ".pbyte", TRUE,  1 },
#define PIC30_CONS_PWORD    9
   { ".pword", TRUE,  3 },
#define PIC30_CONS_SHORT   10
   { ".short", FALSE, 2 },
#define PIC30_CONS_WORD    11
   { ".word",  FALSE, 2 }
};

/* Function prototypes for pseudo-ops. */
void pic30_align (int);
void pic30_attribute (int);
void pic30_bss (int);
void pic30_cons (int);
void pic30_data (int);
void pic30_error (int);
void pic30_fill (int);
void pic30_fillupper (int);
void pic30_fillvalue (int);
void pic30_fixed_cons (int);
void pic30_flags (int);
void pic30_float_cons (int);
void pic30_incbin (int);
void pic30_include (int);
void pic30_memory (int);
void pic30_not_supported (int);
void pic30_org (int);
void pic30_pfillvalue (int);
void pic30_pstringer (int);
void pic30_section (int);
void pic30_popsection (int);
void pic30_space (int);
void pic30_text (int);
#if defined(OBJ_ELF)
void pic30_s_leb128 (int);
#endif

/*
 * The target specific pseudo-ops which we support.  This is a mapping
 * from pseudo-op names to functions.  Use this table to implement
 * pseudo-ops which are specific to the CPU.
 *
 * The following pseudo-ops are defined in the read.c file:
 *   {"extern", s_ignore, 0},      // We treat all undef as ext
 *   {"global", s_globl, 0},
 *   {"name", s_ignore, 0},
 *   {"list", listing_list, 1},    // Turn listing on
 *   {"nolist", listing_list, 0},  // Turn listing off
 *   {"title", listing_title, 0},
 *   {"end", s_end, 0},
 */
const pseudo_typeS md_pseudo_table[] =
{
/* { pseudo-op,    function handler,    argument         } */
   { "align",      pic30_align,         0                },
   { "ascii",      pic30_stringer,      0                },
   { "asciz",      pic30_stringer,      1                },
   { "attribute",  pic30_attribute,     0                },
   { "balign",     pic30_not_supported, (int) BALIGN     },
   { "balignw",    pic30_not_supported, (int) BALIGNW    },
   { "balignl",    pic30_not_supported, (int) BALIGNL    },
   { "bss",        pic30_bss,           0                },
   { "byte",       pic30_cons,          PIC30_CONS_BYTE  },
   { "data",       pic30_data,          0                },
   { "dc",         pic30_cons,          PIC30_CONS_DC    },
   { "dc.b",       pic30_cons,          PIC30_CONS_DC_B  },
   { "dc.l",       pic30_cons,          PIC30_CONS_DC_L  },
   { "dc.w",       pic30_cons,          PIC30_CONS_DC_W  },
   { "dcb",        pic30_not_supported, (int) DCB        },
   { "dcb.b",      pic30_not_supported, (int) DCB_B      },
   { "dcb.d",      pic30_not_supported, (int) DCB_D      },
   { "dcb.l",      pic30_not_supported, (int) DCB_L      },
   { "dcb.s",      pic30_not_supported, (int) DCB_S      },
   { "dcb.w",      pic30_not_supported, (int) DCB_W      },
   { "dcb.x",      pic30_not_supported, (int) DCB_X      },
   { "double",     pic30_float_cons,    'd'              },
   { "ds",         pic30_not_supported, (int) DS         },
   { "ds.b",       pic30_not_supported, (int) DS_B       },
   { "ds.d",       pic30_not_supported, (int) DS_D       },
   { "ds.l",       pic30_not_supported, (int) DS_L       },
   { "ds.p",       pic30_not_supported, (int) DS_P       },
   { "ds.s",       pic30_not_supported, (int) DS_S       },
   { "ds.w",       pic30_not_supported, (int) DS_W       },
   { "ds.x",       pic30_not_supported, (int) DS_X       },
   { "error",      pic30_error,         0                },
   { "fill",       pic30_fill,          0                },
   { "fillupper",  pic30_fillupper,     0                },
   { "fillvalue",  pic30_fillvalue,     0                },
   { "fixed",      pic30_fixed_cons,    0                },
   { "flags",      pic30_flags,         0                },
   { "float",      pic30_float_cons,    'f'              },
   { "hword",      pic30_cons,          PIC30_CONS_HWORD },
   { "int",        pic30_cons,          PIC30_CONS_INT   },
   { "incbin",     pic30_incbin,        0                },
   { "include",    pic30_include,       0                },
   { "long",       pic30_cons,          PIC30_CONS_LONG  },
   { "memory",     pic30_memory,        0                },
   { "octa",       pic30_not_supported, (int) OCTA       },
   { "org",        pic30_org,           0                },
   { "p2align",    pic30_not_supported, (int) P2ALIGN    },
   { "p2alignw",   pic30_not_supported, (int) P2ALIGNW   },
   { "p2alignl",   pic30_not_supported, (int) P2ALIGNL   },
   { "palign",     pic30_align,         1                },
   { "pascii",     pic30_pstringer,     0                },
   { "pasciz",     pic30_pstringer,     1                },
   { "pbyte",      pic30_cons,          PIC30_CONS_PBYTE },
   { "pfill",      pic30_fill,          1                },
   { "pfillvalue", pic30_pfillvalue,    0                },
   { "pincbin",    pic30_incbin,        1                },
   { "popsection", pic30_popsection,    0                },
   { "porg",       pic30_org,           1                },
   { "pskip",      pic30_space,         1                },
   { "pspace",     pic30_space,         1                },
   { "pushsection", pic30_section,      1                },
   { "pword",      pic30_cons,          PIC30_CONS_PWORD },
#ifndef BFD64
   { "quad",       pic30_not_supported, (int) QUAD       },
#endif
   { "short",      pic30_cons,          PIC30_CONS_SHORT },
   { "section",    pic30_section,       0                },
   { "sect",       pic30_section,       0                },
   { "section.s",  pic30_section,       0                },
   { "sect.s",     pic30_section,       0                },
   { "single",     pic30_float_cons,    'f'              },
   { "skip",       pic30_space,         0                },
#ifdef OBJ_ELF
   { "sleb128",    pic30_s_leb128,      1                },
#endif
   { "space",      pic30_space,         0                },
   { "pstring",    pic30_pstringer,     1                },
   { "string",     pic30_stringer,      1                },
   { "text",       pic30_text,          0                },
#ifdef OBJ_ELF
   { "uleb128",    pic30_s_leb128,      0                },
#endif
   { "word",       pic30_cons,          PIC30_CONS_WORD  },
   { "zero",       pic30_not_supported, (int) ZERO       },
   { NULL,         NULL,                0                }
};

/*
 * This is a null terminated const char array of characters which start a
 * comment.
 */
const char comment_chars[] = ";";

/*
 * This is a null terminated const char array of characters which start a
 * comment when they appear at the start of a line.
 */
const char line_comment_chars[] = "#";

/*
 * This is a null terminated const char array of characters which separate
 * lines.  NOTE:  They are not looking for the newline ('\n') character here.
 * This is if you had something similar to the C language's semicolon.
 */
const char line_separator_chars[] = "";

/*
 * This is a null terminated const char array of characters which may be used
 * as the exponent character in a floating point number.  This is normally "eE".
 */
const char EXP_CHARS[] = "Ee";

/*
 * This is a null terminated const char array of characters which may be used
 * to indicate a floating point constant.  A zero followed by one of these
 * characters is assumed to be followed by a floating point number; thus they
 * operate the way that 0x is used to indicate a hexidecimal constant.
 */
const char FLT_CHARS[] = "DdEeFf";

/*
** This table describes how you change sizes for the various types of variable
** size expressions.
** For dsPIC, this is used for relaxing 23-bit abolute addresses to
** 16-bit relative addresses in CALL->RCALL and GOTO->BRA.
*/
const relax_typeS md_relax_table[] =
{
   /*
    * The fields are:
    * 1) Most positive reach of this state
    * 2) Most negative reach of this state
    * 3) How many bytes this state will add to the size of the current frag
    * 4) Which index into this table to try if we cannot fit into this one
    */

   /*
    * The first entry must be unused because an `rlx_more' value of zero ends
    * each list.
    */
#define PIC30_RELAX_END		(0)
#define PIC30_RELAX_RCALL	(1)
#define PIC30_RELAX_CALL	(2)
#define PIC30_RELAX_BRA		(3)
#define PIC30_RELAX_GOTO	(4)
   { 1,		1,	0, 0 },
   { 32767,	-32768, 0, PIC30_RELAX_CALL },
   { 8388606,	0,	4, PIC30_RELAX_END },
   { 32767,	-32768, 0, PIC30_RELAX_GOTO },
   { 8388606,	0,	4, PIC30_RELAX_END }
};

/******************************************************************************/

/*
 *  HOST_SPECIAL_INIT : add default include path, if possible
 */

#ifndef DIR_SEPARATOR
#define DIR_SEPARATOR '/'
#endif

#if defined (_WIN32) || defined (__MSDOS__) || defined (__DJGPP__) || \
  defined (__OS2__)
#define HAVE_DOS_BASED_FILE_SYSTEM
#ifndef DIR_SEPARATOR_2
#define DIR_SEPARATOR_2 '\\'
#define EXE_SUFFIX ".exe"
#endif
#endif

/* Define IS_DIR_SEPARATOR.  */
#ifndef DIR_SEPARATOR_2
# define IS_DIR_SEPARATOR(ch) ((ch) == DIR_SEPARATOR)
#else /* DIR_SEPARATOR_2 */
# define IS_DIR_SEPARATOR(ch) \
        (((ch) == DIR_SEPARATOR) || ((ch) == DIR_SEPARATOR_2))
#endif /* DIR_SEPARATOR_2 */

#ifndef DIR_SEPARATOR_2
#define DIR_SEPARATOR_2 DIR_SEPARATOR
#define EXE_SUFFIX ""
#endif

#define IS_PATH_SEPARATOR(ch) (((ch) == ';') || ((ch) == ':'))

void 
pic30_init(int argc __attribute__ ((__unused__)), char **argv)
{ char *f,safe;
  char *base = 0, *last = 0;
  char *buf;
  unsigned int buflen=80;
  char *PATH;
  int proc_family = pic30_proc_family(global_PROCESSOR);

  buf = xmalloc(buflen);
  for (f = argv[0];  *f; f++) {
    if (IS_DIR_SEPARATOR(*f)) {
      base = f;
    }
  }
  if (base == 0) {
    /* no path information in argv[0]; try to locate pic30-as in PATH */
    char *pseparator;
    char *p;

    PATH = getenv("PATH");
    if (PATH == 0) {
      PATH=".";
    } else {
      char *newPATH = xmalloc(strlen(PATH)+3);
   
      sprintf(newPATH, "%s:.", PATH);
      PATH=newPATH;
    }
    p = PATH;
    do {
      struct stat statbuf;

      for (pseparator = p; *pseparator && !IS_PATH_SEPARATOR(*pseparator); 
           pseparator++);
      safe = *pseparator;
      if (*pseparator) {
        *pseparator = 0;
      }
 
      if ((strlen(p) + 2 + sizeof(EXE_SUFFIX) + sizeof("pic30-as")) >= buflen) {
        buflen = (strlen(p) + sizeof(EXE_SUFFIX) + sizeof("pic30-as") + 20);
        buf = xrealloc(buf, buflen);
      }
      sprintf(buf, "%s%cpic30-as%s", p, DIR_SEPARATOR_2, EXE_SUFFIX);
      if (stat(buf,&statbuf) == 0) break;
      *pseparator = safe;
      if (safe) p = pseparator+1; else p = pseparator;
      safe = 0;
      if (*p == 0) break;
    } while(1);
    base = p;
  } else {
    safe = *base;
    *base = 0;
    f = base;
    base = argv[0];
  }
  if (base) {
    char *include_path;

    for (f = base;  *f; f++) {
      if (IS_DIR_SEPARATOR(*f)) {
        last = f;
      }
    }
    if (safe) {
      *f = safe;
      safe = 0;
    };
    if (last) {
      safe = *last;
      *last = 0;
      f = last;
    }
    switch (proc_family)
    {
      case P30F:
      case P30FSMPS:
        include_path = strdup(MPLABC30_PIC30F_INC_PATH);
        break;
      case P33E:
        include_path = strdup(MPLABC30_PIC33E_INC_PATH);
        break;
      case P33F:
        include_path = strdup(MPLABC30_PIC33F_INC_PATH);
        break;
      case P24E:
        include_path = strdup(MPLABC30_PIC24E_INC_PATH);
        break;
      case P24F:
      case P24FK:
        include_path = strdup(MPLABC30_PIC24F_INC_PATH);
        break;
      case P24H:
        include_path = strdup(MPLABC30_PIC24H_INC_PATH);
        break;
      default:
        include_path = 0;
    }
    if (include_path) {
      char *c;
      char *start = include_path;

      if (strlen(base) + strlen(include_path) + 2 >= buflen) {
        buflen = strlen(base) + strlen(include_path) + 2;
        buf = xrealloc(buf, buflen);
      }
      for (c = include_path; 1; c++) {
        if (*c == DIR_SEPARATOR_STR[0]) *c = DIR_SEPARATOR_2;
        if (*c == PATH_SEPARATOR_STR[0]) {
          *c = 0;
          sprintf(buf, "%s%c%s", base, DIR_SEPARATOR_2, start);
          add_include_dir(buf);
          *c = ':';
          start = ++c;
        } else if (*c == 0) {
          sprintf(buf, "%s%c%s", base, DIR_SEPARATOR_2, start);
          add_include_dir(buf);
          break;
        }
      }
      free(include_path);
    }
    if (safe) *f = safe;
  }
}

/******************************************************************************
 *
 *   Return whether the specified segment is thought to hold READONLY data.
 *
 *   Note: this is a clone of subseg_text_p() in subsegs.c
 *
 ******************************************************************************/
int
subseg_readonly(segT sec)
{
  return (bfd_get_section_flags (stdoutput, sec) & SEC_READONLY) != 0;
}
/******************************************************************************
 *
 *   Return whether the specified segment is thought to hold PSV data.
 *
 *   Note: this is a clone of subseg_text_p() in subsegs.c
 *
 ******************************************************************************/
int
subseg_psv(segT sec)
{
  return ((bfd_get_section_flags (stdoutput, sec) & SEC_READONLY) != 0) |
    sec->psv;
}

/******************************************************************************
 *
 *   Return whether the specified segment is thought to hold EEDATA data.
 *
 *   Note: this is a clone of subseg_text_p() in subsegs.c
 *
 ******************************************************************************/
int
subseg_eedata(segT sec)
{
  return PIC30_IS_EEDATA_ATTR(sec);
}
/******************************************************************************
 *
 *   Return whether the specified segment is thought to hold DMA data.
 *
 ******************************************************************************/
int
subseg_dma(segT sec)
{
  return PIC30_IS_DMA_ATTR(sec);
}
/******************************************************************************/
int
subseg_code(segT sec)
{
  return PIC30_IS_CODE_ATTR(sec);
}
/******************************************************************************/int
subseg_auxflash(segT sec)
{
  return PIC30_IS_AUXFLASH_ATTR(sec);
  //return (bfd_get_section_flags (stdoutput, sec) & sec->auxflash);
}
/******************************************************************************/
int
subseg_memory(segT sec)
{
  return PIC30_IS_MEMORY_ATTR(sec);
}
/******************************************************************************/int
subseg_heap(segT sec)
{
  return PIC30_IS_HEAP_ATTR(sec);
}
/******************************************************************************/int
subseg_stack(segT sec)
{
  return PIC30_IS_STACK_ATTR(sec);
}
/******************************************************************************/

void
pic30_align (p_directive)
   int p_directive;

/******************************************************************************
 *
 *   Handles the .ALIGN and .PALIGN pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   p_directive       This argument determines whether the .palign or .align
 *                     directive was used.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_align (%s)::begin\n",
              p_directive ? "TRUE" : "FALSE");

   global_p_directive_active = p_directive;

   if ((p_directive) && (!subseg_text_p (now_seg) && (now_seg != text_section)))
   {
      as_warn (_(".palign not specified in a code section.  "
                 ".palign ignored."));
      discard_rest_of_line ();
   } /* if (p_directive) */
   else
      s_align_bytes (0);

   if (flag_debug)
      printf ("<-- pic30_align::exit\n");
} /* void pic30_align(int) */

/******************************************************************************/

void
pic30_flags (ignore)
   int ignore __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .FLAGS pseudo-op.  
 *
 *   Sets the current section flags to the absolute value specified.
 *   If zero is specified, also clears section attribute fields that
 *   are not encoded in the attribute word.
 *
 *   Note: This attribute is intended for diagnostic purposes.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/

{
  if (flag_debug)
    printf ("--> pic30_flags::begin\n");

  if (!is_it_end_of_statement ())
    {
      offsetT absolute_value;

      SKIP_WHITESPACE ();
      absolute_value = get_absolute_expression ();

      if (!bfd_set_section_flags(stdoutput, now_seg, absolute_value))
         as_bad (_("could not set flags for section %s."), now_seg->name);

      if (absolute_value == 0) {
        now_seg->unordered = 0;
        now_seg->near = 0;
        now_seg->persistent = 0;
        now_seg->xmemory = 0;
        now_seg->ymemory = 0;
        now_seg->psv = 0;
        now_seg->eedata = 0;
        now_seg->absolute = 0;
        now_seg->reverse = 0;
        now_seg->dma = 0;
        now_seg->boot = 0;
        now_seg->secure = 0;
        now_seg->memory = 0;
        now_seg->heap = 0;
        now_seg->stack = 0;
        now_seg->eds = 0;
        now_seg->page = 0;
	now_seg->auxflash = 0;
      }

      if (flag_debug) {
          printf ("    pic30_flags::input value = 0x%lx\n", absolute_value);
          printf ("    pic30_flags::sec_flags = 0x%x\n", now_seg->flags);
          printf ("    pic30_flags::map = 0x%x\n", pic30_attribute_map(now_seg));
      }

    } else
         as_bad (_(".flags expects a constant value."));

    if (flag_debug)
      printf ("<-- pic30_flags::exit\n");
  } /* pic30_flags */

/******************************************************************************/

/*
** Convert align(),reverse() arg to a power of two
*/
static int
pic30_align_power (offsetT bytes)
{
  unsigned int result = (unsigned int) bytes;
  /* Convert to a power of 2.  */
  if (result != 0)
    {
      unsigned int i;
	
      if (result > 32768)
        as_bad(_("section alignment cannot exceed 32768"));

      for (i = 0; (result & 1) == 0; result >>= 1, ++i)
        ;
      if (result != 1)
        as_bad (_("section alignment must be a power of 2"));
	
      result = i;
    }
  return result;
} /* pic30_align_power */

/*
** Compute alignment of an address
*/
static int
pic30_address_align_power (bfd_vma addr)
{
  int result = 15; /* default, if addr = 0 */

  /* Convert to a power of 2.  */
  if (addr != 0)
    {
      unsigned int i;
	
      for (i = 0; (addr & 1) == 0; addr >>= 1, ++i)
        ;
      result = i;
    }
  return result;
} /* pic30_address_alignment */

/*
** Validate address() arg
*/
static bfd_vma
pic30_address (bfd_vma addr)
{
  if (!PIC30_IS_EVEN(addr))
    as_bad (_("section address must be even"));

  return addr;
} /* pic30_address */

/*
** Validate merge() arg
*/
static bfd_vma
pic30_element_size (bfd_vma size)
{
  if (size > 0x100)
    as_bad (_("element size must be in range [0..256]"));

  return size;
} /* pic30_element_size */

#define SKIP_COMMA()                    \
  {                                     \
    if (* input_line_pointer == ',')    \
      ++ input_line_pointer;            \
  }

static void report_target_attribute_err (char *attr)
{
  as_bad (_("processor %s does not support %s memory"),
          global_PROCESSOR->printable_name, attr );
}

/*
** Create a symbol in the object file
*/
static void
create_object_file_symbol(char *name, valueT val)
{
  symbolS * symbolp;
  symbolp = symbol_new (name, absolute_section,
                            val, &zero_address_frag);

  symbol_table_insert (symbolp);
  symbol_mark_resolved (symbolp);
}

/*
** record a secure access entry point
**  in the object's symbol table
**
**  slot = -1 means unused access entry
**  slot = -2 means interrupt access entry
*/
static void
record_access_entry(const char *name, int seg, int slot)
{
  const char *s0 = "unused";
  const char *s00 = "isr";
  const char *s1 = "__boot_func:";
  const char *s2 = "__secure_func:";
  char *s3;
  symbolS *symbolP;

  switch (seg) {

  case BOOT_SEG:
    s3 = xmalloc( strlen(s1) + strlen(s0) + 1);
    if (slot == -1)
      sprintf(s3, "%s%s", s1, s0);
    else if (slot == -2)
      sprintf(s3, "%s%s", s1, s00);
    else
      sprintf(s3, "%s%d", s1, slot);
    break;

  case SECURE_SEG:
    s3 = xmalloc( strlen(s2) + strlen(s0) + 1);
    if (slot == -1)
      sprintf(s3, "%s%s", s2, s0);
    else if (slot == -2)
      sprintf(s3, "%s%s", s2, s00);
    else
      sprintf(s3, "%s%d", s2, slot);
    break;

  default:
    s3 = (char *) name;
  }

  /* make an external label we can branch to */
  symbolP = symbol_find_or_make (s3);
  S_SET_EXTERNAL (symbolP);
  colon (s3);
}

static char * valid_argument_names[] = {
  "unused",
  "isr",
  0, };

static int
is_valid_argument_name(const char *s)
{
  int i = 0;
  while (valid_argument_names[i]) {  /* null terminated list */
    if (strcmp(s, valid_argument_names[i++]) == 0)
      return 1;
  }
  return 0;
}

void
pic30_attribute (is_section)
   int is_section;

/******************************************************************************
 *
 *   Handles the .ATTRIBUTE pseudo-op.  
 *
 *   Sets attributes for the current section.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   is_section        True if this function has been called to process
 *                     a .section directive. Otherwise false.
 *
 ******************************************************************************/

#define NEAR_DATA_LIMIT 0x1FFF
#define NEAR_CODE_LIMIT 0x7FFF

#define PSV_BASE 0x8000
#define PSV_MASK 0x7FFF

#define SECTION(s) \
  ((strlen(sec->name) == strlen(#s)) && \
           strcmp(sec->name, (#s)) == 0)

{
  asection *sec = now_seg;
  symbolS *symbolp;
  unsigned int attribute_map = 0;
  unsigned int old_map = pic30_attribute_map(sec);
  const char *access_err_msg1 = "Access entry points are supported"
                                " only in CODE and in AUXFLASH sections";
  const char *access_err_msg2 = " must be in the range 0..15 or 17..31";
  const char *access_err_msg3 = " is not recognized as a valid argument of";
  const char *info_sec_prefix = "__memory_";
  char * info_sec_name;
  bfd_boolean access_entry = FALSE;
  bfd_vma      old_lma = sec->lma;

  if (flag_debug) {
    printf ("--> pic30_attribute::begin\n");
    printf ("    pic30_attribute::old_map = 0x%x\n", old_map);
  }

  if (is_it_end_of_statement ()) {
    if (is_section)
      {} /* no attribute list is OK */
    else
      as_bad (_(".attribute expects an attribute name"));
  }

#define MAX_ATTR_LEN 20
  while (!is_it_end_of_statement ())
    {
      offsetT arg = 0;
      int has_arg = 0;
      int has_alpha_arg = 0;
      char *alpha_arg = NULL;
      int is_valid = 0;
      int index = 0;
      char *name, *keep;
      char c,s[MAX_ATTR_LEN],s2[MAX_ATTR_LEN];
      unsigned int i,len,len2;

      SKIP_COMMA ();
      SKIP_WHITESPACE ();

      name = input_line_pointer;
      c = get_symbol_end ();

      /* make a local copy, lower case */
      name = strncpy(s, name, sizeof(s));
      len = strlen(name);
      for(i = 0; i < len; i++)
        s[i] = tolower(s[i]);
      
      if (((strcmp(name, "auxflash") & strcmp(name, "auxpsv")) == 0) &&
	  !pic30_is_ecore_machine(global_PROCESSOR))
	{
	   as_bad (_("%s attribute cannot be used with non-EP devices"), name);
	}

      /* get the argument, if any */
      *input_line_pointer = c;
      if (c == '(') {
        has_arg++;
        input_line_pointer++;
        SKIP_WHITESPACE ();

        /* test for valid string argument */
        keep = alpha_arg = input_line_pointer;
        c = get_symbol_end (); /* advances input_line_pointer, inserts NULL */

        len2 = sizeof(s2);
        alpha_arg = strncpy(s2, alpha_arg, len2);      /* make a local copy */
        s2[len2-1] = 0;                         /* make sure its terminated */

        *input_line_pointer = c;            /* replace char that was NULLed */

        if (strcmp(name, "memory") == 0)
          has_alpha_arg++;       /* argument to memory() is always a string */
        else {
          for(i = 0; s2[i]; i++)            /* make the argument lower case */
            s2[i] = tolower(s2[i]);
          if (is_valid_argument_name(alpha_arg))  /* and check the std list */
            has_alpha_arg++;
        }

        if (!has_alpha_arg) {                   /* if not a valid alpha arg */
          input_line_pointer = keep;          /* restore input_line_pointer */
          arg = get_absolute_expression ();  /* try to evaluate as absolute */
        }

        input_line_pointer++;                     /* skip the closing paren */
      }

      if (flag_debug) {
          printf ("    pic30_attribute::attribute = %s\n", name);
          if (has_alpha_arg)
            printf ("    pic30_attribute::argument  = %s\n", alpha_arg);
          else if (has_arg)
            printf ("    pic30_attribute::argument  = 0x%lx\n", arg);
      }

#undef ATTR
#undef ATTR_IS
#undef MASK1
#undef MASK2
#undef MASK3
#undef MASK4
#define PIC30_SET_MEMORY_ATTR_ARG(a)                                \
        if (has_arg) {                                              \
          asection *memsec;                                         \
          info_sec_name = xmalloc (strlen(alpha_arg) +              \
                          strlen(info_sec_prefix) + 1);             \
          (void) sprintf(info_sec_name, "%s%s",                     \
           info_sec_prefix, alpha_arg);                             \
          memsec = subseg_new (info_sec_name, 0);                   \
          PIC30_SET_INFO_ATTR(memsec);                              \
          symbolp = symbol_new (sec->name, absolute_section,        \
                               (valueT) 1, &zero_address_frag);     \
          fix_new (frag_now, 0, 2, symbolp, 0, FALSE,               \
          BFD_RELOC_PIC30_PGM_ADDR_LSB);                            \
          (void) subseg_new (sec->name, 0);                         \
        }
#define PIC30_SET_ABSOLUTE_ATTR_ARG(a)                              \
        sec->vma = pic30_address(a);                                \
        sec->lma = pic30_address(a);
#define PIC30_SET_ALIGN_ATTR_ARG(a)                                 \
        record_alignment (sec, pic30_align_power(arg));
#define PIC30_SET_MERGE_ATTR_ARG(a)                                 \
        sec->entsize = pic30_element_size(arg);
#define PIC30_SET_BOOT_ATTR_ARG(a)                                  \
        if (has_arg)  {                                             \
          access_entry = TRUE;                                      \
          if (has_alpha_arg && (strcmp(alpha_arg, "unused") == 0))  \
            record_access_entry(sec->name, BOOT_SEG, -1);           \
          else if (has_alpha_arg && (strcmp(alpha_arg, "isr") == 0)) \
            record_access_entry(sec->name, BOOT_SEG, -2);           \
          else if (has_alpha_arg)                                   \
            as_bad(_("\'%s\'%s boot()"), alpha_arg, access_err_msg3);   \
          else if (((a) < 0) || ((a) > 31) || ((a) == 16))          \
            as_bad(_("Argument to boot()%s"), access_err_msg2);     \
          else                                                      \
            record_access_entry(sec->name, BOOT_SEG, (a));          \
        }
#define PIC30_SET_SECURE_ATTR_ARG(a)                                \
        if (has_arg)  {                                             \
          access_entry = TRUE;                                      \
          if (has_alpha_arg && (strcmp(alpha_arg, "unused") == 0))  \
            record_access_entry(sec->name, SECURE_SEG, -1);         \
          else if (has_alpha_arg && (strcmp(alpha_arg, "isr") == 0)) \
            record_access_entry(sec->name, SECURE_SEG, -2);         \
          else if (has_alpha_arg)                                   \
            as_bad(_("\'%s\'%s secure()"), alpha_arg, access_err_msg3); \
          else if (((a) < 0) || ((a) > 31) || ((a) == 16))          \
            as_bad(_("Argument to secure()%s"), access_err_msg2);   \
          else                                                      \
            record_access_entry(sec->name, SECURE_SEG, (a));        \
        }
#define ATTR(_id, _has_arg, _set_it)                                \
      if ((strlen(#_id) == len) && (strcmp( name, (#_id)) == 0)) {  \
        if (_has_arg && !has_arg)                                   \
          as_bad (_("%s requires an argument."), name);             \
        _set_it;                                                    \
        is_valid |= 1;                                              \
        attribute_map |= (1 << index);                              \
      }                                                             \
      index++;
#include "pic30-attributes.h"

      if (strcmp(name, "auxpsv")== 0)
	{
	   is_valid |= 1; 
	  attribute_map |= pic30_is_auxpsv();
	  PIC30_SET_AUXFLASH_ATTR(sec);
	  PIC30_SET_PAGE_ATTR(sec);
	 
	}

      if (!is_valid) {
        as_bad (_("\'%s\' is not a valid attribute name"), name);
        demand_empty_rest_of_line ();
        break;
      }

    } /* while (!is_it_end_of_statement ()) */

  if (flag_debug)
    printf ("    pic30_attribute::map = %x\n", attribute_map);

  /*
  ** If we are processing a section directive,
  ** merge attributes with any previous definition(s).
  */
  if (is_section && old_map) {
    attribute_map |= old_map;
    if (attribute_map != old_map)
      as_warn (_("changed section attributes for %s"), sec->name);
    if (old_lma && (old_lma != sec->lma))
      as_bad (_("section address 0x%lx conflicts with previous value 0x%lx"),
              sec->lma, old_lma);
  }

  /* if any attributes specified, validate them */
  if (attribute_map &&
      !pic30_is_valid_attributes(attribute_map, flag_debug))
    as_bad (_("invalid attribute combination for %s"), sec->name);

  /* validate special case attributes */
  if (PIC30_IS_NEAR_ATTR(sec) &&
      PIC30_IS_ABSOLUTE_ATTR(sec) &&
      (PIC30_IS_DATA_ATTR(sec) ||
       PIC30_IS_BSS_ATTR(sec) ||
       PIC30_IS_PERSIST_ATTR(sec)))
    {
      if (sec->vma > NEAR_DATA_LIMIT)
        as_bad (_("section address 0x%lx exceeds near data range"), sec->vma);
    }

  if (PIC30_IS_ABSOLUTE_ATTR(sec) && SECTION(.const))
    as_bad (_("absolute address can not be specified for section .const"));

  if (PIC30_IS_EEDATA_ATTR(sec) && !pic30_is_eedata_machine(global_PROCESSOR))
    report_target_attribute_err ("eedata");

  if (PIC30_IS_DMA_ATTR(sec) && !pic30_is_dma_machine(global_PROCESSOR))
    report_target_attribute_err ("dma");

  if (PIC30_IS_XMEMORY_ATTR(sec) && !pic30_is_dsp_machine(global_PROCESSOR))
    report_target_attribute_err ("X");

  if (PIC30_IS_YMEMORY_ATTR(sec) && !pic30_is_dsp_machine(global_PROCESSOR))
    report_target_attribute_err ("Y");

  if (PIC30_IS_ABSOLUTE_ATTR(sec) && PIC30_IS_PSV_ATTR(sec) )
    sec->vma = PSV_BASE + (sec->lma & PSV_MASK);

  if (access_entry && !(PIC30_IS_CODE_ATTR(sec) || PIC30_IS_AUXFLASH_ATTR(sec)))
    as_bad(_(access_err_msg1));

  if (flag_debug)
    printf ("<-- pic30_attribute::exit\n");

  } /* pic30_attribute */

/******************************************************************************/

#if defined(OBJ_ELF)
/*
** Output a signed LEB128 value.
*/

static int
pic30_output_sleb128 (p, value)
     char *p;
     offsetT value;
{
  register char *orig = p;
  register int more;

  do
  {
      unsigned byte = (value & 0x7f);

      /* Sadly, we cannot rely on typical arithmetic right shift behaviour.
	 Fortunately, we can structure things so that the extra work reduces
	 to a noop on systems that do things "properly".  */
      value = (value >> 7) | ~(-(offsetT)1 >> 7);

      more = !((((value == 0) && ((byte & 0x40) == 0))
		|| ((value == -1) && ((byte & 0x40) != 0))));
      if (more)
	byte |= 0x80;

      *p++ = byte;
      *p++ = (PIC30_PHANTOM_BYTE);
  }
  while (more);

  return p - orig;
}

/*
** Output an unsigned LEB128 value.
*/
static int
pic30_output_uleb128 (p, value)
     char *p;
     valueT value;
{
  char *orig = p;

  do
    {
      unsigned byte = (value & 0x7f);
      value >>= 7;
      if (value != 0)
	/* More bytes to follow.  */
	byte |= 0x80;

      *p++ = byte;
      *p++ = (PIC30_PHANTOM_BYTE);
    }
  while (value != 0);

  return p - orig;
}
/*
** Output an leb128 number
*/
int
pic30_output_leb128 (p, value, sign)
     char *p;
     valueT value;
     int sign;
{
  if (sign)
    return(pic30_output_sleb128 (p, (offsetT) value));
  else
    return(pic30_output_uleb128 (p, value));
}
/*
** Generate the appropriate fragments for a given expression to emit a
** leb128 value.
*/
static void
pic30_emit_leb128_expr (exp, sign)
     expressionS *exp;
     int sign;
{
  operatorT op = exp->X_op;
  int nbytes;

  if (op == O_absent || op == O_illegal)
    {
      as_warn (_("zero assumed for missing expression"));
      exp->X_add_number = 0;
      op = O_constant;
    }
  else if (op == O_big && exp->X_add_number <= 0)
    {
      as_bad (_("floating point number invalid"));
      exp->X_add_number = 0;
      op = O_constant;
    }
  else if (op == O_register)
    {
      as_warn (_("register value used as expression"));
      op = O_constant;
    }

  /* Let check_eh_frame know that data is being emitted.  nbytes == -1 is
     a signal that this is leb128 data.  It shouldn't optimize this away.  */
  nbytes = -1;
  if (check_eh_frame (exp, &nbytes))
    abort ();

  /* Let the backend know that subsequent data may be byte aligned.  */
#ifdef md_cons_align
  md_cons_align (1);
#endif

  /*
  ** Ensure that the current section is a data section.
  ** If not, issue an error.
  */
  if (((bfd_get_section_flags(stdoutput, now_seg) & SEC_CODE) == SEC_CODE) ||
      (now_seg->auxflash == 1))
  {
        as_bad(_(".%cleb128 can only be used in a data section."),
			sign ? 's' : 'u');
  }
  if (op == O_constant)
  {
      /*
      ** If we've got a constant, emit the thing directly right now.
      */

      valueT value = exp->X_add_number;
      int size;
      char *p;

      size = sizeof_leb128 (value, sign) * 2;
      p = frag_more (size);
      pic30_output_leb128 (p, value, sign);
  }
  else
  {
      /*
      ** Otherwise, complain.
      */
      as_bad (_("invalid argument to .%cleb128"), sign ? 's' : 'u');
  }
}
/******************************************************************************
**
**   Handles the .sleb128 and .uleb128 pseudo-ops.
**
**   ARGUMENT          DESCRIPTION
**   --------------------------------------------------------------------------
**   sign              Whether or not the number is signed.
**
*******************************************************************************/
void
pic30_s_leb128 (sign)
     int sign;
{
  expressionS exp;

  do
  {
      expression (&exp);
      pic30_emit_leb128_expr (&exp, sign);
  }
  while (*input_line_pointer++ == ',');

  input_line_pointer--;
  demand_empty_rest_of_line ();
}
#endif
/******************************************************************************
 *
 *   Handles the .ASCII, .ASCIZ, and .STRING pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   append_zero       Whether or not to append a trailing null terminator.
 *
 ******************************************************************************/
void pic30_stringer (append_zero)
   int append_zero;
{
   if (flag_debug)
   {
      printf ("--> pic30_stringer (%s)::begin\n",
              append_zero ? "TRUE" : "FALSE");
      printf ("    pic30_stringer::global_p_directive_active = %s\n",
              global_p_directive_active ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   if (global_p_directive_active)
   {
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
        {
         record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
         listing_prev_line();  /* attach padding (if any) to previous */
                               /*   line in the listing file          */
        }
   } /* if (global_p_directive_active) */

   /*
    * Reset whether the last directive was a p-directive.  These directives do
    * not support writing to the upper byte.
    */
   global_p_directive_active = FALSE;

   stringer (append_zero);

   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
      PIC30_PAD_PROGRAM_MEMORY_AFTER_INSERT();

   if (flag_debug)
      printf ("<-- pic30_stringer::exit\n");
} /* void pic30_stringer(int) */

/******************************************************************************
 *
 *   Handles the .PASCII, .PASCIZ, and .PSTRING pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   append_zero       Whether or not to append a trailing null terminator.
 *
 ******************************************************************************/
void pic30_pstringer (append_zero)
     int append_zero;
{
  if (flag_debug) {
    printf ("--> pic30_pstringer (%s)::begin\n",
            append_zero ? "TRUE" : "FALSE");
    printf ("    pic30_pstringer::global_p_directive_active = %s\n",
            global_p_directive_active ? "TRUE" : "FALSE");
  } /* if (flag_debug) */

  /*
   * Make sure that code is currently in a .text or code section.  If not,
   * warn the user and ignore the directive.
   */
  if (!subseg_text_p (now_seg) && (now_seg != text_section) &&
      !subseg_auxflash(now_seg))
    {
    as_warn (_(".pascii, .pasciz, or .pstring not specified in a code"
               " section. Directive ignored."));
    discard_rest_of_line ();
    return;
  }

  /*
   * Force alignment if the previous directive was not a p-type
   */
  if (!global_p_directive_active) {
    if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
      {
        record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
        listing_prev_line();  /* attach padding (if any) to previous */
        /*   line in the listing file          */
      }
  }

  global_p_directive_active = TRUE;

  stringer (append_zero);

  if ((subseg_text_p (now_seg)) || (now_seg == text_section))
    PIC30_PAD_PROGRAM_MEMORY_AFTER_INSERT();

  if (flag_debug)
    printf ("<-- pic30_pstringer::exit\n");
} /* void pic30_pstringer(int) */

/******************************************************************************/

void pic30_fixed_cons (ignore)
   int ignore __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .FIXED pseudo-op.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/

{
   if (is_it_end_of_statement ())
   {
      demand_empty_rest_of_line ();
      return;
   } /* if (is_it_end_of_statement ()) */

#ifdef md_flush_pending_output
   md_flush_pending_output ();
#endif

   /*
    * If previous directive was p-type, align to next PC address
    * since this is a non-p-type directive.
    */
   if (global_p_directive_active)
   {
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
         record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
   } /* if (global_p_directive_active) */

   /*
    * Reset whether the last directive was a p-directive.  This directive does
    * not support writing to the upper byte.
    */
   global_p_directive_active = FALSE;

   do
   {
      unsigned char c;
      char * p = NULL;
      double d;

      SKIP_WHITESPACE ();

      p = input_line_pointer;

      /* Check for leading sign */
      if ((*p == '+') || (*p == '-'))
         p++;

      /* Parse the whole number portion of the floating point. */
      for ( ; (((c = *p) != '\0') && (c != '.')); p++)
         if (!isdigit (c))
            break;

      /*
       * If there is a decimal point, skip it and read the fractional portion
       * of the floating point.
       */
      if (c && (c == '.'))
         for (p++; ((c = *p) && isdigit (c)); p++)
            /* just a loop */ ;

      /* Check to see if an exponent was specified. */
      if (c && strchr (EXP_CHARS, c))
      {
         c = *++p;

         /* Skip the optional sign for the exponent */
         if (c && (strchr ("+-", c)))
            c = *++p;

         /* Skip the digits in the exponent */
         for ( ; (c); c = *++p)
            if (!isdigit (c))
               break;
      } /* if */

      *p = '\0';

      d = atof (input_line_pointer);

      if ((d >= -1.0) && (d < 1.0))
      {
         short fixed;
         register char * p;
         char buf[PIC30_FIXED_SIZE];

#if 0
         fixed = (short) floor (d * 32768);
#else
         fixed = (short) floor (d * 32768 + 0.5);
#endif

         /* Put bytes in right order.  */
         md_number_to_chars (buf, (valueT) fixed, PIC30_FIXED_SIZE);

#ifdef TC_CONS_EMIT
         if (!TC_CONS_EMIT(buf, PIC30_FIXED_SIZE))
         {
#endif
            p = frag_more ((int) PIC30_FIXED_SIZE);
            memcpy(p, buf, PIC30_FIXED_SIZE);
#ifdef TC_CONS_EMIT
         } /* if (!TC_CONS_EMIT(buf, PIC30_FIXED_SIZE)) */
#endif

      } /* if ((d >= -1.0) && (d < 1.0)) */
      else
         as_bad (_(".fixed value out of range\n"));

      input_line_pointer = p;
      *input_line_pointer = c;
   } while (*input_line_pointer++ == ',');

   /* Put terminator back into stream.  */
   --input_line_pointer;
   demand_empty_rest_of_line ();
} /* void pic30_fixed_cons(int) */

/******************************************************************************/

void pic30_float_cons (float_type)
   int float_type;

/******************************************************************************
 *
 *   Handles the .DOUBLE, .FLOAT, and .SINGLE pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   float_type        The type of the floating point used for this directive.
 *
 ******************************************************************************/

{
   if (flag_debug)
   {
      printf ("--> pic30_float_cons ('%c')::begin\n", float_type);
      printf ("    pic30_float_cons::global_p_directive_active = %s\n",
              global_p_directive_active ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   if (global_p_directive_active)
   {
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
         record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
   } /* if (global_p_directive_active != cons_data.p_directive) */

   /*
    * Reset whether the last directive was a p-directive.  These directives do
    * not support writing to the upper byte.
    */
   global_p_directive_active = FALSE;

   /*
    * Check for valid prefix: 0e, 0f or 0d.
    * The prefix is optional, unless the hexadecimal
    * notation is used (eg, 0f:xxxxxxxx)
    */
   if (input_line_pointer[0] == '0'
       && isalpha ((unsigned char) input_line_pointer[1]))
     {
       int c = tolower((unsigned char) input_line_pointer[1]);
       if ((c == 'e') || (c == 'f') || (c == 'd'))
         {} /* OK */
       else
         as_bad (_("Prefix (0%c) is not valid"
                   " for a floating point constant."), (unsigned char) c);
     }

   float_cons (float_type);

   if (flag_debug)
      printf ("<-- pic30_float_cons::exit\n");
} /* void pic30_float_cons(int) */

/******************************************************************************/


static unsigned int
pic30_next_char_of_string ()

/******************************************************************************
 *
 * Copied from read.c::next_char_of_string.  We don't want to convert
 *  backquoted characters.
 ******************************************************************************/

{
  register unsigned int c;

  c = *input_line_pointer++ & CHAR_MASK;
  switch (c)
    {
    case '\"':
      c = NOT_A_CHAR;
      break;

    case '\n':
      as_warn (_("Unterminated string: Newline inserted."));
      bump_line_counters ();
      break;

    default: break;
    }
  return c;

}

 
static char * demand_copy_string (lenP)
     int *lenP;

/******************************************************************************
 *
 * Copied from read.c::demand_copy_string.  Convert back slash into forward
 *  slash for cygwin or mingw builds.
 *
 ******************************************************************************/

{
  register unsigned int c;
  register int len;
  char *retval;

  len = 0;
  SKIP_WHITESPACE ();
  if (*input_line_pointer == '\"')
    {
      input_line_pointer++;     /* Skip opening quote.  */

      while (is_a_char (c = pic30_next_char_of_string()))
        {
#if defined (__CYGWIN__) || defined (__MINGW32__)
          if (c == '\\') c = '/';
#endif
          obstack_1grow (&notes, c);
          len++;
        }
      /* JF this next line is so demand_copy_C_string will return a
         null terminated string.  */
      obstack_1grow (&notes, '\0');
      retval = obstack_finish (&notes);
    }
  else
    {
      as_warn (_("Missing string"));
      retval = NULL;
      ignore_rest_of_line ();
    }
  *lenP = len;
  return (retval);
}
/******************************************************************************
 * This function enters a .file entry in the symbol table.
 * The file name is converetd to full-path format before being entered
 * into the symbol table.
 ******************************************************************************/
static void
pic30_install_file_symbol(char *filename)
{
#if defined(__CYGWIN__)
extern void cygwin_conv_to_full_win32_path(const char *path, char *win32_path);
	char win32_path[MAXPATHLEN];

	cygwin_conv_to_full_win32_path(filename, win32_path);
	while ((filename = strchr(win32_path, '\\')) != NULL)
	{
		filename[0] = '/';
	}
	filename = win32_path;
#elif defined(__MINGW32__)
#if !defined(_MAXPATH)
#define	_MAXPATH	260
#endif
	char win32_path[_MAXPATH];

	if (_fullpath(win32_path, filename, sizeof(win32_path)) != NULL)
	{
		filename = win32_path;
		while ((filename = strchr(win32_path, '\\')) != NULL)
		{
			filename[0] = '/';
		}
		filename = win32_path;
	}
#endif
#ifdef OBJ_ELF
	elf_file_symbol(filename);
#else
	c_dot_file_symbol(filename);
#endif
}
/******************************************************************************
 * This function ensures that there is a .file entry in the symbol table.
 * This is required for the '-g' option, and also for dependency annotation.
 ******************************************************************************/
static void
pic30_install_mainfile_symbol(void)
{
	char *filename;
	unsigned lineno;

	as_where(&filename, &lineno);
	/*
	** Make sure that there is a .file symbol
	*/
#ifdef OBJ_ELF
	if ((symbol_rootP == NULL) ||
	    (symbol_get_bfdsym(symbol_rootP)->flags & BSF_FILE) != BSF_FILE)
	{
		pic30_install_file_symbol(filename);
	}
#else
	if ((symbol_rootP == NULL)
			|| S_GET_STORAGE_CLASS (symbol_rootP) != C_FILE)
	{
		pic30_install_file_symbol(filename);
	}
#endif
}

/******************************************************************************
 *
 *   Handles the .INCBIN pseudo-op.  Some of this code was copied
 *   from gas/read.c::s_incbin.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   p_directive       This argument determines whether the .pincbin or
 *                     .incbin directive was used.
 *
 *   Using the above copied utility functions, insured that backquoted chars
 *   remain unconverted and back slashes are converted into forward slashes.
 *   Perhaps we simply wish to define NO_STRING_ESCAPES to fix this?
 * 
 ******************************************************************************/

void pic30_incbin (p_directive)
   int p_directive;
{
  FILE * binfile;
  char * path;
  char * filename;
  char * bindat;
  long   skip = 0;
  long   count = 0;
  long   bytes;
  int    len;

  if (flag_debug)
    printf ("--> pic30_incbin (%s)::begin\n",
            p_directive ? "TRUE" : "FALSE");

#ifdef md_flush_pending_output
  md_flush_pending_output ();
#endif

  if ((p_directive) && (!subseg_text_p (now_seg) && (now_seg != text_section)))
    {
      as_warn (_(".pincbin not specified in a code section.  "
                 ".pincbin ignored."));
      discard_rest_of_line ();
      return;
    } /* if (p_directive) */

  if (flag_debug)
    printf ("    pic30_fill::global_p_directive_active != p_directive\n"
            "    pic30_fill::(%s != %s) => %s\n",
            global_p_directive_active ? "TRUE" : "FALSE",
            p_directive ? "TRUE" : "FALSE",
            (global_p_directive_active != p_directive) ? "TRUE" : "FALSE");

  if (global_p_directive_active != p_directive)
    {
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
        record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
    } /* if (global_p_directive_active != p_directive) */

  global_p_directive_active = p_directive;

  SKIP_WHITESPACE ();
  filename = demand_copy_string (& len);
  if (filename == NULL)
    return;

  SKIP_WHITESPACE ();

  /* Look for optional skip and count.  */
  if (* input_line_pointer == ',')
    {
      ++ input_line_pointer;
      skip = get_absolute_expression ();

      SKIP_WHITESPACE ();

      if (* input_line_pointer == ',')
	{
	  ++ input_line_pointer;

	  count = get_absolute_expression ();
	  if (count < 1) {
            as_warn (_(".incbin count %s, ignoring `%s'"),
                     count ? "negative" : "zero", filename);
            discard_rest_of_line ();
            return;
          }

	  SKIP_WHITESPACE ();
	}
    }

  demand_empty_rest_of_line ();

  /* Try opening absolute path first, then try include dirs.  */
  binfile = fopen (filename, FOPEN_RB);
  if (binfile == NULL)
    {
      int i;

      path = xmalloc ((unsigned long) len + include_dir_maxlen + 5);

      for (i = 0; i < include_dir_count; i++)
	{
	  sprintf (path, "%s/%s", include_dirs[i], filename);

	  binfile = fopen (path, FOPEN_RB);
	  if (binfile != NULL)
	    break;
	}

      if (binfile == NULL)
	as_bad (_("file not found: %s"), filename);
    }
  else
    path = xstrdup (filename);

  if (binfile)
    {
      long   file_len;

      register_dependency (path);

      /* Compute the length of the file.  */
      if (fseek (binfile, 0, SEEK_END) != 0)
	{
	  as_bad (_("seek to end of .incbin file failed `%s'"), path);
	  goto done;
	}
      file_len = ftell (binfile);

      /* If a count was not specified use the size of the file.  */
      if (count == 0)
	count = file_len;

      if (skip + count > file_len)
	{
	  as_bad (_("skip (%ld) + count (%ld) larger than file size (%ld)"),
		  skip, count, file_len);
	  goto done;
	}

      if (fseek (binfile, skip, SEEK_SET) != 0)
	{
	  as_bad (_("could not skip to %ld in file `%s'"), skip, path);
	  goto done;
	}

      /* Allocate a buffer and read the data  */
      bindat = xmalloc (count);

      if (bindat)
        {
          bytes = fread (bindat, 1, count, binfile);
          if (bytes < count)
            as_warn (_("truncated file `%s', %ld of %ld bytes read"),
                     path, bytes, count);

          /* Insert data into the current frag */
          (void) pic30_cons_emit(bindat, bytes);

          free(bindat);
        }
    }

  /*
  ** Dependency annotation in the object file.
  ** First ensure that there is a file entry for the main file,
  ** then add an annotation for the included file.
  */
  pic30_install_mainfile_symbol();
  pic30_install_file_symbol(path);

done:
  if (binfile != NULL)
    fclose (binfile);
  if (path)
    free (path);

   if (flag_debug)
      printf ("<-- pic30_incbin::exit\n");
} /* void pic30_incbin (int) */

/******************************************************************************
 *
 *   Handles the .INCLUDE pseudo-op.  Some of this code was copied
 *   from gas/read.c::s_include.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   arg           This argument is unused.
 *
 *   Using the above copied utility functions, insured that backquoted chars
 *   remain unconverted and back slashes are converted into forward slashes.
 *   Perhaps we simply wish to define NO_STRING_ESCAPES to fix this?
 * 
 ******************************************************************************/

void pic30_include (arg)
   int arg  __attribute__ ((__unused__));
{
  char *filename;
  int i;
  FILE *try;
  char *path;

#if 0
  if (!flag_m68k_mri)
#endif
    {
      filename = demand_copy_string (&i);
      if (filename == NULL)
        {
          /* demand_copy_string has already printed an error and
             called ignore_rest_of_line.  */
          return;
        }
    }
#if 0
  else
    {
      SKIP_WHITESPACE ();
      i = 0;
      while (!is_end_of_line[(unsigned char) *input_line_pointer]
             && *input_line_pointer != ' '
             && *input_line_pointer != '\t')
        {
          obstack_1grow (&notes, *input_line_pointer);
          ++input_line_pointer;
          ++i;
        }

      obstack_1grow (&notes, '\0');
      filename = obstack_finish (&notes);
      while (!is_end_of_line[(unsigned char) *input_line_pointer])
        ++input_line_pointer;
    }
#endif

  demand_empty_rest_of_line ();
  path = xmalloc ((unsigned long) i + include_dir_maxlen + 5 /* slop */ );

  for (i = 0; i < include_dir_count; i++)
    {
      strcpy (path, include_dirs[i]);
      strcat (path, "/");
      strcat (path, filename);
      if (0 != (try = fopen (path, "r")))
        {
          fclose (try);
          goto gotit;
        }
    }

  free (path);
  path = filename;
gotit:
  /*
  ** Dependency annotation in the object file.
  ** First ensure that there is a file entry for the main file,
  ** then add an annotation for the included file.
  */
  pic30_install_mainfile_symbol();
  pic30_install_file_symbol(path);
  /* malloc Storage leak when file is found on path.  FIXME-SOMEDAY.  */
  register_dependency (path);
  input_scrub_insert_file (path);
}


/******************************************************************************/

void pic30_fill (p_directive)
   int p_directive;

/******************************************************************************
 *
 *   Handles the .FILL and .PFILL pseudo-ops.  Some of this code was copied
 *   from gas/read.c::s_fill.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   p_directive       This argument determines whether the .pfill or .fill
 *                     directive was used.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_fill (%s)::begin\n",
              p_directive ? "TRUE" : "FALSE");

   if ((p_directive) && (!subseg_text_p (now_seg) && (now_seg != text_section)))
   {
      as_warn (_(".pfill not specified in a code section.  "
                 ".pfill ignored."));
      discard_rest_of_line ();
   } /* if (p_directive) */
   else
   {
      expressionS repeat_;
      expressionS size;
      expressionS fill;

      if (flag_debug)
         printf ("    pic30_fill::global_p_directive_active != p_directive\n"
                 "    pic30_fill::(%s != %s) => %s\n",
                 global_p_directive_active ? "TRUE" : "FALSE",
                 p_directive ? "TRUE" : "FALSE",
                 (global_p_directive_active != p_directive) ? "TRUE" : "FALSE");

      if (global_p_directive_active != p_directive)
      {
         if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
            record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
      } /* if (global_p_directive_active != p_directive) */

      global_p_directive_active = p_directive;

      /* Initialize Default Values */
      size.X_op = O_constant;
      size.X_add_number = 1;

      fill.X_op = O_constant;

      if ((subseg_text_p (now_seg)) || (now_seg == text_section))
      {
         fill.X_add_number =
            (p_directive ? global_pfillvalue_value : global_fillvalue_value);
      } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */
      else
         fill.X_add_number = DEFAULT_FILL_VALUE;

      /* Parse the line */
      expression (&repeat_);
      SKIP_WHITESPACE ();

      if (!is_it_end_of_statement ())
      {
         if (*input_line_pointer != ',')
            as_warn (_("Missing , assumed."));
         else
            input_line_pointer++;

         expression (&size);

         if (!is_it_end_of_statement ())
         {
            if (*input_line_pointer != ',')
               as_warn (_("Missing , assumed."));
            else
               input_line_pointer++;

            expression (&fill);
         } /* if (!is_it_end_of_statement ()) */
      } /* if (!is_it_end_of_statement ()) */

      /*
       * This is to be compatible with BSD 4.2 AS, not for any rational reason
       */
#define BSD_FILL_SIZE_CROCK_8 (8)
#define BSD_FILL_SIZE_CROCK_4 (4)

      /*
       * Check the parsed information to see if any errors or warnings must be
       * generated.
       */
      if (repeat_.X_op == O_big)
         as_bad (_("Integer constant is too large for repeat argument."));
      else if (repeat_.X_op != O_constant)
         as_bad (_("Expected constant expression for repeat argument."));
      else if (repeat_.X_add_number <= 0)
      {
         if (repeat_.X_add_number < 0)
            as_warn (_("Repeat argument < 0.  %s ignored"),
                     (p_directive ? ".pfill" : ".fill")); 

         size.X_add_number = 0;
      } /* else if (repeat_.X_add_number < 0) */
      else if (size.X_op == O_big)
         as_bad (_("Integer constant is too large for size argument."));
      else if (size.X_op != O_constant)
         as_bad (_("Expected constant expression for size argument."));
      else if (size.X_add_number < 0)
      {
         as_warn (_("Size argument < 0.  %s ignored"),
                  (p_directive ? ".pfill" : ".fill")); 
         size.X_add_number = 0;
      } /* else if (size.X_add_number < 0) */
      else if (size.X_add_number > BSD_FILL_SIZE_CROCK_8)
      {
         as_warn (_("%s size clamped to %d."),
                  (p_directive ? ".pfill" : ".fill"), BSD_FILL_SIZE_CROCK_8);
         size.X_add_number = BSD_FILL_SIZE_CROCK_8;
      } /* else if (size.X_add_number > BSD_FILL_SIZE_CROCK_8) */
      else if (fill.X_op == O_big)
         as_bad (_("Integer constant is too large for fill argument."));
      else if (fill.X_op != O_constant)
         as_bad (_("Expected constant expression for fill argument."));

      if (flag_debug)
      {
         printf ("    pic30_fill::repeat_ = %ld\n", repeat_.X_add_number);
         printf ("    pic30_fill::size = %ld\n", size.X_add_number);
         printf ("    pic30_fill::fill = 0x%02lx\n", fill.X_add_number);
      } /* if (flag_debug) */

      if ((size.X_add_number != 0) && (!need_pass_2))
      {
         /*
          * The magic number BSD_FILL_SIZE_CROCK_4 is from BSD 4.2 VAX
          * flavoured AS.  The following bizzare behaviour is to be
          * compatible with above.  I guess they tried to take up to 8
          * bytes from a 4-byte expression and they forgot to sign
          * extend.
          */
#define BSD_FILL_SIZE_CROCK_4 (4)

        int size_value =  ((size.X_add_number > BSD_FILL_SIZE_CROCK_4) ?
                           BSD_FILL_SIZE_CROCK_4 : size.X_add_number);
        char * buf = xmalloc (size.X_add_number);
        int i, j;

        memset (buf, 0, size.X_add_number);
        md_number_to_chars (buf, (valueT) fill.X_add_number, size_value);

        if ((subseg_text_p (now_seg)) || (now_seg == text_section))
          {
            for (i = 0; i < repeat_.X_add_number; i++)
              for (j = 0; j < size.X_add_number; j++)
                PIC30_INSERT_VALUE_INTO_PROGRAM_MEMORY(buf[j]);

            /*
             * Set the flag that says the location counter has been screwed
             * with.
             */
            global_must_align_location_counter = TRUE;
          }
        else
          {
            for (i = 0; i < repeat_.X_add_number; i++)
              for (j = 0; j < size.X_add_number; j++)
                PIC30_INSERT_VALUE_INTO_DATA_MEMORY(buf[j]);
          }

        free (buf);
      } /* if ((size.X_add_number != 0) && (!need_pass_2)) */

      demand_empty_rest_of_line ();
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_fill::exit\n");
} /* void pic30_fill(int) */

/******************************************************************************/

void pic30_space (p_directive)
   int p_directive;

/******************************************************************************
 *
 *   Handles the .SKIP/.SPACE and .PSKIP/.PSPACE pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   p_directive       This argument determines whether the .pskip/.pspace or
 *                     .skip/.space directive was used.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_space (%s)::begin\n",
              p_directive ? "TRUE" : "FALSE");

   if ((p_directive) && (!subseg_text_p (now_seg) && (now_seg != text_section)))
   {
      as_warn (_("Directive not specified in a code section.  "
                 "Directive ignored."));
      discard_rest_of_line ();
   } /* if (p_directive) */
   else
   {
      expressionS size;
      expressionS fill;

      if (flag_debug)
         printf ("    pic30_space::global_p_directive_active != p_directive\n"
                 "    pic30_space::(%s != %s) => %s\n",
                 global_p_directive_active ? "TRUE" : "FALSE",
                 p_directive ? "TRUE" : "FALSE",
                 (global_p_directive_active != p_directive) ? "TRUE" : "FALSE");

      if (global_p_directive_active != p_directive)
      {
         if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
            record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
      } /* if (global_p_directive_active != p_directive) */

      global_p_directive_active = p_directive;

      expression (&size);
      SKIP_WHITESPACE ();

      if (!is_it_end_of_statement ())
      {
         if (*input_line_pointer != ',')
            as_warn (_("Missing , assumed."));
         else
            input_line_pointer++;

         expression (&fill);
      } /* if (!is_it_end_of_statement ()) */
      else
      {
         fill.X_op = O_constant;

         if ((subseg_text_p (now_seg)) || (now_seg == text_section))
            fill.X_add_number = p_directive ? global_pfillvalue_value
                                            : global_fillvalue_value;
         else
            fill.X_add_number = DEFAULT_FILL_VALUE;
      } /* else */

      if (size.X_op == O_big)
         as_bad (_("Integer constant is too large for size argument."));
      else if ((size.X_op != O_constant) || (size.X_add_number < 0))
         as_bad (_("Expected positive constant expression for size argument."));
      else if (fill.X_op == O_big)
         as_bad (_("Integer constant is too large for fill argument."));
      else if (fill.X_op != O_constant)
         as_bad (_("Expected constant expression for fill argument."));
      else if ((fill.X_add_number < -0x80) || (fill.X_add_number > 0xFF))
      {
         register valueT get;
         register valueT use;
         register valueT mask;
         valueT hibit;
         register valueT unmask;

         /* Don't store these bits. */
         mask = ~(valueT) 0 << (BITS_PER_CHAR * 1);
         hibit = (valueT) 1 << (1 * BITS_PER_CHAR - 1);

         unmask = ~mask;           /* Do store these bits. */

         get = fill.X_add_number;
         use = get & unmask;
         if ((get & mask) != 0 && ((get & mask) != mask || (get & hibit) == 0))
         {
            /* Leading bits contain both 0s & 1s. */
            as_warn (_("Value 0x%lx truncated to 0x%lx."),
                     (unsigned long) get, (unsigned long) use);

            fill.X_add_number = use;
         } /* if */
      } /* else if ((fill.X_add_number < -0x80 || > 0xFF)) */

      if (flag_debug)
      {
         printf ("    pic30_space::size = %ld\n", size.X_add_number);
         printf ("    pic30_space::fill = 0x%02lx\n", fill.X_add_number);
      } /* if (flag_debug) */

      {
         int i;

         for (i = 0; i < size.X_add_number; i++)
            emit_expr (&fill, 1);
      } /* else */

      /*
       * Set the flag that says the location counter has been screwed with.
       */
      if ((subseg_text_p (now_seg)) || (now_seg == text_section))
         global_must_align_location_counter = TRUE;
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_space::exit\n");
} /* void pic30_space(int) */

/******************************************************************************/

unsigned char
pic30_do_org (segment, expr, fill, fill_specified)
   register segT segment;
   PTR expr; /* (expressionS *) */
   long fill;
   unsigned char fill_specified;

/******************************************************************************
 *
 *   This function is called by the do_org function in gas/read.c.  It will
 *   perform the .org if the .org is located in a CODE section.  This is
 *   necessary so that the proper fill pattern can be utilized in a CODE
 *   section based on whether a fill was specified in the .org, the last
 *   specified .fillvalue, .fillupper, and .pfillvalue.
 *
 *   Note that the user has specified a PC offset, which must be converted
 *   into a byte offset.
 *
 *   Supports simple dot expressions such as .=.+2. If the current fill
 *   point is not PC-aligned due to previous .byte or .pbyte directive(s),
 *   dot is considered to mean the current (or last valid) PC address.
 *
 *   RETURNS:
 *      TRUE  - If the .org was handled by this function
 *      FALSE - If the .org was not handled by this function.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;
   unsigned char dot_relative = FALSE;
   expressionS *where = (expressionS *) expr;
   char fill_upper = global_p_directive_active ? global_pfillvalue_value
                                               : global_fillupper_value;

   if (flag_debug)
   {
      printf ("--> pic30_do_org (..., %ld, 0x%02lx, %s)::begin\n",
              where->X_add_number, fill, fill_specified ? "TRUE" : "FALSE");
      printf ("    pic30_do_org::global_p_directive_active = %s\n",
              global_p_directive_active ? "TRUE" : "FALSE");
      printf ("    pic30_do_org::fill_upper = 0x%02x\n", fill_upper);
      printf ("    pic30_do_org::SEC_CODE = %s\n",
              ((subseg_text_p (now_seg)) ||
               (now_seg == text_section)) ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
   {
     long start        /* # of octets left in current program word */
       = global_current_location % PIC30_SIZE_OF_PROGRAM_WORD;
     long  skip_length  /* # of octets to advance global_current_location */
       = (where->X_add_number*2) - start;
     long new_lc;      /* new location counter, in octets */

      if (where->X_op != O_constant)
        {
          if ((where->X_op == O_symbol) && 
              (strcmp (FAKE_LABEL_NAME, S_GET_NAME (where->X_add_symbol)) == 0))
            /* this is a simple DOT expression, we can handle it */
            dot_relative = TRUE;
          else
            as_bad (_("Expected constant expression for new-lc argument."));
        }
        /* make sure that new-lc is even */
        if (where->X_add_number % 2)
            as_bad (_("Odd values are not permitted for new location counter."));

        new_lc = (dot_relative ? global_current_location : 0) + skip_length;

        if (fill_specified)
          {
            if ((fill < -0x80) || (fill > 0xFF))
              {
                register valueT get;
                register valueT use;
                register valueT mask;
                valueT hibit;
                register valueT unmask;

                /* Don't store these bits. */
                mask = ~(valueT) 0 << (BITS_PER_CHAR * 1);
                hibit = (valueT) 1 << (1 * BITS_PER_CHAR - 1);

                unmask = ~mask;           /* Do store these bits. */

                get = fill;
                use = get & unmask;
                if (((get & mask) != 0) &&
                    (((get & mask) != mask) || ((get & hibit) == 0)))
                  {
                    /* Leading bits contain both 0s & 1s. */
                    as_warn (_("Value 0x%lx truncated to 0x%lx."),
                             (unsigned long) get, (unsigned long) use);

                    fill = use;
                  } /* if */
              } /* if (fill < -0x80 || fill > 0xFF) */

            if (global_p_directive_active)
              fill_upper = fill;  /* .porg fills all three bytes */

          } /* if (fill_specified) */
        else
          {
            fill = global_p_directive_active ? global_pfillvalue_value
              : global_fillvalue_value;
          } /* else */

        if ((segment != now_seg) && (segment != absolute_section))
          as_bad (_("invalid segment \"%s\"; segment \"%s\" assumed"),
                  segment_name (segment), segment_name (now_seg));

        if (now_seg == absolute_section)
          {
            if (fill != 0)
              as_warn (_("Ignoring fill value in absolute section"));

            abs_section_offset = where->X_add_number * 2;
          } /* if (now_seg == absolute_section) */
        else
          {
            int i, j;
            char *p;
            unsigned char fill_pattern[PIC30_SIZE_OF_PROGRAM_WORD];

            /*
             * The current fill pattern is based on what will be placed into
             * each byte of the program word.  The pattern should be:
             * byte 0 - fill
             * byte 1 - fill
             * byte 2 - fill_upper
             * byte 3 - PIC30_PHANTOM_BYTE
             *
             * NOTE:  This is TRUE for little endian.  If we were using big
             *        endian, the bytes would be reversed.
             */
            fill_pattern[0] = fill;
            fill_pattern[1] = fill;
            fill_pattern[2] = fill_upper;
            fill_pattern[3] = PIC30_PHANTOM_BYTE;

            if (flag_debug)
              {
                int i;

                printf ("    pic30_do_org::global_current_location = %ld\n",
                        global_current_location);

                printf ("    pic30_do_org::fill_pattern =");
                for (i = 0; i < PIC30_SIZE_OF_PROGRAM_WORD; i++)
                  printf (" 0x%02x", fill_pattern[i]);

                /* Skip 28 spaces for "pic30_do_org::fill_pattern =" */
                //printf ("\n%28c", ' ');
                printf ("\n    pic30_do_org::fill_pattern  ");

                /*
                 * Skip spaces until directly under the space for the starting
                 * byte of the fill pattern
                 */
                for (i = 0; i < start; i++)
                  printf ("%5c", ' ');

                printf ("^start\n");
              } /* if (flag_debug) */

            p = frag_var (rs_org, new_lc, 0,
                          (relax_substateT) 0, (symbolS *) NULL,
                          new_lc, (char *) NULL);

            /*
             * Copy the bytes necessary to fill the remaining portion of the
             * current program word.
             */
            memcpy(p, fill_pattern + start, PIC30_SIZE_OF_PROGRAM_WORD - start);

            /*
             * Copy the remaining bytes to fill up to where->X_add_number*2.
             */
            for (i = PIC30_SIZE_OF_PROGRAM_WORD - start, j = 0;
                 i < where->X_add_number*2;
                 i++, j++)
              p[i] = fill_pattern[j % PIC30_SIZE_OF_PROGRAM_WORD];
          } /* else */

        global_current_location = new_lc;
        global_must_align_location_counter = TRUE;
      
        rc = TRUE;
   } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */

   if (flag_debug)
      printf ("<-- pic30_do_org::exit\n");

   return rc;
} /* unsigned char pic30_do_org(segT, expressionS *, long, unsigned char) */

/******************************************************************************/

void
pic30_cons (directive)
   int directive;

/******************************************************************************
 *
 *   This function is called for all data allocation directives.  It ensures
 *   that the p-variant data allocation directives are only allowed in a CODE
 *   section.  In addition, if the user is switching between a p-variant and
 *   normal data allocation directive (or vice versa), the allocation starts
 *   at the next CODE alignment (thus filling in the skipped space with the
 *   proper fill).
 *
 *   Flag an error if data is specified for a BSS or PERSIST section.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   directive         This argument determines which data allocation directive
 *                     caused this function to be called and corresponds to the
 *                     appropriate structure in the pic30_cons_table array.
 *
 ******************************************************************************/

{
   const pic30_consS cons_data = pic30_cons_table[directive];
   const char *msg1 = "Initial values are not supported in";

   if (flag_debug)
   {
      printf ("--> pic30_cons (%s)::begin\n", cons_data.name);
      printf ("    pic30_cons::p_directive = %s\n",
              cons_data.p_directive ? "TRUE" : "FALSE");
      printf ("    pic30_cons::number_of_bytes = %d\n",
              cons_data.number_of_bytes);
   } /* if (flag_debug) */

   if (PIC30_IS_BSS_ATTR(now_seg))
     as_warn(_("%s bss section \'%s\'."), msg1, now_seg->name);
   else if (PIC30_IS_PERSIST_ATTR(now_seg))
     as_warn(_("%s persist section \'%s\'."), msg1, now_seg->name);

   if (cons_data.p_directive)
   {
      /*
       * Make sure that code is currently in a .text or code section.  If not,
       * warn the user and ignore the directive.
       */
      if (!subseg_text_p (now_seg) && (now_seg != text_section))
      {
         as_warn (_("%s not specified in a code section.  "
                    "%s ignored."), cons_data.name, cons_data.name);
         discard_rest_of_line ();

         return;
      } /* if (!subseg_text_p (now_seg) && (now_seg != text_section)) */
   } /* if (cons_data.p_directive) */

   if (flag_debug)
      printf ("    pic30_cons::global_p_directive_active != "
              "cons_data.p_directive\n    pic30_cons::(%s != %s) => %s\n",
              global_p_directive_active ? "TRUE" : "FALSE",
              cons_data.p_directive ? "TRUE" : "FALSE",
              (global_p_directive_active != cons_data.p_directive) ? "TRUE"
                                                                   : "FALSE");

   if (global_p_directive_active != cons_data.p_directive)
   {
      if (pic30_do_align (PIC30_CODE_ALIGNMENT, 0, 0, 0))
         record_alignment (now_seg, PIC30_CODE_ALIGNMENT);
   } /* if (global_p_directive_active != cons_data.p_directive) */

   global_p_directive_active = cons_data.p_directive;
   global_long_directive_active = (directive == PIC30_CONS_LONG);
   /* DEBUG */
#if 1
     cons (cons_data.number_of_bytes);
#else
   /* if in a CODE section, do the regular thing */
   if (subseg_text_p (now_seg) || (now_seg == text_section))
     cons (cons_data.number_of_bytes);
   else
     /* insert extra padding for data memory */
     cons (cons_data.number_of_bytes * 2);
#endif


   if (flag_debug)
      printf ("<-- pic30_cons::exit\n");
} /* void pic30_cons(int) */

/******************************************************************************/

void
pic30_org (p_directive)
   int p_directive;

/******************************************************************************
 *
 *   Handles the .ORG and .PORG pseudo-ops.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   p_directive       This argument determines whether the .porg or .org
 *                     directive was used.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_org (%s)::begin\n", p_directive ? "TRUE" : "FALSE");

   if ((p_directive) && (!subseg_text_p (now_seg) && (now_seg != text_section)))
   {
      as_warn (_(".porg not specified in a code section.  "
                 ".porg ignored."));
      discard_rest_of_line ();
   } /* if (p_directive) */
   else
   {
      global_p_directive_active = p_directive;
      s_org (p_directive);
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_org::exit\n");
} /* void pic30_org(int) */

/******************************************************************************/

void
pic30_error(ignore)
   int ignore __attribute__((__unused__));

/******************************************************************************
 *
 *   Handles the .error pseudo-op.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            unused
 *
 ******************************************************************************/

{
   char * error_text;
   int i;

   error_text = demand_copy_string(&i);
   if (error_text == NULL)
   {
     /* demand_copy_string has already printed an error and
       called ignore_rest_of_line.  */
     as_bad(_(".error directive encountered"));
     return;
   } /* if (error_text == NULL) */

   demand_empty_rest_of_line();
   as_bad(_("%s"), error_text);
} /* void pic30_error(int) */

/******************************************************************************/

void
pic30_update_section_info (void)

/******************************************************************************
 *
 *   This function is called everytime the section is changed to save the
 *   current section information (i.e., location counter and fill values).
 *
 ******************************************************************************/

{
   const char * section_name = now_seg->name;
   segment_info_type * info = seg_info (now_seg);

   if (flag_debug)
   {
      printf ("--> pic30_update_section_info::begin\n");
      printf ("    pic30_update_section_info::");
      if (!info)
         printf ("[%s] not found.\n", section_name);
      else
      {
         printf ("[%s] found.\n", section_name);
         printf ("    pic30_update_section_info::old.current_location = %lu\n"
                 "    pic30_update_section_info::old.fillvalue        = 0x%x\n"
                 "    pic30_update_section_info::old.fillupper        = 0x%x\n"
                 "    pic30_update_section_info::old.pfillvalue       = 0x%x\n"
                 "    pic30_update_section_info::old.p_directive_active = 0x%x\n",
                 info->tc_segment_info_data.current_location,
                 info->tc_segment_info_data.fillvalue_value,
                 info->tc_segment_info_data.fillupper_value,
                 info->tc_segment_info_data.pfillvalue_value,
                 info->tc_segment_info_data.p_directive_active);
      } /* else */
   } /* if (flag_debug) */

   if (!info)
      as_fatal (_("Could not get section information."));
   else
   {
      info->tc_segment_info_data.current_location = global_current_location;
      info->tc_segment_info_data.fillvalue_value = global_fillvalue_value;
      info->tc_segment_info_data.fillupper_value = global_fillupper_value;
      info->tc_segment_info_data.pfillvalue_value = global_pfillvalue_value;
      info->tc_segment_info_data.p_directive_active = global_p_directive_active;

      if (flag_debug)
      {
         printf ("    pic30_update_section_info::new.current_location = %lu\n"
                 "    pic30_update_section_info::new.fillvalue        = 0x%x\n"
                 "    pic30_update_section_info::new.fillupper        = 0x%x\n"
                 "    pic30_update_section_info::new.pfillvalue       = 0x%x\n"
                 "    pic30_update_section_info::old.p_directive_active = 0x%x\n",
                 info->tc_segment_info_data.current_location,
                 info->tc_segment_info_data.fillvalue_value,
                 info->tc_segment_info_data.fillupper_value,
                 info->tc_segment_info_data.pfillvalue_value,
                 info->tc_segment_info_data.p_directive_active);
      } /* if (flag_debug) */
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_update_section_info::exit\n");
} /* void pic30_update_section_info(void) */

/******************************************************************************/

void
pic30_set_global_info (void)

/******************************************************************************
 *
 *   This function is called everytime the section is changed to restore any
 *   previous section information saved (i.e., location counter and fill
 *   values).
 *
 ******************************************************************************/

{
   const char * section_name = now_seg->name;
   segment_info_type * info = seg_info (now_seg);

   if (flag_debug)
   {
      printf ("--> pic30_set_global_info::begin\n");
      printf ("    pic30_set_global_info::[%s]%sfound.\n",
              section_name, (info ? " " : " not "));
   } /* if (flag_debug) */

   if (info)
   {
      global_current_location = info->tc_segment_info_data.current_location;
      global_fillvalue_value = info->tc_segment_info_data.fillvalue_value;
      global_fillupper_value = info->tc_segment_info_data.fillupper_value;
      global_pfillvalue_value = info->tc_segment_info_data.pfillvalue_value;
      global_p_directive_active = info->tc_segment_info_data.p_directive_active;
   } /* if (section_info) */
   else
   {
      global_current_location = 0;

      PIC30_RESET_GLOBAL_FILL_VALUES ();

      global_p_directive_active = FALSE;
   } /* else */

   if (flag_debug)
   {
      printf ("    pic30_set_global_info::global_current_location = %lu\n"
              "    pic30_set_global_info::global_fillvalue_value = 0x%x\n"
              "    pic30_set_global_info::global_fillupper_value = 0x%x\n"
              "    pic30_set_global_info::global_pfillvalue_value = 0x%x\n"
              "    pic30_set_global_info::global_p_directive_active = %s\n",
              global_current_location, global_fillvalue_value,
              global_fillupper_value, global_pfillvalue_value,
              global_p_directive_active ? "TRUE" : "FALSE");
      printf ("<-- pic30_set_global_info::exit\n");
   } /* if (flag_debug) */
} /* void pic30_set_global_info(void) */

/******************************************************************************/

/******************************************************************************
 *
 *   Handles the .BSS pseudo-op.
 *
 *   There are two forms of this directive:
 *
 *   Zero-argument form -- If the current section is a code section,
 *   perform the usual alignment. Then save the current section info
 *   and reset globals for the new section.
 *
 *   3-argument form -- Reserve some space in .bss without changing the
 *   current section. This is handled by a generic function in gas/read.c,
 *   which has been modified to account for phantom bytes in data memory
 *   and to make the 3rd argument optional.
 *
 *   Syntax of the 3-argument form is: .bss <name>, <size> [, <alignment>]
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/
void
pic30_bss (ignore)
   int ignore __attribute__ ((__unused__));
{
   if (flag_debug)
      printf ("--> pic30_bss::begin\n");

   if (*input_line_pointer == '\n')
     {
       /*
        * This is the zero-argument form.
        * It changes the current section only.
        *
        * Call pic30_do_align to align the current section.
        * This only affects a .text or code section.
        */
       if ((subseg_text_p (now_seg)) || (now_seg == text_section))
         {
           PIC30_DO_CODE_ALIGNMENT ();
           listing_prev_line();  /* attach padding (if any) to previous */
         }                       /*   line in the listing file          */

       /* Store the global values for the current section */
       pic30_update_section_info ();

       /*
        * Reset whether the last directive was a p-directive.
        * No longer valid in the new section.
        */
       global_p_directive_active = FALSE;

       /* Change section */
       subseg_new (".bss", 0);

        /* Reset global values for the new section */
       pic30_set_global_info ();
     }
   else
     {
       /*
        * This is the 3-argument form.
        * It does not change the current section.
        *
        * Call the generic function to reserve space in .bss.
        */
       s_lcomm_bytes (1);
     }

   if (flag_debug)
      printf ("<-- pic30_bss::exit\n");
} /* void pic30_bss(int) */

/******************************************************************************/

void
pic30_data (elf_arg)
   int elf_arg __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .DATA pseudo-op.
 *
 *   This code is identical to that found in the obj-coff.c file for the .data
 *   directive.  The only addition is the pic30_do_align which will fill in the
 *   remaining bytes of a .text or code section so that the .text or code
 *   section is aligned.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   elf_arg           This argument is only used by the elf-specific code.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_data::begin\n");

   /*
    * Call pic30_do_align to align the current section.  This only affects a
    * .text or code section.
    */
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
     {
       PIC30_DO_CODE_ALIGNMENT ();
       listing_prev_line();  /* attach padding (if any) to previous */
     }                       /*   line in the listing file          */

   /* Store the global values for the current section */
   pic30_update_section_info ();

   /*
    * Reset whether the last directive was a p-directive.  No longer valid in
    * the new section.
    */
   global_p_directive_active = FALSE;

#ifdef OBJ_ELF
   obj_elf_data (elf_arg);
#else
   /*
    * This code was copied directly from obj-coff.c::obj_coff_data.  Could not
    * call this function directly as it declared statically.
    */
   if (flag_readonly_data_in_text)
      subseg_new (".text", 0);
   else
      subseg_new (".data", 0);

   demand_empty_rest_of_line ();  /* no subseg number allowed */
#endif

   /* Reset the global values for the new section */
   pic30_set_global_info ();

   if (flag_debug)
      printf ("<-- pic30_data::exit\n");
} /* void pic30_data(int) */

/******************************************************************************/

/******************************************************************************
 *
 *   Handles the .MEMORY pseudo-op.
 *
 *   Process an external memory space definition.
 *
 *   .memory name, size(nn) [,origin(oo)]
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   unused              N/A
 *
 ******************************************************************************/

#define REGION_MAX 0x10000
void
pic30_memory (unused)
   int unused ATTRIBUTE_UNUSED;
{
  char *region_name, *info_sec_name, *name;
  char *info_sec_prefix = "__memory_";
  bfd_vma region_size = 0;
  bfd_vma region_origin = 0;
  asection *sec, *old_sec;
  symbolS *symbolp;
  char c;

  if (flag_debug)
    printf ("--> pic30_memory::begin\n");
  
  /* get the memory region name */
  name = input_line_pointer;
  c = get_symbol_end ();

  /* make a local copy */
  if (strlen(name) == 0) {
    as_bad (_("invalid memory region name (none)\n"));
    region_name = "(none)";
    *input_line_pointer = c;
  } else {
    region_name = xmalloc (input_line_pointer - name + 1);
    strcpy (region_name, name);
    *input_line_pointer = c;
  }

  if (flag_debug)
    printf ("--> pic30_memory::name = %s\n", region_name);

  /* read attributes */
  while (!is_it_end_of_statement ()) {
    offsetT arg = 0;
    int has_arg = 0;
    char *name;
    char c,s[MAX_ATTR_LEN];
    unsigned int i,len;

    SKIP_COMMA ();
    SKIP_WHITESPACE ();

    name = input_line_pointer;
    c = get_symbol_end ();

    /* make a local copy, lower case */
    name = strncpy(s, name, sizeof(s));
    len = strlen(name);
    for(i = 0; i < len; i++)
      s[i] = tolower(s[i]);

    /* get the argument, if any */
    *input_line_pointer = c;
    if (c == '(') {
      has_arg++;
      input_line_pointer++;
      SKIP_WHITESPACE ();

      arg = get_absolute_expression ();    /* try to evaluate the argument */
      input_line_pointer++;                      /* skip the closing paren */
    }

    if (flag_debug) {
      printf ("    pic30_memory::attribute = %s\n", name);
      if (has_arg)
        printf ("    pic30_memory::argument  = 0x%lx\n", arg);
    }
      
    if (strcmp(name, "size") == 0)
      region_size = arg;
    else if (strcmp(name, "origin") == 0)
      region_origin = arg;
    else {
      as_bad (_("Unrecognized memory attribute \'%s\'"), name);
      demand_empty_rest_of_line ();
      break;
    }
  }
  
  if ((region_size == 0) || (region_size > REGION_MAX)) {
    as_bad (_("Size of memory region \'%s\' must be in range 2..%ld"
              " (was %ld)"),
               region_name, (unsigned long) REGION_MAX, region_size);
  }

  else if (region_size & 0x1) {
    as_bad (_("Size of memory region \'%s\' must be even (was %ld)"),
               region_name, (unsigned long) region_size);
  }

  else if (region_origin & 0x1) {
    as_bad (_("Origin of memory region \'%s\' must be even (was %ld)"),
               region_name, (unsigned long) region_origin);
  }

  else if (region_origin > (REGION_MAX - 2)) {
    as_bad (_("Origin of memory region \'%s\' must be in range 0..%ld"
              " (was %ld)"),
               region_name, (unsigned long) (REGION_MAX - 2), region_origin);
  }
  
  else if ((region_origin + region_size) > REGION_MAX) {
    as_bad (_("Memory region \'%s\' exceeds the 64K addressing limit\n"),
               region_name);
  }
  
  info_sec_name = xmalloc (strlen(region_name) + strlen(info_sec_prefix) + 1);
  (void) sprintf(info_sec_name, "%s%s", info_sec_prefix, region_name);
  
  /*
   * Check for multiple definitions of this memory region.
   *
   * References to it (by sections defined earlier) will
   * create the section, but will not define its size.
   */
  sec = bfd_get_section_by_name(stdoutput, info_sec_name);
  if (sec && sec->lma)
    as_bad (_("Multiple definition of memory region \'%s\'\n"),
               region_name);

  old_sec = now_seg;
  sec = subseg_new (info_sec_name, 0);
  PIC30_SET_INFO_ATTR(sec);
  sec->vma = region_origin;
  sec->lma = region_size;
  sec->_cooked_size = region_size;

  (void) subseg_new (old_sec->name, 0);
  
  /*
   * The memory region info (origin, length) is encoded as follows:
   *
   *  internal (BFD) section: (vma, lma)
   *  COFF object file:       (vma, lma)
   *  ELF object file:        (sh_addr, sh_info)
   */
}

/******************************************************************************
 *
 *   Handles the .SECTION pseudo-op.
 *
 *   Call the pic30_do_align function which will fill in the remaining bytes of
 *   a .text or code section so that the .text or code section is aligned.  Then
 *   call the correct object file's function to deal with the .section
 *   pseudo-op.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   push              Directive was .pushsection
 *
 ******************************************************************************/

void
pic30_section (push)
   int push;
{
  bfd_boolean has_flags = FALSE;
  char *save;
  int result;

  if (flag_debug) {
      printf ("--> pic30_section::begin\n");
      printf ("    pic30_section::push = %s\n", push ? "TRUE" : "FALSE");
  }

   /*
    * Call pic30_do_align to align the current section.  This only affects a
    * .text or code section, and only if we're not pushing it.
    */
   if ((push == FALSE) &&
       ((subseg_text_p (now_seg)) || (now_seg == text_section)))
     {
       PIC30_DO_CODE_ALIGNMENT ();
       listing_prev_line();  /* attach padding (if any) to previous */
     }                       /*   line in the listing file          */

   /* Store the global values for the current section */
   pic30_update_section_info ();

   /*
    * Reset whether the last directive was a p-directive.  No longer valid in
    * the new section.
    */
   global_p_directive_active = FALSE;

   if (push)  /* support .pushsection */
     {
       struct section_stack *elt;
       elt = xmalloc (sizeof (struct section_stack));
       elt->next = section_stack;
       elt->seg = now_seg;
       elt->prev_seg = previous_section;
       section_stack = elt;
     }
   previous_section = now_seg;

   /*
    * Look ahead to see if section flags are present
    */
   save = input_line_pointer;
   while (!is_end_of_line[(unsigned char) *input_line_pointer]) {
     if (*input_line_pointer++ == ',') {
       SKIP_WHITESPACE();
       if (*input_line_pointer == '\"') {
         has_flags = TRUE;
         break;
       }
     }
   }
   input_line_pointer = save;

   if (has_flags) {
     /* old style section directive */
     as_warn (_("Quoted section flags are deprecated, use attributes instead"));
#ifdef OBJ_ELF
   obj_elf_section (0);
#else
   obj_coff_section (0);
#endif
   /* set implied attributes, if any */
   result = pic30_set_implied_attributes(now_seg, flag_debug);
   if (result == 1)
     as_warn (_("Implied attributes for section \'%s\'"
                " are deprecated"), now_seg->name);
   else if (result == -1)
     as_bad (_("Attributes for section \'%s\'"
               " conflict with implied attributes"), now_seg->name);

   /* perform other validation on legacy section flags */
#if !defined(OBJ_ELF)
   {
     flagword flags = bfd_get_section_flags (stdoutput, now_seg);
     if ((flags & (SEC_CODE | SEC_READONLY)) == (SEC_CODE | SEC_READONLY) ||
	 ((flags & SEC_READONLY) && (now_seg->auxflash == 1)))
       as_bad (_("'x' and 'r' section attributes are not compatible\n"));
   }
#endif

   } else {
     /* new style section directive */

     char *section_name;
     int has_auto_name = 0;
     char c;
     char *name;
     unsigned int exp = 0; /* subsection number */
     asection *sec;

     /* get the section name */
     section_name = input_line_pointer;
     c = get_symbol_end ();

     /*
     ** Handle *, the auto-generated section name
     **
     ** Two forms are accepted:
     **   .section *,type
     **   .section *name,type
     **
     ** Both forms create an unordered section,
     **  which is not padded to match its alignment.
     **
     ** Interestingly, some earlier assembler
     **  pass removed any spaces that may have
     **  followed *, but not elsewhere
     **  in the input line. So we can't detect
     **  a missing comma after *. Huh...
     */
     if ((c == '*') && (strlen(section_name) == 0))
       {
         static unsigned int auto_name_cnt = 0;
         const char *s = ".scn";
         unsigned int lineno,len;
         char *filename;
         char *p,cc;
         
         *input_line_pointer++ = c;
         has_auto_name = 1;

         p = input_line_pointer;
         cc = get_symbol_end ();
         if (strlen(p) > 0) {  /* if a name is provided, use it */
           name = xmalloc (input_line_pointer - p + 1);
           strcpy (name, p);
         } else {              /* else invent a name */
           as_where( &filename, &lineno);
           len = strlen(filename) + strlen(s) + 5;
           name = xmalloc (len + 1);
           snprintf(name, len, "%s%s%d", filename, s, ++auto_name_cnt);
         }
         *input_line_pointer = cc;
       }
     else if (strlen(section_name) == 0) {
       as_bad (_("invalid section name (none)\n"));
       name = "(none)";
       *input_line_pointer = c;
     }
     else
       {
         name = xmalloc (input_line_pointer - section_name + 1);
         strcpy (name, section_name);
         *input_line_pointer = c;
       }

     /* create and/or switch to the new section */
     sec = subseg_new (name, (subsegT) exp);

     /* process the attribute list, if any */
     pic30_attribute (1);

     if (flag_debug)
       printf ("    pic30_section::new_attribute_map = 0x%x\n",
               pic30_attribute_map (sec));

     /* set implied attributes, if any */
     result = pic30_set_implied_attributes(sec, flag_debug);
     if (result == 1)
       as_warn (_("Implied attributes for section \'%s\'"
                  " are deprecated"), name);
     else if (result == -1)
       as_bad (_("Attributes for section \'%s\'"
                 " conflict with implied attributes"), name);

     /* set unordered attribute, if appropriate */
     if (has_auto_name)
       PIC30_SET_UNORDERED_ATTR(sec);

     /* encode any extended attributes in a symbol */
     if (pic30_extended_attribute_map(sec)) {
      char *name;
      char *ext_attr_prefix = "__ext_attr_";
      symbolS * symbolp;

      name = xmalloc (strlen(sec->name) + strlen(ext_attr_prefix) + 1);
      (void) sprintf(name, "%s%s", ext_attr_prefix, sec->name);
      if (!symbol_find(name)) {
        symbolp = symbol_new (name, absolute_section,
                              (valueT) pic30_extended_attribute_map(sec),
                              &zero_address_frag);

        symbol_table_insert (symbolp);
        symbol_mark_resolved (symbolp);
       }
     }
   } /* new style section directive */

   /*
   ** Detect invalid attribute combinations
   */
   {
     flagword flags = bfd_get_section_flags (stdoutput, now_seg);
     if (((flags & (SEC_CODE | SEC_NEVER_LOAD)) == (SEC_CODE | SEC_NEVER_LOAD)
	  || (( flags & SEC_NEVER_LOAD) && (now_seg->auxflash == 1))))
       as_warn (_("This executable section is not loadable.\n"));
   }

   /* Reset the global values for the new section */
   pic30_set_global_info ();

   if (now_seg->flags & SEC_CODE) 
     pic30_last_code_section = now_seg;
   if (flag_debug) {
      printf("    pic30_section:: %s, flags = %lx, map = %x\n", now_seg->name,
             (long unsigned int) now_seg->flags,
             pic30_attribute_map(now_seg));
      printf ("<-- pic30_section::exit\n");
   }
} /* void pic30_section(int) */

/******************************************************************************/

/******************************************************************************
 *
 *   Handles the .POPSECTION pseudo-op.
 *
 ******************************************************************************/
void
pic30_popsection (xxx)
     int xxx ATTRIBUTE_UNUSED;
{
  struct section_stack *top = section_stack;

  if (flag_debug)
    printf ("--> pic30_popsection::begin\n");

  if (top == NULL)
    {
      as_warn (_(".popsection without corresponding .pushsection; ignored"));
      if (flag_debug) {
        printf (".popsection without corresponding .pushsection; ignored\n");
        printf ("<-- pic30_popsection::exit\n");
      }
      return;
    }

  /*
   * Call pic30_do_align to align the current section.  This only affects a
   * .text or code section.
   */
  if ((subseg_text_p (now_seg)) || (now_seg == text_section))
    {
      PIC30_DO_CODE_ALIGNMENT ();
      listing_prev_line();  /* attach padding (if any) to previous */
    }                       /*   line in the listing file          */

  /* Store the global values for the current section */
  pic30_update_section_info ();

#ifdef md_flush_pending_output
  md_flush_pending_output ();
#endif

  section_stack = top->next;
  now_seg = top->seg;
  previous_section = top->prev_seg;
  free (top);

  /* restore global data associated with the previous section */
  pic30_set_global_info();

  /* switch to the previous section */
  subseg_new (now_seg->name, 0);

#ifdef OBJ_ELF
#ifdef md_elf_section_change_hook
  md_elf_section_change_hook ();
#endif
#endif

  if (flag_debug)
    printf ("<-- pic30_popsection::exit\n");
}

/******************************************************************************/

void
pic30_text (elf_arg)
   int elf_arg __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .TEXT pseudo-op.
 *
 *   This code is identical to that found in the obj-coff.c file for the .text
 *   directive.  The only addition is the pic30_do_align which will fill in the
 *   remaining bytes of a .text or code section so that the .text or code
 *   section is aligned.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   elf_arg           This argument is only used by the elf-specific code.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_text::begin\n");

   /*
    * Call pic30_do_align to align the current section.  This only affects a
    * .text or code section.
    */
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
     {
       PIC30_DO_CODE_ALIGNMENT ();
       listing_prev_line();  /* attach padding (if any) to previous */
     }                       /*   line in the listing file          */

   /* Store the global values for the current section */
   pic30_update_section_info ();

   /*
    * Reset whether the last directive was a p-directive.  No longer valid in
    * the new section.
    */
   global_p_directive_active = FALSE;

#ifdef OBJ_ELF
   obj_elf_text (elf_arg);
#else
   /*
    * This code was copied directly from obj-coff.c::obj_coff_text.  Could not
    * call this function directly as it declared statically.
    */
   subseg_new (".text", 0);
   demand_empty_rest_of_line ();  /* no subseg number allowed */
#endif

   /* Reset the global values for the new section */
   pic30_set_global_info ();

   if (flag_debug)
      printf ("<-- pic30_text::exit\n");
} /* void pic30_text(int) */

/******************************************************************************/

void
pic30_fillupper (ignore)
   int ignore __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .FILLUPPER pseudo-op.  
 *
 *   Sets the global_fillupper_value to the byte specified.  If no byte is
 *   specified, then returns to the default NOP_OPCODE value.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_fillupper::begin\n");

   /*
    * Make sure that code is currently in a .text or code section.  If not,
    * warn the user and ignore the directive.
    */
   if (!subseg_text_p (now_seg) && (now_seg != text_section))
   {
      as_warn (_(".fillupper not specified in a code section.  "
                 ".fillupper ignored."));
      discard_rest_of_line ();
   } /* if (!subseg_text_p (now_seg) && (now_seg != text_section)) */
   else
   {
      /*
       * Reset the global to the default value.
       */
      global_fillupper_value = NOP_OPCODE;

      /*
       * Parse the remainder of the statement.
       */
      if (!is_it_end_of_statement ())
      {
         offsetT absolute_value;

         SKIP_WHITESPACE ();
         absolute_value = get_absolute_expression ();

         /*
          * This directive expects a positive byte value.  If this is not
          * what was found, warn the user and tell them what is going to
          * be used; otherwise, set the global to what the user specified.
          */
         if ((absolute_value > 0xFF) || (absolute_value < 0))
            as_warn (_(".fillupper expects a constant positive byte value.  "
                       "0x%02x assumed"), global_fillupper_value);
         else
            global_fillupper_value = (unsigned char) absolute_value;
      } /* else */

      if (flag_debug)
         printf ("    pic30_fillupper::global_fillupper_value = 0x%02x\n",
                 global_fillupper_value);
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_fillupper::exit\n");
} /* pic30_fillupper */

/******************************************************************************/

void
pic30_fillvalue (ignore)
   int ignore __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .FILLVALUE pseudo-op.  
 *
 *   Sets the global_fillvalue_value to the byte specified.  If no byte is
 *   specified, then returns to the default DEFAULT_FILL_VALUE value.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_fillvalue::begin\n");

   /*
    * Make sure that code is currently in a .text or code section.  If not,
    * warn the user and ignore the directive.
    */
   if (!subseg_text_p (now_seg) && (now_seg != text_section))
   {
      as_warn (_(".fillvalue not specified in a code section.  "
                 ".fillvalue ignored."));
      discard_rest_of_line ();
   } /* if (!subseg_text_p (now_seg) && (now_seg != text_section)) */
   else
   {
      /*
       * Reset the global to the default value.
       */
      global_fillvalue_value = DEFAULT_FILL_VALUE;

      /*
       * Parse the remainder of the statement.
       */
      if (!is_it_end_of_statement ())
      {
         offsetT absolute_value;

         SKIP_WHITESPACE ();
         absolute_value = get_absolute_expression ();

         /*
          * This directive expects a positive byte value.  If this is not
          * what was found, warn the user and tell them what is going to
          * be used; otherwise, set the global to what the user specified.
          */
         if ((absolute_value > 0xFF) || (absolute_value < 0))
            as_warn (_(".fillvalue expects a constant positive byte value.  "
                       "0x%02x assumed"), global_fillvalue_value);
         else
            global_fillvalue_value = (unsigned char) absolute_value;
      } /* else */

      if (flag_debug)
         printf ("    pic30_fillvalue::global_fillvalue_value = 0x%02x\n",
                 global_fillvalue_value);
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_fillvalue::exit\n");
} /* pic30_fillvalue */

/******************************************************************************/

void
pic30_pfillvalue (ignore)
   int ignore __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Handles the .PFILLVALUE pseudo-op.  
 *
 *   Sets the global_pfillvalue_value to the byte specified.  If no byte is
 *   specified, then returns to the default DEFAULT_FILL_VALUE value.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   ignore            This argument is not used by this function, but all
 *                     pseudo-op functions require an argument.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_pfillvalue::begin\n");

   /*
    * Make sure that code is currently in a .text or code section.  If not,
    * warn the user and ignore the directive.
    */
   if (!subseg_text_p (now_seg) && (now_seg != text_section))
   {
      as_warn (_(".pfillvalue not specified in a code section.  "
                 ".pfillvalue ignored."));
      discard_rest_of_line ();
   } /* if (!subseg_text_p (now_seg) && (now_seg != text_section)) */
   else
   {
      /*
       * Reset the global to the default value.
       */
      global_pfillvalue_value = DEFAULT_FILL_VALUE;

      /*
       * Parse the remainder of the statement.
       */
      if (!is_it_end_of_statement ())
      {
         offsetT absolute_value;

         SKIP_WHITESPACE ();
         absolute_value = get_absolute_expression ();

         /*
          * This directive expects a positive byte value.  If this is not
          * what was found, warn the user and tell them what is going to
          * be used; otherwise, set the global to what the user specified.
          */
         if ((absolute_value > 0xFF) || (absolute_value < 0))
            as_warn (_(".pfillvalue expects a constant positive byte value.  "
                       "0x%02x assumed"), global_pfillvalue_value);
         else
            global_pfillvalue_value = (unsigned char) absolute_value;
      } /* else */

      if (flag_debug)
         printf ("    pic30_pfillvalue::global_pfillvalue_value = 0x%02x\n",
                 global_pfillvalue_value);
   } /* else */

   if (flag_debug)
      printf ("<-- pic30_pfillvalue::exit\n");
} /* pic30_pfillvalue */

/******************************************************************************/

void
pic30_not_supported (which_directive)
   int which_directive;

/******************************************************************************
 *
 *   Handles the pseudo-ops that are not supported by the pic30 port.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   which_directive   Which directive was found that is not supported.  Used
 *                     to give a descriptive error message.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_not_supported (%d)::begin\n", which_directive);

   if (which_directive < number_of_directives_not_supported)
      as_bad (_("%s directive not supported in pic30 target."),
               directives_not_supported[which_directive]);
   else
      BAD_CASE (which_directive);

   discard_rest_of_line ();

   if (flag_debug)
      printf ("<-- pic30_not_supported::end\n");
} /* void pic30_not_supported(int) */

/******************************************************************************/

unsigned char
pic30_cons_emit (out_string, length)
   char * out_string;
   unsigned int length;

/******************************************************************************
 *
 *   GAS will call this to emit a data allocation pseudo-op.  This function
 *   assumes that the buffer received is already in the correct endian format.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   out_string        The buffer to output.
 *   length            The length of out_string.
 *
 *   RETURNS:
 *      0 - This function did not output the buffer.  Let the default handling
 *          occur.
 *      1 - This function output the buffer.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;

   if (flag_debug)
   {
      unsigned int i;

      printf ("--> pic30_cons_emit (%u)::begin\n", length);

      printf ("    pic30_cons_emit::");
      for (i = 0; i < length; i++)
         printf ("0x%02x%c", out_string[i], (i == (length - 1)) ? '\n' : ' ');
   } /* if (flag_debug) */

   /*
    * Only want to do this if we are in a .text or code section.  Otherwise,
    * let the default handling occur.
    */
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
   {
      unsigned int i;

      for (i = 0; i < length; i++)
         PIC30_INSERT_VALUE_INTO_PROGRAM_MEMORY (out_string[i]);

      /*
       * Test and pad the program word, if necessary.
       */
      PIC30_PAD_PROGRAM_MEMORY_AFTER_INSERT();

      /*
       * Set the flag that says the location counter has been screwed with.
       */
      global_must_align_location_counter = TRUE;

      rc = TRUE;
   } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */
   else
   {
      unsigned int i;

      for (i = 0; i < length; i++)
        {
         PIC30_INSERT_VALUE_INTO_DATA_MEMORY (out_string[i]);
        }

      rc = TRUE;
   } /* else (data memory) */


   if (flag_debug)
      printf ("<-- pic30_cons_emit::exit\n");

   return rc;
} /* unsigned char pic30_cons_emit(char *, unsigned int) */

/******************************************************************************/

unsigned char
pic30_cons_emit_expr (exp, number_of_bytes)
   expressionS * exp;
   unsigned int number_of_bytes;

/******************************************************************************
 *
 *   GAS will call this to emit an expression for a data allocation pseudo-op.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   exp               The expression to emit.
 *   number_of_bytes   The number of bytes normally required for the data
 *                     allocation pseudo-op.
 *
 *   RETURNS:
 *      0 - This function did not output the buffer.  Let the default handling
 *          occur.
 *      1 - This function output the buffer.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;

   if (flag_debug)
   {
      printf ("--> pic30_cons_emit_expr (..., %u)::begin\n", number_of_bytes);
      printf ("    pic30_cons_emit_expr::SEC_CODE = %s\n",
              ((subseg_text_p (now_seg)) ||
               (now_seg == text_section)) ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   /*
    * Only want to do this if we are in a .text or code section.  Otherwise,
    * let the default handling occur.
    */
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
   {
      char * p;
      unsigned int number_of_bytes_to_allocate = number_of_bytes;
      int j = global_current_location;
      int i;

      if (flag_debug)
         printf ("    pic30_cons_emit_expr::global_current_location = %lu\n",
                 global_current_location);

      for (i = number_of_bytes; i != 0; j++)
      {
         if (PIC30_CAN_WRITE (j, global_p_directive_active))
            i--;
         else
            number_of_bytes_to_allocate++;
      } /* for i */

      if (flag_debug)
         printf ("    pic30_cons_emit_expr::number_of_bytes_to_allocate = %u\n",
                 number_of_bytes_to_allocate);

      /*
       * Set the flag that says the location counter has been screwed with.
       */
      global_must_align_location_counter = TRUE;

      p = frag_more (number_of_bytes_to_allocate);
      memset (p, 0, number_of_bytes_to_allocate);

      TC_CONS_FIX_NEW (frag_now, p - frag_now->fr_literal,
                       (int) number_of_bytes, exp);

      /*
       * Need to manually update the location counter used by pic30 specific
       * functions since we are not using the
       * PIC30_INSERT_VALUE_INTO_PROGRAM_MEMORY macro.  This will allow
       * alignments and fills to work properly.
       */
      global_current_location += number_of_bytes_to_allocate;

      /*
       * Test and pad the program word, if necessary.
       */
      PIC30_PAD_PROGRAM_MEMORY_AFTER_INSERT();

      rc = TRUE;
   } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */
   else
   {
      char * p;
      unsigned int number_of_bytes_to_allocate = number_of_bytes * 2;
 
      p = frag_more (number_of_bytes_to_allocate);
      memset (p, 0, number_of_bytes_to_allocate);

      TC_CONS_FIX_NEW (frag_now, p - frag_now->fr_literal,
                       (int) number_of_bytes, exp);

      rc = TRUE;
   } /* else (data memory) */

   if (flag_debug)
   {
      printf ("    pic30_cons_emit_expr::global_current_location = %lu\n",
              global_current_location);
      printf ("<-- pic30_cons_emit_expr::exit\n");
   } /* if (flag_debug) */

   return rc;
} /* unsigned char pic30_cons_emit_expr(expressionS *, unsigned int) */

/******************************************************************************/

/* The default target format to use.  */

const char *
pic30_target_format(void)
{
#if defined(OBJ_ELF)
    return("elf32-pic30");
#else
    return("coff-pic30");
#endif
}

/******************************************************************************
 *
 *   This function is the implementation of the macro TARGET_MACH.  It returns
 *   the machine number based on whether the --processor command-line option
 *   was specified..
 *
 *   RETURNS:
 *      0 - --processor command-line option was not utilized.  This represents
 *          the default machine
 *      # - The machine number corresponding to the specified machine specified
 *          by the --processor command-line option.
 *
 ******************************************************************************/
unsigned long
pic30_target_mach (void)
{
   unsigned long rc = 0;

   if (global_PROCESSOR)
      rc = global_PROCESSOR->mach;

   return rc;
} /* unsigned long pic30_target_mach(void) */

/******************************************************************************/

static int
pic30_update_processor (argument)
     char *argument;
{
  int rc = 1;

  global_PROCESSOR = pic30_get_machine (argument);

  if (!global_PROCESSOR)
    {
      fprintf (stderr, "Unknown processor %s.\n", argument);
      rc = 0;
    }
  return rc;
}

/******************************************************************************/

int
md_parse_option (option, argument)
   int option;
   char * argument __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function is called by GAS whenever getopt returns an unrecognized
 *   code, presumably indicating a special code value which appears in
 *   md_longopts (i.e., called for unrecognized command-line options to see
 *   if it is PIC30 specific).
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   option            The unique value representing the option found.
 *   argument          The option's argument.
 *
 *   RETURNS:
 *      0 - unrecognized command-line option
 *      1 - recognized command-line option
 *
 *   NOTES:
 *      The --cpu option is equivalent to, and takes precedence over,
 *      -p,--processor. This will helps us manage a transition period
 *      with pic30-gcc and the IDE with regards to "A2" processor IDs.
 *
 ******************************************************************************/

{
   int rc = 1;
   const char *multi_proc_message = "Multiple processors specified.\n";

   if (flag_debug)
   {
      printf ("--> md_parse_option (%d, ...)::begin\n", option);
      printf ("    md_parse_option::old.global_RELAX = %s\n",
              (global_RELAX == NO_RELAX_STATE) ? "none specified" :
              (global_RELAX == DO_NOT_RELAX) ? "do NOT relax" : "relax");
   } /* if (flag_debug) */

   switch (option)
   {
      case (PIC30_NO_RELAX) :
         if (global_RELAX == NO_RELAX_STATE)
            global_RELAX = DO_NOT_RELAX;
         else if (global_RELAX == DO_RELAX)
         {
            fprintf (stderr, "--no-relax cannot be used with --relax.\n");
            rc = 0;
         } /* else if (global_RELAX == DO_RELAX) */
         break;

      case (PIC30_RELAX) :
         if (global_RELAX == NO_RELAX_STATE)
            global_RELAX = DO_RELAX;
         else if (global_RELAX == DO_NOT_RELAX)
         {
            fprintf (stderr, "--relax cannot be used with --no-relax.\n");
            rc = 0;
         } /* else if (global_RELAX = DO_NOT_RELAX) */
         break;

      case 'g':
         global_DEBUGINFO = GEN_DEBUGINFO;
#if defined(OBJ_ELF)
	 /*
	 ** set base-code flag
	 */
      	 debug_type = DEBUG_DWARF2;
#endif
         break;

      case 'p':
         if (has_proc_option)
         {
            fprintf (stderr, multi_proc_message);
            rc = 0;
         }
         else if (has_cpu_option)
           /* --cpu has priority over -p */
           break;
         else
         {
           has_proc_option = TRUE;
           rc = pic30_update_processor(argument);
         }
         break;

      case PIC30_CPU:
         if (has_cpu_option)
         {
            fprintf (stderr, multi_proc_message);
            rc = 0;
         }
         else
         {
           has_cpu_option = TRUE;
           rc = pic30_update_processor(argument);
         }
         break;

      default :
         rc = 0;
         break;
   } /* switch */

   if (flag_debug)
   {
      printf ("    md_parse_option::new.global_RELAX = %s\n",
              (global_RELAX == NO_RELAX_STATE) ? "none specified" :
              (global_RELAX == DO_NOT_RELAX) ? "do NOT relax" : "relax");
      printf ("<-- md_parse_option::exit\n");
   } /* if (flag_debug) */

   return rc;
} /* int md_parse_option(int, char *) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called by GAS when a usage message is printed.  It should
 *   print a description of the machine specific command-line options.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   stream            The FILE stream to output the description of machine
 *                        specific command-line options.
 *
 ******************************************************************************/
void
md_show_usage (stream)
   FILE * stream;
{
   fprintf (stream, "\nMicrochip PIC30 options:\n"
      "  -g                      Generate symbolic debug info.\n"
      "  --no-relax              Turn off relaxation (default).\n"
      "  --relax                 Use shorter branches and RCALLs when possible.\n"
      "  -p,--processor=PROC     Specify the target processor "
                                "(e.g., 30F2010).\n"
      );
} /* void md_show_usage(FILE *) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called by GAS at the start of assembly, after the command
 *   line arguments have been parsed and all the machine independent
 *   initializations have been completed, but before parsing has begun.  Do
 *   any machine specific initializations here.
 *
 ******************************************************************************/
static void
create_absolute_symbol(const char *s, valueT val) {
  symbolS * sym = symbol_new (s, absolute_section, val, &zero_address_frag);
  symbol_table_insert (sym);
  symbol_mark_resolved (sym);
}

void
md_begin (void)
{
   char * previous_name = "";
   register const struct pic30_opcode * opcode;
   register const struct pic30_opcode * last_opcode;
   int symbol_count = 0;
   int is_ecore = pic30_is_ecore_machine(global_PROCESSOR);

   if (flag_debug)
      printf ("--> md_begin::begin\n");

   pic30_opcodes_hash = hash_new ();

   if (flag_debug)
      printf ("    md_begin::inserting opcodes...\n");

   last_opcode = pic30_opcodes + pic30_num_opcodes;
   for (opcode = pic30_opcodes; opcode < last_opcode; opcode++)
   { int add_me = 0;

      add_me = ((opcode->flags & F_ECORE) && (is_ecore));
      add_me |= ((opcode->flags & F_FCORE) && (!is_ecore));
      add_me |= (((opcode->flags & F_ECORE) == 0) && 
		 ((opcode->flags & F_FCORE) == 0));
      if ((strcmp (opcode->name, previous_name) != 0) && add_me)
      {
         previous_name = (char *)opcode->name;
         hash_insert (pic30_opcodes_hash, opcode->name, (void *) opcode);
      } /* if (strcmp (opcode->name, previous_name) != 0) */
   } /* for opcode */

   if (flag_debug)
      printf ("    md_begin::inserting symbols...\n");

   /* Insert the predefined symbols into the symbol table.  */
   for (symbol_count = 0;
        symbol_count < pic30_num_registers;
        symbol_count++)
   {
      /*
       * Insert the predefined symbols into the symbol table.  We use
       * symbol_create rather than symbol_new so that these symbols do not
       * end up in the object files' symbol table.
       *
       * The % 16 is because there are only 16 registers; however, the
       * register array has the registers listed twice.  One with a lower
       * case 'w' and one with an upper case 'W'.
       */
      symbol_table_insert (symbol_create (pic30_registers[symbol_count],
                                          reg_section, symbol_count % 16,
                                          &zero_address_frag));
   } /* for symbol_count */

   if (global_PROCESSOR)
   {
#define MAX_PROC_FAMILY_LEN 16
      int proc_family = pic30_proc_family(global_PROCESSOR);
      char * family =
         (char *) xmalloc (2 + MAX_PROC_FAMILY_LEN + 1);
      char * processor =
         (char *) xmalloc (2 + strlen (global_PROCESSOR->printable_name) + 1);
      symbolS * symbolp;

      if (flag_debug)
         printf ("    md_begin::Creating symbol for processor %s...\n",
                 global_PROCESSOR->printable_name);

      sprintf (processor, "__%s", global_PROCESSOR->printable_name);

      symbolp = symbol_new (processor, absolute_section,
                            (valueT) 1, &zero_address_frag);

      symbol_table_insert (symbolp);

      symbol_mark_resolved (symbolp);

      free (processor);

      /* create a symbol for the processor family */
      switch (proc_family)
        {
        case P30F:
        case P30FSMPS:
          sprintf (family, "__dsPIC30F" );
          break;
        case P33F:
          sprintf (family, "__dsPIC33F" );
          break;
        case P24F:
          sprintf (family, "__PIC24F" );
          break;
        case P24H:
          sprintf (family, "__PIC24H" );
          break;
        case P24FK:
          sprintf (family, "__PIC24FK" );
          break;
        case P24E:
          sprintf (family, "__PIC24E" );
          break;
        case P33E:
          sprintf (family, "__dsPIC33E" );
          break;
        default:
          sprintf (family, "__MCHP16" );
        }

      symbolp = symbol_new (family, absolute_section,
                            (valueT) 1, &zero_address_frag);

      symbol_table_insert (symbolp);
      symbol_mark_resolved (symbolp);
      free (family);

      /* create other symbols we may need */
      if (pic30_is_dsp_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_DSP", 1);

      if (pic30_is_eedata_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_EEDATA", 1);

      { int dma_version = pic30_is_dma_machine(global_PROCESSOR);

        if (dma_version == 1) create_absolute_symbol("__HAS_DMA", 1);
        if (dma_version == 2) create_absolute_symbol("__HAS_DMAV2", 1);
      }

      if (pic30_is_codeguard_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_CODEGUARD", 1);

      if (pic30_is_pmp_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_PMP", 1);

      if (pic30_is_epmp_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_PMP_ENHANCED", 1);

      if (pic30_is_eds_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_EDS", 1);

      if (pic30_is_5V_machine(global_PROCESSOR))
        create_absolute_symbol("__HAS_5VOLTS", 1);


   } /* if (global_PROCESSOR) */
   /*
   ** Create a symbol to indicate which OMF is supported by the assembler
   */
   {
#if defined(OBJ_ELF)
      const char *pszOMFdef = "__C30ELF";
#else
      const char *pszOMFdef = "__C30COFF";
#endif
      symbolS * symbolp;

      symbolp = symbol_new (pszOMFdef, absolute_section,
                            (valueT) 1, &zero_address_frag);

      symbol_table_insert (symbolp);
      symbol_mark_resolved (symbolp);
   }

   if (flag_debug)
      printf ("<-- md_begin::exit\n");
} /* void md_begin(void) */

/******************************************************************************/

void
pic30_cleanup (void)

/******************************************************************************
 *
 *   This function is called by GAS at the end of each input file.  The pic30
 *   port uses this function to align the last CODE section.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_cleanup::begin\n");

   PIC30_DO_CODE_ALIGNMENT ();

   if (flag_debug)
      printf ("<-- pic30_cleanup::exit\n");
} /* void pic30_cleanup(void) */

/******************************************************************************/

static bfd_size_type program_memory_used = 0;
static bfd_size_type data_memory_used = 0;

void
pic30_display_section_info (out_file, sec, display_code_sections)
   FILE * out_file;
   asection * sec;
   unsigned char display_code_sections;
{
   segment_info_type * info = seg_info (sec);
   unsigned int opb = bfd_octets_per_byte (stdoutput);

   /* If seginfo is NULL, we did not create this section; don't do
      anything with it.  */
   if (info == NULL)
      return;

   if (display_code_sections)
   {
      if (((sec->flags & SEC_LOAD) && (sec->flags & SEC_CODE)) ||
          (sec == text_section) || (sec->auxflash == 1))
      {
         bfd_size_type total = sec->_raw_size;
         fprintf (out_file, "%-15s%#17lx%#16lx  (%ld)\n", sec->name, total/opb,
                  (unsigned long) (total * .75), (unsigned long) (total * .75));

         program_memory_used += (total * .75);
      }
   } /* if (display_code_sections) */
   else
   {
     if (((sec->flags & SEC_LOAD) && (sec->flags & SEC_DATA)) /* DATA */ || 
         ((sec->flags & (SEC_ALLOC | SEC_LOAD)) == SEC_ALLOC) /* BSS */ || 
          (sec == data_section))
      {
         bfd_size_type total = sec->_raw_size / opb;
         fprintf (out_file, "%-15s%#17x%#16lx  (%ld)\n", sec->name,
                  sec->alignment_gaps, total, total);

         data_memory_used += total;
      }
   } /* else */
}

/******************************************************************************/

void
pic30_display_code_section_info (abfd, sec, fp)
   bfd * abfd  __attribute__ ((__unused__));
   asection * sec;
   PTR fp;
{
   pic30_display_section_info ((FILE *) fp, sec, TRUE);
}

/******************************************************************************/

void
pic30_display_data_section_info (abfd, sec, fp)
   bfd * abfd  __attribute__ ((__unused__));
   asection * sec;
   PTR fp;
{
   pic30_display_section_info ((FILE *) fp, sec, FALSE);
}

/******************************************************************************/

void
pic30_list_section_info (out_file)
   FILE * out_file;
{
   fprintf (out_file, "\nSECTION INFORMATION:\n\n");
   fprintf (out_file, "Section        Length (PC units)   Length (bytes) (dec)\n");
   fprintf (out_file, "-------        -----------------   --------------------\n");

   bfd_map_over_sections (stdoutput, pic30_display_code_section_info,
                          out_file);

   fprintf (out_file, "\nTOTAL PROGRAM MEMORY USED (bytes): %#13lx  (%ld)\n\n",
            program_memory_used, program_memory_used); 

   fprintf (out_file, "Section           Alignment Gaps   Length (bytes) (dec)\n");
   fprintf (out_file, "-------           --------------   --------------------\n");

   bfd_map_over_sections (stdoutput, pic30_display_data_section_info,
                          out_file);

   fprintf (out_file, "\n   TOTAL DATA MEMORY USED (bytes): %#13lx  (%ld)\n",
            data_memory_used, data_memory_used); 
}

/******************************************************************************/

void
pic30_list_symbol_info (list_file)
   FILE * list_file;

/****************************************************************************
 *
 *   This function is called by GAS after symbol tables have been
 *   written to the listing file.  It is used to list additional symbol
 *   info required by the target. 
 *
 ****************************************************************************/
{
  int i;
  symbolS *s;

  fprintf (list_file, "\n");

  if (pic30_symbol_count == 0)
    return; /* We might prefer to print "NO EQUATE SYMBOLS" here,
             * but this would mis-lead the user. We build this
             * list in tc_frob_symbol(), which is not called if
             * the assembly fails due to errors.
             * If the assembly succeeds, there is always at
             * least one equate symbol: __C30_COFF or __C30_ELF.
             */
  else {
    fprintf (list_file, "EQUATE SYMBOLS\n");

    for (i = 0; i < pic30_symbol_count; i++) {
      s = pic30_equate_symbols[i];
      fprintf(list_file, "%32s = ", S_GET_NAME (s));

      if (S_GET_SEGMENT (s) == absolute_section)
        fprintf(list_file, "0x%lx\n", s->sy_value.X_add_number);
      else
        fprintf(list_file, "%s\n", S_GET_NAME (s->sy_value.X_add_symbol));
    }
  }
}

/****************************************************************************/

void
pic30_end (void)

/******************************************************************************
 *
 *   This function is called by GAS at the end of assembly.  It is used to
 *   cleanup any memory that was allocated in md_begin.  
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_end::begin\n");

   hash_die (pic30_opcodes_hash);

   if (flag_debug)
      printf ("<-- pic30_end::exit\n");
} /* void pic30_end(void) */

/******************************************************************************/

void
print_expression (expressionP)
   const expressionS * expressionP;

/******************************************************************************
 *
 *   This function is a debugging function.  It is called when flag_debug is
 *   TRUE to print out information about an expression that was found.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   expressionP       The expression to print details regarding.
 *
 ******************************************************************************/

{
   unsigned char print_closing_paren = TRUE;

   if (expressionP->X_md.immediate)
      fprintf (stdout, "# ");

   switch (expressionP->X_md.modifier)
   {
      case PIC30_TBLOFFSET_FOUND:
         fprintf (stdout, "tbloffset (");
         break;

      case PIC30_PSVOFFSET_FOUND:
         fprintf (stdout, "psvoffset (");
         break;

      case PIC30_TBLPAGE_FOUND:
         fprintf (stdout, "tblpage (");
         break;

      case PIC30_PSVPAGE_FOUND:
         fprintf (stdout, "psvpage (");
         break;

      case PIC30_HANDLE_FOUND:
         fprintf (stdout, "handle (");
         break;

      case PIC30_PADDR_FOUND:
         fprintf (stdout, "paddr (");
         break;

      case PIC30_DMAPAGE_FOUND:
         fprintf (stdout, "dmapage (");
         break;

      case PIC30_DMAOFFSET_FOUND:
         fprintf (stdout, "dmaoffset (");
         break;

      case PIC30_PSVPTR_FOUND:
         fprintf (stdout, "psvptr (");
         break;

      case PIC30_BOOT_FOUND:
         fprintf (stdout, "boot (");
         break;

      case PIC30_SECURE_FOUND:
         fprintf (stdout, "secure (");
         break;

      case PIC30_EDSPAGE_FOUND:
         fprintf (stdout, "edspage (");
         break;

      case PIC30_EDSOFFSET_FOUND:
         fprintf (stdout, "edsoffset (");
         break;

      default:
         print_closing_paren = FALSE;
         break;
   } /* switch (expressionP->X_md.modifier) */

   switch (expressionP->X_op)
   {
      case O_register:
#ifdef BFD64
         fprintf (stdout, "O_register::%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "O_register::%ld", expressionP->X_add_number);
#endif
         break;

      /* An illegal expression.  */
      case O_illegal:
         fprintf (stdout, "O_illegal");
         break;

      /* A nonexistent expression.  */
      case O_absent:
         fprintf (stdout, "O_absent");
         break;

      /* X_add_number (a constant expression).  */
      case O_constant:
#ifdef BFD64
         fprintf (stdout, "O_constant::%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "O_constant::%ld", expressionP->X_add_number);
#endif
         break;

      /* X_add_symbol + X_add_number.  */
      case O_symbol:
         fprintf (stdout, "O_symbol::");
         fprintf (stdout, "%s", S_GET_NAME (expressionP->X_add_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* X_add_symbol + X_add_number - the base address of the image.  */
      case O_symbol_rva:
         fprintf (stdout, "O_symbol_rva::");
         fprintf (stdout, "%s", S_GET_NAME (expressionP->X_add_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* A big value.  If X_add_number is negative or 0, the value is in
         generic_floating_point_number.  Otherwise the value is in
         generic_bignum, and X_add_number is the number of LITTLENUMs in
         the value.  */
      case O_big:
         fprintf (stdout, "O_big::");

         if (expressionP->X_add_number > 0)
         {
            int i;

            for (i = 0; i < expressionP->X_add_number; i++)
               fprintf (stdout, "%02x ", generic_bignum[i]);
         } /* if (expressionP->X_add_number > 0) */
         else
            fprintf (stdout, "floating_point");

         break;

      /* (- X_add_symbol) + X_add_number.  */
      case O_uminus:
         fprintf (stdout, "O_uminus::");
         fprintf (stdout, "-%s", S_GET_NAME (expressionP->X_add_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (~ X_add_symbol) + X_add_number.  */
      case O_bit_not:
         fprintf (stdout, "O_bit_not::");
         fprintf (stdout, "~%s", S_GET_NAME (expressionP->X_add_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (! X_add_symbol) + X_add_number.  */
      case O_logical_not:
         fprintf (stdout, "O_logical_not::");
         fprintf (stdout, "!%s", S_GET_NAME (expressionP->X_add_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol * X_op_symbol) + X_add_number.  */
      case O_multiply:
         fprintf (stdout, "O_multiply::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "*%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol / X_op_symbol) + X_add_number.  */
      case O_divide:
         fprintf (stdout, "O_divide::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "/%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol % X_op_symbol) + X_add_number.  */
      case O_modulus:
         fprintf (stdout, "O_modulus::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "%%%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol << X_op_symbol) + X_add_number.  */
      case O_left_shift:
         fprintf (stdout, "O_left_shift::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "<<%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol >> X_op_symbol) + X_add_number.  */
      case O_right_shift:
         fprintf (stdout, "O_right_shift::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, ">>%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol | X_op_symbol) + X_add_number.  */
      case O_bit_inclusive_or:
         fprintf (stdout, "O_bit_inclusive_or::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "|%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol |~ X_op_symbol) + X_add_number.  */
      case O_bit_or_not:
         fprintf (stdout, "O_bit_or_not::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "|~%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol ^ X_op_symbol) + X_add_number.  */
      case O_bit_exclusive_or:
         fprintf (stdout, "O_bit_exclusive_or::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "^%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol & X_op_symbol) + X_add_number.  */
      case O_bit_and:
         fprintf (stdout, "O_bit_and::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "&%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol + X_op_symbol) + X_add_number.  */
      case O_add:
         fprintf (stdout, "O_add::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "+%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol - X_op_symbol) + X_add_number.  */
      case O_subtract:
         fprintf (stdout, "O_subtract::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "-%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol == X_op_symbol) + X_add_number.  */
      case O_eq:
         fprintf (stdout, "O_eq::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "==%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol != X_op_symbol) + X_add_number.  */
      case O_ne:
         fprintf (stdout, "O_ne::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "!=%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol < X_op_symbol) + X_add_number.  */
      case O_lt:
         fprintf (stdout, "O_lt::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "<%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol <= X_op_symbol) + X_add_number.  */
      case O_le:
         fprintf (stdout, "O_le::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "<=%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol >= X_op_symbol) + X_add_number.  */
      case O_ge:
         fprintf (stdout, "O_ge::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, ">=%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol > X_op_symbol) + X_add_number.  */
      case O_gt:
         fprintf (stdout, "O_gt::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, ">%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol && X_op_symbol) + X_add_number.  */
      case O_logical_and:
         fprintf (stdout, "O_logical_and::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "&&%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* (X_add_symbol || X_op_symbol) + X_add_number.  */
      case O_logical_or:
         fprintf (stdout, "O_logical_or::");
         fprintf (stdout, "(%s", S_GET_NAME (expressionP->X_add_symbol));
         fprintf (stdout, "||%s)", S_GET_NAME (expressionP->X_op_symbol));
#ifdef BFD64
         fprintf (stdout, "+%lld", expressionP->X_add_number);
#else
         fprintf (stdout, "+%ld", expressionP->X_add_number);
#endif
         break;

      /* X_op_symbol [ X_add_symbol ] */
      case O_index:
         fprintf (stdout, "O_index::");
         fprintf (stdout, "%s)", S_GET_NAME (expressionP->X_op_symbol));
         fprintf (stdout, "[%s]", S_GET_NAME (expressionP->X_add_symbol));
         break;

      case O_REGISTER_INDIRECT:
#ifdef BFD64
         fprintf (stdout, "O_REGISTER_INDIRECT::%lld",
                  expressionP->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_INDIRECT::%ld",
                  expressionP->X_add_number);
#endif
         break;

      case O_REGISTER_PRE_INCREMENT:
#ifdef BFD64
         fprintf (stdout, "O_REGISTER_PRE_INCREMENT::%lld",
                  expressionP->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_PRE_INCREMENT::%ld",
                  expressionP->X_add_number);
#endif
         break;

      case O_REGISTER_PRE_DECREMENT:
#ifdef BFD64
         fprintf (stdout, "O_REGISTER_PRE_DECREMENT::%lld",
                  expressionP->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_PRE_DECREMENT::%ld",
                  expressionP->X_add_number);
#endif
         break;

      case O_REGISTER_WITH_DISPLACEMENT:
      {
         expressionS * left =
            symbol_get_value_expression (expressionP->X_add_symbol);
         expressionS * right =
            symbol_get_value_expression (expressionP->X_op_symbol);

#ifdef BFD64
         fprintf (stdout, "O_REGISTER_WITH_DISPLACEMENT::w%lld + w%lld",
                  left->X_add_number, right->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_WITH_DISPLACEMENT::w%ld + w%ld",
                  left->X_add_number, right->X_add_number);
#endif
         break;
      }

      case O_REGISTER_PLUS_LITERAL:
      {
         expressionS * left =
            symbol_get_value_expression (expressionP->X_add_symbol);
         expressionS * right =
            symbol_get_value_expression (expressionP->X_op_symbol);

#ifdef BFD64
         fprintf (stdout, "O_REGISTER_PLUS_LITERAL::w%lld + %lld",
                  left->X_add_number, right->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_PLUS_LITERAL::w%ld + %ld",
                  left->X_add_number, right->X_add_number);
#endif
         break;
      }

      case O_REGISTER_MINUS_LITERAL:
      {
         expressionS * left =
            symbol_get_value_expression (expressionP->X_add_symbol);
         expressionS * right =
            symbol_get_value_expression (expressionP->X_op_symbol);

#ifdef BFD64
         fprintf (stdout, "O_REGISTER_MINUS_LITERAL::w%lld - %lld",
                  left->X_add_number, right->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_MINUS_LITERAL::w%ld - %ld",
                  left->X_add_number, right->X_add_number);
#endif
         break;
      }

      case O_REGISTER_POST_INCREMENT:
      {
#ifdef BFD64
         fprintf (stdout, "O_REGISTER_POST_INCREMENT::%lld",
                  expressionP->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_POST_INCREMENT::%ld",
                  expressionP->X_add_number);
#endif
         break;
      }

      case O_REGISTER_POST_DECREMENT:
      {
#ifdef BFD64
         fprintf (stdout, "O_REGISTER_POST_DECREMENT::%lld",
                  expressionP->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_POST_DECREMENT::%ld",
                  expressionP->X_add_number);
#endif
         break;
      }

      case O_REGISTER_POST_INCREMENT_BY_N:
      {
         expressionS * left =
            symbol_get_value_expression (expressionP->X_add_symbol);
         expressionS * right =
            symbol_get_value_expression (expressionP->X_op_symbol);

#ifdef BFD64
         fprintf (stdout, "O_REGISTER_POST_INCREMENT_BY_N::[w%lld]+=%lld",
                  left->X_add_number, right->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_POST_INCREMENT_BY_N::[w%ld]+=%ld",
                  left->X_add_number, right->X_add_number);
#endif

         break;
      }

      case O_REGISTER_POST_DECREMENT_BY_N:
      {
         expressionS * left =
            symbol_get_value_expression (expressionP->X_add_symbol);
         expressionS * right =
            symbol_get_value_expression (expressionP->X_op_symbol);

#ifdef BFD64
         fprintf (stdout, "O_REGISTER_POST_DECREMENT_BY_N::[w%lld]+=%lld",
                  left->X_add_number, right->X_add_number);
#else
         fprintf (stdout, "O_REGISTER_POST_DECREMENT_BY_N::[w%ld]+=%ld",
                  left->X_add_number, right->X_add_number);
#endif

         break;
      }

      case O_DSP_ACCUMULATOR_SELECT:
         fprintf (stdout, "O_DSP_ACCUMULATOR_SELECT::%c",
                  expressionP->X_add_number == PIC30_DSP_ACCUMULATOR_A ? 'A'
                                                                       : 'B'); 
         break;

      case O_DSP_MULTIPLICAND:
      {
         char multiplicand[PIC30_MAX_DSP_MULTIPLICAND_LENGTH + 1];

         switch (expressionP->X_add_number)
         {
            case PIC30_DSP_W4_W5_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W4_W5_MULTIPLICAND_STRING);
               break;

            case PIC30_DSP_W4_W6_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W4_W6_MULTIPLICAND_STRING);
               break;

            case PIC30_DSP_W4_W7_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W4_W7_MULTIPLICAND_STRING);
               break;

            case PIC30_DSP_W5_W6_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W5_W6_MULTIPLICAND_STRING);
               break;

            case PIC30_DSP_W5_W7_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W5_W7_MULTIPLICAND_STRING);
               break;

            case PIC30_DSP_W6_W7_MULTIPLICAND:
               strcpy (multiplicand, PIC30_DSP_W6_W7_MULTIPLICAND_STRING);
               break;

            default:
               strcpy (multiplicand, "???");
               break;
         }

         fprintf (stdout, "O_DSP_MULTIPLICAND::%s", multiplicand);

         break;
      } /* case O_DSP_MULTIPLICAND */

      case O_DSP_SQUARE:
      {
#ifdef BFD64
         fprintf (stdout, "O_DSP_SQUARE::w%lld*w%lld",
                  expressionP->X_add_number, expressionP->X_add_number);
#else
         fprintf (stdout, "O_DSP_SQUARE::w%ld*w%ld",
                  expressionP->X_add_number, expressionP->X_add_number);
#endif
         break;
      }

      default:
         fprintf (stdout, "UNKNOWN OPERAND TYPE");
         break;
   } /* switch */

   if (print_closing_paren)
      fprintf (stdout, ")");
} /* void print_expression(const expressionS *) */

/******************************************************************************/

int
pic30_parse_name (name, expr)
   const char * name;
   struct expressionS * expr;

/******************************************************************************
 *
 *   This function is called by GAS for any symbol found in an expression.
 *   This function will check to see if we are currently parsing an operand
 *   that requires special handling for symbols.  The operands that require
 *   special handling include:
 *   - Implied Register -- If the opcode may have an implied register, check to
 *     see if the name equals the PIC30_W_REGISTER_STRING.  If it does,
 *     then this is represented by the operand type O_W_REG and the
 *     expr->X_add_number is set to represent the working register.
 *   - Branch Condition Code -- If the opcode represents a branch instruction
 *     and the operand being parsed is currently in the position of a branch
 *     condition code, check to see if the name equals one of the known branch
 *     condition codes.  If so, this is represented by the operand type
 *     O_BRANCH_ON_CONDITION or O_BRANCH_ON_DSP_CONDITION, depending on the
 *     condition code found and the expr->X_add_number should be set to the
 *     condition code value.
 *   - DSP Accumulator Select -- If the opcode represents a DSP instruction
 *     and the operand being parsed is currently in the position of the DSP
 *     accumulator select, check to see if the name equals one of the two
 *     known accumlators.  If so, this is represented by the operand type
 *     O_DSP_ACCUMULATOR_SELECT and the expr->X_add_number should be set to the
 *     appropriate DSP accumulator select value.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   name              The name of the symbol.
 *   expr              The expression to populate if the symbol is recognized.
 *
 *   RETURNS:
 *      0 - unrecognized symbol
 *      1 - recognized symbol.  expr->X_op and expr->X_add_number set.
 *
 ******************************************************************************/

{
   int rc = 0;

   if (flag_debug)
   {
      fprintf (stdout, "--> pic30_parse_name (%s)::begin\n", name);

      fprintf (stdout, "    pic30_parse_name::HAS_WREG = %s\n",
         ((global_current_opcode_flags & F_HAS_WREG) ? "TRUE" : "FALSE"));

      fprintf (stdout, "    pic30_parse_name::HAS_BRANCH_FLAGS = %s\n",
         (((global_current_opcode_flags & F_HAS_BRANCH_FLAG) ==
           F_HAS_BRANCH_FLAG) &&
          (global_current_operand_position ==
           PIC30_BRANCH_FLAG_OPERAND_POSITION)
         ? "TRUE" : "FALSE"));

      fprintf (stdout, "    pic30_parse_name::F_IS_DSP_INSN = %s\n",
         ((global_current_opcode_flags & F_IS_DSP_INSN) == F_IS_DSP_INSN)
         ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   if (global_current_opcode_flags & F_HAS_WREG)
   {
      rc = 1;

      if (strcasecmp (name, PIC30_W_REGISTER_STRING) == 0)
      {
         expr->X_add_number = PIC30_DESTINATION_WORKING_REGISTER;
         expr->X_op = O_W_REG;
      } /* if (strcasecmp (name, PIC30_W_REGISTER_STRING) == 0) */
      else
         rc = 0;
   } /* if */

   if ((!rc) && ((global_current_opcode_flags & F_HAS_BRANCH_FLAG) ==
         F_HAS_BRANCH_FLAG) &&
        (global_current_operand_position == PIC30_BRANCH_FLAG_OPERAND_POSITION))
   {
      rc = 1;

      if (strcasecmp (name, "OV") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_OVERFLOW;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* if (strcasecmp (name, "OV") == 0) */
      else if (strcasecmp (name, "C") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_CARRY;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "C") == 0) */
      else if (strcasecmp (name, "Z") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ZERO;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "Z") == 0) */
      else if (strcasecmp (name, "EQ") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ZERO;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "EQ") == 0) */
      else if (strcasecmp (name, "N") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NEGATIVE;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "N") == 0) */
      else if (strcasecmp (name, "GE") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_GREATER_OR_EQUAL;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "GE") == 0) */
      else if (strcasecmp (name, "LT") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_LESS_THAN;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "LT") == 0) */
      else if (strcasecmp (name, "GT") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_GREATER_THAN;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "GT") == 0) */
      else if (strcasecmp (name, "LE") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_LESS_OR_EQUAL;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "LE") == 0) */
      else if (strcasecmp (name, "NOV") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NOT_OVERFLOW;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "NOV") == 0) */
      else if (strcasecmp (name, "NC") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NOT_CARRY;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "NC") == 0) */
      else if (strcasecmp (name, "NZ") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NOT_ZERO;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "NZ") == 0) */
      else if (strcasecmp (name, "NEQ") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NOT_ZERO;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "NEQ") == 0) */
      else if (strcasecmp (name, "NN") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_NOT_NEGATIVE;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "NN") == 0) */
      else if (strcasecmp (name, "GEU") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_GREATER_OR_EQUAL_UNSIGNED;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "GEU") == 0) */
      else if (strcasecmp (name, "LTU") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_LESS_THAN_UNSIGNED;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "LTU") == 0) */
      else if (strcasecmp (name, "GTU") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_GREATER_THAN_UNSIGNED;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "GTU") == 0) */
      else if (strcasecmp (name, "LEU") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_LESS_OR_EQUAL_UNSIGNED;
         expr->X_op = O_BRANCH_ON_CONDITION;
      } /* else if (strcasecmp (name, "LEU") == 0) */
      else if (strcasecmp (name, "OA") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ACCUMULATOR_A_OVERFLOW;
         expr->X_op = O_BRANCH_ON_DSP_CONDITION;
      } /* else if (strcasecmp (name, "OA") == 0) */
      else if (strcasecmp (name, "OB") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ACCUMULATOR_B_OVERFLOW;
         expr->X_op = O_BRANCH_ON_DSP_CONDITION;
      } /* else if (strcasecmp (name, "OB") == 0) */
      else if (strcasecmp (name, "SA") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ACCUMULATOR_A_SATURATION;
         expr->X_op = O_BRANCH_ON_DSP_CONDITION;
      } /* else if (strcasecmp (name, "SA") == 0) */
      else if (strcasecmp (name, "SB") == 0)
      {
         expr->X_add_number = PIC30_BRANCH_ON_ACCUMULATOR_B_SATURATION;
         expr->X_op = O_BRANCH_ON_DSP_CONDITION;
      } /* else if (strcasecmp (name, "SB") == 0) */
      else
         rc = 0;
   } /* else if */

   if ((!rc) &&
       ((global_current_opcode_flags & F_IS_DSP_INSN) == F_IS_DSP_INSN))
   {
      rc = 1;

      if (strcasecmp (name, "A") == 0)
      {
         expr->X_add_number = PIC30_DSP_ACCUMULATOR_A;
         expr->X_op = O_DSP_ACCUMULATOR_SELECT;
      } /* if (strcasecmp (name, "A") == 0) */
      else if (strcasecmp (name, "B") == 0)
      {
         expr->X_add_number = PIC30_DSP_ACCUMULATOR_B;
         expr->X_op = O_DSP_ACCUMULATOR_SELECT;
      } /* if (strcasecmp (name, "B") == 0) */
      else
         rc = 0;
   } /* else if (F_IS_DSP_INSN) */

   if (flag_debug)
      fprintf (stdout, "<-- pic30_parse_name::exit(%d)\n", rc);

   return rc;
} /* int pic30_parse_name(const char *, struct expressionS *) */

/******************************************************************************/

int
pic30_do_align (n, fill, len, max)
   int n;
   const char * fill;
   int len;
   int max;

/******************************************************************************
 *
 *   GAS will call this function when the align directive is seen in the input
 *   file.  The PIC30 architecture requires special handling of alignment
 *   for both code and data sections.
 *
 *   In code sections, the lower 16 bits of program memory are filled with
 *   the last .fillvalue value specified, which is located in
 *   global_fillvalue_value, or with DEFAULT_FILL_VALUE if no .fillvalue has
 *   been specified.  The upper program memory byte is filled
 *   with the last .fillupper value specified, which is located in
 *   global_fillupper_value, or with the NOP_OPCODE if no .fillupper has been
 *   specified.  The phantom byte within program memory must be filled in with
 *   the PIC30_PHANTOM_BYTE.
 *
 *   In data sections, we just increase the alignment power by +1, to
 *   account for the fact that frags are always built in octets,
 *   which is 1/2 of an addressible unit.
 *
 *   For the dsPIC, the smallest addressible unit is a 16 bit quantity. In
 *   data memory, there is one phantom octet per addressible unit (every 2nd
 *   octet is a phantom). In program memory, there is one phantom octet per
 *   two addressible units (every 4th octet is a phantom).
 *
 *   An important note about terminology: the GNU code refers to 8 bit
 *   quantities as "octets", and the addressible units as "bytes".
 *   Unfortunately, most of the dsPIC-specific code was written under an
 *   old memory model, when "bytes" and "octets" meant the same thing.
 *   So be forewarned: in dsPIC functions, the term "byte" usually means
 *   "1 octet". In GNU functions, the term "byte" means "2 octets".
 *   
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   n                 The power of two to which to align.
 *   fill              May be NULL, or it may point to the bytes of the fill
 *                     pattern.
 *   len               The length of whatever FILL points to if anything.
 *   max               The maximum number of characters to skip when doing the
 *                     alignment, or 0 if there is no maximum.
 *
 *   RETURNS:
 *      1 - This function handled the alignment directive.
 *   
 ******************************************************************************/

{
extern relax_addressT relax_align PARAMS ((relax_addressT addr, int align));
   int rc = 0;

   if (flag_debug)
   {
      printf ("--> pic30_do_align (n=%d, fill=%s, len=%d, max=%d)::begin\n",
              n, fill ? fill : "NULL", len, max);
      printf ("    pic30_do_align::SEC_CODE = %s\n",
              ((subseg_text_p (now_seg)) ||
               (now_seg == text_section)) ? "TRUE" : "FALSE");
   } /* if (flag_debug) */

   /* If the current section is absolute, compare the requested
      alignment to the section alignment. The requested alignment
      must not be greater. */
   if (PIC30_IS_ABSOLUTE_ATTR(now_seg))
   {
     if (n > pic30_address_align_power(now_seg->lma))
       as_bad (_("Requested alignment \'%d\' is greater than alignment"
                 " of absolute section \'%s\'."), 1<<n, now_seg->name);
   }

   /* Only do this if we are in a CODE section or in the text_section */
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
   {
      unsigned char fill_pattern[PIC30_SIZE_OF_PROGRAM_WORD];
      unsigned long overflow_of_alignment =
         (global_current_location + (1 << (n+1))) % PIC30_SIZE_OF_PROGRAM_WORD;
      unsigned long number_of_bytes_to_skip =
         relax_align (global_current_location, n+1);

      if (max != 0)
      {
         int number_of_writeable_bytes = 0;
         unsigned long i;

         /*
          * Calculate the number of writeable bytes between the current location
          * and the end location of the given alignment.
          */
         for (i = global_current_location;
              i < (global_current_location + number_of_bytes_to_skip);
              i++)
         {
            if (PIC30_CAN_WRITE (i, global_p_directive_active))
               number_of_writeable_bytes++;
         } /* for i */

         if (flag_debug)
         {
            printf ("    pic30_do_align::global_current_location = %ld\n",
                    global_current_location);
            printf ("    pic30_do_align::number_of_writeable_bytes > max\n"
                    "    pic30_do_align::(%d > %d) = %s\n",
                    number_of_writeable_bytes, max,
                    (number_of_writeable_bytes > max) ? "TRUE" : "FALSE");
         } /* if (flag_debug) */

         /*
          * Determine if alignment can be completed and only skip less than
          * or equal MAX bytes.  If not, set rc = 1 to indicate that
          * we processed the alignment directive.
          */
         if (number_of_writeable_bytes > max)
         {
            if (flag_debug)
               printf ("<-- pic30_do_align::exit(1)\n");

            rc = 1;
         } /* if (number_of_writeable_bytes > max) */
         else
            max = 0;
      } /* if (max != 0) */

      if (!rc)
      {
         if (n < PIC30_CODE_ALIGNMENT)
         {
             as_bad (_("Alignment in CODE section must be at least 2 units.  "
                      "Alignment ignored."));
         } /* if (n < PIC30_CODE_ALIGNMENT) */
         else
         {
            char fill_value =
               fill ? *fill
                    : (global_p_directive_active ? global_pfillvalue_value
                                                 : global_fillvalue_value);
            char fill_upper =
               (fill && global_p_directive_active) ? *fill
                    : (global_p_directive_active ? global_pfillvalue_value
                                                 : global_fillupper_value);

            /*
             * The current fill pattern is based on what will be placed into
             * each byte of the program word.  The pattern should be:
             * byte 0 - fill_value
             * byte 1 - fill_value
             * byte 2 - fill_upper
             * byte 3 - PIC30_PHANTOM_BYTE
             *
             * NOTE:  This is TRUE for little endian.  If we were using big
             *        endian, the bytes would be reversed.
             */
            fill_pattern[0] = fill_value;
            fill_pattern[1] = fill_value;
            fill_pattern[2] = fill_upper;
            fill_pattern[3] = PIC30_PHANTOM_BYTE;

            global_current_location += number_of_bytes_to_skip;

            /*
             * If we are going to overflow the alignment (i.e., there is at
             * least a single byte in the current program word), then we need to
             * fill the remaining bytes in the current program word with the
             * values starting in the fill_pattern at overflow_of_alignment.
             */
            if (overflow_of_alignment != 0)
            {
               char * start = fill_pattern + overflow_of_alignment;
               int size_of_pattern =
                  PIC30_SIZE_OF_PROGRAM_WORD - overflow_of_alignment;

               if (flag_debug)
               {
                  int i;

                  printf ("    pic30_do_align::fill_pattern=");

                  for (i = 0; i < size_of_pattern; i++)
                     printf (" 0x%02x", start[i]);

                  printf ("\n");
               } /* if (flag_debug) */

               /*
                * Align to the next program word.
                */
               frag_align_pattern (PIC30_CODE_ALIGNMENT+1, start,
                                   size_of_pattern, max);

               number_of_bytes_to_skip -= 
                 (PIC30_SIZE_OF_PROGRAM_WORD - overflow_of_alignment);

            } /* if (overflow_of_alignment) */

            if (number_of_bytes_to_skip)
            {
              if (flag_debug)
              {
                int i;

                printf ("    pic30_do_align::fill_pattern=");

                for (i = 0; i < PIC30_SIZE_OF_PROGRAM_WORD; i++)
                  printf (" 0x%02x", fill_pattern[i]);

                printf ("\n");
              } /* if (flag_debug) */

              /*
               * Create any remaining alignment based on what was passed to us.
               * This is acceptable because we already got rid of the odd part of
               * the program word in the above if statement.
               */
              frag_align_pattern (n+1, fill_pattern, sizeof (fill_pattern), max);
            } /* if number_of_bytes_to_skip */
         } /* else */

         /*
          * Return that this function did or ignored the alignment.
          */
         rc = 1;
       } /* if (!rc) */
    } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */
#if defined(OBJ_ELF)
   else if (strncmp(now_seg->name, ".debug", 6) == 0)
   {
	   /*
	   ** Debug sections do not have explicit alignment,
	   ** therefore, this must be part of pic30_cleanup.
	   ** In any case, the debug sections should not be
	   ** aligned, since this will break the DWARF
	   ** virtual machine.
	   */
   }
#endif
   else
     {
       /*
       ** Data memory alignment
       ** This is cloned from the vanilla code in read.c:do_align()
       ** except that we apply +1 to the alignment power, since
       ** frags are always built in octets, not addressable units.
       ** We apply *2 to the max_skip value for the same reason.
       */
       if (n != 0 && !need_pass_2)
         {
           if (fill == NULL)
             frag_align (n+1, 0, max*2);
           else if (len <= 1)
             frag_align (n+1, *fill, max*2);
           else
             frag_align_pattern (n+1, fill, len, max*2);
           /*
            * Return that this function did or ignored the alignment.
            */
           rc = 1;
         }
     } /* Data memory alignment */

   if (flag_debug)
      printf ("<-- pic30_do_align::exit(%d)\n", rc);

  /*
   * Return that this function handled the alignment directive.
   */
  return rc;
} /* int pic30_do_align(int, const char *, int, int) */

/******************************************************************************/

void
pic30_handle_align (fragP)
   fragS * fragP;

/******************************************************************************
 *
 *   GAS will call this function at the end of assembly to do special handling
 *   for an alignment or org directive.  For rs_org, the fr_var should be set
 *   correctly if in the .text section.  For rs_align_code, the fill pattern
 *   must be specified.
 *
 ******************************************************************************/

{
   if (flag_debug)
   {
      printf ("--> pic30_handle_align::begin\n");
      printf ("    pic30_handle_align::line = %d\n", fragP->fr_line);
      printf ("    pic30_handle_align::type = ");
      switch (fragP->fr_type)
      {
         case rs_fill:
            printf ("rs_fill\n");
            break;

         case rs_align:
            printf ("rs_align\n");
            break;

         case rs_align_code:
            printf ("rs_align_code\n");
            break;

         case rs_org:
            printf ("rs_org\n");
            break;

         case rs_machine_dependent:
            printf ("rs_machine_dependent\n");
            break;

         case rs_space:
            printf ("rs_space\n");
            break;

         default:
            printf ("<unknown>\n");
            break;
      } /* switch (fragP->fr_type) */
   } /* if (flag_debug) */

   switch (fragP->fr_type)
   {
      case rs_org:
         if (fragP->tc_frag_data.in_text_section)
         {
            long count = fragP->fr_next->fr_address
                          - (fragP->fr_address + fragP->fr_fix);

            if (count > 0)
               fragP->fr_var = count;
            else
               fragP->fr_var = 1;
         } /* if (fragP->tc_frag_data.in_text_section) */
         break;

      case rs_align:
         {
            long count = fragP->fr_next->fr_address
                          - (fragP->fr_address + fragP->fr_fix);
            now_seg->alignment_gaps += (count / OCTETS_PER_BYTE);
         }
         break;

      case rs_align_code:
      {
         unsigned char fill_pattern[PIC30_SIZE_OF_PROGRAM_WORD];
         long start =
            (fragP->fr_address + fragP->fr_fix) % PIC30_SIZE_OF_PROGRAM_WORD;
         long count =
            fragP->fr_next->fr_address - fragP->fr_address - fragP->fr_fix;

         if (count)
         {
            long fill = fragP->tc_frag_data.p_directive_active ?
                          fragP->tc_frag_data.pfillvalue_value :
                          fragP->tc_frag_data.fillvalue_value;
            long fill_upper = fragP->tc_frag_data.p_directive_active ?
                          fragP->tc_frag_data.pfillvalue_value :
                          fragP->tc_frag_data.fillupper_value;

            fragP->fr_var = count;

            /*
             * The current fill pattern is based on what will be placed into
             * each byte of the program word.  The pattern should be:
             * byte 0 - fill saved when frag was created
             * byte 1 - fill saved when frag was created
             * byte 2 - fillupper_value saved when frag was created
             * byte 3 - PIC30_PHANTOM_BYTE
             *
             * NOTE:  This is TRUE for little endian.  If we were using big
             *        endian, the bytes would be reversed.
             */
            fill_pattern[0] = fill;
            fill_pattern[1] = fill;
            fill_pattern[2] = fill_upper;
            fill_pattern[3] = PIC30_PHANTOM_BYTE;

            memcpy(fragP->fr_literal + fragP->fr_fix, fill_pattern + start,
                   count % PIC30_SIZE_OF_PROGRAM_WORD);

            for (count -= count % PIC30_SIZE_OF_PROGRAM_WORD;
                 count; 
                 count -= ((count > PIC30_SIZE_OF_PROGRAM_WORD) ?
                           PIC30_SIZE_OF_PROGRAM_WORD : count))
               memcpy(fragP->fr_literal + fragP->fr_fix, fill_pattern,
                      ((count > PIC30_SIZE_OF_PROGRAM_WORD) ?
                       PIC30_SIZE_OF_PROGRAM_WORD : count));
         } /* if (count) */

         break;
      } /* case rs_align_code */

      default:
         break;
   } /* switch (fragP->fr_type) */

   if (fragP->fr_var == 0)
      fragP->fr_var = 1;

   if (flag_debug)
      printf ("<-- pic30_handle_align::exit\n");
} /* void pic30_handle_align(fragS *) */

/******************************************************************************
 *
 * Macros to help identify boot() and secure() operators
 *
 ******************************************************************************/
#define BOOT_ACCESS(op)   (op.X_md.modifier == PIC30_BOOT_FOUND)
#define SECURE_ACCESS(op) (op.X_md.modifier == PIC30_SECURE_FOUND)
#define BOOT_ACCESS_P(op)   (op->X_md.modifier == PIC30_BOOT_FOUND)
#define SECURE_ACCESS_P(op) (op->X_md.modifier == PIC30_SECURE_FOUND)

/******************************************************************************/

void
pic30_cons_fix_new (frag, where, nbytes, exp)
   fragS * frag;
   int where;
   int nbytes;
   expressionS * exp;

/******************************************************************************
 *
 *   GAS will call this function to generate a fixup for a data allocation
 *   pseudo-op.  It handles cases where pic30-specific operators are used
 *   with the data directive. The rest of this function is identical
 *   to the GAS code found in emit_expr.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   frag              The frag in which to generate the fixup.
 *   where             Where in the frag to generate the fixup.
 *   nbytes            The number of bytes needed for this fixup.
 *   exp               The expression for which to generate a fixup for.
 *
 ******************************************************************************/

{
   bfd_reloc_code_real_type type;

   if (flag_debug)
   {
      printf ("--> pic30_cons_fix_new (..., %d, %d, ...)::begin\n",
              where, nbytes);
      printf ("    pic30_cons_fix_new::global_p_directive_active = %s\n",
              global_p_directive_active ? "TRUE" : "FALSE");
      printf ("    pic30_cons_fix_new::X_md.modifier = %s\n",
               (exp->X_md.modifier == PIC30_TBLOFFSET_FOUND) ? "tbloffset" :
               (exp->X_md.modifier == PIC30_PSVOFFSET_FOUND) ? "psvoffset" :
               (exp->X_md.modifier == PIC30_TBLPAGE_FOUND) ? "tblpage" :
               (exp->X_md.modifier == PIC30_PSVPAGE_FOUND) ? "psvpage" :
               (exp->X_md.modifier == PIC30_HANDLE_FOUND) ? "handle" :
               (exp->X_md.modifier == PIC30_PADDR_FOUND) ? "paddr" :
               (exp->X_md.modifier == PIC30_DMAPAGE_FOUND) ? "dmapage" :
               (exp->X_md.modifier == PIC30_DMAOFFSET_FOUND) ? "dmaoffset" :
               (exp->X_md.modifier == PIC30_PSVPTR_FOUND) ? "psvptr" :
               (exp->X_md.modifier == PIC30_BOOT_FOUND) ? "boot" :
               (exp->X_md.modifier == PIC30_SECURE_FOUND) ? "secure" :
               (exp->X_md.modifier == PIC30_EDSPAGE_FOUND) ? "edspage" :
               (exp->X_md.modifier == PIC30_EDSOFFSET_FOUND) ? "edsoffset" :
               "none");
   } /* if (flag_debug) */

   switch (nbytes)
   {
      case 1:
         type = (global_p_directive_active ? BFD_RELOC_PIC30_PBYTE
                                           : BFD_RELOC_8);

         if (exp->X_md.modifier != PIC30_NO_MODIFIER_FOUND)
         {
            as_bad (_("Cannot use %s with this directive."),
               (exp->X_md.modifier == PIC30_TBLOFFSET_FOUND) ? "tbloffset" :
               (exp->X_md.modifier == PIC30_PSVOFFSET_FOUND) ? "psvoffset" :
               (exp->X_md.modifier == PIC30_TBLPAGE_FOUND) ? "tblpage" :
               (exp->X_md.modifier == PIC30_PSVPAGE_FOUND) ? "psvpage" :
               (exp->X_md.modifier == PIC30_PSVPTR_FOUND) ? "psvptr" :
               (exp->X_md.modifier == PIC30_BOOT_FOUND) ? "boot" :
               (exp->X_md.modifier == PIC30_SECURE_FOUND) ? "secure" :
               (exp->X_md.modifier == PIC30_EDSPAGE_FOUND) ? "edspage" :
               (exp->X_md.modifier == PIC30_EDSOFFSET_FOUND) ? "edsoffset" :
               (exp->X_md.modifier == PIC30_DMAPAGE_FOUND) ? "dmapage" :
               (exp->X_md.modifier == PIC30_DMAOFFSET_FOUND) ? "dmaoffset" :
               (exp->X_md.modifier == PIC30_HANDLE_FOUND) ? "handle" :
               "paddr");

            /* Make it act as a .pword or a .word directive */
            nbytes = (global_p_directive_active ||
                      (exp->X_md.modifier == PIC30_PADDR_FOUND) ? 3 : 2);
         } /* if (exp->X_md.modifier != PIC30_NO_MODIFIER_FOUND) */

         break;

      case 2:
         type = BFD_RELOC_16;

         if (exp->X_md.modifier == PIC30_PADDR_FOUND)
         {
            as_bad (_("Cannot use paddr with this directive."));

            /* Make it act as a .pword directive */
            nbytes = 3;
         } /* if (exp->X_md.modifier == PIC30_PADDR_FOUND) */

         break;

      case 3:
         type = BFD_RELOC_PIC30_PWORD;
         break;

      case 4:
         type = BFD_RELOC_32;
         break;

#ifdef BFD64
      case 8:
         type = BFD_RELOC_64;
         break;
#endif

      default:
         as_bad (_("Unsupported BFD relocation size %u"), nbytes);
         type = BFD_RELOC_32;
         break;
   } /* switch (nbytes) */

   if (nbytes >= 2)
   {
      switch (exp->X_md.modifier)
      {
         case PIC30_TBLOFFSET_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_TBLOFFSET
                                              : BFD_RELOC_PIC30_TBLOFFSET);
            break;

         case PIC30_PSVOFFSET_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_PSVOFFSET
                                              : BFD_RELOC_PIC30_PSVOFFSET);
            break;

         case PIC30_TBLPAGE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_TBLPAGE
                                              : BFD_RELOC_PIC30_TBLPAGE);
            break;

         case PIC30_PSVPAGE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_PSVPAGE
                                              : BFD_RELOC_PIC30_PSVPAGE);
            break;

         case PIC30_HANDLE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_HANDLE
                                              : BFD_RELOC_PIC30_HANDLE);
            break;

         case PIC30_PADDR_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_PADDR
                                              : BFD_RELOC_PIC30_PADDR);
            break;

         case PIC30_DMAOFFSET_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_DMAOFFSET
                                              : BFD_RELOC_PIC30_DMAOFFSET);
            break;
         case PIC30_DMAPAGE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_DMAPAGE
                                              : BFD_RELOC_PIC30_DMAPAGE);
            break;

         case PIC30_PSVPTR_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_PSVPTR
                                              : (nbytes == 4) ? BFD_RELOC_PIC30_L_PSVPTR
                                              : BFD_RELOC_PIC30_PSVPTR);
            break;

         case PIC30_BOOT_FOUND:
         case PIC30_SECURE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_ACCESS
                                              : (nbytes == 4) ? BFD_RELOC_PIC30_L_ACCESS
                                              : BFD_RELOC_PIC30_ACCESS);
              break;

         case PIC30_EDSPAGE_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_EDSPAGE
                                              : BFD_RELOC_PIC30_EDSPAGE);
            break;

         case PIC30_EDSOFFSET_FOUND:
            type = (global_p_directive_active ? BFD_RELOC_PIC30_P_EDSOFFSET
                                              : BFD_RELOC_PIC30_EDSOFFSET);
            break;

         default:
            break;
      } /* switch (exp->X_md.modifier) */
   } /* if (nbytes >= 2) */

   fix_new_exp (frag, where, nbytes, exp, FALSE, type);

   if (flag_debug)
   {
      printf ("    pic30_cons_fix_new::reloc type = %s\n",
              bfd_get_reloc_code_name (type));
      printf ("<-- pic30_cons_fix_new::exit\n");
   } /* if (flag_debug) */
} /* void pic30_cons_fix_new(fragS *, int, int, expressionS *) */

/******************************************************************************/

void
md_operand (operand)
   expressionS * operand;

/******************************************************************************
 *
 *   This function is called by GAS for any expression that cannot be
 *   recognized. When the function is called, input_line_pointer will point to
 *   the start of the expression
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   operand           The expressionS structure that needs to be filled in
 *                     with the contents of the unrecognized expression.
 *
 ******************************************************************************/

{
   char * search_location;
   char * tmp_expression = xmalloc (strlen (input_line_pointer) + 1);
   strcpy (tmp_expression, input_line_pointer);

   if (flag_debug)
   {
      fprintf (stdout, "--> md_operand::begin\n");
      fprintf (stdout, "    md_operand::input_line_pointer = %s\n",
               input_line_pointer);
   } /* if (flag_debug) */

   if (input_line_pointer[0] == '[')
   {
      char * comma_location = strchr (input_line_pointer, ',');
      if (comma_location != NULL)
         comma_location[0] = '\0';

      input_line_pointer++; /* skip '[' */

      if ((strchr (input_line_pointer, '+') == NULL) &&
          (strchr (input_line_pointer, '-') == NULL))
      {
         expression (operand);

         if (operand->X_op == O_register)
         {
            operand->X_op = O_REGISTER_INDIRECT;

            if (input_line_pointer[0] == ']')
               input_line_pointer++; /* skip ']' */
            else
               as_warn (_("Missing ']' assumed."));
         } /* if (operand->X_op == O_register) */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid register in expression %s"), tmp_expression);
         } /* else */
      } /* if */
      else if ((search_location = strstr (input_line_pointer, "]+=")) != NULL)
      {
         /*
          * Clear ]+ character so expression does not think it is an
          * addition sign.
          */
         *search_location = '\0';

         expression (operand);

         if ((operand->X_op == O_register) &&
             (input_line_pointer == search_location))
         {
            expressionS offset;

            input_line_pointer += 3; /* skip "]+=" */

            expression (&offset);
            if (offset.X_op == O_constant)
            {
               if ((offset.X_add_number == 2)  ||
                   (offset.X_add_number == 4)  ||
                   (offset.X_add_number == 6)  ||
                   (offset.X_add_number == -2) ||
                   (offset.X_add_number == -4) ||
                   (offset.X_add_number == -6))
               {
                  operand->X_add_symbol = make_expr_symbol (operand);
                  operand->X_op_symbol = make_expr_symbol (&offset);
                  operand->X_add_number = 0;
                  operand->X_op = O_REGISTER_POST_INCREMENT_BY_N;
               } /* if */
               else
               {
                  operand->X_op = O_illegal;
                  as_bad (_("Invalid post increment value.  "
                            "Must be +/- 2, 4, or 6"));
               } /* else */
            } /* if (offset.X_op == O_constant) */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid post increment value.  "
                         "Expecting a constant +/- 2, 4, or 6"));
            } /* else */
         } /* if */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid register in expression %s"), tmp_expression);
         } /* else */
      }
      else if ((search_location = strstr (input_line_pointer, "]-=")) != NULL)
      {
         /*
          * Clear ]- character so expression does not think it is an
          * subtraction sign.
          */
         *search_location = '\0';

         expression (operand);

         if ((operand->X_op == O_register) &&
             (input_line_pointer == search_location))
         {
            expressionS offset;

            input_line_pointer += 3; /* skip "]-=" */

            expression (&offset);
            if (offset.X_op == O_constant)
            {
               if ((offset.X_add_number == 2)  ||
                   (offset.X_add_number == 4)  ||
                   (offset.X_add_number == 6)  ||
                   (offset.X_add_number == -2) ||
                   (offset.X_add_number == -4) ||
                   (offset.X_add_number == -6))
               {
                  operand->X_add_symbol = make_expr_symbol (operand);
                  operand->X_op_symbol = make_expr_symbol (&offset);
                  operand->X_add_number = 0;
                  operand->X_op = O_REGISTER_POST_DECREMENT_BY_N;
               } /* if */
               else
               {
                  operand->X_op = O_illegal;
                  as_bad (_("Invalid post decrement value.  "
                            "Must be +/- 2, 4, or 6"));
               } /* else */
            } /* if (offset.X_op == O_constant) */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid post decrement value.  "
                         "Expecting a constant +/- 2, 4, or 6"));
            } /* else */
         } /* if */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid register in expression %s"), tmp_expression);
         } /* else */
      }
      else if ((search_location = strstr (input_line_pointer, "++")) != NULL)
      {
         if (search_location == input_line_pointer)
         {
            /*
             * Skip over the ++ characters.
             */
            input_line_pointer += 2;

            expression (operand);

            if ((operand->X_op == O_register) && (*input_line_pointer == ']'))
            {
               operand->X_op = O_REGISTER_PRE_INCREMENT;
               input_line_pointer++; /* skip "]" */
            } /* if */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid use of ++ in operand %s"), tmp_expression);
            } /* else */
         } /* if (search_location == input_line_pointer) */
         else if (search_location[2] == ']')
         {
            /* Post-increment:  "++]" found. */

            /*
             * Clear + character so expression does not think it is an
             * addition sign.
             */
            *search_location = '\0';

            expression (operand);

            if ((operand->X_op == O_register) &&
                (input_line_pointer == search_location))
            {
               operand->X_op = O_REGISTER_POST_INCREMENT;
               input_line_pointer += 3; /* skip "++]" */
            } /* if */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid register in operand %s"), tmp_expression);
            } /* else */
         } /* else if (search_location[2] == ']') */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid use of ++ in operand %s."), tmp_expression);
         }
      } /* else if (strstr (input_line_pointer, "++") != NULL) */
      else if ((search_location = strstr (input_line_pointer, "--")) != NULL)
      {
         if (search_location == input_line_pointer)
         {
            /*
             * Skip over the -- characters.
             */
            input_line_pointer += 2;

            expression (operand);

            if ((operand->X_op == O_register) && (*input_line_pointer == ']'))
            {
               operand->X_op = O_REGISTER_PRE_DECREMENT;
               input_line_pointer++; /* skip "]" */
            } /* if */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid use of -- in operand %s"), tmp_expression);
            } /* else */
         } /* if (search_location == input_line_pointer) */
         else if (search_location[2] == ']')
         {
            /* Post-decrement:  "--]" found. */

            /*
             * Clear - character so expression does not think it is a
             * subtraction sign.
             */
            *search_location = '\0';

            expression (operand);

            if ((operand->X_op == O_register) &&
                (input_line_pointer == search_location))
            {
               operand->X_op = O_REGISTER_POST_DECREMENT;
               input_line_pointer += 3; /* skip "--]" */
            } /* if */
            else
            {
               operand->X_op = O_illegal;
               as_bad (_("Invalid register in operand %s"), tmp_expression);
            } /* else */
         } /* else if (search_location[2] == ']') */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid use of -- in operand %s."), tmp_expression);
         }
      } /* else if (strstr (input_line_pointer, "--") != NULL) */
      else
      {
         expression (operand);
         if (flag_debug)
           {
             printf ("    md_operand::operand = ");
             print_expression (operand);
             printf ("\n");
           }

         if ((operand->X_op == O_add) ||
             (operand->X_op == O_subtract))
         {
            if ((operand->X_add_symbol != NULL) &&
                (operand->X_op_symbol != NULL))
            {
               expressionS * left =
                  symbol_get_value_expression (operand->X_add_symbol);
               /*
               ** The next operation loses some key info about the
               ** right-hand expression. We'll have to test
               ** operand->X_op_symbol->bsym->section below.
               */
               expressionS * right =
                  symbol_get_value_expression (operand->X_op_symbol);
               if (flag_debug)
               {
                  printf ("    md_operand::left  = ");
                  print_expression (left);
                  printf ("\n"
                          "    md_operand::right = ");
                  print_expression (right);
                  printf ("\n"
                          "    md_operand::addend = %ld\n", operand->X_add_number);
               } // if (flag_debug)

               if ((left->X_op == O_register) &&
                   (right->X_op == O_register) &&
                   (operand->X_add_number == 0))
               {
                  if (operand->X_op == O_add)
                     operand->X_op = O_REGISTER_WITH_DISPLACEMENT;
                  else
                  {
                     operand->X_op = O_illegal;
                     as_bad (_("Only support plus register displacement "
                               "(i.e., [Wb+Wn])."));
                  } /* else */
               } /* if */
               else if ((left->X_op == O_register) &&
                        (right->X_op == O_constant) &&
                        (operand->X_op_symbol->bsym->section == absolute_section))
               {
                  if (operand->X_op == O_add)
                     operand->X_op = O_REGISTER_PLUS_LITERAL;
                  else
                     operand->X_op = O_REGISTER_MINUS_LITERAL;

                  /* If an extra addend was provided,
                     combine it with the right-hand constant. */
                  if (operand->X_add_number != 0)
                    {
                      right->X_add_number += operand->X_add_number;
                      operand->X_add_number = 0;
                    }

                  if (global_current_opcode_flags & F_WORD)
                  {
                     if (!PIC30_IS_EVEN (right->X_add_number))
                     {
                        operand->X_op = O_illegal;
                        as_bad (_("Word operations expect even offset."));
                     } /* if (!PIC30_IS_EVEN (right->X_add_number)) */
                     else if (((operand->X_op == O_REGISTER_MINUS_LITERAL) &&
                               ((right->X_add_number > 1024) ||
                                (right->X_add_number < -1022))) ||
                              ((operand->X_op == O_REGISTER_PLUS_LITERAL) &&
                               ((right->X_add_number > 1022) ||
                                (right->X_add_number < -1024))))
                     {
                        operand->X_op = O_illegal;
                        as_bad (_("Word operations expect an even offset "
                                  "between -1024 and 1022."));
                     } /* else if (right->X_add_number <= -1024 || >= 1022) */
                     else
                        right->X_add_number = right->X_add_number / 2;
                  } /* if (global_current_opcode_flags & F_WORD) */
                  else
                  {
                     if (((operand->X_op == O_REGISTER_MINUS_LITERAL) &&
                          ((right->X_add_number > 512) ||
                           (right->X_add_number < -511))) ||
                         ((operand->X_op == O_REGISTER_PLUS_LITERAL) &&
                          ((right->X_add_number > 511) ||
                           (right->X_add_number < -512))))
                     {
                        operand->X_op = O_illegal;
                        as_bad (_("Byte operations expect an offset "
                                  "between -512 and 511."));
                     } /* else if (right->X_add_number <= -512 || >= 511) */
                  } /* else */

                  /*
                   * If this is a subtract operation, then we need to set
                   * the constant value to a negative.  This mode expects
                   * a 10-bit signed literal.
                   */
                  if (operand->X_op == O_REGISTER_MINUS_LITERAL)
                     right->X_add_number *= -1;
               } /* else if */
               else
               {
                  operand->X_op = O_illegal;
                  if (left->X_op != O_register)
                     as_bad (_("Register expected as first operand of "
                               "expression %s."), tmp_expression);
                  else
                     as_bad (_("Register or constant literal expected as "
                               "second operand of expression %s."),
                             tmp_expression);
               } /* else */
            } /* if */
         } /* if ((operand->X_op == O_add) || (operand->X_op == O_subtract)) */
         else
         {
            operand->X_op = O_illegal;
            as_bad (_("Invalid expression (%s) contained inside of the "
                      "brackets."), tmp_expression);
         } /* else */

         if (input_line_pointer[0] == ']')
            input_line_pointer++; /* skip ']' */
         else
            as_warn (_("Missing ']' assumed."));
      } /* else */

      if (comma_location != NULL)
         comma_location[0] = ',';
   } /* if (input_line_pointer[0] == '[') */
   else if (input_line_pointer[0] == '#')
   {
      input_line_pointer++;
      SKIP_WHITESPACE ();
      expression (operand);
      operand->X_md.immediate = TRUE;
   } /* else if (input_line_pointer[0] == '#') */

   free (tmp_expression);

   if (flag_debug)
      printf ("<-- md_operand::exit\n");
} /* void md_operand(expressionS *) */

/******************************************************************************/

long
pic30_apply_operator (type, value, applying_fixups)
   bfd_reloc_code_real_type type;
   long value;
   unsigned char applying_fixups;

/******************************************************************************
 *
 *   This function is called to apply any special operator to the value.  If
 *   fixups are being applied, the PSV offset should not set the upper bit.
 *   Note that paddr() and dmaoffset() do not require any special processing.
 *
 *   RETURNS:
 *      The new value after the operator has been applied.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_apply_operator (%s, %ld, %s)::begin\n",
              bfd_get_reloc_code_name (type), value,
              applying_fixups ? "TRUE" : "FALSE");

   switch (type)
   {
      case BFD_RELOC_PIC30_TBLOFFSET:
      case BFD_RELOC_PIC30_P_TBLOFFSET:
      case BFD_RELOC_PIC30_WORD_TBLOFFSET:
      case BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET:
         value = PIC30_TBLOFFSET (value);
         break;

      case BFD_RELOC_PIC30_PSVOFFSET:
      case BFD_RELOC_PIC30_P_PSVOFFSET:
      case BFD_RELOC_PIC30_WORD_PSVOFFSET:
      case BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET:
         if (applying_fixups)
         {
            /* This is a special case of applying the PSVOFFSET operator.  We
               do not use
               value = PIC30_PSVOFFSET (value);
               because we do not want to set the upper bit.  This function is
               called when applying fixups for the addend.  The addend should
               never get the upper bit set.  If we did, this would confuse the
               linker's calculation. */
            value &= 0xFFFF;
         } /* if (applying_fixups) */
         else
            value = PIC30_PSVOFFSET (value);

         break;

      case BFD_RELOC_PIC30_PSVPTR:
      case BFD_RELOC_PIC30_P_PSVPTR:
      case BFD_RELOC_PIC30_L_PSVPTR:
      case BFD_RELOC_PIC30_WORD_PSVPTR:
      case BFD_RELOC_PIC30_UNSIGNED_10_PSVPTR:
         if (applying_fixups)
         {
            /* do nothing; let the linker do its thing */
         } /* if (applying_fixups) */
         else
            value = PIC30_PSVPTR(value);

         break;

      case BFD_RELOC_PIC30_TBLPAGE:
      case BFD_RELOC_PIC30_P_TBLPAGE:
      case BFD_RELOC_PIC30_WORD_TBLPAGE:
      case BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE:
         value = PIC30_TBLPAGE (value);
         break;

      case BFD_RELOC_PIC30_PSVPAGE:
      case BFD_RELOC_PIC30_P_PSVPAGE:
      case BFD_RELOC_PIC30_WORD_PSVPAGE:
      case BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE:
         value = PIC30_PSVPAGE (value);
         break;

      case BFD_RELOC_PIC30_HANDLE:
      case BFD_RELOC_PIC30_P_HANDLE:
      case BFD_RELOC_PIC30_WORD_HANDLE:
      case BFD_RELOC_PIC30_UNSIGNED_10_HANDLE:
         value = PIC30_HANDLE (value);
         break;

      default:
         break;
   } /* switch (type) */

   if (flag_debug)
      printf ("<-- pic30_apply_operator::exit(%ld)\n", value);

   return value;
}

/******************************************************************************/

void
pic30_expr (expr_to_populate, data_allocation)
   expressionS * expr_to_populate;
   unsigned char data_allocation;

/******************************************************************************
 *
 *   This function will parse an expression.  It specifically knows how to
 *   deal with #immediate syntax, operator syntax, and Wm*Wn.
 *
 *   Also includes special processing for the boot(), secure() operators.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   expr_to_populate  The expressionS structure that needs to be filled in.
 *
 ******************************************************************************/

/* macro for parsing pic30-specific operators such as psvpage(), etc */
#define PARSE_OPERATOR(name,type)                                     \
   if (expr_to_populate->X_md.modifier == PIC30_NO_MODIFIER_FOUND)    \
   {                                                                  \
      modifier = strstr (input_line_pointer, name);                   \
      if (modifier)                                                   \
      {                                                               \
         int whitespace_skipped = 0;                                  \
                                                                      \
         end_of_modifier = modifier + strlen (name);                  \
         while ((*end_of_modifier == ' ') ||                          \
                (*end_of_modifier == '\t'))                           \
         {                                                            \
            end_of_modifier++;                                        \
            whitespace_skipped++;                                     \
         } /* while */                                                \
                                                                      \
         if ((*end_of_modifier == '(') &&                             \
             ((modifier == input_line_pointer) ||                     \
              (modifier[-1] == '#') ||                                \
              (modifier[-1] == ' ') ||                                \
              (modifier[-1] == '\t')))                                \
         {                                                            \
            expr_to_populate->X_md.modifier = type;                   \
         } /* if */                                                   \
      } /* if (modifier) */                                           \
   }                                                                  \

#define BOOT_OR_SECURE  ((expr_to_populate->X_md.modifier ==          \
                         PIC30_BOOT_FOUND) ||                         \
                         (expr_to_populate->X_md.modifier ==          \
                         PIC30_SECURE_FOUND))                         \

{
   char * comma_location = strstr (input_line_pointer, ",");
   char * newline_location = strstr (input_line_pointer, "\n");
   char * close_paren_location = strstr (input_line_pointer, ")");
   char   after_paren = '\0';
   char * modifier = NULL;
   char * end_of_modifier = NULL;
   char * pound_sign_location = NULL;

   if (flag_debug)
      printf ("--> pic30_expr (..., %s)::begin\n",
              data_allocation ? "TRUE" : "FALSE");

   expr_to_populate->X_md.modifier = PIC30_NO_MODIFIER_FOUND;
   expr_to_populate->X_md.immediate = FALSE;

   /*
    * Make the comma the end-of-line so that we are only searching the operand
    * in question and not the whole string.
    */
   if (comma_location)
      comma_location[0] = '\0';

   /*
    * Make the first newline the end-of-line so that we are only searching the
    * current line and not the whole buffer passed to us.  This is different
    * then the comma_location in that if the current line only has a single
    * operand, then there will be no comma; however, a comma may have been
    * seen in another line in the buffer.
    */
   if (newline_location)
      newline_location[0] = '\0';

   /*
    * Look for large negative constants in .long directives. The expression
    * parser does not detect if significant bits will be lost.
    */
   if (global_long_directive_active) {
     errno = 0;
     if (*input_line_pointer == '-')
       (void) strtol(input_line_pointer,0,0);
     else
       (void) strtoul(input_line_pointer,0,0);
     if (errno == ERANGE) {
       as_warn (_("Overflow/ underflow for .long may lose significant bits."));
     }
   }

   /*
    * If there is a pound sign in the operand, then we may need to do special
    * processing below.  Set up the pointer.
    */
   pound_sign_location = strstr (input_line_pointer, "#");

   /*
    * Check for syntax errors that are difficult to detect later.
    */
   if ((*input_line_pointer == '+') || (*input_line_pointer == '-'))
     {
       char c = * (input_line_pointer + 1);
       if ((c == '+') || (c == '-') || (c == '[') || (c == ']') )
         as_bad (_("Invalid operand syntax ('%s')."), input_line_pointer);
     }

   /* parse and record any pic30-specific operators */
   PARSE_OPERATOR("tbloffset", PIC30_TBLOFFSET_FOUND);
   PARSE_OPERATOR("tblpage", PIC30_TBLPAGE_FOUND);
   PARSE_OPERATOR("psvoffset", PIC30_PSVOFFSET_FOUND);
   PARSE_OPERATOR("psvpage", PIC30_PSVPAGE_FOUND);
   PARSE_OPERATOR("handle", PIC30_HANDLE_FOUND);
   PARSE_OPERATOR("paddr", PIC30_PADDR_FOUND);
   PARSE_OPERATOR("psvptr", PIC30_PSVPTR_FOUND);
   PARSE_OPERATOR("dmapage", PIC30_DMAPAGE_FOUND);
   PARSE_OPERATOR("dmaoffset", PIC30_DMAOFFSET_FOUND);
   PARSE_OPERATOR("boot", PIC30_BOOT_FOUND);
   PARSE_OPERATOR("secure", PIC30_SECURE_FOUND);
   PARSE_OPERATOR("edspage", PIC30_EDSPAGE_FOUND);
   PARSE_OPERATOR("edsoffset", PIC30_EDSOFFSET_FOUND);

   /* Reset comma */
   if (comma_location)
      comma_location[0] = ',';

   /* Reset newline */
   if (newline_location)
      newline_location[0] = '\n';

   if (expr_to_populate->X_md.modifier == PIC30_PSVPTR_FOUND)
     as_bad(_("psvptr() is obsolete. Please re-build this file."));

   if (expr_to_populate->X_md.modifier != PIC30_NO_MODIFIER_FOUND)
   {
      /*
       * Advance the input_line_pointer to just after the modifier and
       * any trailing whitespace.  Note this also skips any leading characters
       * found before the modifier (i.e., #).
       */
      input_line_pointer += (end_of_modifier - input_line_pointer);

      /*
       * Mark this as an immediate value since we skipped the # sign above.
       */
      if (pound_sign_location)
         expr_to_populate->X_md.immediate = TRUE;
      else if (!data_allocation && !BOOT_OR_SECURE)
      {
        /*
        ** Don't include boot() or secure() in this message
        ** because they can be used as GOTO, CALL targets
        ** without the # sign
        */
         as_bad (_("%s modifier must be preceded by a #."),
                 (expr_to_populate->X_md.modifier ==
                  PIC30_TBLOFFSET_FOUND) ? "tbloffset" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_PSVOFFSET_FOUND) ? "psvoffset" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_TBLPAGE_FOUND) ? "tblpage" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_PSVPAGE_FOUND) ? "psvpage" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_PSVPTR_FOUND) ? "psvptr" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_EDSPAGE_FOUND) ? "edspage" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_EDSOFFSET_FOUND) ? "edsoffset" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_DMAPAGE_FOUND) ? "dmapage" :
                 (expr_to_populate->X_md.modifier ==
                  PIC30_DMAOFFSET_FOUND) ? "dmaoffset" : "handle");
      } /* else if (!data_allocation) */

      if ((expr_to_populate->X_md.modifier == PIC30_PADDR_FOUND) &&
          (!data_allocation))
         as_bad (_("paddr modifier not allowed in instruction."));
   } /* if (expr_to_populate->X_md.modifier != PIC30_NO_MODIFIER_FOUND) */

   if (data_allocation && pound_sign_location)
      as_bad(_("# sign not valid in data allocation directive."));

   if (BOOT_OR_SECURE && close_paren_location) {

     /* flag an error if arg missing */
     if ((input_line_pointer[0] == '(') && 
         (input_line_pointer[1] == ')')) {
       as_bad(_("%s() requires an argument"),
              BOOT_ACCESS_P(expr_to_populate) ? "boot" : "secure" );
       input_line_pointer +=2;
     }
     else {
       /*
       ** These operators can't be used in expressions,
       ** so terminate the operand after the close_paren.
       ** This helps with error checking.
       */
       after_paren = close_paren_location[1];
       close_paren_location[1] = '\0';
       expression (expr_to_populate);
       close_paren_location[1] = after_paren;
     }
   } /* if (BOOT_OR_SECURE && close_paren_location) */
   else
     expression (expr_to_populate);

   /*
    * Check to see if the operand is a special DSP operand Wm*Wn.
    */
   if (expr_to_populate->X_op == O_multiply)
   {
      expressionS * left =
         symbol_get_value_expression (expr_to_populate->X_add_symbol);
      expressionS * right =
         symbol_get_value_expression (expr_to_populate->X_op_symbol);
      expressionS * temp;

      if ((left->X_op == O_register) && (right->X_op == O_register))
      {
         if (left->X_add_number == right->X_add_number)
         {
            if ((left->X_add_number < 4) || (left->X_add_number > 7))
              as_bad (_("DSP square operand register must be in range W4..W7"
                        " (was W%d)"), (int) left->X_add_number);
            expr_to_populate->X_op = O_DSP_SQUARE;
            expr_to_populate->X_add_number = left->X_add_number;
         } /* if (left->X_add_number == right->X_add_number) */
         else
         {
            expr_to_populate->X_op = O_DSP_MULTIPLICAND;

            if (left->X_add_number > right->X_add_number)
              {
                temp = left;  /* invert the register order */
                left = right;
                right = temp;
              }
            switch (left->X_add_number)
            {
               case 4:
                  switch (right->X_add_number)
                  {
                     case 5:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W4_W5_MULTIPLICAND;
                        break;

                     case 6:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W4_W6_MULTIPLICAND;
                        break;

                     case 7:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W4_W7_MULTIPLICAND;
                        break;

                     default:
                        expr_to_populate->X_op = O_illegal;
                        break;
                  } /* switch (right->X_add_number) */

                  break;

               case 5:
                  switch (right->X_add_number)
                  {
                     case 6:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W5_W6_MULTIPLICAND;
                        break;

                     case 7:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W5_W7_MULTIPLICAND;
                        break;

                     default:
                        expr_to_populate->X_op = O_illegal;
                        break;
                  } /* switch (right->X_add_number) */

                  break;

               case 6:
                  switch (right->X_add_number)
                  {
                     case 7:
                        expr_to_populate->X_add_number =
                           PIC30_DSP_W6_W7_MULTIPLICAND;
                        break;

                     default:
                        expr_to_populate->X_op = O_illegal;
                        break;
                  } /* switch (right->X_add_number) */

                  break;

               default:
                  expr_to_populate->X_op = O_illegal;
                  break;
            } /* switch (left->X_add_number) */
         } /* else */
      } /* if ((left->X_op == O_register) && (right->X_op == O_register)) */
   } /* if (expr_to_populate->X_op == O_multiply) */

     /* argument to boot() or secure() can not be a symbol */
   if ((expr_to_populate->X_add_symbol != NULL) && BOOT_OR_SECURE)
     as_bad(_("Argument to %s() cannot be a symbol"),
            BOOT_ACCESS_P(expr_to_populate) ? "boot" : "secure" );

   /* 
   ** Process operators where constants have special meanings
   */
   if ((expr_to_populate->X_op == O_constant) && BOOT_OR_SECURE) {

     offsetT value = expr_to_populate->X_add_number;
     symbolS *symbolp;

     /* range check the constant arg */
     if (value < 0 || value == 16 || value > 31) {
       as_bad (_("Argument to %s() must be in range 0..15 "
                 "or 17..31"),
               BOOT_ACCESS_P(expr_to_populate) ? "boot" : "secure" );
       expr_to_populate->X_op = O_illegal;
     }
     else {
       /*
       ** Create a target symbol that reflects
       ** the operator and its argument
       */
       const char *s1 = "__boot_entry:";
       const char *s2 = "__secure_entry:";
       char *s = (char *) xmalloc (4 + strlen (s2) + 1);

       sprintf(s, "%s%d", BOOT_ACCESS_P(expr_to_populate) ? s1 : s2, (int) value);
       if (flag_debug)
         fprintf (stdout, "    pic30_expr::%s\n", s);

       /* create a symbol unless it already exists */
       symbolp = symbol_find(s);
       if (!symbolp)
         symbolp = symbol_new(s, undefined_section, 0, &zero_address_frag);

       /* update the expr structure */
       expr_to_populate->X_add_symbol = symbolp;
       expr_to_populate->X_add_number = 0;
       expr_to_populate->X_op = O_symbol;

     } /* else */
   } /* if ((expr_to_populate->X_op == O_constant) && BOOT_OR_SECURE) */

   /*
   ** Process constants in data directives that may be modified by operators
   ** such as tbloffset(), etc.
   */
   if ((expr_to_populate->X_op == O_constant) &&
            (data_allocation) &&
            (expr_to_populate->X_md.modifier != PIC30_NO_MODIFIER_FOUND))
   {
      bfd_reloc_code_real_type type;

      switch (expr_to_populate->X_md.modifier)
        {
        case PIC30_TBLOFFSET_FOUND: type = BFD_RELOC_PIC30_TBLOFFSET;
          break;
        case PIC30_PSVOFFSET_FOUND: type = BFD_RELOC_PIC30_PSVOFFSET;
          break;
        case PIC30_TBLPAGE_FOUND:   type = BFD_RELOC_PIC30_TBLPAGE;
          break;
        case PIC30_PSVPAGE_FOUND:   type = BFD_RELOC_PIC30_PSVPAGE;
          break;
        case PIC30_PSVPTR_FOUND:    type = BFD_RELOC_PIC30_PSVPTR;
          break;
        case PIC30_HANDLE_FOUND:    type = BFD_RELOC_PIC30_HANDLE;
          break;
        case PIC30_DMAOFFSET_FOUND: type = BFD_RELOC_PIC30_DMAOFFSET;
          break;
        case PIC30_PADDR_FOUND: type = BFD_RELOC_PIC30_PADDR;
          break;
        default:
          /* some operators don't map directly to reloc types */
          type = BFD_RELOC_UNUSED;
        }

      /* test for operators that don't accept constants */
      switch (type)
        {
        case BFD_RELOC_PIC30_DMAOFFSET:
          as_bad (_("Cannot use a constant as the argument of dmaoffset."));
          expr_to_populate->X_add_number = 0;

        default: break;
        }

      expr_to_populate->X_add_number =
         pic30_apply_operator (type, expr_to_populate->X_add_number, FALSE);

      expr_to_populate->X_md.modifier = PIC30_NO_MODIFIER_FOUND;
   } /* else if */

   if (flag_debug)
      printf ("<-- pic30_expr::exit\n");
} /* void pic30_expr(expressionS *) */

/******************************************************************************/

short
pic30_parse_operands (operands)
   expressionS operands[];

/******************************************************************************
 *
 *   This function will parse the operands in the currently assembly statement
 *   and populate the operands array.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   operands          An array of expressionS to populate for each
 *                     operand found.
 *
 *   RETURNS:
 *      The number of operands found or a -1 if an illegal operand was found.
 *
 ******************************************************************************/

{
   short operand_count = 0;

   if (flag_debug)
      printf ("--> pic30_parse_operands::begin\n");

   while (TRUE)
   {
      global_current_operand_position = operand_count + 1;

      /* Initialize current operand */
      memset (&(operands[operand_count]), 0, sizeof (expressionS));
      operands[operand_count].X_md.modifier = PIC30_NO_MODIFIER_FOUND;
      operands[operand_count].X_md.immediate = FALSE;

      pic30_expr (&(operands[operand_count]), FALSE);

      /*
       * If we did not find an operand or it was illegal syntax, break out of
       * the loop.  Set operand_count to -1 if illegal syntax was found.
       * 
       * NOTE:  (operand_count = -1) is supposed to be a single equal sign!!!!
       */
      if ((operands[operand_count].X_op == O_absent) ||
          ((operands[operand_count].X_op == O_illegal) &&
           (operand_count = -1)))
         break;

      /*
       * If debugging, then print out the expression that was just found.
       */
      if (flag_debug)
      {
         printf ("    pic30_parse_operands::operands[%d] = ", operand_count);
         print_expression (&(operands[operand_count]));
         printf ("\n");
      } /* if (flag_debug) */

      /*
       * Skip the whitespace in input_line_pointer to move to the next operand
       */
      SKIP_WHITESPACE ();

      /*
       * Skip the comma separating the operands if it exists.
       */
      if (*input_line_pointer == ',')
         input_line_pointer++;
      else if (*input_line_pointer != '\0')
         as_warn (_("Missing , assumed."));

      /*
       * Increment the operand count.
       */
      operand_count++;

      if (operand_count >= MAX_OPERANDS)
      {
         if (*input_line_pointer != '\0')
         {
            as_bad (_("Number of operands exceeds maximum number of %d"),
                   MAX_OPERANDS);
            operand_count = -2;
         } /* if (*input_line_pointer != '\0') */
         break;
      } /* if (operand_count >= MAX_OPERANDS) */
   } /* while */

   demand_empty_rest_of_line ();

   if (flag_debug)
      printf ("<-- pic30_parse_operands::exit(%d)\n", operand_count);

   return operand_count;
} /* short pic30_parse_operands(expressionS[]) */

/******************************************************************************/

static unsigned char
pic30_populate_operand_value (expression, operand_value, operand_type)
   const expressionS * expression;
   struct pic30_operand_value * operand_value;
   unsigned long operand_type;

/******************************************************************************
 *
 *   This function will populate the pic30_operand_value structure based on
 *   the information contained in the expressionS *.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   expression        The expression from which to get the information to
 *                     populate the operand_value.
 *   operand_value     Populated with information based on the expression.
 *
 *   RETURNS:
 *      0 - Could not populate the operand_value.
 *      1 - operand_value was populated.
 *
 ******************************************************************************/

{
   unsigned char rc = TRUE;

   if (flag_debug)
      printf ("--> pic30_populate_operand_value::begin\n");

   operand_value->type = pic30_operator_mask[expression->X_op];
   operand_value->modifier = expression->X_md.modifier;

   switch (expression->X_op)
   {
      case O_constant:
      case O_register:
      case O_REGISTER_INDIRECT:
      case O_REGISTER_POST_DECREMENT:
      case O_REGISTER_POST_INCREMENT:
      case O_REGISTER_PRE_DECREMENT:
      case O_REGISTER_PRE_INCREMENT:
      case O_BRANCH_ON_DSP_CONDITION:
      case O_BRANCH_ON_CONDITION:
      case O_DSP_ACCUMULATOR_SELECT:
      case O_DSP_MULTIPLICAND:
      case O_DSP_SQUARE:
         operand_value->value = expression->X_add_number;
         break;

      case O_REGISTER_WITH_DISPLACEMENT:
      case O_REGISTER_PLUS_LITERAL:
      case O_REGISTER_MINUS_LITERAL:
      case O_REGISTER_POST_INCREMENT_BY_N:
      case O_REGISTER_POST_DECREMENT_BY_N:
         operand_value->value = S_GET_VALUE (expression->X_add_symbol);
         operand_value->optional_value =
            S_GET_VALUE (expression->X_op_symbol);
         break;

      case O_symbol:
 	 if (operand_type & OPND_PAGE_SYMBOL) {
	   operand_value->value = 
             (long int)expression->X_add_symbol->bsym->name;
           operand_value->type = OPND_PAGE_SYMBOL;
         } else rc = FALSE;
         break;
      default:
         rc = FALSE;
   } /* switch */

   if (flag_debug) {
     if (rc) {
       printf ("    pic30_populate_operand_value::value = %#lx\n",
               operand_value->value);
       printf ("    pic30_populate_operand_value::optional_value = %#lx\n",
               operand_value->optional_value);
     }
     printf ("<-- pic30_populate_operand_value::exit(%s)\n",
             rc ? "TRUE" : "FALSE");
   }

   return rc;
} /* static unsigned char pic30_populate_operand_value(...) */

/******************************************************************************/

struct pic30_opcode *
pic30_find_opcode_based_on_operands (opcode, operands, operand_count,
                                     max_operands, min_operands, err_operand,
                                     err_string)
   struct pic30_opcode * opcode;
   expressionS operands[];
   short operand_count;
   unsigned short * max_operands;
   unsigned short * min_operands;
   unsigned short * err_operand;
   char ** err_string;

/******************************************************************************
 *
 *   This function will find the correct opcode based on the operands that are
 *   passed.  It ensures that the operand count is correct and then checks
 *   each operand to see if it is a match.
 *
 *   *max_operands is filled with the maximum number of operands that this
 *                 opcode accepts.
 *   *min_operands is filled with the minimum number of operands that this
 *                 opcode accepts.
 *   *err_operand is filled with the first operand that did not match in the
 *                longest matching opcode.
 *
 *   RETURNS:
 *      NULL - If a matching instruction is not found, or
 *      The opcode structure for the matching instruction.
 *
 ******************************************************************************/

{
   struct pic30_opcode * current_opcode = opcode;
   const struct pic30_opcode * last_opcode = pic30_opcodes + pic30_num_opcodes;
   unsigned char match = FALSE;
   int is_ecore = pic30_is_ecore_machine(global_PROCESSOR);

   if (flag_debug)
      printf ("--> pic30_find_opcode_based_on_operands (..., %d, ...)::begin\n",
              operand_count);

   *max_operands = opcode->number_of_operands;
   *min_operands = opcode->number_of_operands;
   *err_operand = 0;
   *err_string = (char *) 0;

   if (flag_debug) {
     int i;
     printf ("    pic30_find_opcode_based_on_operands::"
             "number_of_operands = %d\n",
             current_opcode->number_of_operands);

     for (i = 0; i < current_opcode->number_of_operands; i++) {
       printf ("    pic30_find_opcode_based_on_operands::%d::(", i+1);
       print_expression (&operands[i]);
       printf (")\n");
     }

     printf ("    pic30_find_opcode_based_on_operands::"
             "testing variants of (%s)\n", opcode->name);
   } /* if (flag_debug) */


   for (; ((current_opcode < last_opcode) &&
	   (strcmp (opcode->name, current_opcode->name) == 0));
	   current_opcode++)
   {  
      if (current_opcode->number_of_operands > *max_operands)
         *max_operands = current_opcode->number_of_operands;

      if (current_opcode->number_of_operands < *min_operands)
         *min_operands = current_opcode->number_of_operands;

      if ((current_opcode->flags & F_ECORE) && !is_ecore) continue;
      if ((current_opcode->flags & F_FCORE) && is_ecore) continue;

      if (current_opcode->number_of_operands == operand_count)
      {
         int i;
         unsigned char types_match = TRUE;

         /* Check to see if the operand types match */
         for (i = 0; i < current_opcode->number_of_operands; i++)
         {
            const struct pic30_operand * operand =
               &(pic30_operands[current_opcode->operands[i]]);
            const unsigned long operator_mask =
               pic30_operator_mask[operands[i].X_op];
            struct pic30_operand_value check_for_match;

            if (flag_debug)
            {
               printf ("    pic30_find_opcode_based_on_operands::%d::(0x%lx == "
                       "0x%lx) && %s\n",
                       i+1, (operand->type & operator_mask), operator_mask,
                       (operand->immediate == operands[i].X_md.immediate)
                       ? "TRUE" : "FALSE");
            } /* if (flag_debug) */
                                
            if (((operand->type & operator_mask) == operator_mask) &&
                (operand->immediate == operands[i].X_md.immediate))
            {
               if ((operand->is_match) &&
                   (pic30_populate_operand_value (&(operands[i]),
                                                 &check_for_match,0)))
                  types_match = (*operand->is_match)(&check_for_match);
            }
            else if (operand->type == OPND_PAGE_SYMBOL) {
              pic30_populate_operand_value(&(operands[i]), &check_for_match,
					   OPND_PAGE_SYMBOL);
              types_match = (*operand->is_match)(&check_for_match);
            }
            else
               types_match = FALSE;

            if (!types_match)
            {
              if (i+1 > *err_operand) {
                  *err_operand = i+1;
                  *err_string = operand->info_string;
              }

               break; /* out of for loop */
            } /* if (!types_match) */
         } /* for i */

         if (types_match)
         {
            /* Found a matching opcode with the correct types of operands */
            match = TRUE;
            break; /* out of while loop */
         } /* if (types_match) */
      } /* if (current_opcode->number_of_operands == operand_count) */
   } /* for */

   if (!match)
      current_opcode = NULL;

   if (flag_debug)
      printf ("<-- pic30_find_opcode_based_on_operands::exit\n");

   return current_opcode;
} /* struct pic30_opcode * pic30_find_opcode_based_on_operands(...) */

/******************************************************************************/

unsigned char
pic30_operands_encode_identically (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[];

/******************************************************************************
 *
 *   Checks to ensure that instructions that share bits for operands actually
 *   encode identically.  This would be the case for [Wn+Wb], [Wn+Wb].  If a
 *   match can be accomplished by switching one of the operands two registers,
 *   then the assembler will switch that operand.
 *
 *   RETURNS:
 *      FALSE - The operands share encoding bits and they do not match.
 *      TRUE  - otherwise
 *
 ******************************************************************************/

{
   unsigned char rc = TRUE;

   if (flag_debug)
      printf ("--> pic30_operands_encode_identically::begin\n");

   if (operands[0].X_op == O_REGISTER_WITH_DISPLACEMENT)
   {
      int i;

      for (i = 0; i < opcode->number_of_operands; i++)
      {
         struct pic30_operand_value opnd_0;

         /*
          * Pulled in so that if more than 2 operands ever need to be checked
          * and a previous operand changed operand 0 this will be calculated
          * again.
          */
         pic30_populate_operand_value (&(operands[0]), &opnd_0, 0);

         if (operands[i].X_op == O_REGISTER_WITH_DISPLACEMENT)
         {
            struct pic30_operand_value opnd_i;

            pic30_populate_operand_value (&(operands[i]), &opnd_i, 0);

            if (opnd_0.optional_value != opnd_i.optional_value)
            {
               if (opnd_0.value == opnd_i.value)
               {
                  /*
                   * Switch the order of the registers in both operands so
                   * that the encoding will match.
                   */
                  symbolS * opnd_left;

                  opnd_left = operands[0].X_add_symbol;
                  operands[0].X_add_symbol = operands[0].X_op_symbol;
                  operands[0].X_op_symbol = opnd_left;

                  opnd_left = operands[i].X_add_symbol;
                  operands[i].X_add_symbol = operands[i].X_op_symbol;
                  operands[i].X_op_symbol = opnd_left;
               } /* if (opnd_0.value == opnd_i.value) */
               else if (opnd_0.value == opnd_i.optional_value)
               {
                  /*
                   * Switch the order of the registers in the 1st operand so
                   * that the encoding will match.
                   */
                  symbolS * opnd_left;

                  opnd_left = operands[0].X_add_symbol;
                  operands[0].X_add_symbol = operands[0].X_op_symbol;
                  operands[0].X_op_symbol = opnd_left;
               } /* else if (operands switched) */
               else if (opnd_0.optional_value == opnd_i.value)
               {
                  /*
                   * Switch the order of the registers in the 2nd operand so
                   * that the encoding will match.
                   */
                  symbolS * opnd_left;

                  opnd_left = operands[i].X_add_symbol;
                  operands[i].X_add_symbol = operands[i].X_op_symbol;
                  operands[i].X_op_symbol = opnd_left;
               } /* else if (operands switched) */
               else
               {
                  rc = FALSE;
                  break;
               } /* else */
            } /* if (opnd_0.optional_value != opnd_i.optional_value) */
         } /* if (operands[i].X_op == O_REGISTER_WITH_DISPLACEMENT) */
      } /* for i */
   } /* if (operands[0].X_op == O_REGISTER_WITH_DISPLACEMENT) */

   if (flag_debug)
      printf ("<-- pic30_operands_encode_identically::exit(%s)\n",
              rc ? "TRUE" : "FALSE");

   return rc;
} /* unsigned char pic30_operands_encode_identically(...) */
/******************************************************************************/


/******************************************************************************
 *
 * Common code for CALL boot() and GOTO boot() instructions
 *
 ******************************************************************************/
#define CREATE_CALL_GOTO_ACCESS_INSTR {                                      \
                                                                             \
         /* create space for the instruction */                              \
         char *                                                              \
         output_frag = frag_more  (PIC30_SIZE_OF_PROGRAM_WORD * 2);          \
                                                                             \
         /* output the first instruction word */                             \
         md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD); \
                                                                             \
         /* create a fixup */                                                \
         fix_new (frag_now, output_frag - frag_now->fr_literal,              \
                  2, operands[0].X_add_symbol,                               \
                  frag_now->fr_offset, FALSE,                                \
                  BFD_RELOC_PIC30_CALL_ACCESS);                              \
                                                                             \
         /* output the second instruction word */                            \
         md_number_to_chars (output_frag + PIC30_SIZE_OF_PROGRAM_WORD,       \
                             0, PIC30_SIZE_OF_PROGRAM_WORD);                 \
         }


/******************************************************************************
 *
 *   This function is called to encode a CALL instruction. A variant frag is
 *   set up to see if the CALL instruction can be relaxed.
 *
 ******************************************************************************/
void
pic30_create_call_insn (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[];
{
   unsigned int insn = opcode->opcode[0];

   if (flag_debug)
      printf ("--> pic30_create_call_insn::begin\n");

   /* keep globals current */
   global_current_location += PIC30_SIZE_OF_PROGRAM_WORD * 2;
   global_p_directive_active = FALSE;;
         
   switch (operands[0].X_op)
   {
      case O_constant:
      {
         symbolS *symbolp;

         if (operands[0].X_add_number < 0)
         {
            as_bad (_("Absolute address must be greater than or equal "
                      "to 0."));
            break;
         } /* if (operands[i].X_add_number < 0) */

         if (!PIC30_IS_EVEN (operands[0].X_add_number))
         {
            as_warn (_("Expecting even address.  "
                       "Address will be rounded."));
            operands[0].X_add_number += 1;
         } /* if (!PIC30_IS_EVEN (operands[0].X_add_number)) */

         symbolp = symbol_new (FAKE_LABEL_NAME, absolute_section,
                             (valueT) operands[0].X_add_number, frag_now);

         symbol_mark_resolved (symbolp);
               
         operands[0].X_op = O_symbol;
         operands[0].X_add_symbol = symbolp;
         operands[0].X_add_number = 0;

         /* No break.  Want to fall through. */
      }

      case O_symbol:
        if (BOOT_ACCESS(operands[0]) || SECURE_ACCESS(operands[0])) {

         CREATE_CALL_GOTO_ACCESS_INSTR;
        }
        else
      {
         char * output_frag =
            frag_var (rs_machine_dependent, PIC30_SIZE_OF_PROGRAM_WORD * 2,
                      PIC30_SIZE_OF_PROGRAM_WORD,
                      (relax_substateT) PIC30_RELAX_RCALL,
                      operands[0].X_add_symbol, operands[0].X_add_number, NULL);

         md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD);
       }

     break;

      default:
         BAD_CASE (operands[0].X_op);
         break;
   } /* switch */

   if (flag_debug)
      printf ("<-- pic30_create_call_insn::exit\n");
} /* void pic30_create_call_insn(...) */
/******************************************************************************/

/******************************************************************************
 *
 *   This function is called to encode a GOTO instruction. A frag is set up
 *   and fixups are created for both the LSB and MSB of the symbol.
 *
 ******************************************************************************/
void
pic30_create_goto_insn (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[];
{
   unsigned int insn = opcode->opcode[0];

   if (flag_debug)
      printf ("--> pic30_create_goto_insn::begin\n");

   /* keep globals current */
   global_current_location += PIC30_SIZE_OF_PROGRAM_WORD * 2;
   global_p_directive_active = FALSE;;
         
   switch (operands[0].X_op)
   {
      case O_constant:
      {
         symbolS *symbolp;

         if (operands[0].X_add_number < 0)
         {
            as_bad (_("Absolute address must be greater than or equal "
                      "to 0."));
            break;
         } /* if (operands[i].X_add_number < 0) */

         if (!PIC30_IS_EVEN (operands[0].X_add_number))
         {
            as_warn (_("Expecting even address.  "
                       "Address will be rounded."));
            operands[0].X_add_number += 1;
         } /* if (!PIC30_IS_EVEN (operands[0].X_add_number)) */

         symbolp = symbol_new (FAKE_LABEL_NAME, absolute_section,
                             (valueT) operands[0].X_add_number, frag_now);

         symbol_mark_resolved (symbolp);

         operands[0].X_op = O_symbol;
         operands[0].X_add_symbol = symbolp;
         operands[0].X_add_number = 0;
         
         /* No break.  Want to fall through. */
      }

      case O_symbol:
      {
         char * output_frag;

#if 1 
         if (operands[0].X_add_number != 0) {
            as_bad (_("PC Relative expression is not a valid GOTO target."));
            break;
         }
#endif

         if (BOOT_ACCESS(operands[0]) || SECURE_ACCESS(operands[0])) {

           CREATE_CALL_GOTO_ACCESS_INSTR;
         }
         else {
         output_frag =
            frag_var (rs_machine_dependent, PIC30_SIZE_OF_PROGRAM_WORD * 2,
                      PIC30_SIZE_OF_PROGRAM_WORD,
                      (relax_substateT) PIC30_RELAX_BRA,
                      operands[0].X_add_symbol, operands[0].X_add_number, NULL);

         md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD);
         }

         break;
      }

      default:
        as_bad (_("This expression is not a valid GOTO target."));
        break;
   } /* switch */

   if (flag_debug)
      printf ("<-- pic30_create_goto_insn::exit\n");
} /* void pic30_create_goto_insn(...) */

/******************************************************************************/

void
pic30_create_do_insn (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[];

/******************************************************************************
 *
 *   This function is called to encode a DO instruction.
 *
 ******************************************************************************/

{
   unsigned short i;

   if (flag_debug)
      printf ("--> pic30_create_do_insn::begin\n");

   /* keep globals current */
   global_current_location += PIC30_SIZE_OF_PROGRAM_WORD;
   global_p_directive_active = FALSE;;
         
   for (i = 0; i < opcode->number_of_operands; i++)
   {
      const struct pic30_operand * opnd =
         &(pic30_operands[opcode->operands[i]]);

      char * output_frag = frag_more (PIC30_SIZE_OF_PROGRAM_WORD);

      unsigned int insn = opcode->opcode[i];

      switch (operands[i].X_op)
      {
         case O_constant:
            if (!operands[i].X_md.immediate)
            {
               symbolS *symbolp;
               expressionS expr;
               fixS * fix;

               if (operands[i].X_add_number < 0)
               {
                  as_bad (_("Absolute address must be greater than or equal "
                            "to 0."));
                  break;
               } /* if (operands[i].X_add_number < 0) */

               if (!PIC30_IS_EVEN (operands[i].X_add_number))
               {
                  as_warn (_("Expecting even address.  "
                             "Address will be rounded."));
                  operands[i].X_add_number += 1;
               } /* if (!PIC30_IS_EVEN (operands[i].X_add_number)) */

               symbolp = symbol_new (FAKE_LABEL_NAME, absolute_section,
                            (valueT) operands[i].X_add_number, frag_now);

               symbol_mark_resolved (symbolp);

               expr.X_op = O_symbol;
               expr.X_add_symbol = symbolp;
               expr.X_add_number = 0;

               fix = fix_new_exp (frag_now, output_frag - frag_now->fr_literal,
                        pic30_relocation_info[opnd->relocation].fix_size,
                        &(expr),
                        pic30_relocation_info[opnd->relocation].pc_relative,
                        BFD_RELOC_PIC30_DO_ABSOLUTE);

               fix->tc_fix_data.operand = opcode->operands[i];

               break;
            } /* if (!operands[i].X_md.immediate) */
            /*
             else want to fall through to default handling.
             */

         case O_register:
            insn = PIC30_ADD_OPERAND (insn, operands[i].X_add_number,
                               opnd->bits, opnd->shift);

            md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD);

            break;

         case O_uminus:
         case O_bit_not:
         case O_logical_not:
         case O_multiply:
         case O_divide:
         case O_modulus:
         case O_left_shift:
         case O_right_shift:
         case O_bit_inclusive_or:
         case O_bit_or_not:
         case O_bit_exclusive_or:
         case O_bit_and:
         case O_add:
         case O_subtract:
         case O_eq:
         case O_ne:
         case O_lt:
         case O_le:
         case O_ge:
         case O_gt:
         case O_logical_and:
         case O_logical_or:
         case O_symbol:
         {
            fixS * fix =
               fix_new_exp (frag_now, output_frag - frag_now->fr_literal,
                            pic30_relocation_info[opnd->relocation].fix_size,
                            &(operands[i]),
                            pic30_relocation_info[opnd->relocation].pc_relative,
                            pic30_relocation_info[opnd->relocation].value);

            fix->tc_fix_data.operand = opcode->operands[i];

            md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD);

            break;
         }

         default:
            BAD_CASE (operands[i].X_op);
            break;
      } /* switch (operands[i].X_op) */
   } /* for i */

   if (flag_debug)
      printf ("<-- pic30_create_do_insn::exit\n");
} /* void pic30_create_do_insn(...) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called when a 2 word instruction needs to be encoded.
 *   It will call the appropriate function based on the opcode of the
 *   instruction.
 *
 ******************************************************************************/
void
pic30_create_2word_insn (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[];
{
   unsigned int insn_word1 = opcode->opcode[0];

   if (flag_debug)
      printf ("--> pic30_create_2word_insn::begin\n");

   switch (insn_word1)
   {
      case DO_INSN:
      case DOW_INSN:
         pic30_create_do_insn (opcode, operands);
         break;

      case CALL_INSN:
         pic30_create_call_insn (opcode, operands);
         break;

      case GOTO_INSN:
         pic30_create_goto_insn (opcode, operands);
         break;

      default:
         BAD_CASE (insn_word1);
         break;
   } /* switch (insn_word1) */
#if defined(OBJ_ELF)
  if (global_DEBUGINFO != NO_GEN_DEBUGINFO)
  {
    dwarf2_emit_insn(PIC30_SIZE_OF_PROGRAM_WORD*2/2);
    frag_wane (frag_now);  /* close off the current frag */
    frag_new (0);          /* and start a new one */
  }
#endif

   if (flag_debug)
      printf ("<-- pic30_create_2word_insn::exit\n");
} /* void pic30_create_2word_insn(...) */

/******************************************************************************/

void
pic30_create_insn (opcode, operands, operand_count)
   struct pic30_opcode * opcode;
   expressionS operands[];
   short operand_count;

/******************************************************************************
 *
 *   This function is called to encode a single word instruction.
 *
 ******************************************************************************/

{
   unsigned int insn = opcode->opcode[0];
   unsigned char have_bra_insn = ((insn == BRA_INSN) ||
                                  (insn == RCALL_INSN) ||
                                  (insn == BRA_CC_INSN) ||
                                  (insn == CPWBEQ_W_INSN) ||
                                  (insn == CPWBEQ_B_INSN) ||
                                  (insn == CPWBGT_W_INSN) ||
                                  (insn == CPWBGT_B_INSN) ||
                                  (insn == CPWBLT_W_INSN) ||
                                  (insn == CPWBLT_B_INSN) ||
                                  (insn == CPWBNE_W_INSN) ||
                                  (insn == CPWBNE_B_INSN) ||
                                  (insn == BRA_DSP_INSN));
   short i;
   char * output_frag = frag_more (PIC30_SIZE_OF_PROGRAM_WORD);

   if (flag_debug)
      printf ("--> pic30_create_insn (..., %d)::begin\n", operand_count);

   /* keep globals current */
   global_current_location += PIC30_SIZE_OF_PROGRAM_WORD;
   global_p_directive_active = FALSE;;
         
   for (i = 0; i < operand_count; i++)
   {
      const struct pic30_operand * operand =
         &(pic30_operands[opcode->operands[i]]);
      const struct relocation_info reloc =
         pic30_relocation_info[operand->relocation];

      switch (operands[i].X_op)
      {

         case O_constant:
            if ((have_bra_insn) && (!operands[i].X_md.immediate))
            {
               symbolS *symbolp;
               expressionS expr;
               fixS * fix;

               if (operands[i].X_add_number < 0)
               {
                   as_bad (_("Absolute address must be greater than or equal "
                             "to 0."));
                   break;
               } /* if (operands[i].X_add_number < 0) */

               if (!PIC30_IS_EVEN (operands[i].X_add_number))
               {
                  as_warn (_("Expecting even address.  "
                             "Address will be rounded."));
                  operands[i].X_add_number += 1;
               } /* if (!PIC30_IS_EVEN (operands[i].X_add_number)) */

               symbolp = symbol_new (FAKE_LABEL_NAME, absolute_section,
                                     (valueT) operands[i].X_add_number,
                                     frag_now);

               symbol_mark_resolved (symbolp);

               expr.X_op = O_symbol;
               expr.X_add_symbol = symbolp;
               expr.X_add_number = 0;
 
               fix = fix_new_exp (frag_now, output_frag - frag_now->fr_literal,
                                  reloc.fix_size, &(expr), reloc.pc_relative,
                                  BFD_RELOC_PIC30_BRANCH_ABSOLUTE);

               fix->tc_fix_data.operand = opcode->operands[i];

               break;
            } /* if ((have_bra_insn) && (!operands[i].X_md.immediate)) */

            if (operands[i].X_md.modifier == PIC30_DMAOFFSET_FOUND)
              {
                as_bad (_("Cannot use a constant as the argument of dmaoffset."));
                operands[i].X_add_number = 0;
              }

            /*
             else want to fall through to default handling.
             */

         default:
         {
            struct pic30_operand_value operand_value;

            if (pic30_populate_operand_value (&(operands[i]), &operand_value,operand->type))
            {
              /*
              ** the operand can be calculated now
              */
               if (operand->insert)
               {
                  char * error_msg = (char *) NULL;

                  insn = (*operand->insert)(insn, opcode->flags,
                                            operand, &operand_value,
                                            &error_msg);

                  if (error_msg != (char *) NULL)
                  {
                     as_fatal (error_msg);
                     free (error_msg);
                  } /* if (error_msg != (char *) NULL) */
               } /* if (operand->insert) */
               else
               {
                  long value = operands[i].X_add_number;

                  switch (operands[i].X_md.modifier)
                  {
                     case PIC30_TBLOFFSET_FOUND:
                        value = PIC30_TBLOFFSET (value);
                        break;

                     case PIC30_PSVOFFSET_FOUND:
                        value = PIC30_PSVOFFSET (value);
                        break;

                     case PIC30_HANDLE_FOUND:
                        value = PIC30_HANDLE (value);
                        break;

                     case PIC30_TBLPAGE_FOUND:
                        value = PIC30_TBLPAGE (value);
                        break;

                     case PIC30_PSVPAGE_FOUND:
                        value = PIC30_PSVPAGE (value);
                        break;

                     case PIC30_PSVPTR_FOUND:
                        value = PIC30_PSVPTR (value);
                        break;

                     default:
                        break;
                  } /* switch (operands[i].X_md.modifier) */

                  insn = PIC30_ADD_OPERAND (insn, value, operand->bits,
                                     operand->shift);
               } /* else */
            } /* if (pic30_populate_operand_value (...)) */
            else
            {
              const char *err_str = " modifier not allowed in instruction";

               if (operands[i].X_op != O_W_REG)
               {
                 /*
                 ** operand refers to a symbol
                 */
                  fixS * fix;

                  if (flag_debug)
                     printf ("    pic30_create_insn::fixup->type = %s\n"
                             "    pic30_create_insn::fixup->where = %d\n"
                             "    pic30_create_insn::fixup->size = %d\n"
                             "    pic30_create_insn::fixup->pc relative = %s\n"
                             "    pic30_create_insn::fixup->value = %d\n",
                             bfd_get_reloc_code_name (reloc.value),
                             output_frag - frag_now->fr_literal,
                             reloc.fix_size,
                             (reloc.pc_relative ? "TRUE" : "FALSE"),
                             reloc.value);
                               
                  if (operands[i].X_md.modifier == PIC30_TBLOFFSET_FOUND)
                  {
                     if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::TBLOFFSET\n");

                     if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                       as_bad (_("%s%s"), "tbloffset", err_str);
                       break;
                     }
                     else
                       fix = fix_new_exp (frag_now,
                                          output_frag - frag_now->fr_literal,
                                          2, &(operands[i]), FALSE,
					  (reloc.value == BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET)?
                                          BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET :
					  BFD_RELOC_PIC30_WORD_TBLOFFSET);
                  } /* if (PIC30_TBLOFFSET_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_PSVOFFSET_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::PSVOFFSET\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "psvoffset", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET :
                                           BFD_RELOC_PIC30_WORD_PSVOFFSET);
                    } /* if (PIC30_PSVOFFSET_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_TBLPAGE_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::TBLPAGE\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "tblpage", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE :
                                           BFD_RELOC_PIC30_WORD_TBLPAGE);
                    } /* else if (PIC30_TBLPAGE_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_PSVPAGE_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::PSVPAGE\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "psvpage", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE :
                                           BFD_RELOC_PIC30_WORD_PSVPAGE);
                    } /* else if (PIC30_PSVPAGE_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_HANDLE_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::HANDLE\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "handle", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                          BFD_RELOC_PIC30_UNSIGNED_10_HANDLE :
                                           BFD_RELOC_PIC30_WORD_HANDLE);
                    } /* else if (PIC30_HANDLE_FOUND) */
                 else if (operands[i].X_md.modifier == PIC30_DMAPAGE_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::DMAPAGE\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "dmapage", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_DMAPAGE :
                                           BFD_RELOC_PIC30_WORD_DMAPAGE);
                    } /* else if (PIC30_DMAPAGE_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_DMAOFFSET_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::DMAOFFSET\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "dmaoffset", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                          BFD_RELOC_PIC30_UNSIGNED_10_DMAOFFSET :
                                           BFD_RELOC_PIC30_WORD_DMAOFFSET);
                    } /* else if (PIC30_DMAOFFSET_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_PSVPTR_FOUND)
                  {
                     if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::PSVPTR\n");

                     fix = fix_new_exp (frag_now,
                                        output_frag - frag_now->fr_literal,
                                        2, &(operands[i]), FALSE,
                                        BFD_RELOC_PIC30_WORD_PSVPTR);
                  } /* else if (PIC30_PSVPTR_FOUND) */
                  else if ((operands[i].X_md.modifier == PIC30_BOOT_FOUND) ||
                           (operands[i].X_md.modifier == PIC30_SECURE_FOUND))
                  { 
                     fix = fix_new_exp (frag_now,
                                        output_frag - frag_now->fr_literal,
                                        2, &(operands[i]), FALSE,
                                        have_bra_insn ?
                                        BFD_RELOC_PIC30_PCREL_ACCESS :
                                        BFD_RELOC_PIC30_WORD_ACCESS);
                  } /* else if ((operands[i].X_md.modifier == PIC30_BOOT_FOUND) || */
                  else if (operands[i].X_md.modifier == PIC30_EDSPAGE_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::EDSPAGE\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10))                      {
                        as_bad (_("%s%s"), "edspage", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_EDSPAGE :
                                           BFD_RELOC_PIC30_WORD_EDSPAGE);
                    } /* else if (PIC30_EDSPAGE_FOUND) */
                  else if (operands[i].X_md.modifier == PIC30_EDSOFFSET_FOUND)
                    {
                      if (flag_debug)
                        fprintf (stdout, "    pic30_create_insn::EDSOFFSET\n");

                      if ((reloc.value != BFD_RELOC_PIC30_WORD ) &&                                       (reloc.value != BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)) {
                        as_bad (_("%s%s"), "edsoffset", err_str);
                        break;
                      }
                      else
                        fix = fix_new_exp (frag_now,
                                           output_frag - frag_now->fr_literal,
                                           2, &(operands[i]), FALSE,
					   (reloc.value == BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10)?
                                           BFD_RELOC_PIC30_UNSIGNED_10_EDSOFFSET :
                                           BFD_RELOC_PIC30_WORD_EDSOFFSET);
                    } /* else if (PIC30_EDSOFFSET_FOUND) */
                  else
                  {
                     fix = fix_new_exp (frag_now,
                                        output_frag - frag_now->fr_literal,
                                        reloc.fix_size, &(operands[i]),
                                        reloc.pc_relative, reloc.value);
                  } /* else */

                  fix->tc_fix_data.operand = opcode->operands[i];
               } /* if (operands[i].X_op != O_W_REG) */
            } /* else */
            break;
         } /* default */
      } /* switch (operands[i].X_op) */
   } /* for (i = 0; i < operand_count; i++) */

   /* Write out the instruction. */
   md_number_to_chars (output_frag, insn, PIC30_SIZE_OF_PROGRAM_WORD);
#if defined(OBJ_ELF)
  if (global_DEBUGINFO != NO_GEN_DEBUGINFO)
  {
    dwarf2_emit_insn(PIC30_SIZE_OF_PROGRAM_WORD/2);
    frag_wane (frag_now);  /* close off the current frag */
    frag_new (0);          /* and start a new one */
  }
#endif

  if (flag_debug) {
      printf ("    pic30_create_insn::opcode = %#x\n", insn);
      printf ("<-- pic30_create_insn::exit\n");
  }
} /* void pic30_create_insn(...) */

/******************************************************************************/

void
pic30_check_isa (opcode, operands)
   struct pic30_opcode * opcode;
   expressionS operands[] __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function is called to check an instruction against the current
 *   isa setting. Issue an error if an instruction does not match the isa.
 *
 ******************************************************************************/

{
   if (flag_debug)
      printf ("--> pic30_check_isa::begin\n");

   if (opcode->flags & F_IS_DSP_INSN)
     {
       if (!pic30_is_dsp_machine(global_PROCESSOR))
         as_bad (_("This instruction is not supported in the %s device."),
                 global_PROCESSOR->printable_name);
     }

   if (flag_debug)
      printf ("<-- pic30_check_isa::exit\n");
} /* void pic30_warn_isa (...) */

/******************************************************************************
 *
 * This function generates a line-number entry for each instruction
 * in the .text section. The feature is intended to support source-
 * level debug of assembly-language files.
 * Since line-numbers are conventionally associated with a function
 * in the symbol table, a dummy function named $AUTOLINE$ is created
 * from which to hang the line numbers.
 *
 * This feature is enabled with the '-g' command-line option.
 *
 ******************************************************************************/
static void
md_gen_debug_info(void)
{
#ifdef OBJ_ELF
	/*
	** Nothing to do here
	*/
#else
	char *filename;
	unsigned lineno;

	if (now_seg != text_section)
	{
		return;
	}
	as_where(&filename, &lineno);

	if (global_DEBUGINFO != GENNING_DEBUGINFO)
	{
		symbolS *sym;

		/*
		** Make sure that there is a .file symbol
		*/
		pic30_install_mainfile_symbol();

		/*
		** Generate a fake symbol to which the line numbers
		** will be linked.
		*/
		sym = symbol_make("$AUTOLINE$");
		S_SET_SEGMENT(sym, text_section);
		symbol_set_frag(sym, frag_now);
		S_SET_VALUE(sym, frag_now_fix());
		S_SET_STORAGE_CLASS(sym, C_STAT);
		S_SET_DATA_TYPE(sym, T_VOID | (DT_FCN << N_BTSHFT));
		S_SET_NUMBER_AUXILIARY(sym, 1);
		SA_SET_SYM_LNNO(sym, lineno);
		coff_add_linesym(sym);

		global_DEBUGINFO = GENNING_DEBUGINFO;
	}
	add_lineno(frag_now, frag_now_fix(), lineno);
#endif
}
/******************************************************************************
 *
 *   This function is called by GAS for each input line which does not contain
 *   a pseudo-op.  The argument is a null-terminated string.  The function
 *   should assemble the string as an instruction with operands.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   input_line        The line to parse and create an instruction from.
 *
 ******************************************************************************/
void
md_assemble (input_line)
   char * input_line;
{
   if (flag_debug)
      printf ("--> md_assemble::begin\n");

   if (input_line != NULL)
   {
      char * token = input_line;
      char * tmp_input_line;

      /*
       * Ensure that the current section is a code section.
       * If not, issue an error.
       */
      if (!subseg_text_p (now_seg) && 
	   (now_seg != text_section) && 
	  !subseg_auxflash (now_seg))
        as_bad(_("Cannot locate executable code in a data section."));

      if (global_DEBUGINFO != NO_GEN_DEBUGINFO)
      {
         md_gen_debug_info();
      }
      /*
       * Ensure that the instruction will be assembled at the beginning of a
       * program word.  This is required because the user is allowed to store
       * data compressed in the text section and the last thing that was
       * stored may not have put the location counter on a
       * PIC30_SIZE_OF_PROGRAM_WORD boundary.
       *
       * We only have to do this if an assembler directive has been called
       * that may have screwed up the alignment of the location counter.  We do
       * this if check here versus calling the pic30_do_align function each
       * time in order to save time on the backend so that alignments do not
       * have to be done for every instruction -- just the first one after an
       * assembler directive that may have screwed up the alignment of the
       * program counter.
       */
      if (global_must_align_location_counter)
      {
         PIC30_DO_CODE_ALIGNMENT ();
         listing_prev_line();  /* attach padding (if any) to previous */
                               /*   line in the listing file          */
      } /* if (global_must_align_location_counter) */

      tmp_input_line = (char *)xmalloc (strlen (input_line) + 1);
      strcpy (tmp_input_line, input_line);

      for ( ; ((isalnum ((int)*input_line)) ||
               (*input_line == '.')); input_line++)
         continue;

      if (token == input_line)
         as_bad(_("Mnemonic not found."));
      else
      {
         if (*input_line != '\0')
            *input_line++ = '\0';

         if (token != NULL)
         {
            struct pic30_opcode * opcode;

            if (flag_debug)
               printf ("    md_assemble::Searching for opcode (%s)\n", token);

            opcode = (struct pic30_opcode *) hash_find (pic30_opcodes_hash,
                                                        token);

            if (opcode == NULL)
            {
               as_bad (_("Invalid mnemonic: '%s'"), token);
            } /* if (opcode == NULL) */
            else
            {
               char *save = input_line_pointer;
               expressionS operands[MAX_OPERANDS];
               short operand_count = 0;

               input_line_pointer = input_line;
               SKIP_WHITESPACE ();

               /*
                * Set the global variable before calling pic30_parse_operands.
                * This will be used when parsing the operands to determine if
                * something that looks like a symbol is actually a flag (i.e.,
                * GT, GTU, etc. in the branch instructions.
                */
               global_current_opcode_flags = opcode->flags;

               operand_count = pic30_parse_operands (operands);

               if (operand_count >= 0)
               {
                  unsigned short max_operands;
                  unsigned short min_operands;
                  unsigned short err_operand;
                  char * err_string;
                  opcode = pic30_find_opcode_based_on_operands (opcode,
                                                                operands,
                                                                operand_count,
                                                                &max_operands,
                                                                &min_operands,
                                                                &err_operand,
                                                                &err_string);

                  if (opcode != NULL)
                  {
                     static unsigned char previous_insn_is_repeat = FALSE;

                     if (pic30_operands_encode_identically (opcode, operands))
                     {
                        if ((previous_insn_is_repeat) &&
                            (opcode->flags & F_CANNOT_FOLLOW_REPEAT))
                           as_bad (_("%s cannot follow a REPEAT."),
                                   opcode->name);
                        else if (opcode->number_of_words == 0)
                           as_fatal (_("A dummy instruction cannot be "
                                       "used!"));
                        else if (opcode->number_of_words == 2)
                          {
                           pic30_create_2word_insn (opcode, operands);
                           pic30_check_isa (opcode, operands);
                          }
                        else
                          {
                           pic30_create_insn (opcode, operands,
                                              operand_count);
                           pic30_check_isa (opcode, operands);
                          }
                     } /* if (pic30_operands_encode_identically (...)) */
                     else
                        as_bad (_("Operands share encoding bits.  "
                                  "The operands must encode identically."));

                     previous_insn_is_repeat =
                        ((opcode->opcode[0] == REPEAT_INSN) |
                         (opcode->opcode[0] == REPEATW_INSN));
                  } /* if (opcode != NULL) */
                  else
                  {
                     if (operand_count > max_operands)
                        as_bad (_("Too many operands ('%s')."), tmp_input_line);
                     else if (operand_count < min_operands)
                        as_bad (_("Too few operands ('%s')."), tmp_input_line);
                     else
                     {
                        as_bad (_("Invalid operands specified ('%s')."),
                                tmp_input_line);
                        if (err_string)
                          as_tsktsk (_("       Check operand #%u. %s"),
                                     err_operand, err_string);
                        else
                          as_tsktsk (_("       Check operand #%u."),
                                     err_operand);
                     } /* else */
                  } /* else */
               } /* if (operand_count >= 0) */
               else if (operand_count == -1)
                  as_bad (_("Invalid operand syntax ('%s')."), tmp_input_line);

               input_line_pointer = save;
            } /* else */
         } /* if */
      } /* else */

      free (tmp_input_line);
   } /* if (input_line != NULL) */

   if (flag_debug)
      printf ("<-- md_assemble::exit\n");
} /* void md_assemble(char *) */

/******************************************************************************/

int
md_chars_to_number (buffer, number_of_bytes)
   unsigned char * buffer;
   int number_of_bytes;

/******************************************************************************
 *
 *   This function converts a character buffer that is in little endian
 *   format to a number.
 *
 *   ARGUMENT          DESCRIPTION
 *   --------------------------------------------------------------------------
 *   buffer            Character buffer in target byte order.
 *   number_of_bytes   Number of bytes in the input.
 *
 ******************************************************************************/

{
   int rc;

   if (flag_debug)
      printf ("--> md_chars_to_number (..., %d)::begin\n", number_of_bytes);

   for (rc = 0; number_of_bytes--;)
   {
      rc <<= 8;
      rc |= buffer[number_of_bytes];
   } /* for rc */

   if (flag_debug)
      printf ("<-- md_chars_to_number::exit(0x%x)\n", rc);

   return rc;
} /* int md_chars_to_number(unsigned char *, int) */

/******************************************************************************/

char *
md_atof (type, literalP, sizeP)
   char type;
   char * literalP;
   int * sizeP;

/******************************************************************************
 *
 *   Turn a string in input_line_pointer into a floating point constant
 *   of type 'type', and store the appropriate bytes in *literalP.  The number
 *   of LITTLENUMS emitted is stored in *sizeP .  An error message is
 *   returned, or NULL on OK.
 *
 ******************************************************************************/

{
   int precision;
   LITTLENUM_TYPE words[MAX_LITTLENUMS];
   LITTLENUM_TYPE * wordP;
   char * t;

   if (flag_debug)
      printf ("--> md_atof::begin\n");

   switch (type)
   {
      case 'F':
      case 'f':
         precision = 2;
         break;

      case 'd':
      case 'D':
         precision = 4;
         break;

      default:
         *sizeP = 0;
         return _("Bad call to md_atof");
   } /* switch */

   t = atof_ieee (input_line_pointer, type, words);
   if (t)
      input_line_pointer = t;

   *sizeP = precision * sizeof (LITTLENUM_TYPE);

   /* This loop outputs the LITTLENUMs in REVERSE order.  */
   for (wordP = words + precision - 1; precision--;)
   {
      md_number_to_chars (literalP, (valueT)(*wordP--),
                          sizeof (LITTLENUM_TYPE));
      literalP += sizeof (LITTLENUM_TYPE);
   } /* for (wordP) */

   if (flag_debug)
      printf ("<-- md_atof::exit\n");

   return NULL;
} /* char * md_atof(char, char *, int *) */

/******************************************************************************/


/******************************************************************************
 *
 *   This function indicates whether a fixup against a locally defined symbol
 *   should be adjusted to be against the section symbol.  It should return
 *   non-zero if the adjustment is acceptable.
 *
 ******************************************************************************/
int
pic30_fix_adjustable (fixP)
   struct fix * fixP;
{
   reloc_howto_type * howto =
      bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);
   if (howto == 0)
   {
     abort();
   }
   return pic30_adjustable_against_section (howto->type);
} /* int pic30_fix_adjustable(struct fix *) */

/******************************************************************************/

void
pic30_frob_label_before_define (name)
   const char * const name __attribute__ ((__unused__));

/******************************************************************************
 *
 *   Ensure that the label defined in a SEC_CODE section will be assembled at
 *   the beginning of a program word. This is required because the user is
 *   allowed to store data compressed in the text section and the last thing
 *   that was stored may not have put the location counter on a
 *   PIC30_SIZE_OF_PROGRAM_WORD boundary.
 *
 *   We only have to do this if an assembler directive has been called
 *   that may have screwed up the alignment of the location counter.  We do
 *   this if check here versus calling the pic30_do_align function each
 *   time in order to save time on the backend so that alignments do not
 *   have to be done for every label and instruction -- just the first one
 *   after an assembler directive that may have screwed up the alignment of
 *   the program counter.
 *
 ******************************************************************************/

{
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))
   {
      if ((global_must_align_location_counter) &&
          (PIC30_VALID_PC_ADDRESS (global_current_location) == FALSE))
      {
         PIC30_DO_CODE_ALIGNMENT ();
         listing_prev_line();  /* attach padding (if any) to previous */
                               /*   line in the listing file          */
      } /* if (global_must_align_location_counter) */
   } /* if SEC_CODE */
} /* void pic30_frob_label_before_define(char *) */

/******************************************************************************/

int
pic30_frob_symbol (symbolS *sym )

/****************************************************************************
 *
 * Perform any target-specific processing on a symbol
 * before it is written to the output file.
 *
 * Return value:
 *  0 - symbol is valid for output
 *  1 - do not output symbol
 *
 * Note: Because we moved the call point in write_object_file(), non-zero
 *  return values do not have the desired effect. To test symbols for
 *  removal, amend target-specific code later in that function (4/3/2009).
 *
 ****************************************************************************/
{
  int result = 0;

  if (symbol_section_p (sym))
    return result;

  /* look for symbols created by .equ */
  if ((S_GET_SEGMENT(sym) == absolute_section) ||
      (symbol_equated_p(sym))) {

    /* start the list, if necessary */
    if (pic30_max_symbols == 0) {
      pic30_equate_symbols = (void *) xmalloc(sizeof(symbolS *)
                                              * PIC30_INIT_SYM_COUNT);
      pic30_max_symbols = PIC30_INIT_SYM_COUNT;
      pic30_symbol_count = 0;
    }

    /* add this symbol */
    if (pic30_symbol_count >= pic30_max_symbols) {
      pic30_max_symbols += PIC30_GROW_SYM_COUNT;
      pic30_equate_symbols = (void *)
                             xrealloc(pic30_equate_symbols,
                                      sizeof(symbolS *) * pic30_max_symbols);
    }
    pic30_equate_symbols[pic30_symbol_count++] = sym;
  }
  return result;
}


/*
** Locate an insn word in the object file.
**
** Note: segment offset must be supplied in octets, not PC units
*/
static int
pic30_locate_insn_word(segT segP, unsigned long offset, unsigned long *pinsn)
{
      segment_info_type *seginfo;
      fragS *fragp;

      seginfo = seg_info(segP);
      if (seginfo != NULL)
      {
         for (fragp = seginfo->frchainP->frch_root;
              fragp;
              fragp = fragp->fr_next)
         {
            if (fragp->fr_fix &&
               (fragp->fr_address <= offset) &&
               ((fragp->fr_address + fragp->fr_fix) > offset))
            {
		   *pinsn = bfd_getl32(fragp->fr_literal +
                                                 (offset - fragp->fr_address));
                   return(TRUE);
            }
         }
      }
      return(FALSE);
}
/*
** Opcodes for DO LOOP verification.
*/
#define CALLW_OPCODE   0x010000ul
#define RCALLW_OPCODE  0x012000ul
#define GOTOW_OPCODE   0x014000ul
#define BRAW_OPCODE    0x016000ul
#define CALL_OPCODE    0x020000ul
#define GOTO_OPCODE    0x040000ul
#define RETLW_OPCODE   0x050000ul
#define RETURN_OPCODE  0x060000ul
#define RETFIE_OPCODE  0x064000ul
#define RCALL_OPCODE   0x070000ul
#define DO_OPCODE      0x080000ul
#define DOW_OPCODE     0x088000ul
#define REPEAT_OPCODE  0x090000ul
#define REPEATW_OPCODE 0x098000ul
#define TRAP_OPCODE    0x0A0000ul
#define BOA_OPCODE     0x0C0000ul
#define BOB_OPCODE     0x0D0000ul
#define BSA_OPCODE     0x0E0000ul
#define BSB_OPCODE     0x0F0000ul

#define BOV_OPCODE     0x300000ul
#define BC_OPCODE      0x310000ul
#define BZ_OPCODE      0x320000ul
#define BN_OPCODE      0x330000ul
#define BLES_OPCODE    0x340000ul
#define BLTS_OPCODE    0x350000ul
#define BLEU_OPCODE    0x360000ul
#define BRA_OPCODE     0x370000ul
#define BNOV_OPCODE    0x380000ul
#define BNC_OPCODE     0x390000ul
#define BNZ_OPCODE     0x3A0000ul
#define BNN_OPCODE     0x3B0000ul
#define BGTS_OPCODE    0x3C0000ul
#define BGES_OPCODE    0x3D0000ul
#define BGTU_OPCODE    0x3E0000ul

#define CPFSGT_OPCODE  0xE60000ul
#define CPFSLT_OPCODE  0xE68000ul
#define CPFSEQ_OPCODE  0xE78000ul
#define CPFSNE_OPCODE  0xE70000ul

#define DISI_MASK      0xFFC000ul
#define DISI_OPCODE    0xFC0000ul

/*
** See if an insn word is valid as the last insn in a DO loop.
*/
static int
pic30_valid_doend_insn_p(unsigned long insn)
{
   int bValid;

   switch (insn & 0xFFF000ul)
   {
   case BOA_OPCODE:
   case BOB_OPCODE:
   case BSA_OPCODE:
   case BSB_OPCODE:

   case BOV_OPCODE:
   case BC_OPCODE:
   case BZ_OPCODE:
   case BN_OPCODE:
   case BLES_OPCODE:
   case BLTS_OPCODE:
   case BLEU_OPCODE:
   case BRA_OPCODE:
   case BNOV_OPCODE:
   case BNC_OPCODE:
   case BNZ_OPCODE:
   case BNN_OPCODE:
   case BGTS_OPCODE:
   case BGES_OPCODE:
   case BGTU_OPCODE:

   case CPFSGT_OPCODE:
   case CPFSLT_OPCODE:
   case CPFSEQ_OPCODE:
   case CPFSNE_OPCODE:

   case GOTO_OPCODE:
   case GOTOW_OPCODE:
   case CALL_OPCODE:
   case RCALL_OPCODE:
   case RCALLW_OPCODE:
   case TRAP_OPCODE:

   case RETFIE_OPCODE:
   case RETURN_OPCODE:
   case RETLW_OPCODE:

   case REPEAT_OPCODE:
   case REPEATW_OPCODE:
   case DO_OPCODE:
   case DOW_OPCODE:
      bValid = FALSE;
      break;
   default:
     /* special cases */
      if ((insn & DISI_MASK) == DISI_OPCODE)
         bValid = FALSE;
      else
         bValid = TRUE;
      break;
   }
   return(bValid);
}
/*
** See if an insn word is valid as the penultimate insn in a DO loop.
** (as of C30 v3.01, the same restrictions apply SSR# BIN30-81
*/
#define pic30_valid_dopreend_insn_p pic30_valid_doend_insn_p

/*
** Validate all DO LOOP instructions.
*/
void
pic30_frob_file_after_relocs(void)
{
  struct pic30_do_fixup *l;

  for (l = pic30_do_fixup_list; l != NULL; l = l->next)
  {
      unsigned long insn;

      assert(l->fixP->fx_r_type == BFD_RELOC_PIC30_PCREL_DO);
      /*
      ** Validate the loop end instruction.
      */
      if (pic30_locate_insn_word(l->seg, l->endaddress, &insn))
      {
         if (!pic30_valid_doend_insn_p(insn))
         {
            as_bad_where(l->fixP->fx_file, l->fixP->fx_line,
		   _("DO loop end instruction is not valid."));
         }
      }
      else
      {
         as_warn_where(l->fixP->fx_file, l->fixP->fx_line,
		   _("DO loop target is not defined in current module."));
         as_warn_where(l->fixP->fx_file, l->fixP->fx_line,
                   _("DO loop end restrictions not verified."));
      }
      /*
      ** Validate the penultimate instruction: it may not be REPEAT.
      */
      if (pic30_locate_insn_word(l->seg, l->endaddress-4, &insn))
      {
         if (!pic30_valid_dopreend_insn_p(insn))
         {
            as_warn_where(l->fixP->fx_file, l->fixP->fx_line,
		   _("DO loop penultimate instruction may not be valid."));
         }
      }
      else
      {
         as_warn_where(l->fixP->fx_file, l->fixP->fx_line,
		   _("DO loop target is not fully defined in current module."));
         as_warn_where(l->fixP->fx_file, l->fixP->fx_line,
                   _("DO loop end restrictions not fully verified."));
      }
  }
}

/*
** Preserve a DO LOOP fixup for later validation.
**
** Note: the target symbol (and offset, if any)
** are expressed in PC units. Must convert to
** octets for the pic30_do_fixup structure.
*/
static void
pic30_record_do(fixP)
    fixS *fixP;
{
  struct pic30_do_fixup *do_fixup;

  do_fixup = ((struct pic30_do_fixup *)
              xmalloc(sizeof(struct pic30_do_fixup)));
  do_fixup->fixP = fixP;
  do_fixup->seg  = S_GET_SEGMENT(fixP->fx_addsy);
  do_fixup->endaddress = S_GET_VALUE(fixP->fx_addsy) +  fixP->fx_offset;
  do_fixup->endaddress *= OCTETS_PER_BYTE;
  do_fixup->next = pic30_do_fixup_list;

  pic30_do_fixup_list = do_fixup;
}

/******************************************************************************
 *
 *   This function is called to apply the fixup.
 *
 ******************************************************************************/
void
md_apply_fix3(fixP, valueP, seg)
   fixS * fixP;
   valueT * valueP;
   segT seg __attribute__ ((__unused__));
{
   long value = (long) *valueP;
   char * fix_position = fixP->fx_frag->fr_literal + fixP->fx_where;
   reloc_howto_type * howto =
      bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);

   if (flag_debug)
   {
      printf ("--> md_apply_fix3 (..., 0x%04lx, ...)::begin\n",
              (long) *valueP);
      printf ("    md_apply_fix3::line number = %d\n", fixP->fx_line);
      printf ("    md_apply_fix3::fx_where = %ld\n", fixP->fx_where);
      printf ("    md_apply_fix3::frag_address = 0x%04lx\n",
              (long) fixP->fx_frag->fr_address);
      printf ("    md_apply_fix3::size = %d\n", fixP->fx_size);
      printf ("    md_apply_fix3::fixP->fx_pcrel = %s\n",
              (fixP->fx_pcrel ? "TRUE" : "FALSE"));
      printf ("    md_apply_fix3::fixP->fx_addsy = %s\n",
              fixP->fx_addsy ? S_GET_NAME(fixP->fx_addsy) : "<none>");
      printf ("    md_apply_fix3::fixP->fx_subsy = %s\n",
              fixP->fx_subsy ? S_GET_NAME(fixP->fx_subsy) : "<none>");
   } /* if (flag_debug) */

   if (howto == (reloc_howto_type *) NULL)
   {
      as_bad_where (fixP->fx_file, fixP->fx_line,
                    _("Relocation %d not supported by object file format"),
                    (int)fixP->fx_r_type);

      if (flag_debug)
         printf ("<-- md_apply_fix3::exit\n");

      return;
   } /* if (howto == (reloc_howto_type *) NULL) */

   fixP->fx_done = TRUE;

   if (fixP->fx_addsy == NULL)
      value = pic30_apply_operator (fixP->fx_r_type, value, TRUE);
   else
   {
      segT add_segment = S_GET_SEGMENT (fixP->fx_addsy);

      if (fixP->fx_pcrel)
      {
         if (!PIC30_IS_EVEN (fixP->fx_offset))
         {
            as_warn_where (fixP->fx_file, fixP->fx_line,
               _("Expecting even offset.  Offset will be rounded."));
            fixP->fx_offset = (offsetT) fixP->fx_offset + 1;
         } /* if (!PIC30_IS_EVEN (fixP->fx_offset)) */

         /* value is target offset, in PC units */
         value = (offsetT) fixP->fx_offset;

         /* Leave for linker to deal with */
         fixP->fx_done = FALSE;
      } /* if (fixP->fx_pcrel) */
      else
      {
         if (add_segment == absolute_section)
         {
            /*
             * If the fixup is for a CALL or GOTO, then we want to delete the
             * symbol value from the value passed.  This type of relocation is
             * created against the "fake" symbol and will be taken care of by
             * the linker; otherwise, apply any special operator that was
             * applied to an absolute value.
             */
            switch (fixP->fx_r_type)
            {
               case BFD_RELOC_PIC30_PGM_ADDR_MSB:
               case BFD_RELOC_PIC30_PGM_ADDR_LSB:
                  value -= S_GET_VALUE (fixP->fx_addsy);
                  fixP->fx_done = FALSE;

                  break;

               default:
                  value = pic30_apply_operator (fixP->fx_r_type, value, TRUE);
                  break;
	     } /* switch (fixP->fx_r_type) */
	   } /* if (add_segment == absolute_section) */
         else if ((fixP->fx_subsy != (symbolS *) NULL) &&
                  (S_GET_SEGMENT (fixP->fx_subsy) == add_segment))
           {
             /* compute the difference between two symbols */
             value = S_GET_VALUE (fixP->fx_addsy) -
               S_GET_VALUE (fixP->fx_subsy);
           } /* else if ((fixP->fx_subsy != (symbolS *) NULL) && ... */
         else
         {   
            fixP->fx_done = FALSE;

            if (fixP->fx_addsy == (symbolS *) NULL)
            value =
               pic30_apply_operator (fixP->fx_r_type, fixP->fx_offset, TRUE);
            else
	      if (fixP->fx_r_type == BFD_RELOC_PIC30_PGM_ADDR_MSB)
                {
                  fixP->fx_offset = (fixP->fx_offset)>>16;
		}
              value = fixP->fx_offset;

            if (fixP->fx_subsy != (symbolS *) NULL)
            {
               if (S_GET_SEGMENT (fixP->fx_subsy) == absolute_section)
                  value -= S_GET_VALUE (fixP->fx_subsy);
               else
                  as_bad_where (fixP->fx_file, fixP->fx_line,
                                _("Expression too complex."));
            } /* if (fixP->fx_subsy != (symbolS *) NULL) */

            switch (fixP->fx_r_type)
            {
               case BFD_RELOC_PIC30_PGM_ADDR_LSB:
               case BFD_RELOC_PIC30_PGM_ADDR_MSB:
                  if ((!subseg_text_p (add_segment)) &&
		      (!subseg_auxflash (add_segment)) &&
                      (add_segment != text_section) &&
                      (add_segment != undefined_section))
                     as_bad_where (fixP->fx_file, fixP->fx_line,
                                   _("Cannot call a symbol (%s) that is "
                                     "not located in an executable section."),
                                   S_GET_NAME (fixP->fx_addsy));
                  break;

               case BFD_RELOC_PIC30_PSVPAGE:
               case BFD_RELOC_PIC30_P_PSVPAGE:
               case BFD_RELOC_PIC30_WORD_PSVPAGE:
	       case BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE:
               case BFD_RELOC_PIC30_PSVOFFSET:
               case BFD_RELOC_PIC30_P_PSVOFFSET:
               case BFD_RELOC_PIC30_WORD_PSVOFFSET:
	       case BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET: 
               case BFD_RELOC_PIC30_TBLPAGE:
               case BFD_RELOC_PIC30_P_TBLPAGE:
               case BFD_RELOC_PIC30_WORD_TBLPAGE:
               case BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE:
               case BFD_RELOC_PIC30_TBLOFFSET:
               case BFD_RELOC_PIC30_P_TBLOFFSET:
               case BFD_RELOC_PIC30_WORD_TBLOFFSET:
               case BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET:
               case BFD_RELOC_PIC30_HANDLE:
               case BFD_RELOC_PIC30_P_HANDLE:
               case BFD_RELOC_PIC30_WORD_HANDLE:
               case BFD_RELOC_PIC30_UNSIGNED_10_HANDLE:
               case BFD_RELOC_PIC30_PADDR:
               case BFD_RELOC_PIC30_P_PADDR:
               case BFD_RELOC_PIC30_PSVPTR:
               case BFD_RELOC_PIC30_P_PSVPTR:
               case BFD_RELOC_PIC30_L_PSVPTR:
               case BFD_RELOC_PIC30_WORD_PSVPTR:
               case BFD_RELOC_PIC30_UNSIGNED_10_PSVPTR:
                  if ((!subseg_text_p (add_segment)) &&
                      (!subseg_psv (add_segment)) &&
                      (!subseg_eedata (add_segment)) &&
		      (!subseg_auxflash (add_segment)) &&
                      (add_segment != text_section) &&
                      (add_segment != undefined_section))
                     as_bad_where (fixP->fx_file, fixP->fx_line,
                        _("Cannot use %s on a symbol (%s) that is "
                        "not located in a code, psv, or eedata section."),
                        (((fixP->fx_r_type == BFD_RELOC_PIC30_PSVPAGE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_PSVPAGE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_PSVPAGE)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE)) ?
                         "psvpage" :
                         ((fixP->fx_r_type == BFD_RELOC_PIC30_PSVOFFSET) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_PSVOFFSET) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_PSVOFFSET)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET)) ?
                         "psvoffset" :
                        ((fixP->fx_r_type == BFD_RELOC_PIC30_PSVPTR) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_PSVPTR) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_L_PSVPTR) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_PSVPTR)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_PSVPTR)) ?
                         "psvptr" :
                        ((fixP->fx_r_type == BFD_RELOC_PIC30_TBLPAGE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_TBLPAGE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_TBLPAGE)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE)) ?
                         "tblpage" :
                         ((fixP->fx_r_type == BFD_RELOC_PIC30_TBLOFFSET) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_TBLOFFSET) ||

                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_TBLOFFSET)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET)) ?
                         "tbloffset" :
                         ((fixP->fx_r_type == BFD_RELOC_PIC30_HANDLE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_P_HANDLE) ||
                          (fixP->fx_r_type == BFD_RELOC_PIC30_WORD_HANDLE)||
			  (fixP->fx_r_type == BFD_RELOC_PIC30_UNSIGNED_10_HANDLE)) ?
                         "handle" : "paddr"),
                        S_GET_NAME (fixP->fx_addsy));
                  break;

               case BFD_RELOC_PIC30_DMAPAGE:
               case BFD_RELOC_PIC30_P_DMAPAGE:
               case BFD_RELOC_PIC30_WORD_DMAPAGE:
	       case BFD_RELOC_PIC30_UNSIGNED_10_DMAPAGE:
		 if (pic30_is_dma_machine(global_PROCESSOR) != 2)
                     as_bad_where (fixP->fx_file, fixP->fx_line,
                   _("dmapage operator not supported"));
                 else if ((subseg_code (add_segment)) ||
			  (subseg_auxflash (add_segment)) ||
                          (subseg_psv (add_segment)) ||
                          (subseg_eedata (add_segment)) ||
                          (subseg_memory (add_segment)) ||
                          (subseg_heap (add_segment)) ||
                          (subseg_stack (add_segment)))
                        as_bad_where (fixP->fx_file, fixP->fx_line,
                   _("Cannot use dmapage on a symbol (%s) that is "
                        "not located in data memory."),
                        S_GET_NAME (fixP->fx_addsy));
		  break;
		 
	       case BFD_RELOC_PIC30_DMAOFFSET:
               case BFD_RELOC_PIC30_P_DMAOFFSET:
               case BFD_RELOC_PIC30_WORD_DMAOFFSET:
               case BFD_RELOC_PIC30_UNSIGNED_10_DMAOFFSET:
                  if ((!subseg_dma (add_segment)) &&
                      (add_segment != undefined_section)&&
		      (pic30_is_dma_machine(global_PROCESSOR) == 1))
                     as_bad_where (fixP->fx_file, fixP->fx_line,
                   _("Cannot use dmaoffset on a symbol (%s) that is "
                        "not located in a dma section."),
                        S_GET_NAME (fixP->fx_addsy));

                  else if ((pic30_is_dma_machine(global_PROCESSOR) == 2)&&                                 ((subseg_code (add_segment)) ||
                            (subseg_auxflash (add_segment)) || 
                            (subseg_psv (add_segment)) ||
                            (subseg_eedata (add_segment)) ||
                            (subseg_memory (add_segment)) ||
                            (subseg_heap (add_segment)) ||
                            (subseg_stack (add_segment))))
                        as_bad_where (fixP->fx_file, fixP->fx_line,
                   _("Cannot use dmaoffset on a symbol (%s) that is "
                        "not located in data memory."),
                        S_GET_NAME (fixP->fx_addsy));
		  break;
 
               case BFD_RELOC_PIC30_FILE_REG_WORD:
               case BFD_RELOC_PIC30_FILE_REG_WORD_WITH_DST:
               case BFD_RELOC_PIC30_FRAME_SIZE:
               case BFD_RELOC_PIC30_WORD:
               /* we permit data directives such as .byte,
                .word, .long  to reference executable symbols */
#if defined(OBJ_ELF)
		  if (strncmp(seg->name, ".debug", 6) == 0)
		  {
			  /*
			  ** debug reference is ok
			  */
		  }
		  else
#endif
                  if ((add_segment == text_section) ||
		       subseg_text_p(add_segment))
		  {
                     as_bad_where (fixP->fx_file, fixP->fx_line,
                        _("Cannot reference executable symbol (%s) in "
                          "a data context"), S_GET_NAME (fixP->fx_addsy));
		  }
                  break;

               default:
                  break;
            } /* switch (fixP->fx_r_type) */
         } /* if (add_segment != absolute_section */
      } /* else */
   } /* if (fixP->fx_addsy != NULL) */

   if (flag_debug)
      printf ("    md_apply_fix3::tc_fix_data.operand = %ld\n",
              fixP->tc_fix_data.operand);

   if (fixP->tc_fix_data.operand >= 0)
   {
      const struct pic30_operand * opnd =
         &(pic30_operands[fixP->tc_fix_data.operand]);

      long insn = bfd_getl32(fix_position);
      struct pic30_operand_value op_value;

      op_value.value = value;
      op_value.modifier = PIC30_NO_MODIFIER_FOUND;

      /* special case: test for fixup of sftac n,#16 */
      if ((fixP->fx_r_type == BFD_RELOC_PIC30_DSP_6) &&
          (op_value.value == 16))
        {
          if (global_ISA == ISA_REV_A)
            as_bad (_("Shift count of 16 is not supported in A2 devices."));
        }

      /* general case: test fixup against operand match functions */
      if (opnd->is_match)
      {
        if (!(*opnd->is_match)(&op_value))
         {
           if (opnd->info_string)
             as_bad_where (fixP->fx_file, fixP->fx_line,
                           _("Invalid value (%ld). %s"),
                           value, opnd->info_string);
           else
             as_bad_where (fixP->fx_file, fixP->fx_line,
                           _("Invalid value (%ld) for relocation %s."),
                           value, bfd_get_reloc_code_name (fixP->fx_r_type));
            fixP->fx_no_overflow = TRUE;
            *valueP = value;
            return;
         } /* if (!(*opnd->is_match)(&op_value)) */
      } /* if (opnd->is_match) */

      if (fixP->fx_r_type == BFD_RELOC_PIC30_FILE_REG_WORD_WITH_DST)
         value >>= howto->rightshift;

      if (fixP->fx_r_type == BFD_RELOC_PIC30_PCREL_BRANCH_SLIT6) {
        /* branch offsets are in instructions, not in the PC units that the
           location counter encoding that the offset is specified in */
        value = value >> 1;
      }

      insn = PIC30_ADD_OPERAND(insn, value, opnd->bits, opnd->shift);

      bfd_putl32(insn, fix_position);
   } /* if (fixP->tc_fix_data.operand >= 0) */
   else
   {
      unsigned char write_value = TRUE;

      if (fixP->tc_fix_data.fill.in_text_section)
      {
         switch (fixP->fx_r_type)
         {
            case BFD_RELOC_8:
            case BFD_RELOC_PIC30_PBYTE:
            case BFD_RELOC_PIC30_TBLOFFSET:
            case BFD_RELOC_PIC30_P_TBLOFFSET:
            case BFD_RELOC_PIC30_PSVOFFSET:
            case BFD_RELOC_PIC30_P_PSVOFFSET:
            case BFD_RELOC_PIC30_TBLPAGE:
            case BFD_RELOC_PIC30_P_TBLPAGE:
            case BFD_RELOC_PIC30_PSVPAGE:
            case BFD_RELOC_PIC30_P_PSVPAGE:
            case BFD_RELOC_PIC30_HANDLE:
            case BFD_RELOC_PIC30_P_HANDLE:
            case BFD_RELOC_PIC30_PADDR:
            case BFD_RELOC_PIC30_P_PADDR:
            case BFD_RELOC_16:
            case BFD_RELOC_PIC30_PWORD:
            case BFD_RELOC_32:
#ifdef BFD64
            case BFD_RELOC_64:
#endif
            {
               int location = fixP->tc_fix_data.current_location;
               int size = fixP->fx_size;
               int i;
               char * buf = xmalloc (size);

               md_number_to_chars (buf, value, size);

               for (i = 0; i < size; i++)
                  PIC30_INSERT_VALUE_INTO_FIX (fixP, buf[i], fix_position,
                                               location)

               free (buf);
               write_value = FALSE;

               break;
            }

            default:
               break;
         } /* switch (fixP->fx_r_type) */
      } /* if (fixP->tc_fix_data.in_text_section) */

      if (write_value)
      {
         if (flag_debug)
            printf ("    md_apply_fix3::Final value = 0x%08lx\n", (long) value);

         if (!fixP->tc_fix_data.fill.in_text_section)
           {
             /*
             ** In data memory, we must interleave
             ** the immediate data with phantom bytes
             */
             unsigned char b[4];
             valueT num1,num2;
             b[0] = value & 0xFF;
             b[1] = (value >> 8)  & 0xFF;
             b[2] = (value >> 16) & 0xFF;
             b[3] = (value >> 24) & 0xFF;

             num1 = (b[1] << 16) | b[0];
             num2 = (b[3] << 16) | b[2];
             switch (fixP->fx_size)
               {
               case 1 :
               case 2 :
                 md_number_to_chars (fix_position, num1, fixP->fx_size*2);
                 break;
               case 4 :
                 md_number_to_chars (fix_position, num1, 4);
                 md_number_to_chars (fix_position+4, num2, 4);
               }
           }
         else
           md_number_to_chars (fix_position, value, fixP->fx_size);
      } /* else */
   } /* else */

   *valueP = value;

   fixP->fx_addnumber = value;

   /*
   ** Save DO loop for later validation
   */
   switch (fixP->fx_r_type)
   {
   case BFD_RELOC_PIC30_PCREL_DO:
      if ((fixP->fx_addsy != NULL) && S_IS_DEFINED(fixP->fx_addsy))
      {
         pic30_record_do(fixP);
         break;
      }
   case BFD_RELOC_PIC30_DO_ABSOLUTE:
      as_warn_where(fixP->fx_file, fixP->fx_line,
		   _("DO loop target is not simple relocatable expression."));
      as_warn_where(fixP->fx_file, fixP->fx_line,
                   _("DO loop end restrictions not verified."));
      break;
   default:
      break;
   }

   if (flag_debug)
      printf ("<-- md_apply_fix3::exit\n");

   return; /* Return value is not used by calling function */
} /* int md_apply_fix3(...) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called if a CALL instruction cannot be relaxed.  It will
 *   Write out the second word of the CALL instruction and create fixups for
 *   the LSB and MSB portions of the symbol. It also sets the fixed length
 *   of the instruction to (PIC30_SIZE_OF_PROGRAM_WORD * 2).
 *
 ******************************************************************************/
void
pic30_relax_call (fragP)
   fragS * fragP;
{
   char * buffer = fragP->fr_fix + fragP->fr_literal;

   if (flag_debug)
      printf ("--> pic30_relax_call::begin\n");

   /* First word of the buffer was set up when frag_var was called. */
   md_number_to_chars (buffer + PIC30_SIZE_OF_PROGRAM_WORD,
                       PIC30_CALL_SECOND_PROGRAM_WORD,
                       PIC30_SIZE_OF_PROGRAM_WORD);
   fix_new (fragP, fragP->fr_fix, 2,
            fragP->fr_symbol, fragP->fr_offset, FALSE,
            BFD_RELOC_PIC30_PGM_ADDR_LSB);
   fix_new (fragP, fragP->fr_fix + PIC30_SIZE_OF_PROGRAM_WORD, 2,
            fragP->fr_symbol, fragP->fr_offset, FALSE,
            BFD_RELOC_PIC30_PGM_ADDR_MSB);

   fragP->fr_fix += (PIC30_SIZE_OF_PROGRAM_WORD * 2);

   if (flag_debug)
      printf ("<-- pic30_relax_call:exit\n");
} /* void pic30_relax_call (fragS *) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called to estimate the size of a variant frag before
 *   a relaxation pass.  Note that the relaxation state (fr_subtype)
 *   may have been changed by previous passes.
 *
 *   If the user has not requested relaxation or the symbol is undefined
 *   in this segment, change the relaxation subtype to the MAX allowable
 *   leave all further handling to md_convert_frag, and return the size
 *   of the MAX instruction. Otherwise, return the size of the instruction
 *   based on its current relaxation state.
 *
 ******************************************************************************/
int
md_estimate_size_before_relax (fragP, sec)
   fragS * fragP;
   segT sec;
{
   int rc = 0;

   if (flag_debug)
      printf ("--> md_estimate_size_before_relax::begin\n");

   /*
    * If the user has not requested relaxation, or the symbol is undefined
    * in this segment, then change the relaxation subtype to the MAX allowable
    * and leave all further handling to md_convert_frag.
    */
   if ((global_RELAX != DO_RELAX) ||
       (S_GET_SEGMENT (fragP->fr_symbol) != sec))
   {
      fragP->fr_subtype = (fragP->fr_subtype == PIC30_RELAX_RCALL)
                          ? PIC30_RELAX_CALL : PIC30_RELAX_GOTO;
      pic30_relax_call (fragP);
      frag_wane (fragP);
      rc = PIC30_SIZE_OF_PROGRAM_WORD * 2;
   } /* if */

   switch (fragP->fr_subtype)
   {
     case PIC30_RELAX_RCALL:
     case PIC30_RELAX_BRA:
       rc = PIC30_SIZE_OF_PROGRAM_WORD;
       break;

     case PIC30_RELAX_CALL:
     case PIC30_RELAX_GOTO:
       rc = PIC30_SIZE_OF_PROGRAM_WORD * 2;
       break;
   } /* switch */

   if (flag_debug)
      printf ("<-- md_estimate_size_before_relax::exit(%d)\n", rc);

   return rc;
} /* int md_estimate_size_before_relax (fragS *, segT) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called to finalize a variable length
 *   instruction based on the fr_subtype of the frag.
 *
 ******************************************************************************/
void
md_convert_frag (abfd, sec, fragP)
   bfd * abfd __attribute__ ((__unused__));
   segT sec __attribute__ ((__unused__));
   fragS * fragP;
{
   char * buffer = fragP->fr_fix + fragP->fr_literal;

   if (flag_debug)
      printf ("--> md_convert_frag::begin\n");

   switch (fragP->fr_subtype)
   {
      case PIC30_RELAX_RCALL:
      case PIC30_RELAX_BRA:
      {
         valueT opcode;
         const struct relocation_info reloc =
            pic30_get_pc_relative_relocation_info ();
         opcode = (fragP->fr_subtype == PIC30_RELAX_RCALL) ? RCALL_INSN :
                                                             BRA_INSN;

         md_number_to_chars (buffer, opcode, PIC30_SIZE_OF_PROGRAM_WORD);
         fix_new (fragP, fragP->fr_fix, reloc.fix_size, fragP->fr_symbol,
                  fragP->fr_offset, reloc.pc_relative, reloc.value);
         fragP->fr_fix += PIC30_SIZE_OF_PROGRAM_WORD;
         fragP->fr_var = 0;
         break;
      } /* case PIC30_RELAX_RCALL */

      case PIC30_RELAX_CALL:
      case PIC30_RELAX_GOTO:
         pic30_relax_call (fragP);
         break;
   } /* switch */

   if (flag_debug)
      printf ("<-- md_convert_frag::exit\n");
} /* void md_convert_frag(...) */

/******************************************************************************/

/******************************************************************************
 *
 *   This function is called prior to scanning the relax table.  The correct
 *   reach of the state must be calculated.  If the state is PIC30_RELAX_RCALL,
 *   then the reach must be calculated as the number of instructions since
 *   RCALL is a PC-relative instruction.  If the state is PIC30_RELAX_CALL,
 *   then the reach must be calculated as the value of the symbol plus the
 *   offset into the frag plus the address of the beginning of the frag.
 *
 ******************************************************************************/
void
pic30_prepare_relax_scan (fragP, address, aim, this_state, this_type)
   fragS * fragP;
   unsigned long address __attribute__ ((__unused__));
   long * aim;
   relax_substateT this_state;
   PTR this_type __attribute__ ((__unused__));
{
   if (flag_debug)
      printf ("--> pic30_prepare_relax_scan::begin\n");

   switch (this_state)
   {
      case PIC30_RELAX_RCALL:
      case PIC30_RELAX_BRA:
         *aim -= PIC30_SIZE_OF_PROGRAM_WORD;
/*          *aim -= (PIC30_SIZE_OF_PROGRAM_WORD * OCTETS_PER_BYTE); */

         /* Since this is PC Relative, we must convert to instruction count */
         *aim /= PIC30_SIZE_OF_PROGRAM_WORD;
/*          *aim /= (PIC30_SIZE_OF_PROGRAM_WORD * OCTETS_PER_BYTE); */
         break;

      case PIC30_RELAX_CALL:
      case PIC30_RELAX_GOTO:
        /*
        ** Note: the target symbol (and offset, if any)
        ** are expressed in PC units. Must convert to
        ** octets before adding the frag address.
        */
/*         *aim = ((S_GET_VALUE (fragP->fr_symbol) + fragP->fr_offset) * OCTETS_PER_BYTE) */
/*           + symbol_get_frag (fragP->fr_symbol)->fr_address; */
/* FIXME
   To be consistent with other uses of S_GET_VALUE,
   the addition of the symbol frag address has
   been deleted here. The addition of the symbol's
   fr_address field is performed by a matching change
   in symbols.c. See the binutils changelog entry
   dated Mon Jul 23 2001. - GM */

        *aim = ((S_GET_VALUE (fragP->fr_symbol) + fragP->fr_offset) * OCTETS_PER_BYTE);
         break;
   } /* switch (this_state) */

   if (flag_debug)
   {
      printf ("    pic30_prepare_relax_scan::aim = %ld\n", *aim);
      printf ("<-- pic30_prepare_relax_scan::exit\n");
   } /* if (flag_debug) */
} /* void pic30_prepare_relax_scan(...) */

/******************************************************************************/

arelent *
tc_gen_reloc (seg, fixp)
   asection *seg __attribute__ ((__unused__));
   fixS *fixp;

/******************************************************************************
 *
 *   This function is called to generate a relocation.  The resulting
 *   relocation is passed to bfd_install_relocation.
 *
 ******************************************************************************/

{
   arelent * reloc = (arelent *) xmalloc (sizeof (arelent));

   if (flag_debug)
      printf ("--> tc_gen_reloc::begin\n");

   reloc->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
   *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);

   /*
    * Frags are built with octets, and relocs count octets...
    * so no adjustment is needed here.
    */
   reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;

   reloc->howto = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);

   if (reloc->howto == (reloc_howto_type *) NULL)
   {
      as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("reloc %d not supported by object file format"),
                    (int)fixp->fx_r_type);
      return NULL;
   } /* if (reloc-howto == (reloc_howto_type *) NULL) */

   if ((fixp->fx_r_type == BFD_RELOC_VTABLE_INHERIT) ||
       (fixp->fx_r_type == BFD_RELOC_VTABLE_ENTRY))
      reloc->address = fixp->fx_offset;
   else
      reloc->addend = fixp->fx_addnumber;

   if (flag_debug)
      printf ("<-- tc_gen_reloc::exit\n");

   return reloc;
} /* arelent * tc_gen_reloc(...) */

/******************************************************************************/

valueT
md_section_align (segment, size)
   segT segment;
   valueT size;

/******************************************************************************
 *
 *   GAS will call this function for each section at the end of the assembly,
 *   to permit the CPU backend to adjust the alignment of a section.
 *
 ******************************************************************************/

{
   /* Get the alignment power for the segment */
   int align = ((PIC30_IS_REVERSE_ATTR(segment) |
                 PIC30_IS_UNORDERED_ATTR(segment)) ?
                1 : bfd_get_section_alignment (stdoutput, segment));

   /* Return the size including alignment */
   return ((size + (1 << align) - 1) & (-1 << align));
} /* valueT md_section_align(segT, valueT) */

/******************************************************************************/

