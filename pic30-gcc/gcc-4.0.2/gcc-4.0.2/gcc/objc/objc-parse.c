/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     TYPENAME = 259,
     SCSPEC = 260,
     STATIC = 261,
     TYPESPEC = 262,
     TYPE_QUAL = 263,
     OBJC_TYPE_QUAL = 264,
     CONSTANT = 265,
     STRING = 266,
     ELLIPSIS = 267,
     SIZEOF = 268,
     ENUM = 269,
     STRUCT = 270,
     UNION = 271,
     IF = 272,
     ELSE = 273,
     WHILE = 274,
     DO = 275,
     FOR = 276,
     SWITCH = 277,
     CASE = 278,
     DEFAULT = 279,
     BREAK = 280,
     CONTINUE = 281,
     RETURN = 282,
     GOTO = 283,
     ASM_KEYWORD = 284,
     TYPEOF = 285,
     ALIGNOF = 286,
     ATTRIBUTE = 287,
     EXTENSION = 288,
     LABEL = 289,
     REALPART = 290,
     IMAGPART = 291,
     VA_ARG = 292,
     CHOOSE_EXPR = 293,
     TYPES_COMPATIBLE_P = 294,
     FUNC_NAME = 295,
     OFFSETOF = 296,
     ASSIGN = 297,
     OROR = 298,
     ANDAND = 299,
     EQCOMPARE = 300,
     ARITHCOMPARE = 301,
     RSHIFT = 302,
     LSHIFT = 303,
     MINUSMINUS = 304,
     PLUSPLUS = 305,
     UNARY = 306,
     HYPERUNARY = 307,
     POINTSAT = 308,
     AT_INTERFACE = 309,
     AT_IMPLEMENTATION = 310,
     AT_END = 311,
     AT_SELECTOR = 312,
     AT_DEFS = 313,
     AT_ENCODE = 314,
     CLASSNAME = 315,
     AT_PUBLIC = 316,
     AT_PRIVATE = 317,
     AT_PROTECTED = 318,
     AT_PROTOCOL = 319,
     AT_CLASS = 320,
     AT_ALIAS = 321,
     AT_THROW = 322,
     AT_TRY = 323,
     AT_CATCH = 324,
     AT_FINALLY = 325,
     AT_SYNCHRONIZED = 326,
     OBJC_STRING = 327
   };
#endif
#define IDENTIFIER 258
#define TYPENAME 259
#define SCSPEC 260
#define STATIC 261
#define TYPESPEC 262
#define TYPE_QUAL 263
#define OBJC_TYPE_QUAL 264
#define CONSTANT 265
#define STRING 266
#define ELLIPSIS 267
#define SIZEOF 268
#define ENUM 269
#define STRUCT 270
#define UNION 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define DO 275
#define FOR 276
#define SWITCH 277
#define CASE 278
#define DEFAULT 279
#define BREAK 280
#define CONTINUE 281
#define RETURN 282
#define GOTO 283
#define ASM_KEYWORD 284
#define TYPEOF 285
#define ALIGNOF 286
#define ATTRIBUTE 287
#define EXTENSION 288
#define LABEL 289
#define REALPART 290
#define IMAGPART 291
#define VA_ARG 292
#define CHOOSE_EXPR 293
#define TYPES_COMPATIBLE_P 294
#define FUNC_NAME 295
#define OFFSETOF 296
#define ASSIGN 297
#define OROR 298
#define ANDAND 299
#define EQCOMPARE 300
#define ARITHCOMPARE 301
#define RSHIFT 302
#define LSHIFT 303
#define MINUSMINUS 304
#define PLUSPLUS 305
#define UNARY 306
#define HYPERUNARY 307
#define POINTSAT 308
#define AT_INTERFACE 309
#define AT_IMPLEMENTATION 310
#define AT_END 311
#define AT_SELECTOR 312
#define AT_DEFS 313
#define AT_ENCODE 314
#define CLASSNAME 315
#define AT_PUBLIC 316
#define AT_PRIVATE 317
#define AT_PROTECTED 318
#define AT_PROTOCOL 319
#define AT_CLASS 320
#define AT_ALIAS 321
#define AT_THROW 322
#define AT_TRY 323
#define AT_CATCH 324
#define AT_FINALLY 325
#define AT_SYNCHRONIZED 326
#define OBJC_STRING 327




/* Copy the first part of user declarations.  */
#line 33 "objc/objc-parse.y"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "langhooks.h"
#include "input.h"
#include "cpplib.h"
#include "intl.h"
#include "timevar.h"
#include "c-pragma.h"		/* For YYDEBUG definition, and parse_in.  */
#include "c-tree.h"
#include "flags.h"
#include "varray.h"
#include "output.h"
#include "toplev.h"
#include "ggc.h"
#include "c-common.h"

#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

/* Like the default stack expander, except (1) use realloc when possible,
   (2) impose no hard maxiumum on stack size, (3) REALLY do not use alloca.

   Irritatingly, YYSTYPE is defined after this %{ %} block, so we cannot
   give malloced_yyvs its proper type.  This is ok since all we need from
   it is to be able to free it.  */

static short *malloced_yyss;
static void *malloced_yyvs;

#define yyoverflow(MSG, SS, SSSIZE, VS, VSSIZE, YYSSZ)			\
do {									\
  size_t newsize;							\
  short *newss;								\
  YYSTYPE *newvs;							\
  newsize = *(YYSSZ) *= 2;						\
  if (malloced_yyss)							\
    {									\
      newss = really_call_realloc (*(SS), newsize * sizeof (short));	\
      newvs = really_call_realloc (*(VS), newsize * sizeof (YYSTYPE));	\
    }									\
  else									\
    {									\
      newss = really_call_malloc (newsize * sizeof (short));		\
      newvs = really_call_malloc (newsize * sizeof (YYSTYPE));		\
      if (newss)							\
        memcpy (newss, *(SS), (SSSIZE));				\
      if (newvs)							\
        memcpy (newvs, *(VS), (VSSIZE));				\
    }									\
  if (!newss || !newvs)							\
    {									\
      yyerror (MSG);							\
      return 2;								\
    }									\
  *(SS) = newss;							\
  *(VS) = newvs;							\
  malloced_yyss = newss;						\
  malloced_yyvs = (void *) newvs;					\
} while (0)


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 99 "objc/objc-parse.y"
typedef union YYSTYPE {long itype; tree ttype; void *otype; struct c_expr exprtype;
	struct c_arg_info *arginfotype; struct c_declarator *dtrtype;
	struct c_type_name *typenametype; struct c_parm *parmtype;
	struct c_declspecs *dsptype; struct c_typespec tstype;
	enum tree_code code; location_t location; } YYSTYPE;
/* Line 190 of yacc.c.  */
#line 290 "objc/objc-parse.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 264 "objc/objc-parse.y"

/* Declaration specifiers of the current declaration.  */
static struct c_declspecs *current_declspecs;
static GTY(()) tree prefix_attributes;

/* List of all the attributes applying to the identifier currently being
   declared; includes prefix_attributes and possibly some more attributes
   just after a comma.  */
static GTY(()) tree all_prefix_attributes;

/* Structure to save declaration specifiers.  */
struct c_declspec_stack {
  /* Saved value of current_declspecs.  */
  struct c_declspecs *current_declspecs;
  /* Saved value of prefix_attributes.  */
  tree prefix_attributes;
  /* Saved value of all_prefix_attributes.  */
  tree all_prefix_attributes;
  /* Next level of stack.  */
  struct c_declspec_stack *next;
};

/* Stack of saved values of current_declspecs, prefix_attributes and
   all_prefix_attributes.  */
static struct c_declspec_stack *declspec_stack;

/* INDIRECT_REF with a TREE_TYPE of the type being queried for offsetof.  */
static tree offsetof_base;

/* PUSH_DECLSPEC_STACK is called from setspecs; POP_DECLSPEC_STACK
   should be called from the productions making use of setspecs.  */
#define PUSH_DECLSPEC_STACK						\
  do {									\
    struct c_declspec_stack *t = XOBNEW (&parser_obstack,		\
					 struct c_declspec_stack);	\
    t->current_declspecs = current_declspecs;				\
    t->prefix_attributes = prefix_attributes;				\
    t->all_prefix_attributes = all_prefix_attributes;			\
    t->next = declspec_stack;						\
    declspec_stack = t;							\
  } while (0)

#define POP_DECLSPEC_STACK						\
  do {									\
    current_declspecs = declspec_stack->current_declspecs;		\
    prefix_attributes = declspec_stack->prefix_attributes;		\
    all_prefix_attributes = declspec_stack->all_prefix_attributes;	\
    declspec_stack = declspec_stack->next;				\
  } while (0)

/* For __extension__, save/restore the warning flags which are
   controlled by __extension__.  */
#define SAVE_EXT_FLAGS()		\
	(pedantic			\
	 | (warn_pointer_arith << 1)	\
	 | (warn_traditional << 2)	\
	 | (flag_iso << 3))

#define RESTORE_EXT_FLAGS(val)			\
  do {						\
    pedantic = val & 1;				\
    warn_pointer_arith = (val >> 1) & 1;	\
    warn_traditional = (val >> 2) & 1;		\
    flag_iso = (val >> 3) & 1;			\
  } while (0)

/* Objective-C specific parser/lexer information */

static int objc_pq_context = 0;

/* The following flag is needed to contextualize ObjC lexical analysis.
   In some cases (e.g., 'int NSObject;'), it is undesirable to bind
   an identifier to an ObjC class, even if a class with that name
   exists.  */
static int objc_need_raw_identifier;
#define OBJC_NEED_RAW_IDENTIFIER(VAL)	objc_need_raw_identifier = VAL


/* Tell yyparse how to print a token's value, if yydebug is set.  */

#define YYPRINT(FILE,YYCHAR,YYLVAL) yyprint(FILE,YYCHAR,YYLVAL)

static void yyprint (FILE *, int, YYSTYPE);
static void yyerror (const char *);
static int yylexname (void);
static inline int _yylex (void);
static int  yylex (void);
static void init_reswords (void);

  /* Initialization routine for this file.  */
void
c_parse_init (void)
{
  init_reswords ();
}



