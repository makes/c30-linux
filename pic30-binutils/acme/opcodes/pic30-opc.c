/* pic30-opc.c -- This file contains the opcodes/operands for Microchip dsPIC30
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

#include <stdio.h>
#include <stdlib.h>
#include "sysdep.h"
#include <dis-asm.h>
#include <opcode/pic30.h>
#include "pic30-opc.h"
#include "bfd.h"
#include <coff-pic30.h>

/******************************************************************************
 * PROTOTYPES
 ******************************************************************************/

unsigned char pic30_match_even PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_even_not_14 PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_div_src_reg PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_file_reg_byte PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_file_reg PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_file_reg_word PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_byte_10bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_10bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_16bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_unsigned_short_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_3bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_34bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_nibble PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_unsigned_14bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_unsigned_15bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_frame_size PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_16bit_unsigned_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_4bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_short_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_6bit_lit PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_dsp_writeback PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_dsp_prefetch_dst PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_dsp_x_prefetch_operation PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_dsp_y_prefetch_operation PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_power_save_mode PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_page_reg PARAMS
   ((const struct pic30_operand_value * check));
unsigned char pic30_match_byte_8bit_lit PARAMS
   ((const struct pic30_operand_value * check));

unsigned long pic30_insert_34bit_lit PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_dividend_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_file_reg_word_with_dst PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_g_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_h_dst_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_p_src_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_q_dst_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_lo_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_dsp_writeback PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_dsp_x_prefetch_operation PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_dsp_y_prefetch_operation PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_byte_cp_8bit_lit PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_byte_l_operand PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_page_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));
unsigned long pic30_insert_wmul_dst_reg PARAMS ((unsigned long insn,
   unsigned long flags,
   const struct pic30_operand * opnd,
   const struct pic30_operand_value * operand_value,
   char **error_msg));

char * pic30_extract_file_reg_word_with_dst PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
unsigned long pic30_get_g_or_h_mode_value PARAMS (( unsigned long type,
   char **error_msg));
char * pic30_generate_g_or_h_operand PARAMS (( unsigned long insn,
   long reg_num, long mode, unsigned long flags, unsigned char * err));
unsigned long pic30_insert_g_or_h_displacement_value PARAMS (( unsigned long insn,
   unsigned long flags, long mode, long value));
char * pic30_extract_g_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info, unsigned long flags,
   const struct pic30_operand * opnd, unsigned char * err));
char * pic30_extract_h_dst_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info, unsigned long flags,
   const struct pic30_operand * opnd, unsigned char * err));
unsigned long pic30_get_p_or_q_mode_value PARAMS (( unsigned long type,
   char **error_msg));
char * pic30_generate_p_or_q_operand PARAMS (( long reg_num, long mode,
   unsigned char * err));

char * pic30_extract_p_src_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info, unsigned long flags,
   const struct pic30_operand * opnd, unsigned char * err));
char * pic30_extract_q_dst_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info, unsigned long flags,
   const struct pic30_operand * opnd, unsigned char * err));
char * pic30_extract_lo_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_byte_10bit_lit PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_w_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_offset PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_branch_on_condition_operand PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_branch_on_dsp_condition_operand PARAMS
   ((unsigned long insn,
     struct disassemble_info * info,
     unsigned long flags, const struct pic30_operand * opnd,
     unsigned char * err));
char * pic30_extract_dsp_accumulator_select PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_writeback PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_x_prefetch_operation PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_prefetch_dst PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_y_prefetch_operation PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_multiplicand PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_dsp_square PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_byte_cp_8bit_lit PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_page_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));
char * pic30_extract_wmul_dst_reg PARAMS ((unsigned long insn,
   struct disassemble_info * info,
   unsigned long flags, const struct pic30_operand * opnd,
   unsigned char * err));


/******************************************************************************
 * pic30_operator_mask[]
 ******************************************************************************/

const unsigned long pic30_operator_mask[] =
{
   OPND_UNKNOWN,                            /* O_illegal */
   OPND_UNKNOWN,                            /* O_absent */
   OPND_CONSTANT,                           /* O_constant */
   OPND_SYMBOL,                             /* O_symbol */
   OPND_SYMBOL,                             /* O_symbol_rva */
   OPND_REGISTER_DIRECT,                    /* O_register */
   OPND_UNKNOWN,                            /* O_big */
   OPND_EXPRESSION,                         /* O_uminus */
   OPND_EXPRESSION,                         /* O_bit_not */
   OPND_EXPRESSION,                         /* O_logical_not */
   OPND_EXPRESSION,                         /* O_multiply */
   OPND_EXPRESSION,                         /* O_divide */
   OPND_EXPRESSION,                         /* O_modulus */
   OPND_EXPRESSION,                         /* O_left_shift */
   OPND_EXPRESSION,                         /* O_right_shift */
   OPND_EXPRESSION,                         /* O_bit_inclusive_or */
   OPND_EXPRESSION,                         /* O_bit_or_not */
   OPND_EXPRESSION,                         /* O_bit_exclusive_or */
   OPND_EXPRESSION,                         /* O_bit_and */
   OPND_EXPRESSION,                         /* O_add */
   OPND_EXPRESSION,                         /* O_subtract */
   OPND_EXPRESSION,                         /* O_eq */
   OPND_EXPRESSION,                         /* O_ne */
   OPND_EXPRESSION,                         /* O_lt */
   OPND_EXPRESSION,                         /* O_le */
   OPND_EXPRESSION,                         /* O_ge */
   OPND_EXPRESSION,                         /* O_gt */
   OPND_EXPRESSION,                         /* O_logical_and */
   OPND_EXPRESSION,                         /* O_logical_or */
   OPND_UNKNOWN,                            /* O_index */
   OPND_REGISTER_INDIRECT,                  /* O_md1 */
   OPND_REGISTER_POST_DECREMENT,            /* O_md2 */
   OPND_REGISTER_POST_INCREMENT,            /* O_md3 */
   OPND_REGISTER_PRE_DECREMENT,             /* O_md4 */
   OPND_REGISTER_PRE_INCREMENT,             /* O_md5 */
   OPND_REGISTER_WITH_DISPLACEMENT,         /* O_md6 */
   OPND_REGISTER_PLUS_LITERAL,              /* O_md7 */
   OPND_REGISTER_MINUS_LITERAL,             /* O_md8 */
   OPND_REGISTER_POST_INCREMENT_BY_N,       /* O_md9 */
   OPND_REGISTER_POST_DECREMENT_BY_N,       /* O_md10 */
   OPND_W_REG,                              /* O_md11 */
   OPND_BRANCH_ON_DSP_CONDITION,            /* O_md12 */
   OPND_BRANCH_ON_CONDITION,                /* O_md13 */
   OPND_DSP_ACCUMULATOR_SELECT,             /* O_md14 */
   OPND_DSP_MULTIPLICAND,                   /* O_md15 */
   OPND_DSP_SQUARE,                         /* O_md16 */
   OPND_UNKNOWN,                            /* O_max */
};

/******************************************************************************
 * pic30_registers[]
 ******************************************************************************/
const char * pic30_registers[] =
{
   "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8", "w9", "w10", "w11",
   "w12", "w13", "w14", "w15",
   "W0", "W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8", "W9", "W10", "W11",
   "W12", "W13", "W14", "W15",
};

const int pic30_num_registers =
   (sizeof(pic30_registers) / sizeof(char *));

const char *pic30_page_registers[] = { "DSRPAG", "DSWPAG", "TBLPAG",
                                       "<not-a-pag>", 0 };

/******************************************************************************
 * pic30_sfr_symbols[]
 ******************************************************************************/
const struct pic30_predefined_symbol pic30_sfr_symbols[] =
{
   { "WREG0",  0x0000 },
   { "WREG1",  0x0002 },
   { "WREG2",  0x0004 },
   { "WREG3",  0x0006 },
   { "WREG4",  0x0008 },
   { "WREG5",  0x000A },
   { "WREG6",  0x000C },
   { "WREG7",  0x000E },
   { "WREG8",  0x0010 },
   { "WREG9",  0x0012 },
   { "WREG10", 0x0014 },
   { "WREG11", 0x0016 },
   { "WREG12", 0x0018 },
   { "WREG13", 0x001A },
   { "WREG14", 0x001C },
   { "WREG15", 0x001E },
   { "PC",     0x0020 }, /* 3 bytes */
   { "TBLPTR", 0x002C }, /* 2 bytes */
   { "STATUS", 0x0030 },
   { "DSPCON", 0x0032 },
   { "AD0DAT", 0x003C },
   { "AD0CON", 0x003E },
   { "ACCA",   0x0040 }, /* 3 bytes */
   { "ACCB",   0x0046 }, /* 3 bytes */
   { "TX",     0x004C },
   { "TXSTA",  0x004E },
   { "PRIMOD", 0x0050 },
   { "SECMOD", 0x0052 },
   { "INTCON", 0x0054 },
   { "PORTA",  0x0056 },
   { "PORTB",  0x0058 },
   { "PAGNUM", 0x005A },
   { "PRODH",  0x005C },
   { "PRODL",  0x005E },
   { "TABLAT", 0x0060 },
   { "TOS",    0x0062 },
   { "PCL",    0x0064 },
   { "PCH",    0x0065 },
   { "PCLATH", 0x0066 },
   { "PCLATU", 0x0068 },
   { "GIE",    0x006A },
   { "WDT",    0x006C },
};



const int pic30_num_sfr_symbols =
   (sizeof(pic30_sfr_symbols) / sizeof(struct pic30_predefined_symbol));

/******************************************************************************
 * pic30_relocation_info[]
 ******************************************************************************/

const struct relocation_info pic30_relocation_info[] =
{
#define PIC30_RELOC_INFO_NONE                                       (0)
   { 0, FALSE, 0, NULL },
#define PIC30_RELOC_INFO_FILE_REG_BYTE                              (1)
   { BFD_RELOC_PIC30_FILE_REG_BYTE, FALSE, 2, pic30_match_file_reg_byte },
#define PIC30_RELOC_INFO_FILE_REG                                   (2)
   { BFD_RELOC_PIC30_FILE_REG, FALSE, 2, pic30_match_file_reg },
#define PIC30_RELOC_INFO_FILE_REG_WORD                              (3)
   { BFD_RELOC_PIC30_FILE_REG_WORD, FALSE, 2, pic30_match_file_reg_word },
#define PIC30_RELOC_INFO_FILE_REG_WORD_WITH_DST                     (4)
   { BFD_RELOC_PIC30_FILE_REG_WORD_WITH_DST, FALSE, 2,
     pic30_match_file_reg_word },
#define PIC30_RELOC_INFO_UNSIGNED_4                                 (5)
   { BFD_RELOC_PIC30_UNSIGNED_4, FALSE, 1, pic30_match_nibble },
#define PIC30_RELOC_INFO_UNSIGNED_5                                 (6)
   { BFD_RELOC_PIC30_UNSIGNED_5, FALSE, 1, pic30_match_unsigned_short_lit },
#define PIC30_RELOC_INFO_PCREL_DO                                   (7)
   { BFD_RELOC_PIC30_PCREL_DO, TRUE, 2, NULL },
#define PIC30_RELOC_INFO_SIGNED_10_BYTE                             (8)
   {
	BFD_RELOC_PIC30_SIGNED_10_BYTE,		/* value */
	FALSE,					/* PC relative ? */
	2,					/* fix size (?) */
	pic30_match_byte_10bit_lit		/* is_match() */
   },
#define PIC30_RELOC_INFO_UNSIGNED_10                                (9)
   {
	BFD_RELOC_PIC30_UNSIGNED_10,		/* value */
	FALSE,					/* PC relative ? */
	2,					/* fix size (?) */
	pic30_match_10bit_lit			/* is_match() */
   },
#define PIC30_RELOC_INFO_UNSIGNED_14                                (10)
   { BFD_RELOC_PIC30_UNSIGNED_14, FALSE, 2, pic30_match_unsigned_14bit_lit },
#define PIC30_RELOC_INFO_FRAME_SIZE                                 (11)
   { BFD_RELOC_PIC30_FRAME_SIZE, FALSE, 2, pic30_match_frame_size },
#define PIC30_RELOC_INFO_WORD                                       (12)
   { BFD_RELOC_PIC30_WORD, FALSE, 2, pic30_match_16bit_lit },
#define PIC30_RELOC_INFO_PCREL_BRANCH                               (13)
   { BFD_RELOC_PIC30_PCREL_BRANCH, TRUE, 2, NULL },
#define PIC30_RELOC_INFO_PGM_ADDR_LSB                               (14)
   { BFD_RELOC_PIC30_PGM_ADDR_LSB, FALSE, 3, NULL },
#define PIC30_RELOC_INFO_PGM_ADDR_MSB                               (15)
   { BFD_RELOC_PIC30_PGM_ADDR_MSB, FALSE, 3, NULL },
#define PIC30_RELOC_INFO_BIT_SELECT_3                               (16)
   { BFD_RELOC_PIC30_BIT_SELECT_3, FALSE, 1, pic30_match_3bit_lit }, 
#define PIC30_RELOC_INFO_BIT_SELECT_NIBBLE_BYTE                     (17)
   { BFD_RELOC_PIC30_BIT_SELECT_4_BYTE, FALSE, 1, pic30_match_3bit_lit }, 
#define PIC30_RELOC_INFO_BIT_SELECT_4                               (18)
   { BFD_RELOC_PIC30_BIT_SELECT_4, FALSE, 1, pic30_match_nibble }, 
#define PIC30_RELOC_INFO_DSP_6                                      (19)
   { BFD_RELOC_PIC30_DSP_6, FALSE, 1, pic30_match_6bit_lit }, 
#define PIC30_RELOC_INFO_DSP_PRESHIFT                               (20)
   { BFD_RELOC_PIC30_DSP_PRESHIFT, FALSE, 1, pic30_match_4bit_lit }, 
#define PIC30_RELOC_INFO_PWRSAV_MODE                                (21)
   { BFD_RELOC_PIC30_PWRSAV_MODE, FALSE, 1, pic30_match_power_save_mode },
#define PIC30_RELOC_INFO_UNSIGNED_8                                 (22)
   { BFD_RELOC_PIC30_UNSIGNED_8, FALSE, 1, pic30_match_byte_10bit_lit },
#define PIC30_RELOC_INFO_UNSIGNED_15                                (23)
   { BFD_RELOC_PIC30_UNSIGNED_15, FALSE, 2, pic30_match_unsigned_15bit_lit },
#define PIC30_RELOC_INFO_NO_SHIFT_UNSIGNED_10                       (24)
   {
     BFD_RELOC_PIC30_NO_SHIFT_UNSIGNED_10,	/* value */
     FALSE,					/* PC relative ? */
     2,					        /* fix size (?) */
     pic30_match_10bit_lit			/* is_match() */
   },
#define PIC30_RELOC_INFO_PCREL_BRANCH_SLIT6                         (25)
   { BFD_RELOC_PIC30_PCREL_BRANCH_SLIT6, TRUE, 1, NULL },

};

/******************************************************************************
 * pic30_operands[]
 ******************************************************************************/

