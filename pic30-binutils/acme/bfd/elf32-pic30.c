/* Microchip dsPIC-specific support for 32-bit ELF
   Copyright 1993, 1995, 1999 Free Software Foundation, Inc.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/pic30.h"
#include "pic30-utils.h"


#define TARGET_LITTLE_SYM	pic30_elf32_vec
#define TARGET_LITTLE_NAME	"elf32-pic30"
#define ELF_ARCH		bfd_arch_pic30
#define ELF_MACHINE_CODE	EM_DSPIC30F
#define ELF_MAXPAGESIZE  	1 /* == No memory-management hardware */

/* Use REL instead of RELA for COFF compatibility */
#define	PIC30_USE_RELA		0
#if (PIC30_USE_RELA)
#define USE_RELA		1
#define	SRC_MASK(m)		0
#else
#define USE_REL			1
#define	SRC_MASK(m)		m
#endif
#define	PIC30_PIP		(!PIC30_USE_RELA) /* partial inplace ? */

#define bfd_elf32_bfd_reloc_type_lookup pic30_elf32_bfd_reloc_type_lookup
#define bfd_elf32_new_section_hook	pic30_bfd_elf32_new_section_hook
#define bfd_elf32_set_section_contents	pic30_bfd_elf32_set_section_contents
#define bfd_elf32_bfd_final_link	pic30_final_link
#if 1
#define bfd_elf32_bfd_link_hash_table_create \
					_bfd_elf_link_hash_table_create
#endif
#if 1 
#define bfd_elf32_bfd_link_add_symbols \
					bfd_elf32_bfd_link_add_symbols
#endif

#define elf_backend_final_write_processing  pic30_final_write_processing
#define elf_backend_object_p            pic30_backend_object_p

#if (PIC30_USE_RELA)
#define elf_info_to_howto		pic30_info_to_howto_rela
#else
#define elf_info_to_howto_rel		pic30_info_to_howto_rel
#endif
#define elf_backend_section_from_bfd_section \
				pic30_elf_backend_section_from_bfd_section

#define elf_backend_gc_mark_hook _bfd_pic30_elf_gc_mark_hook
#define elf_backend_gc_sweep_hook _bfd_pic30_elf_gc_sweep_hook 

#define elf_backend_can_gc_sections 1




int pic30_global_warning = 0;
bfd *pic30_output_bfd;

/* Note: no default names for the startup modules */
const char *pic30_startup0_file;
const char *pic30_startup1_file;

/* Data Structures for the Handle Jump Table */
bfd *handle_bfd;
unsigned char *handle_data;
struct handle_hash_table *handles;

/* Data Structures for the Undefined Symbol Table */
struct pic30_undefsym_table *undefsyms;

/* Data Structures for the Data Init Template */
bfd *init_bfd;
unsigned char *init_data;
asection *init_template = 0;

/* Data Structures for the Default Interrupt Handler */
bfd *isr_bfd;
unsigned char *isr_data;

/* Data Structures for Input Data Sections */
struct pic30_section *data_sections;

/* Data Structures for CodeGuard */
bfd_vma base_address[SEGMENTS][MEMORIES];
bfd_vma  end_address[SEGMENTS][MEMORIES];

/* Variables set by pic30 linker options */
bfd_boolean pic30_debug = 0;
bfd_boolean pic30_data_init = TRUE;
bfd_boolean pic30_has_data_init_option = 0;
bfd_boolean pic30_handles = TRUE;
bfd_boolean pic30_has_handles_option = 0;
bfd_boolean pic30_pack_data = TRUE;
bfd_boolean pic30_has_pack_data_option = 0;
bfd_boolean pic30_has_stack_option = 0;
unsigned int pic30_stack_size = 16;
bfd_boolean pic30_has_stackguard_option = 0;
unsigned int pic30_stackguard_size = 16;
bfd_boolean pic30_has_heap_option = 0;
bfd_boolean pic30_heap_required = 0;
unsigned int pic30_heap_size = 0;
bfd_boolean pic30_smart_io = TRUE;
bfd_boolean pic30_has_smart_io_option = 0;
bfd_boolean pic30_allocate = TRUE;
bfd_boolean pic30_has_allocate_option = 0;
bfd_boolean pic30_report_mem = 0;
bfd_boolean pic30_isr = TRUE;
bfd_boolean pic30_has_isr_option = 0;
bfd_boolean pic30_has_fill_upper_option = 0;
unsigned int pic30_fill_upper = 0;
bfd_boolean pic30_force_link = 0;
bfd_boolean pic30_has_force_link_option = 0;
bfd_boolean pic30_has_processor_option = 0;
bfd_boolean pic30_has_boot_option = 0;
bfd_boolean pic30_has_secure_option = 0;
bfd_boolean pic30_has_general_option = 0;
bfd_boolean pic30_has_user_boot = 0;
unsigned int pic30_boot_flash_size = 0;
unsigned int pic30_boot_ram_size = 0;
bfd_boolean pic30_has_user_secure = 0;
unsigned int pic30_secure_flash_size = 0;
unsigned int pic30_secure_ram_size = 0;
bfd_boolean pic30_select_objects = TRUE;

/* Other state variables */
bfd_boolean pic30_has_user_startup = 0;
bfd_boolean pic30_has_user_default_interrupt = 0;
bfd_boolean pic30_has_managed_psv = 0;
bfd_boolean pic30_has_psv_trap_errata = 0;

/* The base of data memory, which is
   also the limit of SFR address space */
bfd_vma data_base = 0;
bfd_boolean data_base_defined = FALSE;

/* Variables related to dma sections,
   needed by the dmaoffset() operator. */
bfd_boolean need_dma_memory = FALSE;
bfd_vma dma_base = 0;
bfd_boolean dma_base_defined = FALSE;
bfd_vma dma_end = 0;
bfd_boolean dma_end_defined = FALSE;

/* External function prototypes */
extern struct bfd_hash_entry *pic30_undefsym_newfunc
  PARAMS ((struct bfd_hash_entry *, struct bfd_hash_table *, const char *));
extern void pic30_undefsym_traverse
  PARAMS ((struct pic30_undefsym_table *,
           bfd_boolean (*func) PARAMS ((struct bfd_hash_entry *, PTR)), PTR));
extern struct pic30_undefsym_table *pic30_undefsym_init
  PARAMS ((void));

/* Local function prototypes */
static void pic30_final_write_processing
  PARAMS ((bfd *, bfd_boolean));
static bfd_boolean pic30_backend_object_p
  PARAMS ((bfd *));

static bfd_boolean pic30_bfd_elf32_set_section_contents
  PARAMS ((bfd *, sec_ptr, PTR, file_ptr, bfd_size_type));
static void bfd_pic30_check_reloc_flag
  PARAMS ((bfd *, asection *, PTR));
static bfd_boolean pic30_final_link
  PARAMS ( (bfd *, struct bfd_link_info *));
static reloc_howto_type *pic30_elf32_bfd_reloc_type_lookup
  PARAMS ((bfd *, bfd_reloc_code_real_type));
static bfd_boolean pic30_bfd_elf32_new_section_hook
  PARAMS ((bfd *, asection *));
static void pic30_info_to_howto_rel
  PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
#if (PIC30_USE_RELA)
static void pic30_info_to_howto_rela
  PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
#endif

static asection* _bfd_pic30_elf_gc_mark_hook PARAMS ((asection * ,struct bfd_link_info *, Elf_Internal_Rela *, struct elf_link_hash_entry *, Elf_Internal_Sym *));

static bfd_boolean _bfd_pic30_elf_gc_sweep_hook PARAMS ((bfd *,  struct bfd_link_info *, asection *, const Elf_Internal_Rela *));

static void bfd_pic30_print_section_header
  PARAMS ((void));
static void bfd_pic30_report_sections
  PARAMS ((bfd *, asection *, PTR));
static void bfd_pic30_clean_section_names
  PARAMS ((bfd *, asection *, PTR));
static void bfd_pic30_process_data_section
  PARAMS ((asection *, PTR));
static void bfd_pic30_write_data_header
  PARAMS ((unsigned char **,  bfd_vma,  bfd_vma, int));

static bfd_reloc_status_type pic30_elf32_is_even_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_is_file_reg_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_data_directive
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_generic
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_operators
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_p_operators
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_program_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_pc_relative
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_p_data_directive
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type pic30_elf32_perform_dmaoffset_dmapage
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));


static void pic30_elf32_insert_bytes PARAMS ((bfd *, PTR, int,
   bfd_size_type, bfd_vma, unsigned char, unsigned char));
static void pic30_elf32_insert_bytes_data_mem
   PARAMS ((bfd *, PTR, int, bfd_size_type, bfd_vma));
static bfd_vma pic30_elf32_extract_bytes
   PARAMS ((bfd *, PTR, int, bfd_size_type, unsigned char, unsigned char));
static bfd_vma pic30_elf32_extract_bytes_data_mem
   PARAMS ((bfd *, PTR, int, bfd_size_type));
static bfd_vma bfd_pic30_handle
     PARAMS ((bfd *, bfd_vma, asymbol *));

extern bfd_reloc_status_type pic30_bfd_reloc_range_check
  PARAMS ((reloc_howto_type *, bfd_vma, bfd *, asymbol *, char **));


/* local statics */
static bfd *link_output_bfd;

/* defined in bfd/cpu-pic30.c */
extern int pic30_is_generic_machine(unsigned int machine);
extern int pic30_is_dma_machine(const bfd_arch_info_type *);
extern int pic30_is_eds_machine(const bfd_arch_info_type *);
extern int pic30_proc_family(const bfd_arch_info_type *);
extern const bfd_arch_info_type * global_PROCESSOR;

/* Include the target file again for this target.  */
#include "elf32-target.h"

#define RELOC_SPECIAL_FN_FILE_REG_ADDRESS pic30_elf32_is_file_reg_address
#define RELOC_SPECIAL_FN_EVEN_ADDRESS	pic30_elf32_is_even_address
#define RELOC_SPECIAL_FN_GENERIC	pic30_elf32_perform_generic
#define RELOC_SPECIAL_FN_DATA_DIRECTIVE pic30_elf32_perform_data_directive
#define RELOC_SPECIAL_FN_PGM_ADDR	pic30_elf32_perform_program_address
#define RELOC_SPECIAL_FN_PCREL		pic30_elf32_perform_pc_relative
#define RELOC_SPECIAL_FN_OPERATORS	pic30_elf32_perform_operators
#define RELOC_SPECIAL_FN_P_OPERATORS	pic30_elf32_perform_p_operators
#define RELOC_SPECIAL_FN_P_DATA_DIRECTIVE pic30_elf32_perform_p_data_directive
#define RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE pic30_elf32_perform_dmaoffset_dmapage

/* The relocation table used for SHT_REL sections.  */

