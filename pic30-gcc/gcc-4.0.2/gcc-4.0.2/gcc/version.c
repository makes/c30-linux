#include "version.h"

#define version2(X) #X
#define version(X) version2(X)

/* This is the string reported as the version number by all components
   of the compiler.  If you distribute a modified version of GCC,
   please modify this string to indicate that, e.g. by putting your
   organization's name in parentheses at the end of the string.  */

char *version_string = "4.0.3 (dsPIC30, Microchip " version(MCHP_VERSION)
                       ") Build date: " __DATE__;

/* This is the location of the online document giving instructions for
   reporting bugs.  If you distribute a modified version of GCC,
   please change this to refer to a document giving instructions for
   reporting bugs to you, not us.  (You are of course welcome to
   forward us bugs reported to you, if you determine that they are
   not bugs in your modifications.)  */

const char bug_report_url[] = "<URL:http://www.microchip.com/support>";
