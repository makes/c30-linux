/*
** pic30-reports.c
**
** Copyright (c) 2008 Microchip Technology, Inc.
**
** This file contains pic30-specifc reports
** processing for the linker.
**
** It is included by the following target-specific
** emulation files:
**
**   ld/emultmpl/pic30_coff.em
**   ld/emultmpl/pic30_elf32.em
**
*/

/*****************************************************************************/

#define REPORT_AS_PROGRAM(s) \
  (PIC30_IS_CODE_ATTR(s) || PIC30_IS_PSV_ATTR(s) || PIC30_IS_AUXFLASH_ATTR(s))

#define REPORT_AS_DATA(s) \
  (PIC30_SECTION_IN_DATA_MEMORY(s) || PIC30_IS_MEMORY_ATTR(s))

#define REPORT_AS_EEDATA(s) \
  (PIC30_IS_EEDATA_ATTR(sect))

/*****************************************************************************/

/*
** Utility routine: bfd_pic30_report_program_sections()
*/ 
static void
bfd_pic30_report_program_sections (asection *sect, FILE *fp) {
  file_ptr start = sect->lma;
  bfd_size_type total = sect->_raw_size;
  bfd_size_type PCunits = total / 2;
  bfd_size_type actual = total * .75;


  if (REPORT_AS_PROGRAM(sect)) {
      char *name, *c;

      /* make a local copy of the name */
      name = xmalloc(strlen(sect->name) + 1);
      strcpy(name, sect->name);

      /* clean the name of %n */
      c = strchr(name, '%');
      if (c) *c = '\x0';

      fprintf( fp, "%-24s%#10lx%#20lx%#16lx  (%ld)\n", name,
             start, PCunits, actual, actual);
      actual_prog_memory_used += (total * .75);

      free(name);
    }
} /* static void bfd_pic30_report_program_sections (...)*/


/*
** Utility routine: bfd_pic30_report_eedata_sections()
*/ 
static void
bfd_pic30_report_eedata_sections (asection *sect, FILE *fp) {
  file_ptr start = sect->lma;
  bfd_size_type total = sect->_raw_size;
  bfd_size_type PCunits = total / 2;
  bfd_size_type actual = PCunits;

  if (REPORT_AS_EEDATA(sect)) {
      char *name, *c;

      /* make a local copy of the name */
      name = xmalloc(strlen(sect->name) + 1);
      strcpy(name, sect->name);

      /* clean the name of %n */
      c = strchr(name, '%');
      if (c) *c = '\x0';

      fprintf( fp, "%-24s%#10lx%#20lx%#16lx  (%ld)\n", name,
             start, PCunits, actual, actual);
      actual_eedata_memory_used += actual;

      free(name);
    }
} /* static void bfd_pic30_report_eedata_sections (...)*/


/*
** Utility routine: bfd_pic30_report_data_sections()
*/ 
static void
bfd_pic30_report_data_sections (asection *sect, FILE *fp) {
  file_ptr start = sect->vma;
  unsigned int opb = bfd_octets_per_byte (output_bfd);
  bfd_size_type total = sect->_raw_size / opb; /* ignore phantom bytes */

  if (REPORT_AS_DATA(sect))
    {
      unsigned int gaps;
      char *name, *c;
      struct bfd_link_hash_entry *h = (struct bfd_link_hash_entry *) NULL;

      /* make a local copy of the name */
      name = xmalloc(strlen(sect->name) + strlen(GAP_ID) + 1);
      strcpy(name, sect->name);

      strcat(name, GAP_ID);
      h = bfd_link_hash_lookup (link_info.hash, name, FALSE, FALSE, TRUE);

      if (h == (struct bfd_link_hash_entry *) NULL)
        gaps = 0;
      else
        gaps = h->u.def.value / opb; /* ignore phantom bytes */

      /* clean the name of GAP_ID or %n.GAPID */
      c = strchr(name, '%');
      if (c) *c = '\x0';

      fprintf( fp, "%-24s%#10lx%#20x%#16lx  (%ld)\n", name,
             start, gaps, total, total);
      data_memory_used += total;

      free(name);
    }

  return;
} /* static void bfd_pic30_report_data_sections (...)*/


