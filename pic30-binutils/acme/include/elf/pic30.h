/*
 * pic30.h -- ELF format for the PIC30 architecture.
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by John Elliott.
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _ELF_PIC30_H
#define _ELF_PIC30_H

extern int pic30_adjustable_against_section(unsigned short);
extern bfd_boolean pic30_elf_backend_section_from_bfd_section
  PARAMS ((bfd *, asection *, int *));

#define PIC30_IS_EVEN(value) (((value) & 0x1) == 0)

#define PIC30_BITS_PER_PROGRAM_ADDRESS	24
#define PIC30_TBLOFFSET(value)		((value) & 0xFFFF)
#define PIC30_PSVOFFSET(value)		(((value) | 0x8000) & 0xFFFF)
#define PIC30_EDSOFFSET(value)		(((value) | 0x8000) & 0xFFFF)
#define PIC30_TBLPAGE(value)		((value) >> 16)
#define PIC30_PSVPAGE(value)		((value) >> 15)
#define PIC30_HANDLE(value)		((value) & 0xFFFF)
#define PIC30_PSVPTR(value) ((((value) & 0xFFFF8000) << 1) | \
        (((value) & 0x7FFF) << 1) | 0x1 )

/************************************************************************/
/*   names of "special" sections					*/
/************************************************************************/

#define _TEXT		".text"
#define _DATA		".data"
#define _BSS		".bss"
#define _COMMENT	".comment"

/************************************************************************/
/*   pic30-specific ELF header flags                                    */
/************************************************************************/

#define EF_PIC30_PROC 0x0000FFFF /* 2 bytes in the e_flags field
                                    for encoding processor ID */


/************************************************************************/
/*   pic30-specific section flags                                       */
/************************************************************************/

#define SHF_MEMORY   (1 << 18)  /* User-defined memory */
#define SHF_UNUSED   (1 << 19)  /* Unused */
/* OS and processor-specific flags start at postion 20 */
#define SHF_SECURE   (1 << 20)  /* Secure segment */
#define SHF_BOOT     (1 << 21)  /* Boot segment */
#define SHF_DMA      (1 << 22)  /* DMA memory */
#define SHF_NOLOAD   (1 << 23)  /* Do not allocate or load */
#define SHF_NEAR     (1 << 24)  /* Near memory */
#define SHF_PERSIST  (1 << 25)  /* Persistent */
#define SHF_XMEM     (1 << 26)  /* X Memory */
#define SHF_YMEM     (1 << 27)  /* Y Memory */
#define SHF_PSV      (1 << 28)  /* Constants in program memory */
#define SHF_EEDATA   (1 << 29)  /* Data Flash memory */
#define SHF_ABSOLUTE (1 << 30)  /* Absolute address */
#define SHF_REVERSE  (1 << 31)  /* Reverse aligned */


/************************************************************************/
/*   pic30-specific "extended" section flags                            */
/*                                                                      */
/*  To work around space limitations in the section flags field,        */
/*  these values are encoded in a symbol named __ext__attr_secname      */
/************************************************************************/

#define STYP_HEAP    (1 << 0)   /* Heap memory */
#define STYP_STACK   (1 << 1)   /* Stack memory */
#define STYP_EDS     (1 << 2)   /* Extended Data Space memory */
#define STYP_PAGE    (1 << 3)   /* Section avoids page boundaries */
#define STYP_AUXFLASH    (1 << 4)   /* Auxiliary flash memory */

/************************************************************************/


#define PIC30_SECTION_IN_PROGRAM_MEMORY(sec) \
  (((sec->flags & SEC_CODE) == SEC_CODE) | (sec->auxflash == 1))
#define PIC30_SECTION_IN_PSV_MEMORY(sec) \
  ((sec->psv || ((sec->flags & SEC_READONLY) == SEC_READONLY)))
#define PIC30_SECTION_IN_DATA_MEMORY(sec) \
  (PIC30_IS_BSS_ATTR(sec) ||              \
   PIC30_IS_DATA_ATTR(sec) ||             \
   PIC30_IS_PERSIST_ATTR(sec) ||          \
   PIC30_IS_STACK_ATTR(sec) ||            \
   PIC30_IS_HEAP_ATTR(sec))
#define PIC30_SECTION_IS_BSS_TYPE(sec) \
  (((sec->flags & (SEC_ALLOC | SEC_DATA | SEC_READONLY | SEC_CODE)) \
     == SEC_ALLOC) && (sec->psv == 0) && (sec->eedata == 0))
#define PIC30_SECTION_IS_DATA_TYPE(sec) \
  ((sec->flags & SEC_DATA) == SEC_DATA)

#define FILE_REG_LIMIT 8192

#include "elf/reloc-macros.h"

