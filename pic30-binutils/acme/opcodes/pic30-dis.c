/*
 * pic30-dis.c -- Disassembler for the PIC30 architecture.
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 *
 * This file is part of GDB, GAS, and the GNU binutils.
 *
 * GDB, GAS, and the GNU binutils are free software; you can redistribute
 * them and/or modify them under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either version
 * 2, or (at your option) any later version.
 *
 * GDB, GAS, and the GNU binutils are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <stdio.h>
#include "sysdep.h"
#include <dis-asm.h>
#include <opcode/pic30.h>
#include "pic30-opc.h"
#include "../bfd/coff-pic30.h"

bfd_vma pic30_disassembly_address = 0; /* used by some extract functions
                                      in pic30-opc.c */

static unsigned char global_symbolic_disassembly = FALSE;

void pic30_parse_disassembler_options PARAMS (( char * option));

void pic30_disassemble_do_insn PARAMS
(( const struct pic30_opcode * opcode,
   unsigned long insn_word1, unsigned long insn_word2,
   struct disassemble_info * info));

void pic30_disassemble_call_goto_insn PARAMS
(( const struct pic30_opcode * opcode, unsigned long insn_word1,
   unsigned long insn_word2, struct disassemble_info *info));

char pic30_disassemble_2word_insn PARAMS
(( const struct pic30_opcode * opcode, unsigned long insn_word1,
   bfd_vma memaddr, struct disassemble_info *info));

unsigned char pic30_disassemble_1word_insn PARAMS
(( const struct pic30_opcode * opcode, unsigned long insn,
   struct disassemble_info *info));

#define PRINT_SYMBOL (print_symbol && global_symbolic_disassembly)

/* Disassemble PIC30 instructions.  */

/******************************************************************************/

void
pic30_parse_disassembler_options (option)
   char * option;

{
   if (option != NULL)
   {
      char * space = NULL;

      do {
         space = strchr (option, ' ');

         if (space)
            space = '\0';
         
         if (strcmp (option, "symbolic") == 0)
            global_symbolic_disassembly = TRUE;
         else
            fprintf (stderr, "Unrecognized disassembler option:  %s\n", option);

         if (space)
            option = space + 1;
      } while (space != NULL);
   } /* if (option != NULL) */
}

/******************************************************************************/

void
pic30_print_disassembler_options (stream)
   FILE * stream;
{
   fprintf (stream, "\nMicrochip specific disassembler options (supported with "
                    "-M):\n"
                    "  symbolic -- Include symbolic information during "
                    "disassembly.\n\n");
}

/******************************************************************************/

void
pic30_disassemble_do_insn (opcode, insn_word1, insn_word2, info)
   const struct pic30_opcode * opcode;
   unsigned long insn_word1;
   unsigned long insn_word2;
   struct disassemble_info * info;
{
   int i;

   (*info->fprintf_func) (info->stream, "%-10s", opcode->name);

   for (i = 0; i < opcode->number_of_operands; i++)
   {
      const struct pic30_operand * opnd =
         &(pic30_operands[opcode->operands[i]]);

      char operand_string[BUFSIZ] = "";
      unsigned long insn_word = (i == 0) ? insn_word1 : insn_word2;
      long operand_value = 0;
      unsigned char print_symbol = FALSE;

      if (opnd->extract)
      {
         char * extracted_string;
         unsigned char error_found = FALSE;

         extracted_string =
            (*opnd->extract) (insn_word, info,
                              opcode->flags, opnd, &error_found);
         strcpy (operand_string, extracted_string);
         free (extracted_string);
      } /* if (opnd->extract) */
      else
      {
         operand_value = ((insn_word) >> opnd->shift) & ((1 << opnd->bits) - 1);

         switch (opnd->type)
         {
            case OPND_REGISTER_DIRECT:
               strcpy (operand_string, pic30_registers[operand_value]);
               break;

            case OPND_SYMBOL:
               if ((*info->symbol_at_address_func) (operand_value, info))
                  print_symbol = TRUE;
               /* drop through */
            case OPND_VALUE:
            {
               if (opnd->immediate)
                  strcpy (operand_string, "#0x");
               else
                  strcpy (operand_string, "0x");

               if (!PRINT_SYMBOL)
                  sprintf (operand_string, "%s%lx", operand_string,
                           operand_value);
            }
         } /* switch */
      } /* else */

      (*info->fprintf_func) (info->stream, "%s", operand_string);

      if ((!opnd->extract) && (PRINT_SYMBOL))
         (*info->print_address_func) (operand_value, info);

      if ((i != (opcode->number_of_operands - 1)) &&
          (strcmp (operand_string, "") != 0))
         (*info->fprintf_func) (info->stream, ", ");
   }
}

/******************************************************************************/