/*
** Utility routine: bfd_pic30_report_memory_sections()
**
** - show memory usage of sections in user-defined regions
*/ 
static void
bfd_pic30_report_memory_sections (const char *region, asection *sect,
                                  FILE *fp) {
  file_ptr start = sect->vma;
  unsigned int opb = bfd_octets_per_byte (output_bfd);
  bfd_size_type total = sect->_raw_size / opb; /* ignore phantom bytes */

  if (PIC30_IS_MEMORY_ATTR(sect)) {
    const char *c1, *c2;
    struct pic30_section *s;
    unsigned int n;

    /* loop through the list of memory-type sections */
    for (s = user_memory_sections; s != NULL; s = s->next) {
      if (!s->file || !s->sec) continue;
      if (strcmp((char *)s->file, region) != 0) continue;
      
      c1 = (const char *) s->sec;  /* name is stored here! */
      c2 = strchr(c1, '%');        /* avoid the %n terminator */
      n = c2 ? c2 - c1 : strlen(c1);
      if (strncmp(c1, sect->name, n) == 0) {
        char *c, *name = xmalloc(strlen(sect->name) + 1);
        strcpy(name, sect->name);  /* make a local copy of the name */
        c = strchr(name, '%');
        if (c) *c = '\x0';         /* remove the %n terminator */
        
        fprintf(fp, "%-24s%#10lx%#36lx  (%ld)\n", name, start, total, total);
        external_memory_used += total;
        free(name);
        break;
      }
    }
  }
  return;
} /* static void bfd_pic30_report_memory_sections (...)*/


static void
report_percent_used (bfd_vma used, bfd_vma max, FILE *fp) {
  int percent = 100 * ((float) used/max);

  if (percent)
    fprintf( fp, "%d%%", percent);
  else
    fprintf( fp, "<1%%");
}


static int
in_bounds (asection *sec, lang_memory_region_type *region) {
  bfd_vma start;
  bfd_size_type len;
  int result = 0;
  
  if (REPORT_AS_DATA(sec)) {
    start = sec->vma;
    len  = sec->_raw_size / 2;
  }
  else {
    start = sec->lma;
    len  = sec->_raw_size / 2;
  }

  if (start >= region->origin)
    result = 1;

  if ((start + len) > (region->origin + region->length))
    result = 0;

  return result;
}


