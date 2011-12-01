/*
 * cpu-pic30.c -- BFD library support routines for the PIC30 architecture.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "coff-pic30.h"
#include "pic30-utils.h"
#include "libiberty.h"
#include "../../c30_resource/src/c30/resource_info.h"

#ifndef DIR_SEPARATOR
#define DIR_SEPARATOR '/'
#endif

#if defined (_WIN32) || defined (__MSDOS__) || defined (__DJGPP__) || \
  defined (__OS2__)
#ifndef DIR_SEPARATOR_2
#define HAVE_DOS_BASED_FILE_SYSTEM
#define DIR_SEPARATOR_2 '\\\\'
#define EXE_SUFFIX ".exe"
#define PATH_SEPARATOR ';'
#define DIR_SEPARATOR_STR "\\"
#endif
#endif

/* Define IS_DIR_SEPARATOR.  */
#ifndef IS_DIR_SEPARATOR
#ifndef DIR_SEPARATOR_2
# define IS_DIR_SEPARATOR(ch) ((ch) == DIR_SEPARATOR)
#else /* DIR_SEPARATOR_2 */
# define IS_DIR_SEPARATOR(ch) \
        (((ch) == DIR_SEPARATOR) || ((ch) == DIR_SEPARATOR_2))
#endif /* DIR_SEPARATOR_2 */
#endif

#ifndef DIR_SEPARATOR_2 
#define PATH_SEPARATOR ':'
#define DIR_SEPARATOR_2 DIR_SEPARATOR
#define EXE_SUFFIX ""
#define DIR_SEPARATOR_STR "/"
#endif

/******************************************************************************/

/* prototypes */
const bfd_arch_info_type * pic30_get_machine (const char * const name);
int pic30_proc_family(const bfd_arch_info_type *);
int pic30_is_dsp_machine(const bfd_arch_info_type *);
int pic30_is_eedata_machine(const bfd_arch_info_type *);
int pic30_is_dma_machine(const bfd_arch_info_type *);
int pic30_is_codeguard_machine(const bfd_arch_info_type *);
int pic30_is_eds_machine(const bfd_arch_info_type *);
int pic30_is_pmp_machine(const bfd_arch_info_type *);
int pic30_is_epmp_machine(const bfd_arch_info_type *);
int pic30_is_ecore_machine(const bfd_arch_info_type *);
int pic30_is_5V_machine(const bfd_arch_info_type *);
void pic30_update_resource(const char *resource);
void pic30_load_codeguard_settings(const bfd_arch_info_type *, int);
void pic30_clear_codeguard_settings(void);
void * pic30_lookup_codeguard_setting(unsigned int, unsigned int);
void * pic30_lookup_valid_codeguard_setting(unsigned int);
char * pic30_codeguard_setting_name(void *);
unsigned int pic30_codeguard_setting_flags(void *s);
bfd_vma pic30_codeguard_setting_address(void *s);
int pic30_add_selected_codeguard_option(void *);
void pic30_dump_selected_codeguard_options(FILE *);
char * pic30_unique_selected_configword_names(void);
int pic30_decode_CG_settings(char *, unsigned short, int);
unsigned short pic30_encode_CG_settings(char *);


#define ARCH_TABLE 1
#define CODEGUARD_SETTINGS 2
static void process_resource_file(unsigned int, unsigned int, int);


const bfd_arch_info_type * global_PROCESSOR = NULL;

static char *pic30_resource_file = 0;

#define EOL ,
#define ARCH(machine, printable_name, default_machine, next, flags) \
{ 16,                     \
  24,                     \
  16,                      \
  bfd_arch_pic30,         \
  machine,                \
  "pic30",                \
  printable_name,         \
  1,                      \
  default_machine,        \
  bfd_default_compatible, \
  bfd_default_scan,       \
  next }

#if 0
static const bfd_arch_info_type arch_info_struct[] =
{
#include "pic30-procs.h"
};
#endif

