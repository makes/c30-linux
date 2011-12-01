/*
 * tc-pic30.h -- Header file for the assembler for the dsPIC30 architecture.
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
 * You should have received a copy of the GNU General Public License along
 * with GAS; see the file COPYING.  If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * This file is tc-pic30.h and contains all macros for the target cpu for 
 * PIC30.
 */

#ifndef __TC_PIC30_H__
#define __TC_PIC30_H__

#include <opcode/pic30.h>
#include <opcodes/pic30-opc.h>

#ifndef TC_PIC30
#define TC_PIC30 1
#endif

/*
** Command-line option '-g' state enumeration.
*/
enum pic30_debuginfo_state
{
   NO_GEN_DEBUGINFO,
   GEN_DEBUGINFO,
   GENNING_DEBUGINFO
};
/*
** Command-line option '-g' state variable.
*/
extern enum pic30_debuginfo_state global_DEBUGINFO;
extern void pic30_stringer (int);
#if defined(OBJ_ELF)
extern int pic30_output_leb128 PARAMS ((char *p, valueT value, int sign));
#endif

/* This macro is the BFD target name to use when creating the output file. */
#define TARGET_FORMAT pic30_target_format()
extern const char *pic30_target_format PARAMS ((void));

/*
 * This macro is the BFD architecture to pass to bfd_set_arch_mach
 * This should match the bfd/config.bfd file for the PIC30 architecture.
 */
#define TARGET_ARCH bfd_arch_pic30

/*
 * This macro is the BFD machine number to pass to bfd_set_arch_mach.  If it
 * is not defined, GAS will use 0.
 */
extern unsigned long pic30_target_mach PARAMS ((void));
#define TARGET_MACH pic30_target_mach()

/*
 * This macro should be defined to non-zero if the target is big endian, and
 * zero if the target is little endian
 */
#define TARGET_BYTES_BIG_ENDIAN 0

/*
 * If defined, GAS will call it at the end of each input file.
 */
extern void pic30_cleanup PARAMS ((void));
#define md_cleanup pic30_cleanup

/*
 * If this macro is defined, GAS will call it at the end of assembly.
 */
extern void pic30_end PARAMS ((void));
#define md_end pic30_end

/*
 * If defined the $ will be used as the current location in an expression.
 */
#define DOLLAR_DOT

/*
 * Define this macro to the lexical type of the $ character.  The default
 * value is LEX_NAME | LEX_BEGIN_NAME.  For the PIC30 assembly syntax,
 * the $ can only be used to represent the current location.
 */
#define LEX_DOLLAR LEX_BEGIN_NAME

/*
 * If you define this macro, GAS will not require pseudo-ops to start with
 * a '.' character.  Since one of the mnemonics for the PIC30 port is
 * set, which conflicts with the pseudo-op SET, we only want to except
 * pseudo-ops that start with a '.' character.
 */
#define NO_PSEUDO_DOT 0

/*
 * If you define this macro, it should return nonzero if the instruction is
 * permitted to contain an = character.  GAS will use this to decide if a =
 * is an assignment or an instruction.  In PIC30, the = character is not
 * part of the assembly language.  Always return 0.
 */
#define TC_EQUAL_IN_INSN(c, input_line_pointer) 0

/*
 * GAS will call this function when a symbol table lookup fails, before it
 * creates a new symbol.  Typically this would be used to supply symbols
 * whose name or value changes dynamically, possibly in a context sensitive
 * way.  Predefined symbols with fixed values, such as register names or
 * condition codes, are typically entered directly into the symbol table
 * when md_begin is called.
 */
#define md_undefined_symbol(name)     (0)

/*
 * If this macro is defined, GAS will call it for any symbol found in an
 * expression.  pic30 uses this to parse operands for the branch instructions
 * and for those instructions that take an implied register.
 */
extern int pic30_parse_name PARAMS ((const char *, struct expressionS *));
#define md_parse_name(name, exp, c) pic30_parse_name (name, exp)

/*
 * If this macro is defined, GAS will call it when the align directive is seen
 * in the input file.
 */
extern int pic30_do_align PARAMS ((int, const char *, int, int));
#define md_do_align(n, fill, len, max, skip_alignment) \
   if (pic30_do_align (n, fill, len, max)) goto skip_alignment

/*
 * If this macro is defined, GAS will call it at the end of assembly to do
 * special handling for an alignment directive.
 */
extern void pic30_handle_align PARAMS ((fragS *));
#define HANDLE_ALIGN(f) pic30_handle_align(f)