/*
** Utility routine: bfd_pic30_report_memory_usage
**
** - print a section chart to file *fp
**/
static void
bfd_pic30_report_memory_usage (FILE *fp) {
  bfd_size_type max_heap, max_stack;
  lang_memory_region_type *region;
  struct pic30_section *s;
  int has_eedata = 0;

  /* clear the counters */
  actual_prog_memory_used = 0;
  actual_eedata_memory_used = 0;
  data_memory_used = 0;

  /* build an ordered list of output sections */
  pic30_init_section_list(&pic30_section_list);
  bfd_map_over_sections(output_bfd, &pic30_build_section_list, NULL);

  region = region_lookup("program");
  /* print code header */
  fprintf( fp, "\n\nProgram Memory  [Origin = 0x%lx, Length = 0x%lx]\n\n",
           region->origin, region->length);
  fprintf( fp, "section                    address   length (PC units)"
           "   length (bytes) (dec)\n");
  fprintf( fp, "-------                    -------   -----------------"
           "   --------------------\n");

  /* report code sections */
  for (s = pic30_section_list; s != NULL; s = s->next)
    if ((s->sec) && in_bounds(s->sec, region))
      bfd_pic30_report_program_sections (s->sec, fp);

  /* print code summary */
  fprintf( fp, "\n                     Total program memory used (bytes):"
           "     %#10lx  (%ld) ",
         actual_prog_memory_used, actual_prog_memory_used);
  report_percent_used((actual_prog_memory_used * 2)/3,
                      region->length, fp);
  fprintf( fp, "\n");
  
  /* the eedata report is optional */
  for (s = pic30_section_list; s != NULL; s = s->next)
    if ((s->sec) && PIC30_IS_EEDATA_ATTR(s->sec))
      has_eedata = 1;

  if (has_eedata) {
    region = region_lookup("eedata");
    /* print eedata header */
    fprintf( fp, "\n\nData EEPROM Memory  [Origin = 0x%lx, Length = 0x%lx]\n\n",
             region->origin, region->length);
    fprintf( fp, "section                    address   length (PC units)"
             "   length (bytes) (dec)\n");
    fprintf( fp, "-------                    -------   -----------------"
             "   --------------------\n");

    /* report eedata sections */
    for (s = pic30_section_list; s != NULL; s = s->next)
      if ((s->sec) && in_bounds(s->sec, region))
        bfd_pic30_report_eedata_sections (s->sec, fp);

    /* print eedata summary */
    fprintf( fp, "\n                        Total data EEPROM used (bytes):"
             "     %#10lx  (%ld) ",
             actual_eedata_memory_used, actual_eedata_memory_used);
  report_percent_used(actual_eedata_memory_used, region->length, fp);
  fprintf( fp, "\n");
  }

  /* print data header */
  region = region_lookup("data");
  fprintf( fp, "\n\nData Memory  [Origin = 0x%lx, Length = 0x%lx]\n\n",
           region->origin, region->length);
  fprintf( fp, "section                    address      alignment gaps"
           "    total length  (dec)\n");
  fprintf( fp, "-------                    -------      --------------"
           "    -------------------\n");

  /* report data sections */
  for (s = pic30_section_list; s != NULL; s = s->next)
    if ((s->sec) && in_bounds(s->sec, region))
      bfd_pic30_report_data_sections (s->sec, fp);

  /* print data summary */
  fprintf( fp, "\n                        Total data memory used (bytes):"
           "     %#10lx  (%ld) ",
         data_memory_used, data_memory_used);
  if (data_memory_used > 0)
    report_percent_used(data_memory_used, region->length, fp);
  fprintf( fp, "\n");

  /* print dynamic header */
  fprintf( fp, "\n\nDynamic Memory Usage\n\n");
  fprintf( fp, "region                     address                      "
           "maximum length  (dec)\n");
  fprintf( fp, "------                     -------                      "
           "---------------------\n");

  /* report dynamic regions */
  max_heap  = heap_limit - heap_base;
  fprintf( fp, "%-24s%#10x%#36lx  (%ld)\n", "heap",
           heap_base, max_heap, max_heap);

  if (user_defined_stack) {
    max_stack = 0;
    fprintf( fp, "(user-defined stack at 0x%x, length = 0x%x)\n",
             stack_base, (unsigned int) (stack_limit - stack_base));
  }
  else {
    max_stack = stack_limit - stack_base + pic30_stackguard_size;
    fprintf( fp, "%-24s%#10x%#36lx  (%ld)\n", "stack",
             stack_base, max_stack, max_stack);
  }

  /* print dynamic summary */
  fprintf( fp, "\n                        Maximum dynamic memory (bytes):"
           " %#14lx  (%ld)\n\n",
         (max_heap + max_stack), (max_heap + max_stack));

  /* report user-defined memory sections...
     they require some extra effort to organize by
     external memory region */
  if (has_user_defined_memory) {
    struct pic30_section *r, *rnext, *s;
    const char *region_name;
    /* loop through any user-defined regions */
    for (r = memory_region_list; r != NULL; r = rnext) {
  
      rnext = r->next;
      if (r->sec == 0) continue;
    
      region_name = r->sec->name + strlen(memory_region_prefix);
      fprintf( fp, "\nExternal Memory %s"
               "  [Origin = 0x%lx, Length = 0x%lx]\n\n",
                region_name, r->sec->vma, r->sec->lma);

      fprintf( fp, "section                    address                       "
               " total length  (dec)\n");
      fprintf( fp, "-------                    -------                       "
               " -------------------\n");

      external_memory_used = 0;
      for (s = pic30_section_list; s != NULL; s = s->next)
        if (s->sec)
          bfd_pic30_report_memory_sections (region_name, s->sec, fp);

      /* print summary for this region */
      fprintf( fp, "\n                    Total external memory used (bytes):"
               "     %#10lx  (%ld) ",
              external_memory_used, external_memory_used);
      if (external_memory_used > 0)
        report_percent_used(external_memory_used, r->sec->lma, fp);
      fprintf( fp, "\n\n");
    }
  }
  
  /* free the output section list */
  pic30_free_section_list(&pic30_section_list);

} /* static void bfd_pic30_report_memory_usage (...)*/


/*
** build_data_symbol_list
**
** - build a list of external data symbols
** - called via bfd_link_hash_traverse()
*/
#define LINK_HASH_VALUE(H)  ((H)->u.def.value \
                    + (H)->u.def.section->output_offset \
                    + (H)->u.def.section->output_section->lma)

static bfd_boolean
pic30_build_data_symbol_list(struct bfd_link_hash_entry *h, PTR p) {
  struct pic30_symbol **list = ( struct pic30_symbol **) p;

  if (( h->type == bfd_link_hash_defined) &&
      (!(h->u.def.section->flags & SEC_EXCLUDE) &&                                            PIC30_SECTION_IN_DATA_MEMORY(h->u.def.section))) {
    bfd_vma value = LINK_HASH_VALUE(h);
    pic30_add_to_symbol_list( list, h->root.string, value);
  }
  return TRUE;  /* continue */
}


