/* listing.c - mainting assembly listings
   Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002
   Free Software Foundation, Inc.

This file is part of GAS, the GNU Assembler.

GAS is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GAS; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/*
 Contributed by Steve Chamberlain <sac@cygnus.com>

 A listing page looks like:

 LISTING_HEADER  sourcefilename pagenumber
 TITLE LINE
 SUBTITLE LINE
 linenumber address data  source
 linenumber address data  source
 linenumber address data  source
 linenumber address data  source

 If not overridden, the listing commands are:

 .title  "stuff"
 	Put "stuff" onto the title line
 .sbttl  "stuff"
        Put stuff onto the subtitle line

  If these commands come within 10 lines of the top of the page, they
  will affect the page they are on, as well as any subsequent page

 .eject
 	Thow a page
 .list
 	Increment the enable listing counter
 .nolist
 	Decrement the enable listing counter

 .psize Y[,X]
 	Set the paper size to X wide and Y high. Setting a psize Y of
	zero will suppress form feeds except where demanded by .eject

 If the counter goes below zero, listing is suppressed.

 Listings are a maintained by read calling various listing_<foo>
 functions.  What happens most is that the macro NO_LISTING is not
 defined (from the Makefile), then the macro LISTING_NEWLINE expands
 into a call to listing_newline.  The call is done from read.c, every
 time it sees a newline, and -l is on the command line.

 The function listing_newline remembers the frag associated with the
 newline, and creates a new frag - note that this is wasteful, but not
 a big deal, since listing slows things down a lot anyway.  The
 function also rememebers when the filename changes.

 When all the input has finished, and gas has had a chance to settle
 down, the listing is output. This is done by running down the list of
 frag/source file records, and opening the files as needed and printing
 out the bytes and chars associated with them.

 The only things which the architecture can change about the listing
 are defined in these macros:

 LISTING_HEADER		The name of the architecture
 LISTING_WORD_SIZE      The make of the number of bytes in a word, this determines
 			the clumping of the output data. eg a value of
			2 makes words look like 1234 5678, whilst 1
			would make the same value look like 12 34 56
			78
 LISTING_LHS_WIDTH      Number of words of above size for the lhs

 LISTING_LHS_WIDTH_SECOND   Number of words for the data on the lhs
 			for the second line

 LISTING_LHS_CONT_LINES	Max number of lines to use up for a continutation
 LISTING_RHS_WIDTH      Number of chars from the input file to print
                        on a line
*/

#include "as.h"
#include "obstack.h"
#include "safe-ctype.h"
#include "input-file.h"
#include "subsegs.h"
#if defined(TC_PIC30) && defined(__MINGW32__)
#include <fcntl.h>
#endif

#ifndef NO_LISTING

#ifndef LISTING_HEADER
#define LISTING_HEADER "GAS LISTING"
#endif
#ifndef LISTING_WORD_SIZE
#define LISTING_WORD_SIZE 4
#endif
#ifndef LISTING_LHS_WIDTH
#define LISTING_LHS_WIDTH ((LISTING_WORD_SIZE) > 4 ? 1 : 4 / (LISTING_WORD_SIZE))
#endif
#ifndef LISTING_LHS_WIDTH_SECOND
#define LISTING_LHS_WIDTH_SECOND LISTING_LHS_WIDTH
#endif
#ifndef LISTING_RHS_WIDTH
#define LISTING_RHS_WIDTH 100
#endif
#ifndef LISTING_LHS_CONT_LINES
#define LISTING_LHS_CONT_LINES 4
#endif

/* This structure remembers which .s were used.  */
typedef struct file_info_struct {
  struct file_info_struct * next;
  char *                    filename;
  long                      pos;
  unsigned int              linenum;
  int                       at_end;
} file_info_type;

/* This structure rememebrs which line from which file goes into which
   frag.  */
struct list_info_struct {
  /* Frag which this line of source is nearest to.  */
  fragS *frag;

  /* The actual line in the source file.  */
  unsigned int line;
  /* Pointer to the file info struct for the file which this line
     belongs to.  */
  file_info_type *file;

  /* The expanded text of any macro that may have been executing.  */
  char *line_contents;

  /* Next in list.  */
  struct list_info_struct *next;

  /* Pointer to the file info struct for the high level language
     source line that belongs here.  */
  file_info_type *hll_file;
  /* High level language source line.  */
  unsigned int hll_line;

  /* Pointer to any error message associated with this line.  */
  char *message;

  enum {
    EDICT_NONE,
    EDICT_SBTTL,
    EDICT_TITLE,
    EDICT_NOLIST,
    EDICT_LIST,
    EDICT_NOLIST_NEXT,
    EDICT_EJECT
  } edict;
  char *edict_arg;

  /* Nonzero if this line is to be omitted because it contains
     debugging information.  This can become a flags field if we come
     up with more information to store here.  */
  int debugging;
};

typedef struct list_info_struct list_info_type;

int listing_lhs_width        = LISTING_LHS_WIDTH;
int listing_lhs_width_second = LISTING_LHS_WIDTH_SECOND;
int listing_lhs_cont_lines   = LISTING_LHS_CONT_LINES;
int listing_rhs_width        = LISTING_RHS_WIDTH;

#if TC_PIC30
int user_listing_lhs_width        = 0;
int user_listing_lhs_width_second = 0;
#endif

struct list_info_struct *        listing_tail;

static file_info_type *          file_info_head;
static file_info_type *          last_open_file_info;
static FILE *                    last_open_file;
static struct list_info_struct * head;
static int                       paper_width = 200;
static int                       paper_height = 60;

extern int                       listing;