struct pic30_resource_info {
  unsigned int flags;
  bfd_arch_info_type *arch_info;
  struct pic30_resource_info *next;
};

typedef struct pic30_codeguard_setting {
  char *name;
  unsigned int flags;
  unsigned int mask;
  unsigned int value;
  bfd_vma address;
  struct pic30_codeguard_setting *next;
} codeguard_setting_type;

typedef struct codeguard_select_list {
  codeguard_setting_type *sel;
  struct codeguard_select_list *next;
} codeguard_select_list_type;

static codeguard_setting_type *CG_settings, *last_CG_setting;
static codeguard_select_list_type *CG_select, *last_CG_select;

/* This is the "last" of the generic devices, and must not
 * be declared const, because the table of devices read
 * from the resource file will be appended to it. */
static bfd_arch_info_type generic_5 = 
   ARCH ( 94,                "GENERIC-16DSP-EP", TRUE, 0, HAS_DSP | HAS_EDS | HAS_ECORE);

static bfd_arch_info_type generic_4 = 
   ARCH ( 93,                "GENERIC-16BIT-EP", TRUE, &generic_5, HAS_EDS | HAS_ECORE);

static bfd_arch_info_type generic_3 = 
   ARCH ( 92,                "GENERIC-16BIT-DA", TRUE, &generic_4, HAS_EDS);

static const bfd_arch_info_type generic_2 = 
   ARCH ( 91,                "GENERIC-16DSP", TRUE, &generic_3, HAS_DSP);

static const bfd_arch_info_type generic_1 = 
   ARCH ( 90,                "GENERIC-16BIT", TRUE, &generic_2, 0);

const bfd_arch_info_type bfd_pic30_arch = 
   ARCH ( 0,                 "30Fxxxx", TRUE,  &generic_1, 0 ) ;

static struct pic30_resource_info arch_flags_head[] = {
   { 0, (bfd_arch_info_type *) &bfd_pic30_arch, &arch_flags_head[1] },
   { 0, (bfd_arch_info_type *) &generic_1, &arch_flags_head[2] },
   { HAS_DSP, &generic_2, &arch_flags_head[3] },
   { HAS_EDS, &generic_3, &arch_flags_head[4] },
   { HAS_EDS | HAS_ECORE, &generic_4, &arch_flags_head[5] },
   { HAS_DSP | HAS_EDS | HAS_ECORE, &generic_5, 0 }
};

static int pic30_tool_version;
char *pic30_resource_version;

static char *version_part1;
static char *version_part2;

#define QUOTE2(X) #X
#define QUOTE(X) QUOTE2(X)

int
pic30_is_generic_machine(unsigned int machine) {
  if ((machine == 0) || ((machine >= 90) && (machine <= 94))) return 1;
  return 0;
}