/*
** type 
** rightshift - Value the final relocation is shifted right by
** size (0 = byte, 1 = short, 2 = long)
** bitsize - Number of bits in the item to be relocated.
** pc_relative - Boolean specifying if the relocation is relative to the
**               location
** bitpos - Relocated value is left shifted by this much
** complain_on_overflow - Type of overflow error that should be checked for
** special_function - This function is called when not NULL.
** name - textual name
** partial_inplace - When performing partial link, do you modify the
**                   relocation instead of the data.
** src_mask - Which parts of the read in data are to be used in the
**            relocation sum.
** dst_mask - Selects which parts of the instruction are replaced into
**            the instruction.
** pcrel_offset - Signals whether the value of the pc is left in the slot of
**                the instruction.
**
** ========================================================
** Entries in this array should be in the same order as the
** enumeration elf_pic30_reloc_type in include/elf/pic30.h.
** ========================================================
*/
static reloc_howto_type elf_pic30_howto_table_rel[] =
{
  /* No relocation.  */
  HOWTO (R_PIC30_NONE,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 0,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 bfd_elf_generic_reloc,		/* special_function */
	 "R_PIC30_NONE",		/* name */
	 FALSE,				/* partial_inplace? */
	 SRC_MASK(0),			/* src_mask */
	 0,				/* dst_mask */
	 FALSE),			/* pcrel_offset */

  /* generic 8 bit relocation.  */
  HOWTO(R_PIC30_8,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 8,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* pc_relative? */
         RELOC_SPECIAL_FN_DATA_DIRECTIVE,
	 ".byte",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000000ff),		/* src_mask */
	 0x000000ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset */
  /* generic 16 bit relocation.  */
  HOWTO (R_PIC30_16,			/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 RELOC_SPECIAL_FN_DATA_DIRECTIVE,/* special_function */
	 ".word",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset */

  /* generic 32 bit relocation.  */
  HOWTO (R_PIC30_32,			/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 32,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 RELOC_SPECIAL_FN_DATA_DIRECTIVE,/* special_function */
	 ".long",			/* name */
	 FALSE,				/* partial_inplace? */
	 SRC_MASK(0xffffffff),		/* src_mask */
	 0xffffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset */
  /* file-register */
  HOWTO(R_PIC30_FILE_REG_BYTE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 13,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_FILE_REG_ADDRESS,
	 "FILE REG - BYTE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x001fff),		/* src_mask */
	 0x001fff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* file-register */
  HOWTO(R_PIC30_FILE_REG,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 13,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_FILE_REG_ADDRESS,
	 "FILE REG",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x001fff),		/* src_mask */
	 0x001fff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* file-register */
  HOWTO(R_PIC30_FILE_REG_WORD,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_EVEN_ADDRESS,	/* special_function */
	 "FILE REG - WORD",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffff),		/* src_mask */
	 0x00ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* file-register */
  HOWTO (R_PIC30_FILE_REG_WORD_WITH_DST,/* type */
	 1,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 15,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_EVEN_ADDRESS,	/* special_function */
	 "FILE REG - WORD with DST.",	/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x07fff0),		/* src_mask */
	 0x07fff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word */
  HOWTO(R_PIC30_WORD,			/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "WORD",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program byte */
  HOWTO(R_PIC30_PBYTE,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 8,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_DATA_DIRECTIVE,
	 ".pbyte",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000000ff),		/* src_mask */
	 0x000000ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program word */
  HOWTO(R_PIC30_PWORD,			/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 24,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_DATA_DIRECTIVE,
	 ".pword",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffffff),		/* src_mask */
	 0x00ffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* handle */
  HOWTO(R_PIC30_HANDLE,			/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "HANDLE (pointer to function)",/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program address in data memory */
  HOWTO(R_PIC30_PADDR,			/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 32,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "PADDR",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0xffffffff),		/* src_mask */
	 0x00ffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program address in program memory */
  HOWTO(R_PIC30_P_PADDR,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 24,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-PADDR",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffffff),		/* src_mask */
	 0x00ffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* psv offset */
  HOWTO(R_PIC30_PSVOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "PSVOFFSET",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* table offset */
  HOWTO(R_PIC30_TBLOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "TBLOFFSET",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word handle */
  HOWTO(R_PIC30_WORD_HANDLE,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - HANDLE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word psv offset */
  HOWTO(R_PIC30_WORD_PSVOFFSET,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - PSVOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* psv page */
  HOWTO(R_PIC30_PSVPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "PSVPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program psv page */
  HOWTO(R_PIC30_P_PSVPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-PSVPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word psv page */
  HOWTO(R_PIC30_WORD_PSVPAGE,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - PSVPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word table offset */
  HOWTO(R_PIC30_WORD_TBLOFFSET,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - TBLOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* table page */
  HOWTO(R_PIC30_TBLPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "TBLPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program table page */
  HOWTO(R_PIC30_P_TBLPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-TBLPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word table page */
  HOWTO(R_PIC30_WORD_TBLPAGE,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - TBLPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program handle */
  HOWTO(R_PIC30_P_HANDLE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-HANDLE",			/* name */
         TRUE,				/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program psv offset */
  HOWTO(R_PIC30_P_PSVOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-PSVOFFSET",			/* name */
         TRUE,				/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program table offset */
  HOWTO(R_PIC30_P_TBLOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-TBLOFFSET",			/* name */
         TRUE,				/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* pc-relative branch */
  HOWTO(R_PIC30_PCREL_BRANCH,		/* type */
	 1,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 TRUE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
	 "PC RELATIVE BRANCH",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffff),		/* src_mask */
	 0x00ffff,			/* dst_mask */
	 TRUE),				/* pcrel_offset? */
  /* absolute branch */
  HOWTO(R_PIC30_BRANCH_ABSOLUTE,	/* type */
	 1,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 TRUE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
	 "BRANCH ABSOLUTE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffff),		/* src_mask */
	 0x00ffff,			/* dst_mask */
	 TRUE),				/* pcrel_offset? */
  /* pc-relative do */
  HOWTO(R_PIC30_PCREL_DO,		/* type */
	 1,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 TRUE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
	 "PC RELATIVE DO",		/* name */
         PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 TRUE),				/* pcrel_offset? */
  /* absolute do */
  HOWTO(R_PIC30_DO_ABSOLUTE,		/* type */
	 1,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 TRUE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
	 "DO ABSOLUTE",			/* name */
         PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffff),		/* src_mask */
	 0x00ffff,			/* dst_mask */
	 TRUE),				/* pcrel_offset? */
  /* program address */
  HOWTO(R_PIC30_PGM_ADDR_LSB,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PGM_ADDR,	/* special_function */
	 "PROGRAM ADDRESS LSB",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program address */
  HOWTO(R_PIC30_PGM_ADDR_MSB,		/* type */
	 16,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PGM_ADDR,	/* special_function */
	 "PROGRAM ADDRESS MSB",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0xffff),		/* src_mask */
	 0xffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* unsigned 4-bit */
   HOWTO(R_PIC30_UNSIGNED_4,		/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 4,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 4",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000f),		/* src_mask */
	 0x00000f,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* unsigned 5-bit */
   HOWTO(R_PIC30_UNSIGNED_5,		/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 5,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 5",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x001f),		/* src_mask */
	 0x00001f,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* 3-bit immediate */
   HOWTO(R_PIC30_BIT_SELECT_3,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 3,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 13,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "BIT SELECT 3",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00e000),		/* src_mask */
	 0x00e000,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* 4-bit immediate */
   HOWTO(R_PIC30_BIT_SELECT_4_BYTE,	/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 3,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 12,				/* bitpos */
         complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "BIT SELECT 4 (byte)",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x007000),		/* src_mask */
	 0x007000,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* 4-bit immediate */
   HOWTO(R_PIC30_BIT_SELECT_4,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 4,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 12,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "BIT SELECT 4",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00f000),		/* src_mask */
	 0x00f000,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* dsp 6-bit immediate */
   HOWTO(R_PIC30_DSP_6,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 5,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "DSP SIGNED 6",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00003f),		/* src_mask */
	 0x00003f,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* dsp 4-bit immediate */
   HOWTO(R_PIC30_DSP_PRESHIFT,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 4,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 7,				/* bitpos */
	 complain_overflow_signed,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "DSP PRESHIFT",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000780),		/* src_mask */
	 0x000780,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* signed 10-bit immediate */
   HOWTO(R_PIC30_SIGNED_10_BYTE,	/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "SIGNED 10 (byte)",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000ff0),		/* src_mask */
	 0x000ff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* unsigned 10-bit immediate */
   HOWTO(R_PIC30_UNSIGNED_10,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 10",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x003ff0),		/* src_mask */
	 0x003ff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* 14-bit immediate */
   HOWTO(R_PIC30_UNSIGNED_14,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 14,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 14",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x003fff),		/* src_mask */
	 0x003fff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* LNK immediate */
   HOWTO(R_PIC30_FRAME_SIZE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 14,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_EVEN_ADDRESS,	/* special_function */
	 "FRAME SIZE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x003fff),		/* src_mask */
	 0x003fff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
   /* 1-bit immediate */
   HOWTO(R_PIC30_PWRSAV_MODE,		/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 1,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "POWER SAVE MODE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x000001),		/* src_mask */
	 0x000001,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* dma offset */
  HOWTO(R_PIC30_DMAOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "DMAOFFSET",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* program dma offset */
  HOWTO(R_PIC30_P_DMAOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "DMAOFFSET",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word dma offset */
  HOWTO(R_PIC30_WORD_DMAOFFSET,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "WORD - DMAOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* psvptr */
  HOWTO(R_PIC30_PSVPTR, 		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "PSVPTR",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffffff),		/* src_mask */
	 0x00ffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* p-directive psvptr */
  HOWTO(R_PIC30_P_PSVPTR,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-PSVPTR",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00ffffff),		/* src_mask */
	 0x00ffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* long psvptr */
  HOWTO(R_PIC30_L_PSVPTR,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
        "L-PSVPTR",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0xffffffff),		/* src_mask */
	 0xffffffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  /* word psvptr */
  HOWTO(R_PIC30_WORD_PSVPTR,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - PSVPTR",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_CALL_ACCESS,
         0,				/* rightshift */
         2,				/* size (0=byte, 1=short, 2=long) */
         32,				/* bitsize */
         FALSE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
         "CALL/GOTO ACCESS",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0xffffffff),		/* src_mask */
         0xffffffff,			/* dst_mask */
         FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_PCREL_ACCESS,
         1,				/* rightshift */
         1,				/* size (0=byte, 1=short, 2=long) */
         16,				/* bitsize */
         TRUE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_signed,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
         "PC RELATIVE BRANCH - ACCESS", /* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0xffff),		/* src_mask */
         0x00ffff,			/* dst_mask */
         TRUE),                         /* pcrel_offset? */
  HOWTO(R_PIC30_ACCESS,
         0,				/* rightshift */
         1,				/* size (0=byte, 1=short, 2=long) */
         16,				/* bitsize */
         FALSE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
         "ACCESS",                      /* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0x0000ffff),		/* src_mask */
         0x0000ffff,			/* dst_mask */
         FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_P_ACCESS,
         0,				/* rightshift */
         2,				/* size (0=byte, 1=short, 2=long) */
         24,				/* bitsize */
         FALSE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
         "P-ACCESS",                    /* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0x00ffffff),		/* src_mask */
         0x00ffffff,			/* dst_mask */
         FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_L_ACCESS,
         0,				/* rightshift */
         2,				/* size (0=byte, 1=short, 2=long) */
         32,				/* bitsize */
         FALSE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
         "L-ACCESS",	        	/* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0xffffffff),		/* src_mask */
         0xffffffff,			/* dst_mask */
         FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_WORD_ACCESS,
         0,				/* rightshift */
         2,				/* size (0=byte, 1=short, 2=long) */
         16,				/* bitsize */
         FALSE,				/* pc_relative? */
         4,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
         "WORD - ACCESS",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0xffff0),		/* src_mask */
         0x0ffff0,			/* dst_mask */
         FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_EDSPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "EDSPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_P_EDSPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-EDSPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_WORD_EDSPAGE,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - EDSPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_EDSOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "EDSOFFSET",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_P_EDSOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_P_OPERATORS,	/* special_function */
	 "P-EDSOFFSET",			/* name */
         TRUE,				/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_WORD_EDSOFFSET,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "WORD - EDSOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
 /* unsigned 8-bit */
   HOWTO(R_PIC30_UNSIGNED_8,		/* type */
	 0,				/* rightshift */
	 0,				/* size (0=byte, 1=short, 2=long) */
	 8,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 8",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x00039f),		/* src_mask */
	 0x00039f,			/* dst_mask */
	 FALSE),
 /* 15-bit immediate */
   HOWTO(R_PIC30_UNSIGNED_15,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 15,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "UNSIGNED 15",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x007fff),		/* src_mask */
	 0x007fff,			/* dst_mask */
	 FALSE),
 /* unsigned 10-bit immediate */
   HOWTO(R_PIC30_NO_SHIFT_UNSIGNED_10,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_unsigned,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_GENERIC,	/* special_function */
	 "NO SHIFT UNSIGNED 10",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),
 /* pc-relative branch */
  HOWTO(R_PIC30_PCREL_BRANCH_SLIT6,		/* type */
	 1,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 6,				/* bitsize */
	 TRUE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_PCREL,	/* special_function */
	 "PC RELATIVE BRANCH SLIT6",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003f0),		/* src_mask */
	 0x0003f0,			/* dst_mask */
	 TRUE),

 HOWTO(R_PIC30_DMAPAGE,		        /* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "DMAPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_P_DMAPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "P-DMAPAGE",			/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0000ffff),		/* src_mask */
	 0x0000ffff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_WORD_DMAPAGE,		/* type */
	 0,				/* rightshift */
	 2,				/* size (0=byte, 1=short, 2=long) */
	 16,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 4,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "WORD - DMAPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0ffff0),		/* src_mask */
	 0x0ffff0,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_TBLOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - TBLOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),	
  HOWTO(R_PIC30_UNSIGNED_10_PSVOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - PSVOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),
 HOWTO(R_PIC30_UNSIGNED_10_TBLPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - TBLPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
 HOWTO(R_PIC30_UNSIGNED_10_PSVPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - PSVPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_HANDLE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - HANDLE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_DMAPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "UNSIGNED 10 - DMAPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_DMAOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE,	/* special_function */
	 "UNSIGNED 10 - DMAOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_EDSPAGE,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_bitfield,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - EDSPAGE",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
 HOWTO(R_PIC30_UNSIGNED_10_EDSOFFSET,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,    /*complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - EDSOFFSET",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_PSVPTR,		/* type */
	 0,				/* rightshift */
	 1,				/* size (0=byte, 1=short, 2=long) */
	 10,				/* bitsize */
	 FALSE,				/* pc_relative? */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
	 "UNSIGNED 10 - PSVPTR",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
	 SRC_MASK(0x0003ff),		/* src_mask */
	 0x0003ff,			/* dst_mask */
	 FALSE),			/* pcrel_offset? */
  HOWTO(R_PIC30_UNSIGNED_10_ACCESS,
         0,				/* rightshift */
         1,				/* size (0=byte, 1=short, 2=long) */
         106,				/* bitsize */
         FALSE,				/* pc_relative? */
         0,				/* bitpos */
         complain_overflow_dont,	/* complain_on_overflow */
         RELOC_SPECIAL_FN_OPERATORS,	/* special_function */
         "UNSIGNED 10 - ACCESS",		/* name */
	 PIC30_PIP,			/* partial_inplace? */
         SRC_MASK(0x0003ff),		/* src_mask */
         0x0003ff,			/* dst_mask */
         FALSE),			/* pcrel_offset? */	
			
};

/* A mapping from BFD reloc types to PIC30 ELF reloc types.  */

struct elf_reloc_map {
  bfd_reloc_code_real_type bfd_val;
  enum elf_pic30_reloc_type elf_val;
};

static const struct elf_reloc_map pic30_reloc_map[] =
{
  { BFD_RELOC_NONE,			    R_PIC30_NONE },
  { BFD_RELOC_8,			    R_PIC30_8 },
  { BFD_RELOC_16,			    R_PIC30_16 },
  { BFD_RELOC_32,			    R_PIC30_32 },
  { BFD_RELOC_PIC30_FILE_REG,		    R_PIC30_FILE_REG },
  { BFD_RELOC_PIC30_FILE_REG_BYTE,	    R_PIC30_FILE_REG_BYTE },
  { BFD_RELOC_PIC30_FILE_REG_WORD, 	    R_PIC30_FILE_REG_WORD },
  { BFD_RELOC_PIC30_FILE_REG_WORD_WITH_DST, R_PIC30_FILE_REG_WORD_WITH_DST },
  { BFD_RELOC_PIC30_WORD,		    R_PIC30_WORD },
  { BFD_RELOC_PIC30_PBYTE,		    R_PIC30_PBYTE },
  { BFD_RELOC_PIC30_PWORD,		    R_PIC30_PWORD },
  { BFD_RELOC_PIC30_HANDLE,		    R_PIC30_HANDLE },
  { BFD_RELOC_PIC30_PADDR,		    R_PIC30_PADDR },
  { BFD_RELOC_PIC30_P_PADDR,		    R_PIC30_P_PADDR },
  { BFD_RELOC_PIC30_PSVOFFSET,		    R_PIC30_PSVOFFSET },
  { BFD_RELOC_PIC30_TBLOFFSET,		    R_PIC30_TBLOFFSET },
  { BFD_RELOC_PIC30_WORD_HANDLE,	    R_PIC30_WORD_HANDLE },
  { BFD_RELOC_PIC30_WORD_PSVOFFSET,	    R_PIC30_WORD_PSVOFFSET },
  { BFD_RELOC_PIC30_PSVPAGE,	    	    R_PIC30_PSVPAGE },
  { BFD_RELOC_PIC30_P_PSVPAGE,	            R_PIC30_P_PSVPAGE },
  { BFD_RELOC_PIC30_WORD_PSVPAGE,	    R_PIC30_WORD_PSVPAGE },
  { BFD_RELOC_PIC30_WORD_TBLOFFSET,	    R_PIC30_WORD_TBLOFFSET },
  { BFD_RELOC_PIC30_TBLPAGE,	            R_PIC30_TBLPAGE },
  { BFD_RELOC_PIC30_P_TBLPAGE,	            R_PIC30_P_TBLPAGE },
  { BFD_RELOC_PIC30_WORD_TBLPAGE,	    R_PIC30_WORD_TBLPAGE },
  { BFD_RELOC_PIC30_P_HANDLE,		    R_PIC30_P_HANDLE },
  { BFD_RELOC_PIC30_P_PSVOFFSET,	    R_PIC30_P_PSVOFFSET },
  { BFD_RELOC_PIC30_P_TBLOFFSET,	    R_PIC30_P_TBLOFFSET },
  { BFD_RELOC_PIC30_PCREL_BRANCH,	    R_PIC30_PCREL_BRANCH },
  { BFD_RELOC_PIC30_BRANCH_ABSOLUTE,	    R_PIC30_BRANCH_ABSOLUTE },
  { BFD_RELOC_PIC30_PCREL_DO,	    	    R_PIC30_PCREL_DO },
  { BFD_RELOC_PIC30_DO_ABSOLUTE,    	    R_PIC30_DO_ABSOLUTE },
  { BFD_RELOC_PIC30_PGM_ADDR_LSB,	    R_PIC30_PGM_ADDR_LSB },
  { BFD_RELOC_PIC30_PGM_ADDR_MSB,	    R_PIC30_PGM_ADDR_MSB },
  { BFD_RELOC_PIC30_UNSIGNED_4,	            R_PIC30_UNSIGNED_4 },
  { BFD_RELOC_PIC30_UNSIGNED_5,	            R_PIC30_UNSIGNED_5 },
  { BFD_RELOC_PIC30_BIT_SELECT_3,	    R_PIC30_BIT_SELECT_3 },
  { BFD_RELOC_PIC30_BIT_SELECT_4_BYTE,	    R_PIC30_BIT_SELECT_4_BYTE },
  { BFD_RELOC_PIC30_BIT_SELECT_4,	    R_PIC30_BIT_SELECT_4 },
  { BFD_RELOC_PIC30_DSP_6,	    	    R_PIC30_DSP_6 },
  { BFD_RELOC_PIC30_DSP_PRESHIFT,    	    R_PIC30_DSP_PRESHIFT },
  { BFD_RELOC_PIC30_SIGNED_10_BYTE,    	    R_PIC30_SIGNED_10_BYTE },
  { BFD_RELOC_PIC30_UNSIGNED_10,    	    R_PIC30_UNSIGNED_10 },
  { BFD_RELOC_PIC30_UNSIGNED_14,    	    R_PIC30_UNSIGNED_14 },
  { BFD_RELOC_PIC30_FRAME_SIZE,    	    R_PIC30_FRAME_SIZE },
  { BFD_RELOC_PIC30_PWRSAV_MODE,    	    R_PIC30_PWRSAV_MODE },
  { BFD_RELOC_PIC30_DMAOFFSET,		    R_PIC30_DMAOFFSET },
  { BFD_RELOC_PIC30_P_DMAOFFSET,	    R_PIC30_P_DMAOFFSET },
  { BFD_RELOC_PIC30_WORD_DMAOFFSET,	    R_PIC30_WORD_DMAOFFSET },
  { BFD_RELOC_PIC30_PSVPTR,		    R_PIC30_PSVPTR },
  { BFD_RELOC_PIC30_P_PSVPTR,   	    R_PIC30_P_PSVPTR },
  { BFD_RELOC_PIC30_L_PSVPTR,   	    R_PIC30_L_PSVPTR },
  { BFD_RELOC_PIC30_WORD_PSVPTR,	    R_PIC30_WORD_PSVPTR },
  { BFD_RELOC_PIC30_CALL_ACCESS,	    R_PIC30_CALL_ACCESS },
  { BFD_RELOC_PIC30_PCREL_ACCESS,	    R_PIC30_PCREL_ACCESS },
  { BFD_RELOC_PIC30_ACCESS,                 R_PIC30_ACCESS },
  { BFD_RELOC_PIC30_P_ACCESS,               R_PIC30_P_ACCESS },
  { BFD_RELOC_PIC30_L_ACCESS,               R_PIC30_L_ACCESS },
  { BFD_RELOC_PIC30_WORD_ACCESS,            R_PIC30_WORD_ACCESS },
  { BFD_RELOC_PIC30_EDSPAGE,		    R_PIC30_EDSPAGE },
  { BFD_RELOC_PIC30_P_EDSPAGE,		    R_PIC30_P_EDSPAGE },
  { BFD_RELOC_PIC30_WORD_EDSPAGE,	    R_PIC30_WORD_EDSPAGE },
  { BFD_RELOC_PIC30_EDSOFFSET,		    R_PIC30_EDSOFFSET },
  { BFD_RELOC_PIC30_P_EDSOFFSET,      	    R_PIC30_P_EDSOFFSET },
  { BFD_RELOC_PIC30_WORD_EDSOFFSET,	    R_PIC30_WORD_EDSOFFSET },
  { BFD_RELOC_PIC30_UNSIGNED_8,	            R_PIC30_UNSIGNED_8 },
  { BFD_RELOC_PIC30_UNSIGNED_15,    	    R_PIC30_UNSIGNED_15 },
  { BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10,   R_PIC30_NO_SHIFT_UNSIGNED_10 },
  { BFD_RELOC_PIC30_PCREL_BRANCH_SLIT6,	    R_PIC30_PCREL_BRANCH_SLIT6 },
  { BFD_RELOC_PIC30_DMAPAGE,		    R_PIC30_DMAPAGE },
  { BFD_RELOC_PIC30_P_DMAPAGE,	            R_PIC30_P_DMAPAGE },
  { BFD_RELOC_PIC30_WORD_DMAPAGE,	    R_PIC30_WORD_DMAPAGE },
  { BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET,  R_PIC30_UNSIGNED_10_TBLOFFSET },
  { BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET,  R_PIC30_UNSIGNED_10_PSVOFFSET },
  { BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE,    R_PIC30_UNSIGNED_10_TBLPAGE },
  { BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE,    R_PIC30_UNSIGNED_10_PSVPAGE },
  { BFD_RELOC_PIC30_UNSIGNED_10_HANDLE,	    R_PIC30_UNSIGNED_10_HANDLE },
  { BFD_RELOC_PIC30_UNSIGNED_10_DMAPAGE,    R_PIC30_UNSIGNED_10_DMAPAGE },
  { BFD_RELOC_PIC30_UNSIGNED_10_DMAOFFSET,  R_PIC30_UNSIGNED_10_DMAOFFSET },
  { BFD_RELOC_PIC30_UNSIGNED_10_EDSPAGE,    R_PIC30_UNSIGNED_10_EDSPAGE },
  { BFD_RELOC_PIC30_UNSIGNED_10_EDSOFFSET,  R_PIC30_UNSIGNED_10_EDSOFFSET },
  { BFD_RELOC_PIC30_UNSIGNED_10_PSVPTR,	    R_PIC30_UNSIGNED_10_PSVPTR },
  { BFD_RELOC_PIC30_UNSIGNED_10_ACCESS,     R_PIC30_UNSIGNED_10_ACCESS },
};

/* Given a BFD reloc type, return a howto structure.  */

static reloc_howto_type *
pic30_elf32_bfd_reloc_type_lookup (abfd, code)
     bfd *abfd;
     bfd_reloc_code_real_type code;
{
  unsigned int i;
  reloc_howto_type *howto_table = elf_pic30_howto_table_rel;

  for (i = 0;
       i < sizeof (pic30_reloc_map) / sizeof (struct elf_reloc_map);
       i++)
  {
      if (pic30_reloc_map[i].bfd_val == code)
      {
	return(&howto_table[(int) pic30_reloc_map[i].elf_val]);
      }
  }

  switch (code)
  {
  case BFD_RELOC_CTOR:
      /* The type of reloc used in a ctor, which will be as wide as the
	 address - so either a 64, 32, or 16 bitter.  */
	switch (bfd_get_arch_info (abfd)->bits_per_address)
	{
	case 64:
	  BFD_FAIL();
	  break;
	case 32:
	  return(&howto_table[(int) R_PIC30_32]);
	case 16:
	  BFD_FAIL();
	  break;
	default:
	  BFD_FAIL();
	  break;
	}
  default:
      bfd_set_error(bfd_error_bad_value);
      break;
  }
  return((reloc_howto_type *)0);
}

/**********************************************************************
**
** Undefined Symbol Hash Table Routines
**
** We use a hash table to keep track of undefined
** symbols and their object signatures.
**
** Derived from the basic table provided in /bfd/hash.c
*/

/* Create a new hash table entry */
struct bfd_hash_entry *
pic30_undefsym_newfunc (entry, table, string)
     struct bfd_hash_entry *entry;
     struct bfd_hash_table *table;
     const char *string;
{
  struct pic30_undefsym_entry *ret = (struct pic30_undefsym_entry *) entry;

  /* Allocate the structure if it has not already been allocated by a
     subclass.  */
  if (ret == (struct pic30_undefsym_entry *) NULL)
    ret = ((struct pic30_undefsym_entry *)
	   bfd_hash_allocate (table, sizeof (struct pic30_undefsym_entry)));
  if (ret == (struct pic30_undefsym_entry *) NULL)
    return NULL;

  /* Call the allocation method of the superclass.  */
  ret = ((struct pic30_undefsym_entry *)
	 bfd_hash_newfunc ((struct bfd_hash_entry *) ret, table, string));

  if (ret) {     /* Initialize the local fields. */
    ret->most_recent_reference = 0;
    ret->external_options_mask = 0;
    ret->options_set = 0;
    }

  return (struct bfd_hash_entry *) ret;
}

/* Traverse the hash table with a provided function */
void
pic30_undefsym_traverse (table, func, info)
     struct pic30_undefsym_table *table;
     bfd_boolean (*func) PARAMS ((struct bfd_hash_entry *, PTR));
     PTR info;
{
  bfd_hash_traverse (&table->table,
		     ((bfd_boolean (*) PARAMS ((struct bfd_hash_entry *, PTR)))
		      func), info);
}

/* Create a new hash table */
struct pic30_undefsym_table *
pic30_undefsym_init ()
{
  struct pic30_undefsym_table *table;

  table = ((struct pic30_undefsym_table *)
	   bfd_alloc (pic30_output_bfd, sizeof (struct pic30_undefsym_table)));
  if (table == NULL)
    return NULL;

  if (! bfd_hash_table_init_n (&table->table, pic30_undefsym_newfunc,
                               PIC30_UNDEFSYM_INIT)) {
      free (table);
      return NULL;
    }

  table->num = 0;
  return table;
}
/*********************************************************************/

/*
** Given a BFD section, try to locate the corresponding ELF section index.
*/
bfd_boolean
pic30_elf_backend_section_from_bfd_section (abfd, sec, retval)
     bfd *abfd ATTRIBUTE_UNUSED;
     asection *sec;
     int *retval;
{
  /*
  ** This function maps section names defined in the default
  ** linker scripts to a special section header number.
  ** It exists in case the linked program doesn't use the
  ** default names. Auto-generated names for zero-length
  ** sections are also handled.
  */
  const char *snames[] =
  {
          ".bss",  ".data",
	  ".nbss", ".ndata",
	  ".xbss", ".xdata"
  };
  size_t i;
  const char *sname = bfd_get_section_name(abfd, sec);

  if (strstr(sname, "%z") != 0)  /* auto-generated, zero-length */
    {
      	*retval = SHN_LOPROC;
      	return(TRUE);
    }
  for (i = 0; i < sizeof(snames)/sizeof(snames[0]); ++i)
  {
    if (strcmp(sname, snames[i]) == 0)
    {
      	*retval = SHN_LOPROC;

      	return(TRUE);
    }
  }
  if (sec->flags & SEC_DEBUGGING) {
      	*retval = SHN_LOPROC;
      	return(TRUE);
  }

  if (pic30_debug)
  {
    if ((strcmp(sname, "*UND*") != 0) && (strcmp(sname, "*ABS*") != 0))
    {
      printf("%s: %s?\n", __FUNCTION__, sname);
    }
  }
  return(FALSE);
}


/* Given a PIC30 Elf_Internal_Rel, fill in an arelent structure.  */

static void
pic30_info_to_howto_rel (abfd, cache_ptr, dst)
     bfd *abfd ATTRIBUTE_UNUSED;
     arelent *cache_ptr;
     Elf_Internal_Rela *dst;
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  BFD_ASSERT (r_type < (unsigned int) R_PIC30_maxext);
  cache_ptr->howto = &elf_pic30_howto_table_rel[r_type];
}

/* Given a PIC30 Elf_Internal_Rela, fill in an arelent structure.  */

#if (PIC30_USE_RELA)
static void
pic30_info_to_howto_rela (abfd, cache_ptr, dst)
     bfd *abfd;
     arelent *cache_ptr;
     Elf_Internal_Rela *dst;
{
  pic30_info_to_howto_rel (abfd, cache_ptr, dst);

  /*
  ** If we ever need to do any extra processing with dst->r_addend
  ** (the field omitted in an Elf_Internal_Rel) we can do it here.
  */
}
#endif

/*
** This function is called by elfcode.h after an object has
** been determined to be compatible with the link target.
**
** It copies the processorID from the e_flags field
** into abfd->arch_info. Therefore every input BFD identifies which
** processor it was created for.
**
** Note that this ID may be different from the one in the linker's
** output BFD, which is set by command line option, or else contains
** a default value.
*/
static bfd_boolean
pic30_backend_object_p (abfd)
     bfd *abfd;
{
  abfd->arch_info = bfd_lookup_arch (bfd_arch_pic30,
                                     elf_elfheader(abfd)->e_flags & EF_PIC30_PROC);
  return TRUE;
}

/*
** This function is called by elf.c before an object is written
** to disk. It writes the processorID into the lower 16 bits of
** the e_flags field in the elf header, as defined by EF_PIC30_PROC.
*/
static void
pic30_final_write_processing (abfd, linker)
     bfd *abfd;
     bfd_boolean linker;
{
  if ((linker == 0) && global_PROCESSOR)
    {
      elf_elfheader(abfd)->e_flags &= ~EF_PIC30_PROC;
      elf_elfheader(abfd)->e_flags |= (unsigned int) global_PROCESSOR->mach;
    }
}

/*
**  This routine is cloned from _bfd_elf_set_section_contents() in elf.c
**  and modifed to support the following pic30-specific need:
**
**     PSV data sections are expanded as they are written
**     so that the program memory image includes space for
**     the upper byte and phantom byte. Note that the output
**     section size is already correct -- phantom bytes in data
**     memory have reserved space for the upper and phantom
**     byte in program memory.
**
*/
bfd_boolean
pic30_bfd_elf32_set_section_contents (abfd, section, location, offset, count)
     bfd *abfd;
     sec_ptr section;
     PTR location;
     file_ptr offset;
     bfd_size_type count;
{
  PTR pic30_location = location;
  Elf_Internal_Shdr *hdr;
  bfd_signed_vma pos;

  /* Reset the bfd file pointer, because
     there seems to be a bug with fseek()
     in Winblows that makes seeking to
     a position earlier in the file unreliable. */
     (void) bfd_seek(abfd, 0, SEEK_SET);
     
  /*
  ** If this is a linker pass (link_order_head != 0) on
  ** a PSV or EEDATA data section
  ** then expand the section to include padding for
  ** the extra width of program memory.
  **
  ** Since data sections are always aligned we know
  ** the length is even.
  **
  ** Because of PC addressing, the size of the output
  ** section is already correct. Basically we are
  ** swapping 2 phantom bytes in data memory for 2
  ** padding bytes in program memory.
  */
  if ((section->link_order_head != 0) &&
      (PIC30_IS_PSV_ATTR(section) | PIC30_IS_EEDATA_ATTR(section)))
  {
      /*
      ** expand data image for program memory storage
      */
      bfd_size_type j = 0;
      unsigned char *p1 = location;
      unsigned char *p2 = bfd_malloc(count);
      if (!p2)
	      return FALSE;
      while (j < count)
      {
          /* copy two bytes of data, then two byte of padding */
          /* (convert data phantoms into program mem padding) */
          p2[j++] = *p1++; p1++;
          p2[j++] = *p1++; p1++;
          p2[j++] = pic30_fill_upper;
          p2[j++] = 0;
      }
      pic30_location = p2;
      if (pic30_debug)
        printf("expanding %#x bytes into %s at %#lx\n",
               (unsigned int) count, section->name, offset);
  }

  if (! abfd->output_has_begun
      && ! (_bfd_elf_compute_section_file_positions
	    (abfd, (struct bfd_link_info *) NULL)))
    return FALSE;

  hdr = &elf_section_data (section)->this_hdr;
  pos = hdr->sh_offset + offset;
  if (bfd_seek (abfd, pos, SEEK_SET) != 0
      || bfd_bwrite (pic30_location, count, abfd) != count)
    return FALSE;

  return TRUE;
}
/*
** New section target hook
*/
bfd_boolean
pic30_bfd_elf32_new_section_hook (abfd, sec)
     bfd *abfd;
     asection *sec;
{
  const char *sname = bfd_get_section_name(abfd, sec);
  if (strncmp(sname, ".debug", 6) != 0)
  {
    /*
    ** Set the default section alignment for non-debug sections to 2^1
    */
    sec->alignment_power = 1;
  }
  return _bfd_elf_new_section_hook (abfd, sec);
}
int
pic30_adjustable_against_section (type)
   unsigned short type;
{
   int rc = 1;

   switch (type)
   {
   case R_PIC30_PADDR:
   case R_PIC30_P_TBLOFFSET:
   case R_PIC30_P_PSVOFFSET:
   case R_PIC30_P_PADDR:
   case R_PIC30_8:
   case R_PIC30_16:
   case R_PIC30_32:
   case R_PIC30_PBYTE:
   case R_PIC30_PWORD:
   case R_PIC30_BRANCH_ABSOLUTE:
   case R_PIC30_DO_ABSOLUTE:
   case R_PIC30_PCREL_BRANCH_SLIT6:
   case R_PIC30_PCREL_BRANCH:
   case R_PIC30_PCREL_DO:
   case R_PIC30_PGM_ADDR_MSB:
   case R_PIC30_PGM_ADDR_LSB:
   case R_PIC30_TBLOFFSET:
   case R_PIC30_PSVOFFSET:
   case R_PIC30_HANDLE:
   case R_PIC30_P_HANDLE:
   case R_PIC30_WORD_TBLOFFSET:
   case R_PIC30_WORD_PSVOFFSET:
   case R_PIC30_PSVPAGE:
   case R_PIC30_P_PSVPAGE:
   case R_PIC30_WORD_PSVPAGE:
   case R_PIC30_TBLPAGE:
   case R_PIC30_P_TBLPAGE:
   case R_PIC30_WORD_TBLPAGE:
   case R_PIC30_WORD_HANDLE:
   case R_PIC30_WORD:
   case R_PIC30_FILE_REG_WORD:
   case R_PIC30_FILE_REG_WORD_WITH_DST:
   case R_PIC30_DMAOFFSET:
   case R_PIC30_P_DMAOFFSET:
   case R_PIC30_WORD_DMAOFFSET:
   case R_PIC30_PSVPTR:
   case R_PIC30_P_PSVPTR:
   case R_PIC30_L_PSVPTR:
   case R_PIC30_WORD_PSVPTR:
   case R_PIC30_CALL_ACCESS:
   case R_PIC30_PCREL_ACCESS:
   case R_PIC30_ACCESS:
   case R_PIC30_P_ACCESS:
   case R_PIC30_L_ACCESS:
   case R_PIC30_WORD_ACCESS:
   case R_PIC30_UNSIGNED_10_TBLOFFSET:
   case R_PIC30_UNSIGNED_10_PSVOFFSET:
   case R_PIC30_UNSIGNED_10_PSVPAGE:
   case R_PIC30_UNSIGNED_10_TBLPAGE:
   case R_PIC30_UNSIGNED_10_HANDLE:
   case R_PIC30_UNSIGNED_10_DMAPAGE:
   case R_PIC30_UNSIGNED_10_DMAOFFSET:
   case R_PIC30_UNSIGNED_10_EDSPAGE:
   case R_PIC30_UNSIGNED_10_EDSOFFSET:
   case R_PIC30_UNSIGNED_10_PSVPTR:
   case R_PIC30_UNSIGNED_10_ACCESS:
         rc = 0;
         break;

   default:
         break;
   }
   return rc;
}
/*****************************************************************************/

/* Include some utilities for processing section attributes */
#include "pic30-attributes.c"

/*
** This routine is called via bfd_map_over_sections().
*/
static void
bfd_pic30_print_section_header(void)
{
  printf("  %8s %5s %6s %6s %6s\n",
         " section", "flags", "   VMA", "   LMA", "length");
  printf("  %8s %4s %6s %6s %6s\n",
         " -------", "-----", "------", "------", "------");
}

/*
** This routine is called via bfd_map_over_sections().
*/
static void
bfd_pic30_report_sections(abfd, sect, fp) 
     bfd *abfd;
     asection *sect;
     PTR fp ATTRIBUTE_UNUSED ;
{
  unsigned int opb = bfd_octets_per_byte (abfd);

  printf("  %8s %5x %6x %6x %6x\n", sect->name, sect->flags,
         (unsigned int) sect->vma,
         (unsigned int) sect->lma,
         (unsigned int) sect->_raw_size / opb);
}

/*
** bfd_pic30_clean_section_names
**
** Remove the suffix appended to output section
** names by the best-fit allocator.
**
** This routine is called via bfd_map_over_sections().
*/
static void
bfd_pic30_clean_section_names(abfd, sect, fp) 
     bfd *abfd ATTRIBUTE_UNUSED ;
     asection *sect;
     PTR fp ATTRIBUTE_UNUSED ;
{
  char *c;
  Elf_Internal_Shdr *this_hdr ATTRIBUTE_UNUSED ;
  int shidx ATTRIBUTE_UNUSED ;

  c = strchr(sect->name, '%');
  if (c) {
    if (pic30_debug)  printf("  %s",sect->name);
    *c = (char) NULL;
    if (pic30_debug)  printf(" --> %s\n",sect->name);
#if 0
    /* FIXME: unassigned sections with relocations
       appear to store their names elsewhere.
       Currently they are not cleaned. */
    this_hdr = &elf_section_data (sect)->this_hdr;
    shidx = &elf_section_data (sect)->this_idx;
    /* DEBUG */
    printf("bfd_pic30_clean_section_names: %s\n",
           elf_string_from_elf_strtab(abfd, this_hdr->sh_name));
#endif
  }
  return;
}

/*
** Write a data record header
*/
static void
bfd_pic30_write_data_header(d, addr, len, format)
     unsigned char **d;
     bfd_vma addr;
     bfd_vma len;
     int format;
{
  int eds_page;

  /* extract eds page and OR into the format code */
  eds_page = addr >> 15;
  if (pic30_is_eds_machine(global_PROCESSOR)) {
    if (eds_page == 0)
      eds_page = 1;
    else
      addr = PIC30_PSVOFFSET(addr);
  }
  format |= eds_page << 7;
  addr &= 0xFFFF;

  /* write destination address */
  *(*d)++ = (unsigned char) (addr & 0xFF);
  *(*d)++ = (unsigned char) (addr >> 8);
  *(*d)++ = pic30_fill_upper; *(*d)++ = 0; /* skip upper and phantom bytes */

  /* write destination length */
  *(*d)++ = (unsigned char) (len & 0xFF);
  *(*d)++ = (unsigned char) (len >> 8);
  *(*d)++ = pic30_fill_upper; *(*d)++ = 0; /* skip upper and phantom bytes */

  /* write format code */
  *(*d)++ = (unsigned char) (format & 0xFF);
  *(*d)++ = (unsigned char) (format >> 8);
  *(*d)++ = pic30_fill_upper; *(*d)++ = 0; /* skip upper and phantom bytes */
}

/*
** This routine is called by pic30_final_link().
**
** If the section is BSS or DATA, write the appropriate
** record into the data init template.
**
** ToDo: If the data section exceeds 64K, break into
** multiple records.
*/
static void
bfd_pic30_process_data_section(sect, fp) 
     asection *sect;
     PTR fp;
{
  unsigned char *buf,*p;
  unsigned char **d = (unsigned char **) fp;
  bfd_vma runtime_size = sect->_raw_size / 2; /* skip phantoms at runtime */
  bfd_vma runtime_addr = sect->output_offset + sect->output_section->vma;
  Elf_Internal_Shdr *this_hdr;

  enum { CLEAR, COPY2, COPY3 }; /* note matching definition in crt0.c */
      
  /* skip persistent or noload data sections */
  if (PIC30_IS_PERSIST_ATTR(sect) | PIC30_IS_NOLOAD_ATTR(sect))
    {
      if (pic30_debug)
        printf("  %s (skipped), size = %x\n",
               sect->name, (unsigned int) runtime_size);
      return;
    }

  /* process BSS-type sections */
  if (PIC30_IS_BSS_ATTR(sect) && (sect->_raw_size > 0))
    {
      if (pic30_debug)
        printf("  %s (bss), size = %x bytes, template addr = %lx\n",
               sect->name, (unsigned int) runtime_size,
               (long unsigned int) *d);

      /* write header only */
      bfd_pic30_write_data_header(d, runtime_addr, runtime_size, CLEAR);
    }

  /* process DATA-type sections */
  if (PIC30_IS_DATA_ATTR(sect) && (sect->_raw_size > 0))
    {
      if (pic30_debug)
        printf("  %s (data), size = %x bytes, template addr = %lx\n",
               sect->name, (unsigned int) runtime_size,
               (long unsigned int) *d);

      /*
      ** load a copy of the section contents
      **
      ** Note: We are extracting input section data
      ** from an output section. Use output_offset*2
      ** to account for phantom bytes in data memory.
      */
      buf = (unsigned char *) malloc (sect->_raw_size);
      if (!buf)
        {
          fprintf( stderr, "Link Error: not enough memory for section contents\n");
          abort();
        }

      /* Reset the bfd file pointer, because
         there seems to be a bug with fseek()
         in Winblows that makes seeking to
         a position earlier in the file unreliable. */
      bfd_seek(sect->output_section->owner, 0, SEEK_SET);
     
      if (!bfd_get_section_contents(sect->output_section->owner,
                                    sect->output_section,
                                    buf, sect->output_offset*2, sect->_raw_size))
        {
          fprintf( stderr, "Link Error: can't load section %s contents\n",
                   sect->name);
          abort();
        }

      if (pic30_pack_data)
        { int count = 0;
          /* write header */
          bfd_pic30_write_data_header(d, runtime_addr, runtime_size, COPY3);
          /* write packed data */
          /*   _raw_size is a multiple of 4, yet this loop wants to read
               six bytes (3 data bytes, and 3 phantom bytes) at a time.
               We may read 2 bytes (1 data and 1 phantom) to fill up the
               output record piece-meal. */
          for (p = buf; p < (buf + sect->_raw_size); )
            {
              *(*d)++ = *p++; p++; /* copy data bytes, avoid phantom */
	      count++;
              if (count == 3) {
                 *(*d)++ = 0;         /* skip phantom byte in template  */
                 count=0;
              }
            }
          /* fill template with zeroes */
          if (count) while (count < 3) { *(*d)++ = 0; count++; }
          if (count == 3) { *(*d)++ = pic30_fill_upper; };
        }
      else /* (!pic30_pack_data) */
        {
          /* write header */
          bfd_pic30_write_data_header(d, runtime_addr, runtime_size, COPY2);
          /* write non-packed data */
          for (p = buf; p < (buf + sect->_raw_size); )
            {
              *(*d)++ = *p++; p++; /* copy data bytes, avoid phantom */
              *(*d)++ = *p++; p++;
              *(*d)++ = pic30_fill_upper;
              *(*d)++ = 0;         /* skip upper, phantom bytes in template*/ 
            }
        }

      free(buf);

      /* make section not LOADable */
      sect->output_section->flags &= ~ SEC_LOAD;
      sect->output_section->flags |= SEC_NEVER_LOAD;
      this_hdr = &elf_section_data (sect->output_section)->this_hdr;
      this_hdr->sh_flags |= SHF_NOLOAD;

    } /* process DATA-type sections */

  return;
} /* static void bfd_pic30_process_data_section (...)*/

/*
** This routine is called via bfd_map_over_sections().
*/
static void
bfd_pic30_check_reloc_flag(abfd, sect, fp) 
     bfd *abfd ATTRIBUTE_UNUSED ;
     asection *sect;
     PTR fp ATTRIBUTE_UNUSED ;
{
  if (sect->reloc_count == 0) {
    /* Explicitly clear the SEC_RELOC flag.  The linker tends to
       set it (this is probably a bug) and if it is set
       assign_section_numbers will create a reloc section.  */
    sect->flags &=~ SEC_RELOC;
  }
}

/*
** Call the generic final link routine, then scan
** all of the initialized data sections and write
** data init records into section .dinit.
*/
static bfd_boolean
pic30_final_link (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  unsigned int opb = bfd_octets_per_byte (abfd);
  bfd_boolean result;
  asection *dinit_sec;
  bfd_size_type dinit_size;
  file_ptr dinit_offset;
  unsigned char *dat;
  unsigned int i;
  struct pic30_section *s;

  /* save a local copy for reloc range checking */
  link_output_bfd = abfd;

  /*
  ** call the generic final link routine
  */
  if (pic30_debug)
  {
      printf("\nBefore generic final link:\n");
      bfd_pic30_print_section_header();
      bfd_map_over_sections(abfd, &bfd_pic30_report_sections, 0);
  }

  bfd_map_over_sections(abfd, &bfd_pic30_check_reloc_flag, 0);

  result = _bfd_generic_final_link (abfd, info);
    
  if (pic30_debug)
  {
      printf("\nAfter generic final link:\n");
      bfd_pic30_print_section_header();
      bfd_map_over_sections(abfd, &bfd_pic30_report_sections, 0);
  }
  if (pic30_data_init)
  {
      dinit_sec = init_template->output_section;
      dinit_size = init_template->_raw_size;
      dinit_offset = init_template->output_offset;

      if (!dinit_sec)
        {
          fprintf( stderr, "Link Error: could not access data template\n");
          abort();
        }

#if 0
      printf("dinit_sec->name = %s\n", dinit_sec->name);
      printf("dinit_size = %lx\n", dinit_size);
      printf("dinit_offset = %lx\n", dinit_offset);
#endif

      /* clear SEC_IN_MEMORY flag if inaccurate */
      if ((dinit_sec->contents == 0) && ((dinit_sec->flags & SEC_IN_MEMORY) != 0))
        dinit_sec->flags &= ~ SEC_IN_MEMORY;

      /* get a copy of the (blank) template contents */
      if (!bfd_get_section_contents (abfd, dinit_sec,
                                     init_data, dinit_offset*opb, dinit_size))
        {
          fprintf( stderr, "Link Error: can't get section %s contents\n",
                   dinit_sec->name);
          abort();
        }

      /* update the default fill value */
      dat = init_data;
      for (i=0; i < dinit_size; i++)
        *dat++ *= 2;

      /* scan sections and write data records */
      if (pic30_debug)
          printf("\nProcessing data sections:\n");
      dat = init_data;
      for (s = data_sections; s != NULL; s = s->next)
         if ((s->sec) && ((s->sec->flags & SEC_EXCLUDE) == 0))
           bfd_pic30_process_data_section(s->sec, &dat);

      /* write zero terminator */
      *dat++ = 0; *dat++ = 0;
      *dat++ = pic30_fill_upper; *dat++ = 0;      

      if (pic30_debug)
          printf("  last template addr written = %lx\n",
                 (long unsigned int) dat - 1);

      /* insert buffer into the data template section */
      if (!bfd_set_section_contents (abfd, dinit_sec,
                                     init_data, dinit_offset*opb, dinit_size))
        {
          fprintf( stderr, "Link Error: can't write section %s contents\n",
                   dinit_sec->name);
          abort();
        }
  }
  /* clean the section names */
  if (pic30_debug)
    printf("\nCleaning section names:\n");
  bfd_map_over_sections(abfd, &bfd_pic30_clean_section_names, 0);

  if (pic30_debug)
  {
      printf("\nAfter pic30 final link:\n");
      bfd_pic30_print_section_header();
      bfd_map_over_sections(abfd, &bfd_pic30_report_sections, 0);
  }
  return(result);
}

/*
** Utilities for pic30_bfd_reloc_range_check()
*/
const char *outer_archive_name(bfd *);
const char *
outer_archive_name(bfd *abfd)
{
  bfd *b = abfd;

  while (b->my_archive != 0)
    b = b->my_archive;

  return b->filename;
}

struct msg
{
  struct msg *next;
  asymbol *sym;
  const char *file;
};
static struct msg *msg_list_head = (struct msg *)0;

static struct msg *new_msg(asymbol *, const char *);
static struct msg *
new_msg(asymbol *sym, const char *file)
{
  struct msg *m = bfd_alloc(link_output_bfd, sizeof (struct msg));

  if (m == NULL)
    abort();

  m->next = NULL;
  m->sym = sym;
  m->file = file;
  return m;
}

static bfd_boolean first_time_msg(asymbol *, const char *);
static bfd_boolean
first_time_msg(asymbol *sym, const char *file)
{
  if (msg_list_head == NULL)
    {
      msg_list_head = new_msg(sym, file);
      return TRUE;
    }
  else
    {
      struct msg *prev,*m = msg_list_head;

      while (m) {
        if ((m->sym == sym) && (m->file == file))
          return FALSE;
        prev = m;
        m = m->next;
      }
      prev->next = new_msg(sym, file);
      return TRUE;
    }
}


/*****************************************************************************
 *
 * Special function for range checking relocations
 *
 * Includes generic code for all relocation types, and specific tests for
 * types whose entries in the HOWTO table specifies "complain_overflow_dont".
 * For those types, this function replaces the standard overflow checking
 * in bfd/reloc.c.
 *
 * Note that we can't use the range check macros defined in coff-pic30.h
 * because relocation is an unsigned type.
 *
 *****************************************************************************/
bfd_reloc_status_type
pic30_bfd_reloc_range_check(howto, relocation, abfd, symbol, error_msg)
  reloc_howto_type *howto;
  bfd_vma relocation;
  bfd *abfd;
  asymbol *symbol;
  char ** error_msg;

#define family pic30_proc_family
{
  bfd_reloc_status_type rc =  bfd_reloc_ok;

  /* if an input file references SFRs, and was created for
     a different processor family, issue a warning */
  if ((symbol->section == bfd_abs_section_ptr) &&
      (data_base_defined) &&
      (relocation < data_base) &&
      (pic30_force_link == 0) &&
      (global_PROCESSOR != 0) &&
      (abfd->arch_info->mach != 0) &&
      (pic30_is_generic_machine(abfd->arch_info->mach) == 0) &&
#if 0
      (abfd->arch_info->mach != bfd_mach_GENERIC_16BIT) &&
      (abfd->arch_info->mach != bfd_mach_GENERIC_16DSP) &&
#endif
      (family(global_PROCESSOR) != family(abfd->arch_info)) &&
      (strstr(symbol->name, "__") != symbol->name))
    {
      if (first_time_msg(symbol, abfd->filename))
        fprintf( stderr, "Warning: object %s%s%s was created for the %s"
                 " and references register %s\n",
                 abfd->filename,
                 abfd->my_archive ? " from library " : "",
                 abfd->my_archive ? outer_archive_name(abfd) : "",
                 abfd->arch_info->printable_name,
                 symbol->name);
      pic30_global_warning = 1;
    }

   switch (howto->type)
   {
     bfd_vma r_addr;

      case R_PIC30_PBYTE:
      case R_PIC30_SIGNED_10_BYTE:
      case R_PIC30_8:
        /* valid range is [-128..255] */
        if ((relocation > 0xFF) && ~(relocation | 0x7F))
          rc = bfd_reloc_overflow;
        break;
      case R_PIC30_WORD:
      case R_PIC30_16:
        /* valid range is [-32768..65535] */
        if ((relocation > 0xFFFF) && ~(relocation | 0x7FFF))
          rc = bfd_reloc_overflow;
        break;
      case R_PIC30_FILE_REG_WORD_WITH_DST:
        /* valid range is [0..65534] */
        r_addr = relocation << howto->rightshift;
        if ((r_addr > 0xFFFF))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "Address of \'%s\' is too large."
                    " Suggest __eds__ access qualifier (in C)"
                    " or edspage, edsoffset operators (in assembly)\n",
                    symbol->name);
            rc = bfd_reloc_dangerous;
          }
        break;
      case R_PIC30_PWORD:
        /* valid range is [--8388608..16777215] */
        if ((relocation > 0xFFFFFF) && ~(relocation | 0x7FFFFF))
          rc = bfd_reloc_overflow;
        break;
      case R_PIC30_PCREL_BRANCH_SLIT6:
        /* valid range is [-32..31] */
        if ((relocation > 0x1F) && ~(relocation | 0xC000001F))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "PC Relative branch to \'%s\' is out of range.",
                 symbol->name);
            rc = bfd_reloc_dangerous;
          }
        break;
      case R_PIC30_PCREL_BRANCH:
      case R_PIC30_BRANCH_ABSOLUTE:
        /* valid range is [-32768..32767] and not [-2, -1, 0] */
        if ((relocation > 0x7FFF) && ~(relocation | 0xC0007FFF))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "PC Relative branch to \'%s\' is out of range."
                    " Suggest large-code model.",
                 symbol->name);
            rc = bfd_reloc_dangerous;
          }
        break;
      case R_PIC30_PCREL_DO:
      case R_PIC30_DO_ABSOLUTE:
        /* valid range is [-32768..32767] and not [-2, -1, 0] */
        if ((relocation > 0x7FFF) && ~(relocation | 0xC0007FFF))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "DO loop branch to \'%s\' is out of range."
                    " Suggest greater optimization or code factoring.",
                 symbol->name);
            rc = bfd_reloc_dangerous;
            break;
          }

        if (((relocation & 0xFFFF) == 0xFFFE) ||
            ((relocation & 0xFFFF) == 0xFFFF) ||
            (relocation == 0))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "Illegal value for DO instruction offset (-2, -1, or 0)");
            rc = bfd_reloc_dangerous;
          }
        break;
      case R_PIC30_DSP_6:
        /* valid range is [-16..16] */
        if ((relocation > 0x10) && ~(relocation | 0xF))
          rc = bfd_reloc_overflow;
        break;
   }

   return rc;
}

/*****************************************************************************
 *
 * Special function for ensuring that the symbol being relocated is located at
 * an even address.
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Target specific assembler functions have already checked the
 *             bits.  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  If the address is ODD, generate an error message, and return
 *             DANGEROUS; otherwise, call the generic special function to
 *             finish howto processing.  NOTE:  We do not include the
 *             information that exists in the instruction as the assembler has
 *             already verified that it is even.
 *                An even + an odd => an odd.
 *                An even + an even => an even.
 *
 *****************************************************************************/

static bfd_reloc_status_type
pic30_elf32_is_even_address (abfd, reloc_entry, symbol, data, input_section,
                            output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd ATTRIBUTE_UNUSED;
   char ** error_msg;
{
   reloc_howto_type * howto = reloc_entry->howto;
   bfd_reloc_status_type rc;

   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       rc = bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0) &&
          (howto->type != R_PIC30_PCREL_ACCESS)) /* for CodeGuard */
         rc = bfd_reloc_undefined;
      else if (!PIC30_IS_EVEN (symbol->value + reloc_entry->addend))
      {
         *error_msg = (char *) malloc(BUFSIZ);
         sprintf(*error_msg,
                 "Cannot access symbol (%s) at an odd address.", symbol->name);
         rc = bfd_reloc_dangerous;
      } /* if (!PIC30_IS_EVEN (symbol->value + reloc_entry->addend)) */
      else
        /* finish by calling the generic special function */
        rc = pic30_elf32_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);

   } /* else */

   return rc;
}
/*****************************************************************************
 *
 * Special function for ensuring that the FILE_REG and FILE_REG_BYTE
 * relocations fall within the file register address space.
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Target specific assembler functions have already checked the
 *             bits.  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  Pre-calculate the relocation. If the result is >= FILE_REG_LIMIT,
 *             generate an error message, and return DANGEROUS.
 *             Otherwise continue the howto processing as follows:
 *
 *             if howto->type == R_PIC30_FILE_REG
 *               call pic30_elf32_is_even_address()
 *             else
 *               call pic30_elf32_perform_generic()
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_is_file_reg_address (abfd, reloc_entry, symbol, data, input_section,
                                output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd ATTRIBUTE_UNUSED;
   char ** error_msg;
{
   bfd_reloc_status_type rc;

   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       rc = bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      reloc_howto_type * howto = reloc_entry->howto;
      bfd_size_type octets = reloc_entry->address;
      bfd_size_type offset;
      bfd_vma relocation;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
        return bfd_reloc_undefined;  /* exit if symbol is undefined */

      /* pre-calculate the relocation */
      offset = pic30_elf32_extract_bytes (abfd, data, 4, octets, TRUE, TRUE);
      offset &= howto->src_mask;
      offset >>= howto->bitpos;

      offset += symbol->section->output_offset
        + reloc_entry->addend;

      relocation = symbol->value
        + symbol->section->output_section->vma
        + offset;

      relocation >>= (bfd_vma) howto->rightshift;

      /* range check the final value */
      if (relocation >= FILE_REG_LIMIT)
        {
          char *c1 = "Cannot access symbol ";
          char *c2 = "with file register addressing. Value must be"
                     " less than 8192. Suggest large-data model.";
          *error_msg = (char *) malloc(BUFSIZ);

          if (offset)
            sprintf(*error_msg, "%s(%s+%lu) %s", c1, symbol->name, offset, c2);
          else
            sprintf(*error_msg, "%s(%s) %s", c1, symbol->name, c2);
          return bfd_reloc_dangerous;  /* exit on overflow */
        }

      if (howto->type == R_PIC30_FILE_REG)
        /* finish by calling the even address special function */
        rc = pic30_elf32_is_even_address(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);
      else
        /* finish by calling the generic special function */
        rc = pic30_elf32_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);

   } /* else */

   return rc;
}
/*****************************************************************************
 *
 * Special function for a generic relocation.
 * 
 * This code is similar to the generic code in bfd_perform_relocation()
 * with the following exceptions:
 *
 * 1. partial relocations are not supported
 * 2. reloc_entry->address is assumed to be in "octets" units
 * 3. negative values for howto->size are ignored
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  Perform the generic relocation operation.
 *
 *****************************************************************************/

static bfd_reloc_status_type
pic30_elf32_perform_generic (abfd, reloc_entry, symbol, data,
                            input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;
{
  if (output_bfd != (bfd *) NULL)
    {
      /* ASSEMBLER */
      return bfd_reloc_ok;
    } /* if (output_bfd != (bfd *) NULL) */
  else
    {
      /* LINKER */
      bfd_vma relocation;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;

      /* only certain relocation types support undefined non-weak targets */
      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
        switch (howto->type)
          {
          case R_PIC30_CALL_ACCESS:
          case R_PIC30_PCREL_ACCESS:
            break;
          default:
            return bfd_reloc_undefined;
          }

      /*
      ** only certain relocation types support CODE targets
      */
      if (PIC30_IS_CODE_ATTR(symbol->section) || PIC30_IS_AUXFLASH_ATTR(symbol->section) )
      {
        switch (howto->type)
        {
	case R_PIC30_32:	/* DWARF-2 */
        case R_PIC30_PCREL_BRANCH_SLIT6:
        case R_PIC30_PCREL_BRANCH:
        case R_PIC30_PCREL_DO:
        case R_PIC30_PGM_ADDR_LSB:
        case R_PIC30_PGM_ADDR_MSB:
        case R_PIC30_DO_ABSOLUTE:
        case R_PIC30_BRANCH_ABSOLUTE:
        case R_PIC30_CALL_ACCESS:
        case R_PIC30_PCREL_ACCESS:
            /* these types don't use the generic special function */
            /* case R_PIC30_WORD_TBLOFFSET: */
            /* case R_PIC30_WORD_PSVOFFSET: */
            /* case R_PIC30_WORD_TBLPAGE: */
            /* case R_PIC30_WORD_PSVPAGE: */
            /* case R_PIC30_TBLOFFSET: */
            /* case R_PIC30_PSVOFFSET: */
            /* case R_PIC30_TBLPAGE: */
            /* case R_PIC30_PSVPAGE: */
            /* case R_PIC30_HANDLE: */
            /* case R_PIC30_PADDR: */
            /* case R_PIC30_P_TBLOFFSET: */
            /* case R_PIC30_P_PSVOFFSET: */
            /* case R_PIC30_P_TBLPAGE: */
            /* case R_PIC30_P_PSVPAGE: */
            /* case R_PIC30_P_HANDLE: */
            /* case R_PIC30_P_PADDR: */
            break;
        default:
            {
              *error_msg = (char *) malloc(BUFSIZ);
              sprintf(*error_msg, "Cannot use relocation type %s on a symbol (%s)"
                      " that is located in an executable section.",
                      howto->name, symbol->name);
              return bfd_reloc_dangerous;
            }
        }
      }

      /*
       * if reloc is PC relative, back up the data pointer
       * ...because pic30_elf32_perform_pc_relative() advanced it
       */
      if (howto->pc_relative)
        octets -= 4;

      /* Get symbol value.
         Common and undefined weak symbols resolve to zero. */
      if (bfd_is_com_section (symbol->section) ||
          (bfd_is_und_section (symbol->section) && (symbol->flags & BSF_WEAK)))
        relocation = 0;

      else if (bfd_is_und_section (symbol->section) &&
               ((howto->type == R_PIC30_CALL_ACCESS) ||
                (howto->type == R_PIC30_PCREL_ACCESS))) {

        /* target must be boot or secure access slot...  */
        /* calculate the relocation based on symbol name */
        bfd_boolean is_boot = !strncmp(symbol->name, "__boot_entry", 12);
        char *s = strstr(symbol->name, ":");
        unsigned int slot = atoi(++s);

        /* make sure that the access segment has been defined */
        if ((is_boot && !BOOT_IS_ACTIVE(FLASHx)) ||
            (!is_boot && !SECURE_IS_ACTIVE(FLASHx)))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                    "%s segment referenced, but not defined.",
                    is_boot ? "Boot" : "Secure");
            return bfd_reloc_dangerous;
          }

        relocation = is_boot ?
          base_address[BOOTx][FLASHx] + (slot * 2) :
          base_address[SECUREx][FLASHx] + (slot * 2) ;
      }
      else
        relocation = symbol->value;

      if ((symbol->section == 0) ||
          (symbol->section->output_section == 0)) {
                                   /* really this should not happen,
                                      but we must avoid a seg fault */
          *error_msg = (char *) malloc(BUFSIZ);
          sprintf(*error_msg, "Cannot resolve relocation (%s) because"
                  " symbol (%s) is undefined.",
                  howto->name, symbol->name);
          return bfd_reloc_undefined;
        }
      else
        reloc_target_output_section = symbol->section->output_section;

      /* Convert input-section-relative symbol value to absolute.  */
      if (output_bfd && howto->partial_inplace == FALSE)
        output_base = 0;
      else
        output_base = reloc_target_output_section->vma;

      relocation += output_base + symbol->section->output_offset;

      /* Add in supplied addend.  */
      relocation += reloc_entry->addend;

#if 0
  printf("\nsymbol->name = %s\n", symbol->name);
  printf("symbol->value = %x\n", symbol->value);
  printf("output_base = %x\n", output_base);
  printf("symbol->section->output_offset = %x\n", symbol->section->output_offset);
  printf("reloc_entry->address = %x\n", reloc_entry->address);
  printf("reloc_entry->addend = %x\n", reloc_entry->addend);
  printf("relocation = %x\n", relocation);
#endif

      /* Here the variable relocation holds the final address of the
         symbol we are relocating against, plus any addend.  */

      if (howto->pc_relative == TRUE)
        {
          /* This is a PC relative relocation.  We want to set RELOCATION
             to the distance between the address of the symbol and the
             location.  RELOCATION is already the address of the symbol.

             We start by subtracting the address of the section containing
             the location.

             If pcrel_offset is set, we must further subtract the position
             of the location within the section.  Some targets arrange for
             the addend to be the negative of the position of the location
             within the section; for example, i386-aout does this.  For
             i386-aout, pcrel_offset is FALSE.  Some other targets do not
             include the position of the location; for example, m88kbcs,
             or ELF.  For those targets, pcrel_offset is TRUE. */

          relocation -=
            input_section->output_section->vma + input_section->output_offset;

          if (howto->pcrel_offset == TRUE)
            relocation -= (reloc_entry->address / bfd_octets_per_byte (abfd));
        }

      /* FIXME: This overflow checking is incomplete, because the value
         might have overflowed before we get here.  For a correct check we
         need to compute the value in a size larger than bitsize, but we
         can't reasonably do that for a reloc the same size as a host
         machine word.
         FIXME: We should also do overflow checking on the result after
         adding in the value contained in the object file.  */
      if (howto->complain_on_overflow != complain_overflow_dont
          && flag == bfd_reloc_ok)
        flag = bfd_check_overflow (howto->complain_on_overflow,
                                   howto->bitsize,
                                   howto->rightshift,
                                   bfd_arch_bits_per_address (abfd),
                                   relocation);

      /* apply the rightshift parameter */
      relocation >>= (bfd_vma) howto->rightshift;

#if 0
      printf("Final relocation = %lx at %lx\n", relocation, octets);
#endif

      /* Call the pic30-specific range check routine */
      if (flag == bfd_reloc_ok)
        flag = pic30_bfd_reloc_range_check(howto, relocation,
                                           abfd, symbol, error_msg);

      /* Shift everything up to where it's going to be used */
      relocation <<= (bfd_vma) howto->bitpos;

#define DOIT(x) \
  x = ( (x & ~howto->dst_mask) | (((x & howto->src_mask) +  relocation) & howto->dst_mask))

      switch (howto->size)
        {
        case 0:
          {
            char x = bfd_get_8 (abfd, (char *) data + octets);
            DOIT (x);
            bfd_put_8 (abfd, x, (unsigned char *) data + octets);
          }
          break;

        case 1:
          {
            short x = bfd_get_16 (abfd, (bfd_byte *) data + octets);
            DOIT (x);
            bfd_put_16 (abfd, x, (unsigned char *) data + octets);
          }
          break;

        case 2:
          {
            long x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
            DOIT (x);
            bfd_put_32 (abfd, x, (bfd_byte *) data + octets);
          }
          break;

        case 3:
          /* Do nothing */
          break;

        case 4:
#ifdef BFD64
          {
            bfd_vma x = bfd_get_64 (abfd, (bfd_byte *) data + octets);
            DOIT (x);
            bfd_put_64 (abfd, x, (bfd_byte *) data + octets);
          }
#else
          abort ();
#endif
          break;
        default:
          return bfd_reloc_other;
        }

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
    }
}
/*****************************************************************************
 *
 * Special function for data directive relocations (.byte, .word, .long).
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  This special function is used in both the CODE and DATA sections
 *             for symbols located in both the CODE and DATA sections.
 *
 *             If the relocation site is in a CODE section, then the data
 *             placed there by the assembler will skip the upper and phantom
 *             bytes.  Extract the data to add to the relocation, skipping the
 *             upper and phantom bytes if necessary.  Then after checking for
 *             overflow, insert the calculated relocation back into the
 *             relocation site, skipping the upper and phantom bytes if
 *             necessary.
 *
 * Note:  Unlike other offsets and lengths, the relocation address
 *        is provided in octets, not PC units.
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_data_directive (abfd, reloc_entry, symbol, data,
                                   input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;
{
   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       return bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      bfd_vma relocation,offset;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;
      int count = (howto->size == 0) ? 1 : (2 * howto->size);

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
         return bfd_reloc_undefined;

      /* Get symbol value.  (Common symbols are special.)  */
      if (bfd_is_com_section (symbol->section))
         relocation = 0;
      else
         relocation = symbol->value;

      reloc_target_output_section = symbol->section->output_section;
      output_base = reloc_target_output_section->vma;

#if 0
      /* DEBUG */
      printf ("symbol->name = %s\n", symbol->name);
      printf ("symbol->value = 0x%x\n", symbol->value);
      printf ("output_base = 0x%x\n", output_base);
      printf ("reloc_target_output_section->lma = 0x%x\n",
              reloc_target_output_section->lma);
      printf ("symbol->section->output_offset = 0x%x\n",
              symbol->section->output_offset);
#endif

      relocation += symbol->section->output_offset;

      /* Add in supplied addend. */
      relocation += reloc_entry->addend;

      /* Add in the base of the mapped section. */
      relocation += output_base; 

      relocation >>= (bfd_vma) howto->rightshift;

#if 0
      printf("relocation = 0x%x\n", (long) relocation);
#endif

      /* Extract offset data from the encoded instruction. */
      if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
         /* Relocation site is program memory, skip upper & phantom bytes */
         offset = pic30_elf32_extract_bytes (abfd, data, count, octets,
                                           TRUE, TRUE);
      else
         /* Relocation site is data memory */
         offset = pic30_elf32_extract_bytes_data_mem(abfd, data, count, octets);

#if 0
      printf ("offset = %x\n", offset);
#endif

      /* Add in the offset */
      relocation += offset;

      /* Check final relocation for overflow */
      if (howto->complain_on_overflow != complain_overflow_dont)
         flag = bfd_check_overflow (howto->complain_on_overflow,
			            howto->bitsize,
			            0, /* rightshift already applied */
			            bfd_arch_bits_per_address (abfd),
			            relocation);

      if (flag == bfd_reloc_ok)
        flag = pic30_bfd_reloc_range_check (howto, relocation,
                                            abfd, symbol, error_msg);

      /* Insert the final relocation into the BFD */
      if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
         /* Relocation site is program memory, skip upper & phantom bytes */
         pic30_elf32_insert_bytes (abfd, data, count, octets, relocation,
                                  TRUE, TRUE);
      else
         /* Relocation site is data memory */
         pic30_elf32_insert_bytes_data_mem(abfd,data,count,octets,relocation);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   }
}
/*****************************************************************************
 *
 * Special function for p-variant data directive relocations (.pbyte, .pword).
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  This special function is used in the CODE section
 *             for symbols located in both the CODE and DATA sections.  If
 *             this special function is called in a DATA section, then an
 *             error occurred in the assembler, and return NOTSUPPORTED.
 *             If the relocation site is in the CODE section, then the data
 *             placed there by the assembler will skip the phantom byte.
 *             Extract the data to add to the relocation skipping the
 *             phantom byte if necessary.  Then after checking for
 *             overflow, insert the calculated relocation back into the
 *             relocation site skipping the phantom byte if necessary.
 *
 * Note:  Unlike other offsets and lengths, the relocation address
 *        is provided in octets, not PC units.
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_p_data_directive (abfd, reloc_entry, symbol, data,
                                     input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;
{
   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       return bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      bfd_vma relocation, offset;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address ;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;
      int count = howto->bitsize / 8;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
         return bfd_reloc_undefined;

      /* If the relocation site is not in a CODE or AUXFLASH section, then this relocation
         is not supported.  Linker will abort if this ever occurs.  This means
         that there is a problem in the assembler! */
      if (((input_section->output_section->flags & SEC_CODE) == 0) &&
	  (input_section->output_section->auxflash == 0))
         return bfd_reloc_notsupported;   /* exit if not a CODE or AUXFLASH section */

      /* Get symbol value.  (Common symbols are special.)  */
      if (bfd_is_com_section (symbol->section))
         relocation = 0;
      else
         relocation = symbol->value;

      reloc_target_output_section = symbol->section->output_section;
      output_base = reloc_target_output_section->vma;

#if 0
      /* DEBUG */
      printf ("symbol->name = %s\n", symbol->name);
      printf ("symbol->value = 0x%x\n", symbol->value);
      printf ("output_base = 0x%x\n", output_base);
      printf ("reloc_target_output_section->lma = 0x%x\n",
              reloc_target_output_section->lma);
      printf ("symbol->section->output_offset = 0x%x\n",
              symbol->section->output_offset);
#endif

      relocation += symbol->section->output_offset;

      /* Add in supplied addend.  */
      relocation += reloc_entry->addend;

      /* Add in the base of the mapped section. */
      relocation += output_base; 

      relocation >>= (bfd_vma) howto->rightshift;

      /* Extract offset data from the encoded instruction, skipping phantom
         byte */
      offset = pic30_elf32_extract_bytes (abfd, data, count, octets,
                                         FALSE, TRUE);

#if 0
      printf("offset = %x\n",offset);
      printf("output_base = %lx\n",output_base);
#endif

      /* Add in the offset */
      relocation += offset;

#if 0
      printf("relocation = %x\n",relocation);
#endif

      /* check final relocation for overflow */
      if (howto->complain_on_overflow != complain_overflow_dont)
         flag = bfd_check_overflow (howto->complain_on_overflow,
			            howto->bitsize,
			            0, /* rightshift already applied */
			            bfd_arch_bits_per_address (abfd),
			            relocation);

      if (flag == bfd_reloc_ok)
        flag = pic30_bfd_reloc_range_check (howto, relocation,
                                            abfd, symbol, error_msg);
             

      /* Insert the final relocation into the instruction, skipping phantom
         byte */
#if 0
      printf("octets = %lx\n", octets);
      printf("count = %u\n", count);
#endif
      pic30_elf32_insert_bytes (abfd, data, count, octets, relocation,
                               FALSE, TRUE);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
}

static bfd_boolean
operator_is_instruction (unsigned int reloc) {

  switch (reloc)
    {
    case R_PIC30_WORD_TBLOFFSET:
    case R_PIC30_WORD_PSVOFFSET:
    case R_PIC30_WORD_TBLPAGE:
    case R_PIC30_WORD_PSVPAGE:
    case R_PIC30_WORD_PSVPTR:
    case R_PIC30_WORD_ACCESS:
    case R_PIC30_WORD_EDSPAGE:
    case R_PIC30_WORD_EDSOFFSET:
    case R_PIC30_WORD_HANDLE:
    case R_PIC30_UNSIGNED_10_TBLOFFSET:
    case R_PIC30_UNSIGNED_10_PSVOFFSET:
    case R_PIC30_UNSIGNED_10_TBLPAGE:
    case R_PIC30_UNSIGNED_10_PSVPAGE:
    case R_PIC30_UNSIGNED_10_PSVPTR:
    case R_PIC30_UNSIGNED_10_ACCESS:
    case R_PIC30_UNSIGNED_10_EDSPAGE:
    case R_PIC30_UNSIGNED_10_EDSOFFSET:
    case R_PIC30_UNSIGNED_10_HANDLE:
      return TRUE;
    default:
      return FALSE;
    }
}

#define RETURN_ERROR(s)                  \
{                                        \
  *error_msg = (char *) malloc(BUFSIZ);  \
  sprintf(*error_msg, (s),               \
          howto->name, symbol->name);    \
  return bfd_reloc_dangerous;            \
}

/*****************************************************************************
 *
 * Special function for the following operators:
 *
 * tblpage(), tbloffset()
 * psvpage(), psvoffset()
 * handle(),  paddr(),     psvptr(),
 * boot(),    secure(),
 * edspage(), edsoffset()
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  This special function is used in both the CODE and DATA sections
 *             for symbols located in a CODE, PSV, or EEDATA section. If this
 *             special function is called to relocate a symbol in a DATA
 *             section, then an error message is generated and DANGEROUS
 *             is returned. Undefined weak symbols are acceptable; they
 *             resolve to a value of zero.
 *
 *             If the relocation site is in a CODE section and a data
 *             allocation directive was used, then the data placed there by
 *             the assembler will skip the upper and phantom bytes.  Extract
 *             the data to add to the relocation skipping the upper and
 *             phantom bytes if necessary.  Add this value to the relocation
 *             and then apply the operator.  Then after checking for overflow,
 *             insert the calculated relocation back into the relocation site
 *             skipping the upper and phantom bytes if necessary.
 * 
 * Note1:  This is a 32-bit operation because the reloc may not be
 *         right-justified in the instruction (howto->bitpos > 0).
 *
 * Note2:  Unlike other offsets and lengths, the relocation address
 *         is provided in octets, not PC units.
 *
 * Note3:  These kind of relocations must not have a "rightshift"
 *         defined in the howto structure.
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_operators (abfd, reloc_entry, symbol, data,
                              input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;
{
   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       return bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      bfd_vma relocation, target;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;
      unsigned long x;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
        switch (howto->type)
          {
          case R_PIC30_ACCESS:
          case R_PIC30_L_ACCESS:
          case R_PIC30_WORD_ACCESS:
          case R_PIC30_UNSIGNED_10_ACCESS:
            break;
          default:
            return bfd_reloc_undefined;
          }

      /*
      ** Test the section type of the target symbol.
      */
      switch (howto->type)
        {
        case R_PIC30_EDSPAGE:
        case R_PIC30_WORD_EDSPAGE:
        case R_PIC30_UNSIGNED_10_EDSPAGE:
        case R_PIC30_EDSOFFSET:
        case R_PIC30_WORD_EDSOFFSET:
        case R_PIC30_UNSIGNED_10_EDSOFFSET:
          /*
          ** Allow targets in any data or code section,
          ** but not external memory, or absolute sections.
          */
          if (bfd_is_abs_section (symbol->section))
            RETURN_ERROR("Cannot use %s on an absolute symbol (%s).")

          break;

        default:
          /*
          ** Allow targets in code, psv, and eedata sections;
          ** also allow weak symbols (undefined sections) and
          ** absolute symbols (absolute sections).
          */
          if (!PIC30_IS_CODE_ATTR(symbol->section) &&
              !PIC30_IS_PSV_ATTR(symbol->section) &&
              !PIC30_IS_EEDATA_ATTR(symbol->section) &&
	      !PIC30_IS_AUXFLASH_ATTR(symbol->section) &&
              !bfd_is_und_section (symbol->section) &&
              !bfd_is_abs_section (symbol->section))
            RETURN_ERROR("Cannot use %s on a symbol (%s) that is not "
                         "located in an code, psv, or eedata section.")
        } /* switch */

      /* Get symbol value.
         Common and undefined weak symbols resolve to zero. */
      if (bfd_is_com_section (symbol->section) ||
          (bfd_is_und_section (symbol->section) && (symbol->flags & BSF_WEAK)))
         relocation = 0;

      else if (bfd_is_und_section (symbol->section) &&
               ((howto->type == R_PIC30_ACCESS) ||
                (howto->type == R_PIC30_L_ACCESS) ||
                (howto->type == R_PIC30_WORD_ACCESS) ||
                (howto->type == R_PIC30_UNSIGNED_10_ACCESS))) {

        /* target must be boot or secure access slot...  */
        /* calculate the relocation based on symbol name */
        bfd_boolean is_boot = !strncmp(symbol->name, "__boot_entry", 12);
        char *s = strstr(symbol->name, ":");
        unsigned int slot = atoi(++s);

        /* make sure that the access segment has been defined */
        if ((is_boot && !BOOT_IS_ACTIVE(FLASHx)) ||
            (!is_boot && !SECURE_IS_ACTIVE(FLASHx)))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                    "%s segment referenced, but not defined.",
                    is_boot ? "Boot" : "Secure");
            return bfd_reloc_dangerous;
          }

        relocation = is_boot ?
          base_address[BOOTx][FLASHx] + (slot * 2) :
          base_address[SECUREx][FLASHx] + (slot * 2) ;
      }
      else
         relocation = symbol->value;

      reloc_target_output_section = symbol->section->output_section;

      /* Convert input-section-relative symbol value to absolute.  */
      output_base = reloc_target_output_section->lma;

#if 0
      /* DEBUG */
      printf ("symbol->name = %s\n", symbol->name);
      printf ("symbol->value = 0x%x\n", symbol->value);
      printf ("output_base = 0x%x\n", output_base);
      printf ("symbol->section->output_offset = 0x%x\n",
              symbol->section->output_offset);
      printf ("reloc_entry->addend = %#x\n", reloc_entry->addend);
#endif

      relocation += output_base + symbol->section->output_offset;

      /* Add in supplied addend.  */
      relocation += reloc_entry->addend;

      /* resolve handle */
      if ((howto->type == R_PIC30_WORD_HANDLE) ||
          (howto->type == R_PIC30_HANDLE) ||
          (howto->type == R_PIC30_UNSIGNED_10_HANDLE))
        relocation = bfd_pic30_handle (abfd, relocation, symbol);
      
      /* we don't apply any howto->rightshift, because we may
         add an offset that was encoded in the instruction */

      /* Add in data from the current program word */
      if (operator_is_instruction(howto->type))
      { 
         /* do it the easy way */
         x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
         target = ((x & howto->src_mask) >> howto->bitpos);
      } /* if */
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_IS_CODE_ATTR (input_section->output_section)||
	     PIC30_IS_AUXFLASH_ATTR (input_section->output_section))
	 {
           /* Relocation site is program or auxflash memory, skip upper & phantom bytes */
            target = pic30_elf32_extract_bytes (abfd, data, count, octets,
                                               TRUE, TRUE);
	 }
         else
	 {
           /* Relocation site is data memory */
            target = pic30_elf32_extract_bytes_data_mem (abfd, data, count, octets);
	 }
      } /* else */

      relocation += target;

      /*
       * do anything special
       * that is not handled by the howto structure
       */
      switch (howto->type)
        {
        case R_PIC30_TBLPAGE:
        case R_PIC30_WORD_TBLPAGE:
        case R_PIC30_UNSIGNED_10_TBLPAGE:
          relocation = PIC30_TBLPAGE(relocation);
          /* on EDS machines, set a bit that the compiler
             will rotate into position for DSRPAG */
          if (pic30_is_eds_machine(global_PROCESSOR))
            relocation |= 0x100;
          break;
        case R_PIC30_TBLOFFSET:
        case R_PIC30_WORD_TBLOFFSET:
        case R_PIC30_UNSIGNED_10_TBLOFFSET:
          relocation = PIC30_TBLOFFSET (relocation);
          break;
        case R_PIC30_PSVOFFSET:
        case R_PIC30_WORD_PSVOFFSET:
        case R_PIC30_UNSIGNED_10_PSVOFFSET:
          relocation = PIC30_PSVOFFSET (relocation);
          break;
        case R_PIC30_PSVPAGE:
        case R_PIC30_WORD_PSVPAGE:
        case R_PIC30_UNSIGNED_10_PSVPAGE:
        case R_PIC30_EDSPAGE:
        case R_PIC30_WORD_EDSPAGE:
        case R_PIC30_UNSIGNED_10_EDSPAGE:
          relocation = PIC30_PSVPAGE (relocation);
          if (pic30_is_eds_machine(global_PROCESSOR) &&
              (PIC30_SECTION_IN_PROGRAM_MEMORY(reloc_target_output_section) ||
               PIC30_SECTION_IN_PSV_MEMORY(reloc_target_output_section)))
            relocation |= 0x200;
          else if ((relocation == 0) &&
              PIC30_SECTION_IN_DATA_MEMORY(reloc_target_output_section))
            relocation = 0;
           if ((relocation == 0) &&
              ((howto->type == R_PIC30_EDSPAGE) ||
               (howto->type == R_PIC30_WORD_EDSPAGE) ||
               (howto->type == R_PIC30_UNSIGNED_10_EDSPAGE)))
            relocation = 0;
          break;
        case R_PIC30_EDSOFFSET:
        case R_PIC30_WORD_EDSOFFSET:
        case R_PIC30_UNSIGNED_10_EDSOFFSET:
          if (PIC30_SECTION_IN_DATA_MEMORY(reloc_target_output_section) &&
              ((relocation & ~ (bfd_vma)0x7FFF) == 0))
            {} /* use true address */
          else
            relocation = PIC30_EDSOFFSET (relocation);
          break;
        case R_PIC30_PSVPTR:
        case R_PIC30_L_PSVPTR:
        case R_PIC30_WORD_PSVPTR:
        case R_PIC30_UNSIGNED_10_PSVPTR:
          RETURN_ERROR("%s(%s) is obsolete. Please re-build this file.")

        default:
          break;
        } /* switch */

      if (howto->complain_on_overflow != complain_overflow_dont)
         flag = bfd_check_overflow (howto->complain_on_overflow,
                                    howto->bitsize,
                                    0, /* rightshift already applied */
                                    /* bfd_arch_bits_per_address (abfd), */
                                    PIC30_BITS_PER_PROGRAM_ADDRESS,
                                    relocation);

#if 0
      printf("relocation = 0x%x\n", (long) relocation);
#endif

      /* Insert the final relocation */
      if (operator_is_instruction(howto->type))
      {
        /* If reloc site is an instruction, do it the easy way */
        x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
        x = ((x & ~howto->dst_mask) |
             ((relocation << howto->bitpos) & howto->dst_mask));
        bfd_put_32 (abfd, x, (unsigned char *) data + octets);
      } /* if */
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_IS_CODE_ATTR (input_section->output_section) ||
	     PIC30_IS_AUXFLASH_ATTR (input_section->output_section))
	 {
           /* Relocation site is program memory, skip upper & phantom bytes */
            pic30_elf32_insert_bytes (abfd, data, count, octets, relocation,
                                     TRUE, TRUE);
	 }
         else
	 {
           /* Relocation site is data memory */
            pic30_elf32_insert_bytes_data_mem (abfd, data,
                                              count, octets, relocation);
	 }
      } /* else */

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
}
/*****************************************************************************
 *
 * Special function for the following operators, when they appear
 * in p-variant data directives:
 *
 * tblpage(), tbloffset()
 * psvpage(), psvoffset()
 * handle(),  paddr(),     psvptr(),
 * boot(),    secure(),
 * edspage(), edsoffset()
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  This special function is used in CODE sections for symbols
 *             located in a CODE, PSV, or EEDATA section. If this special
 *             function is called to relocate a symbol in a DATA section, then
 *             an error message is generated and DANGEROUS is returned.
 *             Undefined weak symbols are acceptable; they resolve to
 *             a value of zero.
 *
 *             If the relocation site is in a CODE section and a data
 *             allocation directive was used, then the data placed there by
 *             the assembler will skip the phantom byte. Extract the data to
 *             add to the relocation, skipping the phantom byte if necessary.
 *             Then after checking for overflow, insert the calculated
 *             relocation back into the relocation site skipping the
 *             phantom byte if necessary.
 *
 * Note:  Unlike other offsets and lengths, the relocation address
 *        is provided in octets, not PC units.
 *
 *        These kind of relocations must not have a "rightshift"
 *        defined in the howto structure.
 *
*****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_p_operators (abfd, reloc_entry, symbol, data,
                                input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;
{
   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       return bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      bfd_vma relocation, target;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;
      int count = howto->bitsize / 8;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0) &&
          (howto->type != R_PIC30_P_ACCESS))
         return bfd_reloc_undefined;

      /*
      ** Test the section type of the target symbol.
      */
      switch (howto->type)
        {
        case R_PIC30_P_EDSPAGE:
        case R_PIC30_P_EDSOFFSET:
          /*
          ** Allow targets in any data or code section,
          ** but not external memory or absolute sections.
          */
          if (bfd_is_abs_section (symbol->section))
            RETURN_ERROR("Cannot use %s on an absolute symbol (%s).")

          break;

        default:
          /*
          ** Allow targets in code, psv, or eedata sections;
          ** also allow weak symbols (undefined sections) and
          ** absolute symbols (absolute sections).
          */
          if (!PIC30_IS_CODE_ATTR(symbol->section) &&
              !PIC30_IS_PSV_ATTR(symbol->section) &&
              !PIC30_IS_EEDATA_ATTR(symbol->section) &&
	      !PIC30_IS_AUXFLASH_ATTR(symbol->section) &&
              !bfd_is_und_section (symbol->section) &&
              !bfd_is_abs_section (symbol->section))
            RETURN_ERROR("Cannot use %s on a symbol (%s) that is not "
                         "located in an code, psv, or eedata section.")
        } /* switch */

      /* If the relocation site is not in a CODE or AUXFLASH section, then this relocation
         is not supported.  Linker will abort if this ever occurs.  This means
         that there is a problem in the assembler! */
      if (((input_section->output_section->flags & SEC_CODE) == 0) &&
	  (input_section->output_section->auxflash == 0))
         return bfd_reloc_notsupported;   /* exit if not a CODE or AUXFLASH section */

      /* Get symbol value.
         Common and undefined weak symbols resolve to zero. */
      if (bfd_is_com_section (symbol->section) ||
          (bfd_is_und_section (symbol->section) && (symbol->flags & BSF_WEAK)))
         relocation = 0;

      else if (bfd_is_und_section (symbol->section) &&
               (howto->type == R_PIC30_P_ACCESS)) {

        /* target must be boot or secure access slot...  */
        /* calculate the relocation based on symbol name */
        bfd_boolean is_boot = !strncmp(symbol->name, "__boot_entry", 12);
        char *s = strstr(symbol->name, ":");
        unsigned int slot = atoi(++s);

        /* make sure that the access segment has been defined */
        if ((is_boot && !BOOT_IS_ACTIVE(FLASHx)) ||
            (!is_boot && !SECURE_IS_ACTIVE(FLASHx)))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                    "%s segment referenced, but not defined.",
                    is_boot ? "Boot" : "Secure");
            return bfd_reloc_dangerous;
          }

        relocation = is_boot ?
          base_address[BOOTx][FLASHx] + (slot * 2) :
          base_address[SECUREx][FLASHx] + (slot * 2) ;
      }
      else
         relocation = symbol->value;

      reloc_target_output_section = symbol->section->output_section;

      /* Convert input-section-relative symbol value to absolute.  */
      output_base = reloc_target_output_section->lma;