/*
 * You may define this macro to parse an expression used in a data allocation
 * pseudo-op such as .word.  You can use this to recognize relocation
 * directives that may appear in such directives.
 */
extern void pic30_expr (struct expressionS *, unsigned char);
#define TC_PARSE_CONS_EXPRESSION(X, N) pic30_expr(X, TRUE)

/*
 * You may define this macro to generate a fixup for a data allocation
 * pseudo-op.
 */
extern void pic30_cons_fix_new PARAMS ((fragS *, int, int,
                                        struct expressionS *));
#define TC_CONS_FIX_NEW pic30_cons_fix_new

/*
 * Define this macro to the value of the NOP opcode.  This value will be used
 * by GAS when filling a frag located in the text section.
 */
#define NOP_OPCODE 0x00

/*
 * This macro is the default value of the fill value.
 */
#define DEFAULT_FILL_VALUE 0x00

struct pic30_fill_data
{
   unsigned char in_text_section;
   unsigned char fillupper_value;
   unsigned char fillvalue_value;
   unsigned char pfillvalue_value;
   unsigned char p_directive_active;
};

extern unsigned char global_fillupper_value;
extern unsigned char global_fillvalue_value;
extern unsigned char global_pfillvalue_value;
extern unsigned long global_current_location;
extern unsigned char global_p_directive_active;

struct pic30_fix_data
{
   struct pic30_fill_data fill;
   unsigned long current_location;
   long operand;
};

/*
 * A structure to hold the target specific fields of a fixup.
 */
#define TC_FIX_TYPE struct pic30_fix_data

/*
 * A C statement to initialize the target specific fields of fixup FIXP.
 * These fields are defined with the TC_FIX_TYPE macro.
 */
#define TC_INIT_FIX_DATA(FIXP)                                                 \
do {                                                                           \
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))                 \
   {                                                                           \
      (FIXP)->tc_fix_data.fill.in_text_section = TRUE;                         \
      (FIXP)->tc_fix_data.fill.fillupper_value = global_fillupper_value;       \
      (FIXP)->tc_fix_data.fill.fillvalue_value = global_fillvalue_value;       \
      (FIXP)->tc_fix_data.fill.pfillvalue_value = global_pfillvalue_value;     \
      (FIXP)->tc_fix_data.current_location = global_current_location;          \
      (FIXP)->tc_fix_data.fill.p_directive_active = global_p_directive_active; \
   } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */         \
   else                                                                        \
   {                                                                           \
      (FIXP)->tc_fix_data.fill.in_text_section = FALSE;                        \
      (FIXP)->tc_fix_data.fill.fillupper_value = NOP_OPCODE;                   \
      (FIXP)->tc_fix_data.fill.fillvalue_value = DEFAULT_FILL_VALUE;           \
      (FIXP)->tc_fix_data.fill.pfillvalue_value = DEFAULT_FILL_VALUE;          \
      (FIXP)->tc_fix_data.current_location = 0;                                \
      (FIXP)->tc_fix_data.fill.p_directive_active = FALSE;                     \
   } /* else */                                                                \
   (FIXP)->tc_fix_data.operand = -1;                                           \
} while (0)

/*
 * A C statement to output target specific debugging information for fixup
 * FIXP to the output stream FILE.  This macro is called by print_fixup.
 */
#define TC_FIX_DATA_PRINT(FILE, FIXP)                                       \
do {                                                                        \
   fprintf((FILE), "\ttc_fix_data.fill.in_text_section = %s\n",             \
           (FIXP)->tc_fix_data.fill.in_text_section ? "TRUE" : "FALSE");    \
   fprintf((FILE), "\ttc_fix_data.fill.fillupper_value = 0x%02x\n",         \
           (FIXP)->tc_fix_data.fill.fillupper_value);                       \
   fprintf((FILE), "\ttc_fix_data.fill.fillvalue_value = 0x%02x\n",         \
           (FIXP)->tc_fix_data.fill.fillvalue_value);                       \
   fprintf((FILE), "\ttc_fix_data.fill.pfillvalue_value = 0x%02x\n",        \
           (FIXP)->tc_fix_data.fill.pfillvalue_value);                      \
   fprintf((FILE), "\ttc_fix_data.current_location = %ld\n",                \
           (FIXP)->tc_fix_data.current_location);                           \
   fprintf((FILE), "\ttc_fix_data.fill.p_directive_active = %s\n",          \
           (FIXP)->tc_fix_data.fill.p_directive_active ? "TRUE" : "FALSE"); \
   fprintf((FILE), "\ttc_fix_data.operand = %ld\n",                         \
           (FIXP)->tc_fix_data.operand);                                    \
} while (0)