/* stupid prototype */ static void get_resource_path(const char *resource);
static void get_resource_path(const char *resource) {

  char *tool_name = (char *) resource;
  char *c;

  /*
  ** In some command shells, no path information
  ** is provided as part of argv[0].
  */
  if (!IS_DIR_SEPARATOR(resource[0]))
  {
    /*
    ** No DOS path information for the program:
    ** search for the executable using the
    ** PATH environment variable.
    */
    struct stat fs;
    char *separator;
    const char *PATH;
    char *dosPATH = 0;
    int tool_len;
    int len;
    char *command;

    tool_name = xmalloc(strlen(resource) + 4 + 40);
    sprintf(tool_name, "%s", resource);
    tool_len = strlen(tool_name);
#if 0
    fprintf(stderr,"tool_name: %s\n", tool_name);
#endif
#if defined(HAVE_DOS_BASED_FILE_SYSTEM)
    /*
    ** Append ".exe" to the executable name, if necessary
    */
    if ((tolower(tool_name[tool_len-1]) != 'e') ||
        (tolower(tool_name[tool_len-2]) != 'x') ||
        (tolower(tool_name[tool_len-3]) != 'e') ||
        (       (tool_name[tool_len-4]) != '.'))
    {
      strcat(tool_name, ".exe");
      tool_len += 4;
    }
#endif
    /*
    ** Get the current PATH
    */
    PATH = getenv("PATH");
    if (PATH) {
      /*
       *  stupid gnu will print (null) to the string if PATH==0
       */
      dosPATH = xcalloc(1, strlen(PATH) + 3);
      sprintf(dosPATH,"%s%c.",PATH, PATH_SEPARATOR);
      PATH = dosPATH;
    } else {
      PATH=".";
    }
#if 0
    fprintf(stderr,"PATH=%s\n", PATH);
#endif
    len = 256;
    command = xmalloc(len);
    do
    { int path_len;

      separator = strchr(PATH, PATH_SEPARATOR);
      path_len = separator ? (unsigned)(separator-PATH) : strlen(PATH);
      if ((path_len + tool_len + 2)  > len) {
        len = path_len + tool_len + 80;
        free(command);
        command = xmalloc(len);
      }
      command[0] = 0;
      strncpy(command, PATH, path_len);
      command[path_len] = 0;
      if (!IS_DIR_SEPARATOR(command[path_len-1])) {
        strcat(command, DIR_SEPARATOR_STR);
      }
      strcat(command, tool_name);
#if 0
      fprintf(stderr,"dir=%s\n",command);
#endif
      if (stat(command, &fs) == 0) break;
      PATH = 0;
    } while (separator && (PATH = separator+1));
    if (dosPATH) free(dosPATH);
    if (PATH) tool_name = command;
  }
  pic30_resource_file = xmalloc(strlen(tool_name) +
                                sizeof("/c30_device.info") + 1);
  sprintf(pic30_resource_file,"%s", tool_name);
  for (c = pic30_resource_file + strlen(pic30_resource_file);
       !IS_DIR_SEPARATOR(*c); c--);
  *c = 0;
  strcat(pic30_resource_file,"/c30_device.info");
}

void pic30_update_resource(const char *resource) {
  /* The tool_version is defined in the string MCHP_VERSION */
  char *Microchip;
#ifndef MCHP_VERSION
  char *new_version = xstrdup("undefined0_00");
#else
  char *new_version = xstrdup(QUOTE(MCHP_VERSION));
#endif
  int major = 0, minor = 0;

  Microchip = new_version;
  if (Microchip) {
    for (; (*Microchip) && ((*Microchip < '0') || (*Microchip > '9')); 
         Microchip++);
    if (*Microchip) {
      major = strtol(Microchip, &Microchip, 0);
      if ((*Microchip) && ((*Microchip == '_') || (*Microchip == '.'))) {
         Microchip++;
         minor = strtol(Microchip, &Microchip, 0);
         for (; *Microchip && *Microchip != ' '; Microchip++);
      }
      pic30_tool_version = major *100 + minor;
      version_part1 = new_version;
      *Microchip = 0;
      version_part2 = Microchip+1;
    }
  }
  get_resource_path(resource);
  process_resource_file(ARCH_TABLE, 0, 0);
}

static struct pic30_resource_info *selected_processor;