#if 0
      /* DEBUG */
      printf ("symbol->name = %s\n", symbol->name);
      printf ("symbol->value = 0x%x\n", symbol->value);
      printf ("output_base = 0x%x\n", output_base);
      printf ("symbol->section->output_offset = 0x%x\n",
              symbol->section->output_offset);
      printf("reloc_entry->addend = %#x\n", reloc_entry->addend);
#endif

      relocation += output_base + symbol->section->output_offset;

      /* Add in supplied addend.  */
      relocation += reloc_entry->addend;

      /* resolve handle */
      if ((howto->type == R_PIC30_P_HANDLE))
        relocation = bfd_pic30_handle (abfd, relocation, symbol);
      
      /* we don't apply any howto->rightshift, because we may
         add an offset that was encoded in the instruction */

      /* Extract data byte-wise, avoiding phantom bytes */
      target = pic30_elf32_extract_bytes (abfd, data, count, octets,
                                         FALSE, TRUE);
      relocation += target;

      /*
       * do anything special
       * that is not handled by the howto structure
       */
      switch (howto->type)
        {
        case R_PIC30_P_TBLPAGE:
          relocation = PIC30_TBLPAGE(relocation);
          /* on EDS machines, set a bit that the compiler
             will rotate into position for DSRPAG */
          if (pic30_is_eds_machine(global_PROCESSOR))
            relocation |= 0x100;
          break;
        case R_PIC30_P_TBLOFFSET:
          relocation = PIC30_TBLOFFSET (relocation);
          break;
        case R_PIC30_P_PSVOFFSET:
          relocation = PIC30_PSVOFFSET (relocation);
          break;
        case R_PIC30_P_PSVPAGE:
        case R_PIC30_P_EDSPAGE:
          relocation = PIC30_PSVPAGE (relocation);
          if (pic30_is_eds_machine(global_PROCESSOR) &&
              (PIC30_SECTION_IN_PROGRAM_MEMORY(reloc_target_output_section) ||
               PIC30_SECTION_IN_PSV_MEMORY(reloc_target_output_section)))
            relocation |= 0x200;
          else if ((relocation == 0) &&
              PIC30_SECTION_IN_DATA_MEMORY(reloc_target_output_section))
            relocation = 1;
          break;
        case R_PIC30_P_EDSOFFSET:
          if (PIC30_SECTION_IN_DATA_MEMORY(reloc_target_output_section) &&
              ((relocation & ~ (bfd_vma)0x7FFF) == 0))
            {} /* use true address */
          else
            relocation = PIC30_EDSOFFSET (relocation);
          break;
        case R_PIC30_P_PSVPTR:
          RETURN_ERROR("%s(%s) is obsolete. Please re-build this file.")

        default:
          break;
        } /* switch */

      if (howto->complain_on_overflow != complain_overflow_dont)
         flag = bfd_check_overflow (howto->complain_on_overflow,
                                    howto->bitsize,
                                    0, /* rightshift already applied */
                                    /* bfd_arch_bits_per_address (abfd), */
                                    PIC30_BITS_PER_PROGRAM_ADDRESS,
                                    relocation);