/* File to output listings to.  */
static FILE *list_file;

/* This static array is used to keep the text of data to be printed
   before the start of the line.  */

#define MAX_BYTES							\
  (((LISTING_WORD_SIZE * 2) + 1) * listing_lhs_width			\
   + ((((LISTING_WORD_SIZE * 2) + 1) * listing_lhs_width_second)	\
      * listing_lhs_cont_lines)						\
   + 20)

static char *data_buffer;

/* Prototypes.  */
static void listing_message PARAMS ((const char *name, const char *message));
static file_info_type *file_info PARAMS ((const char *file_name));
static void new_frag PARAMS ((void));
static char *buffer_line PARAMS ((file_info_type *file,
				  char *line, unsigned int size));
static void listing_page PARAMS ((list_info_type *list));
static unsigned int calc_hex PARAMS ((list_info_type *list));
static void print_lines PARAMS ((list_info_type *, unsigned int,
				 char *, unsigned int));
static void list_symbol_table PARAMS ((void));
static void print_source PARAMS ((file_info_type *current_file,
				  list_info_type *list,
				  char *buffer,
				  unsigned int width));
static int debugging_pseudo PARAMS ((list_info_type *, const char *));
static void listing_listing PARAMS ((char *name));
#ifdef BFD_ASSEMBLER
static void list_section_information PARAMS ((void));
#endif

static void
listing_message (name, message)
     const char *name;
     const char *message;
{
  if (listing_tail != (list_info_type *) NULL)
    {
      unsigned int l = strlen (name) + strlen (message) + 1;
      char *n = (char *) xmalloc (l);
      strcpy (n, name);
      strcat (n, message);
      listing_tail->message = n;
    }
}

void
listing_warning (message)
     const char *message;
{
  listing_message (_("Warning:"), message);
}

void
listing_error (message)
     const char *message;
{
  listing_message (_("Error:"), message);
}

static file_info_type *
file_info (file_name)
     const char *file_name;
{
  /* Find an entry with this file name.  */
  file_info_type *p = file_info_head;

  while (p != (file_info_type *) NULL)
    {
      if (strcmp (p->filename, file_name) == 0)
	return p;
      p = p->next;
    }

  /* Make new entry.  */

  p = (file_info_type *) xmalloc (sizeof (file_info_type));
  p->next = file_info_head;
  file_info_head = p;
  p->filename = xstrdup (file_name);
  p->pos = 0;
  p->linenum = 0;
  p->at_end = 0;

  return p;
}

static void
new_frag ()
{

  frag_wane (frag_now);
  frag_new (0);

}

void
listing_newline (ps)
     char *ps;
{
  char *file;
  unsigned int line;
  static unsigned int last_line = 0xffff;
  static char *last_file = NULL;
  list_info_type *new = NULL;

  if (listing == 0)
    return;

  if (now_seg == absolute_section)
    return;

#ifdef OBJ_ELF
  /* In ELF, anything in a section beginning with .debug or .line is
     considered to be debugging information.  This includes the
     statement which switches us into the debugging section, which we
     can only set after we are already in the debugging section.  */
  if ((listing & LISTING_NODEBUG) != 0
      && listing_tail != NULL
      && ! listing_tail->debugging)
    {
      const char *segname;

      segname = segment_name (now_seg);
      if (strncmp (segname, ".debug", sizeof ".debug" - 1) == 0
	  || strncmp (segname, ".line", sizeof ".line" - 1) == 0)
	listing_tail->debugging = 1;
    }
#endif

  as_where (&file, &line);
  if (ps == NULL)
    {
      if (line == last_line
	  && !(last_file && file && strcmp (file, last_file)))
	return;

      new = (list_info_type *) xmalloc (sizeof (list_info_type));

      /* Detect if we are reading from stdin by examining the file
	 name returned by as_where().

	 [FIXME: We rely upon the name in the strcmp below being the
	 same as the one used by input_scrub_new_file(), if that is
	 not true, then this code will fail].

	 If we are reading from stdin, then we need to save each input
	 line here (assuming of course that we actually have a line of
	 input to read), so that it can be displayed in the listing
	 that is produced at the end of the assembly.  */
      if (strcmp (file, _("{standard input}")) == 0
	  && input_line_pointer != NULL)
	{
	  char *copy;
	  int len;
	  int seen_quote = 0;

	  for (copy = input_line_pointer - 1;
	       *copy && (seen_quote
			 || (! is_end_of_line [(unsigned char) *copy]));
	       copy++)
	    if (*copy == '"' && copy[-1] != '\\')
	      seen_quote = ! seen_quote;

	  len = (copy - input_line_pointer) + 2;

	  copy = xmalloc (len);

	  if (copy != NULL)
	    {
	      char *src = input_line_pointer - 1;
	      char *dest = copy;

	      while (--len)
		{
		  unsigned char c = *src++;

		  /* Omit control characters in the listing.  */
		  if (!ISCNTRL (c))
		    *dest++ = c;
		}

	      *dest = 0;
	    }

	  new->line_contents = copy;
	}
      else
	new->line_contents = NULL;
    }
  else
    {
      new = (list_info_type *) xmalloc (sizeof (list_info_type));
      new->line_contents = ps;
    }

  last_line = line;
  last_file = file;

  new_frag ();

  if (listing_tail)
    listing_tail->next = new;
  else
    head = new;

  listing_tail = new;

  new->frag = frag_now;
  new->line = line;
  new->file = file_info (file);
  new->next = (list_info_type *) NULL;
  new->message = (char *) NULL;
  new->edict = EDICT_NONE;
  new->hll_file = (file_info_type *) NULL;
  new->hll_line = 0;
  new->debugging = 0;

  new_frag ();

#ifdef OBJ_ELF
  /* In ELF, anything in a section beginning with .debug or .line is
     considered to be debugging information.  */
  if ((listing & LISTING_NODEBUG) != 0)
    {
      const char *segname;

      segname = segment_name (now_seg);
      if (strncmp (segname, ".debug", sizeof ".debug" - 1) == 0
	  || strncmp (segname, ".line", sizeof ".line" - 1) == 0)
	new->debugging = 1;
    }
#endif
}