void
pic30_disassemble_call_goto_insn (opcode, insn_word1, insn_word2, info)
   const struct pic30_opcode * opcode;
   unsigned long insn_word1;
   unsigned long insn_word2;
   struct disassemble_info *info;
{
  /* LC GSM */
/*    unsigned long LSB = ((insn_word1 & 0xFFFF) << 1); */
/*    unsigned long MSB = ((insn_word2 & 0xFFFF) << 17); */
   unsigned long LSB = ((insn_word1 & 0xFFFF));
   unsigned long MSB = ((insn_word2 & 0xFFFF) << 16);
   unsigned long addr = (MSB | LSB);

   info->flags_to_match = SEC_CODE;
   (*info->fprintf_func) (info->stream, "%-10s0x", opcode->name);
   (*info->print_address_func) (addr, info);
   info->flags_to_match = 0;
}

/******************************************************************************/

char
pic30_disassemble_2word_insn (opcode, insn_word1, memaddr, info)
   const struct pic30_opcode * opcode;
   unsigned long insn_word1;
   bfd_vma memaddr;
   struct disassemble_info *info;
{
   unsigned char rc = TRUE;
   int status;
   bfd_byte bytes_read[PIC30_SIZE_OF_PROGRAM_WORD + 1];
   int i;
   unsigned long insn_word2 = 0;

   status = (*info->read_memory_func) (memaddr + 2,
                                       bytes_read, PIC30_SIZE_OF_PROGRAM_WORD,
                                       info);

   if (status != 0)
      return -1;

   for (i = 0; i < PIC30_SIZE_OF_PROGRAM_WORD; i++)
      insn_word2 = insn_word2 | ((unsigned long)bytes_read[i] << (8 * i));

   switch (insn_word1 & opcode->mask)
   {
      case DO_INSN:
      case DOW_INSN:
         pic30_disassemble_do_insn (opcode, insn_word1, insn_word2, info);
         break;

      case CALL_INSN:
      case GOTO_INSN:
         pic30_disassemble_call_goto_insn (opcode, insn_word1,
                                           insn_word2, info);
         break;

      default:
         rc = FALSE;
         break;
   } /* switch (insn_word1) */

   return rc;
}

/******************************************************************************/

unsigned char
pic30_disassemble_1word_insn (opcode, insn, info)
   const struct pic30_opcode * opcode;
   unsigned long insn;
   struct disassemble_info *info;
{
   unsigned char rc = TRUE;
   int j;

   for (j = 0; j < opcode->number_of_operands; j++)
   {
      const struct pic30_operand * opnd =
         &(pic30_operands[opcode->operands[j]]);

      if (opnd->extract)
      {
         char * extracted_string;
         unsigned char error_found = FALSE;

         extracted_string =
            (*opnd->extract) (insn, info, opcode->flags, opnd, &error_found);

         free (extracted_string);

         if (error_found)
         {
            rc = FALSE;
            break;
         } /* if (error_found) */
      } /* if (opnd->extract) */
   } /* for j */

   if (rc)
   {
      (*info->fprintf_func) (info->stream, "%-10s", opcode->name);

      for (j = 0; j < opcode->number_of_operands; j++)
      {
         unsigned char print_symbol = FALSE;
         long operand_value = 0;
         char operand_string[BUFSIZ] = "";
         const struct pic30_operand * opnd =
            &(pic30_operands[opcode->operands[j]]);

         if (opnd->extract)
         {
            char * extracted_string;
            unsigned char error_found = FALSE;

            extracted_string =
               (*opnd->extract) (insn, info,
                                 opcode->flags, opnd, &error_found);
            strcpy (operand_string, extracted_string);
            free (extracted_string);
         } /* if (opnd->extract) */
         else
         {
            operand_value = ((insn) >> opnd->shift) & ((1 << opnd->bits) - 1);

            switch (opnd->type)
            {
               case OPND_REGISTER_DIRECT:
                  strcpy (operand_string, pic30_registers[operand_value]);
                  break;

               case OPND_SYMBOL:
                  if ((*info->symbol_at_address_func) (operand_value, info))
                     print_symbol = TRUE;
                  /* drop through */
               case OPND_VALUE:
               {
                  if (opnd->immediate)
                     strcpy (operand_string, "#0x");
                  else
                     strcpy (operand_string, "0x");

                  if (!PRINT_SYMBOL)
                     sprintf (operand_string, "%s%lx", operand_string,
                                                       operand_value);

                  break;
               }
            } /* switch */
         } /* else */

         (*info->fprintf_func) (info->stream, "%s", operand_string);

         if (PRINT_SYMBOL)
            (*info->print_address_func) (operand_value, info);

         if ((j != (opcode->number_of_operands - 1)) &&
             (strcmp (operand_string, "") != 0))
            (*info->fprintf_func) (info->stream, ", ");
      } /* for j */
   } /* if (rc) */

