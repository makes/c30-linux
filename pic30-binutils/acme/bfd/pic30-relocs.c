/*
** pic30-relocs.c
**
** Copyright (c) 2004 Microchip Technology, Inc.
**
** This file contains info about the relocation
** types associated with the coff-pic30 object
** file format.
*/

/*
** special operator macros
*/
#define PIC30_TBLOFFSET(value) ((value) & 0xFFFF)
#define PIC30_PSVOFFSET(value) (((value) | 0x8000) & 0xFFFF)
#define PIC30_TBLPAGE(value)  ((value) >> 16)
#define PIC30_PSVPAGE(value)  ((value) >> 15)
#define PIC30_HANDLE(value) ((value) & 0xFFFF)

/*
** range checking macros
*/
#define PIC30_IS_EVEN(value) (((value) & 0x1) == 0)
#define PIC30_IS_VALID_DO_OFFSET(lit) \
        (((lit) >= -32768) && ((lit) < 32768) && \
         ((lit) != -1) && ((lit) != 0) && ((lit) != 1))
#define PIC30_IS_1_BIT_LITERAL(lit) (((lit) >= 0) && ((lit) < 2))
#define PIC30_IS_3_BIT_LITERAL(lit) (((lit) >= 0) && ((lit) < 8))
#define PIC30_IS_4_BIT_LITERAL(lit) (((lit) >= -8) && ((lit) < 8))
#define PIC30_IS_4_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 16))
#define PIC30_IS_5_BIT_LITERAL(lit) (((lit) >= -16) && ((lit) < 16))
#define PIC30_IS_5_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 32))
#define PIC30_IS_DSP6_BIT_LITERAL(lit) (((lit) >= -16) && ((lit) <= 16))
#define PIC30_IS_BYTE_VALUE(lit) (((lit) >= -128) && ((lit) < 256))
#define PIC30_IS_10_BIT_UNSIGNED_LITERAL(lit) ((0 <= (lit)) && ((lit) <= 1023))
#define PIC30_IS_14_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 16384))
#define PIC30_IS_16_BIT_LITERAL(lit) (((lit) >= -32768) && ((lit) < 65536))
#define PIC30_IS_16_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 65536))
#define PIC30_IS_13_BIT_FILE_REG(lit) (((lit) >= 0) && ((lit) < 8192))
#define PIC30_IS_BYTE_VALUE_UNSIGNED(lit) (((lit) >= 0) && ((lit) < 256))