/* Attach all current frags to the previous line instead of the
   current line.  This is called by the MIPS backend when it discovers
   that it needs to add some NOP instructions; the added NOP
   instructions should go with the instruction that has the delay, not
   with the new instruction.  */

void
listing_prev_line ()
{
  list_info_type *l;
  fragS *f;

  if (head == (list_info_type *) NULL
      || head == listing_tail)
    return;

  new_frag ();

  for (l = head; l->next != listing_tail; l = l->next)
    ;

  for (f = frchain_now->frch_root; f != (fragS *) NULL; f = f->fr_next)
    if (f->line == listing_tail)
      f->line = l;

  listing_tail->frag = frag_now;
  new_frag ();
}

/* This function returns the next source line from the file supplied,
   truncated to size.  It appends a fake line to the end of each input
   file to make.  */

static char *
buffer_line (file, line, size)
     file_info_type *file;
     char *line;
     unsigned int size;
{
  unsigned int count = 0;
  int c;

  char *p = line;

  /* If we couldn't open the file, return an empty line.  */
  if (file->at_end)
    return "";

  /* Check the cache and see if we last used this file.  */
  if (!last_open_file_info || file != last_open_file_info)
    {
      if (last_open_file)
	{
	  last_open_file_info->pos = ftell (last_open_file);
	  fclose (last_open_file);
	}

      last_open_file_info = file;
      last_open_file = fopen (file->filename, FOPEN_RT);
      if (last_open_file == NULL)
	{
	  file->at_end = 1;
	  return "";
	}
#if defined(TC_PIC30) && defined(__MINGW32__)
      _setmode(_fileno(last_open_file), _O_BINARY);
#endif

      /* Seek to where we were last time this file was open.  */
      if (file->pos)
	fseek (last_open_file, file->pos, SEEK_SET);
    }

  c = fgetc (last_open_file);

  /* Leave room for null.  */
  size -= 1;

  while (c != EOF && c != '\n')
    {
#if defined(TC_PIC30) && defined(__MINGW32__)
      if (c == '\r')
      {
         int c0 = c;
         c = fgetc (last_open_file);
         if ((c == EOF) || (c == '\n'))
         {
             break;
         }
         ungetc(c, last_open_file);
         c = c0;
      }
#endif
      if (count < size)
	*p++ = c;
      count++;

      c = fgetc (last_open_file);

    }
  if (c == EOF)
    {
      file->at_end = 1;
      if (count + 2 < size)
	{
	  *p++ = '.';
	  *p++ = '.';
	  *p++ = '.';
	}
    }
  file->linenum++;
  *p++ = 0;
  return line;
}

static const char *fn;

static unsigned int eject;	/* Eject pending */
static unsigned int page;	/* Current page number */
static char *title;		/* Current title */
static char *subtitle;		/* Current subtitle */
static unsigned int on_page;	/* Number of lines printed on current page */

static void
listing_page (list)
     list_info_type *list;
{
  /* Grope around, see if we can see a title or subtitle edict coming up
     soon.  (we look down 10 lines of the page and see if it's there)  */
  if ((eject || (on_page >= (unsigned int) paper_height))
      && paper_height != 0)
    {
      unsigned int c = 10;
      int had_title = 0;
      int had_subtitle = 0;

      page++;

      while (c != 0 && list)
	{
	  if (list->edict == EDICT_SBTTL && !had_subtitle)
	    {
	      had_subtitle = 1;
	      subtitle = list->edict_arg;
	    }
	  if (list->edict == EDICT_TITLE && !had_title)
	    {
	      had_title = 1;
	      title = list->edict_arg;
	    }
	  list = list->next;
	  c--;
	}

      if (page > 1)
	{
	  fprintf (list_file, "\f");
	}

      fprintf (list_file, "%s %s \t\t\tpage %d\n", LISTING_HEADER, fn, page);
      fprintf (list_file, "%s\n", title);
      fprintf (list_file, "%s\n", subtitle);
      on_page = 3;
      eject = 0;
    }
}