/*
** build_eedata_symbol_list
**
** - build a list of external eedata symbols
** - called via bfd_link_hash_traverse()
*/
static bfd_boolean
pic30_build_eedata_symbol_list(struct bfd_link_hash_entry *h, PTR p) {
  struct pic30_symbol **list = ( struct pic30_symbol **) p;

  if (( h->type == bfd_link_hash_defined) &&
       PIC30_IS_EEDATA_ATTR(h->u.def.section)) {

    bfd_vma value = LINK_HASH_VALUE(h);
    pic30_add_to_symbol_list( list, h->root.string, value);
  }
  return TRUE;  /* continue */
}


/*
** build_program_symbol_list
**
** - build a list of external program symbols
** - called via bfd_link_hash_traverse()
*/
static bfd_boolean
pic30_build_program_symbol_list(struct bfd_link_hash_entry *h, PTR p) {
  struct pic30_symbol **list = ( struct pic30_symbol **) p;

  if (( h->type == bfd_link_hash_defined) &&
      (!(h->u.def.section->flags & SEC_EXCLUDE) &&
      (PIC30_IS_CODE_ATTR(h->u.def.section) ||
       PIC30_IS_PSV_ATTR(h->u.def.section) ||
       PIC30_IS_AUXFLASH_ATTR(h->u.def.section)))) {

    bfd_vma value = LINK_HASH_VALUE(h);
    pic30_add_to_symbol_list( list, h->root.string, value);
  }
  return TRUE;  /* continue */
}


static int
compare_symbol_value (const void *p1, const void *p2) {
  struct pic30_symbol *sym1 = (struct pic30_symbol *) p1;
  struct pic30_symbol *sym2 = (struct pic30_symbol *) p2;

  if (sym1->value < sym2->value)
    return -1;
  else if (sym1->value == sym2->value)
    return 0;
  else
    return 1;
}


static int
compare_symbol_name (const void *p1, const void *p2) {
  struct pic30_symbol *sym1 = (struct pic30_symbol *) p1;
  struct pic30_symbol *sym2 = (struct pic30_symbol *) p2;

  return strcmp(sym1->name, sym2->name);
}



static void
pic30_report_symbol_table (FILE *fp,
                           struct pic30_symbol *list,
                           int count, char * name, int format) {
  char *s1 = "\nExternal Symbols in ";
  char *s2 = " (by address):\n\n";
  char *s3 = " (by name):\n\n";

  fprintf(fp, "%s%s%s", s1, name, s2);
  qsort(list, count, sizeof(struct pic30_symbol), compare_symbol_value);
  pic30_print_symbol_list(list, fp, format);

  fprintf(fp, "%s%s%s", s1, name, s3);
  qsort(list, count, sizeof(struct pic30_symbol), compare_symbol_name);
  pic30_print_symbol_list(list, fp, format);
}


/*
** Utility routine: bfd_pic30_report_external_symbols
**
** - print sorted symbol tables to file *fp
**/
static void
pic30_report_external_symbols (FILE *fp) {

#define DATA 1
#define CODE 2

  pic30_init_symbol_list (&pic30_symbol_list);

  /* data memory */
  bfd_link_hash_traverse (link_info.hash,
                          pic30_build_data_symbol_list, &pic30_symbol_list);
  if (pic30_symbol_count)
    pic30_report_symbol_table (fp, pic30_symbol_list, pic30_symbol_count,
                               "Data Memory", DATA);
  /* data EEPROM */
  pic30_symbol_count = 0;
  bfd_link_hash_traverse (link_info.hash,
                          pic30_build_eedata_symbol_list, &pic30_symbol_list);
    if (pic30_symbol_count)
      pic30_report_symbol_table (fp, pic30_symbol_list, pic30_symbol_count,
                                 "Data EEPROM", CODE);
  /* program memory */
  pic30_symbol_count = 0;
  bfd_link_hash_traverse (link_info.hash,
                          pic30_build_program_symbol_list, &pic30_symbol_list);
    if (pic30_symbol_count)
      pic30_report_symbol_table (fp, pic30_symbol_list, pic30_symbol_count,
                                 "Program Memory", CODE);

  free(pic30_symbol_list);

} /* static void bfd_pic30_report_external_symbols */