#if 0
      printf("relocation = 0x%x\n", (long) relocation);
#endif

      /* Insert data byte-wise, avoiding phantom bytes */
      pic30_elf32_insert_bytes (abfd, data, count, octets, relocation,
                               FALSE, TRUE);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
}

/*****************************************************************************
 *
 * Special function for dmaoffset() & dmapage() operator.
 *
 * Only want to perform the special function if in the linker.
 *
 * The following relocation types are handled by this function:
 * 
 *   BFD_RELOC_PIC30_DMAOFFSET       <-- normal data directive
 *   BFD_RELOC_PIC30_P_DMAOFFSET     <-- p-type data directive
 *   BFD_RELOC_PIC30_WORD_DMAOFFSET  <-- mov #lit16, Wn
 *   BFD_RELOC_PIC30_DMAPAGE         <-- normal data directive
 *   BFD_RELOC_PIC30_P_DMAPAGE       <-- p-type data directive
 *   BFD_RELOC_PIC30_WORD_DMAPAGE    <-- mov #lit16, Wn
 * 
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  This special function is used in both the CODE and DATA sections
 *             for symbols located only in a DMA section if the device does not  
 *             support the new version of DMA. The new DMA can address all data  
 *             memory space including EDS but excluding SFR. So, the symbol may  
 *             not be located in a DMA section. If this special function is      
 *             called to relocate a symbol in any other section and the device   
 *             does not support new version of DMA, then an error message is     
 *             generated and DANGEROUS is returned.
 *
 *             If the relocation site is in a CODE section and a data
 *             allocation directive was used, then the data placed there by
 *             the assembler will skip the upper and phantom bytes.  Extract
 *             the data to add to the relocation skipping the upper and
 *             phantom bytes if necessary.  Add this value to the relocation
 *             and then apply the operator.  Then after checking for overflow,
 *             insert the calculated relocation back into the relocation site
 *             skipping the upper and phantom bytes if necessary.
 * 
 *             If the relocation site is in a CODE section and a P-type data
 *             allocation directive was used, then the data placed there by
 *             the assembler will skip only the phantom byte.  Extract
 *             the data to add to the relocation skipping the
 *             phantom byte if necessary.  Add this value to the relocation
 *             and then apply the operator.  Then after checking for overflow,
 *             insert the calculated relocation back into the relocation site
 *             skipping the phantom byte if necessary.
 * 
 * Note1:  This is a 32-bit operation because the reloc may not be
 *         right-justified in the instruction (howto->bitpos > 0).
 *
 * Note2:  Unlike other offsets and lengths, the relocation address
 *         is provided in octets, not PC units.
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_dmaoffset_dmapage (abfd, reloc_entry, symbol, data,
                              input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;


{
   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       return bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      bfd_vma relocation, target;
      bfd_vma output_base = 0;
      bfd_size_type octets = reloc_entry->address;
      bfd_reloc_status_type flag = bfd_reloc_ok;
      reloc_howto_type * howto = reloc_entry->howto;
      asection * reloc_target_output_section;
      unsigned long x;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
         return bfd_reloc_undefined;

      if (!PIC30_IS_DMA_ATTR(symbol->section) &&
          !bfd_is_und_section (symbol->section) &&
          (pic30_is_dma_machine(global_PROCESSOR) == 1))
      {
         *error_msg = (char *) malloc(BUFSIZ);
         sprintf(*error_msg, "Cannot use %s on a symbol (%s) that is not "
                 "located in a dma section.",
                 howto->name, symbol->name);
         return bfd_reloc_dangerous;
      }


      /* Get symbol value.
         Common and undefined weak symbols resolve to zero. */
      if (bfd_is_com_section (symbol->section) ||
          (bfd_is_und_section (symbol->section) && (symbol->flags & BSF_WEAK)))
         relocation = 0;
      else
         relocation = symbol->value;

      reloc_target_output_section = symbol->section->output_section;

      /* Convert input-section-relative symbol value to absolute.  */
      output_base = reloc_target_output_section->lma;

