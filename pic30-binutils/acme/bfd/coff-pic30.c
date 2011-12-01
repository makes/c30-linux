/*
 * coff-pic30.c -- BFD back-end for Microchip PIC30 COFF binaries.
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 * Modified by John Elliott (john.elliott@microchip.com)
 *
 * This file is part of BFD, the Binary File Descriptor library.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "coff/pic30.h"
#include "coff/internal.h"
#include "libcoff.h"
#include <coff-pic30.h>
#include "pic30-utils.h"

#define PIC30_DEBUG 0

/* defined in bfd/cpu-pic30.c */
extern int pic30_is_generic_machine(unsigned int machine);
extern int pic30_is_dma_machine(const bfd_arch_info_type *);
extern int pic30_is_eds_machine(const bfd_arch_info_type *);
extern int pic30_proc_family(const bfd_arch_info_type *);
extern const bfd_arch_info_type * global_PROCESSOR;

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

extern struct bfd_hash_entry *pic30_undefsym_newfunc
  PARAMS ((struct bfd_hash_entry *, struct bfd_hash_table *, const char *));

extern void pic30_undefsym_traverse
  PARAMS ((struct pic30_undefsym_table *,
           bfd_boolean (*func) PARAMS ((struct bfd_hash_entry *, PTR)), PTR));

extern struct pic30_undefsym_table *pic30_undefsym_init
  PARAMS ((void));

extern void pic30_rtype2howto
  PARAMS ((arelent *, struct internal_reloc *));

static void bfd_pic30_print_section_header
  PARAMS ((void));

extern bfd_reloc_status_type pic30_bfd_reloc_range_check
  PARAMS ((reloc_howto_type *, bfd_vma, bfd *, asymbol *, char **));

static void bfd_pic30_report_sections
  PARAMS ((bfd *, asection *, PTR));

static void bfd_pic30_clean_section_names
  PARAMS ((bfd *, asection *, PTR));

static void bfd_pic30_write_data_header
  PARAMS ((unsigned char **,  bfd_vma,  bfd_vma, int));

static void bfd_pic30_process_data_section
  PARAMS ((asection *, PTR));

static bfd_boolean pic30_final_link
  PARAMS ( (bfd *, struct bfd_link_info *));

static bfd_boolean pic30_set_section_contents
  PARAMS ((bfd *, sec_ptr, PTR, file_ptr, bfd_size_type));

#if 0
extern long pic30_attribute_map
  PARAMS ((asection *));

extern int pic30_set_implied_attributes
  PARAMS ((asection *));
#endif

static bfd_boolean pic30_styp_to_sec_flags
  PARAMS ((bfd *, PTR, const char *, asection *, flagword *));

static long pic30_sec_to_styp_flags
  PARAMS ((const char *, flagword));

static PTR pic30_mkobject_hook
  PARAMS ((bfd *, PTR, PTR));

static reloc_howto_type * pic30_bfd_reloc_type_lookup
  PARAMS ((bfd *, bfd_reloc_code_real_type));

static bfd_reloc_status_type pic30_coff_is_even_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_is_file_reg_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_program_address
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_pc_relative
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_vma pic30_coff_extract_bytes
   PARAMS ((bfd *, PTR, int, bfd_size_type, unsigned char, unsigned char));

static void pic30_coff_insert_bytes PARAMS ((bfd *, PTR, int,
   bfd_size_type, bfd_vma, unsigned char, unsigned char));

static bfd_vma pic30_coff_extract_bytes_data_mem
   PARAMS ((bfd *, PTR, int, bfd_size_type));

static void pic30_coff_insert_bytes_data_mem
   PARAMS ((bfd *, PTR, int, bfd_size_type, bfd_vma));

static bfd_reloc_status_type pic30_coff_perform_data_directive
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_p_data_directive
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_vma bfd_pic30_handle
     PARAMS ((bfd *, bfd_vma, asymbol *));

static bfd_reloc_status_type pic30_coff_perform_operators
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_p_operators
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_dmaoffset_dmapage
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static bfd_reloc_status_type pic30_coff_perform_generic
   PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

/* local statics */
static bfd *link_output_bfd;

#define RELOC_SPECIAL_FN_EVEN_ADDRESS pic30_coff_is_even_address
#define RELOC_SPECIAL_FN_FILE_REG_ADDRESS pic30_coff_is_file_reg_address
#define RELOC_SPECIAL_FN_PGM_ADDR pic30_coff_perform_program_address
#define RELOC_SPECIAL_FN_PCREL pic30_coff_perform_pc_relative
#define RELOC_SPECIAL_FN_DATA_DIRECTIVE pic30_coff_perform_data_directive
#define RELOC_SPECIAL_FN_P_DATA_DIRECTIVE pic30_coff_perform_p_data_directive
#define RELOC_SPECIAL_FN_OPERATORS pic30_coff_perform_operators
#define RELOC_SPECIAL_FN_P_OPERATORS pic30_coff_perform_p_operators
#define RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE pic30_coff_perform_dmaoffset_dmapage
#define RELOC_SPECIAL_FN_GENERIC pic30_coff_perform_generic

#define COFF_DEFAULT_SECTION_ALIGNMENT_POWER (1)
#define	COFF_LONG_FILENAMES 1
#define	COFF_LONG_SECTION_NAMES 1

/* PIC30 COFF encodes the section alignment in the section header flags */
#define COFF_ALIGN_IN_SECTION_HEADER 1
#define COFF_ALIGN_IN_S_FLAGS 1
#define COFF_ENCODE_ALIGNMENT(S,X) ((S).s_flags |= (((unsigned)(X)&0xF)<<28))
#define COFF_DECODE_ALIGNMENT(X) (((X)>>28)&0xF)