static unsigned int
calc_hex (list)
     list_info_type *list;
{
  int data_buffer_size;
  list_info_type *first = list;
  unsigned int address = ~(unsigned int) 0;
  fragS *frag;
  fragS *frag_ptr;
  unsigned int octet_in_frag;

  /* Find first frag which says it belongs to this line.  */
  frag = list->frag;
  while (frag && frag->line != list)
    frag = frag->fr_next;

  frag_ptr = frag;

  data_buffer_size = 0;

  /* Dump all the frags which belong to this line.  */
  while (frag_ptr != (fragS *) NULL && frag_ptr->line == first)
    {
      /* Print as many bytes from the fixed part as is sensible.  */
      octet_in_frag = 0;
      while ((offsetT) octet_in_frag < frag_ptr->fr_fix
	     && data_buffer_size < MAX_BYTES - 3)
	{
	  if (address == ~(unsigned int) 0)
	    {
	      address = frag_ptr->fr_address / OCTETS_PER_BYTE;
	    }

#if TC_PIC30
/*           printf("frag = %lu, address = %d, program = %s\n", */
/*                  frag_ptr, */
/*                  frag_ptr->fr_address + octet_in_frag, */
/*                  frag_ptr->tc_frag_data.in_text_section ? "TRUE" : "FALSE"); */
          if ((PIC30_FRAG_IN_PROGRAM_MEMORY (frag_ptr) &&
              PIC30_CAN_WRITE ((frag_ptr->fr_address + octet_in_frag), TRUE)) ||
              (PIC30_FRAG_IN_DATA_MEMORY (frag_ptr) &&
               PIC30_CAN_WRITE_DATA (frag_ptr->fr_address + octet_in_frag)))
            {
#endif
	  sprintf (data_buffer + data_buffer_size,
		   "%02X",
		   (frag_ptr->fr_literal[octet_in_frag]) & 0xff);
	  data_buffer_size += 2;
#if TC_PIC30
            }
#endif
	  octet_in_frag++;
	}
      if (frag_ptr->fr_type == rs_fill)
	{
	  unsigned int var_rep_max = octet_in_frag;
	  unsigned int var_rep_idx = octet_in_frag;

	  /* Print as many bytes from the variable part as is sensible.  */
	  while (((offsetT) octet_in_frag
		  < (frag_ptr->fr_fix + frag_ptr->fr_var * frag_ptr->fr_offset))
		 && data_buffer_size < MAX_BYTES - 3)
	    {
	      if (address == ~(unsigned int) 0)
		{
		  address = frag_ptr->fr_address / OCTETS_PER_BYTE;
		}
#if TC_PIC30
/*             printf("frag = %lu, address = %d, program = %s\n", */
/*                    frag_ptr, */
/*                    frag_ptr->fr_address + octet_in_frag, */
/*                    frag_ptr->tc_frag_data.in_text_section ? "TRUE" : "FALSE"); */
            if ((PIC30_FRAG_IN_PROGRAM_MEMORY (frag_ptr) &&
                 PIC30_CAN_WRITE ((frag_ptr->fr_address + octet_in_frag), TRUE)) ||
                (PIC30_FRAG_IN_DATA_MEMORY (frag_ptr) &&
                 PIC30_CAN_WRITE_DATA (frag_ptr->fr_address + octet_in_frag)))
            {
#endif
	      sprintf (data_buffer + data_buffer_size,
		       "%02X",
		       (frag_ptr->fr_literal[var_rep_idx]) & 0xff);
#if 0
	      data_buffer[data_buffer_size++] = '*';
	      data_buffer[data_buffer_size++] = '*';
#endif
	      data_buffer_size += 2;
#if TC_PIC30
            }
#endif

	      var_rep_idx++;
	      octet_in_frag++;

	      if ((offsetT) var_rep_idx >= frag_ptr->fr_fix + frag_ptr->fr_var)
		var_rep_idx = var_rep_max;
	    }
	}

      frag_ptr = frag_ptr->fr_next;
    }
  data_buffer[data_buffer_size] = '\0';
  return address;
}

static void
print_lines (list, lineno, string, address)
     list_info_type *list;
     unsigned int lineno;
     char *string;
     unsigned int address;
{
  unsigned int idx;
  unsigned int nchars;
  unsigned int lines;
  unsigned int octet_in_word = 0;
  char *src = data_buffer;
  int cur;

#if TC_PIC30
  int address_printed = 0;
#if 0
  printf("--> print_lines (lineno=%d, address=%d)::begin\n",
         lineno, address);
  printf("    list->frag->fr_address = %lx\n", list->frag->fr_address);
#endif
  /*
   * If the user has not specified a listing_lhs_width,
   * set the default based on where the frag is located.
   */
  if (user_listing_lhs_width == 0)
    listing_lhs_width =
      (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag) ?
       3 : LISTING_LHS_WIDTH);

  if (user_listing_lhs_width_second == 0)
    listing_lhs_width_second =
      (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag) ?
       3 : LISTING_LHS_WIDTH_SECOND);
#endif

  /* Print the stuff on the first line.  */
  listing_page (list);
  nchars = (LISTING_WORD_SIZE * 2 + 1) * listing_lhs_width;

  /* Print the hex for the first line.  */
  if (address == ~(unsigned int) 0)
    {
      fprintf (list_file, "% 4d     ", lineno);
      for (idx = 0; idx < nchars; idx++)
	fprintf (list_file, " ");

      fprintf (list_file, "\t%s\n", string ? string : "");

      on_page++;

      listing_page (0);

      return;
    }

#ifdef TC_PIC30
#if 0
  printf("frag = %lu, address = %lx, program = %s\n",
         (unsigned long) list->frag,
         list->frag->fr_address,
         list->frag->tc_frag_data.in_text_section ? "TRUE" : "FALSE");
#endif
  if (had_errors ())
    {
      if (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag))
          fprintf (list_file, "% 4d ??????  ", lineno);
      else
          fprintf (list_file, "% 4d ???? ", lineno);
    } /*  if (had_errors ()) */
  else if (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag))
    {
      if (PIC30_VALID_PC_ADDRESS (list->frag->fr_address))
        {
          fprintf (list_file, "% 4d %06x  ", lineno, address);
          address_printed++;
        }
      else
          fprintf (list_file, "% 4d %s ", lineno, "       ");
    } /* else if (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag)) */
  else
    fprintf (list_file, "% 4d %04x ", lineno, address);
#else
  if (had_errors ())
    fprintf (list_file, "% 4d ???? ", lineno);
  else
    fprintf (list_file, "% 4d %04x ", lineno, address);
#endif