   return rc;
}

/******************************************************************************/

int
pic30_print_insn (memaddr, info)
   bfd_vma memaddr;
   struct disassemble_info *info;
{
   int rc = 0;
   bfd_byte bytes_read[PIC30_SIZE_OF_PROGRAM_WORD + 1];
   unsigned long insn = 0;
   int opb = info->octets_per_byte;
   int status;
   int i,j;
   int is_ecore = pic30_is_ecore_machine(bfd_lookup_arch(info->arch,info->mach));

   pic30_disassembly_address = memaddr; /* make insn address visible */

   if (info->disassembler_options)
   {
      pic30_parse_disassembler_options (info->disassembler_options);
      
      /* To avoid repeated parsing of these options, we remove them here.  */
      info->disassembler_options = NULL;
   } /* if (info->disassembler_options) */

   if (info->section == NULL) 
     return rc;

   if (PIC30_DISPLAY_AS_DATA_MEMORY(info->section))
   {
      status = (*info->read_memory_func) (memaddr, bytes_read,
                                          PIC30_SIZE_OF_DATA_WORD * opb, info);

      if (status != 0)
      {
         (*info->memory_error_func) (status, memaddr, info);
         return -1;
      } /* if (status != 0) */
   
      for (i = 0, j = 0; i < PIC30_SIZE_OF_DATA_WORD * opb; i++)
        if (PIC30_CAN_PRINT_IN_DATA_MEMORY(i))
          {
            insn = insn | ((unsigned long)bytes_read[i] << (8 * j));
            j++;
          }

      (*info->fprintf_func) (info->stream, ".word %#04x", insn);

      rc = PIC30_SIZE_OF_DATA_WORD * opb;
   } /* if (PIC30_DISPLAY_AS_DATA_MEMORY(info->section)) */

   else if (PIC30_DISPLAY_AS_PROGRAM_MEMORY(info->section))
   {
      unsigned char found_insn = FALSE;

      status = (*info->read_memory_func) (memaddr, bytes_read,
                                          PIC30_SIZE_OF_PROGRAM_WORD, info);

      if (status != 0)
      {
         (*info->memory_error_func) (status, memaddr, info);
         return -1;
      } /* if (status != 0) */

      for (i = 0; i < PIC30_SIZE_OF_PROGRAM_WORD; i++)
         insn = insn | ((unsigned long)bytes_read[i] << (8 * i));

      for (i = 0; i < pic30_num_opcodes; i++)
      {
	
	 if ((pic30_opcodes[i].flags & F_ECORE) && !is_ecore) continue;
         if ((pic30_opcodes[i].flags & F_FCORE) && is_ecore) continue;

         if ((pic30_opcodes[i].number_of_words != 0) &&
             ((pic30_opcodes[i].mask & insn) == pic30_opcodes[i].opcode[0]))
         {
            if (pic30_opcodes[i].number_of_words == 2)
            {
               char rc = pic30_disassemble_2word_insn(&pic30_opcodes[i],
                                                      insn, memaddr, info);

               if (rc < 0)  
                  found_insn = FALSE;
               else
                  found_insn = (unsigned char)rc;
            } /* if (pic30_opcodes[i].number_of_words == 2) */
            else if (pic30_opcodes[i].number_of_words == 1)
               found_insn = pic30_disassemble_1word_insn(&pic30_opcodes[i],
                                                         insn, info);
            else
               found_insn = FALSE;

            break;
         } /* if ((pic30_opcodes[i].mask & insn) == pic30_opcodes[i].opcode) */
      } /* for i */

      rc = PIC30_SIZE_OF_PROGRAM_WORD;

      if (found_insn)
         rc *= pic30_opcodes[i].number_of_words;
      else
         (*info->fprintf_func) (info->stream, ".pword %#08x", insn);
   } /* else if (PIC30_DISPLAY_AS_PROGRAM_MEMORY(info->section))*/

   else if (PIC30_DISPLAY_AS_READONLY_MEMORY(info->section))
     {
       status = (*info->read_memory_func) (memaddr, bytes_read,
                                           PIC30_SIZE_OF_PROGRAM_WORD, info);

       if (status != 0)
         {
           (*info->memory_error_func) (status, memaddr, info);
           return -1;
         } /* if (status != 0) */

       for (i = 0; i < PIC30_SIZE_OF_PROGRAM_WORD; i++)
         insn = insn | ((unsigned long)bytes_read[i] << (8 * i));

       rc = PIC30_SIZE_OF_PROGRAM_WORD;

       (*info->fprintf_func) (info->stream, ".word %#0x", insn & 0xFFFF);
     } /* else if (PIC30_DISPLAY_AS_READONLY_MEMORY(info->section)) */

   return rc;
}