reloc_howto_type pic30_coff_howto_table[] =
{
/* type 
 * rightshift - Value the final relocation is shifted right by
 * size (0 = byte, 1 = short, 2 = long)
 * bitsize - Number of bits in the item to be relocated.
 * pc_relative - Boolean specifying if the relocation is relative to the
 *               location
 * bitpos - Relocated value is left shifted by this much
 * complain_on_overflow - Type of overflow error that should be checked for
 * special_function - This function is called when not NULL.
 * name - textual name
 * partial_inplace - When performing partial link, do you modify the
 *                   relocation instead of the data.
 * src_mask - Which parts of the read in data are to be used in the
 *            relocation sum.
 * dst_mask - Selects which parts of the instruction are replaced into
 *            the instruction.
 * pcrel_offset - Signals whether the value of the pc is left in the slot of
 *                the instruction.
 */
   HOWTO(R_PIC30_FILE_REG_BYTE, 0, 1, 13, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_FILE_REG_ADDRESS, "FILE REG - BYTE",
         TRUE, 0x01fff, 0x001fff, FALSE),
   HOWTO(R_PIC30_FILE_REG, 0, 1, 13, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_FILE_REG_ADDRESS, "FILE REG",
         TRUE, 0x01fff, 0x001fff, FALSE),
   HOWTO(R_PIC30_FILE_REG_WORD, 0, 1, 16, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_EVEN_ADDRESS, "FILE REG - WORD",
         TRUE, 0x0ffff, 0x00ffff, FALSE),
   HOWTO(R_PIC30_FILE_REG_WORD_WITH_DST, 1, 2, 15, FALSE, 4,
         complain_overflow_dont,
         RELOC_SPECIAL_FN_EVEN_ADDRESS, "FILE REG - WORD with DST.",
         TRUE, 0x07fff0, 0x07fff0, FALSE),
   HOWTO(R_PIC30_UNSIGNED_4, 0, 0, 4, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 4",
         TRUE, 0x000f, 0x00000f, FALSE),
   HOWTO(R_PIC30_UNSIGNED_5, 0, 0, 5, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 5",
         TRUE, 0x001f, 0x00001f, FALSE),
   HOWTO(R_PIC30_PCREL_DO, 1, 1, 16, TRUE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE DO",
         TRUE, 0xffff, 0x0000ffff, TRUE),
   HOWTO(R_PIC30_DO_ABSOLUTE, 1, 1, 16, TRUE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_PCREL, "DO ABSOLUTE",
         TRUE, 0xffff, 0x0000ffff, TRUE),
   HOWTO(R_PIC30_SIGNED_10_BYTE, 0, 1, 10, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_GENERIC, "SIGNED 10 (byte)",
         TRUE, 0x000ff0, 0x000ff0, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10, 0, 1, 10, FALSE, 4, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 10",
         TRUE, 0x003ff0, 0x003ff0, FALSE),
   HOWTO(R_PIC30_UNSIGNED_14, 0, 1, 14, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 14",
         TRUE, 0x003fff, 0x003fff, FALSE),
   HOWTO(R_PIC30_FRAME_SIZE, 0, 1, 14, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_EVEN_ADDRESS, "FRAME SIZE",
         TRUE, 0x003fff, 0x003fff, FALSE),
   HOWTO(R_PIC30_WORD, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_GENERIC, "WORD",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_TBLOFFSET, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - TBLOFFSET",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_PSVOFFSET, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - PSVOFFSET",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_TBLPAGE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - TBLPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_PSVPAGE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - PSVPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_HANDLE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - HANDLE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_PCREL_BRANCH, 1, 1, 16, TRUE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE BRANCH",
         TRUE, 0xffff, 0x00ffff, TRUE),
   HOWTO(R_PIC30_BRANCH_ABSOLUTE, 1, 1, 16, TRUE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_PCREL, "BRANCH ABSOLUTE",
         TRUE, 0xffff, 0x00ffff, TRUE),
   HOWTO(R_PIC30_PGM_ADDR_LSB, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_PGM_ADDR, "PROGRAM ADDRESS LSB",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_PGM_ADDR_MSB, 16, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_PGM_ADDR, "PROGRAM ADDRESS MSB",
         TRUE, 0xffff, 0xffff, FALSE),
   HOWTO(R_PIC30_TBLOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "TBLOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_PSVOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "PSVOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_TBLPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "TBLPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_PSVPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "PSVPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_HANDLE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "HANDLE (pointer to function)",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   /* This relocation can only be created via a .long PADDR(foo) statement */
   HOWTO(R_PIC30_PADDR, 0, 2, 32, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "PADDR",
         TRUE, 0xffffffff, 0x00ffffff, FALSE),
   HOWTO(R_PIC30_P_TBLOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-TBLOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_PSVOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-PSVOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_TBLPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-TBLPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_PSVPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-PSVPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_HANDLE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-HANDLE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   /* This relocation can only be created via a .pword PADDR(foo) statement */
   HOWTO(R_PIC30_P_PADDR, 0, 2, 24, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-PADDR",
         TRUE, 0x00ffffff, 0x00ffffff, FALSE),
   HOWTO(R_PIC30_BIT_SELECT_3, 0, 1, 3, FALSE, 13, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "BIT SELECT 3",
         TRUE, 0x00e000, 0x00e000, FALSE),
   HOWTO(R_PIC30_BIT_SELECT_4_BYTE, 0, 1, 3, FALSE, 12,
         complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "BIT SELECT 4 (byte)",
         TRUE, 0x007000, 0x007000, FALSE),
   HOWTO(R_PIC30_BIT_SELECT_4, 0, 1, 4, FALSE, 12, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "BIT SELECT 4",
         TRUE, 0x00f000, 0x00f000, FALSE),
   HOWTO(R_PIC30_DSP_6, 0, 0, 6, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_GENERIC, "DSP SIGNED 6",
         TRUE, 0x003f, 0x00003f, FALSE),
   HOWTO(R_PIC30_DSP_PRESHIFT, 0, 1, 4, FALSE, 7, complain_overflow_signed,
         RELOC_SPECIAL_FN_GENERIC, "DSP PRESHIFT",
         TRUE, 0x000780, 0x000780, FALSE),
   HOWTO(R_PIC30_PWRSAV_MODE, 0, 0, 1, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "POWER SAVE MODE",
         TRUE, 0x000001, 0x000001, FALSE),
   HOWTO(R_PIC30_PBYTE, 0, 0, 8, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_DATA_DIRECTIVE, ".pbyte",
         TRUE, 0x000000ff, 0x000000ff, FALSE),
   HOWTO(R_PIC30_PWORD, 0, 2, 24, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_DATA_DIRECTIVE, ".pword",
         TRUE, 0x00ffffff, 0x00ffffff, FALSE),
   HOWTO(R_RELBYTE, 0,  0,   8,  FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DATA_DIRECTIVE, ".byte",
         TRUE, 0x000000ff,0x000000ff, FALSE),
   HOWTO(R_RELWORD, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DATA_DIRECTIVE, ".word",
         TRUE, 0x0000ffff,0x0000ffff, FALSE),
   HOWTO(R_RELLONG, 0, 2, 32, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_DATA_DIRECTIVE, ".long",
         TRUE, 0xffffffff,0xffffffff, FALSE),
   HOWTO(R_PIC30_DMAOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "DMAOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_DMAOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "P-DMAOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_WORD_DMAOFFSET, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "WORD - DMAOFFSET",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_PSVPTR, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "PSVPTR",
         TRUE, 0x00ffffff, 0x00ffffff, FALSE),
   HOWTO(R_PIC30_P_PSVPTR, 0, 2, 24, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-PSVPTR",
         TRUE, 0x00ffffff, 0x00ffffff, FALSE),
   HOWTO(R_PIC30_L_PSVPTR, 0, 2, 32, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "L-PSVPTR",
         TRUE, 0xffffffff, 0xffffffff, FALSE),
   HOWTO(R_PIC30_WORD_PSVPTR, 0, 1, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - PSVPTR",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_CALL_ACCESS, 0, 2, 32, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_GENERIC, "CALL/GOTO ACCESS",
         TRUE, 0xffffffff, 0xffffffff, FALSE),
   HOWTO(R_PIC30_PCREL_ACCESS, 1, 1, 16, TRUE, 0, complain_overflow_signed,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE BRANCH - ACCESS",
         TRUE, 0xffff, 0x00ffff, TRUE),
   HOWTO(R_PIC30_ACCESS, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "ACCESS",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_ACCESS, 0, 2, 24, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-ACCESS",
         TRUE, 0x00ffffff, 0x00ffffff, FALSE),
   HOWTO(R_PIC30_L_ACCESS, 0, 2, 32, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "L-ACCESS",
         TRUE, 0xffffffff, 0xffffffff, FALSE),
   HOWTO(R_PIC30_WORD_ACCESS, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - ACCESS",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_EDSPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "EDSPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_EDSPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-EDSPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_WORD_EDSPAGE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - EDSPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_EDSOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "EDSOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_EDSOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-EDSOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_WORD_EDSOFFSET, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - EDSOFFSET",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
#ifdef BFD64
   HOWTO(R_PIC30_QUAD, 0, 4, 64, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_GENERIC, ".quad",
         TRUE, 0xffffffffffffffff,0xffffffffffffffff, FALSE),
#endif
   HOWTO(R_PIC30_UNSIGNED_8, 0, 1, 10, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 8",
         TRUE, 0x0000ff, 0x00039f, FALSE),
   HOWTO(R_PIC30_UNSIGNED_15, 0, 1, 15, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 15",
         TRUE, 0x007fff, 0x007fff, FALSE),
   HOWTO(R_PIC30_NO_SHIFT_UNSIGNED_10, 0, 1, 10, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "NO SHIFT UNSIGNED 10",
         TRUE, 0x0003ff, 0x0003ff, FALSE),

   HOWTO(R_PIC30_PCREL_BRANCH_SLIT6, 1, 1, 6, TRUE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE BRANCH SLIT6",
         TRUE, 0x003f0, 0x0003f0, TRUE),
   HOWTO(R_PIC30_DMAPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "DMAPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_DMAPAGE, 0, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "P-DMAPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_WORD_DMAPAGE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "WORD - DMAPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_TBLOFFSET, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - TBLOFFSET",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_PSVOFFSET, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - PSVOFFSET",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_TBLPAGE, 0, 1, 10, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - TBLPAGE",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_PSVPAGE, 0, 1, 10, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - PSVPAGE",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_HANDLE, 0, 1, 10, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - HANDLE",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_DMAPAGE, 0, 1, 10, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "UNSIGNED 10 - DMAPAGE",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_DMAOFFSET, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "UNSIGNED 10 - DMAOFFSET",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_EDSPAGE, 0, 1, 10, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - EDSPAGE",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_EDSOFFSET, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - EDSOFFSET",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
   HOWTO(R_PIC30_UNSIGNED_10_PSVPTR, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - PSVPTR",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
 HOWTO(R_PIC30_UNSIGNED_10_ACCESS, 0, 1, 10, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_OPERATORS, "UNSIGNED 10 - ACCESS",
         TRUE, 0x0003ff, 0x0003ff, FALSE),
};

/******************************************************************************/

int
pic30_adjustable_against_section (type)
   unsigned short type;
{
   int rc = 1;

   switch (type)
   {
      case R_PIC30_PCREL_BRANCH_SLIT6:
      case R_PIC30_PCREL_BRANCH:
      case R_PIC30_PCREL_DO:
      case R_PIC30_BRANCH_ABSOLUTE:
      case R_PIC30_DO_ABSOLUTE:
      case R_PIC30_PGM_ADDR_MSB:
      case R_PIC30_PGM_ADDR_LSB:
      case R_PIC30_TBLOFFSET:
      case R_PIC30_PSVOFFSET:
      case R_PIC30_TBLPAGE:
      case R_PIC30_PSVPAGE:
      case R_PIC30_HANDLE:
      case R_PIC30_PADDR:
      case R_PIC30_P_TBLOFFSET:
      case R_PIC30_P_PSVOFFSET:
      case R_PIC30_P_TBLPAGE:
      case R_PIC30_P_PSVPAGE:
      case R_PIC30_P_HANDLE:
      case R_PIC30_P_PADDR:
      case R_PIC30_WORD_TBLOFFSET:
      case R_PIC30_WORD_PSVOFFSET:
      case R_PIC30_WORD_PSVPAGE:
      case R_PIC30_WORD_TBLPAGE:
      case R_PIC30_WORD_HANDLE:
      case R_PIC30_WORD:
      case R_PIC30_FILE_REG_WORD:
      case R_PIC30_FILE_REG_WORD_WITH_DST:
      case R_PIC30_PBYTE:
      case R_PIC30_PWORD:
      case R_RELBYTE:
      case R_RELWORD:
      case R_RELLONG:
      case R_PIC30_DMAPAGE:
      case R_PIC30_P_DMAPAGE:
      case R_PIC30_WORD_DMAPAGE:
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
      case R_PIC30_EDSPAGE:
      case R_PIC30_P_EDSPAGE:
      case R_PIC30_WORD_EDSPAGE:
      case R_PIC30_EDSOFFSET:
      case R_PIC30_P_EDSOFFSET:
      case R_PIC30_WORD_EDSOFFSET:
      case R_PIC30_NO_SHIFT_UNSIGNED_10:
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
   } /* switch (type) */

   return rc;
} /* int pic30_adjustable_against_section(unsigned short) */

/******************************************************************************/


/* Code to turn an r_type into a howto ptr, uses the above howto table */

void pic30_rtype2howto (internal, dst)
   arelent * internal;
   struct internal_reloc * dst;
{
   switch (dst->r_type)
   {
      case R_PIC30_FILE_REG_BYTE:
         internal->howto = pic30_coff_howto_table + 0;
         break;

      case R_PIC30_FILE_REG:
         internal->howto = pic30_coff_howto_table + 1;
         break;

      case R_PIC30_FILE_REG_WORD:
         internal->howto = pic30_coff_howto_table + 2;
         break;

      case R_PIC30_FILE_REG_WORD_WITH_DST:
         internal->howto = pic30_coff_howto_table + 3;
         break;

      case R_PIC30_UNSIGNED_4:
         internal->howto = pic30_coff_howto_table + 4;
         break;

      case R_PIC30_UNSIGNED_5:
         internal->howto = pic30_coff_howto_table + 5;
         break;

      case R_PIC30_PCREL_DO:
         internal->howto = pic30_coff_howto_table + 6;
         break;

      case R_PIC30_DO_ABSOLUTE:
         internal->howto = pic30_coff_howto_table + 7;
         break;

      case R_PIC30_SIGNED_10_BYTE:
         internal->howto = pic30_coff_howto_table + 8;
         break;

      case R_PIC30_UNSIGNED_10:
         internal->howto = pic30_coff_howto_table + 9;
         break;

      case R_PIC30_UNSIGNED_14:
         internal->howto = pic30_coff_howto_table + 10;
         break;

      case R_PIC30_FRAME_SIZE:
         internal->howto = pic30_coff_howto_table + 11;
         break;

      case R_PIC30_WORD:
         internal->howto = pic30_coff_howto_table + 12;
         break;

      case R_PIC30_WORD_TBLOFFSET:
         internal->howto = pic30_coff_howto_table + 13;
         break;

      case R_PIC30_WORD_PSVOFFSET:
         internal->howto = pic30_coff_howto_table + 14;
         break;

      case R_PIC30_WORD_TBLPAGE:
         internal->howto = pic30_coff_howto_table + 15;
         break;

      case R_PIC30_WORD_PSVPAGE:
         internal->howto = pic30_coff_howto_table + 16;
         break;

      case R_PIC30_WORD_HANDLE:
         internal->howto = pic30_coff_howto_table + 17;
         break;

      case R_PIC30_PCREL_BRANCH:
         internal->howto = pic30_coff_howto_table + 18;
         break;

      case R_PIC30_BRANCH_ABSOLUTE:
         internal->howto = pic30_coff_howto_table + 19;
         break;

      case R_PIC30_PGM_ADDR_LSB:
         internal->howto = pic30_coff_howto_table + 20;
         break;

      case R_PIC30_PGM_ADDR_MSB:
         internal->howto = pic30_coff_howto_table + 21;
         break;

      case R_PIC30_TBLOFFSET:
         internal->howto = pic30_coff_howto_table + 22;
         break;

      case R_PIC30_PSVOFFSET:
         internal->howto = pic30_coff_howto_table + 23;
         break;

      case R_PIC30_TBLPAGE:
         internal->howto = pic30_coff_howto_table + 24;
         break;

      case R_PIC30_PSVPAGE:
         internal->howto = pic30_coff_howto_table + 25;
         break;

      case R_PIC30_HANDLE:
         internal->howto = pic30_coff_howto_table + 26;
         break;

      case R_PIC30_PADDR:
         internal->howto = pic30_coff_howto_table + 27;
         break;

      case R_PIC30_P_TBLOFFSET:
         internal->howto = pic30_coff_howto_table + 28;
         break;

      case R_PIC30_P_PSVOFFSET:
         internal->howto = pic30_coff_howto_table + 29;
         break;

      case R_PIC30_P_TBLPAGE:
         internal->howto = pic30_coff_howto_table + 30;
         break;

      case R_PIC30_P_PSVPAGE:
         internal->howto = pic30_coff_howto_table + 31;
         break;

      case R_PIC30_P_HANDLE:
         internal->howto = pic30_coff_howto_table + 32;
         break;

      case R_PIC30_P_PADDR:
         internal->howto = pic30_coff_howto_table + 33;
         break;

      case R_PIC30_BIT_SELECT_3:
         internal->howto = pic30_coff_howto_table + 34;
         break;

      case R_PIC30_BIT_SELECT_4_BYTE:
         internal->howto = pic30_coff_howto_table + 35;
         break;

      case R_PIC30_BIT_SELECT_4:
         internal->howto = pic30_coff_howto_table + 36;
         break;

      case R_PIC30_DSP_6:
         internal->howto = pic30_coff_howto_table + 37;
         break;

      case R_PIC30_DSP_PRESHIFT:
         internal->howto = pic30_coff_howto_table + 38;
         break;

      case R_PIC30_PWRSAV_MODE:
         internal->howto = pic30_coff_howto_table + 39;
         break;

      case R_PIC30_PBYTE:
         internal->howto = pic30_coff_howto_table + 40;
         break;

      case R_PIC30_PWORD:
         internal->howto = pic30_coff_howto_table + 41;
         break;

      case R_RELBYTE:
         internal->howto = pic30_coff_howto_table + 42;
         break;

      case R_RELWORD:
         internal->howto = pic30_coff_howto_table + 43;
         break;

      case R_RELLONG:
         internal->howto = pic30_coff_howto_table + 44;
         break;

      case R_PIC30_DMAOFFSET:
         internal->howto = pic30_coff_howto_table + 45;
         break;

      case R_PIC30_P_DMAOFFSET:
         internal->howto = pic30_coff_howto_table + 46;
         break;

      case R_PIC30_WORD_DMAOFFSET:
         internal->howto = pic30_coff_howto_table + 47;
         break;

      case R_PIC30_PSVPTR:
         internal->howto = pic30_coff_howto_table + 48;
         break;

      case R_PIC30_P_PSVPTR:
         internal->howto = pic30_coff_howto_table + 49;
         break;

      case R_PIC30_L_PSVPTR:
         internal->howto = pic30_coff_howto_table + 50;
         break;

      case R_PIC30_WORD_PSVPTR:
         internal->howto = pic30_coff_howto_table + 51;
         break;

      case R_PIC30_CALL_ACCESS:
         internal->howto = pic30_coff_howto_table + 52;
         break;

      case R_PIC30_PCREL_ACCESS:
         internal->howto = pic30_coff_howto_table + 53;
         break;

      case R_PIC30_ACCESS:
         internal->howto = pic30_coff_howto_table + 54;
         break;

      case R_PIC30_P_ACCESS:
         internal->howto = pic30_coff_howto_table + 55;
         break;

      case R_PIC30_L_ACCESS:
         internal->howto = pic30_coff_howto_table + 56;
         break;

      case R_PIC30_WORD_ACCESS:
         internal->howto = pic30_coff_howto_table + 57;
         break;

      case R_PIC30_EDSPAGE:
         internal->howto = pic30_coff_howto_table + 58;
         break;

      case R_PIC30_P_EDSPAGE:
         internal->howto = pic30_coff_howto_table + 59;
         break;

      case R_PIC30_WORD_EDSPAGE:
         internal->howto = pic30_coff_howto_table + 60;
         break;

      case R_PIC30_EDSOFFSET:
         internal->howto = pic30_coff_howto_table + 61;
         break;

      case R_PIC30_P_EDSOFFSET:
         internal->howto = pic30_coff_howto_table + 62;
         break;

      case R_PIC30_WORD_EDSOFFSET:
         internal->howto = pic30_coff_howto_table + 63;
         break;

      case R_PIC30_UNSIGNED_8:
         internal->howto = pic30_coff_howto_table + 64;
         break;

      case R_PIC30_UNSIGNED_15:
         internal->howto = pic30_coff_howto_table + 65;
         break;

      case R_PIC30_NO_SHIFT_UNSIGNED_10:
         internal->howto = pic30_coff_howto_table + 66;
         break;

     case R_PIC30_PCREL_BRANCH_SLIT6:
         internal->howto = pic30_coff_howto_table + 67;
         break;
     
     case R_PIC30_DMAPAGE:
         internal->howto = pic30_coff_howto_table + 68;
         break;

      case R_PIC30_P_DMAPAGE:
         internal->howto = pic30_coff_howto_table + 69;
         break;

      case R_PIC30_WORD_DMAPAGE:
         internal->howto = pic30_coff_howto_table + 70;
         break;
     case R_PIC30_UNSIGNED_10_TBLOFFSET:
         internal->howto = pic30_coff_howto_table + 71;
         break;

     case R_PIC30_UNSIGNED_10_PSVOFFSET:
         internal->howto = pic30_coff_howto_table + 72;
         break;

      case R_PIC30_UNSIGNED_10_TBLPAGE:
         internal->howto = pic30_coff_howto_table + 73;
         break;

      case R_PIC30_UNSIGNED_10_PSVPAGE:
         internal->howto = pic30_coff_howto_table + 74;
         break;

      case R_PIC30_UNSIGNED_10_HANDLE:
         internal->howto = pic30_coff_howto_table + 75;
         break;
     case R_PIC30_UNSIGNED_10_DMAPAGE:
         internal->howto = pic30_coff_howto_table + 76;
         break;
     case R_PIC30_UNSIGNED_10_DMAOFFSET:
         internal->howto = pic30_coff_howto_table + 77;
         break;
     case R_PIC30_UNSIGNED_10_EDSPAGE:
         internal->howto = pic30_coff_howto_table + 78;
         break;
     case R_PIC30_UNSIGNED_10_EDSOFFSET:
         internal->howto = pic30_coff_howto_table + 79;
         break;
     case R_PIC30_UNSIGNED_10_PSVPTR:
         internal->howto = pic30_coff_howto_table + 80;
         break;
     case R_PIC30_UNSIGNED_10_ACCESS:
         internal->howto = pic30_coff_howto_table + 81;
         break;

      default:
         abort ();
         break;
   } /* switch (dst->r_type) */
} /* void pic30_rtype2howto(...) */

#define RTYPE2HOWTO(internal, relocentry) pic30_rtype2howto(internal,relocentry)

/* Turn a howto into a reloc number */
#define SELECT_RELOC(x,howto) x.r_type = howto->type

static reloc_howto_type * pic30_bfd_reloc_type_lookup (abfd, code)
   bfd * abfd ATTRIBUTE_UNUSED;
   bfd_reloc_code_real_type code;
{
   switch (code)
   {
      case BFD_RELOC_PIC30_FILE_REG_BYTE:
         return pic30_coff_howto_table + 0;
         break;

      case BFD_RELOC_PIC30_FILE_REG:
         return pic30_coff_howto_table + 1;
         break;

      case BFD_RELOC_PIC30_FILE_REG_WORD:
         return pic30_coff_howto_table + 2;
         break;

      case BFD_RELOC_PIC30_FILE_REG_WORD_WITH_DST:
         return pic30_coff_howto_table + 3;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_4:
         return pic30_coff_howto_table + 4;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_5:
         return pic30_coff_howto_table + 5;
         break;

      case BFD_RELOC_PIC30_PCREL_DO:
         return pic30_coff_howto_table + 6;
         break;

      case BFD_RELOC_PIC30_DO_ABSOLUTE:
         return pic30_coff_howto_table + 7;
         break;

      case BFD_RELOC_PIC30_SIGNED_10_BYTE:
         return pic30_coff_howto_table + 8;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_10:
         return pic30_coff_howto_table + 9;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_14:
         return pic30_coff_howto_table + 10;
         break;

      case BFD_RELOC_PIC30_FRAME_SIZE:
         return pic30_coff_howto_table + 11;
         break;

      case BFD_RELOC_PIC30_WORD:
         return pic30_coff_howto_table + 12;
         break;

      case BFD_RELOC_PIC30_WORD_TBLOFFSET:
         return pic30_coff_howto_table + 13;
         break;

      case BFD_RELOC_PIC30_WORD_PSVOFFSET:
         return pic30_coff_howto_table + 14;
         break;

      case BFD_RELOC_PIC30_WORD_TBLPAGE:
         return pic30_coff_howto_table + 15;
         break;

      case BFD_RELOC_PIC30_WORD_PSVPAGE:
         return pic30_coff_howto_table + 16;
         break;

      case BFD_RELOC_PIC30_WORD_HANDLE:
         return pic30_coff_howto_table + 17;
         break;

      case BFD_RELOC_PIC30_PCREL_BRANCH:
         return pic30_coff_howto_table + 18;
         break;

      case BFD_RELOC_PIC30_BRANCH_ABSOLUTE:
         return pic30_coff_howto_table + 19;
         break;

      case BFD_RELOC_PIC30_PGM_ADDR_LSB:
         return pic30_coff_howto_table + 20;
         break;

      case BFD_RELOC_PIC30_PGM_ADDR_MSB:
         return pic30_coff_howto_table + 21;
         break;

      case BFD_RELOC_PIC30_TBLOFFSET:
         return pic30_coff_howto_table + 22;
         break;

      case BFD_RELOC_PIC30_PSVOFFSET:
         return pic30_coff_howto_table + 23;
         break;

      case BFD_RELOC_PIC30_TBLPAGE:
         return pic30_coff_howto_table + 24;
         break;

      case BFD_RELOC_PIC30_PSVPAGE:
         return pic30_coff_howto_table + 25;
         break;

      case BFD_RELOC_PIC30_HANDLE:
         return pic30_coff_howto_table + 26;
         break;

      case BFD_RELOC_PIC30_PADDR:
         return pic30_coff_howto_table + 27;
         break;

      case BFD_RELOC_PIC30_P_TBLOFFSET:
         return pic30_coff_howto_table + 28;
         break;

      case BFD_RELOC_PIC30_P_PSVOFFSET:
         return pic30_coff_howto_table + 29;
         break;

      case BFD_RELOC_PIC30_P_TBLPAGE:
         return pic30_coff_howto_table + 30;
         break;

      case BFD_RELOC_PIC30_P_PSVPAGE:
         return pic30_coff_howto_table + 31;
         break;

      case BFD_RELOC_PIC30_P_HANDLE:
         return pic30_coff_howto_table + 32;
         break;

      case BFD_RELOC_PIC30_P_PADDR:
         return pic30_coff_howto_table + 33;
         break;

      case BFD_RELOC_PIC30_BIT_SELECT_3:
         return pic30_coff_howto_table + 34;
         break;

      case BFD_RELOC_PIC30_BIT_SELECT_4_BYTE:
         return pic30_coff_howto_table + 35;
         break;

      case BFD_RELOC_PIC30_BIT_SELECT_4:
         return pic30_coff_howto_table + 36;
         break;

      case BFD_RELOC_PIC30_DSP_6:
         return pic30_coff_howto_table + 37;
         break;

      case BFD_RELOC_PIC30_DSP_PRESHIFT:
         return pic30_coff_howto_table + 38;
         break;

      case BFD_RELOC_PIC30_PWRSAV_MODE:
         return pic30_coff_howto_table + 39;
         break;

      case BFD_RELOC_PIC30_PBYTE:
         return pic30_coff_howto_table + 40;
         break;

      case BFD_RELOC_PIC30_PWORD:
         return pic30_coff_howto_table + 41;
         break;

      case BFD_RELOC_8:
         return pic30_coff_howto_table + 42;
         break;

      case BFD_RELOC_16:
         return pic30_coff_howto_table + 43;
         break;

      case BFD_RELOC_32:
         return pic30_coff_howto_table + 44;
         break;

      case BFD_RELOC_PIC30_DMAOFFSET:
         return pic30_coff_howto_table + 45;
         break;

      case BFD_RELOC_PIC30_P_DMAOFFSET:
         return pic30_coff_howto_table + 46;
         break;

      case BFD_RELOC_PIC30_WORD_DMAOFFSET:
         return pic30_coff_howto_table + 47;
         break;

      case BFD_RELOC_PIC30_PSVPTR:
         return pic30_coff_howto_table + 48;
         break;

      case BFD_RELOC_PIC30_P_PSVPTR:
         return pic30_coff_howto_table + 49;
         break;

      case BFD_RELOC_PIC30_L_PSVPTR:
         return pic30_coff_howto_table + 50;
         break;

      case BFD_RELOC_PIC30_WORD_PSVPTR:
         return pic30_coff_howto_table + 51;
         break;

      case BFD_RELOC_PIC30_CALL_ACCESS:
         return pic30_coff_howto_table + 52;
         break;

      case BFD_RELOC_PIC30_PCREL_ACCESS:
         return pic30_coff_howto_table + 53;
         break;

      case BFD_RELOC_PIC30_ACCESS:
         return pic30_coff_howto_table + 54;
         break;

      case BFD_RELOC_PIC30_P_ACCESS:
         return pic30_coff_howto_table + 55;
         break;

      case BFD_RELOC_PIC30_L_ACCESS:
         return pic30_coff_howto_table + 56;
         break;

      case BFD_RELOC_PIC30_WORD_ACCESS:
         return pic30_coff_howto_table + 57;
         break;

      case BFD_RELOC_PIC30_EDSPAGE:
         return pic30_coff_howto_table + 58;
         break;

      case BFD_RELOC_PIC30_P_EDSPAGE:
         return pic30_coff_howto_table + 59;
         break;

      case BFD_RELOC_PIC30_WORD_EDSPAGE:
         return pic30_coff_howto_table + 60;
         break;

      case BFD_RELOC_PIC30_EDSOFFSET:
         return pic30_coff_howto_table + 61;
         break;

      case BFD_RELOC_PIC30_P_EDSOFFSET:
         return pic30_coff_howto_table + 62;
         break;

      case BFD_RELOC_PIC30_WORD_EDSOFFSET:
         return pic30_coff_howto_table + 63;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_8:
         return pic30_coff_howto_table + 64;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_15:
         return pic30_coff_howto_table + 65;
         break;

      case BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10:
         return pic30_coff_howto_table + 66;
         break;

     case BFD_RELOC_PIC30_PCREL_BRANCH_SLIT6:
         return pic30_coff_howto_table + 67;
         break;

     case BFD_RELOC_PIC30_DMAPAGE:
         return pic30_coff_howto_table + 68;
         break;

     case BFD_RELOC_PIC30_P_DMAPAGE:
         return pic30_coff_howto_table + 69;
         break;

     case BFD_RELOC_PIC30_WORD_DMAPAGE:
         return pic30_coff_howto_table + 70;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_TBLOFFSET:
         return pic30_coff_howto_table + 71;
         break;

     case BFD_RELOC_PIC30_UNSIGNED_10_PSVOFFSET:
         return pic30_coff_howto_table + 72;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_10_TBLPAGE:
         return pic30_coff_howto_table + 73;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_10_PSVPAGE:
         return pic30_coff_howto_table + 74;
         break;

      case BFD_RELOC_PIC30_UNSIGNED_10_HANDLE:
         return pic30_coff_howto_table + 75;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_DMAPAGE:
        return pic30_coff_howto_table + 76;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_DMAOFFSET:
        return pic30_coff_howto_table + 77;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_EDSPAGE:
         return pic30_coff_howto_table + 78;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_EDSOFFSET:
        return pic30_coff_howto_table + 79;
         break;
      case BFD_RELOC_PIC30_UNSIGNED_10_PSVPTR:
         return pic30_coff_howto_table + 80;
         break;
     case BFD_RELOC_PIC30_UNSIGNED_10_ACCESS:
         return pic30_coff_howto_table + 81;
         break;
      default:
         BFD_FAIL();

         return NULL;
         break;
   } /* switch (code) */
} /* static reloc_howto_type * pic30_bfd_reloc_type_lookup(...) */
#define coff_bfd_reloc_type_lookup pic30_bfd_reloc_type_lookup

#define BADMAG(x) PIC30BADMAG(x)

/* Compute the addend of a reloc.  If the reloc is to a common symbol,
   the object file contains the value of the common symbol.  By the
   time this is called, the linker may be using a different symbol
   from a different object file with a different value.  Therefore, we
   hack wildly to locate the original symbol from this file so that we
   can make the correct adjustment.  This macro sets coffsym to the
   symbol from the original file, and uses it to set the addend value
   correctly.  If this is not a common symbol, the usual addend
   calculation is done, except that an additional tweak is needed for
   PC relative relocs.
   FIXME: This macro refers to symbols and asect; these are from the
   calling function, not the macro arguments.  */
#define CALC_ADDEND(abfd, ptr, reloc, cache_ptr)                        \
  {                                                                     \
    coff_symbol_type * coffsym = (coff_symbol_type *) NULL;             \
                                                                        \
    if (ptr && bfd_asymbol_bfd (ptr) != abfd)                           \
      coffsym = (obj_symbols (abfd)                                     \
                 + (cache_ptr->sym_ptr_ptr - symbols));                 \
    else if (ptr)                                                       \
      coffsym = coff_symbol_from (abfd, ptr);                           \
                                                                        \
/* Don't do this for pic30. Negative addends based on symbol      */    \
/* size are not needed for common relocation processing.          */    \
/*     if (coffsym != (coff_symbol_type *) NULL                   */    \
/*         && coffsym->native->u.syment.n_scnum == 0)             */    \
/*       cache_ptr->addend = - coffsym->native->u.syment.n_value; */    \
                                                                        \
/* Don't do this for pic30. If the section was defined with an   */     \
/* address() attribute, a negative address is added in to the    */     \
/* relocation, resulting in corruption. This problem would occur */     \
/* for any relocation based on a section name. GSM 2/17/2005     */     \
/*     else if (ptr && bfd_asymbol_bfd (ptr) == abfd             */     \
/*              && (ptr->section != (asection *) NULL))          */     \
/*       cache_ptr->addend = - (ptr->section->vma + ptr->value); */     \
                                                                        \
/*     else                                                       */    \
         cache_ptr->addend = 0;                                         \
                                                                        \
/* With the above changes for pic30, this statement is redundant.*/     \
/*  if ((ptr) && (!pic30_adjustable_against_section(reloc.r_type))) */  \
/*    cache_ptr->addend = 0;                                     */     \
  }


/*
 * Over-ride some of the generic coff entry points
 *
 */
#define coff_bfd_final_link pic30_final_link
#define coff_set_section_contents pic30_set_section_contents
#define styp_to_sec_flags pic30_styp_to_sec_flags
#define sec_to_styp_flags pic30_sec_to_styp_flags
#define coff_mkobject_hook pic30_mkobject_hook
#include "coffcode.h"


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

static void
bfd_pic30_print_section_header()
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
  return;
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
  c = strchr(sect->name, '%');
  if (c) {
    if (pic30_debug)  printf("  %s",sect->name);
    *c = (char) NULL;
    if (pic30_debug)  printf(" --> %s\n",sect->name);
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
  *(*d)++ = pic30_fill_upper;; *(*d)++ = 0; /* skip upper and phantom bytes */

  /* write format code */
  *(*d)++ = (unsigned char) (format & 0xFF);
  *(*d)++ = (unsigned char) (format >> 8);
  *(*d)++ = pic30_fill_upper;; *(*d)++ = 0; /* skip upper and phantom bytes */
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
  if (PIC30_IS_BSS_ATTR(sect) &&
      (sect->_raw_size > 0))
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
#if 0
              if (pic30_debug) printf("%x", *p);
#endif
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

    } /* process DATA-type sections */

  return;
} /* static void bfd_pic30_process_data_section (...)*/


/*****************************************************************************/

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

  /* call the generic final link routine */
  if (pic30_debug)
    {
      printf("\nBefore generic final link:\n");
      bfd_pic30_print_section_header();
      bfd_map_over_sections(abfd, &bfd_pic30_report_sections, 0);
    }

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

      if (!dinit_sec) {
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
        if (s->sec)
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

  return result;
} /* static bfd_boolean pic30_final_link (...)*/


/*****************************************************************************/

/*
 *  This routine is cloned from coff_mkobject_hook() in coffcode.h
 *  and modifed to support the following pic30-specific need:
 *
 *  The magic number from the "optional aout" header, which
 *  represents the input file's processor ID, is used to populate
 *  abfd->arch_info. Therefore every input BFD identifies which
 *  processor it was created for.
 *
 *  Note that this ID may be different from the one in the linker's
 *  output BFD, which is set by command line option, or else contains
 *  a default value.
 *
 */
static PTR
pic30_mkobject_hook (abfd, filehdr, aouthdr)
     bfd * abfd;
     PTR filehdr;
     PTR aouthdr;
{
  struct internal_filehdr *internal_f = (struct internal_filehdr *) filehdr;
  struct internal_aouthdr *internal_a = (struct internal_aouthdr *) aouthdr;
  coff_data_type *coff;

  if (! coff_mkobject (abfd))
    return NULL;

  coff = coff_data (abfd);

  coff->sym_filepos = internal_f->f_symptr;

  /* These members communicate important constants about the symbol
     table to GDB's symbol-reading code.  These `constants'
     unfortunately vary among coff implementations...  */
  coff->local_n_btmask = N_BTMASK;
  coff->local_n_btshft = N_BTSHFT;
  coff->local_n_tmask = N_TMASK;
  coff->local_n_tshift = N_TSHIFT;
  coff->local_symesz = bfd_coff_symesz (abfd);
  coff->local_auxesz = bfd_coff_auxesz (abfd);
  coff->local_linesz = bfd_coff_linesz (abfd);

  coff->timestamp = internal_f->f_timdat;

  obj_raw_syment_count (abfd) =
    obj_conv_table_size (abfd) =
      internal_f->f_nsyms;

  /* pic30 specific */
  if (internal_a)
    abfd->arch_info = bfd_lookup_arch ( bfd_arch_pic30, internal_a->magic );

  return (PTR) coff;
}

/*****************************************************************************/

/*
 *  This routine is cloned from coff_set_section_contents() in coffcode.h
 *  and modifed to support the following pic30-specific need:
 *
 *     READONLY data sections are expanded as they are written
 *     so that the program memory image includes space for
 *     the upper byte and phantom byte. Note that the output
 *     section size is already correct -- phantom bytes in data
 *     memory have reserved space for the upper and phantom
 *     byte in program memory.
 *
 */
static bfd_boolean
pic30_set_section_contents (abfd, section, location, offset, count)
     bfd *abfd;
     sec_ptr section;
     PTR location;
     file_ptr offset;
     bfd_size_type count;
{
  PTR pic30_location = location;

  /* Reset the bfd file pointer, because
     there seems to be a bug with fseek()
     in Winblows that makes seeking to
     a position earlier in the file unreliable. */
     (void) bfd_seek(abfd, 0, SEEK_SET);
     
 /* If this is a linker pass on
     a READONLY, PSV, or EEDATA data section
     then expand the section to include padding for
     the extra width of program memory.

     Since data sections are always aligned we know
     the length is even.

     Because of PC addressing, the size of the output
     section is already correct. Basically we are
     swapping 2 phantom bytes in data memory for 2
     padding bytes in program memory. */
  if ((section->link_order_head != 0) &&
      (PIC30_IS_PSV_ATTR(section) | PIC30_IS_EEDATA_ATTR(section)))
    {
      /* expand data image for program memory storage */
      unsigned int j = 0;
      unsigned char *p1 = location;
      unsigned char *p2 = bfd_malloc(count);
      if (!p2) return FALSE;
      while (j < count)
        {
          /* copy two bytes of data, then two byte of padding */
          /* (convert data phantoms into program mem padding) */
          p2[j++] = *p1++; p1++;
          p2[j++] = *p1++; p1++;
          p2[j++] = pic30_fill_upper;
          p2[j++] = 0;
#if 0
          printf(" %lx", *((unsigned long *)(p2+j-4)));
#endif
        }
      pic30_location = p2;
      if (pic30_debug)
        printf("expanding %#x bytes into %s at %#lx\n",
               (unsigned int) count, section->name, offset);
    }

  if (abfd->output_has_begun == FALSE)	/* set by bfd.c handler */
    {
      if (! coff_compute_section_file_positions (abfd))
        {
          if (pic30_debug)
            printf("coff_compute_section_file_positions() failed!\n");
          return FALSE;
        }
    }

  /* Don't write out bss sections - one way to do this is to
       see if the filepos has not been set.  */
  if (section->filepos == 0)
    return TRUE;

  if (bfd_seek (abfd, (file_ptr) (section->filepos + offset), SEEK_SET) != 0)
    {
      if (pic30_debug)
        printf("bfd_seek() failed!\n");
      return FALSE;
    }

  if (count == 0)
    return TRUE;

  if (bfd_bwrite (pic30_location, count, abfd) != count)
    {
      if (pic30_debug)
        printf("bfd_bwrite() failed!\n");
      return FALSE;
    }
	  
  return TRUE;
}


/*****************************************************************************/

/*
 * Include some utilities and macro definitions
 * for processing section attributes.
 */
#include "pic30-attributes.c"


/*
 *  This routine is cloned from styp_to_sec_flags() in coffcode.h
 *  and modifed to support the pic30-specific section attributes.
 */
static bfd_boolean
pic30_styp_to_sec_flags (abfd, hdr, name, section, flags_ptr)
     bfd *abfd ATTRIBUTE_UNUSED;
     PTR hdr;
     const char *name ATTRIBUTE_UNUSED;
     asection *section;
     flagword *flags_ptr;
{
  struct internal_scnhdr *internal_s = (struct internal_scnhdr *) hdr;
  long styp_flags = internal_s->s_flags;
  flagword sec_flags = 0;

#if PIC30_DEBUG
  printf("pic30_styp_to_sec_flags: %s %lx --> ", name, styp_flags);
#endif

  if ((styp_flags & STYP_LIT) == STYP_LIT)
    {
      /* set the bfd section flags */
      sec_flags = (SEC_LOAD | SEC_ALLOC | SEC_READONLY);
      /* clear the styp flags to avoid confusion,
         since STYP_LIT includes STYP_TEXT */
      styp_flags &= ~STYP_LIT;
    }

  if (styp_flags & STYP_DSECT)
    {
	sec_flags |= SEC_DEBUGGING;

        if (styp_flags & STYP_TEXT)    /* special case of      */
          sec_flags |= SEC_CODE;       /* attributes code,info */
    }
  else if (styp_flags & STYP_TEXT)
    {
	sec_flags |= SEC_CODE | SEC_LOAD | SEC_ALLOC;
    }
  else if (styp_flags & STYP_DATA)
    {
	sec_flags |= SEC_DATA | SEC_LOAD | SEC_ALLOC;
    }
  else if (styp_flags & STYP_BSS)
    {
	sec_flags |= SEC_ALLOC;
    }
  else if (styp_flags & STYP_INFO)
    {
      /* do nothing */
    }
  else if (styp_flags & STYP_PAD)
    {
    sec_flags = 0;
    }
  else if (strcmp (name, _TEXT) == 0)
    {
	sec_flags |= SEC_CODE | SEC_LOAD | SEC_ALLOC;
    }
  else if (strcmp (name, _DATA) == 0)
    {
	sec_flags |= SEC_DATA | SEC_LOAD | SEC_ALLOC;
    }
  else if (strcmp (name, _BSS) == 0)
    {
	sec_flags |= SEC_ALLOC;
    }
  else if (strncmp (name, ".debug", 6) == 0
	   || strcmp (name, _COMMENT) == 0
	   || strncmp (name, ".stab", 5) == 0)
    {
      /* do nothing */
    }
  else
    {
         sec_flags |= SEC_ALLOC | SEC_LOAD;
    }

  /*
   * Copy the flagword (now partially built)
   * into the section for additional processing.
   */
  section->flags = sec_flags;

/*
** PIC30-specific section flags
*/
  if ((styp_flags & STYP_NEAR) == STYP_NEAR)
    { PIC30_SET_NEAR_ATTR(section) }

  if ((styp_flags & STYP_PERSIST) == STYP_PERSIST)
    { PIC30_SET_PERSIST_ATTR(section) }

  if ((styp_flags & STYP_XMEM) == STYP_XMEM)
    { PIC30_SET_XMEMORY_ATTR(section) }

  if ((styp_flags & STYP_YMEM) == STYP_YMEM)
    { PIC30_SET_YMEMORY_ATTR(section) }

  if ((styp_flags & STYP_PSV) == STYP_PSV)
    { PIC30_SET_PSV_ATTR(section) }

  if ((styp_flags & STYP_EEDATA) == STYP_EEDATA)
    { PIC30_SET_EEDATA_ATTR(section) }

  if ((styp_flags & STYP_MEMORY) == STYP_MEMORY)
    { PIC30_SET_MEMORY_ATTR(section) }

  if ((styp_flags & STYP_ABSOLUTE) == STYP_ABSOLUTE)
    { PIC30_SET_ABSOLUTE_ATTR(section) }

  if ((styp_flags & STYP_REVERSE) == STYP_REVERSE)
    { PIC30_SET_REVERSE_ATTR(section) }

/*  if ((styp_flags & STYP_HEAP) == STYP_HEAP)
    { PIC30_SET_HEAP_ATTR(section) }  */

  if ((styp_flags & STYP_DMA) == STYP_DMA)
    { PIC30_SET_DMA_ATTR(section) }

  if ((styp_flags & STYP_BOOT) == STYP_BOOT)
    { PIC30_SET_BOOT_ATTR(section) }

  if ((styp_flags & STYP_SECURE) == STYP_SECURE)
    { PIC30_SET_SECURE_ATTR(section) }

  if ((styp_flags & STYP_NOLOAD) == STYP_NOLOAD)
    { PIC30_SET_NOLOAD_ATTR(section) }

  /*
   * Now reload the flagword to satisfy BFD's
   * arcane calling convention.
   */

#if PIC30_DEBUG
  printf(" %lx\n", (long unsigned) section->flags);
#endif

  if (flags_ptr == NULL)
    return FALSE;

  * flags_ptr = section->flags;
  return TRUE;

}


/*****************************************************************************/

/*
 *  This routine is cloned from sec_to_styp_flags() in coffcode.h
 *  Several blocks of conditional code for other targets have been
 *  removed for clarity.
 *
 *  Note: Not all of the COFF section flags can be set here,
 *  because we don't have access to the entire bfd section.
 *  There is an important block of code in coffcode.h,
 *  coff_write_object_contents(), that tests pic30-specific
 *  attributes and sets additional STYP flags.
 */
static long
pic30_sec_to_styp_flags (sec_name, sec_flags)
     const char *sec_name;
     flagword sec_flags;
{
  long styp_flags = 0;

#if PIC30_DEBUG
  printf("pic30_sec_to_styp_flags: %s %lx --> ", sec_name,
         (long unsigned) sec_flags);
#endif

  if (!strcmp (sec_name, _TEXT))
    {
      styp_flags = STYP_TEXT;
    }
  else if (!strcmp (sec_name, _DATA))
    {
      styp_flags = STYP_DATA;
    }
  else if (!strcmp (sec_name, _BSS))
    {
      styp_flags = STYP_BSS;
    }
  else if (!strcmp (sec_name, _COMMENT))
    {
      styp_flags = STYP_INFO;
    }
  else if (!strncmp (sec_name, ".debug", 6))
    {
      /* Handle the XCOFF debug section and DWARF2 debug sections.  */
      if (!sec_name[6])
        styp_flags = STYP_XCOFF_DEBUG;
      else
        styp_flags = STYP_DEBUG_INFO;
    }
  else if (!strncmp (sec_name, ".stab", 5))
    {
      styp_flags = STYP_DEBUG_INFO;
    }
  /* Try and figure out what it should be */
  else if (sec_flags & SEC_DEBUGGING)
    {
      styp_flags = STYP_DSECT;

      if (sec_flags & SEC_CODE)      /* special case of      */
        styp_flags |= SEC_CODE;      /* attributes code,info */
    }
  else if (sec_flags & SEC_CODE)
    {
      styp_flags = STYP_TEXT;
    }
  else if (sec_flags & SEC_DATA)
    {
      styp_flags = STYP_DATA;
    }
  else if (sec_flags & SEC_READONLY)
    {
      styp_flags = STYP_LIT;
    }
  else if (sec_flags & SEC_LOAD)
    {
      /* do nothing */
/*       styp_flags = STYP_TEXT; */
    }
  else if (sec_flags & SEC_ALLOC)
    {
      styp_flags = STYP_BSS;
    }

  if (sec_flags & SEC_NEVER_LOAD)
    styp_flags |= STYP_NOLOAD;

#if PIC30_DEBUG
  printf(" %lx\n", styp_flags);
#endif

  return styp_flags;
}

/*****************************************************************************/


/*
 * Defines a little-endian bfd_target structure for coff-pic30 named
 * pic30_coff_vec.
 * There are no EXTRA_O_FLAGS (object flags) or EXTRA_S_FLAGS (section flags).
 * The character normally found at the front of a symbol is '_'.  There is no
 * data needed by back-end routines.
 */
CREATE_LITTLE_COFF_TARGET_VEC (pic30_coff_vec, "coff-pic30", 0, 
                               SEC_CODE | SEC_READONLY | SEC_NEVER_LOAD |
                               SEC_DEBUGGING | SEC_MERGE, '_', NULL)

/*****************************************************************************/

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


extern bfd_reloc_status_type
pic30_bfd_reloc_range_check (howto, relocation, abfd, symbol, error_msg)
  reloc_howto_type *howto;
  bfd_vma relocation;
  bfd *abfd;
  asymbol *symbol;
  char ** error_msg;

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
      case R_RELBYTE:
        /* valid range is [-128..255] */
        if ((relocation > 0xFF) && ~(relocation | 0x7F))
          rc = bfd_reloc_overflow;
        break;
      case R_PIC30_WORD:
      case R_RELWORD:
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
        /* valid range is [-32..31] and not [-2, -1, 0] */
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
} /* extern bfd_reloc_status_type pic30_bfd_reloc_range_check (...)*/


/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_is_even_address (abfd, reloc_entry, symbol, data, input_section,
                            output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd ATTRIBUTE_UNUSED;
   char ** error_msg;

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
        rc = pic30_coff_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);

   } /* else */

   return rc;
} /* static bfd_reloc_status_type pic30_coff_is_even_address(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_is_file_reg_address (abfd, reloc_entry, symbol, data, input_section,
                                output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd ATTRIBUTE_UNUSED;
   char ** error_msg;

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
 *               call pic30_coff_is_even_address()
 *             else
 *               call pic30_coff_perform_generic()
 *
 *****************************************************************************/

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
      offset = pic30_coff_extract_bytes (abfd, data, 4, octets, TRUE, TRUE);
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
        rc = pic30_coff_is_even_address(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);
      else
        /* finish by calling the generic special function */
        rc = pic30_coff_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);

   } /* else */

   return rc;
} /* static bfd_reloc_status_type pic30_coff_is_file_reg_address(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_program_address (abfd, reloc_entry, symbol, data,
                                    input_section, output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry ATTRIBUTE_UNUSED;
   asymbol * symbol;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd;
   char ** error_msg;

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
      else if ((((symbol->section->flags & SEC_CODE) == 0) &&                                    (symbol->section->auxflash == 0)) &&
               (!bfd_is_abs_section (symbol->section) &&
                !bfd_is_und_section (symbol->section)))
      {
         *error_msg = (char *) malloc(BUFSIZ);
         sprintf(*error_msg,
                 "Symbol (%s) is not located in an executable section.",
                 symbol->name);
         rc = bfd_reloc_dangerous;
      } /* if ((((symbol->section->flags & SEC_CODE) == 0) 
	       (symbol->section->auxflash == 0))&&
               (!bfd_is_abs_section (symbol->section) &&
               !bfd_is_und_section (symbol->section))) */
      else
        /* finish by calling the generic special function */
        rc = pic30_coff_perform_generic(abfd, reloc_entry, symbol, data,
                                        input_section, output_bfd, error_msg);
   } /* else */

   return rc;
} /* static bfd_reloc_status_type pic30_coff_perform_program_address(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_pc_relative (abfd, reloc_entry, symbol, data,
                                input_section, output_bfd, error_msg)
   bfd * abfd ATTRIBUTE_UNUSED;
   arelent * reloc_entry;
   asymbol * symbol ATTRIBUTE_UNUSED;
   PTR data ATTRIBUTE_UNUSED;
   asection * input_section ATTRIBUTE_UNUSED;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;
   

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
 *             call pic30_coff_perform_generic() to finish the relocation.
 *
 * Note:  Unlike other offsets and lengths, the relocation address
 *        is provided in octets, not PC units.
 * 
 *****************************************************************************/