/*
** relocation types
*/
#define R_PIC30_NONE                     0
#define R_PIC30_FILE_REG_BYTE          160
#define R_PIC30_FILE_REG               161
#define R_PIC30_FILE_REG_WORD          162
#define R_PIC30_FILE_REG_WORD_WITH_DST 163
#define R_PIC30_UNSIGNED_4             164
#define R_PIC30_UNSIGNED_5             165
#define R_PIC30_PCREL_DO               166
#define R_PIC30_DO_ABSOLUTE            167
#define R_PIC30_SIGNED_10_BYTE         168
#define R_PIC30_UNSIGNED_10            169
#define R_PIC30_UNSIGNED_14            170
#define R_PIC30_FRAME_SIZE             171
#define R_PIC30_WORD                   172
#define R_PIC30_WORD_TBLOFFSET         173
#define R_PIC30_WORD_PSVOFFSET         174
#define R_PIC30_WORD_TBLPAGE           175
#define R_PIC30_WORD_PSVPAGE           176
#define R_PIC30_WORD_HANDLE            177
#define R_PIC30_PCREL_BRANCH           178
#define R_PIC30_BRANCH_ABSOLUTE        179
#define R_PIC30_PGM_ADDR_LSB           180
#define R_PIC30_PGM_ADDR_MSB           181
#define R_PIC30_TBLOFFSET              182
#define R_PIC30_PSVOFFSET              183
#define R_PIC30_TBLPAGE                184
#define R_PIC30_PSVPAGE                185
#define R_PIC30_HANDLE                 186
#define R_PIC30_PADDR                  187
#define R_PIC30_P_TBLOFFSET            188
#define R_PIC30_P_PSVOFFSET            189
#define R_PIC30_P_TBLPAGE              190
#define R_PIC30_P_PSVPAGE              191
#define R_PIC30_P_HANDLE               192
#define R_PIC30_P_PADDR                193
#define R_PIC30_BIT_SELECT_3           194
#define R_PIC30_BIT_SELECT_4_BYTE      195
#define R_PIC30_BIT_SELECT_4           196
#define R_PIC30_DSP_6                  197
#define R_PIC30_DSP_PRESHIFT           198
#define R_PIC30_PWRSAV_MODE            199
#define R_PIC30_PBYTE                  200
#define R_PIC30_PWORD                  201
#ifdef INT64
#define R_PIC30_QUAD                   202
#endif
#define R_PIC30_WORD_DMAOFFSET         203
#define R_PIC30_DMAOFFSET              204
#define R_PIC30_P_DMAOFFSET            205
#define R_PIC30_UNSIGNED_8             222
#define R_PIC30_UNSIGNED_15            223
#define R_PIC30_NO_SHIFT_UNSIGNED_10   224
#define R_PIC30_PCREL_BRANCH_SLIT6     225
#define R_PIC30_WORD_DMAPAGE           226
#define R_PIC30_DMAPAGE                227
#define R_PIC30_P_DMAPAGE              228
#define R_PIC30_UNSIGNED_10_TBLOFFSET  229
#define R_PIC30_UNSIGNED_10_PSVOFFSET  230
#define R_PIC30_UNSIGNED_10_TBLPAGE    231
#define R_PIC30_UNSIGNED_10_PSVPAGE    232
#define R_PIC30_UNSIGNED_10_HANDLE     233
#define R_PIC30_UNSIGNED_10_DMAPAGE    234
#define R_PIC30_UNSIGNED_10_DMAOFFSET  235