static void process_resource_file(unsigned int mode, unsigned int procID, int debug) {
  struct resource_introduction_block *rib;
  struct resource_data d;
  static int err_return = 0;
  int version;

  if (err_return) return;

  if (pic30_resource_file == 0) {
    fprintf(stderr,"Provide a resource file");
    err_return = 1;
    return;
  }
  rib = read_device_rib(pic30_resource_file,
                        (global_PROCESSOR) ? global_PROCESSOR->printable_name :
                                             0);
  if (rib == 0) {
    fprintf(stderr,"Could not open resource file: %s\n", pic30_resource_file);
    err_return = 1;
    return;
  }
  if (strcmp(rib->tool_chain,"C30")) {
    fprintf(stderr,"Invalid resource resource file\n");
    close_rib();
    err_return = 1;
    return;
  }

  if (!pic30_resource_version) {
    pic30_resource_version = xmalloc(strlen(version_part1) + 
                                     strlen(version_part2) + 40);
    version = rib->version.major * 100 + rib->version.minor;
    if (version != pic30_tool_version) {
      fprintf(stderr,"Warning: resource version (%d.%.2d) does not match!\n",
              rib->version.major, rib->version.minor);
      sprintf(pic30_resource_version,"%s, resource version %d.%02d (%c), %s",
              version_part1, rib->version.major, rib->version.minor, 
              rib->resource_version_increment,version_part2);
    } else {
      sprintf(pic30_resource_version,"%s (%c) %s",
              version_part1, rib->resource_version_increment, version_part2);
    }
  }

  if ((mode == ARCH_TABLE) && rib->field_count >= 3) {
    bfd_arch_info_type *last_generic = arch_flags_head[sizeof(arch_flags_head)/sizeof(struct pic30_resource_info)-1].arch_info;
    struct pic30_resource_info *last_arch_flags = &arch_flags_head[sizeof(arch_flags_head)/sizeof(struct pic30_resource_info)-1];
    int record;

    for (record = 0; move_to_record(record); record++) {
      bfd_arch_info_type *next;
      struct pic30_resource_info *next_flags; 
      struct resource_data d2;
      struct resource_data d3;

      read_value(rik_string, &d);
      read_value(rik_int, &d2);
      if (d2.v.i & IS_DEVICE_ID) {
        next = xmalloc(sizeof(bfd_arch_info_type));
        memcpy(next, last_generic, sizeof(bfd_arch_info_type));
        last_generic->next = next;
        last_generic = next;
        next_flags = xmalloc(sizeof(struct pic30_resource_info));
        next->printable_name = d.v.s;
        next_flags->flags = d2.v.i;
        read_value(rik_int, &d3);
        next->mach = d3.v.i;
        next_flags->arch_info = next;
        last_arch_flags->next = next_flags;
        last_arch_flags = next_flags;
      } else free(d.v.s);
    }
    last_arch_flags->next = 0;
  }

  if ((mode == CODEGUARD_SETTINGS) && rib->field_count >= 6) {
    int record;

    if (CG_settings) free(CG_settings);
    CG_settings = xmalloc(sizeof(codeguard_setting_type));
    memset(CG_settings, 0, sizeof(codeguard_setting_type));
    last_CG_setting = CG_settings;

    if (debug)
      printf("  matching records (procID = %d) ", procID);
    for (record = 0; move_to_record(record); record++) {
      codeguard_setting_type *next;
      struct resource_data d2,d3,d4,d5,d6;

      read_value(rik_string, &d);
      read_value(rik_int, &d2);
      read_value(rik_int, &d3);

      /* we don't support generic codeguard settings */
      if ((d2.v.i & IS_CODEGUARD_ID) && (d2.v.i & FAMILY_MASK) &&
          (d3.v.i == 0))
        printf("\nGeneric CodeGuard settings are not supported\n");

      if ((d2.v.i & IS_CODEGUARD_ID) && (d3.v.i == procID)) {
        if (debug)
          printf(".");
        read_value(rik_int, &d4);
        read_value(rik_int, &d5);
        read_value(rik_int, &d6);

        next = xmalloc(sizeof(codeguard_setting_type));
        next->name    = d.v.s;
        next->flags   = d2.v.i & CODEGUARD_MASK;
        next->mask    = d4.v.i;
        next->value   = d5.v.i;
        next->address = d6.v.i;
        next->next    = 0;

        last_CG_setting->next = next;
        last_CG_setting = next;
      } else free(d.v.s);
    }
    close_rib();
    if (debug)
      printf("\n");
  }
}