#if 0
      /* DEBUG */
      printf ("\nhowto->name = %s\n", howto->name);
      printf ("symbol->name = %s\n", symbol->name);
      printf ("symbol->value = 0x%x\n", symbol->value);
      printf ("output_base = 0x%x\n", output_base);
      printf ("symbol->section->output_offset = 0x%x\n",
              symbol->section->output_offset);
      printf ("reloc_entry->addend = %#x\n", reloc_entry->addend);
#endif

      relocation += output_base + symbol->section->output_offset;

      /* Add in supplied addend.  */
      relocation += reloc_entry->addend;

      relocation >>= (bfd_vma) howto->rightshift;

      /* Add in data from the current program word */
      if ((howto->type == R_PIC30_WORD_DMAOFFSET) ||
         (howto->type == R_PIC30_WORD_DMAPAGE) ||
         (howto->type == R_PIC30_UNSIGNED_10_DMAOFFSET) ||
         (howto->type == R_PIC30_UNSIGNED_10_DMAPAGE))
      { 
         /* If instruction, do it the easy way */
         x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
         target = ((x & howto->src_mask) >> howto->bitpos);
      } /* if */
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
           {
             /* Relocation site is program memory */
             bfd_boolean skip_upper = (howto->type == R_PIC30_DMAOFFSET);
             target = pic30_elf32_extract_bytes (abfd, data, count, octets,
                                                 skip_upper, TRUE);
           }
         else
           /* Relocation site is data memory */
            target = pic30_elf32_extract_bytes_data_mem (abfd, data, count, octets);
      } /* else */

      relocation += target;

      /* calculate dmaoffset here */
      if ((howto->type == R_PIC30_DMAOFFSET) ||                                           (howto->type == R_PIC30_WORD_DMAOFFSET)||
	  (howto->type == R_PIC30_P_DMAOFFSET))
         
	{
          if (dma_base_defined == FALSE)  /* this should have been checked earlier */
            {                             /*   when the section was allocated      */
             fprintf( stderr, 
                   "Link Error: __DMA_BASE is needed, but not defined (check linker script?)\n");
             abort();
            }
          if (pic30_is_dma_machine(global_PROCESSOR) == 2)
             {
              relocation = relocation;
             }
          else if (relocation < dma_base)  /* should never occur */
             {
              fprintf( stderr, "Link Error: DMA allocation error\n");
              abort();
             }
          else
             relocation -= dma_base;
        }
      else if (howto->type == R_PIC30_UNSIGNED_10_DMAOFFSET)
	relocation = relocation;

      else /* calculate dmapage here */
	relocation = PIC30_TBLPAGE(relocation);

      if (howto->complain_on_overflow != complain_overflow_dont)
         flag = bfd_check_overflow (howto->complain_on_overflow,
                                    howto->bitsize,
                                    0, /* rightshift already applied */
                                    /* bfd_arch_bits_per_address (abfd), */
                                    PIC30_BITS_PER_PROGRAM_ADDRESS,
                                    relocation);