const struct pic30_operand pic30_operands[] =
{
#define REG                             (PIC30_BASE_OPERAND + 0)
   { 4, 0, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE, 0, 0, 0, 0 },

#define BASE_REG                        (PIC30_BASE_OPERAND + 1)
   { 4, 11, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE, 0, 0, 0, 0 },

#define MATH_BASE_REG                   (PIC30_BASE_OPERAND + 2)
   { 4, 15, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE, 0, 0, 0, 0 },

#define DST_REG                         (PIC30_BASE_OPERAND + 3)
   { 4, 7, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE, 0, 0, 0, 0 },

#define G_REG                           (PIC30_BASE_OPERAND + 4)
   { 4, 0, OPND_G_OR_H, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, pic30_insert_g_reg, pic30_extract_g_reg },

#define H_DST_REG                       (PIC30_BASE_OPERAND + 5)
   { 4, 7, OPND_G_OR_H, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, pic30_insert_h_dst_reg, pic30_extract_h_dst_reg },

#define P_SRC_REG                       (PIC30_BASE_OPERAND + 6)
   {
	4,				/* # if bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_P_OR_Q,			/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	pic30_insert_p_src_reg,		/* insert() */
	pic30_extract_p_src_reg		/* extract() */
   },

#define PIND_SRC_REG                    (PIC30_BASE_OPERAND + 7)
   {
	4,				/* # if bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_P_OR_Q_IND,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	pic30_insert_p_src_reg,		/* insert() */
	pic30_extract_p_src_reg		/* extract() */
   },

#define Q_DST_REG                       (PIC30_BASE_OPERAND + 8)
   {
	4,				/* # of bits in operand */
	7,				/* # of bits to shift for alignment */
	OPND_P_OR_Q,			/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	pic30_insert_q_dst_reg,		/* insert() */
	pic30_extract_q_dst_reg		/* extract() */
   },

#define QIND_DST_REG                    (PIC30_BASE_OPERAND + 9)
   {
	4,				/* # of bits in operand */
	7,				/* # of bits to shift for alignment */
	OPND_P_OR_Q_IND,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	pic30_insert_q_dst_reg,		/* insert() */
	pic30_extract_q_dst_reg		/* extract() */
   },

#define DBL_SRC_REG                     (PIC30_BASE_OPERAND + 10)
   {
	4,				/* # of bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_even,		/* is_match() */
        "Register # must be even.",     /* info_string */
	0,				/* insert() */
	0				/* extract() */
   },

#define DBL_DST_REG                     (PIC30_BASE_OPERAND + 11)
   {
	4,				/* # of bits in operand */
	7,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_even,		/* is_match */
        "Register # must be even.",     /* info_string */
	0,				/* insert() */
	0				/* extract() */
   },

#define DIVIDENDFR_REG                  (PIC30_BASE_OPERAND + 12)
   {
	4,				/* # of bits in operand	*/
	11,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	0,				/* insert() */
	0				/* extract() */
   },

#define DIVIDEND16_REG                  (PIC30_BASE_OPERAND + 13)
   {
	4,				/* # of bits in operand	*/
	7,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	0,				/* is_match() */
        0,                              /* info_string */
	0,				/* insert() */
	0				/* extract() */
   },

#define DIVIDEND32_REG                  (PIC30_BASE_OPERAND + 14)
   {
	4,				/* # of bits in operand	*/
	7,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_even,		/* is_match() */
        "Register # must be even.",     /* info_string */
	pic30_insert_dividend_reg,	/* insert() */
	0				/* extract() */
   },

#define DIVISOR_REG                     (PIC30_BASE_OPERAND + 15)
   {
	4,				/* # of bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_div_src_reg,	/* is_match() */
        "Register # must be between 2 and 15, inclusive.",
	0,				/* insert() */
	0				/* extract() */
   },

#define LO_SRC_REG                      (PIC30_BASE_OPERAND + 16)
   { 4, 0, OPND_LO_REGISTER, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, pic30_insert_lo_reg, pic30_extract_lo_reg },

#define LO_DST_REG                      (PIC30_BASE_OPERAND + 17)
   { 4, 7, OPND_LO_REGISTER, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, pic30_insert_lo_reg, pic30_extract_lo_reg },

#define FILE_REG_WORD                   (PIC30_BASE_OPERAND + 18)
   {
	16,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,     		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_FILE_REG_WORD,	/* default relocation type */
	pic30_match_file_reg_word,	/* is_match() */
        "Operand must be even and less than 65536.",
	0,                      	/* insert() */
	0				/* extract() */
   },

#define FILE_REG_WORD_WITH_DST          (PIC30_BASE_OPERAND + 19)
   {
	15,				/* # of bits in operand	*/
	4,				/* # of bits to shift for alignment */
	OPND_VALUE,	        	/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_FILE_REG_WORD_WITH_DST, /* default relocation type */
	pic30_match_file_reg_word,	/* is_match() */
        "Operand must be even and less than 65536.",
	pic30_insert_file_reg_word_with_dst, /* insert() */
	pic30_extract_file_reg_word_with_dst /* extract() */
   },

#define FILE_REG_BYTE                   (PIC30_BASE_OPERAND + 20)
   {
	13,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_FILE_REG_BYTE,	/* default relocation type */
	0,				/* is_match() */
    0, 				/* info_string */
	0,                       	/* insert() */
	0				/* extract() */
   },

#define FILE_REG                        (PIC30_BASE_OPERAND + 21)
   {
	13,				/* # of bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_FILE_REG,      /* default relocation type */
	pic30_match_even,   	/* is_match() */
        "Operand must be even."
        " Specify .b to access data memory at an odd address.",
	0,				/* insert() */
	0				/* extract() */
   },

#define W_REG                           (PIC30_BASE_OPERAND + 22)
   { 1, 13, OPND_W_REG, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_w_reg },

#define BYTE_LITERAL_10BIT              (PIC30_BASE_OPERAND + 23)
   {
	8,				/* # of bits in operand */
	4,				/* # of bits to shift for alignment */
	OPND_VALUE,			/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_SIGNED_10_BYTE,/* default relocation type */
	pic30_match_byte_10bit_lit,	/* is_match() */
        "Operand must be between -128 and 255, inclusive.",
	0,				/* insert() */
	pic30_extract_byte_10bit_lit	/* extract() */
   },

#define LITERAL_10BIT                   (PIC30_BASE_OPERAND + 24)
   {
	10,				/* # of bits in operand */
	4,				/* # of bits to shift for alignment */
	OPND_VALUE,			/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_10,	/* default relocation type */
	pic30_match_10bit_lit,		/* is_match() */
        "Operand must be between 0 and 1023, inclusive.",
	0,				/* insert() */
	0				/* extract() */
   },

#define LITERAL_16BIT                   (PIC30_BASE_OPERAND + 25)
   {
	16,				/* # of bits in operand	*/
	4,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_WORD,		/* default relocation type */
	pic30_match_16bit_lit,        	/* is_match() */
        "Operand must be between -32768 and 65535, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define SHIFT_LITERAL                   (PIC30_BASE_OPERAND + 26)
   {
	4,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_4,	/* default relocation type */
	pic30_match_nibble,        	/* is_match() */
        "Operand must be between 0 and 15, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define UNSIGNED_SHORT_LITERAL          (PIC30_BASE_OPERAND + 27)
   {
	5,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_5,	/* default relocation type */
	pic30_match_unsigned_short_lit,	/* is_match() */
        "Operand must be between 0 and 31, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define BYTE_BIT_SELECT_NIBBLE          (PIC30_BASE_OPERAND + 28)
   {
	4,				/* # of bits in operand	*/
	12,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_BIT_SELECT_NIBBLE_BYTE, /* default relocation type */
	pic30_match_3bit_lit,        	/* is_match() */
        "Operand must be between 0 and 7, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define BIT_SELECT_NIBBLE               (PIC30_BASE_OPERAND + 29)
   {
	4,				/* # of bits in operand	*/
	12,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_BIT_SELECT_4,	/* default relocation type */
	pic30_match_nibble,	        	/* is_match() */
        "Operand must be between 0 and 15, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define BIT_SELECT_3                    (PIC30_BASE_OPERAND + 30)
   {
	3,				/* # of bits in operand	*/
	13,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_BIT_SELECT_3,	/* default relocation type */
	pic30_match_3bit_lit,        	/* is_match() */
        "Operand must be between 0 and 7, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define BIT_SELECT_34                   (PIC30_BASE_OPERAND + 31)
   {
	3,				/* # of bits in operand */
	13,				/* # of bits to shift for alignment */
	OPND_CONSTANT,			/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_34bit_lit,		/* is_match() */
        "Operand must be between 0 and 15, inclusive.",
	pic30_insert_34bit_lit,		/* insert() */
	0				/* extract() */
   },

#define BRANCH_LABEL                    (PIC30_BASE_OPERAND + 32)
   { 16, 0, OPND_VALUE, FALSE, PIC30_RELOC_INFO_PCREL_BRANCH,
     0, 0, 0, pic30_extract_offset },

#define BRANCH_ON_DSP_CONDITION_OPERAND (PIC30_BASE_OPERAND + 33)
   { 2, 16, OPND_BRANCH_ON_DSP_CONDITION, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_branch_on_dsp_condition_operand },

#define BRANCH_ON_CONDITION_OPERAND     (PIC30_BASE_OPERAND + 34)
   { 4, 16, OPND_BRANCH_ON_CONDITION, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_branch_on_condition_operand },

#define LITERAL_14BIT                   (PIC30_BASE_OPERAND + 35)
   {
	14,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_14,	/* default relocation type */
	pic30_match_unsigned_14bit_lit,	/* is_match() */
        "Operand must be between 0 and 16383, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define FRAME_SIZE                      (PIC30_BASE_OPERAND + 36)
   {
	14,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_FRAME_SIZE,	/* default relocation type */
	pic30_match_frame_size,       	/* is_match() */
        "Operand must be even and between 0 and 16382, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define CALL_OPERAND                    (PIC30_BASE_OPERAND + 37)
   { 16, 0, OPND_VALUE, FALSE, PIC30_RELOC_INFO_PGM_ADDR_LSB, 0, 0, 0, 0 },

#define DO_LABEL                        (PIC30_BASE_OPERAND + 38)
   { 16, 0, OPND_VALUE, FALSE, PIC30_RELOC_INFO_PCREL_DO,
     0, 0, 0, pic30_extract_offset },
   
#define DSP_ACCUMULATOR_SELECT          (PIC30_BASE_OPERAND + 39)
   { 1,  15, OPND_DSP_ACCUMULATOR_SELECT, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_dsp_accumulator_select },

#define DSP_PRESHIFT                    (PIC30_BASE_OPERAND + 40)
   {
	4,				/* # of bits in operand	*/
	7,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_DSP_PRESHIFT,	/* default relocation type */
	pic30_match_4bit_lit,       	/* is_match() */
        "Operand must be between -8 and 7, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define DSP_SHORT_LITERAL               (PIC30_BASE_OPERAND + 41)
   {
	6,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_DSP_6,		/* default relocation type */
	pic30_match_6bit_lit,        	/* is_match() */
        "Operand must be between -16 and 16, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },

#define DSP_WRITEBACK                   (PIC30_BASE_OPERAND + 42)
   { 2, 0, OPND_DSP_WRITEBACK, FALSE, PIC30_RELOC_INFO_NONE,
     pic30_match_dsp_writeback, 0,
     pic30_insert_dsp_writeback, pic30_extract_dsp_writeback },

#define DSP_X_PREFETCH_OPERATION        (PIC30_BASE_OPERAND + 43)
   { 4, 6, OPND_DSP_PREFETCH_OPERATION, FALSE, PIC30_RELOC_INFO_NONE,
     pic30_match_dsp_x_prefetch_operation, 0,
     pic30_insert_dsp_x_prefetch_operation,
     pic30_extract_dsp_x_prefetch_operation },

#define DSP_X_PREFETCH_DST              (PIC30_BASE_OPERAND + 44)
   { 2, 12, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE,
     pic30_match_dsp_prefetch_dst, 0, 0, pic30_extract_dsp_prefetch_dst },

#define DSP_Y_PREFETCH_OPERATION        (PIC30_BASE_OPERAND + 45)
   { 4, 2, OPND_DSP_PREFETCH_OPERATION, FALSE, PIC30_RELOC_INFO_NONE,
     pic30_match_dsp_y_prefetch_operation, 0,
     pic30_insert_dsp_y_prefetch_operation,
     pic30_extract_dsp_y_prefetch_operation },

#define DSP_Y_PREFETCH_DST              (PIC30_BASE_OPERAND + 46)
   { 2, 10, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE,
     pic30_match_dsp_prefetch_dst, 0, 0, pic30_extract_dsp_prefetch_dst },

#define DSP_MULTIPLICAND                (PIC30_BASE_OPERAND + 47)
   { 3, 16, OPND_DSP_MULTIPLICAND, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_dsp_multiplicand },

#define DSP_SQUARE                      (PIC30_BASE_OPERAND + 48)
   { 2, 16, OPND_DSP_SQUARE, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_dsp_square },

#define PWRSAV_MODE                     (PIC30_BASE_OPERAND + 49)
   {
	1,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_PWRSAV_MODE,   /* default relocation type */
	pic30_match_power_save_mode,   	/* is_match() */
        "Operand must be 0 or 1.",      /* info_string */
	0,      	                /* insert() */
	0				/* extract() */
   },

#define DBL_MUL_DST_REG                 (PIC30_BASE_OPERAND + 50)
   {
	4,				/* # of bits in operand */
	7,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_even_not_14,	/* is_match */
	"Register # must be a member of [0, 2, 4, ... 12].", /* info_string */
	0,				/* insert() */
	0				/* extract() */
   },
   
#define BYTE_LITERAL_CP_8BIT              (PIC30_BASE_OPERAND + 51)
   {
	8,				/* # of bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,			/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_8 ,   /* default relocation type */
	pic30_match_byte_8bit_lit,	/* is_match() */
        "Operand must be between 0 and 255, inclusive.",
	pic30_insert_byte_cp_8bit_lit, 	/* insert() */
	pic30_extract_byte_cp_8bit_lit	/* extract() */
   },
#define REG_L                             (PIC30_BASE_OPERAND + 52)
   { 4, 0, OPND_REGISTER_DIRECT, FALSE, PIC30_RELOC_INFO_NONE, pic30_match_even, 0, pic30_insert_byte_l_operand, 0 },

#define LITERAL_15BIT                   (PIC30_BASE_OPERAND + 53)
   {
	15,				/* # of bits in operand	*/
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,      		/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_UNSIGNED_15,	/* default relocation type */
	pic30_match_unsigned_15bit_lit,	/* is_match() */
        "Operand must be between 0 and 32767, inclusive.",
	0,      	                /* insert() */
	0				/* extract() */
   },
#define PAGE_REG                        (PIC30_BASE_OPERAND + 54)
   {
     
	2,				/* # of bits in operand	*/
	10,				/* # of bits to shift for alignment */
	OPND_PAGE_SYMBOL,      		/* operand type */
	FALSE,				/* immediate operand ? */
       	PIC30_RELOC_INFO_NONE ,   	/* default relocation type */
	pic30_match_page_reg,   	/* is_match() */
        "Operand must be between 0 and 16383, inclusive.",
	pic30_insert_page_reg,      	                /* insert() */
	pic30_extract_page_reg				/* extract() */
   },
#define LITERAL_10BIT_NO_SHIFT          (PIC30_BASE_OPERAND + 55)
   {
	10,				/* # of bits in operand */
	0,				/* # of bits to shift for alignment */
	OPND_VALUE,			/* operand type */
	TRUE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NO_SHIFT_UNSIGNED_10,	/* default relocation type */
	pic30_match_10bit_lit,		/* is_match() */
        "Operand must be between 0 and 1023, inclusive.",
	0,				/* insert() */
	0				/* extract() */
   },
   

#define ECORE_ACCUMULATOR_SELECT          (PIC30_BASE_OPERAND + 56)
   { 1,  7, OPND_DSP_ACCUMULATOR_SELECT, FALSE, PIC30_RELOC_INFO_NONE,
     0, 0, 0, pic30_extract_dsp_accumulator_select },

#define W_MUL_DST_REG                 (PIC30_BASE_OPERAND + 57)
   {
	3,				/* # of bits in operand */
	8,				/* # of bits to shift for alignment */
	OPND_REGISTER_DIRECT,		/* operand type */
	FALSE,				/* immediate operand ? */
	PIC30_RELOC_INFO_NONE,		/* default relocation type */
	pic30_match_even_not_14,	/* is_match */
	"Register # must be a member of [0, 2, 4, ... 12].", /* info_string */
	pic30_insert_wmul_dst_reg,				/* insert() */
	pic30_extract_wmul_dst_reg	/* extract() */
   },

#define BRANCH_LABEL_SLIT6                    (PIC30_BASE_OPERAND + 58)
   { 6, 4 , OPND_VALUE, FALSE, PIC30_RELOC_INFO_PCREL_BRANCH_SLIT6,
     0, 0, 0, pic30_extract_offset },
 
};
const int pic30_num_operands =
   (sizeof(pic30_operands) / sizeof(struct pic30_operand));

/******************************************************************************
 *
 * pic30_opcodes[]
 *
 * NOTE:  Opcodes with the same name MUST be grouped together
 *
 ******************************************************************************/

/* REV_A enables support for dsPIC Rev. A silicon */
#define REV_A 1

const struct pic30_opcode pic30_opcodes[] =
{
   /***************************************************************************
    * ADD.b
    ***************************************************************************/
   { "add.b",     ADDWFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "add.b",     ADDWFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "add.b",     ADDLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "add.b",     ADDLS_B,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "add.b",     ADD_B,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * ADD.w
    ***************************************************************************/
   { "add.w",     ADDWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "add.w",     ADDWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "add.w",     ADDLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "add.w",     ADDLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "add.w",     ADD_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * ADD
    ***************************************************************************/
   { "add",       DUMMY,      0, { 0 /* for flags */ }, F_WORD |
                                                      F_HAS_IMPLIED_WREG |
                                                      F_IS_DSP_INSN },
   { "add",       ADDWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "add",       ADDWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "add",       ADDLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "add",       ADDLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "add",       ADD_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },
   { "add",       ADDAB,      1, { DSP_ACCUMULATOR_SELECT }, F_WORD |
                                                             F_IS_DSP_INSN },
   { "add",       ADDAC_PS,   3, { G_REG,
                                   DSP_PRESHIFT,
                                   DSP_ACCUMULATOR_SELECT}, F_WORD |
                                                            F_IS_DSP_INSN },
   { "add",       ADDAC,      2, { G_REG,
                                   DSP_ACCUMULATOR_SELECT }, F_WORD |
                                                             F_IS_DSP_INSN },

   /***************************************************************************
    * ADDC.b
    ***************************************************************************/
   { "addc.b",    ADDCWFF_B,  1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "addc.b",    ADDCWFW_B,  2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "addc.b",    ADDCLW_B,   2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "addc.b",    ADDCLS_B,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "addc.b",    ADDC_B,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * ADDC.w
    ***************************************************************************/
   { "addc.w",    ADDCWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "addc.w",    ADDCWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "addc.w",    ADDCLW_W,   2, { LITERAL_10BIT, REG }, F_WORD },
   { "addc.w",    ADDCLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "addc.w",    ADDC_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * ADDC
    ***************************************************************************/
   { "addc",      ADDCWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "addc",      ADDCWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "addc",      ADDCLW_W,   2, { LITERAL_10BIT, REG }, F_WORD },
   { "addc",      ADDCLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "addc",      ADDC_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * AND.b
    ***************************************************************************/
   { "and.b",     ANDWFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "and.b",     ANDWFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "and.b",     ANDLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "and.b",     ANDLS_B,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "and.b",     AND_B,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * AND.w
    ***************************************************************************/
   { "and.w",     ANDWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "and.w",     ANDWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "and.w",     ANDLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "and.w",     ANDLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "and.w",     AND_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * AND
    ***************************************************************************/
   { "and",       ANDWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "and",       ANDWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "and",       ANDLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "and",       ANDLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "and",       AND_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * ASR.b
    ***************************************************************************/
   { "asr.b",     ASRFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "asr.b",     ASRFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "asr.b",     ASR_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * ASR.w
    ***************************************************************************/
   { "asr.w",     ASRFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "asr.w",     ASRFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "asr.w",     ASR_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "asr.w",     ASRW_W,     3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "asr.w",     ASRK_W,     3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * ASR
    ***************************************************************************/
   { "asr",       ASRFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "asr",       ASRFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "asr",       ASR_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "asr",       ASRW_W,     3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "asr",       ASRK_W,     3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * BCLR
    ***************************************************************************/
   { "bclr.w",    BCLR_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "bclr.w",    BCLRF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "bclr",      BCLR_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "bclr",      BCLRF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "bclr.b",    BCLRF_B,    2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   { "bclr.b",    BCLR_B,     2, { P_SRC_REG,
                                   BYTE_BIT_SELECT_NIBBLE }, F_NONE },

   /***************************************************************************
    * BRA
    ***************************************************************************/
   { "bra",       BRA_DSP,    2, { BRANCH_ON_DSP_CONDITION_OPERAND,
                                   BRANCH_LABEL }, F_HAS_BRANCH_FLAG |
                                                   F_CANNOT_FOLLOW_REPEAT |
                                                   F_IS_DSP_INSN },
   { "bra",       BRA,        1, { BRANCH_LABEL }, F_CANNOT_FOLLOW_REPEAT },
   { "bra",       BRA_CC,     2, { BRANCH_ON_CONDITION_OPERAND,
                                   BRANCH_LABEL }, F_HAS_BRANCH_FLAG |
                                                   F_CANNOT_FOLLOW_REPEAT },
   { "bra",      BRAWE,      1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },
   { "bra",      BRAW,       1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_FCORE },
  
   /***************************************************************************
    * BSET
    ***************************************************************************/
   { "bset.w",    BSET_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "bset.w",    BSETF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "bset",      BSET_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "bset",      BSETF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "bset.b",    BSETF_B,    2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   { "bset.b",    BSET_B,     2, { P_SRC_REG,
                                   BYTE_BIT_SELECT_NIBBLE }, F_NONE },

   /***************************************************************************
    * BSW
    ***************************************************************************/
   { "bsw.z",     BSW_Z,      2, { P_SRC_REG, BASE_REG }, F_WORD },
   { "bsw.c",     BSW_C,      2, { P_SRC_REG, BASE_REG }, F_WORD },
   { "bsw",       BSW_Z,      2, { P_SRC_REG, BASE_REG }, F_WORD },

   /***************************************************************************
    * BTG
    ***************************************************************************/
   { "btg.w",     BTG_W,      2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btg.w",     BTGF_W,     2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btg",       BTG_W,      2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btg",       BTGF_W,     2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btg.b",     BTGF_B,     2, { FILE_REG_BYTE, BIT_SELECT_3 },  F_NONE },
   { "btg.b",     BTG_B,      2, { P_SRC_REG,
                                   BYTE_BIT_SELECT_NIBBLE }, F_NONE },

   /***************************************************************************
    * BTSC
    ***************************************************************************/
   { "btsc.w",    BTSC_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btsc.w",    BTSCF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btsc",      BTSC_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btsc",      BTSCF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btsc.b",    BTSCF_B,    2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   /***************************************************************************
    * BTSS
    ***************************************************************************/
   { "btss.w",    BTSS_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btss.w",    BTSSF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btss",      BTSS_W,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btss",      BTSSF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btss.b",    BTSSF_B,    2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   /***************************************************************************
    * BTST
    ***************************************************************************/
   { "btst.z",    BTST_Z,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btst.z",    BTSTW_Z,    2, { P_SRC_REG, BASE_REG }, F_WORD },
   { "btst.c",    BTST_C,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btst.c",    BTSTW_C,    2, { P_SRC_REG, BASE_REG }, F_WORD },
   { "btst",      BTST_Z,     2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btst",      BTSTW_Z,    2, { P_SRC_REG, BASE_REG }, F_WORD },
   { "btst",      BTSTF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btst.w",    BTSTF_W,    2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btst.b",    BTSTF_B,    2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   /***************************************************************************
    * BTSTS
    ***************************************************************************/
   { "btsts.z",   BTSTS_Z,    2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btsts.c",   BTSTS_C,    2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btsts",     BTSTS_Z,    2, { P_SRC_REG, BIT_SELECT_NIBBLE }, F_WORD },
   { "btsts",     BTSTSF_W,   2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btsts.w",   BTSTSF_W,   2, { FILE_REG_BYTE, BIT_SELECT_34 }, F_NONE },
   { "btsts.b",   BTSTSF_B,   2, { FILE_REG_BYTE, BIT_SELECT_3 }, F_NONE },
   /***************************************************************************
    * CALL
    ***************************************************************************/
   { "call",      CALLW,      1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_NONE },
   { "call",      CALL,       1, { CALL_OPERAND }, F_CANNOT_FOLLOW_REPEAT },
   { "call.l",    CALL_L,     1, { REG_L }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },

   /***************************************************************************
    * CLR.b
    ***************************************************************************/
   { "clr.b",     CLRW_B,     1, { W_REG }, F_HAS_IMPLIED_WREG },
   { "clr.b",     CLR_B,      1, { Q_DST_REG }, F_NONE },
   { "clr.b",     CLRF_B,     1, { FILE_REG_BYTE }, F_NONE },

   /***************************************************************************
    * CLR.w
    ***************************************************************************/
   { "clr.w",     CLRW_W,     1, { W_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "clr.w",     CLR_W,      1, { Q_DST_REG }, F_WORD },
   { "clr.w",     CLRF_W,     1, { FILE_REG }, F_WORD },

   /***************************************************************************
    * CLR
    ***************************************************************************/
   { "clr",       DUMMY,      0, { 0 /* for flags */ }, F_WORD |
                                                      F_HAS_IMPLIED_WREG |
                                                      F_IS_DSP_INSN },
   { "clr",       CLRW_W,     1, { W_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "clr",       CLR_W,      1, { Q_DST_REG }, F_WORD },
   { "clr",       CLRF_W,     1, { FILE_REG }, F_WORD },
   { "clr",       CLRAC_A,    1, { DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "clr",       CLRAC_WB,   2, { DSP_ACCUMULATOR_SELECT,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "clr",      CLRAC_X,    3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "clr",       CLRAC_Y,    3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "clr",       CLRAC_XWB,  4, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "clr",       CLRAC_YWB,  4, { DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "clr",       CLRAC_XY,   5, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "clr",       CLRAC,      6, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },

   /***************************************************************************
    * CLRWDT
    ***************************************************************************/
   { "clrwdt",    CLRWDT,     0, { 0 /* OPERANDS */ }, F_NONE },

   /***************************************************************************
    * COM.b
    ***************************************************************************/
   { "com.b",     COMFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "com.b",     COMFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "com.b",     COM_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * COM.w
    ***************************************************************************/
   { "com.w",     COMFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "com.w",     COMFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "com.w",     COM_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * COM
    ***************************************************************************/
   { "com",       COMFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "com",       COMFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "com",       COM_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * CP.b
    ***************************************************************************/
   { "cp.b",      CPLS_B,     2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_FCORE }, 
   { "cp.b",      CPLS8_B,    2, { BASE_REG, BYTE_LITERAL_CP_8BIT }, F_ECORE },
   { "cp.b",      CPF_B,      1, { FILE_REG_BYTE }, F_NONE },
   { "cp.b",      CP_B,       2, { BASE_REG, P_SRC_REG }, F_NONE },

   /***************************************************************************
    * CP.w
    ***************************************************************************/
   { "cp.w",      CPLS_W,     2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_WORD | F_FCORE },
   { "cp.w",      CPLS8_W,    2, { BASE_REG, BYTE_LITERAL_CP_8BIT }, F_WORD | F_ECORE },
   { "cp.w",      CPF_W,      1, { FILE_REG }, F_WORD },
   { "cp.w",      CP_W,       2, { BASE_REG, P_SRC_REG }, F_WORD },

   /***************************************************************************
    * CP
    ***************************************************************************/
   { "cp",        CPLS_W,     2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_WORD | F_FCORE },
   { "cp",        CPLS8_W,    2, { BASE_REG, BYTE_LITERAL_CP_8BIT }, F_WORD | F_ECORE },
   { "cp",        CPF_W,      1, { FILE_REG }, F_WORD },
   { "cp",        CP_W,       2, { BASE_REG, P_SRC_REG }, F_WORD },
   
   /***************************************************************************
    * CPB.b
    ***************************************************************************/   { "cpb.b",     CPBLS8_B,   2, { BASE_REG, BYTE_LITERAL_CP_8BIT }, F_ECORE },
   { "cpb.b",     CPBLS_B,    2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_FCORE }, 
   { "cpb.b",     CPBF_B,     1, { FILE_REG_BYTE }, F_NONE },
   { "cpb.b",     CPB_B,      2, { BASE_REG, P_SRC_REG }, F_NONE },

   /***************************************************************************
    * CPB.w
    ***************************************************************************/
   { "cpb.w",     CPBLS_W,    2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_WORD | F_FCORE },   
   { "cpb.w",     CPBLS8_W,   2, { BASE_REG,BYTE_LITERAL_CP_8BIT }, F_WORD | F_ECORE },
   { "cpb.w",     CPBF_W,     1, { FILE_REG }, F_WORD },
   { "cpb.w",     CPB_W,      2, { BASE_REG, P_SRC_REG }, F_WORD },

   /***************************************************************************
    * CPB
    ***************************************************************************/
   { "cpb",       CPBLS_W,    2, { BASE_REG, UNSIGNED_SHORT_LITERAL }, F_WORD | F_FCORE },   
   { "cpb",       CPBLS8_W,   2, { BASE_REG,BYTE_LITERAL_CP_8BIT }, F_WORD | F_ECORE },
   { "cpb",       CPBF_W,     1, { FILE_REG }, F_WORD },
   { "cpb",       CPB_W,      2, { BASE_REG, P_SRC_REG }, F_WORD },

   /***************************************************************************
    * CP0.b
    ***************************************************************************/
   { "cp0.b",     CP0F_B,     1, { FILE_REG_BYTE }, F_NONE },
   { "cp0.b",     CP0_B,      1, { P_SRC_REG }, F_NONE },

   /***************************************************************************
    * CP0.w
    ***************************************************************************/
   { "cp0.w",     CP0F_W,     1, { FILE_REG }, F_WORD },
   { "cp0.w",     CP0_W,      1, { P_SRC_REG }, F_WORD },

   /***************************************************************************
    * CP0
    ***************************************************************************/
   { "cp0",       CP0F_W,     1, { FILE_REG }, F_WORD },
   { "cp0",       CP0_W,      1, { P_SRC_REG }, F_WORD },

   /***************************************************************************
    * CPBEQ
    ***************************************************************************/
   { "cpbeq.b",   CPWBEQ_B,   3, { BASE_REG, REG,  BRANCH_LABEL_SLIT6  },
                                             F_ECORE | F_CANNOT_FOLLOW_REPEAT },
   { "cpbeq.w",   CPWBEQ_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpbeq",     CPWBEQ_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   /***************************************************************************
    * CPBGT
    ***************************************************************************/
   { "cpbgt.b",   CPWBGT_B,   3, { BASE_REG, REG,  BRANCH_LABEL_SLIT6  },
                                             F_ECORE | F_CANNOT_FOLLOW_REPEAT },
   { "cpbgt.w",   CPWBGT_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpbgt",     CPWBGT_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   /***************************************************************************
    * CPBLT
    ***************************************************************************/
   { "cpblt.b",   CPWBLT_B,   3, { BASE_REG, REG,  BRANCH_LABEL_SLIT6  },
                                             F_ECORE | F_CANNOT_FOLLOW_REPEAT },
   { "cpblt.w",   CPWBLT_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpblt",     CPWBLT_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   /***************************************************************************
    * CPBNEQ
    ***************************************************************************/
   { "cpbne.b",   CPWBNE_B,   3, { BASE_REG, REG,  BRANCH_LABEL_SLIT6  },
                                             F_ECORE | F_CANNOT_FOLLOW_REPEAT },
   { "cpbne.w",   CPWBNE_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpbne",     CPWBNE_W,   3, { BASE_REG, REG, BRANCH_LABEL_SLIT6  },
                                               F_ECORE | F_WORD |
                                               F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * CPSEQ
    ***************************************************************************/
   { "cpseq.b",   CPWSEQ_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT |                                                        F_FCORE },
   { "cpseq.w",   CPWSEQ_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpseq",     CPWSEQ_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   { "cpseq.b",   CPWSEQ_E_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT |
                                                     F_ECORE },
   { "cpseq.w",   CPWSEQ_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpseq",     CPWSEQ_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * CPSGT
    ***************************************************************************/
   { "cpsgt.b",   CPWSGT_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT | F_FCORE },
   { "cpsgt.w",   CPWSGT_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpsgt",     CPWSGT_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   { "cpsgt.b",   CPWSGT_E_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },
   { "cpsgt.w",   CPWSGT_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpsgt",     CPWSGT_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * CPSLT
    ***************************************************************************/
   { "cpslt.b",   CPWSLT_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT |F_FCORE },
   { "cpslt.w",   CPWSLT_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpslt",     CPWSLT_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   { "cpslt.b",   CPWSLT_E_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT |F_ECORE },
   { "cpslt.w",   CPWSLT_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpslt",     CPWSLT_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * CPSNE
    ***************************************************************************/
   { "cpsne.b",   CPWSNE_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT | F_FCORE },
   { "cpsne.w",   CPWSNE_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpsne",     CPWSNE_W,   2, { BASE_REG, REG }, F_WORD | F_FCORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   { "cpsne.b",   CPWSNE_E_B,   2, { BASE_REG, REG }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },
   { "cpsne.w",   CPWSNE_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },
   { "cpsne",     CPWSNE_E_W,   2, { BASE_REG, REG }, F_WORD | F_ECORE |
                                               F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * DAW
    ***************************************************************************/
   { "daw.b",     DAW_B,      1, { REG }, F_NONE },

   /***************************************************************************
    * DEC.b
    ***************************************************************************/
   { "dec.b",     DECFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "dec.b",     DECFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "dec.b",     DEC_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * DEC.w
    ***************************************************************************/
   { "dec.w",     DECFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "dec.w",     DECFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "dec.w",     DEC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * DEC
    ***************************************************************************/
   { "dec",       DECFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "dec",       DECFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "dec",       DEC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * DEC2.b
    ***************************************************************************/
   { "dec2.b",    DEC2FF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "dec2.b",    DEC2FW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "dec2.b",    DEC2_B,     2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * DEC2.w
    ***************************************************************************/
   { "dec2.w",    DEC2FF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "dec2.w",    DEC2FW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "dec2.w",    DEC2_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * DEC2
    ***************************************************************************/
   { "dec2",      DEC2FF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "dec2",      DEC2FW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "dec2",      DEC2_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * DISI
    ***************************************************************************/
   { "disi",      DISI,       1, { LITERAL_14BIT }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
   ** DIVF Wm,Wn	Fractional divide: Wm/Wn -> W0, Rem -> W1
   ****************************************************************************/
   { "divf",      DIVF,       2, { DIVIDENDFR_REG, DIVISOR_REG }, F_IS_DSP_INSN },

   /***************************************************************************
    * DIVS Wm,Wn	Signed integer divide: Wm/Wn -> W0, Rem -> W1
    ***************************************************************************/
   { "div.sd",    DIVS_D,     2, { DIVIDEND32_REG, DIVISOR_REG }, F_NONE},
   { "div.sw",    DIVS_W,     2, { DIVIDEND16_REG, DIVISOR_REG }, F_NONE},
   { "div.s",     DIVS_W,     2, { DIVIDEND16_REG, DIVISOR_REG }, F_NONE},

   /***************************************************************************
    * DIVU Wm,Wn	Unsigned integer divide: Wm/Wn -> W0, Rem -> W1
    ***************************************************************************/
   { "div.ud",    DIVU_D,     2, { DIVIDEND32_REG, DIVISOR_REG }, F_NONE},
   { "div.uw",    DIVU_W,     2, { DIVIDEND16_REG, DIVISOR_REG }, F_NONE},
   { "div.u",     DIVU_W,     2, { DIVIDEND16_REG, DIVISOR_REG }, F_NONE},

   /***************************************************************************
    * DO
    ***************************************************************************/
   { "do",        DOW,        2, { REG, DO_LABEL }, F_CANNOT_FOLLOW_REPEAT |
                                                    F_IS_DSP_INSN |
                                                    F_IS_2_WORD_INSN },
   { "do",        DO,         2, { LITERAL_14BIT,
                                   DO_LABEL }, F_CANNOT_FOLLOW_REPEAT |
                                               F_IS_DSP_INSN |
                                               F_IS_2_WORD_INSN | F_FCORE },
   { "do",        DOE,         2, { LITERAL_15BIT,
                                   DO_LABEL }, F_CANNOT_FOLLOW_REPEAT |
                                               F_IS_DSP_INSN |
                                               F_IS_2_WORD_INSN | F_ECORE },

   /***************************************************************************
    * ED
    ***************************************************************************/
   { "ed",        ED,         5, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },

   /***************************************************************************
    * EDAC
    ***************************************************************************/
   { "edac",      EDAC,       5, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },

   /***************************************************************************
    * EXCH
    ***************************************************************************/
   { "exch",      EXCH,       2, { REG, DST_REG }, F_NONE },

   /***************************************************************************
    * FBCL
    ***************************************************************************/
   { "fbcl",      FBCL,       2, { P_SRC_REG, DST_REG }, F_WORD },

   /***************************************************************************
    * FF1L
    ***************************************************************************/
   { "ff1l",      FF1L,       2, { P_SRC_REG, DST_REG }, F_WORD },

   /***************************************************************************
    * FF1R
    ***************************************************************************/
   { "ff1r",      FF1R,       2, { P_SRC_REG, DST_REG }, F_WORD },

   /***************************************************************************
    * GOTO
    ***************************************************************************/
   { "goto",      GOTOW,      1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_FCORE },
   { "goto",      GOTOWE,      1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },
   { "goto",      GOTO,       1, { CALL_OPERAND }, F_CANNOT_FOLLOW_REPEAT },
   { "goto.l",    GOTOW_L,      1, { REG_L }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },

   /***************************************************************************
    * INC.b
    ***************************************************************************/
   { "inc.b",     INCFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "inc.b",     INCFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "inc.b",     INC_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * INC.w
    ***************************************************************************/
   { "inc.w",     INCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "inc.w",     INCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "inc.w",     INC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * INC
    ***************************************************************************/
   { "inc",       INCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "inc",       INCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "inc",       INC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * INC2.b
    ***************************************************************************/
   { "inc2.b",    INC2FF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "inc2.b",    INC2FW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "inc2.b",    INC2_B,     2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * INC2.w
    ***************************************************************************/
   { "inc2.w",    INC2FF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "inc2.w",    INC2FW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "inc2.w",    INC2_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * INC2
    ***************************************************************************/
   { "inc2",      INC2FF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "inc2",      INC2FW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "inc2",      INC2_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * IOR.b
    ***************************************************************************/
   { "ior.b",     IORWFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "ior.b",     IORWFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "ior.b",     IORLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "ior.b",     IORLS_B,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "ior.b",     IOR_B,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * IOR.w
    ***************************************************************************/
   { "ior.w",     IORWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "ior.w",     IORWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "ior.w",     IORLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "ior.w",     IORLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "ior.w",     IOR_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * IOR
    ***************************************************************************/
   { "ior",       IORWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "ior",       IORWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "ior",       IORLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "ior",       IORLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "ior",       IOR_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * LAC
    ***************************************************************************/
   { "lac",       LAC_PS,     3, { G_REG,
                                   DSP_PRESHIFT,
                                   DSP_ACCUMULATOR_SELECT }, F_WORD |
                                                             F_IS_DSP_INSN },
   { "lac",       LAC,        2, { G_REG,
                                   DSP_ACCUMULATOR_SELECT }, F_WORD |
                                                             F_IS_DSP_INSN },

   /***************************************************************************
    * LNK
    ***************************************************************************/
   { "lnk",       LNK,        1, { FRAME_SIZE }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * LSR.b
    ***************************************************************************/
   { "lsr.b",     LSRFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "lsr.b",     LSRFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "lsr.b",     LSR_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * LSR.w
    ***************************************************************************/
   { "lsr.w",     LSRFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "lsr.w",     LSRFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "lsr.w",     LSR_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "lsr.w",     LSRW_W,     3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "lsr.w",     LSRK_W,     3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * LSR
    ***************************************************************************/
   { "lsr",       LSRFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "lsr",       LSRFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "lsr",       LSR_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "lsr",       LSRW_W,     3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "lsr",       LSRK_W,     3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * MPY - Must be before MAC; otherwise, MAC is chosen during disassembly.
    ***************************************************************************/
   { "mpy",       MPY_A,      2, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "mpy",       MPY_X,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy",       MPY_Y,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy",       MPY,        6, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy",       SQR_A,      2, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "mpy",       SQR_X,      4, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy",       SQR_Y,      4, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy",       SQR,        6, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },

   /***************************************************************************
    * MPY.N
    ***************************************************************************/
   { "mpy.n",     MPYN_A,     2, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "mpy.n",     MPYN_X,     4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy.n",     MPYN_Y,     4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mpy.n",     MPYN,       6, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },

   /***************************************************************************
    * MOVSAC - Must be before MAC; otherwise, MAC is chosen during disasembly.
    ***************************************************************************/
   { "movsac",    MOVSAC_A,   1, { DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_WB,  2, { DSP_ACCUMULATOR_SELECT,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_X,   3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_Y,   3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_XWB, 4, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_YWB, 4, { DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC_XY,  5, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "movsac",    MOVSAC,     6, { DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },

   /***************************************************************************
    * MAC
    ***************************************************************************/
   { "mac",       MAC_A,      2, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "mac",       MAC_WB,     3, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "mac",       MAC_X,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mac",       MAC_Y,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mac",       MAC_XWB,    5, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "mac",       MAC_YWB,    5, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "mac",       MAC_XY,     6, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mac",       MAC,        7, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "mac",       SQRAC_A,    2, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "mac",       SQRAC_X,    4, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mac",       SQRAC_Y,    4, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "mac",       SQRAC,      6, { DSP_SQUARE,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },

   /***************************************************************************
    * MOV.b
    ***************************************************************************/
   { "mov.b",     MOVWF_B,    2, { W_REG,
                                   FILE_REG_BYTE }, F_HAS_REQUIRED_WREG },
   { "mov.b",     MOVLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "mov.b",     MOVFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "mov.b",     MOVFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "mov.b",     MOV_B,      2, { G_REG, H_DST_REG }, F_NONE },
   { "mov.b",     LDWLO_B,    2, { LO_SRC_REG, DST_REG }, F_NONE },
   { "mov.b",     STWLO_B,    2, { REG, LO_DST_REG }, F_NONE },

   /***************************************************************************
    * MOV.d
    ***************************************************************************/
   { "mov.d",     LDDW,       2, { PIND_SRC_REG,DBL_DST_REG }, F_WORD |
                                                       F_CANNOT_FOLLOW_REPEAT },
   { "mov.d",     STDW,       2, { DBL_SRC_REG,	QIND_DST_REG }, F_WORD |
                                                       F_CANNOT_FOLLOW_REPEAT },
   { "mov.d",     LDDW,       2, { DBL_SRC_REG,	DBL_DST_REG }, F_WORD |
                                                       F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * MOV.w
    ***************************************************************************/
   { "mov.w",     MOVWF_W,    2, { W_REG, FILE_REG }, F_WORD |
                                                      F_HAS_REQUIRED_WREG },
   { "mov.w",     MOVL_W,     2, { LITERAL_16BIT, REG }, F_WORD },
   { "mov.w",     MOVLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "mov.w",     MOVFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "mov.w",     MOVFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "mov.w",     MOV_W,      2, { G_REG, H_DST_REG }, F_WORD },
   { "mov.w",     LDWLO_W,    2, { LO_SRC_REG, DST_REG }, F_WORD },
   { "mov.w",     STWLO_W,    2, { REG, LO_DST_REG }, F_WORD },
   { "mov.w",     STW,        2, { REG, FILE_REG_WORD_WITH_DST }, F_WORD },
   { "mov.w",     LDW,        2, { FILE_REG_WORD_WITH_DST, REG }, F_WORD },

   /***************************************************************************
    * MOV
    ***************************************************************************/
   { "mov",       MOVWF_W,    2, { W_REG, FILE_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "mov",       MOVL_W,     2, { LITERAL_16BIT, REG }, F_WORD },
   { "mov",       MOVFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "mov",       MOVFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "mov",       MOV_W,      2, { G_REG, H_DST_REG }, F_WORD },
   { "mov",       LDWLO_W,    2, { LO_SRC_REG, DST_REG }, F_WORD },
   { "mov",       STWLO_W,    2, { REG, LO_DST_REG }, F_WORD },
   { "mov",       STW,        2, { REG, FILE_REG_WORD_WITH_DST }, F_WORD },
   { "mov",       LDW,        2, { FILE_REG_WORD_WITH_DST, REG }, F_WORD },
   { "movpag",   MOVPAG_W,   2, { REG, PAGE_REG}, F_ECORE },
   { "movpag",    MOVPAG,     2, { LITERAL_10BIT_NO_SHIFT, PAGE_REG}, F_ECORE },
  

   /***************************************************************************
    * MSC
    ***************************************************************************/
   { "msc",       MSC_A,      2, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },
   { "msc",       MSC_WB,     3, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "msc",       MSC_X,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST }, F_IS_DSP_INSN },
   { "msc",       MSC_Y,      4, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "msc",       MSC_XWB,    5, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "msc",       MSC_YWB,    5, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },
   { "msc",       MSC_XY,     6, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST }, F_IS_DSP_INSN },
   { "msc",       MSC,        7, { DSP_MULTIPLICAND,
                                   DSP_ACCUMULATOR_SELECT,
                                   DSP_X_PREFETCH_OPERATION,
                                   DSP_X_PREFETCH_DST,
                                   DSP_Y_PREFETCH_OPERATION,
                                   DSP_Y_PREFETCH_DST,
                                   DSP_WRITEBACK }, F_IS_DSP_INSN },

   /***************************************************************************
    * MUL
    ***************************************************************************/
   { "mul.b",     MULWF_B,      1, { FILE_REG_BYTE }, F_NONE },
   { "mul.w",     MULWF_W,      1, { FILE_REG }, F_WORD },
   { "mul",       MULWF_W,      1, { FILE_REG }, F_WORD },


   { "mul.ss",    MUL_SS_ACC,   3, { BASE_REG,
                                     P_SRC_REG,
                                     ECORE_ACCUMULATOR_SELECT },
                                     F_ECORE | F_IS_DSP_INSN },

   { "mul.ss",    MUL_SS,       3,  { BASE_REG,
                                      P_SRC_REG,
                                      DBL_MUL_DST_REG }, F_WORD },
 
   { "mulw.ss",   MULW_SS,      3, { BASE_REG,
                                     P_SRC_REG,
                                     W_MUL_DST_REG }, F_ECORE },
  
   { "mul.su",    MULLS_SU_ACC, 3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     ECORE_ACCUMULATOR_SELECT },                                                                      F_ECORE | F_IS_DSP_INSN },

   { "mul.su",    MUL_SU_ACC,   3, { BASE_REG,
                                     P_SRC_REG,
                                     ECORE_ACCUMULATOR_SELECT },
                                     F_ECORE | F_IS_DSP_INSN },

   { "mul.su",    MULLS_SU,     3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     DBL_MUL_DST_REG }, F_WORD }, 

   { "mul.su",    MUL_SU,       3, { BASE_REG,
                                     P_SRC_REG,
                                     DBL_MUL_DST_REG }, F_WORD },
 

   { "mulw.su",   MULLSW_SU,    3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     W_MUL_DST_REG }, F_ECORE }, 

   { "mulw.su",   MULW_SU,      3, { BASE_REG,
                                     P_SRC_REG,
                                     W_MUL_DST_REG }, F_ECORE },
  
  
   { "mul.uu",    MULLS_UU_ACC, 3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     ECORE_ACCUMULATOR_SELECT },                                                                      F_ECORE | F_IS_DSP_INSN },

   { "mul.uu",    MUL_UU_ACC,   3, { BASE_REG,
                                     P_SRC_REG,
                                     ECORE_ACCUMULATOR_SELECT },                                                                      F_ECORE | F_IS_DSP_INSN },
   
   { "mul.uu",    MULLS_UU,     3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     DBL_MUL_DST_REG }, F_WORD },

   { "mul.uu",    MUL_UU,       3, { BASE_REG,
                                     P_SRC_REG,
                                     DBL_MUL_DST_REG },F_WORD },

   { "mulw.uu",   MULLSW_UU,    3, { BASE_REG,
                                     UNSIGNED_SHORT_LITERAL,
                                     W_MUL_DST_REG }, F_ECORE },

   { "mulw.uu",   MULW_UU,      3, { BASE_REG,
                                     P_SRC_REG,
                                     W_MUL_DST_REG },F_ECORE },


   { "mul.us",    MUL_US_ACC,   3, { BASE_REG,
                                     P_SRC_REG,
                                     ECORE_ACCUMULATOR_SELECT  },                                                                                               F_ECORE | F_IS_DSP_INSN },

   { "mul.us",    MUL_US,       3, { BASE_REG,
                                     P_SRC_REG,
                                     DBL_MUL_DST_REG }, F_WORD },

   { "mulw.us",   MULW_US,      3, { BASE_REG,
                                     P_SRC_REG,
                                     W_MUL_DST_REG }, F_ECORE },

   /***************************************************************************
    * NEG.b
    ***************************************************************************/
   { "neg.b",     NEGFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "neg.b",     NEGFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "neg.b",     NEG_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * NEG.w
    ***************************************************************************/
   { "neg.w",     NEGFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "neg.w",     NEGFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "neg.w",     NEG_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * NEG
    ***************************************************************************/
   { "neg",       DUMMY,      0, { 0 /* for flags */ }, F_WORD |
                                                      F_HAS_IMPLIED_WREG |
                                                      F_IS_DSP_INSN },
   { "neg",       NEGFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "neg",       NEGFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "neg",       NEG_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "neg",       NEGAB,      1, { DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },

   /***************************************************************************
    * NOP
    ***************************************************************************/
   { "nop",       NOP,        0, { 0 /* OPERANDS */ }, F_NONE },
   { "nopr",      NOPR,       0, { 0 /* OPERANDS */ }, F_NONE },

   /***************************************************************************
    * POP
    ***************************************************************************/
   { "pop.w",     POP_W,      1, { H_DST_REG }, F_WORD },
   { "pop",       POP_W,      1, { H_DST_REG }, F_WORD },
   { "pop",       POPF,       1, { FILE_REG_WORD }, F_NONE },

   /***************************************************************************
    * POP.d
    ***************************************************************************/
   { "pop.d",     POP_D,      1, { DBL_DST_REG }, F_NONE },

   /***************************************************************************
    * POP.s
    ***************************************************************************/
   { "pop.s",     ITCH,       0, { 0 /* OPERANDS */ }, F_NONE },

   /***************************************************************************
    * PWRSAV
    ***************************************************************************/
   { "pwrsav",    PWRSAV,     1, { PWRSAV_MODE }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * PUSH
    ***************************************************************************/
   { "push.w",    PUSH_W,     1, { G_REG }, F_WORD },
   { "push",      PUSH_W,     1, { G_REG }, F_WORD },
   { "push",      PUSHF,      1, { FILE_REG_WORD }, F_NONE },

   /***************************************************************************
    * PUSH.d
    ***************************************************************************/
   { "push.d",    PUSH_D,     1, { DBL_SRC_REG }, F_NONE },

   /***************************************************************************
    * PUSH.s
    ***************************************************************************/
   { "push.s",    SCRATCH,    0, { 0 /* OPERANDS */ }, F_NONE },

   /***************************************************************************
    * RCALL
    ***************************************************************************/
   { "rcall",     RCALLW,     1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_FCORE },
   { "rcall",     RCALLWE,    1, { REG }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },
   { "rcall",     RCALL,      1, { BRANCH_LABEL }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * REPEAT
    ***************************************************************************/
   { "repeat",    REPEATW,    1, { REG }, F_CANNOT_FOLLOW_REPEAT },
   { "repeat",    REPEAT,     1, { LITERAL_14BIT }, F_CANNOT_FOLLOW_REPEAT| F_FCORE },
   { "repeat",    REPEATE,     1, { LITERAL_15BIT }, F_CANNOT_FOLLOW_REPEAT | F_ECORE },

   /***************************************************************************
    * RESET
    ***************************************************************************/
   { "reset",     RESET,      0, { 0 /* OPERANDS */ }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * RETFIE
    ***************************************************************************/
   { "retfie",    RETFIE,     0, { 0 /* OPERANDS */ }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * RETLW
    ***************************************************************************/
   { "retlw.b",   RETLW_B,    2, { BYTE_LITERAL_10BIT,
                                   REG }, F_CANNOT_FOLLOW_REPEAT },
   { "retlw.w",   RETLW_W,    2, { LITERAL_10BIT,
                                   REG }, F_WORD | F_CANNOT_FOLLOW_REPEAT },
   { "retlw",     RETLW_W,    2, { LITERAL_10BIT,
                                   REG }, F_WORD | F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * RETURN
    ***************************************************************************/
   { "return",    RETURN,     0, { 0 /* OPERANDS */ }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * RLC.b
    ***************************************************************************/
   { "rlc.b",     RLCFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "rlc.b",     RLCFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "rlc.b",     RLC_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * RLC.w
    ***************************************************************************/
   { "rlc.w",     RLCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rlc.w",     RLCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rlc.w",     RLC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RLC
    ***************************************************************************/
   { "rlc",       RLCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rlc",       RLCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rlc",       RLC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RLNC.b
    ***************************************************************************/
   { "rlnc.b",    RLNCFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "rlnc.b",    RLNCFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "rlnc.b",    RLNC_B,     2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * RLNC.w
    ***************************************************************************/
   { "rlnc.w",    RLNCFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rlnc.w",    RLNCFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rlnc.w",    RLNC_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RLNC
    ***************************************************************************/
   { "rlnc",      RLNCFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rlnc",      RLNCFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rlnc",      RLNC_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RRC.b
    ***************************************************************************/
   { "rrc.b",     RRCFF_B,    1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "rrc.b",     RRCFW_B,    2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "rrc.b",     RRC_B,      2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * RRC.w
    ***************************************************************************/
   { "rrc.w",     RRCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rrc.w",     RRCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rrc.w",     RRC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RRC
    ***************************************************************************/
   { "rrc",       RRCFF_W,    1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rrc",       RRCFW_W,    2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rrc",       RRC_W,      2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RRNC.b
    ***************************************************************************/
   { "rrnc.b",    RRNCFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "rrnc.b",    RRNCFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "rrnc.b",    RRNC_B,     2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * RRNC.w
    ***************************************************************************/
   { "rrnc.w",    RRNCFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rrnc.w",    RRNCFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rrnc.w",    RRNC_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * RRNC
    ***************************************************************************/
   { "rrnc",      RRNCFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "rrnc",      RRNCFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "rrnc",      RRNC_W,     2, { P_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SAC
    ***************************************************************************/
   { "sac",       SAC_PS,     3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_PRESHIFT,
                                   G_REG }, F_WORD | F_IS_DSP_INSN },
   { "sac",       SAC,        2, { DSP_ACCUMULATOR_SELECT,
                                   G_REG }, F_WORD | F_IS_DSP_INSN },

   /***************************************************************************
    * SRAC
    ***************************************************************************/
   { "sac.r",     SAC_R_PS,   3, { DSP_ACCUMULATOR_SELECT,
                                   DSP_PRESHIFT,
                                   G_REG }, F_WORD | F_IS_DSP_INSN },
   { "sac.r",     SAC_R,      2, { DSP_ACCUMULATOR_SELECT,
                                   G_REG }, F_WORD | F_IS_DSP_INSN },

   /***************************************************************************
    * SE
    ***************************************************************************/
   { "se",        SE_W,       2, { P_SRC_REG, DST_REG }, F_WORD },
   { "se.w",      SE_W,       2, { P_SRC_REG, DST_REG }, F_WORD },

   /***************************************************************************
    * SETM.b
    ***************************************************************************/
   { "setm.b",    SETW_B,     1, { W_REG }, F_HAS_IMPLIED_WREG },
   { "setm.b",    SET_B,      1, { Q_DST_REG }, F_NONE },
   { "setm.b",    SETF_B,     1, { FILE_REG_BYTE }, F_NONE },

   /***************************************************************************
    * SET.w
    ***************************************************************************/
   { "setm.w",    SETW_W,     1, { W_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "setm.w",    SET_W,      1, { Q_DST_REG }, F_WORD },
   { "setm.w",    SETF_W,     1, { FILE_REG }, F_WORD },

   /***************************************************************************
    * SET
    ***************************************************************************/
   { "setm",      SETW_W,     1, { W_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "setm",      SET_W,      1, { Q_DST_REG }, F_WORD },
   { "setm",      SETF_W,     1, { FILE_REG }, F_WORD },

   /***************************************************************************
    * SFTAC
    ***************************************************************************/
   { "sftac",     SFTACW,     2, { DSP_ACCUMULATOR_SELECT,
                                   REG }, F_WORD | F_IS_DSP_INSN },
   { "sftac",     SFTACK,     2, { DSP_ACCUMULATOR_SELECT,
                                   DSP_SHORT_LITERAL }, F_WORD |
                                                        F_IS_DSP_INSN },

   /***************************************************************************
    * SL.b
    ***************************************************************************/
   { "sl.b",      SLFF_B,     1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "sl.b",      SLFW_B,     2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "sl.b",      SL_B,       2, { P_SRC_REG, Q_DST_REG }, F_NONE },

   /***************************************************************************
    * SL.w
    ***************************************************************************/
   { "sl.w",      SLFF_W,     1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "sl.w",      SLFW_W,     2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "sl.w",      SL_W,       2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "sl.w",      SLW_W,      3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "sl.w",      SLK_W,      3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * SL
    ***************************************************************************/
   { "sl",        SLFF_W,     1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "sl",        SLFW_W,     2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "sl",        SL_W,       2, { P_SRC_REG, Q_DST_REG }, F_WORD },
   { "sl",        SLW_W,      3, { BASE_REG, REG, DST_REG }, F_WORD },
   { "sl",        SLK_W,      3, { BASE_REG, SHIFT_LITERAL, DST_REG }, F_WORD },

   /***************************************************************************
    * SUB.b
    ***************************************************************************/
   { "sub.b",     SUBWFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "sub.b",     SUBWFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "sub.b",     SUBLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   /* FIXME: The next instr is not valid in the current isa */
   { "sub.b",     SUBLW_B,    2, { REG, BYTE_LITERAL_10BIT }, F_NONE },
   { "sub.b",     SUBLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "sub.b",     SUBLS_B,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "sub.b",     SUB_B,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * SUB.w
    ***************************************************************************/
   { "sub.w",     SUBWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "sub.w",     SUBWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "sub.w",     SUBLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   /* FIXME: The next instr is not valid in the current isa */
   { "sub.w",     SUBLW_W,    2, { REG, LITERAL_10BIT }, F_WORD },
   { "sub.w",     SUBLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "sub.w",     SUB_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUB
    ***************************************************************************/
   { "sub",       DUMMY,      0, { 0 /* for flags */ }, F_WORD |
                                                      F_HAS_IMPLIED_WREG |
                                                      F_IS_DSP_INSN },
   { "sub",       SUBWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "sub",       SUBWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   /* FIXME: The next instr is not valid in the current isa */
   { "sub",       SUBLW_W,    2, { REG, LITERAL_10BIT }, F_WORD },
   { "sub",       SUBLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "sub",       SUBLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "sub",       SUB_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },
   { "sub",       SUBAB,      1, { DSP_ACCUMULATOR_SELECT }, F_IS_DSP_INSN },

   /***************************************************************************
    * SUBB.b
    ***************************************************************************/
   { "subb.b",    SUBBWFF_B,  1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "subb.b",    SUBBWFW_B,  2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "subb.b",    SUBBLW_B,   2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   /* FIXME: The next instr is not valid in the current isa */
   { "subb.b",    SUBBLW_B,   2, { REG, BYTE_LITERAL_10BIT }, F_NONE },
   { "subb.b",    SUBBLS_B,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "subb.b",    SUBB_B,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * SUBB.w
    ***************************************************************************/
   { "subb.w",    SUBBWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subb.w",    SUBBWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subb.w",    SUBBLW_W,   2, { LITERAL_10BIT, REG }, F_WORD },
   /* FIXME: The next instr is not valid in the current isa */
   { "subb.w",    SUBBLW_W,   2, { REG, LITERAL_10BIT }, F_WORD },
   { "subb.w",    SUBBLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subb.w",    SUBB_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUBB
    ***************************************************************************/
   { "subb",      SUBBWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subb",      SUBBWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subb",      SUBBLW_W,   2, { LITERAL_10BIT, REG }, F_WORD },
   /* FIXME: The next instr is not valid in the current isa */
   { "subb",      SUBBLW_W,   2, { REG, LITERAL_10BIT }, F_WORD },
   { "subb",      SUBBLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subb",      SUBB_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUBR.b
    ***************************************************************************/
   { "subr.b",    SUBRWFF_B,  1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "subr.b",    SUBRWFW_B,  2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "subr.b",    SUBRLS_B,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "subr.b",    SUBR_B,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * SUBR.w
    ***************************************************************************/
   { "subr.w",    SUBRWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subr.w",    SUBRWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subr.w",    SUBRLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subr.w",    SUBR_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUBR
    ***************************************************************************/
   { "subr",      SUBRWFF_W,  1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subr",      SUBRWFW_W,  2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subr",      SUBRLS_W,   3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subr",      SUBR_W,     3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUBBR.b
    ***************************************************************************/
   { "subbr.b",   SUBBRWFF_B, 1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "subbr.b",   SUBBRWFW_B, 2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "subbr.b",   SUBBRLS_B,  3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "subbr.b",   SUBBR_B,    3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * SUBBR.w
    ***************************************************************************/
   { "subbr.w",   SUBBRWFF_W, 1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subbr.w",   SUBBRWFW_W, 2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subbr.w",   SUBBRLS_W,  3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subbr.w",   SUBBR_W,    3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SUBBR
    ***************************************************************************/
   { "subbr",     SUBBRWFF_W, 1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "subbr",     SUBBRWFW_W, 2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "subbr",     SUBBRLS_W,  3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "subbr",     SUBBR_W,    3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * SWAP
    ***************************************************************************/
   { "swap.b",    SWAP_B,     1, { REG }, F_NONE },
   { "swap.w",    SWAP_W,     1, { REG }, F_WORD },
   { "swap",      SWAP_W,     1, { REG }, F_WORD },

   /***************************************************************************
    * TBLRDH
    ***************************************************************************/
   { "tblrdh.b",  TBLRDH_B,   2, { PIND_SRC_REG, Q_DST_REG }, F_NONE },
   { "tblrdh.w",  TBLRDH_W,   2, { PIND_SRC_REG, Q_DST_REG }, F_WORD },
   { "tblrdh",    TBLRDH_W,   2, { PIND_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * TBLRDL
    ***************************************************************************/
   { "tblrdl.b",  TBLRDL_B,   2, { PIND_SRC_REG, Q_DST_REG }, F_NONE },
   { "tblrdl.w",  TBLRDL_W,   2, { PIND_SRC_REG, Q_DST_REG }, F_WORD },
   { "tblrdl",    TBLRDL_W,   2, { PIND_SRC_REG, Q_DST_REG }, F_WORD },

   /***************************************************************************
    * TBLWTH
    ***************************************************************************/
   { "tblwth.b",  TBLWTH_B,   2, { P_SRC_REG, QIND_DST_REG }, F_NONE },
   { "tblwth.w",  TBLWTH_W,   2, { P_SRC_REG, QIND_DST_REG }, F_WORD },
   { "tblwth",    TBLWTH_W,   2, { P_SRC_REG, QIND_DST_REG }, F_WORD },

   /***************************************************************************
    * TBLWTL
    ***************************************************************************/
   { "tblwtl.b",  TBLWTL_B,   2, { P_SRC_REG, QIND_DST_REG }, F_NONE },
   { "tblwtl.w",  TBLWTL_W,   2, { P_SRC_REG, QIND_DST_REG }, F_WORD },
   { "tblwtl",    TBLWTL_W,   2, { P_SRC_REG, QIND_DST_REG }, F_WORD },

   /***************************************************************************
    * ULNK
    ***************************************************************************/
   { "ulnk",      ULNK,       0, { 0 /* OPERANDS */ }, F_CANNOT_FOLLOW_REPEAT },

   /***************************************************************************
    * XOR.b
    ***************************************************************************/
   { "xor.b",     XORWFF_B,   1, { FILE_REG_BYTE }, F_HAS_IMPLIED_WREG },
   { "xor.b",     XORWFW_B,   2, { FILE_REG_BYTE, W_REG }, F_HAS_IMPLIED_WREG },
   { "xor.b",     XORLW_B,    2, { BYTE_LITERAL_10BIT, REG }, F_NONE },
   { "xor.b",     XORLS_B,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_NONE },
   { "xor.b",     XOR_B,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_NONE },

   /***************************************************************************
    * XOR.w
    ***************************************************************************/
   { "xor.w",     XORWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "xor.w",     XORWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "xor.w",     XORLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "xor.w",     XORLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "xor.w",     XOR_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * XOR
    ***************************************************************************/
   { "xor",       XORWFF_W,   1, { FILE_REG }, F_WORD | F_HAS_IMPLIED_WREG },
   { "xor",       XORWFW_W,   2, { FILE_REG, W_REG }, F_WORD |
                                                      F_HAS_IMPLIED_WREG },
   { "xor",       XORLW_W,    2, { LITERAL_10BIT, REG }, F_WORD },
   { "xor",       XORLS_W,    3, { MATH_BASE_REG,
                                   UNSIGNED_SHORT_LITERAL,
                                   Q_DST_REG }, F_WORD },
   { "xor",       XOR_W,      3, { MATH_BASE_REG,
                                   P_SRC_REG,
                                   Q_DST_REG }, F_WORD },

   /***************************************************************************
    * ZE
    ***************************************************************************/
   { "ze",        ZE_W,       2, { P_SRC_REG, DST_REG }, F_WORD },
   { "ze.w",      ZE_W,       2, { P_SRC_REG, DST_REG }, F_WORD },

};
const int pic30_num_opcodes =
   (sizeof (pic30_opcodes) / sizeof (struct pic30_opcode));

/******************************************************************************
 *
 *   This function will insert the tttt encoding into the divide instruction.
 *
 ******************************************************************************/
unsigned long
pic30_insert_dividend_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

{
   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);
   insn = PIC30_ADD_OPERAND (insn, operand_value->value+1,
                             opnd->bits, opnd->shift+4);
   return(insn);
} /* unsigned long pic30_insert_dividend_reg(...) */

/******************************************************************************/

unsigned long
pic30_insert_file_reg_word_with_dst (insn, flags, opnd,
                                     operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This functions inserts a 15-bit file register with an implied 0 into an
 *   instruction that also accepts a destination register.  This function is
 *   required because the value must be shifted right by 1 before being
 *   inserted into the instruction.
 *
 ******************************************************************************/

{
   /* Need to shift the value by 1 to fit into the 15-bit field.  This
      operand has an implied 0.  */
   insn = PIC30_ADD_OPERAND (insn, operand_value->value >> 1,
                             opnd->bits, opnd->shift);

   return insn;
} /* unsigned long pic30_insert_file_reg_word_with_dst(...) */

/******************************************************************************/

char * pic30_extract_file_reg_word_with_dst (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract a 15-bit file register with an implied 0 from
 *   an instruction that also accepts a destination register.  This function is
 *   required because the value extracted from the instruction must be shifted
 *   left by 1 before printing the address.  This is because of the implied 0.
 *
 ******************************************************************************/

{
   long file_reg_word = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   char * rc = (char *) malloc (1 + BASE_10_STRING_LENGTH + 1);

   file_reg_word <<= 1;

   sprintf (rc, "0x%lx", file_reg_word);

   return rc;
} /* char * pic30_extract_file_reg_word_with_dst(...) */
   
/******************************************************************************/

unsigned long
pic30_get_g_or_h_mode_value (type, error_msg)
   unsigned long type;
   char **error_msg;

/******************************************************************************
 *
 *   This function returns the correct encoding for the specified operand type
 *   addressing mode.
 *
 ******************************************************************************/

{
   unsigned long rc = 0;

   switch (type)
   {
      case OPND_REGISTER_DIRECT :
         rc = G_OR_H_REGISTER_DIRECT;
         break;

      case OPND_REGISTER_INDIRECT :
         rc = G_OR_H_REGISTER_INDIRECT;
         break;

      case OPND_REGISTER_POST_DECREMENT :
         rc = G_OR_H_REGISTER_POST_DECREMENT;
         break;

      case OPND_REGISTER_POST_INCREMENT :
         rc = G_OR_H_REGISTER_POST_INCREMENT;
         break;

      case OPND_REGISTER_PRE_DECREMENT :
         rc = G_OR_H_REGISTER_PRE_DECREMENT;
         break;

      case OPND_REGISTER_PRE_INCREMENT :
         rc = G_OR_H_REGISTER_PRE_INCREMENT;
         break;

      case OPND_REGISTER_WITH_DISPLACEMENT :
         rc = G_OR_H_REGISTER_WITH_DISPLACEMENT;
         break;

      default :
         *error_msg = (char *) malloc (BUFSIZ);
         strcpy (*error_msg, "Internal error:  "
                             "pic30_get_g_or_h_mode_value called with an "
                             "invalid operand type");
         break;
   } /* switch */

   return rc;
} /* unsigned long pic30_get_g_or_h_mode_value(unsigned long, char **) */

/******************************************************************************/

char *
pic30_generate_g_or_h_operand (insn, reg_num, mode, flags, err)
   unsigned long insn;
   long reg_num;
   long mode;
   unsigned long flags;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will generate a string given the addressing mode.
 *
 ******************************************************************************/

{
   const long DISPLACEMENT_BITS = 4;
   const long DSP_DISPLACEMENT_SHIFT = 11;
   const long DISPLACEMENT_SHIFT = 15;

   const char * register_string = pic30_registers[reg_num];

   long shift =
      (flags & F_IS_DSP_INSN) ? DSP_DISPLACEMENT_SHIFT : DISPLACEMENT_SHIFT;

   char * rc;

   switch (mode)
   {
      case G_OR_H_REGISTER_DIRECT:
         rc = (char *) malloc (strlen (register_string) + 1);
         strcpy (rc, register_string);
         break;

      case G_OR_H_REGISTER_INDIRECT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[]") + 1);
         sprintf (rc, "[%s]", register_string);
         break;

      case G_OR_H_REGISTER_POST_DECREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[--]") + 1);
         sprintf (rc, "[%s--]", register_string);
         break;

      case G_OR_H_REGISTER_POST_INCREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[++]") + 1);
         sprintf (rc, "[%s++]", register_string);
         break;

      case G_OR_H_REGISTER_PRE_DECREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[--]") + 1);
         sprintf (rc, "[--%s]", register_string);
         break;

      case G_OR_H_REGISTER_PRE_INCREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[++]") + 1);
         sprintf (rc, "[++%s]", register_string);
         break;

      case G_OR_H_REGISTER_WITH_DISPLACEMENT:
      {
         long displacement_reg_num =
            PIC30_EXTRACT_OPERAND (insn, DISPLACEMENT_BITS, shift);
         const char * displacement_register_string =
            pic30_registers[displacement_reg_num];

         rc = (char *) malloc (strlen (register_string) +
                               strlen (displacement_register_string) +
                               strlen ("[+]") + 1);
         sprintf (rc, "[%s+%s]", register_string, displacement_register_string);
         break;
      } /* case G_OR_H_REGISTER_WITH_DISPLACEMENT: */

      default :
         *err = TRUE;

         rc = (char *) malloc (strlen ("") + 1);
         strcpy (rc, "");
         break;
   } /* switch (mode) */

   return rc;
} /* char * pic30_generate_g_or_h_operand(long, long, unsigned char *) */

/******************************************************************************/

unsigned long
pic30_insert_g_or_h_displacement_value (insn, flags, mode, value)
   unsigned long insn;
   unsigned long flags;
   long mode;
   long value;

/******************************************************************************
 *
 *   This function will insert an operand in the form [Wn+Wb] into the given
 *   instruction.
 *
 ******************************************************************************/

{
   const long DISPLACEMENT_BITS = 4;
   const long DSP_DISPLACEMENT_SHIFT = 11;
   const long DISPLACEMENT_SHIFT = 15;

   if (mode == G_OR_H_REGISTER_WITH_DISPLACEMENT)
   {
      long shift =
         (flags & F_IS_DSP_INSN) ? DSP_DISPLACEMENT_SHIFT : DISPLACEMENT_SHIFT;

      insn = PIC30_ADD_OPERAND (insn, value, DISPLACEMENT_BITS, shift);
   } /* if (mode == G_OR_H_REGISTER_WITH_DISPLACEMENT) */

   return insn;
} /* unsigned long pic30_insert_g_or_h_displacement_value(...) */

/******************************************************************************/

unsigned long
pic30_insert_g_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags;
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the GGG SSSS encoding into the instruction.
 *
 ******************************************************************************/

{
   long mode = pic30_get_g_or_h_mode_value (operand_value->type, error_msg);

   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);

   insn = PIC30_ADD_OPERAND (insn, mode, PIC30_ADDRESSING_MODE_BITS,
                             PIC30_SRC_MODE_SHIFT);

   insn =
      pic30_insert_g_or_h_displacement_value (insn, flags, mode,
                                              operand_value->optional_value);

   return insn;
} /* unsigned long pic30_insert_g_reg(...) */

/******************************************************************************/

char *
pic30_extract_g_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags;
   const struct pic30_operand * opnd;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will extract the GGG SSSS operand from the instruction.
 *
 ******************************************************************************/

{
   long reg_num = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   long mode = PIC30_EXTRACT_OPERAND (insn, PIC30_ADDRESSING_MODE_BITS,
                                      PIC30_SRC_MODE_SHIFT);

   return pic30_generate_g_or_h_operand (insn, reg_num, mode, flags, err);
} /* char * pic30_extract_g_reg(...) */

/******************************************************************************/

unsigned long
pic30_insert_h_dst_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags;
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the HHH DDDD encoding into the instruction.
 *
 ******************************************************************************/

{
   long mode = pic30_get_g_or_h_mode_value (operand_value->type, error_msg);

   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);

   insn = PIC30_ADD_OPERAND (insn, mode, PIC30_ADDRESSING_MODE_BITS,
                             PIC30_DST_MODE_SHIFT);

   insn =
      pic30_insert_g_or_h_displacement_value (insn, flags, mode,
                                              operand_value->optional_value);

   return insn;
} /* unsigned long pic30_insert_h_dst_reg(...) */

/******************************************************************************/

char *
pic30_extract_h_dst_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags;
   const struct pic30_operand * opnd;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will extract the HHH DDDD operand from the instruction.
 *
 ******************************************************************************/

{
   long reg_num = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   long mode = PIC30_EXTRACT_OPERAND (insn, PIC30_ADDRESSING_MODE_BITS,
                                      PIC30_DST_MODE_SHIFT);

   return pic30_generate_g_or_h_operand (insn, reg_num, mode, flags, err);
} /* char * pic30_extract_h_dst_reg(...) */

/******************************************************************************/

unsigned long
pic30_get_p_or_q_mode_value (type, error_msg)
   unsigned long type;
   char **error_msg;

/******************************************************************************
 *
 *   This function returns the correct encoding for the specified operand type
 *   addressing mode.
 *
 ******************************************************************************/

{
   unsigned long rc = 0;

   switch (type)
   {
      case OPND_REGISTER_DIRECT :
         rc = P_OR_Q_REGISTER_DIRECT;
         break;

      case OPND_REGISTER_INDIRECT :
         rc = P_OR_Q_REGISTER_INDIRECT;
         break;

      case OPND_REGISTER_POST_DECREMENT :
         rc = P_OR_Q_REGISTER_POST_DECREMENT;
         break;

      case OPND_REGISTER_POST_INCREMENT :
         rc = P_OR_Q_REGISTER_POST_INCREMENT;
         break;

      case OPND_REGISTER_PRE_DECREMENT :
         rc = P_OR_Q_REGISTER_PRE_DECREMENT;
         break;

      case OPND_REGISTER_PRE_INCREMENT :
         rc = P_OR_Q_REGISTER_PRE_INCREMENT;
         break;

      default :
         *error_msg = (char *) malloc (BUFSIZ);
         strcpy (*error_msg, "Internal error:  "
                             "pic30_get_p_or_q_mode_value called with an "
                             "invalid operand type");
         break;
   } /* switch */

   return rc;
} /* unsigned long pic30_get_p_or_q_mode_value(unsigned long, char **) */

/******************************************************************************/

char *
pic30_generate_p_or_q_operand (reg_num, mode, err)
   long reg_num;
   long mode;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will generate a string given the addressing mode.
 *
 ******************************************************************************/

{
   const char * register_string = pic30_registers[reg_num];

   char * rc;

   switch (mode)
   {
      case P_OR_Q_REGISTER_DIRECT:
         rc = (char *) malloc (strlen (register_string) + 1);
         strcpy (rc, register_string);
         break;

      case P_OR_Q_REGISTER_INDIRECT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[]") + 1);
         sprintf (rc, "[%s]", register_string);
         break;

      case P_OR_Q_REGISTER_POST_DECREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[--]") + 1);
         sprintf (rc, "[%s--]", register_string);
         break;

      case P_OR_Q_REGISTER_POST_INCREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[++]") + 1);
         sprintf (rc, "[%s++]", register_string);
         break;

      case P_OR_Q_REGISTER_PRE_DECREMENT:
         rc = (char *) malloc (strlen (register_string) + strlen ("[--]") + 1);
         sprintf (rc, "[--%s]", register_string);
         break;

      case P_OR_Q_REGISTER_PRE_INCREMENT:
         rc = (char * )malloc (strlen (register_string) + strlen ("[++]") + 1);
         sprintf (rc, "[++%s]", register_string);
         break;

      default :
         *err = TRUE;

         rc = (char *) malloc (strlen ("") + 1);
         strcpy (rc, "");
         break;
   } /* switch (mode) */

   return rc;
} /* char * pic30_generate_p_or_q_operand(long, long, unsigned char *) */

/******************************************************************************/

unsigned long
pic30_insert_p_src_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the PPP SSSS encoding into the instruction.
 *
 ******************************************************************************/

{
   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);

   insn = PIC30_ADD_OPERAND (insn,
                             pic30_get_p_or_q_mode_value (operand_value->type,
                                                          error_msg),
                             PIC30_ADDRESSING_MODE_BITS, PIC30_SRC_MODE_SHIFT);

   return insn;
} /* unsigned long pic30_insert_p_src_reg(...) */

/******************************************************************************/

char *
pic30_extract_p_src_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will extract the PPP SSSS operand from the instruction.
 *
 ******************************************************************************/

{
   long reg_num = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   long mode = PIC30_EXTRACT_OPERAND (insn, PIC30_ADDRESSING_MODE_BITS,
                                      PIC30_SRC_MODE_SHIFT);

   return pic30_generate_p_or_q_operand (reg_num, mode, err);
} /* char * pic30_extract_p_src_reg(...) */

/******************************************************************************/

unsigned long
pic30_insert_q_dst_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the QQQ DDDD encoding into the instruction.
 *
 ******************************************************************************/

{
   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);

   insn = PIC30_ADD_OPERAND (insn,
                             pic30_get_p_or_q_mode_value (operand_value->type,
                                                          error_msg),
                             PIC30_ADDRESSING_MODE_BITS, PIC30_DST_MODE_SHIFT);

   return insn;
} /* unsigned long pic30_insert_q_dst_reg(...) */

/******************************************************************************/

char *
pic30_extract_q_dst_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err;

/******************************************************************************
 *
 *   This function will extract the QQQ DDDD operand from the instruction.
 *
 ******************************************************************************/

{
   long reg_num = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   long mode = PIC30_EXTRACT_OPERAND (insn, PIC30_ADDRESSING_MODE_BITS,
                                      PIC30_DST_MODE_SHIFT);

   return pic30_generate_p_or_q_operand (reg_num, mode, err);
} /* char * pic30_extract_q_dst_reg(...) */

/******************************************************************************/

unsigned long
pic30_insert_lo_reg (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function inserts an operand in the form [Wn+Slit10] into the
 *   instruction.  NOTE:  The Slit10 is encoded in three separate pieces.
 *
 ******************************************************************************/

{
   const long LO1_MASK = 0x0007;
   const long LO2_MASK = 0x0038;
   const long LO3_MASK = 0x03C0;

   const long LO1_BITS = 3;
   const long LO2_BITS = 3;
   const long LO3_BITS = 4;

   const long LO1_SHIFT = 4;
   const long LO2_SHIFT = 11;
   const long LO3_SHIFT = 15;

   unsigned long lo1 = (operand_value->optional_value & LO1_MASK);
   unsigned long lo2 = (operand_value->optional_value & LO2_MASK) >> 3;
   unsigned long lo3 = (operand_value->optional_value & LO3_MASK) >> 6;

   insn = PIC30_ADD_OPERAND (insn, operand_value->value,
                             opnd->bits, opnd->shift);

   insn = PIC30_ADD_OPERAND (insn, lo1, LO1_BITS, LO1_SHIFT);
   insn = PIC30_ADD_OPERAND (insn, lo2, LO2_BITS, LO2_SHIFT);
   insn = PIC30_ADD_OPERAND (insn, lo3, LO3_BITS, LO3_SHIFT);

   return insn;
} /* unsigned long pic30_insert_lo_src_reg(...) */

/******************************************************************************/

char *
pic30_extract_lo_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts an operand in the form [Wn+Slit10] into the
 *   instruction.  NOTE:  The Slit10 is encoded in three separate pieces.
 *
 ******************************************************************************/

{
   const long LO1_BITS = 3;
   const long LO2_BITS = 3;
   const long LO3_BITS = 4;

   const long LO1_SHIFT = 4;
   const long LO2_SHIFT = 11;
   const long LO3_SHIFT = 15;

   long reg_num = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   const char * register_string = pic30_registers[reg_num];

   const long STRING_LENGTH = strlen (register_string)
                            + strlen ("[+]")
                            + BASE_10_STRING_LENGTH
                            + 1;

   long lo1 = PIC30_EXTRACT_OPERAND (insn, LO1_BITS, LO1_SHIFT);
   long lo2 = PIC30_EXTRACT_OPERAND (insn, LO2_BITS, LO2_SHIFT);
   long lo3 = PIC30_EXTRACT_OPERAND (insn, LO3_BITS, LO3_SHIFT);

   long literal_offset = (lo3 << 6)
                       | (lo2 << 3)
                       | (lo1 << 0);

   char * rc = (char *) malloc (STRING_LENGTH);

   /* Sign-extend the result if literal is negative. */
   if (literal_offset & 0x200)
      literal_offset |= ~((1 << (10)) - 1);

   /*
    * Undo the change that the assembler made to convert the byte offset to
    * a word offset.
    */
   if (flags & F_WORD)
      literal_offset *= 2;

   /*
    * The result should contain a plus sign if positive literal offset;
    * otherwise, let the negative sign for the literal offset act as the
    * operator.
    */
   sprintf(rc, "[%s%s%ld]", register_string,
                            ((literal_offset >= 0) ? "+" : ""),
                            literal_offset);

   return rc;
}

/******************************************************************************/

unsigned char
pic30_match_even (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the operand value is even.
 *
 ******************************************************************************/

{
   return PIC30_IS_EVEN (check->value);
} /* unsigned char pic30_match_even(...) */

/******************************************************************************/

unsigned char
pic30_match_even_not_14 (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the operand value is even and not 14.
 *    (used for testing the destination of MUL instructions)
 *
 ******************************************************************************/

{
   return (PIC30_IS_EVEN (check->value) && (check->value != 14));
} /* unsigned char pic30_match_even_not_14(...) */

/******************************************************************************/unsigned char
pic30_match_div_src_reg (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This operand ensures that the operand value (register number) is between
 *   2 and 15, inclusive.
 *
 ******************************************************************************/

{
   return ((check->value >=2) && (check->value <= 15));
} /* unsigned char pic30_match_div_src_reg(...) */

/******************************************************************************/

unsigned char
pic30_match_file_reg_byte (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is able to fit in
 *   a 13-bit file register.
 *
 ******************************************************************************/

{
   return (PIC30_IS_13_BIT_FILE_REG (check->value));
} /* unsigned char pic30_match_file_reg_byte(...) */

/******************************************************************************/
unsigned char
pic30_match_file_reg (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is able to fit in
 *   a 13-bit file register and that it is an even value.
 *
 ******************************************************************************/

{
   return ((PIC30_IS_13_BIT_FILE_REG (check->value)) &&
           (PIC30_IS_EVEN (check->value)));
} /* unsigned char pic30_match_file_reg(...) */


unsigned char
pic30_match_page_reg (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is able to fit in
 *   a 13-bit file register and that it is an even value.
 *
 ******************************************************************************/

{ int i;

  for (i = 0; pic30_page_registers[i]; i++) {
    if (strcmp(pic30_page_registers[i], check->value) == 0) return 1;
  }
  return 0;
} 

/******************************************************************************/

unsigned char
pic30_match_file_reg_word (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is able to fit in
 *   a 16-bit file register and that it is an even value.
 *
 ******************************************************************************/

{
   return ((PIC30_IS_16_BIT_UNSIGNED_LITERAL (check->value)) &&
           (PIC30_IS_EVEN (check->value)));
} /* unsigned char pic30_match_file_reg_word(...) */

/******************************************************************************/

unsigned char
pic30_match_byte_10bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function is used for byte instructions.  It ensures that the operand
 *   value is a able to fit in the given 8-bits.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_BYTE_VALUE (check->value)));
} /* unsigned char pic30_match_byte_10bit_lit(...) */

/******************************************************************************/unsigned char
pic30_match_byte_8bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function is used for byte instructions.  It ensures that the operand
 *   value is a able to fit in the given 8-bits.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_BYTE_VALUE_UNSIGNED (check->value)));
} /* unsigned char pic30_match_byte_10bit_lit(...) */

/******************************************************************************/

char *
pic30_extract_byte_10bit_lit (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function is used to extract the 10-bit literal for byte instructions.
 *
 ******************************************************************************/

{
   long lit = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   char * rc;

   lit = (lit & 0xFF);
 
   rc = (char *) malloc (1 + BASE_10_STRING_LENGTH + 1);
   sprintf (rc, "#0x%lx", lit);

   return rc;
} /* char * pic30_extract_byte_10bit_lit(...) */

/******************************************************************************/char *
pic30_extract_byte_cp_8bit_lit (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

{
   long  lit = PIC30_EXTRACT_OPERAND (insn, 10, opnd->shift);
   long lit1;
   char * rc;

   lit = (lit & 0x3FF);
   lit1 = (lit & 0x380);
   lit1 =(lit1 >> 2);
   lit = (lit1 | (lit & 0x1F));

   rc = (char *) malloc (1 + BASE_10_STRING_LENGTH + 1);
   sprintf (rc, "#0x%lx", lit);

   return rc;
}
/******************************************************************************/char *
pic30_extract_page_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

{
   long  lit = PIC30_EXTRACT_OPERAND (insn, 2, opnd->shift);
   char * rc;

   rc = (char *) malloc (1 + strlen(pic30_page_registers[lit]) + 1);
   sprintf (rc, "%s", pic30_page_registers[lit]);

   return rc;
}

/******************************************************************************/char *
pic30_extract_wmul_dst_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

{
   long  lit = PIC30_EXTRACT_OPERAND (insn, 3, opnd->shift);
   char * rc;

   rc = (char *) malloc (1 + strlen(pic30_registers[lit*2]) + 1);
   sprintf (rc, "%s",  pic30_registers[lit*2]);

   return rc;
}



/******************************************************************************
 *
 *   This function ensures that the given operand value is a 10-bit literal.
 *
 ******************************************************************************/unsigned char
pic30_match_10bit_lit (check)
   const struct pic30_operand_value * check;
{
   return ((check->modifier != PIC30_NO_MODIFIER_FOUND) ||
           (PIC30_IS_10_BIT_UNSIGNED_LITERAL(check->value)));
} /* unsigned char pic30_match_10bit_lit(...) */

/******************************************************************************/

char *
pic30_extract_w_reg (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags;
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This instruction is used for those instructions that utilize the default
 *   WREG.  It will extract WREG if the D bit is 0.
 *
 ******************************************************************************/

{
   char * rc = malloc (strlen (PIC30_W_REGISTER_STRING) + 1);

   if (flags & F_HAS_REQUIRED_WREG)
      strcpy (rc, PIC30_W_REGISTER_STRING);
   else
   {
      long w_reg_flag =
         PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

      if (!w_reg_flag)
         strcpy (rc, PIC30_W_REGISTER_STRING);
      else
         strcpy (rc, "");
   } /* else */

   return rc;
} /* char * pic30_extract_w_reg(...) */

/******************************************************************************/

unsigned char
pic30_match_16bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is a 16-bit literal.
 *
 ******************************************************************************/

{
   return ((check->modifier != PIC30_NO_MODIFIER_FOUND) ||
           (PIC30_IS_16_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_16bit_lit(...) */

/******************************************************************************/

unsigned char
pic30_match_unsigned_short_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is a 5-bit unsigned
 *   literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_5_BIT_UNSIGNED_LITERAL (check->value)));
} /* unsigned char pic30_match_unsigned_short_lit(...) */

/******************************************************************************/

unsigned char
pic30_match_3bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is a 3-bit value.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_3_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_3bit_lit(...) */

/******************************************************************************/


/******************************************************************************
 *
 *   This function ensures that the given operand value is a 4-bit unsigned
 *   literal value. The 4-bit value will be encoded as a 3-bit value.
 *
 ******************************************************************************/
unsigned char
pic30_match_34bit_lit (check)
   const struct pic30_operand_value * check;
{
   unsigned char rc;

   rc = ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
         (PIC30_IS_4_BIT_UNSIGNED_LITERAL (check->value)));

   return(rc);
} /* unsigned char pic30_match_34bit_lit(...) */

/******************************************************************************
 *
 *   This function will insert the +1 encoding into the bit manipulation
 *   instructions in the case that the bit number is greater than seven.
 *
 ******************************************************************************/
unsigned long
pic30_insert_34bit_lit (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

{
   insn = PIC30_ADD_OPERAND(insn, operand_value->value,
                                  opnd->bits, opnd->shift);
   if (operand_value->value > 7)
   {
     /* insert 1 into the 13-bit file register address field */
      insn = PIC30_ADD_OPERAND(insn, 1, 13, 0);
   }
   return(insn);
} /* unsigned long pic30_insert_34bit_lit(...) */

/******************************************************************************/
unsigned long
pic30_insert_byte_cp_8bit_lit (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
     const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

{
  long lit;
  long lit1;

  lit = operand_value->value;
  lit1=(lit & 0xE0) << 2;
  lit = lit1 | (lit & 0x1F);
  
  insn = PIC30_ADD_OPERAND(insn, lit, 10, 0);
   
  return(insn);
}

/******************************************************************************/
/******************************************************************************/
unsigned long
pic30_insert_byte_l_operand (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
     const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg __attribute__ ((__unused__));

{
  long lit;
  long lit1;
  long lit2;

  lit1 = operand_value->value;
  lit2 = lit1 + 0x01;
  lit2 = ( lit2 << 11 );
  lit = lit1 | lit2;
  
  insn = PIC30_ADD_OPERAND(insn, lit, 14, 0);
   
  return(insn);
}

/******************************************************************************/
unsigned char
pic30_match_nibble (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is a 4-bit unsigned
 *   literal value.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_4_BIT_UNSIGNED_LITERAL (check->value)));
} /* unsigned char pic30_match_nibble(...) */

/******************************************************************************/

/* for communication to pic30-dis.c */
extern bfd_vma pic30_disassembly_address;

char *
pic30_extract_offset (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags;
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract a PC-relative offset from the instruction.
 *
 ******************************************************************************/
{
   long offset = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   char * rc = (char *) malloc (BUFSIZ);

   /* Sign-extend the result if offset is negative. */
   if (opnd->bits == 16) {
     if (offset & 0x8000)
        offset |= ~((1 << (16)) - 1);
   } else if (opnd->bits == 6) {
     if (offset & 0x20)
        offset |= ~((1 << (6)) - 1);
   } /* else error message */

   offset = (offset + 1) * 2;
   info->target =  offset + (signed long) pic30_disassembly_address;

   if (flags & F_IS_2_WORD_INSN)
     info->target += 2;  /* display correct target for DO instr */

   if (offset) {  /* Find a symbol name if possible  */
     long min = 0;
     long max = info->num_symbols;
     long thisplace;
     long sym_found = 0;
     asymbol *sym = 0;

     /* Perform a binary search of the range (min, max).  */
     if (info->num_symbols > 0)
       while (min + 1 < max) {
         bfd_vma try;

         thisplace = (max + min) / 2;
         thisplace = min + 1;
         sym = info->symbols[thisplace];
         try = bfd_asymbol_value (sym);

         if (try > info->target)
           max = thisplace;
         else if (try < info->target)
           min = thisplace;
         else {
           if ((try == info->target) &&
               PIC30_DISPLAY_AS_PROGRAM_MEMORY(sym->section))
             sym_found = thisplace;
           break;
         }
       }
     sprintf(rc, "0x%lx", info->target);
     if (sym_found) {
       strcat(rc, " <");
       strcat(rc, bfd_asymbol_name(sym));
       strcat(rc, ">");
     } 
   }
   else
     sprintf(rc, ".");

   return rc;
} /* char * pic30_extract_offset(...) */

/******************************************************************************/

char *
pic30_extract_branch_on_dsp_condition_operand (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract a BRA instruction's DSP condition code.
 *
 ******************************************************************************/

{
   long condition_code =
      PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   char * rc = (char *) malloc (PIC30_MAX_CONDITION_CODE_LENGTH + 1);

   switch (condition_code)
   {
      case PIC30_BRANCH_ON_ACCUMULATOR_A_OVERFLOW :
         strcpy (rc, "OA");
         break;

      case PIC30_BRANCH_ON_ACCUMULATOR_B_OVERFLOW :
         strcpy (rc, "OB");
         break;

      case PIC30_BRANCH_ON_ACCUMULATOR_A_SATURATION :
         strcpy (rc, "SA");
         break;

      case PIC30_BRANCH_ON_ACCUMULATOR_B_SATURATION :
         strcpy (rc, "SB");
         break;

      default:
         strcpy (rc, "");
         break;
   } /* switch (condition_code) */

   return rc;
} /* char * pic30_extract_branch_on_dsp_condition_operand(...) */

/******************************************************************************/

char *
pic30_extract_branch_on_condition_operand (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract a BRA instruction's MCU condition code.
 *
 ******************************************************************************/

{
   long condition_code =
      PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   char * rc = (char *) malloc (PIC30_MAX_CONDITION_CODE_LENGTH + 1);

   switch (condition_code)
   {
      case PIC30_BRANCH_ON_OVERFLOW :
         strcpy (rc, "OV");
         break;

      case PIC30_BRANCH_ON_CARRY :
         strcpy (rc, "C");
         break;

      case PIC30_BRANCH_ON_ZERO :
         strcpy (rc, "Z");
         break;

      case PIC30_BRANCH_ON_NEGATIVE :
         strcpy (rc, "N");
         break;

      case PIC30_BRANCH_ON_GREATER_OR_EQUAL :
         strcpy (rc, "GE");
         break;

      case PIC30_BRANCH_ON_LESS_THAN :
         strcpy (rc, "LT");
         break;

      case PIC30_BRANCH_ON_GREATER_THAN :
         strcpy (rc, "GT");
         break;

      case PIC30_BRANCH_ON_LESS_OR_EQUAL :
         strcpy (rc, "LE");
         break;

      case PIC30_BRANCH_ON_NOT_OVERFLOW :
         strcpy (rc, "NOV");
         break;

      case PIC30_BRANCH_ON_NOT_CARRY :
         strcpy (rc, "NC");
         break;

      case PIC30_BRANCH_ON_NOT_ZERO :
         strcpy (rc, "NZ");
         break;

      case PIC30_BRANCH_ON_NOT_NEGATIVE :
         strcpy (rc, "NN");
         break;

      case PIC30_BRANCH_ON_GREATER_THAN_UNSIGNED :
         strcpy (rc, "GTU");
         break;

      case PIC30_BRANCH_ON_LESS_OR_EQUAL_UNSIGNED :
         strcpy (rc, "LEU");
         break;

      default:
         strcpy (rc, "");
         break;
   } /* switch (condition_code) */

   return rc;
} /* char * pic30_extract_branch_on_condition_operand(...) */

/******************************************************************************/

unsigned char
pic30_match_unsigned_14bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is an unsigned 14-bit
 *   literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_14_BIT_UNSIGNED_LITERAL (check->value)));
} /* unsigned char pic30_match_unsigned_14bit_lit(...) */

/******************************************************************************/
unsigned char
pic30_match_unsigned_15bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is an unsigned 15-bit
 *   literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_15_BIT_UNSIGNED_LITERAL (check->value)));
} /* unsigned char pic30_match_unsigned_14bit_lit(...) */

/******************************************************************************/
unsigned char
pic30_match_frame_size (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is allowed for the
 *   LNK instruction.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_14_BIT_UNSIGNED_LITERAL (check->value)) &&
           (PIC30_IS_EVEN (check->value)));
} /* unsigned char pic30_match_frame_size(...) */

/******************************************************************************/

unsigned char
pic30_match_16bit_unsigned_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is an unsigned 16-bit
 *   literal.
 *
 ******************************************************************************/

{
   return ((check->modifier != PIC30_NO_MODIFIER_FOUND) ||
           (PIC30_IS_16_BIT_UNSIGNED_LITERAL (check->value)));
} /* unsigned char pic30_match_16bit_unsigned_lit(...) */

/******************************************************************************/

char *
pic30_extract_dsp_accumulator_select (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract the accumulator from the instructions A bit.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_DSP_ACCUMULATOR_LENGTH + 1);
   long dsp_accumulator = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   if (dsp_accumulator == PIC30_DSP_ACCUMULATOR_A)
      strcpy (rc, PIC30_DSP_ACCUMULATOR_A_STRING);
   else if (dsp_accumulator == PIC30_DSP_ACCUMULATOR_B)
      strcpy (rc, PIC30_DSP_ACCUMULATOR_B_STRING);
   else
      strcpy (rc, "");

   return rc;
} /* char * pic30_extract_dsp_accumulator_select(...) */

/******************************************************************************/
unsigned char
pic30_match_4bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function will ensure that the given operand value is a 4-bit literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_4_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_4bit_lit(...) */

/******************************************************************************/

unsigned char
pic30_match_short_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function will ensure that the given operand value is a 5-bit literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_5_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_short_lit(...) */

/******************************************************************************/

unsigned char
pic30_match_6bit_lit (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function will ensure that the given operand value is a 6-bit literal.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_DSP6_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_4bit_lit(...) */

/******************************************************************************/

unsigned char
pic30_match_dsp_writeback (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This operand ensures that the operand value is valid for a DSP writeback
 *   operand.
 *
 ******************************************************************************/

{
   unsigned char rc = TRUE;

   switch (check->type)
   {
      case OPND_REGISTER_POST_INCREMENT_BY_N:
         rc = (check->optional_value == 2);
         /* Want to fall through to next case.  No break. */

      case OPND_REGISTER_DIRECT:
         rc &= (check->value == PIC30_DSP_WRITEBACK_REGISTER_NUMBER);
         break;

      default:
         rc = FALSE;
         break;
   } /* switch (check->type) */

   return rc;
} /* unsigned char pic30_match_dsp_writeback(...) */

/******************************************************************************/

unsigned long
pic30_insert_dsp_writeback (insn, flags, opnd, operand_value, error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the DSP writeback operand into the instruction.
 *
 ******************************************************************************/

{
   unsigned short writeback = 0;

   switch (operand_value->type)
   {
      case OPND_REGISTER_DIRECT:
         writeback = PIC30_DSP_WRITEBACK_DIRECT;
         break;

      case OPND_REGISTER_POST_INCREMENT_BY_N:
         writeback = PIC30_DSP_WRITEBACK_POST_INCREMENT_BY_N;
         break;

      default:
         *error_msg = (char *) malloc (BUFSIZ);
         strcpy (*error_msg, "Internal error:  "
                             "pic30_insert_dsp_writeback called with an "
                             "invalid operand type");
         break;
   } /* switch (check->type) */

   insn = PIC30_ADD_OPERAND (insn, writeback, opnd->bits, opnd->shift);

   return insn;
} /* unsigned long pic30_insert_dsp_writeback(...) */


/******************************************************************************/

char *
pic30_extract_dsp_writeback (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts the DSP writeback operand from an instruction.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_DSP_WRITEBACK_LENGTH + 1);
   long writeback = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   switch (writeback)
   {
      case PIC30_DSP_WRITEBACK_DIRECT:
         sprintf (rc, "w%d", PIC30_DSP_WRITEBACK_REGISTER_NUMBER);
         break;

      case PIC30_DSP_WRITEBACK_POST_INCREMENT_BY_N:
         sprintf (rc, "[w%d]+=2", PIC30_DSP_WRITEBACK_REGISTER_NUMBER);
         break;

      case 2:
         strcpy (rc, "");
         break;

      case 3:
         strcpy (rc, "???");
         break;

      default:
         strcpy (rc, "");
         break;
   } /* switch (writeback) */

   return rc;
} /* char * pic30_extract_dsp_writeback(...) */

/******************************************************************************/

unsigned char
pic30_match_dsp_x_prefetch_operation (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the operand value is valid for a DSP X-space
 *   prefetch.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;

   switch (check->type)
   {
      case OPND_REGISTER_INDIRECT:
         if ((check->value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ||
             (check->value == PIC30_X_PREFETCH_MAC2_REGISTER_VALUE))
            rc = TRUE;

         break;

      case OPND_REGISTER_POST_INCREMENT_BY_N:
      case OPND_REGISTER_POST_DECREMENT_BY_N:
         if (((check->value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ||
              (check->value == PIC30_X_PREFETCH_MAC2_REGISTER_VALUE)) &&
             ((check->optional_value == -2) ||
              (check->optional_value == -4) ||
              (check->optional_value == -6) ||
              (check->optional_value == 2)  ||
              (check->optional_value == 4)  ||
              (check->optional_value == 6)))
            rc = TRUE;

         break;

      case OPND_REGISTER_WITH_DISPLACEMENT:
         if ((check->value == PIC30_X_PREFETCH_MAC2_REGISTER_VALUE) &&
             (check->optional_value ==
              PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER_VALUE))
            rc = TRUE;

         break;

      default:
         break;
   } /* switch (check_expression->expr.X_op) */

   return rc;
} /* unsigned char pic30_match_dsp_x_prefetch_operation(...) */

/******************************************************************************/

unsigned long
pic30_insert_dsp_x_prefetch_operation (insn, flags, opnd, operand_value,
                                       error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function inserts the DSP X-space prefetch operation into the
 *   instruction.
 *
 ******************************************************************************/

{
   long register_value = operand_value->value;
   long dsp_operation = PIC30_DSP_X_PREFETCH_OPERATION_NO_PREFETCH;

   switch (operand_value->type)
   {
      case OPND_REGISTER_INDIRECT:
         dsp_operation =
            (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
            PIC30_DSP_X_PREFETCH_OPERATION_MAC1_INDIRECT             :
            PIC30_DSP_X_PREFETCH_OPERATION_MAC2_INDIRECT;

         break;

      case OPND_REGISTER_POST_INCREMENT_BY_N:
         switch (operand_value->optional_value)
         {
            case 2:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS2                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS2;

               break;

            case 4:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS4                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS4;

               break;

            case 6:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS6                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS6;

               break;

            case -2:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS2               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS2;

               break;


            case -4:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS4               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS4;

               break;

            case -6:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS6               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS6;

               break;

            default:
               *error_msg = (char *) malloc (BUFSIZ);
               strcpy (*error_msg, "Internal error:  "
                                   "pic30_insert_dsp_x_prefetch_operation "
                                   "called with an invalid offset");
               break;
         } /* switch (operand_value->optional_value) */

         break;

      case OPND_REGISTER_POST_DECREMENT_BY_N:
         switch (operand_value->optional_value)
         {
            case 2:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS2               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS2;

               break;


            case 4:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS4               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS4;

               break;

            case 6:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS6               :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS6;

               break;

            case -2:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS2                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS2;

               break;

            case -4:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS4                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS4;

               break;

            case -6:
               dsp_operation =
                  (register_value == PIC30_X_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS6                :
                  PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS6;

               break;

            default:
               *error_msg = (char *) malloc (BUFSIZ);
               strcpy (*error_msg, "Internal error:  "
                                   "pic30_insert_dsp_x_prefetch_operation "
                                   "called with an invalid offset");
               break;
         } /* switch (operand_value->optional_value) */

         break;

      case OPND_REGISTER_WITH_DISPLACEMENT:
         dsp_operation = PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS_REG;
         break;

      default:
         *error_msg = (char *) malloc (BUFSIZ);
         strcpy (*error_msg, "Internal error:  "
                             "pic30_insert_dsp_x_prefetch_operation "
                             "called with an invalid operand type");
         break;
   } /* switch (operand_value->type) */

   insn = PIC30_ADD_OPERAND (insn, dsp_operation, opnd->bits, opnd->shift);

   return insn;
} /* unsigned long pic30_insert_dsp_x_prefetch_operation (...) */

/******************************************************************************/unsigned long
pic30_insert_page_reg (insn, flags, opnd, operand_value,
                                       error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;
{

  int i;

  for (i = 0; pic30_page_registers[i]; i++) {
    if (strcmp(pic30_page_registers[i], operand_value->value) == 0) {
      break;
    }
  }
  i = i << 10;
  
  insn = PIC30_ADD_OPERAND(insn, i, 12, 0);
   
  return(insn);
}

/******************************************************************************/unsigned long
pic30_insert_wmul_dst_reg (insn, flags, opnd, operand_value,
                                       error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;
{

  int i = operand_value->value;

  
  i = i << 7;
  
  insn = PIC30_ADD_OPERAND(insn, i, 11, 0);
   
  return(insn);
} 

/******************************************************************************/
char *
pic30_extract_dsp_x_prefetch_operation (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts the DSP X-space prefetch operation from an
 *   instruction.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_DSP_X_PREFETCH_OPERATION_LENGTH + 1);
   long operation = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   switch (operation)
   {
      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_INDIRECT:
         sprintf (rc, "[%s]", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS2:
         sprintf (rc, "[%s]+=2", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS4:
         sprintf (rc, "[%s]+=4", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS6:
         sprintf (rc, "[%s]+=6", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS2:
         sprintf (rc, "[%s]-=2", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS4:
         sprintf (rc, "[%s]-=4", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS6:
         sprintf (rc, "[%s]-=6", PIC30_X_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_INDIRECT:
         sprintf (rc, "[%s]", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS2:
         sprintf (rc, "[%s]+=2", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS4:
         sprintf (rc, "[%s]+=4", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS6:
         sprintf (rc, "[%s]+=6", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS2:
         sprintf (rc, "[%s]-=2", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS_REG:
         sprintf (rc, "[%s+%s]", PIC30_X_PREFETCH_MAC2_REGISTER,
                                 PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS4:
         sprintf (rc, "[%s]-=4", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS6:
         sprintf (rc, "[%s]-=6", PIC30_X_PREFETCH_MAC2_REGISTER);
         break;

      default:
         strcpy (rc, "");
         break;
   } /* switch (operation) */

   return rc;
} /* char * pic30_extract_dsp_x_prefetch_operation(...) */

/******************************************************************************/

unsigned char
pic30_match_dsp_prefetch_dst (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given oeprand value is a valid DSP prefetch
 *   destination.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;

   switch (check->type)
   {
      case OPND_REGISTER_DIRECT:
         if ((check->value >= PIC30_MIN_DSP_PREFETCH_DST_REGISTER_VALUE) &&
             (check->value <= PIC30_MAX_DSP_PREFETCH_DST_REGISTER_VALUE))
            rc = TRUE;

      default:
         break;
   } /* switch (check->type) */

   return rc;
} /* unsigned char pic30_match_dsp_prefetch_dst(...) */

/******************************************************************************/

char *
pic30_extract_dsp_prefetch_dst (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts a DSP prefetch destination from the instruction.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_REGISTER_LENGTH + 1);
   long which_register = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   /*
    * Add the minimum register number.  This is done because the register
    * first DSP prefetch destination register is encoded as 0.
    */
   which_register += PIC30_MIN_DSP_PREFETCH_DST_REGISTER_VALUE;

   strcpy(rc, pic30_registers[which_register]);

   return rc;
} /* char * pic30_extract_dsp_x_prefetch_operation(...) */

/******************************************************************************/

unsigned char
pic30_match_dsp_y_prefetch_operation (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is a DSP Y-space
 *   prefetch operation.
 *
 ******************************************************************************/

{
   unsigned char rc = FALSE;

   switch (check->type)
   {
      case OPND_REGISTER_INDIRECT:
         if ((check->value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ||
             (check->value == PIC30_Y_PREFETCH_MAC2_REGISTER_VALUE))
            rc = TRUE;

         break;

      case OPND_REGISTER_POST_INCREMENT_BY_N:
      case OPND_REGISTER_POST_DECREMENT_BY_N:
         if (((check->value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ||
              (check->value == PIC30_Y_PREFETCH_MAC2_REGISTER_VALUE)) &&
             ((check->optional_value == -2) ||
              (check->optional_value == -4) ||
              (check->optional_value == -6) ||
              (check->optional_value == 2)  ||
              (check->optional_value == 4)  ||
              (check->optional_value == 6)))
            rc = TRUE;

         break;

      case OPND_REGISTER_WITH_DISPLACEMENT:
         if ((check->value == PIC30_Y_PREFETCH_MAC2_REGISTER_VALUE) &&
             (check->optional_value ==
              PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER_VALUE))
            rc = TRUE;

         break;

      default:
         break;
   } /* switch (check_expression->expr.X_op) */

   return rc;
} /* unsigned char pic30_match_dsp_y_prefetch_operation(...) */

/******************************************************************************/

unsigned long
pic30_insert_dsp_y_prefetch_operation (insn, flags, opnd, operand_value,
                                       error_msg)
   unsigned long insn;
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   const struct pic30_operand_value * operand_value;
   char **error_msg;

/******************************************************************************
 *
 *   This function will insert the DSP Y-space prefetch operation into the
 *   given instruction.
 *
 ******************************************************************************/

{
   long register_value = operand_value->value;
   long dsp_operation = PIC30_DSP_Y_PREFETCH_OPERATION_NO_PREFETCH;

   switch (operand_value->type)
   {
      case OPND_REGISTER_INDIRECT:
         dsp_operation =
            (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
            PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_INDIRECT             :
            PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_INDIRECT;

         break;

      case OPND_REGISTER_POST_INCREMENT_BY_N:
         switch (operand_value->optional_value)
         {
            case 2:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS2                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS2;

               break;

            case 4:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS4                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS4;

               break;

            case 6:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS6                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS6;

               break;

            case -2:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS2               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS2;

               break;


            case -4:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS4               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS4;

               break;

            case -6:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS6               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS6;

               break;

            default:
               *error_msg = (char *) malloc (BUFSIZ);
               strcpy (*error_msg, "Internal error:  "
                                   "pic30_insert_dsp_y_prefetch_operation "
                                   "called with an invalid offset");
               break;
         } /* switch (operand_value->optional_value) */

         break;

      case OPND_REGISTER_POST_DECREMENT_BY_N:
         switch (operand_value->optional_value)
         {
            case 2:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS2               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS2;

               break;


            case 4:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS4               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS4;

               break;

            case 6:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS6               :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS6;

               break;

            case -2:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS2                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS2;

               break;

            case -4:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS4                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS4;

               break;

            case -6:
               dsp_operation =
                  (register_value == PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE) ?
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS6                :
                  PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS6;

               break;

            default:
               *error_msg = (char *) malloc (BUFSIZ);
               strcpy (*error_msg, "Internal error:  "
                                   "pic30_insert_dsp_y_prefetch_operation "
                                   "called with an invalid offset");
               break;
         } /* switch (operand_value->optional_value) */

         break;

      case OPND_REGISTER_WITH_DISPLACEMENT:
         dsp_operation = PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS_REG;
         break;

      default:
         *error_msg = (char *) malloc (BUFSIZ);
         strcpy (*error_msg, "Internal error:  "
                             "pic30_insert_dsp_y_prefetch_operation "
                             "called with an invalid operand type");
         break;
   } /* switch (operand_value->type) */

   insn = PIC30_ADD_OPERAND (insn, dsp_operation, opnd->bits, opnd->shift);

   return insn;
} /* unsigned long pic30_insert_dsp_y_prefetch_operation (...) */

/******************************************************************************/

char *
pic30_extract_dsp_y_prefetch_operation (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function will extract the DSP Y-space prefetch operation from the
 *   given instruction.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_DSP_Y_PREFETCH_OPERATION_LENGTH + 1);
   long operation = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   switch (operation)
   {
      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_INDIRECT:
         sprintf (rc, "[%s]", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS2:
         sprintf (rc, "[%s]+=2", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS4:
         sprintf (rc, "[%s]+=4", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS6:
         sprintf (rc, "[%s]+=6", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS2:
         sprintf (rc, "[%s]-=2", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS4:
         sprintf (rc, "[%s]-=4", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS6:
         sprintf (rc, "[%s]-=6", PIC30_Y_PREFETCH_MAC1_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_INDIRECT:
         sprintf (rc, "[%s]", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS2:
         sprintf (rc, "[%s]+=2", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS4:
         sprintf (rc, "[%s]+=4", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS6:
         sprintf (rc, "[%s]+=6", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS_REG:
         sprintf (rc, "[%s+%s]", PIC30_Y_PREFETCH_MAC2_REGISTER,
                                 PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS2:
         sprintf (rc, "[%s]-=2", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS4:
         sprintf (rc, "[%s]-=4", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      case PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS6:
         sprintf (rc, "[%s]-=6", PIC30_Y_PREFETCH_MAC2_REGISTER);
         break;

      default:
         strcpy (rc, "");
         break;
   } /* switch (operation) */

   return rc;
} /* char * pic30_extract_dsp_y_prefetch_operation(...) */

/******************************************************************************/

char *
pic30_extract_dsp_multiplicand (insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts the DSP multiplicand operand from the given
 *   instruction.
 *
 ******************************************************************************/

{
   char * rc = malloc (PIC30_MAX_DSP_MULTIPLICAND_LENGTH + 1);
   long multiplicand = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);

   switch (multiplicand)
   {
      case PIC30_DSP_W4_W5_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W4_W5_MULTIPLICAND_STRING);
         break;

      case PIC30_DSP_W4_W6_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W4_W6_MULTIPLICAND_STRING);
         break;

      case PIC30_DSP_W4_W7_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W4_W7_MULTIPLICAND_STRING);
         break;

      case PIC30_DSP_W5_W6_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W5_W6_MULTIPLICAND_STRING);
         break;

      case PIC30_DSP_W5_W7_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W5_W7_MULTIPLICAND_STRING);
         break;

      case PIC30_DSP_W6_W7_MULTIPLICAND:
         strcpy (rc, PIC30_DSP_W6_W7_MULTIPLICAND_STRING);
         break;

      default:
         strcpy (rc, "???");
         break;
   }
   return rc;
} /* char * pic30_extract_dsp_multiplicand(...) */

/******************************************************************************/

char *
pic30_extract_dsp_square(insn, info, flags, opnd, err)
   unsigned long insn;
   struct disassemble_info * info __attribute__ ((__unused__));
   unsigned long flags __attribute__ ((__unused__));
   const struct pic30_operand * opnd;
   unsigned char * err __attribute__ ((__unused__));

/******************************************************************************
 *
 *   This function extracts the DSP square operand from the given instruction.
 *
 ******************************************************************************/

{
   long register_number = PIC30_EXTRACT_OPERAND (insn, opnd->bits, opnd->shift);
   char * rc = malloc(strlen("Wnn*Wnn") + 1);

   /*
    * Add the minimum register number.  This is done because the register
    * first DSP prefetch destination register is encoded as 0.
    */
   register_number += PIC30_MIN_DSP_PREFETCH_DST_REGISTER_VALUE;

   sprintf(rc, "w%ld * w%ld", register_number, register_number);

   return rc;
} /* char * pic30_extract_dsp_square(...) */

/******************************************************************************/

unsigned char
pic30_match_power_save_mode (check)
   const struct pic30_operand_value * check;

/******************************************************************************
 *
 *   This function ensures that the given operand value is valid for a
 *   PWRSAV mode operand.
 *
 ******************************************************************************/

{
   return ((check->modifier == PIC30_NO_MODIFIER_FOUND) &&
           (PIC30_IS_1_BIT_LITERAL (check->value)));
} /* unsigned char pic30_match_power_save_mode(...) */

/******************************************************************************/

const struct relocation_info
pic30_get_pc_relative_relocation_info ()
{
   return pic30_relocation_info[PIC30_RELOC_INFO_PCREL_BRANCH];
} /* const struct relocation_info pic30_get_pc_relative_relocation_info(void) */

/******************************************************************************/