/*
** Load CodeGuard settings from a resource file
** for a particular processor.
*/
void pic30_load_codeguard_settings(const bfd_arch_info_type *proc, int debug)
{
  /* do nothing if already loaded */
  if (CG_settings) {
    if (debug)
      printf("\nCodeGuard settings already loaded.\n");
    return;
  }

  /* do nothing if no processor specified */
  if (!proc) {
    if (debug)
      printf("\nCan't load CodeGuard settings; no target device specified.\n");
    return;
  }

  /* else read the resource file */
  if (debug)
    printf("\nLoading CodeGuard settings for %s:\n", proc->printable_name);
  process_resource_file(CODEGUARD_SETTINGS, proc->mach, debug);
}

/*
** Clear the CodeGuard settings
*/
void pic30_clear_codeguard_settings()
{
  if (CG_settings) free(CG_settings);
}


/*
** Lookup a CodeGuard setting
** from the list of currently selected options.
**
** Takes two arguments:
**
**  base_flags = flags that must be set
**  seek_flags = flags we are looking for
**
** Sample usage:
**   Which of flags a|b|c is set for xx:yy?
**
** Returns:
**   A setting for xx:yy that includes at least one of a|b|c
**   or NULL
*/
void * pic30_lookup_codeguard_setting(unsigned int base_flags,
                                      unsigned int seek_flags)
{
  codeguard_select_list_type *s;

  if (!CG_select) return NULL;

#if 0
  printf("\nLooking up CodeGuard setting, base_flags = 0x%x,"
         " seek_flags = 0x%x\n", base_flags, seek_flags);
#endif
  for (s = CG_select; s; s = s->next ) {
#if 0
    printf("  %s  0x%x\n", s->sel->name, s->sel->flags);
#endif
    if (((s->sel->flags & base_flags) == base_flags) &&
        ((s->sel->flags & seek_flags) != 0))
      return s->sel;
  }
#if 0
  printf("  (not found)\n");
#endif
  return NULL;
}


/*
** Lookup an exact CodeGuard setting
** from the list of all valid options.
**
** Note that the flags argument is complete.
**
** Sample usage:
**   Is setting xx:yy:zz valid for this device?
*/
void * pic30_lookup_valid_codeguard_setting(unsigned int flags)
{
  codeguard_setting_type *s;

  if (!CG_settings) return NULL;

  for (s = CG_settings; s; s = s->next ) {
#if 0
    printf("  %s  0x%x\n", s->name, s->flags);
#endif
    if (s->flags == flags)
      return s;
  }
  return NULL;
}

/*
** Accessor functions for CodeGuard settings
*/
char * pic30_codeguard_setting_name(void *s)
{
  return ((codeguard_setting_type *)s)->name;
}


unsigned int pic30_codeguard_setting_flags(void *s)
{
  return ((codeguard_setting_type *)s)->flags;
}

bfd_vma pic30_codeguard_setting_address(void *s)
{
  return ((codeguard_setting_type *)s)->address;
}


/*
** Collect CodeGuard options and look for conflicts
**
** Note that option names have format "name:setting:value"
*/
int pic30_add_selected_codeguard_option(void *op)
{
  codeguard_setting_type *opt = (codeguard_setting_type *) op;
  codeguard_select_list_type *s,*next,*last;

  if (!CG_select) {
    /* first item in the list, add it */
    next = xmalloc(sizeof(codeguard_select_list_type));
    next->sel = opt;
    next->next = 0;
    CG_select = next;
    return 1;
  }
  else {
    /* look for conflicts */
    for (s = CG_select; s; last = s, s = s->next ) {
      char *p; int len;

      /* take the first two fields of opt->name */
      p = strstr(opt->name, ":"); if (p++) p = strstr(p, ":");
      len = p - opt->name;

      /* compare these fields only */
      if (strncmp(opt->name, s->sel->name, len) == 0) {

        /* name:setting match; check for duplicate or alias...
           we must test the mask and hex value, not the
           value string, in order to support aliases */
        if ((opt->mask == s->sel->mask) &&
            (opt->value == s->sel->value))
          return 1;  /* exact match OK */
        else {
          fprintf(stderr,"Error: %s does not match %s\n", opt->name, s->sel->name);
          return 0;
        }
      }
    }
  }

  /* no conflict found; add to the end of the list */
  next = xmalloc(sizeof(codeguard_select_list_type));
  next->sel = (codeguard_setting_type *) opt;
  next->next = 0;

  last->next = next;
  return 1;
}