/*
 * A structure to hold the target specific fields of a frag.
 */
#define TC_FRAG_TYPE struct pic30_fill_data

/*
 * A C statement to output target specific debugging information for frag
 * FRAGP to the output stream FILE.  This macro is called by print_frag.
 */
#define TC_FRAG_DATA_PRINT(FILE, FRAGP)                                       \
do {                                                                          \
   fprintf((FILE), "\ttc_frag_data.in_text_section = %s\n",                   \
           (FRAGP)->tc_frag_data.in_text_section ? "TRUE" : "FALSE");         \
   fprintf((FILE), "\ttc_frag_data.fillupper_value = 0x%02x\n",               \
           (FRAGP)->tc_frag_data.fillupper_value);                            \
   fprintf((FILE), "\ttc_frag_data.fillvalue_value = 0x%02x\n",               \
           (FRAGP)->tc_frag_data.fillvalue_value);                            \
   fprintf((FILE), "\ttc_frag_data.pfillvalue_value = 0x%02x\n",              \
           (FRAGP)->tc_frag_data.pfillvalue_value);                           \
   fprintf((FILE), "\ttc_frag_data.p_directive_active = %s\n",                \
           (FRAGP)->tc_frag_data.p_directive_active ? "TRUE" : "FALSE");      \
} while (0)

/*
 * A C statement to initialize the target specific fields of frag FRAGP.
 * These fields are defined with the TC_FRAG_TYPE macro.
 */
#define TC_FRAG_INIT(FRAGP)                                                 \
do {                                                                        \
   if ((subseg_text_p (now_seg)) || (now_seg == text_section))              \
   {                                                                        \
      (FRAGP)->tc_frag_data.in_text_section = TRUE;                         \
      (FRAGP)->tc_frag_data.fillupper_value = global_fillupper_value;       \
      (FRAGP)->tc_frag_data.fillvalue_value = global_fillvalue_value;       \
      (FRAGP)->tc_frag_data.pfillvalue_value = global_pfillvalue_value;     \
      (FRAGP)->tc_frag_data.p_directive_active = global_p_directive_active; \
   } /* if ((subseg_text_p (now_seg)) || (now_seg == text_section)) */      \
   else                                                                     \
   {                                                                        \
      (FRAGP)->tc_frag_data.in_text_section = FALSE;                        \
      (FRAGP)->tc_frag_data.fillupper_value = NOP_OPCODE;                   \
      (FRAGP)->tc_frag_data.fillvalue_value = DEFAULT_FILL_VALUE;           \
      (FRAGP)->tc_frag_data.pfillvalue_value = DEFAULT_FILL_VALUE;          \
      (FRAGP)->tc_frag_data.p_directive_active = FALSE;                     \
   } /* else */                                                             \
} while (0)

/*
 * A structure to hold the target specific fields of a section.
 */
typedef struct pic30_section_info {
   unsigned long current_location;
   unsigned char fillupper_value;
   unsigned char fillvalue_value;
   unsigned char pfillvalue_value;
   unsigned char p_directive_active;
} pic30_section_infoS;

#define TC_SEGMENT_INFO_TYPE pic30_section_infoS

/*
 * This should just call either number_to_chars_bigendian or
 * number_to_chars_littleendian, whichever is appropriate.  On targets that
 * support options to change the endianness, which function to call is a
 * runtime decision.  On other targets, md_number_to_chars can be a simple
 * macro.
 */
#define md_number_to_chars(buf, val, n) \
   ( (TARGET_BYTES_BIG_ENDIAN) ? \
     number_to_chars_bigendian((buf), (val), (n)) : \
     number_to_chars_littleendian((buf), (val), (n)) )

/*
 * If WORKING_DOT_WORD is defined, GAS will not do broken word processing.
 * Broken word processing is not needed if the .word directive emits a value
 * large enough to contain an address (or, more correctly, any possible
 * difference between two addresses.  In the initial release of the C compiler
 * the pointer size will be 16 bits.  This means that broken word processing
 * is not needed.
 */
#define WORKING_DOT_WORD

/*
 * If you do not define md_relax_frag, you may define this macro as a table of
 * relax_typeS structures.  The machine independent code knows how to use
 * such a table to relax PC relative calls (kind of... See
 * md_prepare_relax_scan).
 */