{
   bfd_reloc_status_type rc = bfd_reloc_continue;

   if (output_bfd != (bfd *) NULL)
   {
       /* ASSEMBLER */
       rc = bfd_reloc_ok;
   } /* if (output_bfd != (bfd *) NULL) */
   else
   {
      short offset;
      /* LINKER */
      reloc_howto_type * howto = reloc_entry->howto;

      if ((bfd_is_und_section (symbol->section)) &&
          ((symbol->flags & BSF_WEAK) == 0) &&
          (howto->type != R_PIC30_PCREL_ACCESS)) /* for CodeGuard */
         rc = bfd_reloc_undefined;
      else
      {
        /* look for instruction offset */
        bfd_size_type octets = reloc_entry->address;
        short offset = pic30_coff_extract_bytes (abfd, data, 2, octets,
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
            byte_data[octets+1] = byte_data[octets+1] & 0xFC;
          }
        }

	else if (offset) /* store as addend, clear instruction offset */
          {
            reloc_entry->addend = offset;
            pic30_coff_insert_bytes (abfd, data, 2, octets, 0, TRUE, TRUE);
          }

        /* adjust reloc address by 1 pword (4 octets) to get offset right */
        reloc_entry->address += 4;

        /* finish by calling the even_address special function */
        rc = pic30_coff_is_even_address (abfd, reloc_entry, symbol, data,
                                         input_section, output_bfd, error_msg);
      } /* else */
   } /* else */

   return rc;
} /* static bfd_reloc_status_type pic30_coff_perform_pc_relative(...) */