#if 0
      printf("relocation = 0x%x\n", (long) relocation);
#endif

      /* Insert the final relocation */
      if ((howto->type == R_PIC30_WORD_DMAOFFSET) ||
         (howto->type == R_PIC30_WORD_DMAPAGE) ||
         (howto->type == R_PIC30_UNSIGNED_10_DMAOFFSET) ||
         (howto->type == R_PIC30_UNSIGNED_10_DMAPAGE))
      {
        /* If reloc site is an instruction, do it the easy way */
        x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
        x = ((x & ~howto->dst_mask) |
             ((relocation << howto->bitpos) & howto->dst_mask));
        bfd_put_32 (abfd, x, (unsigned char *) data + octets);
      } /* if */
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
           {
             /* Relocation site is program memory */
             bfd_boolean skip_upper = (howto->type == R_PIC30_DMAOFFSET);
             pic30_elf32_insert_bytes (abfd, data, count, octets,
                                       relocation, skip_upper, TRUE);
           }
         else
           /* Relocation site is data memory */
            pic30_elf32_insert_bytes_data_mem (abfd, data,
                                              count, octets, relocation);
      } /* else */

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /* static bfd_reloc_status_type pic30_coff_perform_dmaoffset(...) */

/*****************************************************************************
 *
 * Special function for ensuring that the symbol being relocated is located
 * in either a CODE or ABSOLUTE section.
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  If the address is not in either a code or the absolute section,
 *             generate an error message, and return DANGEROUS; otherwise,
 *             call generic special function to finish the howto processing.
 *             Undefined weak symbols are acceptable; the generic special
 *             function will relocate them with a value of zero.
 *
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_program_address (abfd, reloc_entry, symbol, data,
                                    input_section, output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry ATTRIBUTE_UNUSED;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd;
   char ** error_msg;
{
   bfd_reloc_status_type rc;

   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       rc = bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0))
         rc = bfd_reloc_undefined;
      else if (((symbol->section->flags & SEC_CODE) == 0) &&
	       (symbol->section->auxflash == 0) &&
               (!bfd_is_abs_section (symbol->section) &&
                !bfd_is_und_section (symbol->section)))
      {
         *error_msg = (char *) malloc(BUFSIZ);
         sprintf(*error_msg,
                 "Symbol (%s) is not located in an executable section.",
                 symbol->name);
         rc = bfd_reloc_dangerous;
      } /* if ((symbol->section->flags & SEC_CODE) == 0) &&
	       (symbol->section->auxflash == 0) &&
               (!bfd_is_abs_section (symbol->section) &&
               !bfd_is_und_section (symbol->section))) */
      else
        /* finish by calling the generic special function */
        rc = pic30_elf32_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);
   } /* else */

   return rc;
}
/*****************************************************************************
 *
 * Special function for processing PC-relative relocations.
 *
 * Only want to perform the special function if in the linker.
 *
 * ASSEMBLER:  Return OK as the target specific assembler functions have
 *             already filled the bits with the required information.
 *
 *    LINKER:  If there is an offset in the instruction, convert it into
 *             a relocation addend and clear the instruction field.
 *             Add 4 to the relocation entry address.  PC-relative offsets
 *             are calculated from the next instruction word.  Also check to
 *             ensure that the address being branched to is even.  If not,
 *             display an error message and return DANGEROUS; otherwise,
 *             call pic30_elf32_perform_generic() to finish the relocation.
 *
 * Note:  Unlike other offsets and lengths, the relocation address
 *        is provided in octets, not PC units.
 * 
 *****************************************************************************/