#ifdef TC_PIC30
  /*
  ** First line is special. In program memory
  ** anything less than 3 bytes is right-justified,
  ** so that its position on the line corresponds
  ** to its position in the current program word.
  **
  ** If the user has specified a really small
  ** listing_lhs_width, don't bother with the
  ** extra formatting.
  */
  if ((PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag)) && (listing_lhs_width > 2))
    switch (list->frag->fr_address % PIC30_SIZE_OF_PROGRAM_WORD)
      {
      case 2:
        /* upper byte */
        nchars -= 3;
        fprintf (list_file, "   ");
      case 1:
        /* middle byte */
        nchars -= 3;
        fprintf (list_file, "   ");
      }
#endif

  /* And the data to go along with it.  */
  idx = 0;
  cur = 0;
  while (src[cur] && idx < nchars)
    {
      int offset;
      offset = cur;
      fprintf (list_file, "%c%c", src[offset], src[offset + 1]);
      cur += 2;
      octet_in_word++;

      if (octet_in_word == LISTING_WORD_SIZE)
	{
	  fprintf (list_file, " ");
	  idx++;
	  octet_in_word = 0;
	}

      idx += 2;
    }

  for (; idx < nchars; idx++)
    fprintf (list_file, " ");

  fprintf (list_file, "\t%s\n", string ? string : "");
  on_page++;
  listing_page (list);

  if (list->message)
    {
      fprintf (list_file, "****  %s\n", list->message);
      listing_page (list);
      on_page++;
    }

  for (lines = 0;
       lines < (unsigned int) listing_lhs_cont_lines
	 && src[cur];
       lines++)
    {
      nchars = ((LISTING_WORD_SIZE * 2) + 1) * listing_lhs_width_second - 1;
      idx = 0;

      /* Print any more lines of data, but more compactly.  */
#if TC_PIC30
      if (PIC30_FRAG_IN_PROGRAM_MEMORY (list->frag))
        {
          if (address_printed)
            fprintf (list_file, "% 4d         ", lineno);
          else
            {
              fprintf (list_file, "% 4d %06x  ", lineno,
                       (address + 2) & ~1);
              address_printed++;
            }
        }
      else
#endif
      fprintf (list_file, "% 4d      ", lineno);

      while (src[cur] && idx < nchars)
	{
	  int offset;
	  offset = cur;
	  fprintf (list_file, "%c%c", src[offset], src[offset + 1]);
	  cur += 2;
	  idx += 2;
	  octet_in_word++;

	  if (octet_in_word == LISTING_WORD_SIZE)
	    {
	      fprintf (list_file, " ");
	      idx++;
	      octet_in_word = 0;
	    }
	}

      fprintf (list_file, "\n");
      on_page++;
      listing_page (list);
    }
}

static void
list_symbol_table ()
{
  extern symbolS *symbol_rootP;
  int got_some = 0;

  symbolS *ptr;
  eject = 1;
  listing_page (0);

  for (ptr = symbol_rootP; ptr != (symbolS *) NULL; ptr = symbol_next (ptr))
    {
      if (SEG_NORMAL (S_GET_SEGMENT (ptr))
	  || S_GET_SEGMENT (ptr) == absolute_section)
	{
#ifdef BFD_ASSEMBLER
	  /* Don't report section symbols.  They are not interesting.  */
	  if (symbol_section_p (ptr))
	    continue;
#endif
	  if (S_GET_NAME (ptr))
	    {
	      char buf[30], fmt[8];
	      valueT val = S_GET_VALUE (ptr);

	      /* @@ Note that this is dependent on the compilation options,
		 not solely on the target characteristics.  */
	      if (sizeof (val) == 4 && sizeof (int) == 4)
		sprintf (buf, "%08lx", (unsigned long) val);
	      else if (sizeof (val) <= sizeof (unsigned long))
		{
		  sprintf (fmt, "%%0%lulx",
			   (unsigned long) (sizeof (val) * 2));
		  sprintf (buf, fmt, (unsigned long) val);
		}
#if defined (BFD64)
	      else if (sizeof (val) > 4)
		sprintf_vma (buf, val);
#endif
	      else
		abort ();

	      if (!got_some)
		{
		  fprintf (list_file, "DEFINED SYMBOLS\n");
		  on_page++;
		  got_some = 1;
		}

	      if (symbol_get_frag (ptr) && symbol_get_frag (ptr)->line)
		{
		  fprintf (list_file, "%20s:%-5d  %s:%s %s\n",
			   symbol_get_frag (ptr)->line->file->filename,
			   symbol_get_frag (ptr)->line->line,
			   segment_name (S_GET_SEGMENT (ptr)),
			   buf, S_GET_NAME (ptr));
		}
	      else
		{
		  fprintf (list_file, "%33s:%s %s\n",
			   segment_name (S_GET_SEGMENT (ptr)),
			   buf, S_GET_NAME (ptr));
		}

	      on_page++;
	      listing_page (0);
	    }
	}

    }
  if (!got_some)
    {
      fprintf (list_file, "NO DEFINED SYMBOLS\n");
      on_page++;
    }
  fprintf (list_file, "\n");
  on_page++;
  listing_page (0);

  got_some = 0;

  for (ptr = symbol_rootP; ptr != (symbolS *) NULL; ptr = symbol_next (ptr))
    {
      if (S_GET_NAME (ptr) && strlen (S_GET_NAME (ptr)) != 0)
	{
	  if (S_GET_SEGMENT (ptr) == undefined_section)
	    {
	      if (!got_some)
		{
		  got_some = 1;
		  fprintf (list_file, "UNDEFINED SYMBOLS\n");
		  on_page++;
		  listing_page (0);
		}
	      fprintf (list_file, "%s\n", S_GET_NAME (ptr));
	      on_page++;
	      listing_page (0);
	    }
	}
    }
  if (!got_some)
    {
      fprintf (list_file, "NO UNDEFINED SYMBOLS\n");
      on_page++;
      listing_page (0);
    }

#ifdef TC_PIC30
  /* emit target-specific symbol info */
#ifdef tc_list_symbol_information
   tc_list_symbol_information (list_file);
#endif
#endif

}