/*****************************************************************************
 * Utility routine for data relocations.
 *****************************************************************************/

static bfd_vma
pic30_coff_extract_bytes (abfd, data, count, addr, skip_upper, skip_phantom)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   unsigned char skip_upper;
   unsigned char skip_phantom;

/*****************************************************************************
 *
 * Extracts COUNT bytes from DATA with ability to skip upper and phantom bytes.
 *
 *****************************************************************************/

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
} /* static bfd_vma pic30_coff_extract_bytes(...) */

/*****************************************************************************/

static void
pic30_coff_insert_bytes (abfd, data, count, addr, value,
                        skip_upper, skip_phantom)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   bfd_vma value;
   unsigned char skip_upper;
   unsigned char skip_phantom;

/*****************************************************************************
 *
 * Inserts COUNT bytes of VALUE into DATA with ability to skip upper and
 * phantom bytes.
 *
 *****************************************************************************/

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
} /* static void pic30_coff_insert_bytes(...) */

/*****************************************************************************/

static bfd_vma
pic30_coff_extract_bytes_data_mem (abfd, data, count, addr)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;

/*****************************************************************************
 *
 * Extract COUNT bytes from a data memory section. Skips phantom bytes.
 *
 *****************************************************************************/

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
} /* static bfd_vma pic30_coff_extract_bytes_data_mem(...) */