static bfd_reloc_status_type
pic30_elf32_perform_pc_relative (abfd, reloc_entry, symbol, data,
                                input_section, output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol ATTRIBUTE_UNUSED;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;
{
  reloc_howto_type * howto = reloc_entry->howto;
  bfd_reloc_status_type rc = bfd_reloc_continue;

   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       rc = bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      /* LINKER */
      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0) &&
          (howto->type != R_PIC30_PCREL_ACCESS)) /* for CodeGuard */
         rc = bfd_reloc_undefined;
      else
      {
        /* look for instruction offset */
        bfd_size_type octets = reloc_entry->address;
        short offset = pic30_elf32_extract_bytes (abfd, data, 2, octets,
                                                 TRUE, TRUE);

        if (howto->type == R_PIC30_PCREL_BRANCH_SLIT6) {
          offset = (offset >> 4) & 0x3f;
          if (offset){
            unsigned char *byte_data = data;
            // offset truncated in insn for full range, unpack
            if (offset >= 32)
               offset -= 64;
            reloc_entry->addend = offset * 2;
            /* zero out 6 bits starting at bit 4 */
            byte_data[octets+0] = byte_data[octets+0] & 0x0F;
            byte_data[octets+1] = byte_data[octets+1] & 0xFC;}
        }
        else if (offset) /* store as addend, clear instruction offset */
          {
            reloc_entry->addend = offset;
            pic30_elf32_insert_bytes (abfd, data, 2, octets, 0, TRUE, TRUE);
          }

        /* adjust reloc address by 1 pword (4 octets) to get offset right */
        reloc_entry->address += 4;

        /* finish by calling the even_address special function */
        rc = pic30_elf32_is_even_address (abfd, reloc_entry, symbol, data,
                                         input_section, output_bfd, error_msg);
      } /* else */
   } /* else */
   return rc;
}
/*****************************************************************************/