struct relax_type
{
  /* Forward reach. Signed number. > 0.  */
  long rlx_forward;
  /* Backward reach. Signed number. < 0.  */
  long rlx_backward;

  /* Bytes length of this address.  */
  unsigned char rlx_length;

  /* Next longer relax-state.  0 means there is no 'next' relax-state.  */
  relax_substateT rlx_more;
};

typedef struct relax_type relax_typeS;

extern const struct relax_type md_relax_table[];
#define TC_GENERIC_RELAX_TABLE md_relax_table

/*
 * If defined, it is a C statement that is invoked prior to scanning the
 * relax table.
 */
extern void pic30_prepare_relax_scan PARAMS ((fragS *, unsigned long,
                                              long *, relax_substateT,
                                              PTR));
#define md_prepare_relax_scan(fragP, address, aim, this_state, this_type) \
   pic30_prepare_relax_scan (fragP, address, &aim, this_state, this_type)

/*
 * Call md_apply_fix3 instead of md_apply_fix.  This gives you the segment
 * as a third argument
 */
#define MD_APPLY_FIX3

/*
 * If this macro is defined, it means that md_apply_fix[3] correctly sets the
 * fx_done field in the fixup.
 */
#define TC_HANDLES_FX_DONE

/*
 * You may define this macro to indicate whether a fixup against a locally
 * defined symbol should be adjusted to be against the section symbol.  It
 * should return non-zero if the adjustment is acceptable.
 */
extern int pic30_fix_adjustable PARAMS ((struct fix *));
#define tc_fix_adjustable pic30_fix_adjustable

/*
 * PC relative operands are relative to the start of the following instruction
 * Set to 0 because the assembler does not fill in the pc-relative instructions.
 * The assembler leaves this to the linker.  The only thing that the assembler
 * encodes is any offset.
 */
#define MD_PCREL_FROM_SECTION(FIXP, SEC) 0

/*
 * If defined, GAS will call this with the name of the symbol prior to defining
 * a label.
 */
extern void pic30_frob_label_before_define PARAMS ((const char *));
#define tc_frob_label_before_define(name) pic30_frob_label_before_define(name)

/*
 * If defined, GAS will call this to see if a symbol should be excluded
 * from the object file.
 */
extern int pic30_frob_symbol PARAMS ((symbolS * ));
#define tc_frob_symbol(sym, punt) punt = pic30_frob_symbol (sym)

/*
 * If defined, GAS will call this after relocations have been written
 * to the object file
 */
extern void pic30_frob_file_after_relocs PARAMS ((void));
#define tc_frob_file_after_relocs() pic30_frob_file_after_relocs ()

#define TC_FORCE_RELOCATION(FIXP)                             \
   (((FIXP)->fx_done == 0) ||                                 \
    ((FIXP)->fx_r_type == BFD_RELOC_PIC30_BRANCH_ABSOLUTE) || \
    ((FIXP)->fx_r_type == BFD_RELOC_PIC30_DO_ABSOLUTE))

#define NEED_INDEX_OPERATOR

/*
 * A string to use on the header line of a listing.  The default value is
 * simply "GAS LISTING"
 */
#define LISTING_HEADER "MPLAB ASM30 Listing: "

/*
 * The number of bytes to put into a word in a listing.  This affects the way
 * the bytes are clumped together in the listing.  For example, a value of 2
 * might print 1234 5678 where a value of 1 would print 12 34 56 78.  The
 * default value is 4.
 */
#define LISTING_WORD_SIZE 1

/*
 * The number of listing continuation lines. This affects how the hex data
 * for long constants is displayed. If the constant requires more lines than
 * this, the hex display is truncated. The user can override this value
 * with the --listing-cont-lines option. If this macro is not defined,
 * the default value is 4.
 */
#define LISTING_LHS_CONT_LINES 8

/*
 * A function to perform automatic alignment of the bss section to
 * accommodate local common objects. If this macro is not defined,
 * a default value is derived based on the size of the object. For PIC30,
 * we don't need any alignment for local common objects.
 */
#define TC_IMPLICIT_LCOMM_ALIGNMENT(SIZE, P2VAR) {}

/******************************************************************************
 * New macros used by gas/as.c
 ******************************************************************************/

/*
 * If this macro is defined, GAS will not accept the --gstabs command-line
 * option.
 */
#if (1)
#define TC_NO_GSTABS
#endif