/*****************************************************************************/

static void
pic30_coff_insert_bytes_data_mem (abfd, data, count, addr, value)
   bfd * abfd ATTRIBUTE_UNUSED;
   PTR data;
   int count;
   bfd_size_type addr;
   bfd_vma value;

/*****************************************************************************
 *
 * Insert COUNT bytes of VALUE into a data memory section. Skips phantom bytes.
 *
 *****************************************************************************/

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
} /* static void pic30_coff_insert_bytes_data_mem(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_data_directive (abfd, reloc_entry, symbol, data,
                                   input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;

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
         offset = pic30_coff_extract_bytes (abfd, data, count, octets,
                                           TRUE, TRUE);
      else
         /* Relocation site is data memory */
         offset = pic30_coff_extract_bytes_data_mem (abfd, data, count, octets);

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
         pic30_coff_insert_bytes (abfd, data, count, octets, relocation,
                                  TRUE, TRUE);
      else
         /* Relocation site is data memory */
         pic30_coff_insert_bytes_data_mem (abfd, data, count, octets, relocation);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /* static bfd_reloc_status_type pic30_coff_perform_data_directive(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_p_data_directive (abfd, reloc_entry, symbol, data,
                                     input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg ATTRIBUTE_UNUSED;

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

      /* If the relocation site is not in a CODE  or auxflash section, then this relocation
         is not supported.  Linker will abort if this ever occurs.  This means
         that there is a problem in the assembler! */
      if (((input_section->output_section->flags & SEC_CODE) == 0) &&
	  (input_section->output_section->auxflash != 1))
         return bfd_reloc_notsupported;   /* exit if not a CODE or auxflash section */

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
      offset = pic30_coff_extract_bytes (abfd, data, count, octets,
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
      pic30_coff_insert_bytes (abfd, data, count, octets, relocation,
                               FALSE, TRUE);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /*  static bfd_reloc_status_type pic30_coff_perform_p_data_directive(...) */

/*****************************************************************************/

static bfd_vma
bfd_pic30_handle (abfd, addr, symbol)
     bfd *abfd;
     bfd_vma addr;
     asymbol *symbol;

/*****************************************************************************
 *
 * Resolve handle relocations by looking up the symbol in the handle hash
 * table and returning the (16 bit) jump table entry.
 *
 * Note: if address fits in 16 bits, just return it.
 *
 *****************************************************************************/

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


/*****************************************************************************/

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


static bfd_reloc_status_type
pic30_coff_perform_operators (abfd, reloc_entry, symbol, data,
                              input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;

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
 *             for symbols located in a CODE, PSV, or READONLY section. If this
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
      }
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
           /* Relocation site is program memory, skip upper & phantom bytes */
            target = pic30_coff_extract_bytes (abfd, data, count, octets,
                                               TRUE, TRUE);
         else
           /* Relocation site is data memory */
            target = pic30_coff_extract_bytes_data_mem (abfd, data, count, octets);
      } /* else */