/*****************************************************************************
 * Utility routine for data relocations.
 *****************************************************************************/

/*****************************************************************************
 *
 * Resolve handle relocations by looking up the symbol in the handle hash
 * table and returning the (16 bit) jump table entry.
 *
 * Note: if address fits in 16 bits, just return it.
 *
 *****************************************************************************/
static bfd_vma
bfd_pic30_handle (abfd, addr, symbol)
     bfd *abfd;
     bfd_vma addr;
     asymbol *symbol;
{
  bfd_vma target;

#if 0
  printf("resolving handle to %s\t%x\n", symbol->name, addr);
#endif

  if ((addr == (addr & 0xFFFF)) ||  /* if address fits in 16 bits      */
       (!handles))                  /*  or no handle hash table exists */
    return addr;
  else
    {
      struct handle_hash_entry *handle;
      asection *sec;
      char *name;
      bfd_size_type offset;
      bfd_vma table;

      /* if symbol is local, construct a global name */
      if ((symbol->flags & BSF_LOCAL) == BSF_LOCAL)
        {
          name = (char *) bfd_malloc ( strlen(abfd->filename) +
                                       strlen(symbol->name) + 2);
          if (!name)
            {
              fprintf( stderr, 
                       "Link Error: not enough memory for handle symbol\n");
              abort();
            }
          strcpy (name, abfd->filename);
          strcat (name, ":");
          strcat (name, symbol->name);
        }
      else
        name = (char *) symbol->name;

      /* lookup the handle hash entry for this symbol */
#if 0
      printf("looking up hash for %s\n", name);
#endif
      handle = handle_hash_lookup (handles, name, 0, 0);
      if (!handle)
        {
          fprintf( stderr, "Link Error: can't access handle %s\n", name);
          abort();
        }

      /* get a fresh copy of the handle section */
      sec = bfd_get_section_by_name(handle_bfd, ".handle");
#if 0
      printf("%s\t%lx\t%x\t%s\t%lx\n", sec->name,
             (unsigned long) sec,
             (unsigned int) sec->output_offset,
             sec->output_section->name,
             sec->output_section->vma);
#endif

      /* get the base address of the jump table */
      table = sec->output_section->vma + sec->output_offset;

      /* calculate the offset of this entry     */
      offset = handle->index * 4;

      /* sanity check the result */
      target = table + offset;
      if (target != (target & 0xFFFF))
          fprintf( stderr, "Linker Script Error: section .handle "
                   "must be allocated low in program memory.\n");
      return (target);
        
    }
}
/*****************************************************************************
 *
 * Extracts COUNT bytes from DATA with ability to skip upper and phantom bytes.
 *
 *****************************************************************************/
static bfd_vma
pic30_elf32_extract_bytes (abfd, data, count, addr, skip_upper, skip_phantom)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   unsigned char skip_upper;
   unsigned char skip_phantom;
{
   unsigned char x;
   bfd_size_type octets = addr;
   bfd_vma target = 0;
   int i;

   for (i = 0; i < count; i++)
   {
      /* If address is upper byte, skip ahead if requested */
      if (skip_upper)
         if ((octets & 0x02) == 0x02)
            octets++;

      /* If address is phantom byte, skip ahead if requested */
      if (skip_phantom)
         if ((octets & 0x03) == 0x03)
            octets++;

      /* Read a byte */
      x = bfd_get_8 (abfd, (char *) data + octets);

      /* Accumulate it */
      target = (target | (bfd_vma) (x << (8 * i)));
      octets++;
   } /* for i */

   return target;
}
/*****************************************************************************/

/*****************************************************************************
 *
 * Extract COUNT bytes from a data memory section. Skips phantom bytes.
 *
 *****************************************************************************/
static bfd_vma
pic30_elf32_extract_bytes_data_mem (abfd, data, count, addr)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
{
   unsigned char x;
   bfd_size_type octets = addr;
   bfd_vma target = 0;
   int i;

   for (i = 0; i < count; i++)
   {
      /* If address is phantom byte, skip ahead */
      if ((octets & 0x01) == 0x01)
        octets++;

      /* Read a byte */
      x = bfd_get_8 (abfd, (char *) data + octets);

      /* Accumulate it */
      target = (target | (bfd_vma) (x << (8 * i)));
      octets++;
   } /* for i */

   return target;
}
/*****************************************************************************
 *
 * Inserts COUNT bytes of VALUE into DATA with ability to skip upper and
 * phantom bytes.
 *
 *****************************************************************************/
static void
pic30_elf32_insert_bytes (abfd, data, count, addr, value,
                        skip_upper, skip_phantom)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   bfd_vma value;
   unsigned char skip_upper;
   unsigned char skip_phantom;
{
   unsigned char x;
   bfd_size_type octets = addr;
   int i;

   for (i = 0; i < count; i++)
   {
      /* If address is upper byte, skip ahead if requested */
      if (skip_upper)
         if ((octets & 0x02) == 0x02)
            octets++;

      /* If address is phantom byte, skip ahead if requested */
      if (skip_phantom)
         if ((octets & 0x03) == 0x03)
             octets++;

      /* Select a byte in the value */
      x = (value >> (i * 8) & 0xFF);

      /* Insert it */
      bfd_put_8 (abfd, x, (unsigned char *) data + octets);
      octets++;
   } /* for i */
}
/*****************************************************************************
 *
 * Insert COUNT bytes of VALUE into a data memory section. Skips phantom bytes.
 *
 *****************************************************************************/
static void
pic30_elf32_insert_bytes_data_mem (abfd, data, count, addr, value)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   bfd_vma value;
{
   unsigned char x;
   bfd_size_type octets = addr;
   int i;

   for (i = 0; i < count; i++)
   {
      if ((octets & 0x01) == 0x01)
          octets++;

      /* Select a byte in the value */
      x = (value >> (i * 8) & 0xFF);

      /* Insert it */
      bfd_put_8 (abfd, x, (unsigned char *) data + octets);
      octets++;
   } /* for i */
}
/*****************************************************************************/

/* Return the section that should be marked against GC for a given
   relocation.  */

asection *
_bfd_pic30_elf_gc_mark_hook (sec, info, rel, h, sym)
     asection *sec;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
     Elf_Internal_Rela *rel;
     struct elf_link_hash_entry *h;
     Elf_Internal_Sym *sym;
{
  /* ??? Do mips16 stub sections need to be handled special?  */

  if (h != NULL)
    {
#if 0
      switch (ELF_R_TYPE (sec->owner, rel->r_info))
	{
  
	case R_MIPS_GNU_VTINHERIT:
	case R_MIPS_GNU_VTENTRY:
	  break;


	default:
#endif
	  switch (h->root.type)
	    {
	    case bfd_link_hash_defined:
	    case bfd_link_hash_defweak:
	      return h->root.u.def.section;

	    case bfd_link_hash_common:
	      return h->root.u.c.p->section;

	    default:
	      break;
	    }
	  //}
    }
  else
    return bfd_section_from_elf_index (sec->owner, sym->st_shndx);

  return NULL;
}

/*****************************************************************************/
/* Update the got entry reference counts for the section being removed.  */

bfd_boolean
_bfd_pic30_elf_gc_sweep_hook (abfd, info, sec, relocs)
     bfd *abfd ATTRIBUTE_UNUSED;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
     asection *sec ATTRIBUTE_UNUSED;
     const Elf_Internal_Rela *relocs ATTRIBUTE_UNUSED;
{
#if 0
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  bfd_signed_vma *local_got_refcounts;
  const Elf_Internal_Rela *rel, *relend;
  unsigned long r_symndx;
  struct elf_link_hash_entry *h;

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (abfd);
  local_got_refcounts = elf_local_got_refcounts (abfd);

  relend = relocs + sec->reloc_count;
  for (rel = relocs; rel < relend; rel++)
    switch (ELF_R_TYPE (abfd, rel->r_info))
      {
      case R_MIPS_GOT16:
      case R_MIPS_CALL16:
      case R_MIPS_CALL_HI16:
      case R_MIPS_CALL_LO16:
      case R_MIPS_GOT_HI16:
      case R_MIPS_GOT_LO16:
      case R_MIPS_GOT_DISP:
      case R_MIPS_GOT_PAGE:
      case R_MIPS_GOT_OFST:
	/* ??? It would seem that the existing MIPS code does no sort
	   of reference counting or whatnot on its GOT and PLT entries,
	   so it is not possible to garbage collect them at this time.  */
	break;

      default:
	break;
      }
#endif

  return TRUE;
}
/*****************************************************************************/