/*
** If this macro is defined, GAS will not accept the --gdwarf2 command-line
** option.
**
** dsPIC30 uses the '-g' command-line option to enable auto-debug-info.
*/
#if (1)
#define TC_NO_GDWARF2
#endif
/*
** Dwarf-2 debug address size. This is four bytes for dsPIC30 elf-32.
*/
#define DWARF2_ADDR_SIZE(bfd) 	4

/******************************************************************************
 * New macros used by gas/expr.h
 ******************************************************************************/

/*
 * A structure to hold the target specific fields of an expression.
 */
struct pic30_expression_data {
   unsigned char immediate;
   enum pic30_modifier modifier;
};

/*
 * If you define this macro, this will be the type of the X_md field in the
 * expression structure; otherwise, it will be an unsigned short.
 *
 * NOTE:  This is a new macro added for the PIC30 port.
 */
#define TC_EXPRESSION_TYPE struct pic30_expression_data

/******************************************************************************
 * New macros used by gas/read.c
 ******************************************************************************/

extern unsigned char pic30_do_org PARAMS ((register segT, PTR,
                                           long, unsigned char));
#define TC_DO_ORG pic30_do_org

/*
 * If this macro is defined, GAS will call this to emit a data allocation
 * pseudo-op.  out_string is the buffer to output.  length is the length of
 * output_string.
 */
extern unsigned char pic30_cons_emit PARAMS ((char *, unsigned int));
#define TC_CONS_EMIT(out_string, length) pic30_cons_emit(out_string, length)

extern unsigned char pic30_cons_emit_expr PARAMS ((struct expressionS *,
                                                   unsigned int));
#define TC_CONS_EMIT_EXPR(EXP, NBYTES) pic30_cons_emit_expr(EXP, NBYTES)

extern unsigned char global_must_align_location_counter;
#define TC_FRAG_APPEND_1_CHAR(c)                                  \
   {                                                              \
      if ((subseg_text_p (now_seg)) || (now_seg == text_section)) \
      {                                                           \
         PIC30_INSERT_VALUE_INTO_PROGRAM_MEMORY(c);               \
         global_must_align_location_counter = TRUE;               \
      } /* if */                                                  \
      else                                                        \
      {                                                           \
         PIC30_INSERT_VALUE_INTO_DATA_MEMORY(c);                  \
      } /* else */                                                \
   }

/******************************************************************************
 * New macros used by gas/listing.c
 ******************************************************************************/

/*
 * If this macro is defined, GAS will call it to display symbol information
 * in the listing file.
 */
extern void pic30_list_symbol_info PARAMS ((FILE *));
#define tc_list_symbol_information(OUTFILE) pic30_list_symbol_info (OUTFILE)

/*
 * If this macro is defined, GAS will call it to display section information
 * in the listing file.
 */
extern void pic30_list_section_info PARAMS ((FILE *));
#define tc_list_section_information(OUTFILE) pic30_list_section_info (OUTFILE)

/******************************************************************************
 * PIC30 SPECIFIC MACROS
 ******************************************************************************/

/*
 * This macro is the value that should be placed in the program memory's
 * phantom byte when writing to program memory.
 */
#define PIC30_PHANTOM_BYTE 0x00

#define PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION 2
#define PIC30_PHANTOM_BYTE_LOCATION 3

#define PIC30_FRAG_APPEND_1_CHAR(value) \
   {                                    \
      FRAG_APPEND_1_CHAR(value);        \
      global_current_location++;        \
   }

/******************************************************************************/

#define PIC30_INSERT_VALUE_INTO_PROGRAM_MEMORY(value)                    \
   while (TRUE)                                                          \
   {                                                                     \
      if (((global_current_location % PIC30_SIZE_OF_PROGRAM_WORD) ==     \
           PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) &&                  \
          (!global_p_directive_active))                                  \
      {                                                                  \
         PIC30_FRAG_APPEND_1_CHAR(global_fillupper_value);               \
      } /* if (PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) */              \
      else if ((global_current_location % PIC30_SIZE_OF_PROGRAM_WORD) == \
               PIC30_PHANTOM_BYTE_LOCATION)                              \
      {                                                                  \
         PIC30_FRAG_APPEND_1_CHAR(PIC30_PHANTOM_BYTE);                   \
      } /* else if (PIC30_PHANTOM_BYTE_LOCATION) */                      \
      else                                                               \
      {                                                                  \
         PIC30_FRAG_APPEND_1_CHAR(value);                                \
         break;                                                          \
      } /* else */                                                       \
   } /* while (TRUE) */

/******************************************************************************/