/* Line 213 of yacc.c.  */
#line 399 "objc/objc-parse.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   4880

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  270
/* YYNRULES -- Number of rules. */
#define YYNRULES  720
/* YYNRULES -- Number of states. */
#define YYNSTATES  1168

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   327

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,     2,     2,     2,    59,    50,     2,
      65,    92,    57,    55,    91,    56,    64,    58,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    87,
       2,    42,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    94,    49,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    93,    48,    88,    89,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    43,    46,    47,
      51,    52,    53,    54,    60,    61,    62,    63,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     6,     7,    11,    12,    17,    19,
      21,    23,    26,    28,    29,    33,    38,    43,    46,    49,
      52,    54,    55,    56,    65,    70,    71,    72,    81,    86,
      87,    88,    96,   100,   102,   104,   106,   108,   110,   112,
     114,   116,   118,   120,   122,   126,   127,   129,   131,   135,
     137,   140,   143,   146,   149,   152,   157,   160,   165,   168,
     171,   173,   175,   177,   179,   184,   186,   190,   194,   198,
     202,   206,   210,   214,   218,   222,   226,   230,   234,   235,
     240,   241,   246,   247,   248,   256,   257,   263,   267,   271,
     273,   275,   277,   279,   280,   288,   292,   296,   300,   304,
     309,   316,   317,   325,   330,   339,   344,   351,   356,   361,
     365,   369,   372,   375,   377,   379,   381,   383,   385,   387,
     391,   396,   397,   399,   402,   404,   406,   409,   412,   417,
     422,   425,   428,   431,   432,   434,   439,   444,   448,   452,
     455,   458,   460,   463,   466,   469,   472,   475,   477,   480,
     482,   485,   488,   491,   494,   497,   500,   502,   505,   508,
     511,   514,   517,   520,   523,   526,   529,   532,   535,   538,
     541,   544,   547,   550,   552,   555,   558,   561,   564,   567,
     570,   573,   576,   579,   582,   585,   588,   591,   594,   597,
     600,   603,   606,   609,   612,   615,   618,   621,   624,   627,
     630,   633,   636,   639,   642,   645,   648,   651,   654,   657,
     660,   663,   666,   669,   672,   675,   678,   681,   684,   686,
     688,   690,   692,   694,   696,   698,   700,   702,   704,   706,
     708,   710,   712,   714,   716,   718,   720,   722,   724,   726,
     728,   730,   732,   734,   736,   738,   740,   742,   744,   746,
     748,   750,   752,   754,   756,   758,   760,   762,   764,   766,
     768,   770,   772,   774,   776,   778,   780,   782,   784,   786,
     788,   790,   792,   794,   796,   797,   799,   801,   803,   805,
     807,   809,   811,   813,   816,   819,   821,   826,   831,   833,
     838,   840,   845,   846,   853,   857,   858,   865,   869,   870,
     872,   874,   877,   886,   890,   892,   896,   897,   899,   904,
     911,   916,   918,   920,   922,   924,   926,   928,   930,   931,
     936,   938,   939,   942,   944,   948,   952,   955,   956,   961,
     963,   964,   969,   971,   973,   975,   978,   981,   983,   989,
     993,   994,   995,  1002,  1003,  1004,  1011,  1013,  1015,  1020,
    1024,  1027,  1031,  1033,  1035,  1037,  1041,  1044,  1046,  1050,
    1053,  1057,  1061,  1066,  1070,  1075,  1079,  1082,  1084,  1086,
    1089,  1091,  1094,  1096,  1099,  1100,  1108,  1114,  1115,  1123,
    1129,  1130,  1139,  1140,  1148,  1151,  1154,  1157,  1158,  1160,
    1161,  1163,  1165,  1168,  1169,  1173,  1176,  1181,  1185,  1188,
    1192,  1194,  1196,  1199,  1201,  1206,  1208,  1213,  1216,  1221,
    1225,  1228,  1233,  1237,  1239,  1243,  1245,  1247,  1251,  1252,
    1256,  1257,  1259,  1260,  1262,  1265,  1267,  1269,  1271,  1275,
    1278,  1282,  1287,  1291,  1294,  1297,  1299,  1304,  1308,  1313,
    1319,  1325,  1327,  1329,  1331,  1333,  1335,  1338,  1341,  1344,
    1347,  1349,  1352,  1355,  1358,  1360,  1363,  1366,  1369,  1372,
    1374,  1377,  1379,  1381,  1383,  1385,  1388,  1389,  1390,  1392,
    1394,  1397,  1401,  1403,  1406,  1408,  1410,  1414,  1416,  1418,
    1421,  1424,  1425,  1426,  1429,  1433,  1436,  1439,  1442,  1446,
    1450,  1452,  1462,  1472,  1480,  1488,  1489,  1490,  1500,  1501,
    1502,  1516,  1517,  1519,  1522,  1524,  1527,  1529,  1542,  1543,
    1552,  1555,  1557,  1559,  1561,  1563,  1565,  1568,  1571,  1574,
    1578,  1580,  1584,  1589,  1591,  1595,  1598,  1600,  1607,  1612,
    1616,  1621,  1622,  1625,  1626,  1632,  1636,  1638,  1641,  1643,
    1645,  1649,  1655,  1658,  1663,  1670,  1671,  1673,  1676,  1681,
    1690,  1692,  1696,  1702,  1710,  1711,  1713,  1714,  1716,  1718,
    1722,  1729,  1739,  1741,  1745,  1747,  1748,  1749,  1750,  1754,
    1757,  1758,  1759,  1766,  1769,  1770,  1772,  1774,  1778,  1780,
    1784,  1789,  1794,  1798,  1803,  1807,  1812,  1817,  1821,  1826,
    1830,  1832,  1833,  1837,  1839,  1842,  1844,  1848,  1850,  1854,
    1856,  1858,  1860,  1862,  1864,  1866,  1868,  1870,  1874,  1878,
    1883,  1886,  1887,  1891,  1892,  1893,  1894,  1904,  1905,  1911,
    1912,  1922,  1928,  1929,  1936,  1940,  1941,  1943,  1947,  1948,
    1952,  1953,  1955,  1957,  1959,  1960,  1964,  1967,  1969,  1970,
    1972,  1974,  1976,  1977,  1978,  1985,  1986,  1989,  1990,  1994,
    1996,  1998,  1999,  2000,  2006,  2011,  2013,  2019,  2022,  2025,
    2026,  2030,  2031,  2034,  2036,  2038,  2041,  2043,  2045,  2047,
    2049,  2051,  2053,  2055,  2057,  2059,  2061,  2063,  2065,  2067,
    2069,  2071,  2073,  2075,  2077,  2079,  2081,  2083,  2085,  2087,
    2089,  2091,  2093,  2095,  2098,  2099,  2102,  2104,  2111,  2115,
    2121,  2124,  2126,  2128,  2130,  2133,  2135,  2139,  2142,  2144,
    2146,  2148,  2153,  2155,  2157,  2159,  2162,  2165,  2167,  2172,
    2177
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
      96,     0,    -1,    -1,    97,    -1,    -1,   101,    98,   100,
      -1,    -1,    97,   101,    99,   100,    -1,   103,    -1,   102,
      -1,   284,    -1,   310,   100,    -1,   311,    -1,    -1,   135,
     169,    87,    -1,   155,   135,   169,    87,    -1,   154,   135,
     168,    87,    -1,   161,    87,    -1,     1,    87,    -1,     1,
      88,    -1,    87,    -1,    -1,    -1,   154,   135,   198,   104,
     130,   250,   105,   244,    -1,   154,   135,   198,     1,    -1,
      -1,    -1,   155,   135,   203,   106,   130,   250,   107,   244,
      -1,   155,   135,   203,     1,    -1,    -1,    -1,   135,   203,
     108,   130,   250,   109,   244,    -1,   135,   203,     1,    -1,
       3,    -1,     4,    -1,    74,    -1,    50,    -1,    56,    -1,
      55,    -1,    61,    -1,    60,    -1,    89,    -1,    90,    -1,
     120,    -1,   112,    91,   120,    -1,    -1,   114,    -1,   120,
      -1,   114,    91,   120,    -1,   126,    -1,    57,   119,    -1,
     310,   119,    -1,   111,   119,    -1,    47,   110,    -1,   116,
     115,    -1,   116,    65,   224,    92,    -1,   117,   115,    -1,
     117,    65,   224,    92,    -1,    35,   119,    -1,    36,   119,
      -1,    13,    -1,    31,    -1,    30,    -1,   115,    -1,    65,
     224,    92,   119,    -1,   119,    -1,   120,    55,   120,    -1,
     120,    56,   120,    -1,   120,    57,   120,    -1,   120,    58,
     120,    -1,   120,    59,   120,    -1,   120,    54,   120,    -1,
     120,    53,   120,    -1,   120,    52,   120,    -1,   120,    51,
     120,    -1,   120,    50,   120,    -1,   120,    48,   120,    -1,
     120,    49,   120,    -1,    -1,   120,    47,   121,   120,    -1,
      -1,   120,    46,   122,   120,    -1,    -1,    -1,   120,    44,
     123,   112,    45,   124,   120,    -1,    -1,   120,    44,   125,
      45,   120,    -1,   120,    42,   120,    -1,   120,    43,   120,
      -1,     3,    -1,    10,    -1,    11,    -1,    40,    -1,    -1,
      65,   224,    92,    93,   127,   183,    88,    -1,    65,   112,
      92,    -1,    65,     1,    92,    -1,   248,   246,    92,    -1,
     248,     1,    92,    -1,   126,    65,   113,    92,    -1,    37,
      65,   120,    91,   224,    92,    -1,    -1,    41,    65,   224,
      91,   128,   129,    92,    -1,    41,    65,     1,    92,    -1,
      38,    65,   120,    91,   120,    91,   120,    92,    -1,    38,
      65,     1,    92,    -1,    39,    65,   224,    91,   224,    92,
      -1,    39,    65,     1,    92,    -1,   126,    66,   112,    94,
      -1,   126,    64,   110,    -1,   126,    67,   110,    -1,   126,
      61,    -1,   126,    60,    -1,   358,    -1,   362,    -1,   363,
      -1,   364,    -1,    86,    -1,   110,    -1,   129,    64,   110,
      -1,   129,    66,   112,    94,    -1,    -1,   132,    -1,   250,
     133,    -1,   131,    -1,   239,    -1,   132,   131,    -1,   131,
     239,    -1,   156,   135,   168,    87,    -1,   157,   135,   169,
      87,    -1,   156,    87,    -1,   157,    87,    -1,   250,   137,
      -1,    -1,   174,    -1,   154,   135,   168,    87,    -1,   155,
     135,   169,    87,    -1,   154,   135,   192,    -1,   155,   135,
     195,    -1,   161,    87,    -1,   310,   137,    -1,     8,    -1,
     138,     8,    -1,   139,     8,    -1,   138,   175,    -1,   140,
       8,    -1,   141,     8,    -1,   175,    -1,   140,   175,    -1,
     163,    -1,   142,     8,    -1,   143,     8,    -1,   142,   165,
      -1,   143,   165,    -1,   138,   163,    -1,   139,   163,    -1,
     164,    -1,   142,   175,    -1,   142,   166,    -1,   143,   166,
      -1,   138,   164,    -1,   139,   164,    -1,   144,     8,    -1,
     145,     8,    -1,   144,   165,    -1,   145,   165,    -1,   140,
     163,    -1,   141,   163,    -1,   144,   175,    -1,   144,   166,
      -1,   145,   166,    -1,   140,   164,    -1,   141,   164,    -1,
     180,    -1,   146,     8,    -1,   147,     8,    -1,   138,   180,
      -1,   139,   180,    -1,   146,   180,    -1,   147,   180,    -1,
     146,   175,    -1,   148,     8,    -1,   149,     8,    -1,   140,
     180,    -1,   141,   180,    -1,   148,   180,    -1,   149,   180,
      -1,   148,   175,    -1,   150,     8,    -1,   151,     8,    -1,
     150,   165,    -1,   151,   165,    -1,   146,   163,    -1,   147,
     163,    -1,   142,   180,    -1,   143,   180,    -1,   150,   180,
      -1,   151,   180,    -1,   150,   175,    -1,   150,   166,    -1,
     151,   166,    -1,   146,   164,    -1,   147,   164,    -1,   152,
       8,    -1,   153,     8,    -1,   152,   165,    -1,   153,   165,
      -1,   148,   163,    -1,   149,   163,    -1,   144,   180,    -1,
     145,   180,    -1,   152,   180,    -1,   153,   180,    -1,   152,
     175,    -1,   152,   166,    -1,   153,   166,    -1,   148,   164,
      -1,   149,   164,    -1,   142,    -1,   143,    -1,   144,    -1,
     145,    -1,   150,    -1,   151,    -1,   152,    -1,   153,    -1,
     138,    -1,   139,    -1,   140,    -1,   141,    -1,   146,    -1,
     147,    -1,   148,    -1,   149,    -1,   142,    -1,   143,    -1,
     150,    -1,   151,    -1,   138,    -1,   139,    -1,   146,    -1,
     147,    -1,   142,    -1,   143,    -1,   144,    -1,   145,    -1,
     138,    -1,   139,    -1,   140,    -1,   141,    -1,   142,    -1,
     143,    -1,   144,    -1,   145,    -1,   138,    -1,   139,    -1,
     140,    -1,   141,    -1,   138,    -1,   139,    -1,   140,    -1,
     141,    -1,   142,    -1,   143,    -1,   144,    -1,   145,    -1,
     146,    -1,   147,    -1,   148,    -1,   149,    -1,   150,    -1,
     151,    -1,   152,    -1,   153,    -1,    -1,   159,    -1,   165,
      -1,   167,    -1,   166,    -1,     7,    -1,   212,    -1,   207,
      -1,     4,    -1,    74,   324,    -1,     4,   325,    -1,   325,
      -1,   118,    65,   112,    92,    -1,   118,    65,   224,    92,
      -1,   170,    -1,   168,    91,   136,   170,    -1,   172,    -1,
     169,    91,   136,   172,    -1,    -1,   198,   283,   174,    42,
     171,   181,    -1,   198,   283,   174,    -1,    -1,   203,   283,
     174,    42,   173,   181,    -1,   203,   283,   174,    -1,    -1,
     175,    -1,   176,    -1,   175,   176,    -1,    32,   293,    65,
      65,   177,    92,    92,   294,    -1,    32,     1,   294,    -1,
     178,    -1,   177,    91,   178,    -1,    -1,   179,    -1,   179,
      65,     3,    92,    -1,   179,    65,     3,    91,   114,    92,
      -1,   179,    65,   113,    92,    -1,   110,    -1,   180,    -1,
       7,    -1,     8,    -1,     6,    -1,     5,    -1,   120,    -1,
      -1,    93,   182,   183,    88,    -1,     1,    -1,    -1,   184,
     213,    -1,   185,    -1,   184,    91,   185,    -1,   189,    42,
     187,    -1,   191,   187,    -1,    -1,   110,    45,   186,   187,
      -1,   187,    -1,    -1,    93,   188,   183,    88,    -1,   120,
      -1,     1,    -1,   190,    -1,   189,   190,    -1,    64,   110,
      -1,   191,    -1,    66,   120,    12,   120,    94,    -1,    66,
     120,    94,    -1,    -1,    -1,   198,   193,   130,   250,   194,
     249,    -1,    -1,    -1,   203,   196,   130,   250,   197,   249,
      -1,   199,    -1,   203,    -1,    65,   174,   199,    92,    -1,
     199,    65,   305,    -1,   199,   232,    -1,    57,   162,   199,
      -1,     4,    -1,   201,    -1,   202,    -1,   201,    65,   305,
      -1,   201,   232,    -1,     4,    -1,   202,    65,   305,    -1,
     202,   232,    -1,    57,   162,   201,    -1,    57,   162,   202,
      -1,    65,   174,   202,    92,    -1,   203,    65,   305,    -1,
      65,   174,   203,    92,    -1,    57,   162,   203,    -1,   203,
     232,    -1,     3,    -1,    15,    -1,    15,   175,    -1,    16,
      -1,    16,   175,    -1,    14,    -1,    14,   175,    -1,    -1,
     204,   110,    93,   208,   215,    88,   174,    -1,   204,    93,
     215,    88,   174,    -1,    -1,   205,   110,    93,   209,   215,
      88,   174,    -1,   205,    93,   215,    88,   174,    -1,    -1,
     206,   110,    93,   210,   222,   214,    88,   174,    -1,    -1,
     206,    93,   211,   222,   214,    88,   174,    -1,   204,   110,
      -1,   205,   110,    -1,   206,   110,    -1,    -1,    91,    -1,
      -1,    91,    -1,   216,    -1,   216,   217,    -1,    -1,   216,
     217,    87,    -1,   216,    87,    -1,    72,    65,    74,    92,
      -1,   158,   135,   218,    -1,   158,   135,    -1,   159,   135,
     219,    -1,   159,    -1,     1,    -1,   310,   217,    -1,   220,
      -1,   218,    91,   136,   220,    -1,   221,    -1,   219,    91,
     136,   221,    -1,   198,   174,    -1,   198,    45,   120,   174,
      -1,    45,   120,   174,    -1,   203,   174,    -1,   203,    45,
     120,   174,    -1,    45,   120,   174,    -1,   223,    -1,   222,
      91,   223,    -1,     1,    -1,   110,    -1,   110,    42,   120,
      -1,    -1,   160,   225,   226,    -1,    -1,   228,    -1,    -1,
     228,    -1,   229,   175,    -1,   230,    -1,   229,    -1,   231,
      -1,    57,   162,   229,    -1,    57,   162,    -1,    57,   162,
     230,    -1,    65,   174,   228,    92,    -1,   231,    65,   295,
      -1,   231,   232,    -1,    65,   295,    -1,   232,    -1,    66,
     162,   120,    94,    -1,    66,   162,    94,    -1,    66,   162,
      57,    94,    -1,    66,     6,   162,   120,    94,    -1,    66,
     159,     6,   120,    94,    -1,   234,    -1,   235,    -1,   236,
      -1,   237,    -1,   253,    -1,   234,   253,    -1,   235,   253,
      -1,   236,   253,    -1,   237,   253,    -1,   134,    -1,   234,
     134,    -1,   235,   134,    -1,   237,   134,    -1,   254,    -1,
     234,   254,    -1,   235,   254,    -1,   236,   254,    -1,   237,
     254,    -1,   239,    -1,   238,   239,    -1,   234,    -1,   235,
      -1,   236,    -1,   237,    -1,     1,    87,    -1,    -1,    -1,
     242,    -1,   243,    -1,   242,   243,    -1,    34,   309,    87,
      -1,   249,    -1,     1,   249,    -1,    93,    -1,    88,    -1,
     241,   247,    88,    -1,   233,    -1,     1,    -1,    65,    93,
      -1,   245,   246,    -1,    -1,    -1,   251,   254,    -1,   240,
     251,   253,    -1,   250,   280,    -1,   250,   281,    -1,   250,
     112,    -1,   240,   251,   258,    -1,   240,   251,    87,    -1,
     252,    -1,    17,   240,   250,    65,   255,    92,   256,    18,
     257,    -1,    17,   240,   250,    65,   255,    92,   257,    18,
     257,    -1,    17,   240,   250,    65,   255,    92,   256,    -1,
      17,   240,   250,    65,   255,    92,   257,    -1,    -1,    -1,
      19,   240,   250,    65,   255,    92,   259,   260,   252,    -1,
      -1,    -1,    20,   240,   250,   259,   260,   252,    19,   263,
     264,    65,   255,    92,    87,    -1,    -1,   112,    -1,   265,
      87,    -1,   137,    -1,   250,   265,    -1,   265,    -1,    21,
     240,    65,   266,   250,   267,    87,   268,    92,   259,   260,
     252,    -1,    -1,    22,   240,    65,   112,    92,   271,   259,
     252,    -1,   112,    87,    -1,   258,    -1,   261,    -1,   262,
      -1,   269,    -1,   270,    -1,    25,    87,    -1,    26,    87,
      -1,    27,    87,    -1,    27,   112,    87,    -1,   285,    -1,
      28,   110,    87,    -1,    28,    57,   112,    87,    -1,    87,
      -1,    81,   112,    87,    -1,    81,    87,    -1,   279,    -1,
      85,   250,    65,   112,    92,   249,    -1,    83,    65,   302,
      92,    -1,   273,    93,   246,    -1,   273,    93,     1,    88,
      -1,    -1,   275,   274,    -1,    -1,    82,   250,   249,   277,
     275,    -1,    84,   250,   249,    -1,   276,    -1,   276,   278,
      -1,   249,    -1,   272,    -1,    23,   120,    45,    -1,    23,
     120,    12,   120,    45,    -1,    24,    45,    -1,   110,   250,
      45,   174,    -1,    29,   293,    65,   292,    92,   294,    -1,
      -1,   282,    -1,   282,    87,    -1,    29,     1,   294,    87,
      -1,    29,   287,   293,    65,   286,    92,   294,    87,    -1,
     292,    -1,   292,    45,   288,    -1,   292,    45,   288,    45,
     288,    -1,   292,    45,   288,    45,   288,    45,   291,    -1,
      -1,     8,    -1,    -1,   289,    -1,   290,    -1,   289,    91,
     290,    -1,   292,   294,    65,   112,    92,   293,    -1,    66,
     110,    94,   292,   294,    65,   112,    92,   293,    -1,   292,
      -1,   291,    91,   292,    -1,    11,    -1,    -1,    -1,    -1,
     174,   296,   297,    -1,   300,    92,    -1,    -1,    -1,   301,
      87,   298,   174,   299,   297,    -1,     1,    92,    -1,    -1,
      12,    -1,   301,    -1,   301,    91,    12,    -1,   303,    -1,
     301,    91,   302,    -1,   154,   135,   200,   174,    -1,   154,
     135,   203,   174,    -1,   154,   135,   227,    -1,   155,   135,
     203,   174,    -1,   155,   135,   227,    -1,   156,   304,   200,
     174,    -1,   156,   304,   203,   174,    -1,   156,   304,   227,
      -1,   157,   304,   203,   174,    -1,   157,   304,   227,    -1,
     135,    -1,    -1,   174,   306,   307,    -1,   297,    -1,   308,
      92,    -1,     3,    -1,   308,    91,     3,    -1,   110,    -1,
     309,    91,   110,    -1,    33,    -1,   317,    -1,   313,    -1,
     314,    -1,   322,    -1,   332,    -1,    70,    -1,   110,    -1,
     312,    91,   110,    -1,    79,   312,    87,    -1,    80,   110,
     110,    87,    -1,    45,   110,    -1,    -1,    93,   326,    88,
      -1,    -1,    -1,    -1,    68,   110,   315,   324,   318,   316,
     319,   335,    70,    -1,    -1,    69,   110,   315,   320,   316,
      -1,    -1,    68,   110,    65,   110,    92,   324,   321,   335,
      70,    -1,    69,   110,    65,   110,    92,    -1,    -1,    78,
     110,   324,   323,   335,    70,    -1,    78,   312,    87,    -1,
      -1,   325,    -1,    52,   312,    52,    -1,    -1,   326,   327,
     328,    -1,    -1,    76,    -1,    77,    -1,    75,    -1,    -1,
     328,   329,    87,    -1,   328,    87,    -1,   217,    -1,    -1,
      87,    -1,    55,    -1,    56,    -1,    -1,    -1,   331,   333,
     341,   330,   334,   244,    -1,    -1,   335,   338,    -1,    -1,
     335,   336,   102,    -1,    87,    -1,     1,    -1,    -1,    -1,
     331,   339,   341,   340,   337,    -1,    65,   351,    92,   345,
      -1,   345,    -1,    65,   351,    92,   346,   342,    -1,   346,
     342,    -1,   343,   344,    -1,    -1,   343,    91,   302,    -1,
      -1,    91,    12,    -1,   347,    -1,   352,    -1,   346,   352,
      -1,     3,    -1,     4,    -1,    74,    -1,   348,    -1,    14,
      -1,    15,    -1,    16,    -1,    17,    -1,    18,    -1,    19,
      -1,    20,    -1,    21,    -1,    22,    -1,    23,    -1,    24,
      -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,    29,
      -1,    13,    -1,    30,    -1,    31,    -1,     7,    -1,     8,
      -1,     9,    -1,     9,    -1,   350,   349,    -1,    -1,   350,
     224,    -1,   350,    -1,   347,    45,    65,   351,    92,   110,
      -1,   347,    45,   110,    -1,    45,    65,   351,    92,   110,
      -1,    45,   110,    -1,   347,    -1,   354,    -1,   356,    -1,
     354,   356,    -1,   114,    -1,   347,    45,   355,    -1,    45,
     355,    -1,   112,    -1,    74,    -1,     4,    -1,    66,   357,
     353,    94,    -1,   347,    -1,   360,    -1,   361,    -1,   360,
     361,    -1,   347,    45,    -1,    45,    -1,    71,    65,   359,
      92,    -1,    78,    65,   110,    92,    -1,    73,    65,   224,
      92,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   364,   364,   367,   375,   375,   378,   377,   383,   384,
     385,   386,   388,   394,   398,   401,   403,   405,   407,   408,
     409,   416,   421,   415,   426,   429,   434,   428,   439,   442,
     447,   441,   452,   457,   458,   459,   462,   464,   466,   469,
     471,   473,   475,   479,   480,   487,   488,   492,   494,   499,
     500,   504,   507,   512,   515,   522,   526,   531,   536,   539,
     545,   549,   553,   557,   558,   564,   565,   567,   569,   571,
     573,   575,   577,   579,   581,   583,   585,   587,   590,   589,
     597,   596,   604,   608,   603,   616,   615,   627,   631,   639,
     646,   648,   650,   654,   653,   674,   679,   681,   687,   692,
     695,   700,   699,   709,   711,   723,   725,   737,   739,   742,
     745,   751,   754,   757,   760,   763,   766,   769,   779,   781,
     783,   787,   789,   796,   801,   802,   803,   804,   812,   814,
     816,   819,   828,   837,   857,   862,   864,   866,   868,   870,
     872,   918,   920,   922,   927,   932,   934,   939,   941,   946,
     948,   950,   952,   954,   956,   958,   963,   965,   967,   969,
     971,   973,   978,   980,   982,   984,   986,   988,   993,   995,
     997,   999,  1001,  1006,  1008,  1010,  1012,  1014,  1016,  1018,
    1023,  1028,  1030,  1032,  1034,  1036,  1038,  1043,  1048,  1050,
    1052,  1054,  1056,  1058,  1060,  1062,  1064,  1066,  1071,  1073,
    1075,  1077,  1079,  1084,  1086,  1088,  1090,  1092,  1094,  1096,
    1098,  1100,  1102,  1107,  1109,  1111,  1113,  1115,  1121,  1122,
    1123,  1124,  1125,  1126,  1127,  1128,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1143,  1144,  1145,  1146,  1150,  1151,
    1152,  1153,  1157,  1158,  1159,  1160,  1164,  1165,  1166,  1167,
    1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1182,  1183,
    1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,
    1194,  1195,  1196,  1197,  1203,  1204,  1230,  1231,  1235,  1239,
    1243,  1247,  1251,  1256,  1259,  1265,  1268,  1278,  1290,  1291,
    1295,  1296,  1301,  1300,  1315,  1325,  1324,  1339,  1349,  1350,
    1355,  1357,  1362,  1365,  1370,  1372,  1378,  1379,  1381,  1383,
    1385,  1393,  1394,  1395,  1396,  1400,  1401,  1407,  1410,  1409,
    1413,  1420,  1422,  1426,  1427,  1433,  1436,  1440,  1439,  1445,
    1450,  1449,  1453,  1455,  1459,  1460,  1464,  1466,  1470,  1474,
    1480,  1492,  1479,  1510,  1522,  1509,  1542,  1543,  1549,  1551,
    1553,  1555,  1557,  1566,  1567,  1571,  1573,  1575,  1580,  1582,
    1584,  1586,  1588,  1596,  1598,  1600,  1602,  1604,  1609,  1611,
    1616,  1618,  1623,  1625,  1637,  1636,  1644,  1651,  1650,  1656,
    1663,  1662,  1669,  1668,  1677,  1679,  1681,  1689,  1691,  1694,
    1696,  1714,  1716,  1722,  1723,  1725,  1729,  1734,  1737,  1745,
    1748,  1753,  1755,  1761,  1762,  1767,  1768,  1773,  1777,  1781,
    1789,  1793,  1797,  1808,  1809,  1814,  1820,  1822,  1828,  1827,
    1838,  1839,  1844,  1846,  1849,  1856,  1857,  1861,  1862,  1867,
    1870,  1875,  1877,  1879,  1881,  1884,  1892,  1894,  1896,  1898,
    1901,  1912,  1913,  1914,  1918,  1922,  1923,  1924,  1925,  1926,
    1930,  1931,  1937,  1938,  1942,  1943,  1944,  1945,  1946,  1950,
    1951,  1955,  1956,  1957,  1958,  1961,  1966,  1971,  1973,  1979,
    1980,  1984,  1998,  2000,  2003,  2006,  2007,  2011,  2012,  2016,
    2027,  2036,  2041,  2043,  2048,  2053,  2071,  2075,  2088,  2093,
    2097,  2101,  2105,  2109,  2113,  2120,  2124,  2128,  2139,  2140,
    2137,  2149,  2150,  2155,  2157,  2161,  2173,  2178,  2189,  2188,
    2201,  2203,  2205,  2207,  2209,  2211,  2213,  2215,  2217,  2219,
    2221,  2222,  2224,  2226,  2228,  2230,  2232,  2234,  2239,  2244,
    2246,  2250,  2252,  2257,  2256,  2262,  2267,  2269,  2275,  2277,
    2284,  2286,  2288,  2290,  2307,  2315,  2316,  2321,  2323,  2330,
    2337,  2340,  2343,  2346,  2354,  2355,  2369,  2370,  2374,  2375,
    2380,  2384,  2392,  2394,  2400,  2412,  2416,  2427,  2426,  2435,
    2437,  2439,  2436,  2443,  2454,  2459,  2468,  2470,  2475,  2477,
    2484,  2488,  2492,  2495,  2500,  2508,  2512,  2516,  2519,  2524,
    2530,  2540,  2539,  2548,  2549,  2564,  2566,  2572,  2574,  2579,
    2590,  2591,  2592,  2593,  2594,  2595,  2603,  2605,  2610,  2617,
    2624,  2625,  2629,  2630,  2635,  2639,  2634,  2648,  2647,  2657,
    2656,  2665,  2673,  2672,  2685,  2693,  2696,  2700,  2709,  2711,
    2714,  2716,  2717,  2718,  2721,  2723,  2724,  2732,  2743,  2745,
    2753,  2754,  2759,  2764,  2758,  2778,  2780,  2781,  2781,  2785,
    2786,  2791,  2797,  2790,  2806,  2811,  2816,  2821,  2831,  2839,
    2842,  2851,  2854,  2861,  2865,  2867,  2874,  2875,  2876,  2877,
    2881,  2881,  2881,  2881,  2881,  2881,  2881,  2881,  2882,  2882,
    2882,  2882,  2882,  2882,  2883,  2883,  2883,  2883,  2883,  2884,
    2884,  2884,  2888,  2892,  2897,  2903,  2907,  2914,  2919,  2924,
    2929,  2936,  2937,  2941,  2942,  2950,  2962,  2966,  2973,  2975,
    2979,  2986,  2991,  2992,  2996,  2997,  3004,  3008,  3015,  3022,
    3031
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "TYPENAME", "SCSPEC",
  "STATIC", "TYPESPEC", "TYPE_QUAL", "OBJC_TYPE_QUAL", "CONSTANT",
  "STRING", "ELLIPSIS", "SIZEOF", "ENUM", "STRUCT", "UNION", "IF", "ELSE",
  "WHILE", "DO", "FOR", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE",
  "RETURN", "GOTO", "ASM_KEYWORD", "TYPEOF", "ALIGNOF", "ATTRIBUTE",
  "EXTENSION", "LABEL", "REALPART", "IMAGPART", "VA_ARG", "CHOOSE_EXPR",
  "TYPES_COMPATIBLE_P", "FUNC_NAME", "OFFSETOF", "'='", "ASSIGN", "'?'",
  "':'", "OROR", "ANDAND", "'|'", "'^'", "'&'", "EQCOMPARE",
  "ARITHCOMPARE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'",
  "MINUSMINUS", "PLUSPLUS", "UNARY", "HYPERUNARY", "'.'", "'('", "'['",
  "POINTSAT", "AT_INTERFACE", "AT_IMPLEMENTATION", "AT_END", "AT_SELECTOR",
  "AT_DEFS", "AT_ENCODE", "CLASSNAME", "AT_PUBLIC", "AT_PRIVATE",
  "AT_PROTECTED", "AT_PROTOCOL", "AT_CLASS", "AT_ALIAS", "AT_THROW",
  "AT_TRY", "AT_CATCH", "AT_FINALLY", "AT_SYNCHRONIZED", "OBJC_STRING",
  "';'", "'}'", "'~'", "'!'", "','", "')'", "'{'", "']'", "$accept",
  "program", "extdefs", "@1", "@2", "extdef", "save_obstack_position",
  "datadef", "fndef", "@3", "@4", "@5", "@6", "@7", "@8", "identifier",
  "unop", "expr", "exprlist", "nonnull_exprlist", "unary_expr", "sizeof",
  "alignof", "typeof", "cast_expr", "expr_no_commas", "@9", "@10", "@11",
  "@12", "@13", "primary", "@14", "@15", "offsetof_member_designator",
  "old_style_parm_decls", "lineno_datadecl", "datadecls", "datadecl",
  "lineno_decl", "setspecs", "maybe_resetattrs", "decl",
  "declspecs_nosc_nots_nosa_noea", "declspecs_nosc_nots_nosa_ea",
  "declspecs_nosc_nots_sa_noea", "declspecs_nosc_nots_sa_ea",
  "declspecs_nosc_ts_nosa_noea", "declspecs_nosc_ts_nosa_ea",
  "declspecs_nosc_ts_sa_noea", "declspecs_nosc_ts_sa_ea",
  "declspecs_sc_nots_nosa_noea", "declspecs_sc_nots_nosa_ea",
  "declspecs_sc_nots_sa_noea", "declspecs_sc_nots_sa_ea",
  "declspecs_sc_ts_nosa_noea", "declspecs_sc_ts_nosa_ea",
  "declspecs_sc_ts_sa_noea", "declspecs_sc_ts_sa_ea", "declspecs_ts",
  "declspecs_nots", "declspecs_ts_nosa", "declspecs_nots_nosa",
  "declspecs_nosc_ts", "declspecs_nosc_nots", "declspecs_nosc",
  "declspecs", "maybe_type_quals_attrs", "typespec_nonattr",
  "typespec_attr", "typespec_reserved_nonattr", "typespec_reserved_attr",
  "typespec_nonreserved_nonattr", "initdecls", "notype_initdecls",
  "initdcl", "@16", "notype_initdcl", "@17", "maybe_attribute",
  "attributes", "attribute", "attribute_list", "attrib", "any_word",
  "scspec", "init", "@18", "initlist_maybe_comma", "initlist1", "initelt",
  "@19", "initval", "@20", "designator_list", "designator",
  "array_designator", "nested_function", "@21", "@22",
  "notype_nested_function", "@23", "@24", "declarator",
  "after_type_declarator", "parm_declarator",
  "parm_declarator_starttypename", "parm_declarator_nostarttypename",
  "notype_declarator", "struct_head", "union_head", "enum_head",
  "structsp_attr", "@25", "@26", "@27", "@28", "structsp_nonattr",
  "maybecomma", "maybecomma_warn", "component_decl_list",
  "component_decl_list2", "component_decl", "components",
  "components_notype", "component_declarator",
  "component_notype_declarator", "enumlist", "enumerator", "typename",
  "@29", "absdcl", "absdcl_maybe_attribute", "absdcl1", "absdcl1_noea",
  "absdcl1_ea", "direct_absdcl1", "array_declarator", "stmts_and_decls",
  "lineno_stmt_decl_or_labels_ending_stmt",
  "lineno_stmt_decl_or_labels_ending_decl",
  "lineno_stmt_decl_or_labels_ending_label",
  "lineno_stmt_decl_or_labels_ending_error", "lineno_stmt_decl_or_labels",
  "errstmt", "c99_block_start", "maybe_label_decls", "label_decls",
  "label_decl", "compstmt_or_error", "compstmt_start", "compstmt_nostart",
  "compstmt_contents_nonempty", "compstmt_primary_start", "compstmt",
  "save_location", "lineno_labels", "c99_block_lineno_labeled_stmt",
  "lineno_stmt", "lineno_label", "condition", "if_statement_1",
  "if_statement_2", "if_statement", "start_break", "start_continue",
  "while_statement", "do_statement", "@30", "@31", "xexpr",
  "for_init_stmt", "for_cond_expr", "for_incr_expr", "for_statement",
  "switch_statement", "@32", "stmt_nocomp", "objc_catch_prefix",
  "objc_catch_clause", "objc_opt_catch_list", "objc_try_catch_clause",
  "@33", "objc_finally_clause", "objc_try_catch_stmt", "stmt", "label",
  "simple_asm_expr", "maybeasm", "asmdef", "asm_stmt", "asm_argument",
  "maybe_volatile", "asm_operands", "nonnull_asm_operands", "asm_operand",
  "asm_clobbers", "asm_string", "stop_string_translation",
  "start_string_translation", "parmlist", "@34", "parmlist_1", "@35",
  "@36", "parmlist_2", "parms", "parm", "firstparm", "setspecs_fp",
  "parmlist_or_identifiers", "@37", "parmlist_or_identifiers_1",
  "identifiers", "identifiers_or_typenames", "extension", "objcdef",
  "identifier_list", "classdecl", "aliasdecl", "superclass", "class_ivars",
  "classdef", "@38", "@39", "@40", "@41", "protocoldef", "@42",
  "protocolrefs", "non_empty_protocolrefs", "ivar_decl_list",
  "visibility_spec", "ivar_decls", "ivar_decl", "opt_semi", "methodtype",
  "methoddef", "@43", "@44", "methodprotolist", "@45", "semi_or_error",
  "methodproto", "@46", "@47", "methoddecl", "optparmlist", "optparms",
  "optellipsis", "unaryselector", "keywordselector", "selector",
  "reservedwords", "objc_qual", "objc_quals", "objc_typename",
  "keyworddecl", "messageargs", "keywordarglist", "keywordexpr",
  "keywordarg", "receiver", "objcmessageexpr", "selectorarg",
  "keywordnamelist", "keywordname", "objcselectorexpr", "objcprotocolexpr",
  "objcencodeexpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    61,   297,    63,    58,   298,   299,   124,    94,
      38,   300,   301,   302,   303,    43,    45,    42,    47,    37,
     304,   305,   306,   307,    46,    40,    91,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,    59,   125,   126,
      33,    44,    41,   123,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,    95,    96,    96,    98,    97,    99,    97,   100,   100,
     100,   100,   100,   101,   102,   102,   102,   102,   102,   102,
     102,   104,   105,   103,   103,   106,   107,   103,   103,   108,
     109,   103,   103,   110,   110,   110,   111,   111,   111,   111,
     111,   111,   111,   112,   112,   113,   113,   114,   114,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   117,   118,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   121,   120,
     122,   120,   123,   124,   120,   125,   120,   120,   120,   126,
     126,   126,   126,   127,   126,   126,   126,   126,   126,   126,
     126,   128,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   129,   129,
     129,   130,   130,   131,   132,   132,   132,   132,   133,   133,
     133,   133,   134,   135,   136,   137,   137,   137,   137,   137,
     137,   138,   138,   138,   139,   140,   140,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   143,   143,   143,   143,
     143,   143,   144,   144,   144,   144,   144,   144,   145,   145,
     145,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     147,   148,   148,   148,   148,   148,   148,   149,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   151,   151,
     151,   151,   151,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   153,   153,   153,   153,   153,   154,   154,
     154,   154,   154,   154,   154,   154,   155,   155,   155,   155,
     155,   155,   155,   155,   156,   156,   156,   156,   157,   157,
     157,   157,   158,   158,   158,   158,   159,   159,   159,   159,
     160,   160,   160,   160,   160,   160,   160,   160,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   162,   162,   163,   163,   164,   165,
     165,   166,   167,   167,   167,   167,   167,   167,   168,   168,
     169,   169,   171,   170,   170,   173,   172,   172,   174,   174,
     175,   175,   176,   176,   177,   177,   178,   178,   178,   178,
     178,   179,   179,   179,   179,   180,   180,   181,   182,   181,
     181,   183,   183,   184,   184,   185,   185,   186,   185,   185,
     188,   187,   187,   187,   189,   189,   190,   190,   191,   191,
     193,   194,   192,   196,   197,   195,   198,   198,   199,   199,
     199,   199,   199,   200,   200,   201,   201,   201,   202,   202,
     202,   202,   202,   203,   203,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   208,   207,   207,   209,   207,   207,
     210,   207,   211,   207,   212,   212,   212,   213,   213,   214,
     214,   215,   215,   216,   216,   216,   216,   217,   217,   217,
     217,   217,   217,   218,   218,   219,   219,   220,   220,   220,
     221,   221,   221,   222,   222,   222,   223,   223,   225,   224,
     226,   226,   227,   227,   227,   228,   228,   229,   229,   230,
     230,   231,   231,   231,   231,   231,   232,   232,   232,   232,
     232,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     235,   235,   235,   235,   236,   236,   236,   236,   236,   237,
     237,   238,   238,   238,   238,   239,   240,   241,   241,   242,
     242,   243,   244,   244,   245,   246,   246,   247,   247,   248,
     249,   250,   251,   251,   252,   253,   254,   255,   256,   257,
     257,   258,   258,   258,   258,   259,   260,   261,   263,   264,
     262,   265,   265,   266,   266,   267,   268,   269,   271,   270,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   273,   274,
     274,   275,   275,   277,   276,   278,   279,   279,   280,   280,
     281,   281,   281,   281,   282,   283,   283,   284,   284,   285,
     286,   286,   286,   286,   287,   287,   288,   288,   289,   289,
     290,   290,   291,   291,   292,   293,   294,   296,   295,   297,
     298,   299,   297,   297,   300,   300,   300,   300,   301,   301,
     302,   302,   302,   302,   302,   303,   303,   303,   303,   303,
     304,   306,   305,   307,   307,   308,   308,   309,   309,   310,
     311,   311,   311,   311,   311,   311,   312,   312,   313,   314,
     315,   315,   316,   316,   318,   319,   317,   320,   317,   321,
     317,   317,   323,   322,   322,   324,   324,   325,   326,   326,
     327,   327,   327,   327,   328,   328,   328,   329,   330,   330,
     331,   331,   333,   334,   332,   335,   335,   336,   335,   337,
     337,   339,   340,   338,   341,   341,   341,   341,   342,   343,
     343,   344,   344,   345,   346,   346,   347,   347,   347,   347,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   349,   350,   350,   351,   351,   352,   352,   352,
     352,   353,   353,   354,   354,   355,   356,   356,   357,   357,
     357,   358,   359,   359,   360,   360,   361,   361,   362,   363,
     364
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     0,     3,     0,     4,     1,     1,
       1,     2,     1,     0,     3,     4,     4,     2,     2,     2,
       1,     0,     0,     8,     4,     0,     0,     8,     4,     0,
       0,     7,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     4,     2,     4,     2,     2,
       1,     1,     1,     1,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     0,     4,
       0,     4,     0,     0,     7,     0,     5,     3,     3,     1,
       1,     1,     1,     0,     7,     3,     3,     3,     3,     4,
       6,     0,     7,     4,     8,     4,     6,     4,     4,     3,
       3,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       4,     0,     1,     2,     1,     1,     2,     2,     4,     4,
       2,     2,     2,     0,     1,     4,     4,     3,     3,     2,
       2,     1,     2,     2,     2,     2,     2,     1,     2,     1,
       2,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     4,     4,     1,     4,
       1,     4,     0,     6,     3,     0,     6,     3,     0,     1,
       1,     2,     8,     3,     1,     3,     0,     1,     4,     6,
       4,     1,     1,     1,     1,     1,     1,     1,     0,     4,
       1,     0,     2,     1,     3,     3,     2,     0,     4,     1,
       0,     4,     1,     1,     1,     2,     2,     1,     5,     3,
       0,     0,     6,     0,     0,     6,     1,     1,     4,     3,
       2,     3,     1,     1,     1,     3,     2,     1,     3,     2,
       3,     3,     4,     3,     4,     3,     2,     1,     1,     2,
       1,     2,     1,     2,     0,     7,     5,     0,     7,     5,
       0,     8,     0,     7,     2,     2,     2,     0,     1,     0,
       1,     1,     2,     0,     3,     2,     4,     3,     2,     3,
       1,     1,     2,     1,     4,     1,     4,     2,     4,     3,
       2,     4,     3,     1,     3,     1,     1,     3,     0,     3,
       0,     1,     0,     1,     2,     1,     1,     1,     3,     2,
       3,     4,     3,     2,     2,     1,     4,     3,     4,     5,
       5,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       1,     2,     2,     2,     1,     2,     2,     2,     2,     1,
       2,     1,     1,     1,     1,     2,     0,     0,     1,     1,
       2,     3,     1,     2,     1,     1,     3,     1,     1,     2,
       2,     0,     0,     2,     3,     2,     2,     2,     3,     3,
       1,     9,     9,     7,     7,     0,     0,     9,     0,     0,
      13,     0,     1,     2,     1,     2,     1,    12,     0,     8,
       2,     1,     1,     1,     1,     1,     2,     2,     2,     3,
       1,     3,     4,     1,     3,     2,     1,     6,     4,     3,
       4,     0,     2,     0,     5,     3,     1,     2,     1,     1,
       3,     5,     2,     4,     6,     0,     1,     2,     4,     8,
       1,     3,     5,     7,     0,     1,     0,     1,     1,     3,
       6,     9,     1,     3,     1,     0,     0,     0,     3,     2,
       0,     0,     6,     2,     0,     1,     1,     3,     1,     3,
       4,     4,     3,     4,     3,     4,     4,     3,     4,     3,
       1,     0,     3,     1,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     4,
       2,     0,     3,     0,     0,     0,     9,     0,     5,     0,
       9,     5,     0,     6,     3,     0,     1,     3,     0,     3,
       0,     1,     1,     1,     0,     3,     2,     1,     0,     1,
       1,     1,     0,     0,     6,     0,     2,     0,     3,     1,
       1,     0,     0,     5,     4,     1,     5,     2,     2,     0,
       3,     0,     2,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     2,     1,     6,     3,     5,
       2,     1,     1,     1,     2,     1,     3,     2,     1,     1,
       1,     4,     1,     1,     1,     2,     2,     1,     4,     4,
       4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
      13,     0,    13,     4,     1,     6,     0,     0,     0,   282,
     316,   315,   279,   141,   372,   368,   370,     0,    62,     0,
     599,     0,   640,   641,     0,     0,   605,   625,     0,     0,
       0,    20,     5,     9,     8,     0,     0,   226,   227,   228,
     229,   218,   219,   220,   221,   230,   231,   232,   233,   222,
     223,   224,   225,   133,   133,     0,   149,   156,   276,   278,
     277,   147,   300,   173,     0,     0,     0,   281,   280,     0,
      10,     0,    12,   601,   602,   600,   603,   285,   642,   604,
       7,    18,    19,   284,   373,   369,   371,   566,     0,   566,
       0,    33,    34,    35,   606,     0,   611,   611,   283,   626,
     625,     0,     0,     0,     0,   367,   274,   298,     0,   290,
       0,   142,   154,   160,   144,   176,   143,   155,   161,   177,
     145,   166,   171,   148,   183,   146,   167,   172,   184,   150,
     152,   158,   157,   194,   151,   153,   159,   195,   162,   164,
     169,   168,   209,   163,   165,   170,   210,   174,   192,   201,
     180,   178,   175,   193,   202,   179,   181,   207,   216,   187,
     185,   182,   208,   217,   186,   188,   190,   199,   198,   196,
     189,   191,   200,   197,   203,   205,   214,   213,   211,   204,
     206,   215,   212,     0,     0,    17,   301,   393,   384,   393,
     385,   382,   386,   547,    11,     0,     0,     0,   303,     0,
     627,     0,     0,     0,   625,     0,   617,   622,   624,   608,
       0,    89,    90,    91,    60,    61,     0,     0,     0,     0,
       0,    92,     0,     0,    36,    38,    37,     0,    40,    39,
       0,     0,     0,     0,     0,   117,    41,    42,     0,     0,
      63,     0,     0,    65,    43,    49,   254,   255,   256,   257,
     250,   251,   252,   253,   418,     0,     0,     0,   113,   114,
     115,   116,   246,   247,   248,   249,   275,     0,     0,   299,
      14,   298,    32,   565,   298,   274,     0,   366,   546,   298,
     352,   274,   298,     0,   288,     0,   346,   347,     0,     0,
       0,     0,     0,   374,     0,   377,     0,   380,   666,   667,
     689,   690,   691,   686,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     687,   688,     0,   694,   668,   638,   655,   659,   663,   669,
     664,   548,   564,     0,   306,   607,   610,     0,   614,     0,
     613,   645,   609,    58,    59,     0,     0,     0,     0,    53,
      50,     0,   479,     0,     0,   710,   709,   708,     0,     0,
       0,     0,    52,     0,   286,     0,    54,     0,    56,     0,
       0,    82,    80,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,   111,     0,    45,
       0,     0,   420,   287,     0,     0,   475,     0,   468,   469,
       0,    51,   365,     0,     0,   134,   591,   363,   274,   275,
       0,     0,   481,     0,   481,   125,     0,   297,     0,     0,
      16,   298,    24,     0,   298,   298,   350,    15,    28,     0,
       0,   298,   401,   395,   246,   247,   248,   249,   242,   243,
     244,   245,   133,   133,   392,     0,   393,   298,   393,   415,
     416,   389,   413,     0,   694,   700,   696,     0,   639,   643,
     657,   661,     0,   665,     0,   566,   313,   314,   311,     0,
     304,   307,   312,   625,   613,   621,   628,   618,   647,     0,
       0,     0,     0,     0,     0,     0,    96,    95,     0,     0,
     701,     0,   702,   703,   717,   712,     0,   713,   714,     0,
       0,    44,     0,     0,    87,    88,     0,     0,     0,     0,
      76,    77,    75,    74,    73,    72,    71,    66,    67,    68,
      69,    70,   109,     0,    46,    47,     0,   110,   274,   298,
     419,   421,   426,   425,   427,   435,    98,   597,     0,   478,
     450,   477,   481,   481,   481,   481,     0,   459,     0,     0,
     445,   454,   470,    97,   364,   291,   545,     0,     0,     0,
       0,   437,     0,   465,    30,   127,   126,   123,   238,   239,
     234,   235,   240,   241,   236,   237,   133,   133,   295,   351,
       0,     0,   481,   294,   349,   481,     0,   376,   398,     0,
     394,   402,     0,   379,     0,     0,   390,     0,   389,     0,
     692,   695,   693,     0,     0,     0,   658,   694,   698,   544,
     306,     0,    45,   619,   615,   630,   623,   651,     0,   646,
       0,   105,     0,   107,     0,   103,   101,    93,    64,   705,
     707,     0,   711,     0,   704,   716,   718,     0,   715,   720,
     719,    55,    57,     0,     0,    81,    79,    99,     0,   108,
     429,   567,   434,   298,   433,   471,     0,   451,   446,   455,
     452,   447,   456,     0,   448,   457,   453,   449,   458,   460,
     476,    89,   282,   466,   466,   466,   466,   466,     0,     0,
       0,     0,     0,     0,   554,   625,     0,   481,   481,   523,
     474,   481,     0,   132,   133,   133,     0,   467,   538,   511,
     512,   513,   514,   515,   539,   536,   526,   485,   486,   520,
       0,     0,   595,   575,   133,   133,   593,     0,   576,   578,
     592,     0,     0,     0,   438,   436,     0,   130,     0,   131,
       0,     0,   348,   289,   545,    22,   292,    26,   396,     0,
     298,   397,   403,     0,   298,   399,   405,   298,   298,   417,
     414,   298,     0,     0,   654,   659,     0,   644,   472,   662,
     226,   227,   228,   229,   218,   219,   220,   221,   230,   231,
     232,   233,   222,   223,   224,   225,   133,   133,   660,     0,
     305,   566,    89,     0,   645,   645,   633,   631,   632,   612,
     634,     0,   648,     0,   133,   133,     0,     0,     0,     0,
       0,   706,    83,    86,    48,   428,   430,     0,     0,   567,
     432,   598,   481,   481,   481,     0,     0,     0,   542,   516,
     517,   518,     0,     0,     0,   555,   565,   525,     0,     0,
       0,     0,   510,     0,     0,   139,   480,   481,   537,   140,
     573,   590,   422,   422,   569,   570,     0,     0,   594,   439,
     440,    31,     0,     0,   320,   318,   317,   296,     0,     0,
       0,   298,     0,   407,   298,   298,     0,   410,   298,   375,
     378,   383,   298,   699,   656,   473,   422,   422,     0,   302,
       0,   308,   310,   647,   647,     0,   652,     0,     0,   100,
       0,   106,   118,     0,   333,     0,     0,   330,     0,   332,
       0,   387,   323,   329,     0,   334,     0,     0,   431,   568,
       0,     0,   495,   501,     0,     0,   540,   519,     0,   521,
       0,   524,   533,     0,   298,     0,   137,   340,     0,   138,
     343,     0,   357,   274,   298,   298,   353,   354,   298,   587,
     423,   426,   274,   298,   298,   589,   298,   577,   579,   596,
     128,   129,     0,    23,   293,    27,   409,   298,     0,   412,
     298,     0,   381,   298,   298,   582,   298,   584,   697,     0,
     620,   616,   636,   637,     0,     0,     0,     0,     0,   102,
     336,    43,     0,   327,    94,     0,   322,     0,     0,   335,
     337,   326,    84,   481,   481,   496,   502,   504,     0,   481,
       0,     0,   522,     0,   531,     0,   543,   135,     0,   136,
       0,   535,   429,   567,   585,   298,   356,   298,   359,   586,
     424,   429,   567,   588,   571,     0,   408,   404,   411,   406,
     580,   581,   583,   309,   635,   650,   649,   653,   104,   119,
       0,     0,   339,     0,     0,   324,   325,     0,     0,     0,
       0,   466,   503,   481,   508,   541,     0,   550,   534,     0,
     481,   481,   360,   361,     0,   355,   358,     0,   319,   120,
       0,   331,   328,   487,   466,   495,   482,     0,   501,     0,
     495,   566,   556,     0,     0,   532,   527,   341,   344,   362,
     572,   338,   482,   490,   493,   494,   496,   481,   498,   505,
     501,   466,     0,     0,   551,   557,   558,   566,     0,     0,
       0,     0,   481,   466,   466,   466,   484,   483,   499,   506,
       0,   509,   549,     0,   556,     0,     0,     0,     0,   529,
     342,   345,   489,   488,   482,   491,   492,   497,     0,   495,
       0,   552,   559,     0,   528,   530,   481,   481,   496,   566,
       0,     0,     0,   466,     0,   553,   562,   565,     0,   507,
       0,     0,   560,   500,     0,   563,   565,   561
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,     2,     6,     7,    32,     3,    33,    34,   423,
     858,   429,   860,   276,   726,   898,   238,   353,   523,   524,
     240,   241,   242,    35,   243,   244,   509,   508,   506,   907,
     507,   245,   800,   799,   893,   412,   413,   414,   567,   540,
      36,   404,   693,   246,   247,   248,   249,   250,   251,   252,
     253,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,   714,   715,   442,   266,   254,    55,   267,    56,    57,
      58,    59,    60,   283,   108,   284,   859,   109,   731,   405,
     269,    62,   469,   470,   471,    63,   857,   952,   900,   901,
     902,  1044,   903,   982,   904,   905,   906,   926,  1008,  1110,
     929,  1010,  1111,   734,   286,   935,   936,   937,   287,    64,
      65,    66,    67,   446,   448,   453,   296,    68,   986,   597,
     291,   292,   444,   741,   745,   742,   746,   451,   452,   255,
     392,   530,   939,   940,   532,   533,   534,   277,   541,   542,
     543,   544,   545,   546,   415,  1076,   397,   398,   399,   757,
     697,   400,   548,   256,   758,   416,  1097,  1093,  1116,  1117,
    1049,  1094,  1095,   699,   995,  1051,   700,   701,  1118,  1138,
     998,   999,  1079,  1120,   702,   703,  1080,   704,  1084,  1085,
    1058,   705,  1004,   838,   706,   707,   708,   278,   279,    70,
     709,  1056,   826,  1104,  1105,  1106,  1155,  1107,    88,   196,
     652,   808,   716,   946,  1067,   717,   718,   778,   719,   842,
     407,   557,   720,   721,   538,   257,    72,    95,    73,    74,
     204,   477,    75,   474,   785,   340,   784,    76,   341,    98,
      77,   615,   790,   885,   974,   459,    78,    79,   195,   604,
     478,   618,  1037,   619,   791,   975,   325,   460,   461,   606,
     326,   327,   328,   329,   602,   456,   457,   330,   491,   492,
     630,   493,   358,   258,   496,   497,   498,   259,   260,   261
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -970
static const short int yypact[] =
{
     135,   149,   154,  -970,  -970,  -970,  3846,  3846,   514,   111,
    -970,  -970,  -970,  -970,   153,   153,   153,   117,  -970,   128,
    -970,   312,  -970,  -970,   312,   312,  -970,   111,   312,   312,
     312,  -970,  -970,  -970,  -970,   136,   356,   637,  1165,  3950,
    4005,   126,   573,   373,  1301,  3971,  4045,  4024,  4066,   730,
    1423,   767,  1516,  -970,  -970,    86,  -970,  -970,  -970,  -970,
    -970,   153,  -970,  -970,   134,   158,   161,  -970,  -970,   122,
    -970,  3846,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,   153,   153,   153,  -970,   187,  -970,
     193,  -970,  -970,  -970,  -970,   208,   345,   449,  -970,  -970,
     363,   366,   378,   312,  3249,  -970,   302,   153,   391,  -970,
    1296,  -970,  -970,  -970,   153,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,   153,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,   153,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,   153,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
     153,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,   153,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,   153,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,   153,  -970,  -970,
    -970,  -970,  -970,   367,   356,  -970,  -970,   219,   251,   219,
     261,  -970,   274,  -970,  -970,  4160,   230,   374,  -970,   331,
    -970,   312,   312,   312,   111,   312,  -970,  -970,  -970,  -970,
     314,  -970,  -970,  -970,  -970,  -970,  3509,  3509,   347,   358,
     379,  -970,   398,   312,  -970,  -970,  -970,  3509,  -970,  -970,
    2331,  3315,   435,   438,   440,  -970,  -970,  -970,  3509,   484,
    -970,  3573,  3637,  -970,  4821,   812,  4476,  4538,  4506,  4558,
     699,   999,   902,  1056,  -970,   336,  2051,  3509,  -970,  -970,
    -970,  -970,   375,   480,   385,   512,  -970,   356,   356,   153,
    -970,   153,  -970,  -970,   153,   344,  1397,  -970,  -970,   153,
    -970,   302,   153,   461,  -970,  2583,   638,   711,   473,  2553,
     459,   443,  3796,  -970,   450,  -970,   279,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,   258,  -970,  -970,   456,  -970,  4193,   546,  -970,
    -970,  -970,  -970,   522,   667,  -970,  -970,   537,  -970,   540,
     554,  -970,  -970,  -970,  -970,  3509,  3077,   823,  4098,  -970,
    -970,   564,  -970,   712,   566,  -970,  -970,   575,  4228,  4261,
    4519,   312,  -970,  3509,  -970,  2331,  -970,  2331,  -970,  3509,
    3509,   632,  -970,  -970,  3509,  3509,  3509,  3509,  3509,  3509,
    3509,  3509,  3509,  3509,  3509,  3509,  -970,  -970,   312,  3509,
    3509,   312,   488,  -970,   593,   312,  -970,  2240,   653,  -970,
     604,  -970,   711,   433,   356,  -970,  -970,  -970,   302,   693,
    2628,   614,  -970,  1451,    88,  -970,  1577,   670,   367,   367,
    -970,   153,  -970,  1397,   153,   153,  -970,  -970,  -970,  1397,
     635,   153,  -970,  -970,  4476,  4538,  4506,  4558,   699,   999,
     902,  1056,  -970,   726,   639,  2817,   219,   153,   219,  -970,
     686,   649,  -970,   279,  -970,  -970,  3878,   651,  -970,  -970,
    -970,   656,   546,  -970,   291,  -970,  -970,  -970,  -970,   727,
    -970,   703,  -970,   111,   554,  -970,  -970,  -970,   457,  4689,
     666,  4716,   672,   679,   695,   687,  -970,  -970,  3011,  3509,
     740,   716,  4228,  -970,  -970,   760,   724,  4261,  -970,   734,
     744,  4821,   753,   755,  4821,  4821,  3509,   787,  3509,  3509,
    1884,  1913,  1992,  1109,   736,   694,   694,   659,   659,  -970,
    -970,  -970,  -970,   770,   737,  4821,   424,  -970,   302,   153,
    -970,  -970,  -970,  -970,   756,  -970,  -970,  -970,   483,   614,
    -970,  -970,   106,   107,   109,   110,   858,  -970,   776,  2857,
    -970,  -970,  -970,  -970,  -970,  -970,   264,  1535,  3509,  3509,
    2692,  -970,  4571,  -970,  -970,  -970,  -970,  -970,  4308,  4414,
     996,  1817,  4338,  4427,  1642,  1835,   784,   794,  -970,   638,
     464,   367,  -970,   851,  -970,  -970,   811,  -970,   337,   239,
    -970,  -970,   817,  -970,   824,  3509,   312,   831,   649,   829,
    -970,  -970,  -970,  4193,    96,  4119,  -970,  -970,  -970,  -970,
     667,   835,  3701,  -970,  -970,   645,  -970,  -970,  3931,  -970,
    4519,  -970,  3509,  -970,  4519,  -970,  -970,  -970,  -970,   737,
    -970,  3509,  -970,   740,  -970,  -970,  -970,   760,  -970,  -970,
    -970,   830,   830,   108,  3509,  1505,  1735,  -970,  3509,  -970,
     488,   488,  -970,   153,  -970,  -970,   312,  -970,  -970,  -970,
    -970,  -970,  -970,  2945,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,   894,   201,  -970,  -970,  -970,  -970,  -970,  3509,   896,
     863,   867,  3381,   318,   948,   382,  3445,  -970,  -970,  -970,
    -970,  -970,   495,  -970,  -970,  -970,   871,    71,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,   885,  -970,  -970,  -970,  -970,
    3165,   881,  -970,  -970,  -970,  -970,  -970,   884,   530,  -970,
    -970,   751,  4591,  4620,  -970,  -970,    96,  -970,   367,  -970,
     356,  1949,  -970,  -970,   949,  -970,  -970,  -970,  -970,  3509,
     224,   889,  -970,  3509,   332,   897,  -970,   153,   153,  4821,
    -970,   153,   908,   312,  -970,  4193,   904,  -970,  -970,  -970,
    4308,  4414,  4343,  4444,   996,  1817,  1728,  1839,  4338,  4427,
    4372,  4457,  1642,  1835,  1766,  1863,  -970,  -970,  -970,   906,
    -970,  -970,   766,   907,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  4160,  -970,   356,  -970,  -970,   917,  4735,   924,   312,
    1826,  -970,  -970,  4548,  4821,  -970,  -970,   925,  2783,  -970,
    -970,  -970,  -970,  -970,  -970,   953,   954,  4785,  -970,  -970,
    -970,  -970,   547,  3509,   933,  -970,  -970,  -970,   559,   904,
     957,   978,  -970,   367,   356,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,   445,   334,  -970,  -970,  4289,  1022,  -970,  -970,
    -970,  -970,   570,   577,  -970,  -970,  4821,  -970,    96,  1949,
      96,  4766,  3509,  -970,   153,  4766,  3509,  -970,   153,  -970,
    -970,  -970,   153,  -970,  -970,  -970,   445,   334,   312,  -970,
    3509,  -970,  -970,   539,   542,  3769,  -970,   367,   356,  -970,
    3509,  -970,  -970,   269,  -970,   312,  3315,  -970,   982,  4821,
     941,   942,  -970,  -970,   417,  -970,  1631,  3509,  -970,  -970,
     967,   969,  -970,  3165,  3509,  3509,  -970,  -970,   589,  -970,
     970,  -970,  -970,  3509,   153,   591,  -970,   455,   599,  -970,
     783,   904,  -970,   302,   153,   153,   801,   826,   242,  -970,
    -970,   153,   302,   153,   242,  -970,   153,  -970,  -970,  -970,
    -970,  -970,  1826,  -970,  -970,  -970,  -970,  4766,   337,  -970,
    4766,   239,  -970,   153,   242,  -970,   242,  -970,  -970,   797,
    -970,  -970,  -970,  -970,   950,   113,  4669,   312,  3509,  -970,
    -970,  1236,  1826,  -970,  -970,  2397,  -970,  2463,  3509,  -970,
    -970,  -970,  4548,  -970,  -970,  -970,   575,  -970,   951,  -970,
     809,  4803,  -970,   374,  -970,   852,  -970,  -970,  1397,  -970,
    1397,  -970,   445,   354,  -970,   153,  -970,   153,  -970,  -970,
     153,   334,   334,  -970,  -970,   960,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
     517,  3509,  -970,   961,  2463,  -970,  -970,  1236,  3509,   944,
     958,  -970,  -970,  -970,  -970,  -970,   964,  1015,   983,   904,
    -970,  -970,   801,   826,   493,  -970,  -970,  2783,  -970,  -970,
    4640,  -970,  -970,   575,  -970,  -970,  -970,  1050,  3509,   974,
    -970,  -970,   141,  1009,   985,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  1057,  1077,  -970,  -970,  -970,  -970,
    3509,  -970,  1010,   312,  1051,  1008,  -970,  -970,  4393,  2149,
     904,   904,   103,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    1006,  -970,  -970,  1011,   141,   141,  1036,  1012,  1014,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  1044,  -970,
     374,  1062,  -970,  3509,  -970,  -970,  1025,  -970,  -970,  -970,
     374,   855,  1021,  -970,  1052,  1024,  -970,  -970,  1031,  -970,
    3509,   374,  -970,  -970,   857,  -970,  -970,  -970
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -970,  -970,  -970,  -970,  -970,   177,  1118,   501,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,   146,  -970,   -27,   509,  -425,
     673,  -970,  -970,  -970,  -114,  1117,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,  -380,   708,  -970,  -970,   255,
     482,  -394,  -658,    -2,     0,    22,    44,    74,    76,    17,
      19,  -369,  -342,  -602,  -575,  -337,  -307,  -568,  -557,  -518,
    -488,   710,   713,  -970,  -259,  -970,  -487,  -240,   945,  1007,
    1157,  1182,  -970,  -642,  -169,   543,  -970,   728,  -970,   409,
      -5,    37,  -970,   518,  -970,  1042,   272,  -970,  -827,  -970,
     155,  -970,  -810,  -970,  -970,   231,   235,  -970,  -970,  -970,
    -970,  -970,  -970,  -170,   534,   266,   132,   -50,    18,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,   536,
    -122,  -970,  -392,  -970,  -970,   188,   184,   697,   552,   -47,
    -970,  -970,  -531,  -374,  -504,  -574,  -970,   200,  -970,  -970,
    -970,  -970,  -970,  -970,  -322,  -618,  -970,  -970,   757,  -641,
    -970,  -637,  -970,  -970,  -527,   423,  -862,  -935,  -305,  -278,
    -921,  -970,  -142,    39,  -936,  -969,  -970,  -970,  -970,  -970,
    -952,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,  -970,
    -970,  -970,  -970,  -970,  -970,  -970,  -970,   150,  -273,  -970,
    -970,  -970,  -970,    30,  -970,    33,  -970,  -180,   -19,   -81,
     506,  -970,  -736,  -970,  -970,  -970,  -970,  -781,  -970,   460,
    -411,  -970,  -970,  -970,  -970,    13,  -970,   946,  -970,  -970,
    1081,   700,  -970,  -970,  -970,  -970,  -970,  -970,  -970,   -32,
      12,  -970,  -970,  -970,  -970,  -970,  -408,  -970,  -970,  -970,
     210,  -970,  -970,  -970,  -970,  -970,   392,   430,  -970,  -970,
     583,   584,  -264,  -970,  -970,  -970,  -376,  -282,  -970,  -970,
     557,   698,  -970,  -970,  -970,  -970,   692,  -970,  -970,  -970
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -630
static const short int yytable[] =
{
      90,    61,    61,   770,    37,    37,    38,    38,   198,    84,
      85,    86,   424,   285,   584,   288,   409,   333,   531,    71,
      71,    83,   698,    43,    43,    44,    44,   581,    39,    39,
     771,   694,   114,   443,   123,   410,   132,   774,   141,    99,
     150,   418,   159,   582,   168,   463,   177,   572,   775,   585,
      40,    40,   839,   591,   110,   812,   813,   814,   815,   816,
     836,   695,   696,   462,   629,   948,    61,   294,   207,    37,
     617,    38,   909,  1050,   573,   547,   806,   239,   599,   574,
      41,    41,    42,    42,    71,   851,   852,   776,    43,  -122,
      44,   565,   550,    39,   490,   495,   991,   756,   186,    61,
     794,    61,   343,   344,   262,   395,   263,  -461,  -462,   575,
    -463,  -464,    99,   350,  1035,    40,  1077,   777,    87,   551,
     673,   186,   186,   186,   362,  1025,  1099,  1115,   264,    89,
     795,    10,    11,    12,   129,    -2,   698,    91,    92,  1096,
      14,    15,    16,   401,  1101,    41,   805,    42,  1119,     4,
     265,   186,   332,   802,    -3,  1043,    69,    69,    19,   396,
     186,    91,    92,    21,    91,    92,  1121,    94,   558,   186,
      96,    97,   338,   185,   100,    94,   103,  1046,   186,  1153,
    1137,  -122,  -565,   354,    80,    19,   443,   186,   572,   690,
    1132,   925,   694,  -565,  -441,  -442,   186,  -443,  -444,   363,
    1036,   104,   289,  1148,   357,   186,   629,  1103,    93,   193,
     188,   190,   192,  -262,   186,   573,    99,   953,  1159,   955,
     574,    69,   695,   696,   669,    61,  1152,   187,   633,   875,
    1112,   779,    93,   637,  1072,    93,   768,   658,   661,   664,
     667,   114,   105,   123,   770,   132,   -34,   141,   194,   210,
     575,   189,   197,    21,   191,   997,    19,   114,   199,   123,
     200,    91,    92,   769,   659,   662,   665,   668,   772,   862,
      61,   771,  1146,   262,    19,   263,    61,   807,   774,   262,
     449,   263,    91,    92,   743,   402,   403,    61,   650,   775,
     434,   290,   435,   273,    91,    92,   106,   264,   773,   201,
     483,   485,   922,   264,   107,   445,   186,   274,   275,   440,
      13,   441,   945,   499,   436,    91,    92,   331,   502,   265,
     503,    91,    92,   454,   592,   265,   594,  1127,   776,   274,
     275,  1090,    93,   977,    19,   978,   437,   105,   941,   941,
     105,   280,    61,    61,   293,   965,   967,   335,   336,   337,
     408,   339,    13,    93,   295,    61,   607,   105,   777,   105,
      61,   979,    61,   526,    19,    93,   438,   297,   439,   349,
     105,   280,   941,   941,   628,   823,    19,   866,    10,    11,
      12,   138,   739,   111,   609,   332,    93,    14,    15,    16,
     202,   942,    93,   120,   281,   694,   334,   274,   275,   943,
     275,   342,   282,    61,  1011,    19,   262,    19,   263,   601,
     203,   933,   345,   106,   568,    21,   569,    19,   740,   934,
     275,   107,   556,   346,   281,   695,   696,   -35,   393,   114,
     264,   123,   282,   132,    21,   141,   402,   403,   806,   572,
      61,   613,   450,   434,   347,   435,   350,   806,   105,   932,
    -606,    61,   265,   208,  -606,   969,  1092,   201,   445,   987,
    -264,   424,   440,   348,   441,   209,   573,   436,   455,   201,
     958,   574,  1129,   463,   961,   617,   617,   768,   270,   643,
     468,   895,   271,   988,   273,    99,   426,  -545,   116,   437,
     570,   462,   571,   973,   202,  1134,  1134,  -545,   274,   275,
     359,   575,   933,   360,   769,   361,   770,   500,   805,   772,
     934,   275,    22,    23,   205,   363,   268,   805,   649,   438,
     125,   439,   692,    61,   430,   554,   262,   616,   263,   425,
     275,   431,  1086,   771,   522,   183,   184,   527,   447,   773,
     774,   537,  -545,   458,    61,   528,  -545,    37,   420,    38,
     264,   775,   421,   529,   275,   568,   732,   569,  1017,   275,
     427,   853,   710,   114,   271,   132,    43,   150,    44,   168,
     655,    39,   265,   796,   656,   363,   364,   798,    10,    11,
      12,   134,   832,  1130,  1131,  1089,   363,    14,    15,    16,
     776,   464,   535,    40,    22,    23,   401,    22,    23,   450,
      61,    81,    82,   760,  1065,   761,  1066,   744,   363,   970,
     608,  1069,   971,    61,   465,    61,    37,   845,    38,    61,
     777,   846,   766,    41,   767,    42,   443,   762,  1060,   473,
    1061,   570,   475,   571,   917,    43,   692,    44,   363,   807,
      39,     9,    10,    11,    12,   111,   921,   476,   807,   763,
     363,    14,    15,    16,   424,   822,   486,   950,   488,   828,
    -263,   421,    40,   927,   951,   928,   363,    18,   271,    19,
      91,    92,    10,    11,   466,   467,  1002,   -85,  1007,   764,
     363,   765,   421,   406,    83,   536,  1009,   395,   417,    21,
     271,   419,    41,  1012,    42,   691,   553,    99,   572,   559,
     879,   563,  1021,   425,   275,    61,    12,   129,    37,   586,
      38,    27,   578,    14,    15,    16,   383,   384,   385,   288,
     786,   787,   788,   710,  -258,   573,   590,    43,   595,    44,
     574,    19,    39,   789,   654,    10,    11,    12,   165,   768,
     596,    93,   450,   603,    14,    15,    16,   605,   556,   381,
     382,   383,   384,   385,    40,   114,   468,   123,   621,   132,
     575,   141,    19,   150,   623,   159,   769,   168,   612,   177,
     624,   772,    10,    11,    12,   174,   274,   275,   626,   426,
     426,    14,    15,    16,    41,   631,    42,   625,   740,   379,
     380,   381,   382,   383,   384,   385,   918,   657,   660,    19,
     666,   773,   811,   363,   487,   635,   568,   920,   569,   691,
     632,   556,   273,  -400,  -400,  -545,   636,  -270,   610,   611,
     549,   653,   275,  1057,   482,  -545,   639,     9,   648,   824,
      12,    13,   644,   583,   406,   564,   640,    14,    15,    16,
     587,    61,   847,   848,   760,   641,   761,   642,   274,   275,
     535,   535,   930,    18,  -272,    19,   593,   880,   881,   411,
     938,   944,   647,   766,   670,   767,  1015,   275,   762,   357,
    -545,   727,   386,   387,  -545,    21,   388,   389,   390,   391,
      61,   729,   570,   434,   571,   435,   996,  1000,   648,  1033,
     763,  1017,   275,   736,   964,   966,  1005,    27,   445,   873,
     363,  1054,   440,   738,   441,   747,   556,   436,    61,    12,
     138,    37,   748,    38,   366,   368,    14,    15,    16,   751,
     764,   753,   765,   627,   588,   589,   710,   781,    61,   437,
      43,   262,    44,   263,    19,    39,  1020,    61,   651,   -33,
     262,   818,   263,   363,  1059,   892,   363,  1157,   363,  1166,
     819,  1040,   579,   580,   820,   264,   825,    40,   835,   438,
    1149,   439,  1063,  1064,   264,   549,   549,   663,   549,   837,
    1156,  1135,  1136,   840,   101,   102,   844,   265,   273,   744,
     864,  1165,   112,   117,   121,   126,   265,    41,   868,    42,
     148,   153,   157,   162,   883,   884,   872,   690,   878,   882,
    1102,    10,    11,    12,   129,   735,    12,   134,   737,   889,
      14,    15,    16,    14,    15,    16,   891,   908,   913,   914,
     919,  1073,   923,   924,   968,   949,  1126,   983,    19,   984,
     402,   403,   993,   985,   994,  1003,  1074,  1034,  1052,   402,
     403,   980,   535,   535,   113,   118,   122,   127,  1068,  1071,
    1075,   996,   149,   154,   158,   163,  1081,   186,   728,   730,
    1082,  1100,   809,    12,   143,   568,  1083,   569,  1154,  1098,
      14,    15,    16,   996,  1108,  1113,   535,   535,  1109,   115,
     119,   124,   128,   133,   137,   142,   146,   151,   155,   160,
     164,   169,   173,   178,   182,  1114,  1124,  1122,  1139,  1125,
     793,  1143,  1145,    61,  1144,  1140,   760,  1150,   761,  1147,
     829,   830,  1132,  1158,   831,  1161,  1151,  1160,  1163,   792,
       5,   783,   566,  1039,   733,   766,   576,   767,   780,   577,
     762,   954,   555,  1164,   752,   989,  1016,  1018,  1162,   990,
    1045,   570,   963,   571,  1062,  1029,  1027,  1167,   750,   863,
     598,  1133,   763,   867,  1141,   552,   869,   870,  1142,   810,
     871,   378,   379,   380,   381,   382,   383,   384,   385,     9,
      10,    11,    12,   116,   614,   843,   833,   834,   206,    14,
      15,    16,   764,   886,   765,   874,   754,   755,   801,   638,
     634,   112,   117,   121,   126,    18,   841,   841,   130,   135,
     139,   144,     0,     0,     0,     0,   166,   171,   175,   180,
       0,     0,   535,   535,     0,     0,     0,    21,     0,     0,
       0,   535,   535,   131,   136,   140,   145,     0,     0,     0,
       0,   167,   172,   176,   181,   910,   911,   912,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     0,  1041,  1123,
       0,     0,  -259,   113,   118,   122,   127,     0,   876,   877,
     931,     0,  1016,  1018,  1018,     0,     0,     0,     0,     0,
     956,     0,     0,     0,   959,     0,   887,   888,   369,   370,
     371,   962,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,     0,   272,     0,     0,
     -29,   -29,   -29,   -29,   -29,     0,    10,    11,    12,   143,
     -29,   -29,   -29,     0,     0,    14,    15,    16,     0,     0,
       0,     0,     0,     0,     0,   273,   -29,     0,  -545,     0,
    1042,     0,     0,  1006,     0,     0,     0,     0,  -545,     0,
       0,     0,     0,  1013,  1014,     0,     0,  1019,   -29,     0,
       0,     0,  1022,  1023,     0,  1024,     0,     0,     0,     0,
       0,   274,   275,     0,     0,     0,  1026,     0,     0,  1028,
     -29,     0,  1030,  1031,     0,  1032,   472,     0,     0,   112,
     117,   121,   126,  -545,     0,     0,     0,  -545,  -265,   -29,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,  -481,  -481,  -481,  -481,  -481,     0,   130,   135,   139,
     144,  -481,  -481,  -481,     0,     0,  1048,  1048,     0,     0,
       0,     0,  1053,     0,   406,     0,   406,  -481,    10,    11,
      12,   170,   131,   136,   140,   145,     0,    14,    15,    16,
       0,   113,   118,   122,   127,     0,     0,     0,     0,  -481,
       0,     0,   411,     0,     0,  -124,  -124,  -124,  -124,  -124,
       0,     0,   479,   481,     0,  -124,  -124,  -124,     0,     0,
       0,  -481,     0,     0,     0,     0,  1078,     0,     0,     0,
     501,  -124,     0,  1087,  1088,     0,   504,   505,     0,     0,
    -121,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,  -124,     0,     0,   525,     0,     0,     0,
    -271,     0,     0,   112,   117,     0,     0,   148,   153,     0,
     663,    10,    11,    12,   179,  -124,     0,   562,     0,     0,
      14,    15,    16,     0,     0,   663,   711,     0,   712,     9,
      10,    11,    12,    13,  -124,     0,     0,   713,     0,    14,
      15,    16,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,    18,     0,     0,     0,   663,
    1048,     0,     0,     0,     0,   113,   118,     0,     0,   149,
     154,     9,    10,    11,    12,    13,     0,    21,     0,     0,
       0,    14,    15,    16,     0,   130,   135,   139,   144,     0,
       0,     0,     0,  -273,     0,     0,   525,    18,     0,    27,
     115,   119,   133,   137,   151,   155,   169,   173,     0,     0,
     131,   136,   140,   145,     0,   645,   646,  -574,     0,    21,
       0,     0,   894,     0,   211,     0,     0,     0,     0,     0,
       0,   212,   213,     0,   214,     0,     0,    10,    11,    12,
     165,    27,   472,     0,     0,     0,    14,    15,    16,     0,
       0,     0,   215,     0,    20,     0,   216,   217,   218,   219,
     220,   221,   222,  -337,    19,   722,   723,     0,   223,     0,
       0,   224,     0,     0,     0,     0,   225,   226,   227,     0,
       0,   228,   229,     0,     0,  -337,   230,   231,     0,     0,
       0,     0,   232,     0,   233,   112,   117,   121,   126,   234,
       0,     0,   749,   148,   153,   157,   162,   235,     0,     0,
     236,   237,     0,     0,   897,     0,     0,   130,   135,   525,
       0,   166,   171,    10,    11,    12,   138,     0,     0,   797,
       0,     0,    14,    15,    16,     0,     0,     0,   525,     0,
       0,     0,   131,   136,     0,     0,   167,   172,     0,     0,
      19,   803,     0,     0,     0,   804,     0,   113,   118,   122,
     127,    10,    11,    12,   174,   149,   154,   158,   163,     0,
      14,    15,    16,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   817,     0,     0,    19,     0,
       0,     0,   115,   119,   124,   128,   133,   137,   142,   146,
     151,   155,   160,   164,   169,   173,   178,   182,     0,     0,
       0,     0,    10,    11,    12,   134,     0,   894,     0,   671,
      92,    14,    15,    16,     0,     0,   212,   213,     0,   214,
      10,    11,    12,   170,    10,    11,    12,   143,   856,    14,
      15,    16,     0,    14,    15,    16,   861,   215,     0,    20,
     865,   216,   217,   218,   219,   220,   221,   222,    10,    11,
      12,   179,     0,   223,     0,     0,   224,    14,    15,    16,
       0,   225,   226,   227,     0,     0,   228,   229,     0,     0,
     895,   230,   896,     0,     0,     0,     0,   232,     0,   233,
      93,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,  -321,   236,   237,   899,     0,   897,
       0,   130,   135,   139,   144,     0,     0,     0,     0,   166,
     171,   175,   180,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,     0,     0,   131,   136,   140,   145,
     854,     0,   211,     0,   167,   172,   176,   181,     0,   212,
     213,     0,   214,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,     0,     0,     0,   856,     0,     0,   957,
     215,     0,    20,   960,   216,   217,   218,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,   525,     0,   224,
       0,     0,     0,     0,   225,   226,   227,   976,     0,   228,
     229,     0,     0,   981,   230,   231,     0,     0,     0,     0,
     232,     0,   233,   899,   992,     0,     0,   234,     0,     0,
       0,     0,  1001,     0,     0,   235,     0,     0,   236,   237,
       0,     0,   855,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   394,     0,  -467,  -467,  -467,  -467,  -467,  -467,
       0,  -467,  -467,     0,  -467,  -467,  -467,  -467,  -467,   899,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,   395,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,     0,     0,     0,     0,     0,  -467,   899,
       0,  -467,   899,  -467,   899,  1047,  -467,  -467,  -467,     0,
       0,  -467,  -467,     0,     0,     0,  -467,  -467,     0,     0,
       0,     0,  -467,     0,  -467,  -467,     0,     0,     0,  -467,
       0,     0,  -467,  -467,     0,     0,  -467,  -467,  -467,   396,
    -467,  -467,     0,     0,  -467,     0,     0,     0,     0,     0,
    1128,     0,  -467,  -467,  -467,  -467,  -467,  -467,  1070,  -467,
    -467,   899,  -467,  -467,  -467,  -467,  -467,     0,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,   395,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,     0,     0,     0,     0,     0,  -467,     0,     0,  -467,
       0,  -467,     0,     0,  -467,  -467,  -467,     0,     0,  -467,
    -467,     0,     0,     0,  -467,  -467,     0,     0,     0,     0,
    -467,     0,  -467,  -467,     0,     0,     0,  -467,     0,     0,
    -467,  -467,     0,     0,  -467,  -467,  -467,   396,  -467,  -467,
       0,   539,  -467,  -481,  -481,  -481,  -481,  -481,  -481,     0,
    -481,  -481,     0,  -481,  -481,  -481,  -481,  -481,     0,  -481,
    -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,
    -481,  -481,  -481,  -481,     0,  -481,  -481,  -481,  -481,  -481,
    -481,  -481,     0,     0,     0,     0,     0,  -481,     0,     0,
    -481,     0,  -481,     0,     0,  -481,  -481,  -481,     0,     0,
    -481,  -481,     0,     0,     0,  -481,  -481,     0,     0,     0,
       0,  -481,     0,  -481,  -481,     0,     0,     0,  -481,     0,
       0,  -481,  -481,     0,     0,  -481,  -481,  -481,     0,  -481,
    -481,     0,   351,  -481,   211,     9,     0,     0,    12,    13,
       0,   212,   213,     0,   214,    14,    15,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,   215,    19,    20,     0,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,   224,     0,    21,     0,     0,   225,   226,   227,     0,
       0,   228,   229,     0,     0,     0,   230,   231,   894,     0,
     671,    92,   232,     0,   233,    27,     0,   212,   213,   234,
     214,     0,     0,     0,     0,     0,     0,   235,     0,     0,
     236,   237,     0,     0,   352,     0,     0,     0,   215,     0,
      20,     0,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,   224,     0,     0,
       0,     0,   225,   226,   227,     0,     0,   228,   229,     0,
       0,   895,   230,   896,   894,     0,   211,     0,   232,     0,
     233,    93,     0,   212,   213,   234,   214,     0,     0,     0,
       0,     0,     0,   235,     0,  -388,   236,   237,     0,     0,
     897,     0,     0,     0,   215,     0,    20,     0,   216,   217,
     218,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,   224,     0,     0,     0,     0,   225,   226,
     227,     0,     0,   228,   229,     0,     0,     0,   230,   231,
       0,     0,     0,     0,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,   236,   237,   428,     0,   897,   -25,   -25,   -25,
     -25,   -25,     0,     0,     0,     0,     0,   -25,   -25,   -25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,   -25,   422,  -545,     0,   -21,   -21,   -21,
     -21,   -21,     0,     0,     0,  -545,     0,   -21,   -21,   -21,
       0,     0,     0,     0,     0,   -25,     0,     0,     0,     0,
       0,     0,   273,   -21,     0,  -545,     0,     0,   274,   275,
       0,     0,     0,     0,     0,  -545,     0,   -25,     0,     0,
       0,   211,     0,     0,     0,   -21,     0,     0,   212,   213,
    -545,   214,     0,     0,  -545,     0,   -25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -21,     0,   215,
       0,    20,     0,   216,   217,   218,   219,   220,   221,   222,
    -545,     0,     0,     0,  -545,   223,   -21,     0,   224,     0,
       0,     0,     0,   225,   226,   560,     0,     0,   228,   229,
       0,     0,     0,   230,   231,   211,     0,     0,     0,   232,
       0,   233,   212,   213,     0,   214,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,   236,   237,     0,
       0,     0,   561,   215,     0,    20,     0,   216,   217,   218,
     219,   220,   221,   222,     0,     0,     0,     0,     0,   223,
       0,     0,   224,     0,     0,     0,     0,   225,   226,   227,
       0,     0,   228,   229,     0,     0,     0,   230,   231,     0,
       0,     0,     0,   232,     0,   233,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,   236,   237,     0,   711,     0,   724,     9,    10,    11,
      12,    13,     0,     0,     0,   713,     0,    14,    15,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,   432,     0,
       0,     9,     0,     0,    12,    13,     0,     0,     0,     0,
       0,    14,    15,    16,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,    19,
      20,     0,     0,     0,     0,     0,     0,    27,     0,     0,
     671,   672,    10,    11,    12,    13,     0,   212,   213,    21,
     214,    14,    15,    16,   673,  -574,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,    18,   215,    19,
      20,    27,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,   224,     0,    21,
       0,     0,   225,   226,   227,     0,     0,   228,   229,     0,
       0,     0,   230,   231,     0,     0,     0,     0,   232,     0,
     233,   685,     0,     0,     0,   234,     0,     0,   686,   687,
       0,     0,   688,   235,   689,     0,   236,   237,   671,    92,
     690,     0,     0,     0,     0,   212,   213,     0,   214,     0,
       0,     0,   673,     0,   674,   675,   676,   677,   678,   679,
     680,   681,   682,   683,   684,     0,   215,     0,    20,     0,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,   224,     0,     0,     0,     0,
     225,   226,   227,     0,     0,   228,   229,     0,     0,     0,
     230,   231,     0,     0,   211,     0,   232,     0,   233,    93,
       0,   212,   213,   234,   214,     0,   686,   687,     0,     0,
     688,   235,   689,     0,   236,   237,     0,     0,   690,     0,
       0,     0,   215,     0,    20,     0,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,   224,     0,     0,     0,     0,   225,   226,   227,     0,
       0,   228,   229,     0,     0,     0,   230,   231,   480,     0,
     211,     0,   232,     0,   233,     0,     0,   212,   213,   234,
     214,     0,     0,     0,     0,     0,     0,   235,     0,     0,
     236,   237,     0,     0,   627,     0,     0,     0,   215,     0,
      20,     0,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,   224,     0,     0,
       0,     0,   225,   226,   227,     0,     0,   228,   229,     0,
       0,     0,   230,   231,     0,     0,     0,     0,   232,     0,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,   236,   237,   211,     9,
      10,    11,    12,    13,     0,   212,   213,     0,   214,    14,
      15,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,   215,    19,    20,     0,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,   224,     0,    21,     0,     0,
     225,   226,   227,     0,     0,   228,   229,     0,     0,     0,
     230,   231,     0,     0,     0,     0,   232,     0,   233,    27,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   235,   211,     9,   236,   237,    12,    13,     0,   212,
     213,     0,   214,    14,    15,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
     215,    19,    20,     0,   216,   217,   218,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,   224,
       0,    21,     0,     0,   225,   226,   227,     0,     0,   228,
     229,     0,     0,     0,   230,   231,     0,     0,   211,   355,
     232,     0,   233,    27,     0,   212,   213,   234,   214,     0,
       0,     0,     0,     0,     0,   235,     0,     0,   236,   237,
       0,     0,     0,     0,     0,     0,   215,     0,    20,     0,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,   224,     0,     0,     0,     0,
     225,   226,   227,     0,     0,   228,   229,     0,     0,     0,
     230,   231,     0,     0,   211,     0,   232,     0,   233,   356,
       0,   212,   213,   234,   214,     0,     0,     0,     0,     0,
       0,   235,     0,     0,   236,   237,     0,     0,     0,     0,
       0,     0,   215,     0,    20,     0,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,   224,     0,     0,     0,     0,   225,   226,   227,     0,
       0,   228,   229,     0,     0,     0,   230,   231,   211,     0,
       0,     0,   232,     0,   233,   212,   213,     0,   214,   234,
       0,     0,     0,     0,     0,     0,     0,   235,   821,     0,
     236,   237,     0,     0,     0,     0,   215,     0,    20,     0,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,   224,     0,     0,     0,     0,
     225,   226,   227,     0,     0,   228,   229,     0,     0,     0,
     230,   231,   211,     0,     0,     0,   232,     0,   233,   212,
     213,     0,   214,   234,     0,     0,     0,     0,     0,     0,
       0,   235,   827,     0,   236,   237,     0,     0,     0,     0,
     215,     0,    20,     0,   216,   217,   218,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,   224,
       0,     0,     0,     0,   225,   226,   227,     0,     0,   228,
     229,     0,     0,     0,   230,   231,   211,     0,     0,     0,
     232,     0,   233,   212,   213,     0,   214,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,   236,   237,
       0,     0,     0,     0,   215,     0,    20,     0,   216,   217,
     218,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,   224,     0,     0,     0,     0,   225,   226,
     227,     0,     0,   228,   229,     0,     0,     0,   365,   231,
     211,     0,     0,     0,   232,     0,   233,   212,   213,     0,
     214,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,   236,   237,     0,     0,     0,     0,   215,     0,
      20,     0,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,   224,     0,     0,
       0,     0,   225,   226,   227,     0,     0,   228,   229,     0,
       0,     0,   367,   231,   782,     0,     0,     0,   232,     0,
     233,   212,   213,     0,   214,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,   236,   237,     0,     0,
       0,     0,   215,     0,    20,     0,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,   224,     0,     0,     0,     0,   225,   226,   227,     0,
       0,   228,   229,     0,     0,     0,   230,   231,     0,     0,
     432,     0,   232,     9,   233,     0,    12,    13,     0,   234,
       0,     0,     0,    14,    15,    16,     0,   235,     0,     0,
     236,   237,     0,     0,     0,     0,     0,   432,     0,    18,
       9,    19,    20,    12,    13,     0,     0,     0,     0,     0,
      14,    15,    16,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,    18,     0,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    27,  -629,  -629,  -629,     8,    21,  -133,
       9,    10,    11,    12,    13,     0,   972,  -629,     0,     0,
      14,    15,    16,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    17,    18,     0,    19,    20,
       0,     0,     9,   433,  -391,    12,    13,   600,     0,     0,
       0,     0,    14,    15,    16,     0,     0,     0,    21,     0,
       0,    22,    23,  -133,     0,     0,     0,     0,    18,     0,
      19,  -133,     0,     0,    24,    25,    26,     0,     0,     0,
      27,     0,     0,     0,    28,    29,    30,     0,     0,     0,
      21,     0,     8,    31,  -133,     9,    10,    11,    12,    13,
       0,     0,     0,     0,     0,    14,    15,    16,     0,     0,
       0,     0,    27,     0,     9,    10,    11,    12,   120,     0,
       0,    18,     0,    19,    14,    15,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    10,    11,    12,   147,
      18,     0,    19,    21,     0,    14,    15,    16,  -133,     0,
       0,     0,     0,     0,     0,     0,  -133,     0,     0,     0,
       0,    18,    21,    19,     0,    27,     0,     0,     0,     9,
      10,    11,    12,   125,     0,     0,     0,     0,    31,    14,
      15,    16,     0,    21,    27,     0,     0,     0,     9,    10,
      11,    12,   156,     0,     0,    18,     0,  -260,    14,    15,
      16,     0,     0,     0,     0,    27,     0,     0,     0,     9,
      10,    11,    12,   152,    18,     0,    19,    21,  -266,    14,
      15,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       9,    10,    11,    12,   161,    18,    21,     0,     0,    27,
      14,    15,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -261,     0,     0,     0,    18,    21,    27,   484,
       0,     0,     9,     0,     0,    12,    13,     0,     0,     0,
       0,  -268,    14,    15,    16,     0,     0,     0,    21,    27,
       0,     0,     0,     9,    10,    11,    12,    13,    18,     0,
      19,   759,  -267,    14,    15,    16,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,     0,     0,     0,    18,
      21,    19,     0,  -269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   299,     0,     0,   300,   301,   302,
       0,    21,    27,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,     0,    27,     0,     0,   298,   299,     0,     0,
     300,   301,   302,     0,     0,   322,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   323,     0,     0,     0,     0,
       0,   298,   299,     0,   324,   300,   301,   302,   322,     0,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
       0,     0,     0,     0,   298,   299,     0,   324,   300,   301,
     302,     0,     0,   489,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,     9,    10,    11,    12,    13,     0,     0,
       0,   947,   324,    14,    15,    16,   494,     0,     0,     0,
       0,     0,     9,    10,    11,    12,   111,     0,     0,    18,
       0,    19,    14,    15,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,    18,     0,
      19,    21,     9,    10,    11,    12,   147,     9,    10,    11,
      12,   120,    14,    15,    16,     0,     0,    14,    15,    16,
      21,     0,     0,    27,     0,     0,     0,     0,    18,     0,
      19,     0,     0,    18,     0,    19,     9,    10,    11,    12,
     156,     0,    27,     0,     0,     0,    14,    15,    16,     0,
      21,     0,     0,     0,     0,    21,     0,     9,    10,    11,
      12,    13,    18,     0,    19,     0,     0,    14,    15,    16,
       0,     0,    27,     0,     0,     0,     0,    27,     9,    10,
      11,    12,   116,    18,    21,    19,     0,     0,    14,    15,
      16,     9,    10,    11,    12,   152,     0,     0,     0,     0,
       0,    14,    15,    16,    18,    21,    27,     0,     9,    10,
      11,    12,   125,     0,     0,     0,     0,    18,    14,    15,
      16,     9,    10,    11,    12,   161,    21,    27,     0,     0,
       0,    14,    15,    16,    18,     0,     0,     0,     0,    21,
       9,     0,     0,    12,   111,     0,     0,    18,    27,     0,
      14,    15,    16,     0,     0,     0,    21,     0,     0,     0,
       0,    27,     0,     0,     0,     0,    18,     0,    19,    21,
       9,     0,     0,    12,   120,     0,     0,     0,    27,     0,
      14,    15,    16,     9,     0,     0,    12,    13,    21,     0,
       0,    27,     0,    14,    15,    16,    18,     0,    19,     0,
       0,     0,     9,     0,     0,    12,   116,     0,     0,    18,
      27,    19,    14,    15,    16,     0,     0,     0,    21,     0,
       0,     0,     9,     0,     0,    12,   125,     0,    18,     0,
       0,    21,    14,    15,    16,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      21,     0,   371,    27,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,     0,     0,
      21,     0,    27,   369,   370,   371,     0,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,     0,    27,   369,   370,   371,     0,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   369,   370,   371,   725,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
       0,     0,   369,   370,   371,   849,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   369,   370,   371,   850,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   369,   370,   371,  1091,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   369,   370,
     371,  1038,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,     0,   369,   370,   371,
     620,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,     0,     0,   915,    19,     0,
       0,     0,     0,     0,     0,     0,     0,   622,   369,   370,
     371,     0,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   890,   369,   370,   371,
     916,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   369,   370,   371,  1055,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   369,   370,   371,     0,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385
};

static const short int yycheck[] =
{
      19,     6,     7,   605,     6,     7,     6,     7,    89,    14,
      15,    16,   285,   183,   425,   184,   275,   197,   392,     6,
       7,     9,   549,     6,     7,     6,     7,   421,     6,     7,
     605,   549,    37,   292,    39,   275,    41,   605,    43,    27,
      45,   281,    47,   423,    49,   327,    51,   416,   605,   429,
       6,     7,   710,   445,    36,   673,   674,   675,   676,   677,
     697,   549,   549,   327,   489,   846,    71,   189,   100,    71,
     478,    71,   808,   994,   416,   397,   650,   104,   454,   416,
       6,     7,     6,     7,    71,   726,   728,   605,    71,     1,
      71,   413,   397,    71,   358,   359,   906,     1,    61,   104,
     618,   106,   216,   217,   106,    34,   106,     1,     1,   416,
       1,     1,   100,   227,     1,    71,  1051,   605,     1,   397,
      17,    84,    85,    86,   238,   952,  1078,  1096,   106,     1,
     618,     5,     6,     7,     8,     0,   663,     3,     4,  1075,
      14,    15,    16,   257,  1080,    71,   650,    71,  1100,     0,
     106,   114,    11,    45,     0,   982,     6,     7,    32,    88,
     123,     3,     4,    52,     3,     4,  1101,    21,   408,   132,
      24,    25,   204,    87,    28,    29,    30,   987,   141,  1148,
    1115,    93,    65,   230,     7,    32,   445,   150,   557,    93,
      87,   833,   710,    65,    88,    88,   159,    88,    88,    91,
      87,    65,   184,  1139,   231,   168,   631,    66,    74,    87,
      64,    65,    66,    87,   177,   557,   204,   858,  1153,   860,
     557,    71,   710,   710,   546,   230,  1147,    93,   492,   756,
    1092,   607,    74,   497,  1044,    74,   605,   542,   543,   544,
     545,   246,     3,   248,   846,   250,    45,   252,    71,   103,
     557,    93,    65,    52,    93,   913,    32,   262,    65,   264,
      52,     3,     4,   605,   542,   543,   544,   545,   605,    45,
     275,   846,  1134,   275,    32,   275,   281,   651,   846,   281,
       1,   281,     3,     4,    45,   267,   268,   292,   528,   846,
     292,    72,   292,    29,     3,     4,    57,   275,   605,    91,
     347,   348,   829,   281,    65,   292,   269,    65,    66,   292,
       8,   292,   843,   360,   292,     3,     4,    87,   365,   275,
     367,     3,     4,    65,   446,   281,   448,  1108,   846,    65,
      66,  1067,    74,    64,    32,    66,   292,     3,   842,   843,
       3,     4,   347,   348,    93,   876,   877,   201,   202,   203,
       6,   205,     8,    74,    93,   360,    65,     3,   846,     3,
     365,    92,   367,   390,    32,    74,   292,    93,   292,   223,
       3,     4,   876,   877,   488,    57,    32,    45,     5,     6,
       7,     8,    45,     8,   465,    11,    74,    14,    15,    16,
      45,    57,    74,     8,    57,   913,    65,    65,    66,    65,
      66,    87,    65,   408,   931,    32,   408,    32,   408,   456,
      65,    57,    65,    57,   416,    52,   416,    32,   588,    65,
      66,    65,   404,    65,    57,   913,   913,    45,    92,   434,
     408,   436,    65,   438,    52,   440,   418,   419,  1012,   808,
     445,   473,   296,   445,    65,   445,   560,  1021,     3,     4,
      87,   456,   408,    87,    91,   880,  1074,    91,   445,    42,
      87,   734,   445,    65,   445,    87,   808,   445,   322,    91,
     864,   808,  1109,   755,   868,   883,   884,   846,    87,   506,
     334,    64,    91,    66,    29,   473,   286,    32,     8,   445,
     416,   755,   416,   885,    45,  1113,  1114,    42,    65,    66,
      65,   808,    57,    65,   846,    65,  1108,   361,  1012,   846,
      65,    66,    55,    56,    65,    91,   107,  1021,    94,   445,
       8,   445,   549,   528,    65,    92,   528,    70,   528,    65,
      66,    88,  1059,  1108,   388,    53,    54,   391,    88,   846,
    1108,   395,    87,    87,   549,    57,    91,   549,    87,   549,
     528,  1108,    91,    65,    66,   557,    92,   557,    65,    66,
      87,   730,   549,   568,    91,   570,   549,   572,   549,   574,
      87,   549,   528,   620,    91,    91,    92,   624,     5,     6,
       7,     8,    87,  1110,  1111,    92,    91,    14,    15,    16,
    1108,    45,   392,   549,    55,    56,   710,    55,    56,   453,
     605,    87,    88,   605,  1015,   605,  1017,   589,    91,    70,
     464,    94,    70,   618,    92,   620,   618,    87,   618,   624,
    1108,    91,   605,   549,   605,   549,   885,   605,  1008,    92,
    1010,   557,    92,   557,    87,   618,   663,   618,    91,  1013,
     618,     4,     5,     6,     7,     8,    87,    93,  1022,   605,
      91,    14,    15,    16,   927,   682,    92,    87,    92,   686,
      87,    91,   618,   833,    87,   834,    91,    30,    91,    32,
       3,     4,     5,     6,     7,     8,    87,    45,    87,   605,
      91,   605,    91,   274,   672,    92,    87,    34,   279,    52,
      91,   282,   618,   933,   618,   549,    92,   685,  1067,     6,
     781,    87,   942,    65,    66,   710,     7,     8,   710,    74,
     710,    74,    42,    14,    15,    16,    57,    58,    59,   888,
      75,    76,    77,   710,    87,  1067,    87,   710,    42,   710,
    1067,    32,   710,    88,   534,     5,     6,     7,     8,  1108,
      91,    74,   596,    92,    14,    15,    16,    91,   730,    55,
      56,    57,    58,    59,   710,   760,   610,   762,    92,   764,
    1067,   766,    32,   768,    92,   770,  1108,   772,    65,   774,
      91,  1108,     5,     6,     7,     8,    65,    66,    91,   579,
     580,    14,    15,    16,   710,    45,   710,    92,   958,    53,
      54,    55,    56,    57,    58,    59,   823,   542,   543,    32,
     545,  1108,   656,    91,    92,    45,   808,   826,   808,   663,
      94,   793,    29,    87,    88,    32,    92,    87,    91,    92,
     397,    65,    66,  1003,     1,    42,    92,     4,    91,   683,
       7,     8,    45,   424,   425,   412,    92,    14,    15,    16,
     431,   846,    91,    92,   846,    92,   846,    92,    65,    66,
     650,   651,   834,    30,    87,    32,   447,    91,    92,     1,
     842,   843,    92,   846,    88,   846,    65,    66,   846,   896,
      87,    87,    60,    61,    91,    52,    64,    65,    66,    67,
     885,    87,   808,   885,   808,   885,   913,   914,    91,    92,
     846,    65,    66,    42,   876,   877,   923,    74,   885,   753,
      91,    92,   885,    92,   885,    88,   888,   885,   913,     7,
       8,   913,    88,   913,   241,   242,    14,    15,    16,    88,
     846,    92,   846,    93,   442,   443,   913,    92,   933,   885,
     913,   933,   913,   933,    32,   913,   941,   942,   529,    45,
     942,    45,   942,    91,    92,   799,    91,    92,    91,    92,
      87,   978,   418,   419,    87,   933,     8,   913,    87,   885,
    1140,   885,  1012,  1013,   942,   542,   543,   544,   545,    84,
    1150,  1113,  1114,    92,    28,    29,    92,   933,    29,   961,
      91,  1161,    37,    38,    39,    40,   942,   913,    91,   913,
      45,    46,    47,    48,   784,   785,    88,    93,    92,    92,
    1081,     5,     6,     7,     8,   582,     7,     8,   585,    92,
      14,    15,    16,    14,    15,    16,    92,    92,    65,    65,
      87,  1048,    65,    45,   878,     3,  1107,    45,    32,    88,
    1012,  1013,    65,    91,    65,    65,    92,    87,    87,  1021,
    1022,   895,   842,   843,    37,    38,    39,    40,    88,    88,
      92,  1078,    45,    46,    47,    48,    92,  1020,   576,   577,
      45,    87,   653,     7,     8,  1067,    83,  1067,  1149,    19,
      14,    15,    16,  1100,    65,    18,   876,   877,    93,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    18,    45,    87,    92,    91,
     618,    65,    88,  1108,    92,    94,  1108,    45,  1108,    65,
     687,   688,    87,    92,   691,    91,  1143,    65,    87,   618,
       2,   612,   414,   977,   581,  1108,   416,  1108,   610,   416,
    1108,   859,   404,  1160,   598,   904,   936,   937,  1157,   904,
     985,  1067,   876,  1067,  1012,   961,   958,  1166,   596,   740,
     453,  1112,  1108,   744,  1124,   398,   747,   748,  1125,   653,
     751,    52,    53,    54,    55,    56,    57,    58,    59,     4,
       5,     6,     7,     8,   474,   715,   694,   695,    97,    14,
      15,    16,  1108,   791,  1108,   755,   603,   603,   631,   497,
     492,   246,   247,   248,   249,    30,   714,   715,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      -1,    -1,  1012,  1013,    -1,    -1,    -1,    52,    -1,    -1,
      -1,  1021,  1022,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,   812,   813,   814,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,  1103,
      -1,    -1,    87,   246,   247,   248,   249,    -1,   776,   777,
     837,    -1,  1062,  1063,  1064,    -1,    -1,    -1,    -1,    -1,
     861,    -1,    -1,    -1,   865,    -1,   794,   795,    42,    43,
      44,   872,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,     1,    -1,    -1,
       4,     5,     6,     7,     8,    -1,     5,     6,     7,     8,
      14,    15,    16,    -1,    -1,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    32,    -1,
      94,    -1,    -1,   924,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   934,   935,    -1,    -1,   938,    52,    -1,
      -1,    -1,   943,   944,    -1,   946,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,   957,    -1,    -1,   960,
      74,    -1,   963,   964,    -1,   966,   334,    -1,    -1,   434,
     435,   436,   437,    87,    -1,    -1,    -1,    91,    87,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,     5,     6,     7,     8,    -1,   250,   251,   252,
     253,    14,    15,    16,    -1,    -1,   993,   994,    -1,    -1,
      -1,    -1,   999,    -1,  1015,    -1,  1017,    30,     5,     6,
       7,     8,   250,   251,   252,   253,    -1,    14,    15,    16,
      -1,   434,   435,   436,   437,    -1,    -1,    -1,    -1,    52,
      -1,    -1,     1,    -1,    -1,     4,     5,     6,     7,     8,
      -1,    -1,   345,   346,    -1,    14,    15,    16,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,  1053,    -1,    -1,    -1,
     363,    30,    -1,  1060,  1061,    -1,   369,   370,    -1,    -1,
      93,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,    52,    -1,    -1,   389,    -1,    -1,    -1,
      87,    -1,    -1,   568,   569,    -1,    -1,   572,   573,    -1,
    1097,     5,     6,     7,     8,    74,    -1,   410,    -1,    -1,
      14,    15,    16,    -1,    -1,  1112,     1,    -1,     3,     4,
       5,     6,     7,     8,    93,    -1,    -1,    12,    -1,    14,
      15,    16,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    30,    -1,    -1,    -1,  1146,
    1147,    -1,    -1,    -1,    -1,   568,   569,    -1,    -1,   572,
     573,     4,     5,     6,     7,     8,    -1,    52,    -1,    -1,
      -1,    14,    15,    16,    -1,   438,   439,   440,   441,    -1,
      -1,    -1,    -1,    87,    -1,    -1,   489,    30,    -1,    74,
     568,   569,   570,   571,   572,   573,   574,   575,    -1,    -1,
     438,   439,   440,   441,    -1,   508,   509,    92,    -1,    52,
      -1,    -1,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    11,    -1,    13,    -1,    -1,     5,     6,     7,
       8,    74,   610,    -1,    -1,    -1,    14,    15,    16,    -1,
      -1,    -1,    31,    -1,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    32,   558,   559,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    73,   760,   761,   762,   763,    78,
      -1,    -1,   595,   768,   769,   770,   771,    86,    -1,    -1,
      89,    90,    -1,    -1,    93,    -1,    -1,   570,   571,   612,
      -1,   574,   575,     5,     6,     7,     8,    -1,    -1,   622,
      -1,    -1,    14,    15,    16,    -1,    -1,    -1,   631,    -1,
      -1,    -1,   570,   571,    -1,    -1,   574,   575,    -1,    -1,
      32,   644,    -1,    -1,    -1,   648,    -1,   760,   761,   762,
     763,     5,     6,     7,     8,   768,   769,   770,   771,    -1,
      14,    15,    16,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,   678,    -1,    -1,    32,    -1,
      -1,    -1,   760,   761,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,   772,   773,   774,   775,    -1,    -1,
      -1,    -1,     5,     6,     7,     8,    -1,     1,    -1,     3,
       4,    14,    15,    16,    -1,    -1,    10,    11,    -1,    13,
       5,     6,     7,     8,     5,     6,     7,     8,   731,    14,
      15,    16,    -1,    14,    15,    16,   739,    31,    -1,    33,
     743,    35,    36,    37,    38,    39,    40,    41,     5,     6,
       7,     8,    -1,    47,    -1,    -1,    50,    14,    15,    16,
      -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    88,    89,    90,   800,    -1,    93,
      -1,   764,   765,   766,   767,    -1,    -1,    -1,    -1,   772,
     773,   774,   775,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    -1,   764,   765,   766,   767,
       1,    -1,     3,    -1,   772,   773,   774,   775,    -1,    10,
      11,    -1,    13,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    -1,    -1,   859,    -1,    -1,   862,
      31,    -1,    33,   866,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    47,   880,    -1,    50,
      -1,    -1,    -1,    -1,    55,    56,    57,   890,    -1,    60,
      61,    -1,    -1,   896,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    73,   906,   907,    -1,    -1,    78,    -1,    -1,
      -1,    -1,   915,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    -1,    93,    51,    52,    53,    54,    55,    56,    57,
      58,    59,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    11,    -1,    13,    14,    15,    16,    17,   952,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,   982,
      -1,    50,   985,    52,   987,   988,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,  1041,    10,
      11,  1044,    13,    14,    15,    16,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    -1,    -1,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,     1,    93,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    -1,    13,    14,    15,    16,    17,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    52,    -1,    -1,    55,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    71,    -1,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    -1,    -1,    85,    86,    87,    -1,    89,
      90,    -1,     1,    93,     3,     4,    -1,    -1,     7,     8,
      -1,    10,    11,    -1,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    52,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,     1,    -1,
       3,     4,    71,    -1,    73,    74,    -1,    10,    11,    78,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    31,    -1,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    65,    66,     1,    -1,     3,    -1,    71,    -1,
      73,    74,    -1,    10,    11,    78,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    31,    -1,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,     1,    -1,    93,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,     1,    32,    -1,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    42,    -1,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    -1,    32,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    74,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    52,    -1,    -1,    10,    11,
      87,    13,    -1,    -1,    91,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      87,    -1,    -1,    -1,    91,    47,    93,    -1,    50,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    -1,    60,    61,
      -1,    -1,    -1,    65,    66,     3,    -1,    -1,    -1,    71,
      -1,    73,    10,    11,    -1,    13,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    94,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    -1,    60,    61,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,     1,    -1,    94,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,    -1,    -1,     7,     8,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    10,    11,    52,
      13,    14,    15,    16,    17,    92,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    74,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,
      -1,    -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      -1,    -1,    85,    86,    87,    -1,    89,    90,     3,     4,
      93,    -1,    -1,    -1,    -1,    10,    11,    -1,    13,    -1,
      -1,    -1,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    -1,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,     3,    -1,    71,    -1,    73,    74,
      -1,    10,    11,    78,    13,    -1,    81,    82,    -1,    -1,
      85,    86,    87,    -1,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    31,    -1,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,     1,    -1,
       3,    -1,    71,    -1,    73,    -1,    -1,    10,    11,    78,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    31,    -1,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    89,    90,     3,     4,
       5,     6,     7,     8,    -1,    10,    11,    -1,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,
      55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,     3,     4,    89,    90,     7,     8,    -1,    10,
      11,    -1,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    -1,    -1,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,     3,     4,
      71,    -1,    73,    74,    -1,    10,    11,    78,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,     3,    -1,    71,    -1,    73,    74,
      -1,    10,    11,    78,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,     3,    -1,
      -1,    -1,    71,    -1,    73,    10,    11,    -1,    13,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      89,    90,    -1,    -1,    -1,    -1,    31,    -1,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    66,     3,    -1,    -1,    -1,    71,    -1,    73,    10,
      11,    -1,    13,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    89,    90,    -1,    -1,    -1,    -1,
      31,    -1,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    66,     3,    -1,    -1,    -1,
      71,    -1,    73,    10,    11,    -1,    13,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    66,
       3,    -1,    -1,    -1,    71,    -1,    73,    10,    11,    -1,
      13,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    31,    -1,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    66,     3,    -1,    -1,    -1,    71,    -1,
      73,    10,    11,    -1,    13,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    31,    -1,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
       1,    -1,    71,     4,    73,    -1,     7,     8,    -1,    78,
      -1,    -1,    -1,    14,    15,    16,    -1,    86,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    -1,     1,    -1,    30,
       4,    32,    33,     7,     8,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    30,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    75,    76,    77,     1,    52,     3,
       4,     5,     6,     7,     8,    -1,    87,    88,    -1,    -1,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    29,    30,    -1,    32,    33,
      -1,    -1,     4,    87,    88,     7,     8,     9,    -1,    -1,
      -1,    -1,    14,    15,    16,    -1,    -1,    -1,    52,    -1,
      -1,    55,    56,    57,    -1,    -1,    -1,    -1,    30,    -1,
      32,    65,    -1,    -1,    68,    69,    70,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,    -1,
      52,    -1,     1,    87,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    -1,    -1,
      -1,    -1,    74,    -1,     4,     5,     6,     7,     8,    -1,
      -1,    30,    -1,    32,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,     8,
      30,    -1,    32,    52,    -1,    14,    15,    16,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    30,    52,    32,    -1,    74,    -1,    -1,    -1,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    87,    14,
      15,    16,    -1,    52,    74,    -1,    -1,    -1,     4,     5,
       6,     7,     8,    -1,    -1,    30,    -1,    87,    14,    15,
      16,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     4,
       5,     6,     7,     8,    30,    -1,    32,    52,    87,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       4,     5,     6,     7,     8,    30,    52,    -1,    -1,    74,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    30,    52,    74,     1,
      -1,    -1,     4,    -1,    -1,     7,     8,    -1,    -1,    -1,
      -1,    87,    14,    15,    16,    -1,    -1,    -1,    52,    74,
      -1,    -1,    -1,     4,     5,     6,     7,     8,    30,    -1,
      32,    12,    87,    14,    15,    16,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      52,    32,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,     7,     8,     9,
      -1,    52,    74,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    74,    -1,    -1,     3,     4,    -1,    -1,
       7,     8,     9,    -1,    -1,    45,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    65,    -1,    -1,    -1,    -1,
      -1,     3,     4,    -1,    74,     7,     8,     9,    45,    -1,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,     3,     4,    -1,    74,     7,     8,
       9,    -1,    -1,    45,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    74,    14,    15,    16,    45,    -1,    -1,    -1,
      -1,    -1,     4,     5,     6,     7,     8,    -1,    -1,    30,
      -1,    32,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    30,    -1,
      32,    52,     4,     5,     6,     7,     8,     4,     5,     6,
       7,     8,    14,    15,    16,    -1,    -1,    14,    15,    16,
      52,    -1,    -1,    74,    -1,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    30,    -1,    32,     4,     5,     6,     7,
       8,    -1,    74,    -1,    -1,    -1,    14,    15,    16,    -1,
      52,    -1,    -1,    -1,    -1,    52,    -1,     4,     5,     6,
       7,     8,    30,    -1,    32,    -1,    -1,    14,    15,    16,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    74,     4,     5,
       6,     7,     8,    30,    52,    32,    -1,    -1,    14,    15,
      16,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    30,    52,    74,    -1,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    30,    14,    15,
      16,     4,     5,     6,     7,     8,    52,    74,    -1,    -1,
      -1,    14,    15,    16,    30,    -1,    -1,    -1,    -1,    52,
       4,    -1,    -1,     7,     8,    -1,    -1,    30,    74,    -1,
      14,    15,    16,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    30,    -1,    32,    52,
       4,    -1,    -1,     7,     8,    -1,    -1,    -1,    74,    -1,
      14,    15,    16,     4,    -1,    -1,     7,     8,    52,    -1,
      -1,    74,    -1,    14,    15,    16,    30,    -1,    32,    -1,
      -1,    -1,     4,    -1,    -1,     7,     8,    -1,    -1,    30,
      74,    32,    14,    15,    16,    -1,    -1,    -1,    52,    -1,
      -1,    -1,     4,    -1,    -1,     7,     8,    -1,    30,    -1,
      -1,    52,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      52,    -1,    44,    74,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    -1,
      52,    -1,    74,    42,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    74,    42,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    94,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    -1,    42,    43,    44,    94,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    94,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      -1,    42,    43,    44,    94,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    92,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    42,    43,    44,
      91,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    -1,    12,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    42,    43,
      44,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    91,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    42,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,    96,    97,   101,     0,   101,    98,    99,     1,     4,
       5,     6,     7,     8,    14,    15,    16,    29,    30,    32,
      33,    52,    55,    56,    68,    69,    70,    74,    78,    79,
      80,    87,   100,   102,   103,   118,   135,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   161,   163,   164,   165,   166,
     167,   175,   176,   180,   204,   205,   206,   207,   212,   282,
     284,   310,   311,   313,   314,   317,   322,   325,   331,   332,
     100,    87,    88,   325,   175,   175,   175,     1,   293,     1,
     293,     3,     4,    74,   110,   312,   110,   110,   324,   325,
     110,   312,   312,   110,    65,     3,    57,    65,   169,   172,
     203,     8,   163,   164,   175,   180,     8,   163,   164,   180,
       8,   163,   164,   175,   180,     8,   163,   164,   180,     8,
     165,   166,   175,   180,     8,   165,   166,   180,     8,   165,
     166,   175,   180,     8,   165,   166,   180,     8,   163,   164,
     175,   180,     8,   163,   164,   180,     8,   163,   164,   175,
     180,     8,   163,   164,   180,     8,   165,   166,   175,   180,
       8,   165,   166,   180,     8,   165,   166,   175,   180,     8,
     165,   166,   180,   135,   135,    87,   176,    93,   110,    93,
     110,    93,   110,    87,   100,   333,   294,    65,   294,    65,
      52,    91,    45,    65,   315,    65,   315,   324,    87,    87,
     110,     3,    10,    11,    13,    31,    35,    36,    37,    38,
      39,    40,    41,    47,    50,    55,    56,    57,    60,    61,
      65,    66,    71,    73,    78,    86,    89,    90,   111,   112,
     115,   116,   117,   119,   120,   126,   138,   139,   140,   141,
     142,   143,   144,   145,   160,   224,   248,   310,   358,   362,
     363,   364,   138,   139,   140,   141,   159,   162,   174,   175,
      87,    91,     1,    29,    65,    66,   108,   232,   282,   283,
       4,    57,    65,   168,   170,   198,   199,   203,   169,   203,
      72,   215,   216,    93,   215,    93,   211,    93,     3,     4,
       7,     8,     9,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    45,    65,    74,   341,   345,   346,   347,   348,
     352,    87,    11,   292,    65,   110,   110,   110,   324,   110,
     320,   323,    87,   119,   119,    65,    65,    65,    65,   110,
     119,     1,    93,   112,   224,     4,    74,   112,   357,    65,
      65,    65,   119,    91,    92,    65,   115,    65,   115,    42,
      43,    44,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    64,    65,
      66,    67,   225,    92,     1,    34,    88,   241,   242,   243,
     246,   119,   203,   203,   136,   174,   174,   305,     6,   159,
     162,     1,   130,   131,   132,   239,   250,   174,   162,   174,
      87,    91,     1,   104,   283,    65,   232,    87,     1,   106,
      65,    88,     1,    87,   138,   139,   140,   141,   142,   143,
     144,   145,   158,   159,   217,   310,   208,    88,   209,     1,
     110,   222,   223,   210,    65,   110,   350,   351,    87,   330,
     342,   343,   347,   352,    45,    92,     7,     8,   110,   177,
     178,   179,   180,    92,   318,    92,    93,   316,   335,   120,
       1,   120,     1,   224,     1,   224,    92,    92,    92,    45,
     347,   353,   354,   356,    45,   347,   359,   360,   361,   224,
     110,   120,   224,   224,   120,   120,   123,   125,   122,   121,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   110,   113,   114,   120,   112,   110,    57,    65,
     226,   228,   229,   230,   231,   232,    92,   110,   309,     1,
     134,   233,   234,   235,   236,   237,   238,   239,   247,   250,
     253,   254,   243,    92,    92,   172,   203,   306,   162,     6,
      57,    94,   120,    87,   250,   239,   131,   133,   138,   139,
     142,   143,   146,   147,   150,   151,   156,   157,    42,   199,
     199,   136,   130,   174,   305,   130,    74,   174,   135,   135,
      87,   217,   215,   174,   215,    42,    91,   214,   222,   351,
       9,   224,   349,    92,   334,    91,   344,    65,   110,   294,
      91,    92,    65,   324,   316,   326,    70,   331,   336,   338,
      91,    92,    91,    92,    91,    92,    91,    93,   119,   114,
     355,    45,    94,   347,   356,    45,    92,   347,   361,    92,
      92,    92,    92,   112,    45,   120,   120,    92,    91,    94,
     162,   174,   295,    65,   232,    87,    91,   134,   253,   254,
     134,   253,   254,   250,   253,   254,   134,   253,   254,   239,
      88,     3,     4,    17,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    74,    81,    82,    85,    87,
      93,   110,   112,   137,   154,   155,   161,   245,   249,   258,
     261,   262,   269,   270,   272,   276,   279,   280,   281,   285,
     310,     1,     3,    12,   156,   157,   297,   300,   301,   303,
     307,   308,   120,   120,    94,    94,   109,    87,   135,    87,
     135,   173,    92,   170,   198,   250,    42,   250,    92,    45,
     198,   218,   220,    45,   203,   219,   221,    88,    88,   120,
     223,    88,   214,    92,   345,   346,     1,   244,   249,    12,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   302,   351,
     178,    92,     3,   113,   321,   319,    75,    76,    77,    88,
     327,   339,   102,   135,   154,   155,   224,   120,   224,   128,
     127,   355,    45,   120,   120,   229,   230,   228,   296,   174,
     295,   110,   240,   240,   240,   240,   240,   120,    45,    87,
      87,    87,   112,    57,   110,     8,   287,    87,   112,   250,
     250,   250,    87,   135,   135,    87,   246,    84,   278,   137,
      92,   135,   304,   304,    92,    87,    91,    91,    92,    94,
      94,   244,   168,   169,     1,    93,   120,   181,   105,   171,
     107,   120,    45,   174,    91,   120,    45,   174,    91,   174,
     174,   174,    88,   110,   342,   249,   135,   135,    92,   294,
      91,    92,    92,   335,   335,   328,   341,   135,   135,    92,
      91,    92,   110,   129,     1,    64,    66,    93,   110,   120,
     183,   184,   185,   187,   189,   190,   191,   124,    92,   297,
     250,   250,   250,    65,    65,    12,    45,    87,   112,    87,
     293,    87,   249,    65,    45,   168,   192,   198,   169,   195,
     203,   250,     4,    57,    65,   200,   201,   202,   203,   227,
     228,   229,    57,    65,   203,   227,   298,    12,   302,     3,
      87,    87,   182,   244,   181,   244,   174,   120,   136,   174,
     120,   136,   174,   200,   203,   227,   203,   227,   110,   114,
      70,    70,    87,   217,   329,   340,   120,    64,    66,    92,
     110,   120,   188,    45,    88,    91,   213,    42,    66,   190,
     191,   187,   120,    65,    65,   259,   112,   137,   265,   266,
     112,   120,    87,    65,   277,   112,   174,    87,   193,    87,
     196,   249,   162,   174,   174,    65,   232,    65,   232,   174,
     175,   162,   174,   174,   174,   183,   174,   220,   174,   221,
     174,   174,   174,    92,    87,     1,    87,   337,    92,   110,
     112,    12,    94,   183,   186,   185,   187,   120,   250,   255,
     255,   260,    87,   250,    92,    45,   286,   292,   275,    92,
     130,   130,   201,   202,   202,   305,   305,   299,    88,    94,
     120,    88,   187,   112,    92,    92,   240,   252,   250,   267,
     271,    92,    45,    83,   273,   274,   249,   250,   250,    92,
     297,    94,   240,   252,   256,   257,   259,   251,    19,   265,
      87,   259,   294,    66,   288,   289,   290,   292,    65,    93,
     194,   197,   251,    18,    18,   260,   253,   254,   263,   265,
     268,   252,    87,   110,    45,    91,   294,   302,     1,   246,
     249,   249,    87,   258,   240,   257,   257,   252,   264,    92,
      94,   288,   290,    65,    92,    88,   251,    65,   259,   292,
      45,   112,   255,   260,   294,   291,   292,    92,    92,   252,
      65,    91,   293,    87,   112,   292,    92,   293
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  yyvsp[0] = yylval;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 364 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids an empty source file");
		;}
    break;

  case 4:
#line 375 "objc/objc-parse.y"
    { (yyval.dsptype) = NULL; ;}
    break;

  case 5:
#line 376 "objc/objc-parse.y"
    { obstack_free (&parser_obstack, (yyvsp[-2].otype)); ;}
    break;

  case 6:
#line 378 "objc/objc-parse.y"
    { (yyval.dsptype) = NULL; ggc_collect (); ;}
    break;

  case 7:
#line 379 "objc/objc-parse.y"
    { obstack_free (&parser_obstack, (yyvsp[-2].otype)); ;}
    break;

  case 11:
#line 387 "objc/objc-parse.y"
    { RESTORE_EXT_FLAGS ((yyvsp[-1].itype)); ;}
    break;

  case 13:
#line 394 "objc/objc-parse.y"
    { (yyval.otype) = obstack_alloc (&parser_obstack, 0); ;}
    break;

  case 14:
#line 399 "objc/objc-parse.y"
    { pedwarn ("data definition has no type or storage class");
		  POP_DECLSPEC_STACK; ;}
    break;

  case 15:
#line 402 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 16:
#line 404 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 17:
#line 406 "objc/objc-parse.y"
    { shadow_tag (finish_declspecs ((yyvsp[-1].dsptype))); ;}
    break;

  case 20:
#line 410 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C does not allow extra %<;%> outside of a function"); ;}
    break;

  case 21:
#line 416 "objc/objc-parse.y"
    { if (!start_function (current_declspecs, (yyvsp[0].dtrtype),
				       all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 22:
#line 421 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = (yyvsp[0].location);
		  store_parm_decls (); ;}
    break;

  case 23:
#line 424 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 24:
#line 427 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 25:
#line 429 "objc/objc-parse.y"
    { if (!start_function (current_declspecs, (yyvsp[0].dtrtype),
				       all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 26:
#line 434 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = (yyvsp[0].location);
		  store_parm_decls (); ;}
    break;

  case 27:
#line 437 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 28:
#line 440 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 29:
#line 442 "objc/objc-parse.y"
    { if (!start_function (current_declspecs, (yyvsp[0].dtrtype),
				       all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 30:
#line 447 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = (yyvsp[0].location);
		  store_parm_decls (); ;}
    break;

  case 31:
#line 450 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 32:
#line 453 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 36:
#line 463 "objc/objc-parse.y"
    { (yyval.code) = ADDR_EXPR; ;}
    break;

  case 37:
#line 465 "objc/objc-parse.y"
    { (yyval.code) = NEGATE_EXPR; ;}
    break;

  case 38:
#line 467 "objc/objc-parse.y"
    { (yyval.code) = CONVERT_EXPR;
		;}
    break;

  case 39:
#line 470 "objc/objc-parse.y"
    { (yyval.code) = PREINCREMENT_EXPR; ;}
    break;

  case 40:
#line 472 "objc/objc-parse.y"
    { (yyval.code) = PREDECREMENT_EXPR; ;}
    break;

  case 41:
#line 474 "objc/objc-parse.y"
    { (yyval.code) = BIT_NOT_EXPR; ;}
    break;

  case 42:
#line 476 "objc/objc-parse.y"
    { (yyval.code) = TRUTH_NOT_EXPR; ;}
    break;

  case 44:
#line 481 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_compound_expr ((yyvsp[-2].exprtype).value, (yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = COMPOUND_EXPR; ;}
    break;

  case 45:
#line 487 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 47:
#line 493 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].exprtype).value); ;}
    break;

  case 48:
#line 495 "objc/objc-parse.y"
    { chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].exprtype).value)); ;}
    break;

  case 50:
#line 501 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_indirect_ref ((yyvsp[0].exprtype).value, "unary *");
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 51:
#line 505 "objc/objc-parse.y"
    { (yyval.exprtype) = (yyvsp[0].exprtype);
		  RESTORE_EXT_FLAGS ((yyvsp[-1].itype)); ;}
    break;

  case 52:
#line 508 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_unary_op ((yyvsp[-1].code), (yyvsp[0].exprtype).value, 0);
		  overflow_warning ((yyval.exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 53:
#line 513 "objc/objc-parse.y"
    { (yyval.exprtype).value = finish_label_address_expr ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 54:
#line 516 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_sizeof--;
		  if (TREE_CODE ((yyvsp[0].exprtype).value) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND ((yyvsp[0].exprtype).value, 1)))
		    error ("%<sizeof%> applied to a bit-field");
		  (yyval.exprtype) = c_expr_sizeof_expr ((yyvsp[0].exprtype)); ;}
    break;

  case 55:
#line 523 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_sizeof--;
		  (yyval.exprtype) = c_expr_sizeof_type ((yyvsp[-1].typenametype)); ;}
    break;

  case 56:
#line 527 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_alignof--;
		  (yyval.exprtype).value = c_alignof_expr ((yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 57:
#line 532 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_alignof--;
		  (yyval.exprtype).value = c_alignof (groktypename ((yyvsp[-1].typenametype)));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 58:
#line 537 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_unary_op (REALPART_EXPR, (yyvsp[0].exprtype).value, 0);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 59:
#line 540 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_unary_op (IMAGPART_EXPR, (yyvsp[0].exprtype).value, 0);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 60:
#line 545 "objc/objc-parse.y"
    { skip_evaluation++; in_sizeof++; ;}
    break;

  case 61:
#line 549 "objc/objc-parse.y"
    { skip_evaluation++; in_alignof++; ;}
    break;

  case 62:
#line 553 "objc/objc-parse.y"
    { skip_evaluation++; in_typeof++; ;}
    break;

  case 64:
#line 559 "objc/objc-parse.y"
    { (yyval.exprtype).value = c_cast_expr ((yyvsp[-2].typenametype), (yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 66:
#line 566 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 67:
#line 568 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 68:
#line 570 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 69:
#line 572 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 70:
#line 574 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 71:
#line 576 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 72:
#line 578 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 73:
#line 580 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 74:
#line 582 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 75:
#line 584 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 76:
#line 586 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 77:
#line 588 "objc/objc-parse.y"
    { (yyval.exprtype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 78:
#line 590 "objc/objc-parse.y"
    { (yyvsp[-1].exprtype).value = lang_hooks.truthvalue_conversion
		    (default_conversion ((yyvsp[-1].exprtype).value));
		  skip_evaluation += (yyvsp[-1].exprtype).value == truthvalue_false_node; ;}
    break;

  case 79:
#line 594 "objc/objc-parse.y"
    { skip_evaluation -= (yyvsp[-3].exprtype).value == truthvalue_false_node;
		  (yyval.exprtype) = parser_build_binary_op (TRUTH_ANDIF_EXPR, (yyvsp[-3].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 80:
#line 597 "objc/objc-parse.y"
    { (yyvsp[-1].exprtype).value = lang_hooks.truthvalue_conversion
		    (default_conversion ((yyvsp[-1].exprtype).value));
		  skip_evaluation += (yyvsp[-1].exprtype).value == truthvalue_true_node; ;}
    break;

  case 81:
#line 601 "objc/objc-parse.y"
    { skip_evaluation -= (yyvsp[-3].exprtype).value == truthvalue_true_node;
		  (yyval.exprtype) = parser_build_binary_op (TRUTH_ORIF_EXPR, (yyvsp[-3].exprtype), (yyvsp[0].exprtype)); ;}
    break;

  case 82:
#line 604 "objc/objc-parse.y"
    { (yyvsp[-1].exprtype).value = lang_hooks.truthvalue_conversion
		    (default_conversion ((yyvsp[-1].exprtype).value));
		  skip_evaluation += (yyvsp[-1].exprtype).value == truthvalue_false_node; ;}
    break;

  case 83:
#line 608 "objc/objc-parse.y"
    { skip_evaluation += (((yyvsp[-4].exprtype).value == truthvalue_true_node)
				      - ((yyvsp[-4].exprtype).value == truthvalue_false_node)); ;}
    break;

  case 84:
#line 611 "objc/objc-parse.y"
    { skip_evaluation -= (yyvsp[-6].exprtype).value == truthvalue_true_node;
		  (yyval.exprtype).value = build_conditional_expr ((yyvsp[-6].exprtype).value, (yyvsp[-3].exprtype).value,
						     (yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 85:
#line 616 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids omitting the middle term of a ?: expression");
		  /* Make sure first operand is calculated only once.  */
		  (yyvsp[0].ttype) = save_expr (default_conversion ((yyvsp[-1].exprtype).value));
		  (yyvsp[-1].exprtype).value = lang_hooks.truthvalue_conversion ((yyvsp[0].ttype));
		  skip_evaluation += (yyvsp[-1].exprtype).value == truthvalue_true_node; ;}
    break;

  case 86:
#line 623 "objc/objc-parse.y"
    { skip_evaluation -= (yyvsp[-4].exprtype).value == truthvalue_true_node;
		  (yyval.exprtype).value = build_conditional_expr ((yyvsp[-4].exprtype).value, (yyvsp[-3].ttype),
						     (yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 87:
#line 628 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_modify_expr ((yyvsp[-2].exprtype).value, NOP_EXPR, (yyvsp[0].exprtype).value);
		  (yyval.exprtype).original_code = MODIFY_EXPR;
		;}
    break;

  case 88:
#line 632 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_modify_expr ((yyvsp[-2].exprtype).value, (yyvsp[-1].code), (yyvsp[0].exprtype).value);
		  TREE_NO_WARNING ((yyval.exprtype).value) = 1;
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 89:
#line 640 "objc/objc-parse.y"
    {
		  if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  (yyval.exprtype).value = build_external_ref ((yyvsp[0].ttype), yychar == '(');
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 90:
#line 647 "objc/objc-parse.y"
    { (yyval.exprtype).value = (yyvsp[0].ttype); (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 91:
#line 649 "objc/objc-parse.y"
    { (yyval.exprtype).value = (yyvsp[0].ttype); (yyval.exprtype).original_code = STRING_CST; ;}
    break;

  case 92:
#line 651 "objc/objc-parse.y"
    { (yyval.exprtype).value = fname_decl (C_RID_CODE ((yyvsp[0].ttype)), (yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 93:
#line 654 "objc/objc-parse.y"
    { start_init (NULL_TREE, NULL, 0);
		  (yyval.ttype) = groktypename ((yyvsp[-2].typenametype));
		  if (C_TYPE_VARIABLE_SIZE ((yyval.ttype)))
		    {
		      error ("compound literal has variable size");
		      (yyval.ttype) = error_mark_node;
		    }
		  really_start_incremental_init ((yyval.ttype)); ;}
    break;

  case 94:
#line 663 "objc/objc-parse.y"
    { struct c_expr init = pop_init_level (0);
		  tree constructor = init.value;
		  tree type = (yyvsp[-2].ttype);
		  finish_init ();
		  maybe_warn_string_init (type, init);

		  if (pedantic && !flag_isoc99)
		    pedwarn ("ISO C90 forbids compound literals");
		  (yyval.exprtype).value = build_compound_literal (type, constructor);
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 95:
#line 675 "objc/objc-parse.y"
    { (yyval.exprtype).value = (yyvsp[-1].exprtype).value;
		  if (TREE_CODE ((yyval.exprtype).value) == MODIFY_EXPR)
		    TREE_NO_WARNING ((yyval.exprtype).value) = 1;
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 96:
#line 680 "objc/objc-parse.y"
    { (yyval.exprtype).value = error_mark_node; (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 97:
#line 682 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids braced-groups within expressions");
		  (yyval.exprtype).value = c_finish_stmt_expr ((yyvsp[-2].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 98:
#line 688 "objc/objc-parse.y"
    { c_finish_stmt_expr ((yyvsp[-2].ttype));
		  (yyval.exprtype).value = error_mark_node;
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 99:
#line 693 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_function_call ((yyvsp[-3].exprtype).value, (yyvsp[-1].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 100:
#line 696 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_va_arg ((yyvsp[-3].exprtype).value, groktypename ((yyvsp[-1].typenametype)));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 101:
#line 700 "objc/objc-parse.y"
    { tree type = groktypename ((yyvsp[-1].typenametype));
		  if (type == error_mark_node)
		    offsetof_base = error_mark_node;
		  else
		    offsetof_base = build1 (INDIRECT_REF, type, NULL);
		;}
    break;

  case 102:
#line 707 "objc/objc-parse.y"
    { (yyval.exprtype).value = fold_offsetof ((yyvsp[-1].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 103:
#line 710 "objc/objc-parse.y"
    { (yyval.exprtype).value = error_mark_node; (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 104:
#line 713 "objc/objc-parse.y"
    {
                  tree c;

                  c = fold ((yyvsp[-5].exprtype).value);
                  STRIP_NOPS (c);
                  if (TREE_CODE (c) != INTEGER_CST)
                    error ("first argument to %<__builtin_choose_expr%> not"
			   " a constant");
                  (yyval.exprtype) = integer_zerop (c) ? (yyvsp[-1].exprtype) : (yyvsp[-3].exprtype);
		;}
    break;

  case 105:
#line 724 "objc/objc-parse.y"
    { (yyval.exprtype).value = error_mark_node; (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 106:
#line 726 "objc/objc-parse.y"
    {
		  tree e1, e2;

		  e1 = TYPE_MAIN_VARIANT (groktypename ((yyvsp[-3].typenametype)));
		  e2 = TYPE_MAIN_VARIANT (groktypename ((yyvsp[-1].typenametype)));

		  (yyval.exprtype).value = comptypes (e1, e2)
		    ? build_int_cst (NULL_TREE, 1)
		    : build_int_cst (NULL_TREE, 0);
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 107:
#line 738 "objc/objc-parse.y"
    { (yyval.exprtype).value = error_mark_node; (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 108:
#line 740 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_array_ref ((yyvsp[-3].exprtype).value, (yyvsp[-1].exprtype).value);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 109:
#line 743 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_component_ref ((yyvsp[-2].exprtype).value, (yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 110:
#line 746 "objc/objc-parse.y"
    {
                  tree expr = build_indirect_ref ((yyvsp[-2].exprtype).value, "->");
		  (yyval.exprtype).value = build_component_ref (expr, (yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK;
		;}
    break;

  case 111:
#line 752 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_unary_op (POSTINCREMENT_EXPR, (yyvsp[-1].exprtype).value, 0);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 112:
#line 755 "objc/objc-parse.y"
    { (yyval.exprtype).value = build_unary_op (POSTDECREMENT_EXPR, (yyvsp[-1].exprtype).value, 0);
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 113:
#line 758 "objc/objc-parse.y"
    { (yyval.exprtype).value = objc_build_message_expr ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 114:
#line 761 "objc/objc-parse.y"
    { (yyval.exprtype).value = objc_build_selector_expr ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 115:
#line 764 "objc/objc-parse.y"
    { (yyval.exprtype).value = objc_build_protocol_expr ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 116:
#line 767 "objc/objc-parse.y"
    { (yyval.exprtype).value = objc_build_encode_expr ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 117:
#line 770 "objc/objc-parse.y"
    { (yyval.exprtype).value = objc_build_string_object ((yyvsp[0].ttype));
		  (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 118:
#line 780 "objc/objc-parse.y"
    { (yyval.ttype) = build_component_ref (offsetof_base, (yyvsp[0].ttype)); ;}
    break;

  case 119:
#line 782 "objc/objc-parse.y"
    { (yyval.ttype) = build_component_ref ((yyvsp[-2].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 120:
#line 784 "objc/objc-parse.y"
    { (yyval.ttype) = build_array_ref ((yyvsp[-3].ttype), (yyvsp[-1].exprtype).value); ;}
    break;

  case 123:
#line 797 "objc/objc-parse.y"
    { ;}
    break;

  case 128:
#line 813 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 129:
#line 815 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 130:
#line 817 "objc/objc-parse.y"
    { shadow_tag_warned (finish_declspecs ((yyvsp[-1].dsptype)), 1);
		  pedwarn ("empty declaration"); ;}
    break;

  case 131:
#line 820 "objc/objc-parse.y"
    { pedwarn ("empty declaration"); ;}
    break;

  case 132:
#line 829 "objc/objc-parse.y"
    { ;}
    break;

  case 133:
#line 837 "objc/objc-parse.y"
    { pending_xref_error ();
		  PUSH_DECLSPEC_STACK;
		  if ((yyvsp[0].dsptype))
		    {
		      prefix_attributes = (yyvsp[0].dsptype)->attrs;
		      (yyvsp[0].dsptype)->attrs = NULL_TREE;
		      current_declspecs = (yyvsp[0].dsptype);
		    }
		  else
		    {
		      prefix_attributes = NULL_TREE;
		      current_declspecs = build_null_declspecs ();
		    }
		  current_declspecs = finish_declspecs (current_declspecs);
		  all_prefix_attributes = prefix_attributes; ;}
    break;

  case 134:
#line 858 "objc/objc-parse.y"
    { all_prefix_attributes = chainon ((yyvsp[0].ttype), prefix_attributes); ;}
    break;

  case 135:
#line 863 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 136:
#line 865 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 137:
#line 867 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 138:
#line 869 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 139:
#line 871 "objc/objc-parse.y"
    { shadow_tag (finish_declspecs ((yyvsp[-1].dsptype))); ;}
    break;

  case 140:
#line 873 "objc/objc-parse.y"
    { RESTORE_EXT_FLAGS ((yyvsp[-1].itype)); ;}
    break;

  case 141:
#line 919 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual (build_null_declspecs (), (yyvsp[0].ttype)); ;}
    break;

  case 142:
#line 921 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 143:
#line 923 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 144:
#line 928 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 145:
#line 933 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 146:
#line 935 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 147:
#line 940 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs (build_null_declspecs (), (yyvsp[0].ttype)); ;}
    break;

  case 148:
#line 942 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 149:
#line 947 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type (build_null_declspecs (), (yyvsp[0].tstype)); ;}
    break;

  case 150:
#line 949 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 151:
#line 951 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 152:
#line 953 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 153:
#line 955 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 154:
#line 957 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 155:
#line 959 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 156:
#line 964 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type (build_null_declspecs (), (yyvsp[0].tstype)); ;}
    break;

  case 157:
#line 966 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 158:
#line 968 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 159:
#line 970 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 160:
#line 972 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 161:
#line 974 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 162:
#line 979 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 163:
#line 981 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 164:
#line 983 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 165:
#line 985 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 166:
#line 987 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 167:
#line 989 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 168:
#line 994 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 169:
#line 996 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 170:
#line 998 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 171:
#line 1000 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 172:
#line 1002 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 173:
#line 1007 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec (build_null_declspecs (), (yyvsp[0].ttype)); ;}
    break;

  case 174:
#line 1009 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 175:
#line 1011 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 176:
#line 1013 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 177:
#line 1015 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 178:
#line 1017 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 179:
#line 1019 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 180:
#line 1024 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 181:
#line 1029 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 182:
#line 1031 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 183:
#line 1033 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 184:
#line 1035 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 185:
#line 1037 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 186:
#line 1039 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 187:
#line 1044 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 188:
#line 1049 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 189:
#line 1051 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 190:
#line 1053 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 191:
#line 1055 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 192:
#line 1057 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 193:
#line 1059 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 194:
#line 1061 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 195:
#line 1063 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 196:
#line 1065 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 197:
#line 1067 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 198:
#line 1072 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 199:
#line 1074 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 200:
#line 1076 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 201:
#line 1078 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 202:
#line 1080 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 203:
#line 1085 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 204:
#line 1087 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_qual ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 205:
#line 1089 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 206:
#line 1091 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 207:
#line 1093 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 208:
#line 1095 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 209:
#line 1097 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 210:
#line 1099 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 211:
#line 1101 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 212:
#line 1103 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_scspec ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 213:
#line 1108 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_attrs ((yyvsp[-1].dsptype), (yyvsp[0].ttype)); ;}
    break;

  case 214:
#line 1110 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 215:
#line 1112 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 216:
#line 1114 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 217:
#line 1116 "objc/objc-parse.y"
    { (yyval.dsptype) = declspecs_add_type ((yyvsp[-1].dsptype), (yyvsp[0].tstype)); ;}
    break;

  case 274:
#line 1203 "objc/objc-parse.y"
    { (yyval.dsptype) = NULL; ;}
    break;

  case 275:
#line 1205 "objc/objc-parse.y"
    { (yyval.dsptype) = (yyvsp[0].dsptype); ;}
    break;

  case 279:
#line 1240 "objc/objc-parse.y"
    { OBJC_NEED_RAW_IDENTIFIER (1);
		  (yyval.tstype).kind = ctsk_resword;
		  (yyval.tstype).spec = (yyvsp[0].ttype); ;}
    break;

  case 282:
#line 1252 "objc/objc-parse.y"
    { /* For a typedef name, record the meaning, not the name.
		     In case of `foo foo, bar;'.  */
		  (yyval.tstype).kind = ctsk_typedef;
		  (yyval.tstype).spec = lookup_name ((yyvsp[0].ttype)); ;}
    break;

  case 283:
#line 1257 "objc/objc-parse.y"
    { (yyval.tstype).kind = ctsk_objc;
		  (yyval.tstype).spec = objc_get_protocol_qualified_type ((yyvsp[-1].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 284:
#line 1260 "objc/objc-parse.y"
    { (yyval.tstype).kind = ctsk_objc;
		  (yyval.tstype).spec = objc_get_protocol_qualified_type ((yyvsp[-1].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 285:
#line 1266 "objc/objc-parse.y"
    { (yyval.tstype).kind = ctsk_objc;
		  (yyval.tstype).spec = objc_get_protocol_qualified_type (NULL_TREE, (yyvsp[0].ttype)); ;}
    break;

  case 286:
#line 1269 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_typeof--;
		  if (TREE_CODE ((yyvsp[-1].exprtype).value) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND ((yyvsp[-1].exprtype).value, 1)))
		    error ("%<typeof%> applied to a bit-field");
		  (yyval.tstype).kind = ctsk_typeof;
		  (yyval.tstype).spec = TREE_TYPE ((yyvsp[-1].exprtype).value);
		  pop_maybe_used (variably_modified_type_p ((yyval.tstype).spec,
							    NULL_TREE)); ;}
    break;

  case 287:
#line 1279 "objc/objc-parse.y"
    { skip_evaluation--;
		  in_typeof--;
		  (yyval.tstype).kind = ctsk_typeof;
		  (yyval.tstype).spec = groktypename ((yyvsp[-1].typenametype));
		  pop_maybe_used (variably_modified_type_p ((yyval.tstype).spec,
							    NULL_TREE)); ;}
    break;

  case 292:
#line 1301 "objc/objc-parse.y"
    { (yyval.ttype) = start_decl ((yyvsp[-3].dtrtype), current_declspecs, true,
					  chainon ((yyvsp[-1].ttype), all_prefix_attributes));
		  if (!(yyval.ttype))
		    (yyval.ttype) = error_mark_node;
		  start_init ((yyval.ttype), (yyvsp[-2].ttype), global_bindings_p ()); ;}
    break;

  case 293:
#line 1308 "objc/objc-parse.y"
    { finish_init ();
		  if ((yyvsp[-1].ttype) != error_mark_node)
		    {
		      maybe_warn_string_init (TREE_TYPE ((yyvsp[-1].ttype)), (yyvsp[0].exprtype));
		      finish_decl ((yyvsp[-1].ttype), (yyvsp[0].exprtype).value, (yyvsp[-4].ttype));
		    }
		;}
    break;

  case 294:
#line 1316 "objc/objc-parse.y"
    { tree d = start_decl ((yyvsp[-2].dtrtype), current_declspecs, false,
				       chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  if (d)
		    finish_decl (d, NULL_TREE, (yyvsp[-1].ttype));
                ;}
    break;

  case 295:
#line 1325 "objc/objc-parse.y"
    { (yyval.ttype) = start_decl ((yyvsp[-3].dtrtype), current_declspecs, true,
					  chainon ((yyvsp[-1].ttype), all_prefix_attributes));
		  if (!(yyval.ttype))
		    (yyval.ttype) = error_mark_node;
		  start_init ((yyval.ttype), (yyvsp[-2].ttype), global_bindings_p ()); ;}
    break;

  case 296:
#line 1332 "objc/objc-parse.y"
    { finish_init ();
		  if ((yyvsp[-1].ttype) != error_mark_node)
		    {
		      maybe_warn_string_init (TREE_TYPE ((yyvsp[-1].ttype)), (yyvsp[0].exprtype));
		      finish_decl ((yyvsp[-1].ttype), (yyvsp[0].exprtype).value, (yyvsp[-4].ttype));
		    }
		;}
    break;

  case 297:
#line 1340 "objc/objc-parse.y"
    { tree d = start_decl ((yyvsp[-2].dtrtype), current_declspecs, false,
				       chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  if (d)
                    finish_decl (d, NULL_TREE, (yyvsp[-1].ttype)); ;}
    break;

  case 298:
#line 1349 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 299:
#line 1351 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 300:
#line 1356 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 301:
#line 1358 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 302:
#line 1364 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[-3].ttype); ;}
    break;

  case 303:
#line 1366 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 304:
#line 1371 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 305:
#line 1373 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 306:
#line 1378 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 307:
#line 1380 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); ;}
    break;

  case 308:
#line 1382 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[-3].ttype), build_tree_list (NULL_TREE, (yyvsp[-1].ttype))); ;}
    break;

  case 309:
#line 1384 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[-5].ttype), tree_cons (NULL_TREE, (yyvsp[-3].ttype), (yyvsp[-1].ttype))); ;}
    break;

  case 310:
#line 1386 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[-3].ttype), (yyvsp[-1].ttype)); ;}
    break;

  case 317:
#line 1408 "objc/objc-parse.y"
    { (yyval.exprtype) = (yyvsp[0].exprtype); ;}
    break;

  case 318:
#line 1410 "objc/objc-parse.y"
    { really_start_incremental_init (NULL_TREE); ;}
    break;

  case 319:
#line 1412 "objc/objc-parse.y"
    { (yyval.exprtype) = pop_init_level (0); ;}
    break;

  case 320:
#line 1414 "objc/objc-parse.y"
    { (yyval.exprtype).value = error_mark_node; (yyval.exprtype).original_code = ERROR_MARK; ;}
    break;

  case 321:
#line 1420 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids empty initializer braces"); ;}
    break;

  case 325:
#line 1434 "objc/objc-parse.y"
    { if (pedantic && !flag_isoc99)
		    pedwarn ("ISO C90 forbids specifying subobject to initialize"); ;}
    break;

  case 326:
#line 1437 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("obsolete use of designated initializer without %<=%>"); ;}
    break;

  case 327:
#line 1440 "objc/objc-parse.y"
    { set_init_label ((yyvsp[-1].ttype));
		  if (pedantic)
		    pedwarn ("obsolete use of designated initializer with %<:%>"); ;}
    break;

  case 328:
#line 1444 "objc/objc-parse.y"
    {;}
    break;

  case 330:
#line 1450 "objc/objc-parse.y"
    { push_init_level (0); ;}
    break;

  case 331:
#line 1452 "objc/objc-parse.y"
    { process_init_element (pop_init_level (0)); ;}
    break;

  case 332:
#line 1454 "objc/objc-parse.y"
    { process_init_element ((yyvsp[0].exprtype)); ;}
    break;

  case 336:
#line 1465 "objc/objc-parse.y"
    { set_init_label ((yyvsp[0].ttype)); ;}
    break;

  case 338:
#line 1471 "objc/objc-parse.y"
    { set_init_index ((yyvsp[-3].exprtype).value, (yyvsp[-1].exprtype).value);
		  if (pedantic)
		    pedwarn ("ISO C forbids specifying range of elements to initialize"); ;}
    break;

  case 339:
#line 1475 "objc/objc-parse.y"
    { set_init_index ((yyvsp[-1].exprtype).value, NULL_TREE); ;}
    break;

  case 340:
#line 1480 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (!start_function (current_declspecs, (yyvsp[0].dtrtype),
				       all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		;}
    break;

  case 341:
#line 1492 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  DECL_SOURCE_LOCATION (decl) = (yyvsp[0].location);
		  store_parm_decls (); ;}
    break;

  case 342:
#line 1501 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  add_stmt ((yyvsp[0].ttype));
		  finish_function ();
		  pop_function_context ();
		  add_stmt (build_stmt (DECL_EXPR, decl)); ;}
    break;

  case 343:
#line 1510 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (!start_function (current_declspecs, (yyvsp[0].dtrtype),
				       all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		;}
    break;

  case 344:
#line 1522 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  DECL_SOURCE_LOCATION (decl) = (yyvsp[0].location);
		  store_parm_decls (); ;}
    break;

  case 345:
#line 1531 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  add_stmt ((yyvsp[0].ttype));
		  finish_function ();
		  pop_function_context ();
		  add_stmt (build_stmt (DECL_EXPR, decl)); ;}
    break;

  case 348:
#line 1550 "objc/objc-parse.y"
    { (yyval.dtrtype) = (yyvsp[-2].ttype) ? build_attrs_declarator ((yyvsp[-2].ttype), (yyvsp[-1].dtrtype)) : (yyvsp[-1].dtrtype); ;}
    break;

  case 349:
#line 1552 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator ((yyvsp[0].arginfotype), (yyvsp[-2].dtrtype)); ;}
    break;

  case 350:
#line 1554 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner ((yyvsp[0].dtrtype), (yyvsp[-1].dtrtype), false); ;}
    break;

  case 351:
#line 1556 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 352:
#line 1558 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_id_declarator ((yyvsp[0].ttype)); ;}
    break;

  case 355:
#line 1572 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator ((yyvsp[0].arginfotype), (yyvsp[-2].dtrtype)); ;}
    break;

  case 356:
#line 1574 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner ((yyvsp[0].dtrtype), (yyvsp[-1].dtrtype), false); ;}
    break;

  case 357:
#line 1576 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_id_declarator ((yyvsp[0].ttype)); ;}
    break;

  case 358:
#line 1581 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator ((yyvsp[0].arginfotype), (yyvsp[-2].dtrtype)); ;}
    break;

  case 359:
#line 1583 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner ((yyvsp[0].dtrtype), (yyvsp[-1].dtrtype), false); ;}
    break;

  case 360:
#line 1585 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 361:
#line 1587 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 362:
#line 1589 "objc/objc-parse.y"
    { (yyval.dtrtype) = (yyvsp[-2].ttype) ? build_attrs_declarator ((yyvsp[-2].ttype), (yyvsp[-1].dtrtype)) : (yyvsp[-1].dtrtype); ;}
    break;

  case 363:
#line 1597 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator ((yyvsp[0].arginfotype), (yyvsp[-2].dtrtype)); ;}
    break;

  case 364:
#line 1599 "objc/objc-parse.y"
    { (yyval.dtrtype) = (yyvsp[-2].ttype) ? build_attrs_declarator ((yyvsp[-2].ttype), (yyvsp[-1].dtrtype)) : (yyvsp[-1].dtrtype); ;}
    break;

  case 365:
#line 1601 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 366:
#line 1603 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner ((yyvsp[0].dtrtype), (yyvsp[-1].dtrtype), false); ;}
    break;

  case 367:
#line 1605 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_id_declarator ((yyvsp[0].ttype)); ;}
    break;

  case 368:
#line 1610 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 369:
#line 1612 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 370:
#line 1617 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 371:
#line 1619 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 372:
#line 1624 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 373:
#line 1626 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 374:
#line 1637 "objc/objc-parse.y"
    { (yyval.ttype) = start_struct (RECORD_TYPE, (yyvsp[-1].ttype));
		  /* Start scope of tag before parsing components.  */
		;}
    break;

  case 375:
#line 1641 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_struct ((yyvsp[-3].ttype), nreverse ((yyvsp[-2].ttype)),
					   chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef; ;}
    break;

  case 376:
#line 1645 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_struct (start_struct (RECORD_TYPE,
							 NULL_TREE),
					   nreverse ((yyvsp[-2].ttype)), chainon ((yyvsp[-4].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef;
		;}
    break;

  case 377:
#line 1651 "objc/objc-parse.y"
    { (yyval.ttype) = start_struct (UNION_TYPE, (yyvsp[-1].ttype)); ;}
    break;

  case 378:
#line 1653 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_struct ((yyvsp[-3].ttype), nreverse ((yyvsp[-2].ttype)),
					   chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef; ;}
    break;

  case 379:
#line 1657 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_struct (start_struct (UNION_TYPE,
							 NULL_TREE),
					   nreverse ((yyvsp[-2].ttype)), chainon ((yyvsp[-4].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef;
		;}
    break;

  case 380:
#line 1663 "objc/objc-parse.y"
    { (yyval.ttype) = start_enum ((yyvsp[-1].ttype)); ;}
    break;

  case 381:
#line 1665 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_enum ((yyvsp[-4].ttype), nreverse ((yyvsp[-3].ttype)),
					 chainon ((yyvsp[-7].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef; ;}
    break;

  case 382:
#line 1669 "objc/objc-parse.y"
    { (yyval.ttype) = start_enum (NULL_TREE); ;}
    break;

  case 383:
#line 1671 "objc/objc-parse.y"
    { (yyval.tstype).spec = finish_enum ((yyvsp[-4].ttype), nreverse ((yyvsp[-3].ttype)),
					 chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype)));
		  (yyval.tstype).kind = ctsk_tagdef; ;}
    break;

  case 384:
#line 1678 "objc/objc-parse.y"
    { (yyval.tstype) = parser_xref_tag (RECORD_TYPE, (yyvsp[0].ttype)); ;}
    break;

  case 385:
#line 1680 "objc/objc-parse.y"
    { (yyval.tstype) = parser_xref_tag (UNION_TYPE, (yyvsp[0].ttype)); ;}
    break;

  case 386:
#line 1682 "objc/objc-parse.y"
    { (yyval.tstype) = parser_xref_tag (ENUMERAL_TYPE, (yyvsp[0].ttype));
		  /* In ISO C, enumerated types can be referred to
		     only if already defined.  */
		  if (pedantic && !COMPLETE_TYPE_P ((yyval.tstype).spec))
		    pedwarn ("ISO C forbids forward references to %<enum%> types"); ;}
    break;

  case 390:
#line 1697 "objc/objc-parse.y"
    { if (pedantic && !flag_isoc99)
		    pedwarn ("comma at end of enumerator list"); ;}
    break;

  case 391:
#line 1715 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 392:
#line 1717 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-1].ttype));
		  pedwarn ("no semicolon at end of struct or union"); ;}
    break;

  case 393:
#line 1722 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 394:
#line 1724 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[-2].ttype)); ;}
    break;

  case 395:
#line 1726 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified"); ;}
    break;

  case 396:
#line 1730 "objc/objc-parse.y"
    { (yyval.ttype) = nreverse (objc_get_class_ivars ((yyvsp[-1].ttype))); ;}
    break;

  case 397:
#line 1735 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 398:
#line 1738 "objc/objc-parse.y"
    {
		  /* Support for unnamed structs or unions as members of
		     structs or unions (which is [a] useful and [b] supports
		     MS P-SDK).  */
		  (yyval.ttype) = grokfield (build_id_declarator (NULL_TREE),
				  current_declspecs, NULL_TREE);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 399:
#line 1746 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 400:
#line 1749 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids member declarations with no members");
		  shadow_tag_warned (finish_declspecs ((yyvsp[0].dsptype)), pedantic);
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 401:
#line 1754 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 402:
#line 1756 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype);
		  RESTORE_EXT_FLAGS ((yyvsp[-1].itype)); ;}
    break;

  case 404:
#line 1763 "objc/objc-parse.y"
    { TREE_CHAIN ((yyvsp[0].ttype)) = (yyvsp[-3].ttype); (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 406:
#line 1769 "objc/objc-parse.y"
    { TREE_CHAIN ((yyvsp[0].ttype)) = (yyvsp[-3].ttype); (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 407:
#line 1774 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield ((yyvsp[-1].dtrtype), current_declspecs, NULL_TREE);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 408:
#line 1778 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield ((yyvsp[-3].dtrtype), current_declspecs, (yyvsp[-1].exprtype).value);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 409:
#line 1782 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield (build_id_declarator (NULL_TREE),
				  current_declspecs, (yyvsp[-1].exprtype).value);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 410:
#line 1790 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield ((yyvsp[-1].dtrtype), current_declspecs, NULL_TREE);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 411:
#line 1794 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield ((yyvsp[-3].dtrtype), current_declspecs, (yyvsp[-1].exprtype).value);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 412:
#line 1798 "objc/objc-parse.y"
    { (yyval.ttype) = grokfield (build_id_declarator (NULL_TREE),
				  current_declspecs, (yyvsp[-1].exprtype).value);
		  decl_attributes (&(yyval.ttype),
				   chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); ;}
    break;

  case 414:
#line 1810 "objc/objc-parse.y"
    { if ((yyvsp[-2].ttype) == error_mark_node)
		    (yyval.ttype) = (yyvsp[-2].ttype);
		  else
		    TREE_CHAIN ((yyvsp[0].ttype)) = (yyvsp[-2].ttype), (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 415:
#line 1815 "objc/objc-parse.y"
    { (yyval.ttype) = error_mark_node; ;}
    break;

  case 416:
#line 1821 "objc/objc-parse.y"
    { (yyval.ttype) = build_enumerator ((yyvsp[0].ttype), NULL_TREE); ;}
    break;

  case 417:
#line 1823 "objc/objc-parse.y"
    { (yyval.ttype) = build_enumerator ((yyvsp[-2].ttype), (yyvsp[0].exprtype).value); ;}
    break;

  case 418:
#line 1828 "objc/objc-parse.y"
    { pending_xref_error ();
		  (yyval.dsptype) = finish_declspecs ((yyvsp[0].dsptype)); ;}
    break;

  case 419:
#line 1831 "objc/objc-parse.y"
    { (yyval.typenametype) = XOBNEW (&parser_obstack, struct c_type_name);
		  (yyval.typenametype)->specs = (yyvsp[-1].dsptype);
		  (yyval.typenametype)->declarator = (yyvsp[0].dtrtype); ;}
    break;

  case 420:
#line 1838 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_id_declarator (NULL_TREE); ;}
    break;

  case 422:
#line 1844 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs, all_prefix_attributes,
				     build_id_declarator (NULL_TREE)); ;}
    break;

  case 423:
#line 1847 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs, all_prefix_attributes,
				     (yyvsp[0].dtrtype)); ;}
    break;

  case 424:
#line 1850 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes),
				     (yyvsp[-1].dtrtype)); ;}
    break;

  case 428:
#line 1863 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 429:
#line 1868 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator
		    ((yyvsp[0].dsptype), build_id_declarator (NULL_TREE)); ;}
    break;

  case 430:
#line 1871 "objc/objc-parse.y"
    { (yyval.dtrtype) = make_pointer_declarator ((yyvsp[-1].dsptype), (yyvsp[0].dtrtype)); ;}
    break;

  case 431:
#line 1876 "objc/objc-parse.y"
    { (yyval.dtrtype) = (yyvsp[-2].ttype) ? build_attrs_declarator ((yyvsp[-2].ttype), (yyvsp[-1].dtrtype)) : (yyvsp[-1].dtrtype); ;}
    break;

  case 432:
#line 1878 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator ((yyvsp[0].arginfotype), (yyvsp[-2].dtrtype)); ;}
    break;

  case 433:
#line 1880 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner ((yyvsp[0].dtrtype), (yyvsp[-1].dtrtype), true); ;}
    break;

  case 434:
#line 1882 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_function_declarator
		    ((yyvsp[0].arginfotype), build_id_declarator (NULL_TREE)); ;}
    break;

  case 435:
#line 1885 "objc/objc-parse.y"
    { (yyval.dtrtype) = set_array_declarator_inner
		    ((yyvsp[0].dtrtype), build_id_declarator (NULL_TREE), true); ;}
    break;

  case 436:
#line 1893 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_array_declarator ((yyvsp[-1].exprtype).value, (yyvsp[-2].dsptype), false, false); ;}
    break;

  case 437:
#line 1895 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_array_declarator (NULL_TREE, (yyvsp[-1].dsptype), false, false); ;}
    break;

  case 438:
#line 1897 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_array_declarator (NULL_TREE, (yyvsp[-2].dsptype), false, true); ;}
    break;

  case 439:
#line 1899 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_array_declarator ((yyvsp[-1].exprtype).value, (yyvsp[-2].dsptype), true, false); ;}
    break;

  case 440:
#line 1902 "objc/objc-parse.y"
    { (yyval.dtrtype) = build_array_declarator ((yyvsp[-1].exprtype).value, (yyvsp[-3].dsptype), true, false); ;}
    break;

  case 443:
#line 1915 "objc/objc-parse.y"
    {
		  error ("label at end of compound statement");
		;}
    break;

  case 451:
#line 1932 "objc/objc-parse.y"
    {
		  if ((pedantic && !flag_isoc99)
		      || warn_declaration_after_statement)
		    pedwarn_c90 ("ISO C90 forbids mixed declarations and code");
		;}
    break;

  case 466:
#line 1966 "objc/objc-parse.y"
    { (yyval.ttype) = c_begin_compound_stmt (flag_isoc99); ;}
    break;

  case 468:
#line 1974 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids label declarations"); ;}
    break;

  case 471:
#line 1985 "objc/objc-parse.y"
    { tree link;
		  for (link = (yyvsp[-1].ttype); link; link = TREE_CHAIN (link))
		    {
		      tree label = declare_label (TREE_VALUE (link));
		      C_DECLARED_LABEL_FLAG (label) = 1;
		      add_stmt (build_stmt (DECL_EXPR, label));
		    }
		;}
    break;

  case 472:
#line 1999 "objc/objc-parse.y"
    { add_stmt ((yyvsp[0].ttype)); ;}
    break;

  case 474:
#line 2003 "objc/objc-parse.y"
    { (yyval.ttype) = c_begin_compound_stmt (true); ;}
    break;

  case 479:
#line 2017 "objc/objc-parse.y"
    { if (cur_stmt_list == NULL)
		    {
		      error ("braced-group within expression allowed "
			     "only inside a function");
		      YYERROR;
		    }
		  (yyval.ttype) = c_begin_stmt_expr ();
		;}
    break;

  case 480:
#line 2028 "objc/objc-parse.y"
    { (yyval.ttype) = c_end_compound_stmt ((yyvsp[-1].ttype), true); ;}
    break;

  case 481:
#line 2036 "objc/objc-parse.y"
    { if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  (yyval.location) = input_location; ;}
    break;

  case 484:
#line 2049 "objc/objc-parse.y"
    { (yyval.ttype) = c_end_compound_stmt ((yyvsp[-2].ttype), flag_isoc99); ;}
    break;

  case 485:
#line 2054 "objc/objc-parse.y"
    {
		  /* Two cases cannot and do not have line numbers associated:
		     If stmt is degenerate, such as "2;", then stmt is an
		     INTEGER_CST, which cannot hold line numbers.  But that's
		     ok because the statement will either be changed to a
		     MODIFY_EXPR during gimplification of the statement expr,
		     or discarded.  If stmt was compound, but without new
		     variables, we will have skipped the creation of a BIND
		     and will have a bare STATEMENT_LIST.  But that's ok
		     because (recursively) all of the component statments
		     should already have line numbers assigned.  */
		  if ((yyvsp[0].ttype) && EXPR_P ((yyvsp[0].ttype)))
		    SET_EXPR_LOCATION ((yyvsp[0].ttype), (yyvsp[-1].location));
		;}
    break;

  case 486:
#line 2072 "objc/objc-parse.y"
    { if ((yyvsp[0].ttype)) SET_EXPR_LOCATION ((yyvsp[0].ttype), (yyvsp[-1].location)); ;}
    break;

  case 487:
#line 2076 "objc/objc-parse.y"
    { (yyval.ttype) = lang_hooks.truthvalue_conversion ((yyvsp[0].exprtype).value);
		  if (EXPR_P ((yyval.ttype)))
		    SET_EXPR_LOCATION ((yyval.ttype), (yyvsp[-1].location)); ;}
    break;

  case 488:
#line 2089 "objc/objc-parse.y"
    { (yyval.ttype) = c_end_compound_stmt ((yyvsp[-2].ttype), flag_isoc99); ;}
    break;

  case 489:
#line 2094 "objc/objc-parse.y"
    { if (extra_warnings)
		    add_stmt (build (NOP_EXPR, NULL_TREE, NULL_TREE));
		  (yyval.ttype) = c_end_compound_stmt ((yyvsp[-2].ttype), flag_isoc99); ;}
    break;

  case 491:
#line 2103 "objc/objc-parse.y"
    { c_finish_if_stmt ((yyvsp[-6].location), (yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype), true);
		  add_stmt (c_end_compound_stmt ((yyvsp[-7].ttype), flag_isoc99)); ;}
    break;

  case 492:
#line 2107 "objc/objc-parse.y"
    { c_finish_if_stmt ((yyvsp[-6].location), (yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype), false);
		  add_stmt (c_end_compound_stmt ((yyvsp[-7].ttype), flag_isoc99)); ;}
    break;

  case 493:
#line 2111 "objc/objc-parse.y"
    { c_finish_if_stmt ((yyvsp[-4].location), (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL, true);
		  add_stmt (c_end_compound_stmt ((yyvsp[-5].ttype), flag_isoc99)); ;}
    break;

  case 494:
#line 2115 "objc/objc-parse.y"
    { c_finish_if_stmt ((yyvsp[-4].location), (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL, false);
		  add_stmt (c_end_compound_stmt ((yyvsp[-5].ttype), flag_isoc99)); ;}
    break;

  case 495:
#line 2120 "objc/objc-parse.y"
    { (yyval.ttype) = c_break_label; c_break_label = NULL; ;}
    break;

  case 496:
#line 2124 "objc/objc-parse.y"
    { (yyval.ttype) = c_cont_label; c_cont_label = NULL; ;}
    break;

  case 497:
#line 2130 "objc/objc-parse.y"
    { c_finish_loop ((yyvsp[-6].location), (yyvsp[-4].ttype), NULL, (yyvsp[0].ttype), c_break_label,
				 c_cont_label, true);
		  add_stmt (c_end_compound_stmt ((yyvsp[-7].ttype), flag_isoc99));
		  c_break_label = (yyvsp[-2].ttype); c_cont_label = (yyvsp[-1].ttype); ;}
    break;

  case 498:
#line 2139 "objc/objc-parse.y"
    { (yyval.ttype) = c_break_label; c_break_label = (yyvsp[-3].ttype); ;}
    break;

  case 499:
#line 2140 "objc/objc-parse.y"
    { (yyval.ttype) = c_cont_label; c_cont_label = (yyvsp[-3].ttype); ;}
    break;

  case 500:
#line 2142 "objc/objc-parse.y"
    { c_finish_loop ((yyvsp[-10].location), (yyvsp[-2].ttype), NULL, (yyvsp[-7].ttype), (yyvsp[-5].ttype),
				 (yyvsp[-4].ttype), false);
		  add_stmt (c_end_compound_stmt ((yyvsp[-11].ttype), flag_isoc99)); ;}
    break;

  case 501:
#line 2149 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 502:
#line 2151 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].exprtype).value; ;}
    break;

  case 503:
#line 2156 "objc/objc-parse.y"
    { c_finish_expr_stmt ((yyvsp[-1].ttype)); ;}
    break;

  case 504:
#line 2158 "objc/objc-parse.y"
    { check_for_loop_decls (); ;}
    break;

  case 505:
#line 2162 "objc/objc-parse.y"
    { if ((yyvsp[0].ttype))
		    {
		      (yyval.ttype) = lang_hooks.truthvalue_conversion ((yyvsp[0].ttype));
		      if (EXPR_P ((yyval.ttype)))
			SET_EXPR_LOCATION ((yyval.ttype), (yyvsp[-1].location));
		    }
		  else
		    (yyval.ttype) = NULL;
		;}
    break;

  case 506:
#line 2174 "objc/objc-parse.y"
    { (yyval.ttype) = c_process_expr_stmt ((yyvsp[0].ttype)); ;}
    break;

  case 507:
#line 2181 "objc/objc-parse.y"
    { c_finish_loop ((yyvsp[-7].location), (yyvsp[-6].ttype), (yyvsp[-4].ttype), (yyvsp[0].ttype), c_break_label,
				 c_cont_label, true);
		  add_stmt (c_end_compound_stmt ((yyvsp[-10].ttype), flag_isoc99));
		  c_break_label = (yyvsp[-2].ttype); c_cont_label = (yyvsp[-1].ttype); ;}
    break;

  case 508:
#line 2189 "objc/objc-parse.y"
    { (yyval.ttype) = c_start_case ((yyvsp[-1].exprtype).value); ;}
    break;

  case 509:
#line 2191 "objc/objc-parse.y"
    { c_finish_case ((yyvsp[0].ttype));
		  if (c_break_label)
		    add_stmt (build (LABEL_EXPR, void_type_node,
				     c_break_label));
		  c_break_label = (yyvsp[-1].ttype);
		  add_stmt (c_end_compound_stmt ((yyvsp[-6].ttype), flag_isoc99)); ;}
    break;

  case 510:
#line 2202 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_expr_stmt ((yyvsp[-1].exprtype).value); ;}
    break;

  case 511:
#line 2204 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 512:
#line 2206 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 513:
#line 2208 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 514:
#line 2210 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 515:
#line 2212 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 516:
#line 2214 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_bc_stmt (&c_break_label, true); ;}
    break;

  case 517:
#line 2216 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_bc_stmt (&c_cont_label, false); ;}
    break;

  case 518:
#line 2218 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_return (NULL_TREE); ;}
    break;

  case 519:
#line 2220 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_return ((yyvsp[-1].exprtype).value); ;}
    break;

  case 521:
#line 2223 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_goto_label ((yyvsp[-1].ttype)); ;}
    break;

  case 522:
#line 2225 "objc/objc-parse.y"
    { (yyval.ttype) = c_finish_goto_ptr ((yyvsp[-1].exprtype).value); ;}
    break;

  case 523:
#line 2227 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 524:
#line 2229 "objc/objc-parse.y"
    { (yyval.ttype) = objc_build_throw_stmt ((yyvsp[-1].exprtype).value); ;}
    break;

  case 525:
#line 2231 "objc/objc-parse.y"
    { (yyval.ttype) = objc_build_throw_stmt (NULL_TREE); ;}
    break;

  case 526:
#line 2233 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 527:
#line 2235 "objc/objc-parse.y"
    { objc_build_synchronized ((yyvsp[-4].location), (yyvsp[-2].exprtype).value, (yyvsp[0].ttype)); (yyval.ttype) = NULL_TREE; ;}
    break;

  case 528:
#line 2240 "objc/objc-parse.y"
    { objc_begin_catch_clause (grokparm ((yyvsp[-1].parmtype))); ;}
    break;

  case 529:
#line 2245 "objc/objc-parse.y"
    { objc_finish_catch_clause (); ;}
    break;

  case 530:
#line 2247 "objc/objc-parse.y"
    { objc_finish_catch_clause (); ;}
    break;

  case 533:
#line 2257 "objc/objc-parse.y"
    { objc_begin_try_stmt ((yyvsp[-1].location), (yyvsp[0].ttype)); ;}
    break;

  case 535:
#line 2263 "objc/objc-parse.y"
    { objc_build_finally_clause ((yyvsp[-1].location), (yyvsp[0].ttype)); ;}
    break;

  case 536:
#line 2268 "objc/objc-parse.y"
    { objc_finish_try_stmt (); ;}
    break;

  case 537:
#line 2270 "objc/objc-parse.y"
    { objc_finish_try_stmt (); ;}
    break;

  case 538:
#line 2276 "objc/objc-parse.y"
    { add_stmt ((yyvsp[0].ttype)); (yyval.ttype) = NULL_TREE; ;}
    break;

  case 540:
#line 2285 "objc/objc-parse.y"
    { (yyval.ttype) = do_case ((yyvsp[-1].exprtype).value, NULL_TREE); ;}
    break;

  case 541:
#line 2287 "objc/objc-parse.y"
    { (yyval.ttype) = do_case ((yyvsp[-3].exprtype).value, (yyvsp[-1].exprtype).value); ;}
    break;

  case 542:
#line 2289 "objc/objc-parse.y"
    { (yyval.ttype) = do_case (NULL_TREE, NULL_TREE); ;}
    break;

  case 543:
#line 2291 "objc/objc-parse.y"
    { tree label = define_label ((yyvsp[-2].location), (yyvsp[-3].ttype));
		  if (label)
		    {
		      decl_attributes (&label, (yyvsp[0].ttype), 0);
		      (yyval.ttype) = add_stmt (build_stmt (LABEL_EXPR, label));
		    }
		  else
		    (yyval.ttype) = NULL_TREE;
		;}
    break;

  case 544:
#line 2309 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[-2].ttype); ;}
    break;

  case 545:
#line 2315 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 547:
#line 2322 "objc/objc-parse.y"
    { assemble_asm ((yyvsp[-1].ttype)); ;}
    break;

  case 548:
#line 2324 "objc/objc-parse.y"
    {;}
    break;

  case 549:
#line 2332 "objc/objc-parse.y"
    { (yyval.ttype) = build_asm_stmt ((yyvsp[-6].ttype), (yyvsp[-3].ttype)); ;}
    break;

  case 550:
#line 2338 "objc/objc-parse.y"
    { (yyval.ttype) = build_asm_expr ((yyvsp[0].ttype), 0, 0, 0, true); ;}
    break;

  case 551:
#line 2341 "objc/objc-parse.y"
    { (yyval.ttype) = build_asm_expr ((yyvsp[-2].ttype), (yyvsp[0].ttype), 0, 0, false); ;}
    break;

  case 552:
#line 2344 "objc/objc-parse.y"
    { (yyval.ttype) = build_asm_expr ((yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype), 0, false); ;}
    break;

  case 553:
#line 2347 "objc/objc-parse.y"
    { (yyval.ttype) = build_asm_expr ((yyvsp[-6].ttype), (yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype), false); ;}
    break;

  case 554:
#line 2354 "objc/objc-parse.y"
    { (yyval.ttype) = 0; ;}
    break;

  case 555:
#line 2356 "objc/objc-parse.y"
    { if ((yyvsp[0].ttype) != ridpointers[RID_VOLATILE])
		    {
		      warning ("%E qualifier ignored on asm", (yyvsp[0].ttype));
		      (yyval.ttype) = 0;
		    }
		  else
		    (yyval.ttype) = (yyvsp[0].ttype);
		;}
    break;

  case 556:
#line 2369 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 559:
#line 2376 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[0].ttype)); ;}
    break;

  case 560:
#line 2382 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list (build_tree_list (NULL_TREE, (yyvsp[-5].ttype)),
					(yyvsp[-2].exprtype).value); ;}
    break;

  case 561:
#line 2386 "objc/objc-parse.y"
    { (yyvsp[-7].ttype) = build_string (IDENTIFIER_LENGTH ((yyvsp[-7].ttype)),
				     IDENTIFIER_POINTER ((yyvsp[-7].ttype)));
		  (yyval.ttype) = build_tree_list (build_tree_list ((yyvsp[-7].ttype), (yyvsp[-5].ttype)), (yyvsp[-2].exprtype).value); ;}
    break;

  case 562:
#line 2393 "objc/objc-parse.y"
    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), NULL_TREE); ;}
    break;

  case 563:
#line 2395 "objc/objc-parse.y"
    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-2].ttype)); ;}
    break;

  case 564:
#line 2401 "objc/objc-parse.y"
    { if (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE ((yyvsp[0].ttype))))
		      != char_type_node)
		    {
		      error ("wide string literal in %<asm%>");
		      (yyval.ttype) = build_string (1, "");
		    }
		  else
		    (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 565:
#line 2412 "objc/objc-parse.y"
    { c_lex_string_translate = 0; ;}
    break;

  case 566:
#line 2416 "objc/objc-parse.y"
    { c_lex_string_translate = 1; ;}
    break;

  case 567:
#line 2427 "objc/objc-parse.y"
    { push_scope ();
		  declare_parm_level (); ;}
    break;

  case 568:
#line 2430 "objc/objc-parse.y"
    { (yyval.arginfotype) = (yyvsp[0].arginfotype);
		  pop_scope (); ;}
    break;

  case 570:
#line 2437 "objc/objc-parse.y"
    { mark_forward_parm_decls (); ;}
    break;

  case 571:
#line 2439 "objc/objc-parse.y"
    { /* Dummy action so attributes are in known place
		     on parser stack.  */ ;}
    break;

  case 572:
#line 2442 "objc/objc-parse.y"
    { (yyval.arginfotype) = (yyvsp[0].arginfotype); ;}
    break;

  case 573:
#line 2444 "objc/objc-parse.y"
    { (yyval.arginfotype) = XOBNEW (&parser_obstack, struct c_arg_info);
		  (yyval.arginfotype)->parms = 0;
		  (yyval.arginfotype)->tags = 0;
		  (yyval.arginfotype)->types = 0;
		  (yyval.arginfotype)->others = 0; ;}
    break;

  case 574:
#line 2454 "objc/objc-parse.y"
    { (yyval.arginfotype) = XOBNEW (&parser_obstack, struct c_arg_info);
		  (yyval.arginfotype)->parms = 0;
		  (yyval.arginfotype)->tags = 0;
		  (yyval.arginfotype)->types = 0;
		  (yyval.arginfotype)->others = 0; ;}
    break;

  case 575:
#line 2460 "objc/objc-parse.y"
    { (yyval.arginfotype) = XOBNEW (&parser_obstack, struct c_arg_info);
		  (yyval.arginfotype)->parms = 0;
		  (yyval.arginfotype)->tags = 0;
		  (yyval.arginfotype)->others = 0;
		  /* Suppress -Wold-style-definition for this case.  */
		  (yyval.arginfotype)->types = error_mark_node;
		  error ("ISO C requires a named argument before %<...%>");
		;}
    break;

  case 576:
#line 2469 "objc/objc-parse.y"
    { (yyval.arginfotype) = get_parm_info (/*ellipsis=*/false); ;}
    break;

  case 577:
#line 2471 "objc/objc-parse.y"
    { (yyval.arginfotype) = get_parm_info (/*ellipsis=*/true); ;}
    break;

  case 578:
#line 2476 "objc/objc-parse.y"
    { push_parm_decl ((yyvsp[0].parmtype)); ;}
    break;

  case 579:
#line 2478 "objc/objc-parse.y"
    { push_parm_decl ((yyvsp[0].parmtype)); ;}
    break;

  case 580:
#line 2485 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 581:
#line 2489 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 582:
#line 2493 "objc/objc-parse.y"
    { (yyval.parmtype) = (yyvsp[0].parmtype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 583:
#line 2496 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 584:
#line 2501 "objc/objc-parse.y"
    { (yyval.parmtype) = (yyvsp[0].parmtype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 585:
#line 2509 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 586:
#line 2513 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 587:
#line 2517 "objc/objc-parse.y"
    { (yyval.parmtype) = (yyvsp[0].parmtype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 588:
#line 2520 "objc/objc-parse.y"
    { (yyval.parmtype) = build_c_parm (current_declspecs,
				     chainon ((yyvsp[0].ttype), all_prefix_attributes), (yyvsp[-1].dtrtype));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 589:
#line 2525 "objc/objc-parse.y"
    { (yyval.parmtype) = (yyvsp[0].parmtype);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 590:
#line 2531 "objc/objc-parse.y"
    { prefix_attributes = chainon (prefix_attributes, (yyvsp[-3].ttype));
		  all_prefix_attributes = prefix_attributes; ;}
    break;

  case 591:
#line 2540 "objc/objc-parse.y"
    { push_scope ();
		  declare_parm_level (); ;}
    break;

  case 592:
#line 2543 "objc/objc-parse.y"
    { (yyval.arginfotype) = (yyvsp[0].arginfotype);
		  pop_scope (); ;}
    break;

  case 594:
#line 2550 "objc/objc-parse.y"
    { (yyval.arginfotype) = XOBNEW (&parser_obstack, struct c_arg_info);
		  (yyval.arginfotype)->parms = 0;
		  (yyval.arginfotype)->tags = 0;
		  (yyval.arginfotype)->types = (yyvsp[-1].ttype);
		  (yyval.arginfotype)->others = 0;

		  /* Make sure we have a parmlist after attributes.  */
		  if ((yyvsp[-3].ttype) != 0)
		    YYERROR1;
		;}
    break;

  case 595:
#line 2565 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); ;}
    break;

  case 596:
#line 2567 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); ;}
    break;

  case 597:
#line 2573 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); ;}
    break;

  case 598:
#line 2575 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); ;}
    break;

  case 599:
#line 2580 "objc/objc-parse.y"
    { (yyval.itype) = SAVE_EXT_FLAGS ();
		  pedantic = 0;
		  warn_pointer_arith = 0;
		  warn_traditional = 0;
		  flag_iso = 0; ;}
    break;

  case 605:
#line 2596 "objc/objc-parse.y"
    {
		  objc_finish_implementation ();
		;}
    break;

  case 606:
#line 2604 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); ;}
    break;

  case 607:
#line 2606 "objc/objc-parse.y"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); ;}
    break;

  case 608:
#line 2611 "objc/objc-parse.y"
    {
		  objc_declare_class ((yyvsp[-1].ttype));
		;}
    break;

  case 609:
#line 2618 "objc/objc-parse.y"
    {
		  objc_declare_alias ((yyvsp[-2].ttype), (yyvsp[-1].ttype));
		;}
    break;

  case 610:
#line 2624 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].ttype); ;}
    break;

  case 611:
#line 2625 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 614:
#line 2635 "objc/objc-parse.y"
    {
		  objc_start_class_interface ((yyvsp[-2].ttype), (yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 615:
#line 2639 "objc/objc-parse.y"
    {
		  objc_continue_interface ();
		;}
    break;

  case 616:
#line 2643 "objc/objc-parse.y"
    {
		  objc_finish_interface ();
		;}
    break;

  case 617:
#line 2648 "objc/objc-parse.y"
    {
		  objc_start_class_implementation ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 618:
#line 2652 "objc/objc-parse.y"
    {
		  objc_continue_implementation ();
		;}
    break;

  case 619:
#line 2657 "objc/objc-parse.y"
    {
		  objc_start_category_interface ((yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 620:
#line 2661 "objc/objc-parse.y"
    {
		  objc_finish_interface ();
		;}
    break;

  case 621:
#line 2666 "objc/objc-parse.y"
    {
		  objc_start_category_implementation ((yyvsp[-3].ttype), (yyvsp[-1].ttype));
		;}
    break;

  case 622:
#line 2673 "objc/objc-parse.y"
    {
		  objc_pq_context = 1;
		  objc_start_protocol ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 623:
#line 2678 "objc/objc-parse.y"
    {
		  objc_pq_context = 0;
		  objc_finish_interface ();
		;}
    break;

  case 624:
#line 2686 "objc/objc-parse.y"
    {
		  objc_declare_protocols ((yyvsp[-1].ttype));
		;}
    break;

  case 625:
#line 2693 "objc/objc-parse.y"
    {
		  (yyval.ttype) = NULL_TREE;
		;}
    break;

  case 627:
#line 2701 "objc/objc-parse.y"
    {
		  if ((yyvsp[-2].code) == LT_EXPR && (yyvsp[0].code) == GT_EXPR)
		    (yyval.ttype) = (yyvsp[-1].ttype);
		  else
		    YYERROR1;
		;}
    break;

  case 631:
#line 2716 "objc/objc-parse.y"
    { objc_set_visibility (2); ;}
    break;

  case 632:
#line 2717 "objc/objc-parse.y"
    { objc_set_visibility (0); ;}
    break;

  case 633:
#line 2718 "objc/objc-parse.y"
    { objc_set_visibility (1); ;}
    break;

  case 636:
#line 2725 "objc/objc-parse.y"
    {
		  if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified");
		;}
    break;

  case 637:
#line 2733 "objc/objc-parse.y"
    {
		  /* Comma-separated ivars are chained together in
		     reverse order; add them one by one.  */
		  tree ivar = nreverse ((yyvsp[0].ttype));

		  for (; ivar; ivar = TREE_CHAIN (ivar))
		    objc_add_instance_variable (copy_node (ivar));
		;}
    break;

  case 639:
#line 2746 "objc/objc-parse.y"
    {
		  if (pedantic)
		    pedwarn ("extra semicolon in method definition specified");
		;}
    break;

  case 642:
#line 2759 "objc/objc-parse.y"
    {
		  objc_set_method_type ((yyvsp[0].code));
		  objc_pq_context = 1;
		;}
    break;

  case 643:
#line 2764 "objc/objc-parse.y"
    {
		  objc_pq_context = 0;
		  objc_start_method_definition ((yyvsp[-1].ttype));
		;}
    break;

  case 644:
#line 2769 "objc/objc-parse.y"
    {
		  objc_finish_method_definition (current_function_decl);
		;}
    break;

  case 647:
#line 2781 "objc/objc-parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 651:
#line 2791 "objc/objc-parse.y"
    {
		  objc_set_method_type ((yyvsp[0].code));
		  /* Remember protocol qualifiers in prototypes.  */
		  objc_pq_context = 1;
		;}
    break;

  case 652:
#line 2797 "objc/objc-parse.y"
    {
		  /* Forget protocol qualifiers here.  */
		  objc_pq_context = 0;
		  objc_add_method_declaration ((yyvsp[0].ttype));
		;}
    break;

  case 654:
#line 2807 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_method_signature ((yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE);
		;}
    break;

  case 655:
#line 2812 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_method_signature (NULL_TREE, (yyvsp[0].ttype), NULL_TREE);
		;}
    break;

  case 656:
#line 2817 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_method_signature ((yyvsp[-3].ttype), (yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 657:
#line 2822 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_method_signature (NULL_TREE, (yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 658:
#line 2832 "objc/objc-parse.y"
    {
		  TREE_OVERFLOW ((yyval.ttype)) = (yyvsp[0].itype);
		;}
    break;

  case 659:
#line 2839 "objc/objc-parse.y"
    {
		  (yyval.ttype) = make_node (TREE_LIST);
		;}
    break;

  case 660:
#line 2843 "objc/objc-parse.y"
    {
		  (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE,
						     grokparm ((yyvsp[0].parmtype))));
		;}
    break;

  case 661:
#line 2851 "objc/objc-parse.y"
    {
		  (yyval.itype) = 0;
		;}
    break;

  case 662:
#line 2855 "objc/objc-parse.y"
    {
		  (yyval.itype) = 1;
		;}
    break;

  case 665:
#line 2868 "objc/objc-parse.y"
    {
		  (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 693:
#line 2893 "objc/objc-parse.y"
    {
		  (yyval.ttype) = chainon ((yyvsp[-1].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype)));
		;}
    break;

  case 694:
#line 2897 "objc/objc-parse.y"
    {
		  (yyval.ttype) = NULL_TREE;
		;}
    break;

  case 695:
#line 2904 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), groktypename ((yyvsp[0].typenametype)));
		;}
    break;

  case 696:
#line 2908 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE);
		;}
    break;

  case 697:
#line 2915 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_keyword_decl ((yyvsp[-5].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 698:
#line 2920 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_keyword_decl ((yyvsp[-2].ttype), NULL_TREE, (yyvsp[0].ttype));
		;}
    break;

  case 699:
#line 2925 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_keyword_decl (NULL_TREE, (yyvsp[-2].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 700:
#line 2930 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_build_keyword_decl (NULL_TREE, NULL_TREE, (yyvsp[0].ttype));
		;}
    break;

  case 704:
#line 2943 "objc/objc-parse.y"
    {
		  (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 705:
#line 2951 "objc/objc-parse.y"
    {
		  if (TREE_CHAIN ((yyvsp[0].ttype)) == NULL_TREE)
		    /* just return the expr., remove a level of indirection */
		    (yyval.ttype) = TREE_VALUE ((yyvsp[0].ttype));
                  else
		    /* we have a comma expr., we will collapse later */
		    (yyval.ttype) = (yyvsp[0].ttype);
		;}
    break;

  case 706:
#line 2963 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list ((yyvsp[-2].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 707:
#line 2967 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype));
		;}
    break;

  case 708:
#line 2974 "objc/objc-parse.y"
    { (yyval.ttype) = (yyvsp[0].exprtype).value; ;}
    break;

  case 709:
#line 2976 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_get_class_reference ((yyvsp[0].ttype));
		;}
    break;

  case 710:
#line 2980 "objc/objc-parse.y"
    {
		  (yyval.ttype) = objc_get_class_reference ((yyvsp[0].ttype));
		;}
    break;

  case 711:
#line 2987 "objc/objc-parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[-2].ttype), (yyvsp[-1].ttype)); ;}
    break;

  case 715:
#line 2998 "objc/objc-parse.y"
    {
		  (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		;}
    break;

  case 716:
#line 3005 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), NULL_TREE);
		;}
    break;

  case 717:
#line 3009 "objc/objc-parse.y"
    {
		  (yyval.ttype) = build_tree_list (NULL_TREE, NULL_TREE);
		;}
    break;

  case 718:
#line 3016 "objc/objc-parse.y"
    {
		  (yyval.ttype) = (yyvsp[-1].ttype);
		;}
    break;

  case 719:
#line 3023 "objc/objc-parse.y"
    {
		  (yyval.ttype) = (yyvsp[-1].ttype);
		;}
    break;

  case 720:
#line 3032 "objc/objc-parse.y"
    {
		  (yyval.ttype) = groktypename ((yyvsp[-1].typenametype));
		;}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 6372 "objc/objc-parse.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 yydestruct ("Error: popping",
                             yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 3037 "objc/objc-parse.y"


/* yylex() is a thin wrapper around c_lex(), all it does is translate
   cpplib.h's token codes into yacc's token codes.  */

static enum cpp_ttype last_token;

/* The reserved keyword table.  */
struct resword
{
  const char *word;
  ENUM_BITFIELD(rid) rid : 16;
  unsigned int disable   : 16;
};

/* Disable mask.  Keywords are disabled if (reswords[i].disable & mask) is
   _true_.  */
#define D_C89	0x01	/* not in C89 */
#define D_EXT	0x02	/* GCC extension */
#define D_EXT89	0x04	/* GCC extension incorporated in C99 */
#define D_OBJC	0x08	/* Objective C only */

static const struct resword reswords[] =
{
  { "_Bool",		RID_BOOL,	0 },
  { "_Complex",		RID_COMPLEX,	0 },
  { "__FUNCTION__",	RID_FUNCTION_NAME, 0 },
  { "__PRETTY_FUNCTION__", RID_PRETTY_FUNCTION_NAME, 0 },
  { "__alignof",	RID_ALIGNOF,	0 },
  { "__alignof__",	RID_ALIGNOF,	0 },
  { "__asm",		RID_ASM,	0 },
  { "__asm__",		RID_ASM,	0 },
  { "__attribute",	RID_ATTRIBUTE,	0 },
  { "__attribute__",	RID_ATTRIBUTE,	0 },
  { "__builtin_choose_expr", RID_CHOOSE_EXPR, 0 },
  { "__builtin_offsetof", RID_OFFSETOF, 0 },
  { "__builtin_types_compatible_p", RID_TYPES_COMPATIBLE_P, 0 },
  { "__builtin_va_arg",	RID_VA_ARG,	0 },
  { "__complex",	RID_COMPLEX,	0 },
  { "__complex__",	RID_COMPLEX,	0 },
  { "__const",		RID_CONST,	0 },
  { "__const__",	RID_CONST,	0 },
  { "__extension__",	RID_EXTENSION,	0 },
  { "__func__",		RID_C99_FUNCTION_NAME, 0 },
  { "__imag",		RID_IMAGPART,	0 },
  { "__imag__",		RID_IMAGPART,	0 },
  { "__inline",		RID_INLINE,	0 },
  { "__inline__",	RID_INLINE,	0 },
  { "__label__",	RID_LABEL,	0 },
  { "__real",		RID_REALPART,	0 },
  { "__real__",		RID_REALPART,	0 },
  { "__restrict",	RID_RESTRICT,	0 },
  { "__restrict__",	RID_RESTRICT,	0 },
  { "__signed",		RID_SIGNED,	0 },
  { "__signed__",	RID_SIGNED,	0 },
  { "__thread",		RID_THREAD,	0 },
  { "__typeof",		RID_TYPEOF,	0 },
  { "__typeof__",	RID_TYPEOF,	0 },
  { "__volatile",	RID_VOLATILE,	0 },
  { "__volatile__",	RID_VOLATILE,	0 },
  { "asm",		RID_ASM,	D_EXT },
  { "auto",		RID_AUTO,	0 },
  { "break",		RID_BREAK,	0 },
  { "case",		RID_CASE,	0 },
  { "char",		RID_CHAR,	0 },
  { "const",		RID_CONST,	0 },
  { "continue",		RID_CONTINUE,	0 },
  { "default",		RID_DEFAULT,	0 },
  { "do",		RID_DO,		0 },
  { "double",		RID_DOUBLE,	0 },
  { "else",		RID_ELSE,	0 },
  { "enum",		RID_ENUM,	0 },
  { "extern",		RID_EXTERN,	0 },
  { "float",		RID_FLOAT,	0 },
  { "for",		RID_FOR,	0 },
  { "goto",		RID_GOTO,	0 },
  { "if",		RID_IF,		0 },
  { "inline",		RID_INLINE,	D_EXT89 },
  { "int",		RID_INT,	0 },
  { "long",		RID_LONG,	0 },
  { "register",		RID_REGISTER,	0 },
  { "restrict",		RID_RESTRICT,	D_C89 },
  { "return",		RID_RETURN,	0 },
  { "short",		RID_SHORT,	0 },
  { "signed",		RID_SIGNED,	0 },
  { "sizeof",		RID_SIZEOF,	0 },
  { "static",		RID_STATIC,	0 },
  { "struct",		RID_STRUCT,	0 },
  { "switch",		RID_SWITCH,	0 },
  { "typedef",		RID_TYPEDEF,	0 },
  { "typeof",		RID_TYPEOF,	D_EXT },
  { "union",		RID_UNION,	0 },
  { "unsigned",		RID_UNSIGNED,	0 },
  { "void",		RID_VOID,	0 },
  { "volatile",		RID_VOLATILE,	0 },
  { "while",		RID_WHILE,	0 },


  /* These objc keywords are recognized only immediately after
     an '@'.  */
  { "class",		RID_AT_CLASS,		D_OBJC },
  { "compatibility_alias", RID_AT_ALIAS,	D_OBJC },
  { "defs",		RID_AT_DEFS,		D_OBJC },
  { "encode",		RID_AT_ENCODE,		D_OBJC },
  { "end",		RID_AT_END,		D_OBJC },
  { "implementation",	RID_AT_IMPLEMENTATION,	D_OBJC },
  { "interface",	RID_AT_INTERFACE,	D_OBJC },
  { "private",		RID_AT_PRIVATE,		D_OBJC },
  { "protected",	RID_AT_PROTECTED,	D_OBJC },
  { "protocol",		RID_AT_PROTOCOL,	D_OBJC },
  { "public",		RID_AT_PUBLIC,		D_OBJC },
  { "selector",		RID_AT_SELECTOR,	D_OBJC },
  { "throw",		RID_AT_THROW,		D_OBJC },
  { "try",		RID_AT_TRY,		D_OBJC },
  { "catch",		RID_AT_CATCH,		D_OBJC },
  { "finally",		RID_AT_FINALLY,		D_OBJC },
  { "synchronized",	RID_AT_SYNCHRONIZED,	D_OBJC },
  /* These are recognized only in protocol-qualifier context
     (see above) */
  { "bycopy",		RID_BYCOPY,		D_OBJC },
  { "byref",		RID_BYREF,		D_OBJC },
  { "in",		RID_IN,			D_OBJC },
  { "inout",		RID_INOUT,		D_OBJC },
  { "oneway",		RID_ONEWAY,		D_OBJC },
  { "out",		RID_OUT,		D_OBJC },
};
#define N_reswords (sizeof reswords / sizeof (struct resword))

/* Table mapping from RID_* constants to yacc token numbers.
   Unfortunately we have to have entries for all the keywords in all
   three languages.  */
static const short rid_to_yy[RID_MAX] =
{
  /* RID_STATIC */	STATIC,
  /* RID_UNSIGNED */	TYPESPEC,
  /* RID_LONG */	TYPESPEC,
  /* RID_CONST */	TYPE_QUAL,
  /* RID_EXTERN */	SCSPEC,
  /* RID_REGISTER */	SCSPEC,
  /* RID_TYPEDEF */	SCSPEC,
  /* RID_SHORT */	TYPESPEC,
  /* RID_INLINE */	SCSPEC,
  /* RID_VOLATILE */	TYPE_QUAL,
  /* RID_SIGNED */	TYPESPEC,
  /* RID_AUTO */	SCSPEC,
  /* RID_RESTRICT */	TYPE_QUAL,

  /* C extensions */
  /* RID_COMPLEX */	TYPESPEC,
  /* RID_THREAD */	SCSPEC,

  /* C++ */
  /* RID_FRIEND */	0,
  /* RID_VIRTUAL */	0,
  /* RID_EXPLICIT */	0,
  /* RID_EXPORT */	0,
  /* RID_MUTABLE */	0,

  /* ObjC */
  /* RID_IN */		OBJC_TYPE_QUAL,
  /* RID_OUT */		OBJC_TYPE_QUAL,
  /* RID_INOUT */	OBJC_TYPE_QUAL,
  /* RID_BYCOPY */	OBJC_TYPE_QUAL,
  /* RID_BYREF */	OBJC_TYPE_QUAL,
  /* RID_ONEWAY */	OBJC_TYPE_QUAL,

  /* C */
  /* RID_INT */		TYPESPEC,
  /* RID_CHAR */	TYPESPEC,
  /* RID_FLOAT */	TYPESPEC,
  /* RID_DOUBLE */	TYPESPEC,
  /* RID_VOID */	TYPESPEC,
  /* RID_ENUM */	ENUM,
  /* RID_STRUCT */	STRUCT,
  /* RID_UNION */	UNION,
  /* RID_IF */		IF,
  /* RID_ELSE */	ELSE,
  /* RID_WHILE */	WHILE,
  /* RID_DO */		DO,
  /* RID_FOR */		FOR,
  /* RID_SWITCH */	SWITCH,
  /* RID_CASE */	CASE,
  /* RID_DEFAULT */	DEFAULT,
  /* RID_BREAK */	BREAK,
  /* RID_CONTINUE */	CONTINUE,
  /* RID_RETURN */	RETURN,
  /* RID_GOTO */	GOTO,
  /* RID_SIZEOF */	SIZEOF,

  /* C extensions */
  /* RID_ASM */		ASM_KEYWORD,
  /* RID_TYPEOF */	TYPEOF,
  /* RID_ALIGNOF */	ALIGNOF,
  /* RID_ATTRIBUTE */	ATTRIBUTE,
  /* RID_VA_ARG */	VA_ARG,
  /* RID_EXTENSION */	EXTENSION,
  /* RID_IMAGPART */	IMAGPART,
  /* RID_REALPART */	REALPART,
  /* RID_LABEL */	LABEL,

  /* RID_CHOOSE_EXPR */			CHOOSE_EXPR,
  /* RID_TYPES_COMPATIBLE_P */		TYPES_COMPATIBLE_P,

  /* RID_FUNCTION_NAME */		FUNC_NAME,
  /* RID_PRETTY_FUNCTION_NAME */	FUNC_NAME,
  /* RID_C99_FUNCTION_NAME */		FUNC_NAME,

  /* C++ */
  /* RID_BOOL */	TYPESPEC,
  /* RID_WCHAR */	0,
  /* RID_CLASS */	0,
  /* RID_PUBLIC */	0,
  /* RID_PRIVATE */	0,
  /* RID_PROTECTED */	0,
  /* RID_TEMPLATE */	0,
  /* RID_NULL */	0,
  /* RID_CATCH */	0,
  /* RID_DELETE */	0,
  /* RID_FALSE */	0,
  /* RID_NAMESPACE */	0,
  /* RID_NEW */		0,
  /* RID_OFFSETOF */    OFFSETOF,
  /* RID_OPERATOR */	0,
  /* RID_THIS */	0,
  /* RID_THROW */	0,
  /* RID_TRUE */	0,
  /* RID_TRY */		0,
  /* RID_TYPENAME */	0,
  /* RID_TYPEID */	0,
  /* RID_USING */	0,

  /* casts */
  /* RID_CONSTCAST */	0,
  /* RID_DYNCAST */	0,
  /* RID_REINTCAST */	0,
  /* RID_STATCAST */	0,

  /* Objective C */
  /* RID_AT_ENCODE */		AT_ENCODE,
  /* RID_AT_END */		AT_END,
  /* RID_AT_CLASS */		AT_CLASS,
  /* RID_AT_ALIAS */		AT_ALIAS,
  /* RID_AT_DEFS */		AT_DEFS,
  /* RID_AT_PRIVATE */		AT_PRIVATE,
  /* RID_AT_PROTECTED */	AT_PROTECTED,
  /* RID_AT_PUBLIC */		AT_PUBLIC,
  /* RID_AT_PROTOCOL */		AT_PROTOCOL,
  /* RID_AT_SELECTOR */		AT_SELECTOR,
  /* RID_AT_THROW */		AT_THROW,
  /* RID_AT_TRY */		AT_TRY,
  /* RID_AT_CATCH */		AT_CATCH,
  /* RID_AT_FINALLY */		AT_FINALLY,
  /* RID_AT_SYNCHRONIZED */	AT_SYNCHRONIZED,
  /* RID_AT_INTERFACE */	AT_INTERFACE,
  /* RID_AT_IMPLEMENTATION */	AT_IMPLEMENTATION
};

static void
init_reswords (void)
{
  unsigned int i;
  tree id;
  int mask = (flag_isoc99 ? 0 : D_C89)
	      | (flag_no_asm ? (flag_isoc99 ? D_EXT : D_EXT|D_EXT89) : 0);

  if (!c_dialect_objc ())
     mask |= D_OBJC;

  ridpointers = GGC_CNEWVEC (tree, (int) RID_MAX);
  for (i = 0; i < N_reswords; i++)
    {
      /* If a keyword is disabled, do not enter it into the table
	 and so create a canonical spelling that isn't a keyword.  */
      if (reswords[i].disable & mask)
	continue;

      id = get_identifier (reswords[i].word);
      C_RID_CODE (id) = reswords[i].rid;
      C_IS_RESERVED_WORD (id) = 1;
      ridpointers [(int) reswords[i].rid] = id;
    }
}

#define NAME(type) cpp_type2name (type)

static void
yyerror (const char *msgid)
{
  c_parse_error (msgid, last_token, yylval.ttype);
}

static int
yylexname (void)
{
  tree decl;

  int objc_force_identifier = objc_need_raw_identifier;
  OBJC_NEED_RAW_IDENTIFIER (0);

  if (C_IS_RESERVED_WORD (yylval.ttype))
    {
      enum rid rid_code = C_RID_CODE (yylval.ttype);

      if (!OBJC_IS_AT_KEYWORD (rid_code)
	  && (!OBJC_IS_PQ_KEYWORD (rid_code) || objc_pq_context))
      {
	/* Return the canonical spelling for this keyword.  */
	yylval.ttype = ridpointers[(int) rid_code];
	return rid_to_yy[(int) rid_code];
      }
    }

  decl = lookup_name (yylval.ttype);
  if (decl)
    {
      if (TREE_CODE (decl) == TYPE_DECL)
	return TYPENAME;
    }
  else
    {
      tree objc_interface_decl = objc_is_class_name (yylval.ttype);
      /* ObjC class names are in the same namespace as variables and
	 typedefs, and hence are shadowed by local declarations.  */
      if (objc_interface_decl
	  && (global_bindings_p ()
	      || (!objc_force_identifier && !decl)))
	{
	  yylval.ttype = objc_interface_decl;
	  return CLASSNAME;
	}
    }

  return IDENTIFIER;
}

static inline int
_yylex (void)
{
 get_next:
  last_token = c_lex (&yylval.ttype);
  switch (last_token)
    {
    case CPP_EQ:					return '=';
    case CPP_NOT:					return '!';
    case CPP_GREATER:	yylval.code = GT_EXPR;		return ARITHCOMPARE;
    case CPP_LESS:	yylval.code = LT_EXPR;		return ARITHCOMPARE;
    case CPP_PLUS:	yylval.code = PLUS_EXPR;	return '+';
    case CPP_MINUS:	yylval.code = MINUS_EXPR;	return '-';
    case CPP_MULT:	yylval.code = MULT_EXPR;	return '*';
    case CPP_DIV:	yylval.code = TRUNC_DIV_EXPR;	return '/';
    case CPP_MOD:	yylval.code = TRUNC_MOD_EXPR;	return '%';
    case CPP_AND:	yylval.code = BIT_AND_EXPR;	return '&';
    case CPP_OR:	yylval.code = BIT_IOR_EXPR;	return '|';
    case CPP_XOR:	yylval.code = BIT_XOR_EXPR;	return '^';
    case CPP_RSHIFT:	yylval.code = RSHIFT_EXPR;	return RSHIFT;
    case CPP_LSHIFT:	yylval.code = LSHIFT_EXPR;	return LSHIFT;

    case CPP_COMPL:					return '~';
    case CPP_AND_AND:					return ANDAND;
    case CPP_OR_OR:					return OROR;
    case CPP_QUERY:					return '?';
    case CPP_OPEN_PAREN:				return '(';
    case CPP_EQ_EQ:	yylval.code = EQ_EXPR;		return EQCOMPARE;
    case CPP_NOT_EQ:	yylval.code = NE_EXPR;		return EQCOMPARE;
    case CPP_GREATER_EQ:yylval.code = GE_EXPR;		return ARITHCOMPARE;
    case CPP_LESS_EQ:	yylval.code = LE_EXPR;		return ARITHCOMPARE;

    case CPP_PLUS_EQ:	yylval.code = PLUS_EXPR;	return ASSIGN;
    case CPP_MINUS_EQ:	yylval.code = MINUS_EXPR;	return ASSIGN;
    case CPP_MULT_EQ:	yylval.code = MULT_EXPR;	return ASSIGN;
    case CPP_DIV_EQ:	yylval.code = TRUNC_DIV_EXPR;	return ASSIGN;
    case CPP_MOD_EQ:	yylval.code = TRUNC_MOD_EXPR;	return ASSIGN;
    case CPP_AND_EQ:	yylval.code = BIT_AND_EXPR;	return ASSIGN;
    case CPP_OR_EQ:	yylval.code = BIT_IOR_EXPR;	return ASSIGN;
    case CPP_XOR_EQ:	yylval.code = BIT_XOR_EXPR;	return ASSIGN;
    case CPP_RSHIFT_EQ:	yylval.code = RSHIFT_EXPR;	return ASSIGN;
    case CPP_LSHIFT_EQ:	yylval.code = LSHIFT_EXPR;	return ASSIGN;

    case CPP_OPEN_SQUARE:				return '[';
    case CPP_CLOSE_SQUARE:				return ']';
    case CPP_OPEN_BRACE:				return '{';
    case CPP_CLOSE_BRACE:				return '}';
    case CPP_ELLIPSIS:					return ELLIPSIS;

    case CPP_PLUS_PLUS:					return PLUSPLUS;
    case CPP_MINUS_MINUS:				return MINUSMINUS;
    case CPP_DEREF:					return POINTSAT;
    case CPP_DOT:					return '.';

      /* The following tokens may affect the interpretation of any
	 identifiers following, if doing Objective-C.  */
    case CPP_COLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ':';
    case CPP_COMMA:		OBJC_NEED_RAW_IDENTIFIER (0);	return ',';
    case CPP_CLOSE_PAREN:	OBJC_NEED_RAW_IDENTIFIER (0);	return ')';
    case CPP_SEMICOLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ';';

    case CPP_EOF:
      return 0;

    case CPP_NAME:
      return yylexname ();

    case CPP_AT_NAME:
      /* This only happens in Objective-C; it must be a keyword.  */
      return rid_to_yy [(int) C_RID_CODE (yylval.ttype)];

    case CPP_NUMBER:
    case CPP_CHAR:
    case CPP_WCHAR:
      return CONSTANT;

    case CPP_STRING:
    case CPP_WSTRING:
      return STRING;

    case CPP_OBJC_STRING:
      return OBJC_STRING;

      /* These tokens are C++ specific (and will not be generated
         in C mode, but let's be cautious).  */
    case CPP_SCOPE:
    case CPP_DEREF_STAR:
    case CPP_DOT_STAR:
    case CPP_MIN_EQ:
    case CPP_MAX_EQ:
    case CPP_MIN:
    case CPP_MAX:
      /* These tokens should not survive translation phase 4.  */
    case CPP_HASH:
    case CPP_PASTE:
      error ("syntax error at %qs token", NAME(last_token));
      goto get_next;

    default:
      abort ();
    }
  /* NOTREACHED */
}

static int
yylex (void)
{
  int r;
  timevar_push (TV_LEX);
  r = _yylex();
  timevar_pop (TV_LEX);
  return r;
}

/* Function used when yydebug is set, to print a token in more detail.  */

static void
yyprint (FILE *file, int yychar, YYSTYPE yyl)
{
  tree t = yyl.ttype;

  fprintf (file, " [%s]", NAME(last_token));

  switch (yychar)
    {
    case IDENTIFIER:
    case TYPENAME:
    case TYPESPEC:
    case TYPE_QUAL:
    case SCSPEC:
    case STATIC:
      if (IDENTIFIER_POINTER (t))
	fprintf (file, " '%s'", IDENTIFIER_POINTER (t));
      break;

    case CONSTANT:
      fprintf (file, " %s", GET_MODE_NAME (TYPE_MODE (TREE_TYPE (t))));
      if (TREE_CODE (t) == INTEGER_CST)
	{
	  fputs (" ", file);
	  fprintf (file, HOST_WIDE_INT_PRINT_DOUBLE_HEX,
		   TREE_INT_CST_HIGH (t), TREE_INT_CST_LOW (t));
	}
      break;
    }
}

/* This is not the ideal place to put this, but we have to get it out
   of c-lex.c because cp/lex.c has its own version.  */

/* Parse the file.  */
void
c_parse_file (void)
{
  yyparse ();

  if (malloced_yyss)
    {
      free (malloced_yyss);
      free (malloced_yyvs);
      malloced_yyss = 0;
    }
}

#ifdef __XGETTEXT__
/* Depending on the version of Bison used to compile this grammar,
   it may issue generic diagnostics spelled "syntax error" or
   "parse error".  To prevent this from changing the translation
   template randomly, we list all the variants of this particular
   diagnostic here.  Translators: there is no fine distinction
   between diagnostics with "syntax error" in them, and diagnostics
   with "parse error" in them.  It's okay to give them both the same
   translation.  */
const char d1[] = N_("syntax error");
const char d2[] = N_("parse error");
const char d3[] = N_("syntax error; also virtual memory exhausted");
const char d4[] = N_("parse error; also virtual memory exhausted");
const char d5[] = N_("syntax error: cannot back up");
const char d6[] = N_("parse error: cannot back up");
#endif

#include "gt-c-parse.h"


