#ifndef GCC_VERSION_H
#define GCC_VERSION_H
extern char *version_string;
extern const char bug_report_url[];

#ifdef MCHP_VERSION
#define PIC30_STRING1(x) #x
#define PIC30_STRING(x) PIC30_STRING1(x)
#define SET_MCHP_VERSION(x)                                        \
  { char *version = (char *)PIC30_STRING(MCHP_VERSION);            \
    char *c;                                                       \
    int mm = 0;                                                    \
    int major=0;                                                   \
    int minor=0;                                                   \
                                                                   \
    x = 0;                                                         \
    for (c = version; *c; c++) {                                   \
      if ((*c >= '0') && (*c <= '9')) {                            \
        if (mm == 1) minor = (minor * 10) + (*c - '0');            \
        else if (mm == 0) major = (major *10) + (*c - '0');        \
        /* else ignore it */                                       \
      } else if ((*c == '_') || (*c == '.')) mm++;                 \
    }                                                              \
    if (minor > 99) minor = 99;                                    \
    x = major*100 + minor;                                         \
  }
#endif

#endif /* ! GCC_VERSION_H */