#ifdef BFD_ASSEMBLER
#ifndef tc_list_section_information
static void
dump_section (abfd, sec, fp)
     bfd *abfd ATTRIBUTE_UNUSED;
     asection *sec;
     PTR fp;
{
  FILE * out_file = (FILE *) fp;
  char *comma = "";
  unsigned int opb = bfd_octets_per_byte (abfd);

  fprintf (out_file, "%3d %-13s %08lx  ", sec->index,
          bfd_get_section_name (abfd, sec),
          (unsigned long) bfd_section_size (abfd, sec) / opb);
  fprintf_vma (out_file, bfd_get_section_vma (abfd, sec));
  fprintf (out_file, "  ");
  fprintf_vma (out_file, sec->lma);
  fprintf (out_file, "  %08lx  2**%u", sec->filepos,
          bfd_get_section_alignment (abfd, sec));
  fprintf (out_file, "\n                ");
  fprintf (out_file, "  ");

#define PF(x, y) \
  if (sec->flags & x) { fprintf (out_file, "%s%s", comma, y); comma = ", "; }

  PF (SEC_HAS_CONTENTS, "CONTENTS");
  PF (SEC_ALLOC, "ALLOC");
  PF (SEC_CONSTRUCTOR, "CONSTRUCTOR");
#ifdef SEC_CONSTRUCTOR_TEXT
  PF (SEC_CONSTRUCTOR_TEXT, "CONSTRUCTOR TEXT");
#endif
#ifdef SEC_CONSTRUCTOR_DATA
  PF (SEC_CONSTRUCTOR_DATA, "CONSTRUCTOR DATA");
#endif
#ifdef SEC_CONSTRUCTOR_BSS
  PF (SEC_CONSTRUCTOR_BSS, "CONSTRUCTOR BSS");
#endif
  PF (SEC_LOAD, "LOAD");
  PF (SEC_RELOC, "RELOC");
#ifdef SEC_BALIGN
  PF (SEC_BALIGN, "BALIGN");
#endif
  PF (SEC_READONLY, "READONLY");
  PF (SEC_CODE, "CODE");
  PF (SEC_DATA, "DATA");
  PF (SEC_ROM, "ROM");
  PF (SEC_DEBUGGING, "DEBUGGING");
  PF (SEC_NEVER_LOAD, "NEVER_LOAD");
  PF (SEC_EXCLUDE, "EXCLUDE");
  PF (SEC_SORT_ENTRIES, "SORT_ENTRIES");
  PF (SEC_BLOCK, "BLOCK");
  PF (SEC_CLINK, "CLINK");
  PF (SEC_SMALL_DATA, "SMALL_DATA");
  PF (SEC_SHARED, "SHARED");

#ifdef PIC30
#define PF2(x, y) \
  if (section->x) { printf ("%s%s", comma, y); comma = ", "; }

  PF2 (near, "NEAR");
  PF2 (persistent, "PERSIST");
  PF2 (xmemory, "XMEMORY");
  PF2 (ymemory, "YMEMORY");
  PF2 (psv, "PSV");
  PF2 (eedata, "EEDATA");
  PF2 (absolute, "ABSOLUTE");
  PF2 (reverse, "REVERSE");
  PF2 (heap, "HEAP");
  PF2 (stack, "STACK");
  PF2 (dma, "DMA");
  PF2 (boot, "BOOT");
  PF2 (secure, "SECURE");
  PF2 (memory, "MEMORY");
  PF2 (eds, "EDS");
  PF2 (page, "PAGE");
  PF2 (auxflash, "AUXFLASH");

#undef PF2
#endif

  if ((sec->flags & SEC_LINK_ONCE) != 0)
    {
      const char *ls;

      switch (sec->flags & SEC_LINK_DUPLICATES)
        {
        default:
          abort ();
        case SEC_LINK_DUPLICATES_DISCARD:
          ls = "LINK_ONCE_DISCARD";
          break;
        case SEC_LINK_DUPLICATES_ONE_ONLY:
          ls = "LINK_ONCE_ONE_ONLY";
          break;
        case SEC_LINK_DUPLICATES_SAME_SIZE:
          ls = "LINK_ONCE_SAME_SIZE";
          break;
        case SEC_LINK_DUPLICATES_SAME_CONTENTS:
          ls = "LINK_ONCE_SAME_CONTENTS";
          break;
        }
      fprintf (out_file, "%s%s", comma, ls);

      if (sec->comdat != NULL)
        fprintf (out_file, " (COMDAT %s %ld)", sec->comdat->name,
                sec->comdat->symbol);

      comma = ", ";
    }

  fprintf (out_file, "\n");
#undef PF
}
#endif

static void
list_section_information (void)
{
#if defined(TC_PIC30)
#ifdef tc_list_section_information
  tc_list_section_information (list_file);
#else
  fprintf (out_file, _("Sections:\n"));

#ifndef BFD64
  fprintf (out_file, _("Idx Name          Size      VMA       LMA       File off  Algn"));
#else
  fprintf (out_file, _("Idx Name          Size      VMA               LMA               File off  Algn"));
#endif

  fprintf (out_file, "\n");

  bfd_map_over_sections (stdoutput, dump_section, (PTR) out_file);
#endif
#endif
}
#endif