#if 0
      printf("target = 0x%x\n", (long) target);
#endif

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
        /* do it the easy way */
        x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
        x = ((x & ~howto->dst_mask) |
             ((relocation << howto->bitpos) & howto->dst_mask));
        bfd_put_32 (abfd, x, (unsigned char *) data + octets);
      } /* if */
      else
      {
         int count = (howto->size == 0) ? 1 : (2 * howto->size);

         if (PIC30_SECTION_IN_PROGRAM_MEMORY (input_section->output_section))
           /* Relocation site is program memory, skip upper & phantom bytes */
            pic30_coff_insert_bytes (abfd, data, count, octets, relocation,
                                     TRUE, TRUE);
         else
           /* Relocation site is data memory */
            pic30_coff_insert_bytes_data_mem (abfd, data,
                                              count, octets, relocation);
      } /* else */

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /* static bfd_reloc_status_type pic30_coff_perform_operators(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_p_operators (abfd, reloc_entry, symbol, data,
                                input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;

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
 *             located in a CODE or READONLY section. If this special
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

      /* If the relocation site is not in a CODE section, then this relocation
         is not supported.  Linker will abort if this ever occurs.  This means
         that there is a problem in the assembler! */
      if (((input_section->output_section->flags & SEC_CODE) == 0) &&
           (input_section->output_section->auxflash != 1))
         return bfd_reloc_notsupported;   /* exit if not a CODE section */

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
      printf ("howto->name = %s\n", howto->name);
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
      target = pic30_coff_extract_bytes (abfd, data, count, octets,
                                         FALSE, TRUE);
