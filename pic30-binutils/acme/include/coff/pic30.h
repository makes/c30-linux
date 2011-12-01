/*
 * pic30.h -- COFF format for the PIC30 architecture.
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
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

/******************************************************************************
 *   FILE HEADER
 ******************************************************************************/

struct external_filehdr
{
   char f_magic[2];  /* magic number             */
   char f_nscns[2];  /* number of sections       */
   char f_timdat[4]; /* time & date stamp        */
   char f_symptr[4]; /* file pointer to symtab   */
   char f_nsyms[4];  /* number of symtab entries */
   char f_opthdr[2]; /* sizeof(optional hdr)     */
   char f_flags[2];  /* flags                    */
};

#define FILHDR struct external_filehdr
#define FILHSZ 20

#define PIC30MAGIC 0x1236

#define PIC30BADMAG(x) (((x).f_magic!=PIC30MAGIC))

/******************************************************************************
 *   AOUT "OPTIONAL HEADER"
 ******************************************************************************/

struct external_optional_header
{
   char magic[2];      /* type of file                          */
   char vstamp[2];     /* version stamp                         */
   char tsize[4];      /* text size in bytes, padded to FW bdry */
   char dsize[4];      /* initialized data "  "                 */
   char bsize[4];      /* uninitialized data "   "              */
   char entry[4];      /* entry pt.                             */
   char text_start[4]; /* base of text used for this file       */
   char data_start[4]; /* base of data used for this file       */
};

#define AOUTHDR struct external_optional_header
#define AOUTHDRSZ 28
#define AOUTSZ 28

/******************************************************************************
 *   SECTION HEADER
 ******************************************************************************
 *
 *   See file "internal.h" for a list of standard section flags.
 *   Also note that section alignment is encoded as a power-of-two
 *   in the most significant four bits of s_flags.
 *
 ******************************************************************************/

/* pic30-specific section flags */
#define STYP_NEAR      (0x10000)  /* Near memory */
#define STYP_PERSIST   (0x20000)  /* Persistent */
#define STYP_XMEM      (0x40000)  /* X memory */
#define STYP_YMEM      (0x80000)  /* Y memory */
#define STYP_PSV      (0x100000)  /* Constants in program memory */
#define STYP_EEDATA   (0x200000)  /* Data Flash memory */
#define STYP_ABSOLUTE (0x400000)  /* Absolute address */
#define STYP_REVERSE  (0x800000)  /* Reverse aligned */
#define STYP_MEMORY  (0x1000000)  /* User-defined memory */
#define STYP_DMA     (0x2000000)  /* DMA memory */
#define STYP_BOOT    (0x4000000)  /* Boot segment */
#define STYP_SECURE  (0x8000000)  /* Secure segment */
#define STYP_RESRVD (0xF0000000)  /* Alignment is encoded here */

/*
 * pic30-specific "extended" section flags
 *
 * To work around space limitations in the section flags field,
 * these values are encoded in a symbol named __ext__attr_secname
 */

#define STYP_HEAP    (0x1)        /* Heap memory */
#define STYP_STACK   (0x2)        /* Stack memory */
#define STYP_EDS     (0x4)        /* Extended Data Space memory */
#define STYP_PAGE    (0x8)        /* Section avoids page boundaries */
#define STYP_AUXFLASH (0x10)        /* Auxiliary flash memory */

struct external_scnhdr
{
   char s_name[8];    /* section name                     */
   char s_paddr[4];   /* physical address, aliased s_nlib */
   char s_vaddr[4];   /* virtual address                  */
   char s_size[4];    /* section size                     */
   char s_scnptr[4];  /* file ptr to raw data for section */
   char s_relptr[4];  /* file ptr to relocation           */
   char s_lnnoptr[4]; /* file ptr to line numbers         */
   char s_nreloc[2];  /* number of relocation entries     */
   char s_nlnno[2];   /* number of line number entries    */
   char s_flags[4];   /* flags                            */
};

#define SCNHDR struct external_scnhdr
#define SCNHSZ 40

/******************************************************************************
 *   names of "special" sections
 ******************************************************************************/

#define _TEXT ".text"
#define _DATA ".data"
#define _BSS ".bss"
#define _COMMENT ".comment"

/******************************************************************************
 *   LINE NUMBERS
 ******************************************************************************
 *   1 line number entry for every "breakpointable" source line in a section.
 *   Line numbers are grouped on a per function basis; first entry in a function
 *   grouping will have l_lnno = 0 and in place of physical address will be the
 *   symbol table index of the function name.
 ******************************************************************************/

struct external_lineno
{
   union
   {
      char l_symndx[4]; /* function name symbol index, iff l_lnno == 0 */
      char l_paddr[4];  /* (physical) address of line number           */
   } l_addr;
   char l_lnno[4];      /* line number                                 */
};

#define LINENO struct external_lineno
#define LINESZ 8

/******************************************************************************
 *   SYMBOLS
 ******************************************************************************/

#define E_SYMNMLEN 8  /* # characters in a symbol name         */

struct external_syment 
{
   union
   {
      char e_name[E_SYMNMLEN];
      struct
      {
         char e_zeroes[4];
         char e_offset[4];
      } e;
   } e;
   char e_value[4];
   char e_scnum[2];
   char e_type[2];
   char e_sclass[1];
   char e_numaux[1];
};

#define SYMENT struct external_syment
#define SYMESZ 18 

/******************************************************************************/

#define N_BTMASK (0xf)
#define N_TMASK  (0x30)
#define N_BTSHFT (4)
#define N_TSHIFT (2)

/******************************************************************************
 *   AUXILIARY ENTRY
 ******************************************************************************/
  
#define E_FILNMLEN 14 /* # characters in a file name           */
#define E_DIMNUM 4    /* # array dimensions in auxiliary entry */

union external_auxent
{
   struct
   {
      char x_tagndx[4]; /* str, un, or enum tag indx */
      union
      {
         struct
         {
            char x_lnno[2]; /* declaration line number */
            char x_size[2]; /* str/union/array size */
         } x_lnsz;
         char x_fsize[4];   /* size of function */
      } x_misc;
      union
      {
         struct
         {  /* if ISFCN, tag, or .bb */
            char x_lnnoptr[4]; /* ptr to fcn line # */
            char x_endndx[4];  /* entry ndx past block end */
         } x_fcn;
         struct
         {  /* if ISARY, up to 4 dimen. */
            char x_dimen[E_DIMNUM][2];
         } x_ary;
      } x_fcnary;
      char x_tvndx[2]; /* tv index */
   } x_sym;

   union
   {
      char x_fname[E_FILNMLEN];
      struct
      {
         char x_zeroes[4];
	 char x_offset[4];
      } x_n;
   } x_file;

   struct
   {
      char x_scnlen[4]; /* section length */
      char x_nreloc[2]; /* # relocation entries */
      char x_nlinno[2]; /* # line numbers */
   } x_scn;

   struct
   {
      char x_tvfill[4];   /* tv fill value */
      char x_tvlen[2];    /* length of .tv */
      char x_tvran[2][2]; /* tv range */
   } x_tv;  /* info about .tv section (in auxent of symbol .tv)) */
};

#define AUXENT union external_auxent
#define AUXESZ 18

/******************************************************************************
 *   RELOCATION DIRECTIVES
 ******************************************************************************/

struct external_reloc
{
  char r_vaddr[4];
  char r_symndx[4];
  char r_type[2];
};

#define RELOC struct external_reloc
#define RELSZ 10

#define HOWTO_TABLE_BASE