static void
print_source (current_file, list, buffer, width)
     file_info_type *current_file;
     list_info_type *list;
     char *buffer;
     unsigned int width;
{
  if (!current_file->at_end)
    {
      while (current_file->linenum < list->hll_line
	     && !current_file->at_end)
	{
	  char *p = buffer_line (current_file, buffer, width);
	  fprintf (list_file, "%4u:%-13s **** %s\n", current_file->linenum,
		   current_file->filename, p);
	  on_page++;
	  listing_page (list);
	}
    }
}

/* Sometimes the user doesn't want to be bothered by the debugging
   records inserted by the compiler, see if the line is suspicious.  */

static int
debugging_pseudo (list, line)
     list_info_type *list;
     const char *line;
{
  static int in_debug;
  int was_debug;

  if (list->debugging)
    {
      in_debug = 1;
      return 1;
    }

  was_debug = in_debug;
  in_debug = 0;

  while (ISSPACE (*line))
    line++;

  if (*line != '.')
    {
#ifdef OBJ_ELF
      /* The ELF compiler sometimes emits blank lines after switching
         out of a debugging section.  If the next line drops us back
         into debugging information, then don't print the blank line.
         This is a hack for a particular compiler behaviour, not a
         general case.  */
      if (was_debug
	  && *line == '\0'
	  && list->next != NULL
	  && list->next->debugging)
	{
	  in_debug = 1;
	  return 1;
	}
#endif

      return 0;
    }

  line++;

  if (strncmp (line, "def", 3) == 0)
    return 1;
  if (strncmp (line, "val", 3) == 0)
    return 1;
  if (strncmp (line, "scl", 3) == 0)
    return 1;
  if (strncmp (line, "line", 4) == 0)
    return 1;
  if (strncmp (line, "endef", 5) == 0)
    return 1;
  if (strncmp (line, "ln", 2) == 0)
    return 1;
  if (strncmp (line, "type", 4) == 0)
    return 1;
  if (strncmp (line, "size", 4) == 0)
    return 1;
  if (strncmp (line, "dim", 3) == 0)
    return 1;
  if (strncmp (line, "tag", 3) == 0)
    return 1;

  if (strncmp (line, "stabs", 5) == 0)
    return 1;
  if (strncmp (line, "stabn", 5) == 0)
    return 1;

  return 0;
}

static void
listing_listing (name)
     char *name ATTRIBUTE_UNUSED;
{
  list_info_type *list = head;
  file_info_type *current_hll_file = (file_info_type *) NULL;
  char *message;
  char *buffer;
  char *p;
  int show_listing = 1;
  unsigned int width;

  buffer = xmalloc (listing_rhs_width);
  data_buffer = xmalloc (MAX_BYTES);
  eject = 1;
  list = head;

  while (list != (list_info_type *) NULL && 0)
    {
      if (list->next)
	list->frag = list->next->frag;
      list = list->next;

    }

  list = head->next;

  while (list)
    {
      unsigned int list_line;

      width = listing_rhs_width > paper_width ? paper_width :
	listing_rhs_width;

      list_line = list->line;
      switch (list->edict)
	{
	case EDICT_LIST:
	  /* Skip all lines up to the current.  */
	  list_line--;
	  break;
	case EDICT_NOLIST:
	  show_listing--;
	  break;
	case EDICT_NOLIST_NEXT:
	  if (show_listing == 0)
	    list_line--;
	  break;
	case EDICT_EJECT:
	  break;
	case EDICT_NONE:
	  break;
	case EDICT_TITLE:
	  title = list->edict_arg;
	  break;
	case EDICT_SBTTL:
	  subtitle = list->edict_arg;
	  break;
	default:
	  abort ();
	}

      if (show_listing <= 0)
	{
	  while (list->file->linenum < list_line
		 && !list->file->at_end)
	    p = buffer_line (list->file, buffer, width);
	}

      if (list->edict == EDICT_LIST
	  || (list->edict == EDICT_NOLIST_NEXT && show_listing == 0))
	{
	  /* Enable listing for the single line that caused the enable.  */
	  list_line++;
	  show_listing++;
	}

      if (show_listing > 0)
	{
	  /* Scan down the list and print all the stuff which can be done
	     with this line (or lines).  */
	  message = 0;

	  if (list->hll_file)
	    {
	      current_hll_file = list->hll_file;
	    }

	  if (current_hll_file && list->hll_line && (listing & LISTING_HLL))
	    {
	      print_source (current_hll_file, list, buffer, width);
	    }

	  if (list->line_contents)
	    {
	      if (!((listing & LISTING_NODEBUG)
		    && debugging_pseudo (list, list->line_contents)))
		{
		  print_lines (list,
			       list->file->linenum == 0 ? list->line : list->file->linenum,
			       list->line_contents, calc_hex (list));
		}
	      free (list->line_contents);
	      list->line_contents = NULL;
	    }
	  else
	    {
	      while (list->file->linenum < list_line
		     && !list->file->at_end)
		{
		  unsigned int address;

		  p = buffer_line (list->file, buffer, width);

		  if (list->file->linenum < list_line)
		    address = ~(unsigned int) 0;
		  else
		    address = calc_hex (list);

		  if (!((listing & LISTING_NODEBUG)
			&& debugging_pseudo (list, p)))
		    print_lines (list, list->file->linenum, p, address);
		}
	    }

	  if (list->edict == EDICT_EJECT)
	    {
	      eject = 1;
	    }
	}

      if (list->edict == EDICT_NOLIST_NEXT && show_listing == 1)
	--show_listing;

      list = list->next;
    }

  free (buffer);
  free (data_buffer);
  data_buffer = NULL;
}