/*
** ==============================================
** Relocation types.
** ==============================================
** These should be in the same order as the array
** elf_pic30_howto_table_rel in bfd/elf32-pic30.c
** ==============================================
*/
START_RELOC_NUMBERS (elf_pic30_reloc_type)
  RELOC_NUMBER (R_PIC30_NONE,			0)
  RELOC_NUMBER (R_PIC30_8,			1)
  RELOC_NUMBER (R_PIC30_16,			2)
  RELOC_NUMBER (R_PIC30_32,			3)
  RELOC_NUMBER (R_PIC30_FILE_REG_BYTE,		4)
  RELOC_NUMBER (R_PIC30_FILE_REG,		5)
  RELOC_NUMBER (R_PIC30_FILE_REG_WORD, 		6)
  RELOC_NUMBER (R_PIC30_FILE_REG_WORD_WITH_DST, 7)
  RELOC_NUMBER (R_PIC30_WORD,			8)
  RELOC_NUMBER (R_PIC30_PBYTE,			9)
  RELOC_NUMBER (R_PIC30_PWORD,			10)
  RELOC_NUMBER (R_PIC30_HANDLE,			11)
  RELOC_NUMBER (R_PIC30_PADDR,			12)
  RELOC_NUMBER (R_PIC30_P_PADDR,		13)
  RELOC_NUMBER (R_PIC30_PSVOFFSET,		14)
  RELOC_NUMBER (R_PIC30_TBLOFFSET,		15)
  RELOC_NUMBER (R_PIC30_WORD_HANDLE,		16)
  RELOC_NUMBER (R_PIC30_WORD_PSVOFFSET,		17)
  RELOC_NUMBER (R_PIC30_PSVPAGE,		18)
  RELOC_NUMBER (R_PIC30_P_PSVPAGE,		19)
  RELOC_NUMBER (R_PIC30_WORD_PSVPAGE,		20)
  RELOC_NUMBER (R_PIC30_WORD_TBLOFFSET,		21)
  RELOC_NUMBER (R_PIC30_TBLPAGE,		22)
  RELOC_NUMBER (R_PIC30_P_TBLPAGE,		23)
  RELOC_NUMBER (R_PIC30_WORD_TBLPAGE,		24)
  RELOC_NUMBER (R_PIC30_P_HANDLE,		25)
  RELOC_NUMBER (R_PIC30_P_PSVOFFSET,		26)
  RELOC_NUMBER (R_PIC30_P_TBLOFFSET,		27)
  RELOC_NUMBER (R_PIC30_PCREL_BRANCH,		28)
  RELOC_NUMBER (R_PIC30_BRANCH_ABSOLUTE,	29)
  RELOC_NUMBER (R_PIC30_PCREL_DO,		30)
  RELOC_NUMBER (R_PIC30_DO_ABSOLUTE,		31)
  RELOC_NUMBER (R_PIC30_PGM_ADDR_LSB,		32)
  RELOC_NUMBER (R_PIC30_PGM_ADDR_MSB,		33)
  RELOC_NUMBER (R_PIC30_UNSIGNED_4,		34)
  RELOC_NUMBER (R_PIC30_UNSIGNED_5,		35)
  RELOC_NUMBER (R_PIC30_BIT_SELECT_3,		36)
  RELOC_NUMBER (R_PIC30_BIT_SELECT_4_BYTE,	37)
  RELOC_NUMBER (R_PIC30_BIT_SELECT_4,		38)
  RELOC_NUMBER (R_PIC30_DSP_6,			39)
  RELOC_NUMBER (R_PIC30_DSP_PRESHIFT,		40)
  RELOC_NUMBER (R_PIC30_SIGNED_10_BYTE,		41)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10,		42)
  RELOC_NUMBER (R_PIC30_UNSIGNED_14,		43)
  RELOC_NUMBER (R_PIC30_FRAME_SIZE,		44)
  RELOC_NUMBER (R_PIC30_PWRSAV_MODE,		45)
  RELOC_NUMBER (R_PIC30_DMAOFFSET,		46)
  RELOC_NUMBER (R_PIC30_P_DMAOFFSET,		47)
  RELOC_NUMBER (R_PIC30_WORD_DMAOFFSET,		48)
  RELOC_NUMBER (R_PIC30_PSVPTR, 		49)
  RELOC_NUMBER (R_PIC30_P_PSVPTR,		50)
  RELOC_NUMBER (R_PIC30_L_PSVPTR,		51)
  RELOC_NUMBER (R_PIC30_WORD_PSVPTR,		52)
  RELOC_NUMBER (R_PIC30_CALL_ACCESS,		53)
  RELOC_NUMBER (R_PIC30_PCREL_ACCESS,		54)
  RELOC_NUMBER (R_PIC30_ACCESS,  		55)
  RELOC_NUMBER (R_PIC30_P_ACCESS,  		56)
  RELOC_NUMBER (R_PIC30_L_ACCESS,  		57)
  RELOC_NUMBER (R_PIC30_WORD_ACCESS,  		58)
  RELOC_NUMBER (R_PIC30_EDSPAGE,  		59)
  RELOC_NUMBER (R_PIC30_P_EDSPAGE,  		60)
  RELOC_NUMBER (R_PIC30_WORD_EDSPAGE,  		61)
  RELOC_NUMBER (R_PIC30_EDSOFFSET,  		62)
  RELOC_NUMBER (R_PIC30_P_EDSOFFSET,  		63)
  RELOC_NUMBER (R_PIC30_WORD_EDSOFFSET, 	64)
  RELOC_NUMBER (R_PIC30_UNSIGNED_8,		65)
  RELOC_NUMBER (R_PIC30_UNSIGNED_15,		66)
  RELOC_NUMBER (R_PIC30_NO_SHIFT_UNSIGNED_10,   67)
  RELOC_NUMBER (R_PIC30_PCREL_BRANCH_SLIT6,     68)
  RELOC_NUMBER (R_PIC30_DMAPAGE,		69)
  RELOC_NUMBER (R_PIC30_P_DMAPAGE,		70)
  RELOC_NUMBER (R_PIC30_WORD_DMAPAGE,		71)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_TBLOFFSET,  72)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_PSVOFFSET,  73)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_TBLPAGE,    74)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_PSVPAGE,    75)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_HANDLE,     76)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_DMAPAGE,    77)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_DMAOFFSET,  78)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_EDSPAGE,    79)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_EDSOFFSET,  80)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_PSVPTR,     81)
  RELOC_NUMBER (R_PIC30_UNSIGNED_10_ACCESS,     82)

END_RELOC_NUMBERS (R_PIC30_maxext)

#endif /* _ELF_PIC30_H */