#define PIC30_INSERT_VALUE_INTO_FIX(fix, value, output, location)        \
   while (TRUE)                                                          \
   {                                                                     \
      if (((location % PIC30_SIZE_OF_PROGRAM_WORD) ==                    \
           PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) &&                  \
          (!fix->tc_fix_data.fill.p_directive_active))                   \
      {                                                                  \
         *output++ = fix->tc_fix_data.fill.fillupper_value;              \
         location++;                                                     \
      } /* if (PIC30_PROGRAM_MEMORY_UPPER_BYTE) */                       \
      else if ((location % PIC30_SIZE_OF_PROGRAM_WORD) ==                \
                PIC30_PHANTOM_BYTE_LOCATION)                             \
      {                                                                  \
         *output++ = PIC30_PHANTOM_BYTE;                                 \
         location++;                                                     \
      } /* else if (PIC30_PHANTOM_BYTE_LOCATION) */                      \
      else                                                               \
      {                                                                  \
         *output++ = value;                                              \
         location++;                                                     \
         break;                                                          \
      } /* else */                                                       \
   } /* while (TRUE) */

/******************************************************************************/

#define PIC30_CAN_WRITE(byte, p_directive_active)                           \
   (((byte % PIC30_SIZE_OF_PROGRAM_WORD) != PIC30_PHANTOM_BYTE_LOCATION) && \
    (((byte % PIC30_SIZE_OF_PROGRAM_WORD) !=                                \
      PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) ||                          \
     (((byte % PIC30_SIZE_OF_PROGRAM_WORD) ==                               \
       PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) && (p_directive_active))))

#endif

/******************************************************************************/

#define OCTETS_PER_BYTE_POWER 1
#define OCTETS_PER_BYTE 2

#define PIC30_INSERT_VALUE_INTO_DATA_MEMORY(value)                       \
   {                                                                     \
     char * frag_output = frag_more (OCTETS_PER_BYTE);                   \
     *frag_output++ = (value);                                           \
     *frag_output++ = (PIC30_PHANTOM_BYTE);                              \
   }

#define PIC30_INSERT_DATA_INTO_DATA_MEMORY(ptr,len)                      \
   {                                                                     \
     int i;                                                              \
     char *p = (char *)(ptr);                                            \
     char * frag_output = frag_more (len * OCTETS_PER_BYTE);             \
     for ( i=0; i<len; i++)                                              \
     {                                                                   \
       *frag_output++ = *p++;                                            \
       *frag_output++ = (PIC30_PHANTOM_BYTE);                            \
     }                                                                   \
   }

/******************************************************************************/

#define PIC30_PAD_PROGRAM_MEMORY_AFTER_INSERT()                          \
   {                                                                     \
      if (((global_current_location % PIC30_SIZE_OF_PROGRAM_WORD) ==     \
           PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) &&                  \
          (!global_p_directive_active))                                  \
      {                                                                  \
         PIC30_FRAG_APPEND_1_CHAR(global_fillupper_value);               \
      } /* if (PIC30_PROGRAM_MEMORY_UPPER_BYTE_LOCATION) */              \
      if ((global_current_location % PIC30_SIZE_OF_PROGRAM_WORD) ==      \
               PIC30_PHANTOM_BYTE_LOCATION)                              \
      {                                                                  \
         PIC30_FRAG_APPEND_1_CHAR(PIC30_PHANTOM_BYTE);                   \
      } /* if (PIC30_PHANTOM_BYTE_LOCATION) */                           \
   }

/******************************************************************************/

#define PIC30_VALID_PC_ADDRESS(byte)                                     \
   (((byte) % PIC30_SIZE_OF_PROGRAM_WORD) == 0)


/******************************************************************************/

#define PIC30_FRAG_IN_DATA_MEMORY(frag)                                  \
   (frag->tc_frag_data.in_text_section == FALSE)

#define PIC30_FRAG_IN_PROGRAM_MEMORY(frag)                               \
   (frag->tc_frag_data.in_text_section == TRUE)

/******************************************************************************/

#define PIC30_CAN_WRITE_DATA(byte)                                       \
   (((byte) % PIC30_SIZE_OF_DATA_WORD) == 0)

/*
 * fix for SSR 16051
 *
 * maintain the last used code style section in the variable 
 * pic30_last_code_section...
 *
 * This variable will *always* be a code section, it is initialized to
 * text_section and only ever changes if subseg_set() is called with a code
 * style section.
 */
extern segT pic30_last_code_section;

extern void pic30_init(int argc, char **argv);
#define HOST_SPECIAL_INIT pic30_init

extern char *pic30_resource_version;