#if 0
      printf("target = 0x%x\n", (long) target);
#endif
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
      pic30_coff_insert_bytes (abfd, data, count, octets, relocation,
                               FALSE, TRUE);

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /* static bfd_reloc_status_type pic30_coff_perform_p_operators(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_dmaoffset_dmapage (abfd, reloc_entry, symbol, data,
                              input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;

/*****************************************************************************
 *
 * Special function for dmaoffset() & dmapage() operators.
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
          (howto->type == R_PIC30_WORD_DMAPAGE)||
          (howto->type == R_PIC30_UNSIGNED_10_DMAOFFSET)||
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
             target = pic30_coff_extract_bytes (abfd, data, count, octets,
                                                skip_upper, TRUE);
           }
         else
           /* Relocation site is data memory */
            target = pic30_coff_extract_bytes_data_mem (abfd, data, count, octets);
      } /* else */

      relocation += target;

      /* calculate dmaoffset here */
      if ((howto->type == R_PIC30_DMAOFFSET) ||                                           (howto->type == R_PIC30_WORD_DMAOFFSET)||
	  (howto->type == R_PIC30_P_DMAOFFSET))
	{
	   if (dma_base_defined == FALSE)  /* this should have been checked earlier */
             {                             /*   when the section was allocated      */
              fprintf( stderr, 
                   "Link Error: __DMA_BASE is needed, but not defined (check                     linker script?)\n");
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
      if ((howto->type == R_PIC30_WORD_DMAOFFSET)||
          (howto->type == R_PIC30_WORD_DMAPAGE)||
          (howto->type == R_PIC30_UNSIGNED_10_DMAOFFSET)||
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
             pic30_coff_insert_bytes (abfd, data, count, octets,
                                      relocation, skip_upper, TRUE);
           }
         else
           /* Relocation site is data memory */
            pic30_coff_insert_bytes_data_mem (abfd, data,
                                              count, octets, relocation);
      } /* else */

      /*
       * Relocation is complete.  Return the overflow flag to
       * bfd_perform_relocation
       */
      return flag;
   } /* else */
} /* static bfd_reloc_status_type pic30_coff_perform_dmaoffset(...) */