/*
** relocation field descriptions
*/
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
         complain_overflow_unsigned,
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
   HOWTO(R_PIC30_WORD_TBLPAGE, 16, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - TBLPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_PSVPAGE, 15, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - PSVPAGE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_WORD_HANDLE, 0, 2, 16, FALSE, 4, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "WORD - HANDLE",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_PCREL_BRANCH, 1, 1, 16, TRUE, 0, complain_overflow_signed,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE BRANCH",
         TRUE, 0xffff, 0x00ffff, TRUE),
   HOWTO(R_PIC30_BRANCH_ABSOLUTE, 1, 1, 16, TRUE, 0, complain_overflow_signed,
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
   HOWTO(R_PIC30_TBLPAGE, 16, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_OPERATORS, "TBLPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_PSVPAGE, 15, 1, 16, FALSE, 0, complain_overflow_bitfield,
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
   HOWTO(R_PIC30_P_TBLPAGE, 16, 1, 16, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_P_OPERATORS, "P-TBLPAGE",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_PSVPAGE, 15, 1, 16, FALSE, 0, complain_overflow_bitfield,
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
   HOWTO(R_PIC30_DSP_6, 0, 0, 5, FALSE, 0, complain_overflow_dont,
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
#ifdef INT64
   HOWTO(R_PIC30_QUAD, 0, 4, 64, FALSE, 0, complain_overflow_bitfield,
         RELOC_SPECIAL_FN_GENERIC, ".quad",
         TRUE, 0xffffffffffffffff,0xffffffffffffffff, FALSE)
#endif
   HOWTO(R_PIC30_DMAOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "DMAOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_P_DMAOFFSET, 0, 1, 16, FALSE, 0, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "P-DMAOFFSET",
         TRUE, 0x0000ffff, 0x0000ffff, FALSE),
   HOWTO(R_PIC30_WORD_DMAOFFSET, 0, 2, 16, FALSE, 4, complain_overflow_dont,
         RELOC_SPECIAL_FN_DMAOFFSET_DMAPAGE, "WORD - DMAOFFSET",
         TRUE, 0xffff0, 0x0ffff0, FALSE),
   HOWTO(R_PIC30_UNSIGNED_8, 0, 1, 8, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 8",
         TRUE, 0x00039f, 0x00039f, FALSE),
   HOWTO(R_PIC30_UNSIGNED_15, 0, 1, 15, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "UNSIGNED 15",
         TRUE, 0x007fff, 0x007fff, FALSE),
   HOWTO(R_PIC30_NO_SHIFT_UNSIGNED_10, 0, 1, 10, FALSE, 0, complain_overflow_unsigned,
         RELOC_SPECIAL_FN_GENERIC, "NO SHIFT UNSIGNED 10",
	TRUE, 0x0003ff, 0x0003ff, FALSE),

   HOWTO(R_PIC30_PCREL_BRANCH_SLIT6, 0, 0, 6, TRUE, 4, complain_overflow_signed,
         RELOC_SPECIAL_FN_PCREL, "PC RELATIVE BRANCH SLIT6",
         TRUE, 0x03f0, 0x0003f0, TRUE),
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
};


int
pic30_adjustable_against_section (type)
   unsigned short type;
/******************************************************************************
 *
 *   This function indicate whether a fixup against a locally defined symbol
 *   should be adjusted to be against the section symbol.  It should return
 *   non-zero if the adjustment is acceptable.
 *
 ******************************************************************************/
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
      case R_PIC30_UNSIGNED_10_TBLOFFSET:
      case R_PIC30_UNSIGNED_10_PSVOFFSET:
      case R_PIC30_UNSIGNED_10_PSVPAGE:
      case R_PIC30_UNSIGNED_10_TBLPAGE:
      case R_PIC30_UNSIGNED_10_HANDLE:
      case R_PIC30_WORD:
      case R_PIC30_FILE_REG_WORD:
      case R_PIC30_FILE_REG_WORD_WITH_DST:
      case R_PIC30_PBYTE:
      case R_PIC30_PWORD:
      case R_RELBYTE:
      case R_RELWORD:
      case R_RELLONG:
         rc = 0;
         break;

      default:
         break;
   } /* switch (type) */

   return rc;
}


extern reloc_status_type
pic30_reloc_range_check (howto, relocation, error_msg)
  reloc_howto_type *howto;
  unsigned long relocation;
  char ** error_msg;
/*****************************************************************************
 *
 * Special function for range checking relocation types when the HOWTO specifies
 * "complain_overflow_dont".
 *
 * Note that we can't use the range check macros defined above
 * because relocation is an unsigned type.
 *
 *****************************************************************************/
{
  reloc_status_type rc =  reloc_ok;

   switch (howto->type)
   {
      case R_PIC30_PBYTE:
      case R_PIC30_SIGNED_10_BYTE:
      case R_RELBYTE:
        /* valid range is [-128..255] */
        if ((relocation > 0xFF) && ~(relocation | 0x7F))
          rc = reloc_overflow;
        break;
      case R_PIC30_WORD:
      case R_RELWORD:
        /* valid range is [-32768..65535] */
        if ((relocation > 0xFFFF) && ~(relocation | 0x7FFF))
          rc = reloc_overflow;
        break;
      case R_PIC30_PWORD:
        /* valid range is [--8388608..16777215] */
        if ((relocation > 0xFFFFFF) && ~(relocation | 0x7FFFFF))
          rc = reloc_overflow;
        break;
      case R_PIC30_PCREL_DO:
      case R_PIC30_DO_ABSOLUTE:
        /* valid range is [-32768..32767] and not [-2, -1, 0] */
        if ((relocation > 0x7FFF) && ~(relocation | 0xC0007FFF))
          rc = reloc_overflow;
        if (((relocation & 0xFFFF) == 0xFFFE) ||
            ((relocation & 0xFFFF) == 0xFFFF) ||
            (relocation == 0))
          {
            *error_msg = (char *) malloc(BUFSIZ);
            sprintf(*error_msg,
                 "Illegal value for DO instruction offset (-2, -1, or 0)");
            rc = reloc_dangerous;
          }
        break;
      case R_PIC30_DSP_6:
        /* valid range is [-16..16] */
        if ((relocation > 0x10) && ~(relocation | 0xF))
          rc = reloc_overflow;
        break;
   }

   return rc;
}