void
listing_print (name)
     char *name;
{
  int using_stdout;

  title = "";
  subtitle = "";

  if (name == NULL)
    {
      list_file = stdout;
      using_stdout = 1;
    }
  else
    {
      list_file = fopen (name, FOPEN_WT);
      if (list_file != NULL)
	using_stdout = 0;
      else
	{
	  as_perror (_("can't open list file: %s"), name);
	  list_file = stdout;
	  using_stdout = 1;
	}
    }

  if (listing & LISTING_NOFORM)
    {
      paper_height = 0;
    }

  if (listing & LISTING_LISTING)
    {
      listing_listing (name);
    }

  if (listing & LISTING_SYMBOLS)
    {
      list_symbol_table ();
    }

#ifdef BFD_ASSEMBLER
  if (listing & LISTING_SECINFO)
    {
      list_section_information ();
    }
#endif

  if (! using_stdout)
    {
      if (fclose (list_file) == EOF)
	as_perror (_("error closing list file: %s"), name);
    }

  if (last_open_file)
    {
      fclose (last_open_file);
    }
}

void
listing_file (name)
     const char *name;
{
  fn = name;
}

void
listing_eject (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  if (listing)
    listing_tail->edict = EDICT_EJECT;
}

void
listing_flags (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  while ((*input_line_pointer++) && (*input_line_pointer != '\n'))
    input_line_pointer++;

}

/* Turn listing on or off.  An argument of 0 means to turn off
   listing.  An argument of 1 means to turn on listing.  An argument
   of 2 means to turn off listing, but as of the next line; that is,
   the current line should be listed, but the next line should not.  */

void
listing_list (on)
     int on;
{
  if (listing)
    {
      switch (on)
	{
	case 0:
	  if (listing_tail->edict == EDICT_LIST)
	    listing_tail->edict = EDICT_NONE;
	  else
	    listing_tail->edict = EDICT_NOLIST;
	  break;
	case 1:
	  if (listing_tail->edict == EDICT_NOLIST
	      || listing_tail->edict == EDICT_NOLIST_NEXT)
	    listing_tail->edict = EDICT_NONE;
	  else
	    listing_tail->edict = EDICT_LIST;
	  break;
	case 2:
	  listing_tail->edict = EDICT_NOLIST_NEXT;
	  break;
	default:
	  abort ();
	}
    }
}

void
listing_psize (width_only)
     int width_only;
{
  static unsigned char paper_height_set = FALSE;
  static unsigned char paper_width_set = FALSE;

  if (! width_only)
    {
      if (paper_height_set)
        as_warn (_("Overriding previously set paper height."));

      paper_height = get_absolute_expression ();
      paper_height_set = TRUE;

      if (paper_height < 0 || paper_height > 1000)
	{
	  paper_height = 0;
	  as_warn (_("strange paper height, set to no form"));
	}

      if (*input_line_pointer != ',')
	{
	  demand_empty_rest_of_line ();
	  return;
	}

      ++input_line_pointer;
    }

  if (paper_width_set)
     as_warn (_("Overriding previously set paper width."));

  paper_width = get_absolute_expression ();
  paper_width_set = TRUE;

  demand_empty_rest_of_line ();
}

void
listing_nopage (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  paper_height = 0;
}

void
listing_title (depth)
     int depth;
{
  int quoted;
  char *start;
  char *ttl;
  unsigned int length;

  SKIP_WHITESPACE ();
  if (*input_line_pointer != '\"')
    quoted = 0;
  else
    {
      quoted = 1;
      ++input_line_pointer;
    }

  start = input_line_pointer;

  while (*input_line_pointer)
    {
      if (quoted
	  ? *input_line_pointer == '\"'
	  : is_end_of_line[(unsigned char) *input_line_pointer])
	{
	  if (listing)
	    {
	      length = input_line_pointer - start;
	      ttl = xmalloc (length + 1);
	      memcpy (ttl, start, length);
	      ttl[length] = 0;
	      listing_tail->edict = depth ? EDICT_SBTTL : EDICT_TITLE;
	      listing_tail->edict_arg = ttl;
	    }
	  if (quoted)
	    input_line_pointer++;
	  demand_empty_rest_of_line ();
	  return;
	}
      else if (*input_line_pointer == '\n')
	{
	  as_bad (_("new line in title"));
	  demand_empty_rest_of_line ();
	  return;
	}
      else
	{
	  input_line_pointer++;
	}
    }
}

void
listing_source_line (line)
     unsigned int line;
{
  if (listing)
    {
      new_frag ();
      listing_tail->hll_line = line;
      new_frag ();
    }
}

void
listing_source_file (file)
     const char *file;
{
  if (listing)
    listing_tail->hll_file = file_info (file);
}

#else

/* Dummy functions for when compiled without listing enabled.  */

void
listing_flags (ignore)
     int ignore;
{
  s_ignore (0);
}

void
listing_list (on)
     int on;
{
  s_ignore (0);
}

void
listing_eject (ignore)
     int ignore;
{
  s_ignore (0);
}

void
listing_psize (ignore)
     int ignore;
{
  s_ignore (0);
}

void
listing_nopage (ignore)
     int ignore;
{
  s_ignore (0);
}

void
listing_title (depth)
     int depth;
{
  s_ignore (0);
}

void
listing_file (name)
     const char *name;
{

}

void
listing_newline (name)
     char *name;
{

}

void
listing_source_line (n)
     unsigned int n;
{

}

void
listing_source_file (n)
     const char *n;
{

}

#endif