/*****************************************************************************/

static bfd_reloc_status_type
pic30_coff_perform_generic (abfd, reloc_entry, symbol, data,
                            input_section, output_bfd, error_msg)
   bfd * abfd;
   arelent * reloc_entry;
   asymbol * symbol;
   PTR data;
   asection * input_section;
   bfd * output_bfd;
   char ** error_msg;

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

      /* only certain relocation types support CODE targets */
      if ((symbol->section->flags & SEC_CODE) || 
	  (symbol->section->auxflash == 1))
        switch (howto->type)
          {
          case R_PIC30_PCREL_BRANCH_SLIT6:
          case R_PIC30_PCREL_DO:
          case R_PIC30_DO_ABSOLUTE:
          case R_PIC30_PCREL_BRANCH:
          case R_PIC30_BRANCH_ABSOLUTE:
          case R_PIC30_PGM_ADDR_LSB:
          case R_PIC30_PGM_ADDR_MSB:
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


      /*
       * if reloc is PC relative, back up the data pointer
       * ...because pic30_coff_perform_pc_relative() advanced it
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
      else if (howto->type == R_PIC30_UNSIGNED_8) {

        if ((symbol->value & howto->src_mask) != symbol->value) {
           *error_msg = (char *) malloc(BUFSIZ);
           sprintf(*error_msg, "Cannot resolve relocation (%s) because "
                   " symbol (%s) is too large.",
                   howto->name, symbol->name);
           flag =  bfd_reloc_overflow;
        }
        relocation = symbol->value;
        relocation = ((symbol->value & 0xE0) << 2) +
                     (symbol->value & 0x1F);
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
        flag = pic30_bfd_reloc_range_check (howto, relocation,
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

/*****************************************************************************/