/*
** Dump selected CodeGuard options
*/
void pic30_dump_selected_codeguard_options(FILE *f)
{
  codeguard_select_list_type *s;

  if (!CG_select) return;

  fprintf(f,"\nSelected CodeGuard Options:\n");

  for (s = CG_select; s; s = s->next )
    fprintf(f,"  %s\n", s->sel->name);
}


/*
** Create a list of unique CodeGuard option configword names
**
** Note that options have the format "name:setting:value"
**
** The result is formatted as "name1,name2,nameN,"
*/
char * pic30_unique_selected_configword_names(void)
{
#define BUFFER_SIZE 40

  codeguard_select_list_type *s;
  char *p,*str,*result = xmalloc(BUFFER_SIZE);
  unsigned int total = 0;

  result[0] = '\0';
  for (s = CG_select; s; s = s->next ) {
    if (!s) continue;

    /* extract the config word name */
    str = strdup(s->sel->name);
    p = strtok(str, ":");
      
    /* loop if not unique */
    if (strstr(result, p)) {
      free(str);
      continue;
    }

    total += strlen(p);
    if (total >= BUFFER_SIZE - 2)
      result = realloc(result, total + BUFFER_SIZE);
    result = strcat(result, p);
    result = strcat(result, ",");
    free(str);
  }
  return result;
}



/*
** Decode a CodeGuard config word,
** validate the data and update CG_select.
**
** return code:
**   1 = valid settings found (and no errors)
**   0 = no valid settings found
*/
int pic30_decode_CG_settings(char *name, unsigned short value, int debug)
{
  codeguard_setting_type *s;
  int matches = 0, errors = 0;

  if (!CG_settings) return 0;

  for (s = CG_settings; s->next; s = s->next ) {
#if 0
    if (s) printf("  pic30_decode_CG_settings: %s, mask = %x, value = %x\n",
                  s->name, s->mask, s->value);
#endif
    if (s->name && strncmp(s->name, name, strlen(name)) == 0) {
      if ((value & s->mask) == s->value) {
        if (pic30_add_selected_codeguard_option(s))
          matches++;
        else
          errors++;
        if (debug)
          printf("  %s\n", s->name);
      }
    }
  }

  if (matches && !errors)
    return 1;
  else
    return 0;
}


unsigned short pic30_encode_CG_settings(char *name)
{
  codeguard_select_list_type *s;
  unsigned short result = ~0;

  if (!CG_select) return result;

  for (s = CG_select; s; s = s->next ) {
    if (strncmp(s->sel->name, name, strlen(name)) == 0)
      result = (result & ~(s->sel->mask)) | s->sel->value;
  }
  if (pic30_is_ecore_machine(global_PROCESSOR) && 
      (strcmp(name, "FGS") == 0)){
        if ((result& 3) == 3)
          result = result & 0xffcf;}

  return result;
}


/*
** Lookup a machine description
**
*/
const bfd_arch_info_type *
pic30_get_machine (name)
   const char * const name;
{

   const bfd_arch_info_type * p;
   struct pic30_resource_info *f;

   f = arch_flags_head[0].next;
   
   for (p = bfd_pic30_arch.next; p != NULL; f = f->next, p = p->next)
   {
      if (strcasecmp (name, p->printable_name) == 0)
         break;
   } /* for p */
   if (p) {
     if (f->arch_info != p) {
       fprintf(stderr,"Architecture description mis-match!\n");
     } else selected_processor = f;
   }
   return p;
}


/*
** Lookup a processor family
*/
int
pic30_proc_family(const bfd_arch_info_type *proc)
{
  int rc = -1;
  struct pic30_resource_info *f;

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & FAMILY_MASK;
    }

  return rc;
}


/*
** Query machine for DSP support
*/
int
pic30_is_dsp_machine(const bfd_arch_info_type *proc)
{
  int rc = 1;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,  */
    return rc;         /*  assume it supports DSP instructions */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_DSP;
    }

  return rc;
}


/*
** Query machine for EEPROM support
*/
int
pic30_is_eedata_machine(const bfd_arch_info_type *proc)
{
  int rc = 1;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,  */
    return rc;         /*  assume it supports EEDATA */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_EEDATA;
    }

  return rc;
}


/*
** Query machine for DMA support
*/
int
pic30_is_dma_machine(const bfd_arch_info_type *proc)
{
  int rc = 1;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,  */
    return rc;         /*  assume it supports DMA V1 */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = 0;
      if (f->flags & HAS_DMA) rc = 1;
      if (f->flags & HAS_DMAV2) rc = 2;
    }

  return rc;
}


/*
** Query machine for CodeGuard support
*/
int
pic30_is_codeguard_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,  */
    return rc;         /*  assume it does NOT support CodeGuard */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_CODEGUARD;
    }

  return rc;
}


/*
** Query machine for EDS support
*/
int
pic30_is_eds_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,        */
    return rc;         /*  assume it does NOT support EDS,           */
                       /*  allowing the sim30 device to use more RAM */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_EDS;
    }

  return rc;
}


/*
** Query machine for PMP support
*/
int
pic30_is_pmp_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,        */
    return rc;         /*  assume it does NOT support PMP            */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_PMP;
    }

  return rc;
}


int
pic30_is_epmp_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,        */
    return rc;         /*  assume it does NOT support EPMP           */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_PMPV2;
    }

  return rc;
}

int
pic30_is_ecore_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,        */
    return rc;         /*  assume it does NOT support ECORE          */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = f->flags & HAS_ECORE;
    }

  return rc;
}

int
pic30_is_5V_machine(const bfd_arch_info_type *proc)
{
  int rc = 0;
  struct pic30_resource_info *f;

  if (proc == NULL)    /* if no processor has been specified,        */
    return rc;         /*  assume it does NOT support ECORE          */

  for (f = arch_flags_head[0].next; f != NULL; f = f->next)
    if (proc == f->arch_info) {
      rc = (f->flags & HAS_5VOLTS) || (f->flags & P30F);
    }

  return rc;
}

int
pic30_display_as_program_memory_p(asection *sec)
{
  int fDisplay;

    fDisplay = ((sec->flags & SEC_CODE) | (sec->auxflash));

  return(fDisplay);
}

int
pic30_display_as_data_memory_p(asection *sec)
{
  int fDisplay;

  if ((sec->owner->flags & EXEC_P) == 0) /* unlinked object */
    fDisplay = !PIC30_IS_CODE_ATTR(sec) && !PIC30_IS_AUXFLASH_ATTR(sec);
  else
    fDisplay = (!PIC30_IS_CODE_ATTR(sec) &&
                !PIC30_IS_PSV_ATTR(sec) &&
                !PIC30_IS_EEDATA_ATTR(sec) &&
                !PIC30_IS_AUXFLASH_ATTR(sec));

  return(fDisplay);
}

int
pic30_display_as_readonly_memory_p(asection *sec)
{
  int fDisplay;

    fDisplay = ((PIC30_IS_PSV_ATTR(sec) || PIC30_IS_EEDATA_ATTR(sec))
                && ((sec->owner->flags & EXEC_P) == EXEC_P));

  return(fDisplay);
}
