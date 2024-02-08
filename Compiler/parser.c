/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

        #include <stdio.h>
        //#include "lex.yy.h" // alphayylex?
        #include "table_k_quads/Symbol_Table.h"
        #include "table_k_quads/quads.h"
        #include "vm.h"

        #define TXT_FILE "maou.txt"
        #define BINARY_FILE "binary.abc"

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);
        
        extern unsigned scopeSpaceCounter;
        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
        extern int CURR_SCOPE;
        extern unsigned functionLocalOffset;
        extern var_table* table ;
        extern int currQuad;
        var *curr = NULL;
        struct Stack * save_fuctionlocals;
        struct Stack * loopcounterStack;

        int curr_anonymous = 0; //keep track for anonymous
        int loopcounter =0;
        int error_flag = 0;     // check if there were errors
        int fuctioncounter = 0;

        /*function for check ids and insert to STable*/
        var *insert_ID(char* name){
                //kanoume lookup apo mesa pros ta eksw
                var *myvar = lookup_in_out(CURR_SCOPE,name);
                var_id curr_id = local;
                //An den vrethei tpt kanthn insert sto Curr_scope
                if(myvar == NULL){
                        if(CURR_SCOPE == 0) // an einai global
                                curr_id = global;
                        myvar =new_var(varr,curr_id,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                        hash_insert(myvar,table);
                        //print_var(myvar);
                        inccurrscopeoffset();              
                } 
                /*an yparxei hdh */
                else{ 
                        //aneferomaste kai menei na doume an exoume prosvash
                        //printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name); //TESTING PRINT
                        //check if we have access
                        if(check_access(myvar) == 0 && myvar->scope != 0){
                                yyerror("Cannot access var ");
                        }   
                } // einai hdh sto table
                return myvar;
        }

        /*check if global variable exist p.x. ::x (global x)*/
        var * check_global(char *name){
                //kanoume lookup sto global scope 0
                var *myvar = lookup_scope(0,name);
                //an einai NULL den iparxei
                if (myvar == NULL ) {
                        yyerror("Global var not found");
                        return NULL;
                }
                //alliws anaferomaste ekei
                //printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name);  //TESTING PRINT
                return myvar;
        }

        /*function to insert formal variables*/
        void insert_formal(char* name){
                var *myvar = lookup_scope(CURR_SCOPE,name);
                /*an yparxei sto idio scope error p.x. fuction (x,y,x) */
                if(myvar != NULL){
                        yyerror("Already defined ");
                        return;
                }
                /*an exei collision me libfuction error */
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                /*alliws insert sto syble table san formal */
                myvar = new_var(varr,formal,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                inccurrscopeoffset();
                hash_insert(myvar,table);
                //print_var(myvar);
        }

        /*insert a new fuction to the table*/
        var* function_insert(char* name){
                //kanoume lookuop sto trexon scope
                var* myfuction = lookup_scope(CURR_SCOPE,name);
                
                // an yparxei collision me lib_fuct einai error
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return myfuction;
                }
                //an vrethei metavlhth h synarthsh einai error
                if(myfuction != NULL){ 
                        yyerror("Already defined");
                        return myfuction;
                }
                //alliws thn kanoume insert
                myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                myfuction->fuctionAddress = 0; //kati
                myfuction->totalLocals = 0;//pali kati
                hash_insert(myfuction,table);
                return myfuction;
                //print_var(myfuction);
        }

        /*Insert a local var with name = name */
        var* insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        //printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
                        if(currVar->type == fuction)
                                printf("Warning line 124 parser.y");
                        return currVar;
                }
                /*tsekare an yparxoun collisions me lib fuction*/
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return currVar;
                }
                /*an eimaste sto scope 0 tote thn kanoume insert san global*/
                if(CURR_SCOPE == 0)
                        curr_id = global;
                        //printf("GT %d\n",currVar->hide);
                currVar = new_var(varr,curr_id,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                hash_insert(currVar,table);
                //print_var(currVar);
                inccurrscopeoffset();
                return currVar;         
        }

        /*check if the curr string is '_' to create the next anonymous fuction*/
        char *newemptyfuncname(){
                char* str = malloc(sizeof(char) * 30);
		sprintf(str,"_f%d",curr_anonymous++);
		return str;
        }

#line 221 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_WHILE = 5,                      /* WHILE  */
  YYSYMBOL_FOR = 6,                        /* FOR  */
  YYSYMBOL_FUNCTION = 7,                   /* FUNCTION  */
  YYSYMBOL_RETURN = 8,                     /* RETURN  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 10,                  /* CONTINUE  */
  YYSYMBOL_AND = 11,                       /* AND  */
  YYSYMBOL_NOT = 12,                       /* NOT  */
  YYSYMBOL_OR = 13,                        /* OR  */
  YYSYMBOL_LOCAL = 14,                     /* LOCAL  */
  YYSYMBOL_NILL = 15,                      /* NILL  */
  YYSYMBOL_ASSIGNMENT = 16,                /* ASSIGNMENT  */
  YYSYMBOL_ADDITION = 17,                  /* ADDITION  */
  YYSYMBOL_SUBTRACTION = 18,               /* SUBTRACTION  */
  YYSYMBOL_MULTI = 19,                     /* MULTI  */
  YYSYMBOL_DIVISION = 20,                  /* DIVISION  */
  YYSYMBOL_MODULUS = 21,                   /* MODULUS  */
  YYSYMBOL_UMINUS = 22,                    /* UMINUS  */
  YYSYMBOL_EQUAL = 23,                     /* EQUAL  */
  YYSYMBOL_NOTEQUAL = 24,                  /* NOTEQUAL  */
  YYSYMBOL_INCREMENT = 25,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 26,                 /* DECREMENT  */
  YYSYMBOL_GRETER_THAN = 27,               /* GRETER_THAN  */
  YYSYMBOL_LESS_THAN = 28,                 /* LESS_THAN  */
  YYSYMBOL_GRE_EQUAL = 29,                 /* GRE_EQUAL  */
  YYSYMBOL_LES_EQUAL = 30,                 /* LES_EQUAL  */
  YYSYMBOL_INTEGER = 31,                   /* INTEGER  */
  YYSYMBOL_REAL = 32,                      /* REAL  */
  YYSYMBOL_STRING = 33,                    /* STRING  */
  YYSYMBOL_ID = 34,                        /* ID  */
  YYSYMBOL_TRUE = 35,                      /* TRUE  */
  YYSYMBOL_FALSE = 36,                     /* FALSE  */
  YYSYMBOL_LEFT_CURLY_BRACKET = 37,        /* LEFT_CURLY_BRACKET  */
  YYSYMBOL_RIGHT_CURLY_BRACKET = 38,       /* RIGHT_CURLY_BRACKET  */
  YYSYMBOL_LEFT_SQUARE_BRACKET = 39,       /* LEFT_SQUARE_BRACKET  */
  YYSYMBOL_RIGHT_SQUARE_BRACKET = 40,      /* RIGHT_SQUARE_BRACKET  */
  YYSYMBOL_LEFT_PARENTHESIS = 41,          /* LEFT_PARENTHESIS  */
  YYSYMBOL_RIGHT_PARENTHESIS = 42,         /* RIGHT_PARENTHESIS  */
  YYSYMBOL_SEMICOLON = 43,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 44,                     /* COMMA  */
  YYSYMBOL_SCOPE_RESOLUTION = 45,          /* SCOPE_RESOLUTION  */
  YYSYMBOL_COLON = 46,                     /* COLON  */
  YYSYMBOL_FULL_STOP = 47,                 /* FULL_STOP  */
  YYSYMBOL_DOUBLE_FULL_STOP = 48,          /* DOUBLE_FULL_STOP  */
  YYSYMBOL_EXTRA_CHARS = 49,               /* EXTRA_CHARS  */
  YYSYMBOL_LINE_COMMENT = 50,              /* LINE_COMMENT  */
  YYSYMBOL_BLOCK_COMMENT = 51,             /* BLOCK_COMMENT  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_program = 53,                   /* program  */
  YYSYMBOL_stmts = 54,                     /* stmts  */
  YYSYMBOL_brk_stm = 55,                   /* brk_stm  */
  YYSYMBOL_cnt_stm = 56,                   /* cnt_stm  */
  YYSYMBOL_stmt = 57,                      /* stmt  */
  YYSYMBOL_expr = 58,                      /* expr  */
  YYSYMBOL_term = 59,                      /* term  */
  YYSYMBOL_assignexpr = 60,                /* assignexpr  */
  YYSYMBOL_primary = 61,                   /* primary  */
  YYSYMBOL_lvalue = 62,                    /* lvalue  */
  YYSYMBOL_member = 63,                    /* member  */
  YYSYMBOL_call = 64,                      /* call  */
  YYSYMBOL_callsuffix = 65,                /* callsuffix  */
  YYSYMBOL_normcall = 66,                  /* normcall  */
  YYSYMBOL_methodcall = 67,                /* methodcall  */
  YYSYMBOL_elist = 68,                     /* elist  */
  YYSYMBOL_moreElist = 69,                 /* moreElist  */
  YYSYMBOL_objectdef = 70,                 /* objectdef  */
  YYSYMBOL_indexed = 71,                   /* indexed  */
  YYSYMBOL_moreindexedelem = 72,           /* moreindexedelem  */
  YYSYMBOL_indexedelem = 73,               /* indexedelem  */
  YYSYMBOL_block = 74,                     /* block  */
  YYSYMBOL_75_1 = 75,                      /* $@1  */
  YYSYMBOL_funcname = 76,                  /* funcname  */
  YYSYMBOL_funcprefix = 77,                /* funcprefix  */
  YYSYMBOL_funcargs = 78,                  /* funcargs  */
  YYSYMBOL_funcbody = 79,                  /* funcbody  */
  YYSYMBOL_funcblockstart = 80,            /* funcblockstart  */
  YYSYMBOL_funcblockend = 81,              /* funcblockend  */
  YYSYMBOL_funcdef = 82,                   /* funcdef  */
  YYSYMBOL_83_2 = 83,                      /* $@2  */
  YYSYMBOL_const = 84,                     /* const  */
  YYSYMBOL_idlist = 85,                    /* idlist  */
  YYSYMBOL_moreidilist = 86,               /* moreidilist  */
  YYSYMBOL_ifprefix = 87,                  /* ifprefix  */
  YYSYMBOL_elseprefix = 88,                /* elseprefix  */
  YYSYMBOL_ifstmt = 89,                    /* ifstmt  */
  YYSYMBOL_loopstart = 90,                 /* loopstart  */
  YYSYMBOL_loopend = 91,                   /* loopend  */
  YYSYMBOL_loopstmt = 92,                  /* loopstmt  */
  YYSYMBOL_whilestart = 93,                /* whilestart  */
  YYSYMBOL_whilecond = 94,                 /* whilecond  */
  YYSYMBOL_whilestmt = 95,                 /* whilestmt  */
  YYSYMBOL_N = 96,                         /* N  */
  YYSYMBOL_M = 97,                         /* M  */
  YYSYMBOL_forprefix = 98,                 /* forprefix  */
  YYSYMBOL_forstmt = 99,                   /* forstmt  */
  YYSYMBOL_returnstart = 100,              /* returnstart  */
  YYSYMBOL_returnstmt = 101                /* returnstmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   722

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  199

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   229,   229,   232,   236,   242,   246,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     280,   286,   289,   290,   297,   303,   316,   330,   343,   357,
     360,   383,   384,   385,   386,   390,   393,   394,   395,   396,
     399,   401,   408,   409,   412,   415,   417,   433,   438,   446,
     447,   450,   454,   461,   464,   469,   472,   473,   481,   485,
     495,   507,   511,   512,   519,   526,   526,   534,   543,   544,
     548,   562,   567,   575,   583,   587,   591,   591,   603,   604,
     605,   606,   607,   608,   611,   612,   615,   616,   620,   626,
     632,   636,   653,   654,   657,   660,   663,   671,   680,   681,
     682,   689,   700,   704,   715
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "WHILE",
  "FOR", "FUNCTION", "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR",
  "LOCAL", "NILL", "ASSIGNMENT", "ADDITION", "SUBTRACTION", "MULTI",
  "DIVISION", "MODULUS", "UMINUS", "EQUAL", "NOTEQUAL", "INCREMENT",
  "DECREMENT", "GRETER_THAN", "LESS_THAN", "GRE_EQUAL", "LES_EQUAL",
  "INTEGER", "REAL", "STRING", "ID", "TRUE", "FALSE", "LEFT_CURLY_BRACKET",
  "RIGHT_CURLY_BRACKET", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET",
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA",
  "SCOPE_RESOLUTION", "COLON", "FULL_STOP", "DOUBLE_FULL_STOP",
  "EXTRA_CHARS", "LINE_COMMENT", "BLOCK_COMMENT", "$accept", "program",
  "stmts", "brk_stm", "cnt_stm", "stmt", "expr", "term", "assignexpr",
  "primary", "lvalue", "member", "call", "callsuffix", "normcall",
  "methodcall", "elist", "moreElist", "objectdef", "indexed",
  "moreindexedelem", "indexedelem", "block", "$@1", "funcname",
  "funcprefix", "funcargs", "funcbody", "funcblockstart", "funcblockend",
  "funcdef", "$@2", "const", "idlist", "moreidilist", "ifprefix",
  "elseprefix", "ifstmt", "loopstart", "loopend", "loopstmt", "whilestart",
  "whilecond", "whilestmt", "N", "M", "forprefix", "forstmt",
  "returnstart", "returnstmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-169)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     174,   -16,  -169,     6,   -14,  -169,   -29,    17,   414,     7,
    -169,   414,    21,    21,  -169,  -169,  -169,  -169,  -169,  -169,
      18,   222,    -2,  -169,    29,   100,   174,  -169,  -169,  -169,
     443,  -169,  -169,  -169,    20,  -169,   -20,  -169,  -169,  -169,
    -169,  -169,   174,  -169,    65,  -169,  -169,  -169,   254,  -169,
     414,   414,  -169,  -169,  -169,  -169,  -169,  -169,  -169,   101,
      57,   -20,    57,  -169,   174,   414,  -169,   636,  -169,    13,
      69,    66,  -169,   506,    70,  -169,  -169,  -169,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,  -169,   414,  -169,  -169,   414,   286,    77,    79,  -169,
    -169,  -169,   414,   318,    80,    75,   114,   414,  -169,   414,
    -169,   464,   526,   -40,    82,   129,   203,  -169,   414,  -169,
      84,  -169,    78,   664,   650,    50,    50,  -169,  -169,  -169,
     678,   678,   692,   692,   692,   692,   636,   566,  -169,    30,
    -169,    85,   590,  -169,    31,  -169,   -27,  -169,  -169,   174,
     546,   174,  -169,    86,  -169,  -169,  -169,    78,  -169,   414,
    -169,  -169,   350,  -169,  -169,   382,  -169,  -169,  -169,  -169,
      88,  -169,    10,    92,  -169,  -169,  -169,  -169,   414,   614,
    -169,    55,  -169,    59,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,   485,  -169,  -169,  -169,  -169,  -169,  -169,  -169
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,   105,     0,    79,   112,     0,     0,     0,     0,
      91,     0,     0,     0,    88,    89,    90,    47,    92,    93,
      75,     0,     0,    16,     0,     0,     2,    12,    13,     4,
       0,    31,    17,    39,    41,    46,    42,    43,    14,    86,
      15,    45,     0,     8,     0,     9,   108,    10,     0,    11,
       0,     0,    78,    80,     5,     6,    34,    48,    33,     0,
      35,     0,    37,    77,     0,     0,    68,    65,    66,     0,
       0,    71,    72,     0,     0,    49,     1,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,    36,    38,     0,     0,     0,     0,    56,
      59,    60,     0,     0,     0,     0,   100,     0,   102,     0,
     113,     0,     0,     0,     0,     0,     0,    69,     0,    70,
       0,    32,    44,    29,    30,    18,    19,    20,    21,    22,
      27,    28,    23,    25,    24,    26,    40,     0,    62,     0,
      50,     0,     0,    54,     0,    52,     0,    84,    99,     0,
       0,     0,   107,     0,   114,    98,   109,     0,    76,     0,
      67,    73,     0,    51,    61,     0,    53,    55,    94,    81,
       0,    96,     0,     0,   101,   106,   103,   108,     0,     0,
      57,     0,    63,     0,    95,    82,    97,    83,    85,   104,
     102,     0,    74,    58,    64,    87,   108,   110,   111
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,    67,  -169,  -169,   -24,     0,  -169,  -169,  -169,
      64,  -169,    81,  -169,  -169,  -169,   -69,   -45,  -169,  -169,
    -169,     3,   -43,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
     -21,  -169,  -169,   -39,  -169,  -169,  -169,  -169,  -169,  -169,
     -50,  -169,  -169,  -169,  -168,  -169,  -169,  -169,  -169,  -169
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    25,    26,    27,    28,    29,    67,    31,    32,    33,
      34,    35,    36,    99,   100,   101,    68,    69,    37,    70,
      71,    72,    38,    64,    53,    39,   147,   188,   173,   195,
      40,   105,    41,   171,   172,    42,   149,    43,   151,   189,
     152,    44,   108,    45,   109,   178,    46,    47,    48,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      30,    74,    77,   156,   118,     4,   113,   168,    56,   190,
       8,    58,     9,    10,    54,   169,    11,   170,   106,   102,
      52,   103,    73,    12,    13,    50,    30,   104,   198,    14,
      15,    16,    17,    18,    19,     9,    92,    21,   114,    22,
     153,    57,    30,    24,   168,    93,    94,    51,   111,   160,
     112,   139,   185,   117,   170,    17,    63,   118,   144,    95,
      55,    96,    59,    75,    30,   116,    24,    97,    98,    82,
      83,    84,   164,   167,   118,   118,    60,    62,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    77,   136,    61,    61,   137,    95,   193,    96,   118,
      76,   194,   142,   118,    97,    98,   107,   150,     4,   119,
     120,   140,   122,   141,   145,    30,   146,   181,   148,   162,
     183,    65,   184,   161,   157,   174,   165,   176,   177,    20,
     187,   115,     1,   186,     2,     3,     4,     5,     6,     7,
     196,     8,     0,     9,    10,     0,     0,    11,     0,    30,
       0,    30,     0,     0,    12,    13,     0,     0,     0,   179,
      14,    15,    16,    17,    18,    19,    20,   158,    21,     0,
      22,     0,    23,     0,    24,     0,     0,     1,   191,     2,
       3,     4,     5,     6,     7,     0,     8,     0,     9,    10,
       0,     0,    11,     0,     0,     0,     0,     0,     0,    12,
      13,     0,     0,     0,     0,    14,    15,    16,    17,    18,
      19,    20,     0,    21,    78,    22,    79,    23,     0,    24,
      80,    81,    82,    83,    84,     0,    85,    86,     0,     0,
      87,    88,    89,    90,     8,     0,     9,    10,     0,     0,
      11,     0,     0,     0,     0,     0,     0,    12,    13,   159,
       0,     0,     0,    14,    15,    16,    17,    18,    19,    65,
       0,    21,    66,    22,     0,     0,     8,    24,     9,    10,
       0,     0,    11,     0,     0,     0,     0,     0,     0,    12,
      13,     0,     0,     0,     0,    14,    15,    16,    17,    18,
      19,     0,     0,    21,     0,    22,     0,   110,     8,    24,
       9,    10,     0,     0,    11,     0,     0,     0,     0,     0,
       0,    12,    13,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,     0,     0,    21,     0,    22,   138,     0,
       8,    24,     9,    10,     0,     0,    11,     0,     0,     0,
       0,     0,     0,    12,    13,     0,     0,     0,     0,    14,
      15,    16,    17,    18,    19,     0,     0,    21,     0,    22,
     143,     0,     8,    24,     9,    10,     0,     0,    11,     0,
       0,     0,     0,     0,     0,    12,    13,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,     0,     0,    21,
       0,    22,   180,     0,     8,    24,     9,    10,     0,     0,
      11,     0,     0,     0,     0,     0,     0,    12,    13,     0,
       0,     0,     0,    14,    15,    16,    17,    18,    19,     0,
       0,    21,     0,    22,   182,     0,     8,    24,     9,    10,
       0,     0,    11,     0,     0,     0,     0,     0,     0,    12,
      13,     0,     0,     0,     0,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    78,    22,    79,     0,     0,    24,
      80,    81,    82,    83,    84,     0,    85,    86,     0,     0,
      87,    88,    89,    90,     0,    78,     0,    79,     0,     0,
       0,    80,    81,    82,    83,    84,    91,    85,    86,     0,
       0,    87,    88,    89,    90,     0,    78,     0,    79,     0,
       0,     0,    80,    81,    82,    83,    84,   154,    85,    86,
       0,     0,    87,    88,    89,    90,     0,    78,     0,    79,
       0,     0,     0,    80,    81,    82,    83,    84,   197,    85,
      86,     0,     0,    87,    88,    89,    90,    78,     0,    79,
       0,     0,     0,    80,    81,    82,    83,    84,   121,    85,
      86,     0,     0,    87,    88,    89,    90,    78,     0,    79,
       0,     0,     0,    80,    81,    82,    83,    84,   155,    85,
      86,     0,     0,    87,    88,    89,    90,    78,     0,    79,
       0,     0,     0,    80,    81,    82,    83,    84,   175,    85,
      86,     0,     0,    87,    88,    89,    90,     0,     0,     0,
       0,    78,     0,    79,     0,     0,   163,    80,    81,    82,
      83,    84,     0,    85,    86,     0,     0,    87,    88,    89,
      90,     0,     0,     0,     0,    78,     0,    79,     0,     0,
     166,    80,    81,    82,    83,    84,     0,    85,    86,     0,
       0,    87,    88,    89,    90,     0,     0,    78,     0,    79,
       0,     0,   192,    80,    81,    82,    83,    84,     0,    85,
      86,    78,     0,    87,    88,    89,    90,    80,    81,    82,
      83,    84,     0,    85,    86,     0,     0,    87,    88,    89,
      90,    80,    81,    82,    83,    84,     0,    85,    86,     0,
       0,    87,    88,    89,    90,    80,    81,    82,    83,    84,
       0,    -1,    -1,     0,     0,    87,    88,    89,    90,    80,
      81,    82,    83,    84,     0,     0,     0,     0,     0,    -1,
      -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
       0,    22,    26,    43,    44,     7,    51,    34,     8,   177,
      12,    11,    14,    15,    43,    42,    18,    44,    42,    39,
      34,    41,    22,    25,    26,    41,    26,    47,   196,    31,
      32,    33,    34,    35,    36,    14,    16,    39,    59,    41,
     109,    34,    42,    45,    34,    25,    26,    41,    48,   118,
      50,    96,    42,    40,    44,    34,    38,    44,   103,    39,
      43,    41,    41,    34,    64,    65,    45,    47,    48,    19,
      20,    21,    42,    42,    44,    44,    12,    13,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   115,    92,    12,    13,    95,    39,    42,    41,    44,
       0,    42,   102,    44,    47,    48,    41,   107,     7,    40,
      44,    34,    42,    34,    34,   115,    41,   162,     4,    41,
     165,    37,    34,   120,    42,   149,    41,   151,    42,    37,
     173,    64,     3,   172,     5,     6,     7,     8,     9,    10,
     190,    12,    -1,    14,    15,    -1,    -1,    18,    -1,   149,
      -1,   151,    -1,    -1,    25,    26,    -1,    -1,    -1,   159,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      41,    -1,    43,    -1,    45,    -1,    -1,     3,   178,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    -1,    39,    11,    41,    13,    43,    -1,    45,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    12,    -1,    14,    15,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    46,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      -1,    39,    40,    41,    -1,    -1,    12,    45,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    41,    -1,    43,    12,    45,
      14,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    41,    42,    -1,
      12,    45,    14,    15,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    41,
      42,    -1,    12,    45,    14,    15,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    41,    42,    -1,    12,    45,    14,    15,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    41,    42,    -1,    12,    45,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    11,    41,    13,    -1,    -1,    45,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    43,    23,    24,    -1,
      -1,    27,    28,    29,    30,    -1,    11,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    43,    23,    24,
      -1,    -1,    27,    28,    29,    30,    -1,    11,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    43,    23,
      24,    -1,    -1,    27,    28,    29,    30,    11,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    42,    23,
      24,    -1,    -1,    27,    28,    29,    30,    11,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    42,    23,
      24,    -1,    -1,    27,    28,    29,    30,    11,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    42,    23,
      24,    -1,    -1,    27,    28,    29,    30,    -1,    -1,    -1,
      -1,    11,    -1,    13,    -1,    -1,    40,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    -1,    27,    28,    29,
      30,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,
      40,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      -1,    27,    28,    29,    30,    -1,    -1,    11,    -1,    13,
      -1,    -1,    38,    17,    18,    19,    20,    21,    -1,    23,
      24,    11,    -1,    27,    28,    29,    30,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    -1,    27,    28,    29,
      30,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      -1,    27,    28,    29,    30,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    -1,    27,    28,    29,    30,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     6,     7,     8,     9,    10,    12,    14,
      15,    18,    25,    26,    31,    32,    33,    34,    35,    36,
      37,    39,    41,    43,    45,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    70,    74,    77,
      82,    84,    87,    89,    93,    95,    98,    99,   100,   101,
      41,    41,    34,    76,    43,    43,    58,    34,    58,    41,
      62,    64,    62,    38,    75,    37,    40,    58,    68,    69,
      71,    72,    73,    58,    82,    34,     0,    57,    11,    13,
      17,    18,    19,    20,    21,    23,    24,    27,    28,    29,
      30,    43,    16,    25,    26,    39,    41,    47,    48,    65,
      66,    67,    39,    41,    47,    83,    57,    41,    94,    96,
      43,    58,    58,    69,    82,    54,    58,    40,    44,    40,
      44,    42,    42,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    42,    69,
      34,    34,    58,    42,    69,    34,    41,    78,     4,    88,
      58,    90,    92,    68,    43,    42,    43,    42,    38,    46,
      68,    73,    41,    40,    42,    41,    40,    42,    34,    42,
      44,    85,    86,    80,    57,    42,    57,    42,    97,    58,
      42,    69,    42,    69,    34,    42,    85,    74,    79,    91,
      96,    58,    38,    42,    42,    81,    92,    43,    96
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    61,    61,    61,    61,    61,    62,    62,    62,    62,
      63,    63,    63,    63,    64,    64,    64,    64,    64,    65,
      65,    66,    66,    67,    67,    68,    69,    69,    70,    70,
      70,    71,    72,    72,    73,    75,    74,    74,    76,    76,
      77,    78,    78,    79,    80,    81,    83,    82,    84,    84,
      84,    84,    84,    84,    85,    85,    86,    86,    87,    88,
      89,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   101
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     2,     2,
       3,     4,     3,     4,     3,     4,     2,     5,     6,     1,
       1,     3,     2,     4,     5,     1,     1,     3,     2,     3,
       3,     1,     1,     3,     5,     0,     4,     2,     1,     0,
       2,     2,     3,     1,     0,     0,     0,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     4,     1,
       2,     4,     0,     0,     3,     1,     3,     3,     0,     0,
       7,     7,     1,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* stmts: stmts stmt  */
#line 232 "parser.y"
                   {
                (yyval.stmtValue)->breaklist = mergelist((yyvsp[-1].stmtValue)->breaklist, (yyvsp[0].stmtValue)->breaklist);
                (yyval.stmtValue)->contlist = mergelist((yyvsp[-1].stmtValue)->contlist, (yyvsp[0].stmtValue)->contlist);
                        }
#line 1562 "parser.c"
    break;

  case 4: /* stmts: stmt  */
#line 236 "parser.y"
              {
                (yyval.stmtValue) = ((yyvsp[0].stmtValue)) ? (yyvsp[0].stmtValue) : stmt_constractor(0,0);
        }
#line 1570 "parser.c"
    break;

  case 5: /* brk_stm: BREAK SEMICOLON  */
#line 242 "parser.y"
                        {       (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->breaklist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        }
#line 1579 "parser.c"
    break;

  case 6: /* cnt_stm: CONTINUE SEMICOLON  */
#line 246 "parser.y"
                           {    (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->contlist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        }
#line 1588 "parser.c"
    break;

  case 7: /* stmt: expr SEMICOLON  */
#line 251 "parser.y"
                       {(yyval.stmtValue) = stmt_constractor(0,0); resettemp();}
#line 1594 "parser.c"
    break;

  case 8: /* stmt: ifstmt  */
#line 252 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1600 "parser.c"
    break;

  case 9: /* stmt: whilestmt  */
#line 253 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1606 "parser.c"
    break;

  case 10: /* stmt: forstmt  */
#line 254 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1612 "parser.c"
    break;

  case 11: /* stmt: returnstmt  */
#line 255 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1618 "parser.c"
    break;

  case 12: /* stmt: brk_stm  */
#line 256 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1624 "parser.c"
    break;

  case 13: /* stmt: cnt_stm  */
#line 257 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1630 "parser.c"
    break;

  case 14: /* stmt: block  */
#line 258 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1636 "parser.c"
    break;

  case 15: /* stmt: funcdef  */
#line 259 "parser.y"
                     {(yyval.stmtValue) = stmt_constractor(0,0);}
#line 1642 "parser.c"
    break;

  case 16: /* stmt: SEMICOLON  */
#line 260 "parser.y"
                   {resettemp(); (yyval.stmtValue) = stmt_constractor(0,0);}
#line 1648 "parser.c"
    break;

  case 17: /* expr: assignexpr  */
#line 264 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1654 "parser.c"
    break;

  case 18: /* expr: expr ADDITION expr  */
#line 265 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1660 "parser.c"
    break;

  case 19: /* expr: expr SUBTRACTION expr  */
#line 266 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1666 "parser.c"
    break;

  case 20: /* expr: expr MULTI expr  */
#line 267 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1672 "parser.c"
    break;

  case 21: /* expr: expr DIVISION expr  */
#line 268 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1678 "parser.c"
    break;

  case 22: /* expr: expr MODULUS expr  */
#line 269 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1684 "parser.c"
    break;

  case 23: /* expr: expr GRETER_THAN expr  */
#line 270 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater); emit_relop((yyval.exprValue),if_greater);}
#line 1690 "parser.c"
    break;

  case 24: /* expr: expr GRE_EQUAL expr  */
#line 271 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_geatereq); emit_relop((yyval.exprValue), if_geatereq);}
#line 1696 "parser.c"
    break;

  case 25: /* expr: expr LESS_THAN expr  */
#line 272 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_less);    emit_relop((yyval.exprValue),if_less);}
#line 1702 "parser.c"
    break;

  case 26: /* expr: expr LES_EQUAL expr  */
#line 273 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_lesseq);  emit_relop((yyval.exprValue),if_lesseq);}
#line 1708 "parser.c"
    break;

  case 27: /* expr: expr EQUAL expr  */
#line 274 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_eq);      emit_relop((yyval.exprValue),if_eq);}
#line 1714 "parser.c"
    break;

  case 28: /* expr: expr NOTEQUAL expr  */
#line 275 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_noteq);   emit_relop((yyval.exprValue),if_noteq);}
#line 1720 "parser.c"
    break;

  case 29: /* expr: expr AND expr  */
#line 276 "parser.y"
                                {       (yyval.exprValue) =   newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),and)) ; 
                                        (yyval.exprValue)->sym = newtemp();     
                                        emit(and,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno);
                                }
#line 1729 "parser.c"
    break;

  case 30: /* expr: expr OR expr  */
#line 280 "parser.y"
                                {       (yyval.exprValue) = newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),or)); 
                                        (yyval.exprValue)->sym = newtemp();       
                                        emit(or,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno); // epishs auto htan nextquad +2
                                        /*emit(jump,NULL,NULL,NULL,nextquadlabel()+3,yylineno);
                                        emit_relop($$,or); //htan to idio kai sthn and/not nmzw apla ta vgazoume k gg */
                                }
#line 1740 "parser.c"
    break;

  case 31: /* expr: term  */
#line 286 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1746 "parser.c"
    break;

  case 32: /* term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 289 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 1752 "parser.c"
    break;

  case 33: /* term: SUBTRACTION expr  */
#line 290 "parser.y"
                                             { //%prec: priority of uminus operator 
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = istempexpr((yyvsp[0].exprValue)) ? (yyvsp[0].exprValue)->sym : newtemp();
                
                emit(mul,(yyvsp[0].exprValue),newexpr_constnum(1),(yyval.exprValue),0,yylineno);
        }
#line 1764 "parser.c"
    break;

  case 34: /* term: NOT expr  */
#line 297 "parser.y"
                                {//not a
                (yyval.exprValue) = newexpr(constbool_e);
                (yyval.exprValue)->sym = newtemp();
                (yyval.exprValue)->boolConst = !(check_if_bool((yyvsp[0].exprValue)));
                emit(not,(yyvsp[0].exprValue),NULL,(yyval.exprValue),0,yylineno);
        }
#line 1775 "parser.c"
    break;

  case 35: /* term: INCREMENT lvalue  */
#line 303 "parser.y"
                                { //++lvalue
                check_arith((yyvsp[0].exprValue));
                if((yyvsp[0].exprValue)->type == tableitem_e){
                        (yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));
                        emit(add, (yyval.exprValue), newexpr_constnum(1), (yyval.exprValue), 0, yylineno);
                        emit(tablesetelem, (yyvsp[0].exprValue), (yyvsp[0].exprValue)->index, (yyval.exprValue), 0, yylineno);
                }else{
                        emit(add, (yyvsp[0].exprValue), newexpr_constnum(1), (yyvsp[0].exprValue), 0, yylineno);
                        (yyval.exprValue) = newexpr(arithexpr_e);
                        (yyval.exprValue)->sym = newtemp();
                        emit(assign, (yyvsp[0].exprValue), NULL, (yyval.exprValue), 0, yylineno);
                }                                     //edw exw balei ayta xwris na kserw an einai
        }
#line 1793 "parser.c"
    break;

  case 36: /* term: lvalue INCREMENT  */
#line 316 "parser.y"
                                {    //lvalue++
                check_arith((yyvsp[-1].exprValue));
                (yyval.exprValue) = newexpr(var_e);
                (yyval.exprValue)->sym = newtemp();
                if((yyvsp[-1].exprValue)->type == tableitem_e){
                        expr* val = emit_iftableitem((yyvsp[-1].exprValue));
                        emit(assign, val, NULL, (yyval.exprValue), 0, yylineno);
                        emit(add, val, newexpr_constnum(1), val,0,yylineno);
                        emit(tablesetelem, (yyvsp[-1].exprValue), (yyvsp[-1].exprValue)->index, val,0,yylineno);
                }else{
                        emit(assign, (yyvsp[-1].exprValue), NULL, (yyval.exprValue),0,yylineno);
                        emit(add, (yyvsp[-1].exprValue), newexpr_constnum(1), (yyvsp[-1].exprValue),0,yylineno);
                }
        }
#line 1812 "parser.c"
    break;

  case 37: /* term: DECREMENT lvalue  */
#line 330 "parser.y"
                                { //--lvalue
                check_arith((yyvsp[0].exprValue));
                if((yyvsp[0].exprValue)->type == tableitem_e){
                        (yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));
                        emit(sub, (yyval.exprValue), newexpr_constnum(1), (yyval.exprValue), 0, yylineno);
                        emit(tablesetelem, (yyvsp[0].exprValue), (yyvsp[0].exprValue)->index, (yyval.exprValue), 0, yylineno);
                }else{
                        emit(sub, (yyvsp[0].exprValue), newexpr_constnum(1), (yyvsp[0].exprValue), 0, yylineno);
                        (yyval.exprValue) = newexpr(arithexpr_e);
                        (yyval.exprValue)->sym = newtemp();
                        emit(assign, (yyvsp[0].exprValue), NULL, (yyval.exprValue), 0, yylineno);
                } 
        }
#line 1830 "parser.c"
    break;

  case 38: /* term: lvalue DECREMENT  */
#line 343 "parser.y"
                                { //lvalue--
                check_arith((yyvsp[-1].exprValue));
                (yyval.exprValue) = newexpr(var_e);
                (yyval.exprValue)->sym = newtemp();
                if((yyvsp[-1].exprValue)->type == tableitem_e){
                        expr* val = emit_iftableitem((yyvsp[-1].exprValue));
                        emit(assign, val, NULL, (yyval.exprValue), 0, yylineno);
                        emit(sub, val, newexpr_constnum(1), val,0,yylineno);
                        emit(tablesetelem, (yyvsp[-1].exprValue), (yyvsp[-1].exprValue)->index, val,0,yylineno);
                }else{
                        emit(assign, (yyvsp[-1].exprValue), NULL, (yyval.exprValue),0,yylineno);
                        emit(sub, (yyvsp[-1].exprValue), newexpr_constnum(1), (yyvsp[-1].exprValue),0,yylineno);
                }
        }
#line 1849 "parser.c"
    break;

  case 39: /* term: primary  */
#line 357 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1855 "parser.c"
    break;

  case 40: /* assignexpr: lvalue ASSIGNMENT expr  */
#line 360 "parser.y"
                                  {
				{
					//check_if_fuction($1); //check gia to table
                                        //lvalue[i] = expr
					if((yyvsp[-2].exprValue)->type == tableitem_e) {
						emit(tablesetelem, (yyvsp[0].exprValue), (yyvsp[-2].exprValue)->index, (yyvsp[-2].exprValue), 0, yylineno);
						(yyval.exprValue) = emit_iftableitem((yyvsp[-2].exprValue));
						(yyval.exprValue)->type = assignexpr_e;
					} 
                                        // lvalue = expr
                                        else {
						emit(assign, (yyvsp[0].exprValue), NULL,(yyvsp[-2].exprValue), 0, yylineno); //paizei na thelei ta arg anapoda
						(yyval.exprValue) = newexpr(assignexpr_e);
						(yyval.exprValue)->sym = newtemp();
                                               // $1->type = $3->type;
                                               // copy_value($1,$3);
						emit(assign ,(yyvsp[-2].exprValue), NULL, (yyval.exprValue),0, yylineno); //omoiws me to apo panw
                                                //copy_value($assignexpr,$1);
                                        }
				}               
        }
#line 1881 "parser.c"
    break;

  case 41: /* primary: lvalue  */
#line 383 "parser.y"
                                {(yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));}
#line 1887 "parser.c"
    break;

  case 42: /* primary: call  */
#line 384 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1893 "parser.c"
    break;

  case 43: /* primary: objectdef  */
#line 385 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1899 "parser.c"
    break;

  case 44: /* primary: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS  */
#line 386 "parser.y"
                                                   {
                (yyval.exprValue) = newexpr(programfunc_e);
                (yyval.exprValue)->sym = (yyvsp[-1].symbolEntry); 
        }
#line 1908 "parser.c"
    break;

  case 45: /* primary: const  */
#line 390 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1914 "parser.c"
    break;

  case 46: /* lvalue: member  */
#line 393 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1920 "parser.c"
    break;

  case 47: /* lvalue: ID  */
#line 394 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 1926 "parser.c"
    break;

  case 48: /* lvalue: LOCAL ID  */
#line 395 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 1932 "parser.c"
    break;

  case 49: /* lvalue: SCOPE_RESOLUTION ID  */
#line 396 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 1938 "parser.c"
    break;

  case 50: /* member: lvalue FULL_STOP ID  */
#line 399 "parser.y"
                                {// a.x;
                                (yyval.exprValue) = member_item((yyvsp[-2].exprValue),yylval.stringValue);}
#line 1945 "parser.c"
    break;

  case 51: /* member: lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET  */
#line 401 "parser.y"
                                                              { // a[3]
                                (yyvsp[-3].exprValue) = emit_iftableitem((yyvsp[-3].exprValue));
                                (yyval.exprValue) = newexpr(tableitem_e);
                                (yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
                                (yyval.exprValue)->index = (yyvsp[-1].exprValue); //index of expr
        }
#line 1956 "parser.c"
    break;

  case 54: /* call: call LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 413 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-2].exprValue),NULL);
                                }
#line 1963 "parser.c"
    break;

  case 55: /* call: call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 416 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-3].exprValue), (yyvsp[-1].exprValue));}
#line 1969 "parser.c"
    break;

  case 56: /* call: lvalue callsuffix  */
#line 418 "parser.y"
                                {
                                (yyvsp[-1].exprValue) = emit_iftableitem((yyvsp[-1].exprValue));            //se periptwsh p htan table item
                                if((yyvsp[0].callValue)->method){
                                        expr* t = (yyvsp[-1].exprValue);
                                        (yyvsp[-1].exprValue) = emit_iftableitem(member_item(t, (yyvsp[0].callValue)->name));
                                        if((yyvsp[0].callValue)->elist)//!EDW isws prepei na to treksw mexri to telos k meta na to kanw insert?
                                        {       expr *e = (yyvsp[0].callValue)->elist;
                                                while(e->next) e = e->next;
                                                 e->next = t;   }        // insert san prwto arg (reversed,so last)
                                        else
                                                (yyvsp[0].callValue)->elist = t;
                                }
                                (yyval.exprValue) = make_call((yyvsp[-1].exprValue), (yyvsp[0].callValue)->elist);
        }
#line 1988 "parser.c"
    break;

  case 57: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 433 "parser.y"
                                                                                       {
                                expr* func = newexpr(programfunc_e);
				func->sym = (yyvsp[-3].symbolEntry);
				(yyval.exprValue) = make_call(func, NULL);
        }
#line 1998 "parser.c"
    break;

  case 58: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 439 "parser.y"
                                {
                                expr* func = newexpr(programfunc_e);
                                func->sym = (yyvsp[-4].symbolEntry);
                                (yyval.exprValue) = make_call(func, (yyvsp[-1].exprValue));                
        }
#line 2008 "parser.c"
    break;

  case 59: /* callsuffix: normcall  */
#line 446 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue); }
#line 2014 "parser.c"
    break;

  case 60: /* callsuffix: methodcall  */
#line 447 "parser.y"
                               {(yyval.callValue) = (yyvsp[0].callValue);}
#line 2020 "parser.c"
    break;

  case 61: /* normcall: LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 450 "parser.y"
                                                          {
                                (yyval.callValue) = call_constractor((yyvsp[-1].exprValue),0,NULL);
        }
#line 2028 "parser.c"
    break;

  case 62: /* normcall: LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 454 "parser.y"
                                             {
                                (yyval.callValue) = call_constractor(NULL,0,NULL);
        }
#line 2036 "parser.c"
    break;

  case 63: /* methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 461 "parser.y"
                                                              {
                                (yyval.callValue) = call_constractor(NULL,1,(yyvsp[-2].stringValue));
        }
#line 2044 "parser.c"
    break;

  case 64: /* methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 464 "parser.y"
                                                                         {
                                (yyval.callValue) = call_constractor((yyvsp[-1].exprValue),1,(yyvsp[-3].stringValue));
        }
#line 2052 "parser.c"
    break;

  case 65: /* elist: expr  */
#line 469 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2058 "parser.c"
    break;

  case 66: /* moreElist: elist  */
#line 472 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2064 "parser.c"
    break;

  case 67: /* moreElist: moreElist COMMA elist  */
#line 473 "parser.y"
                                {
                                (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 2073 "parser.c"
    break;

  case 68: /* objectdef: LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET  */
#line 482 "parser.y"
                                {     //[]
                                (yyval.exprValue) = tablecreate_and_emit();
                                }
#line 2081 "parser.c"
    break;

  case 69: /* objectdef: LEFT_SQUARE_BRACKET moreElist RIGHT_SQUARE_BRACKET  */
#line 486 "parser.y"
                                {    // [20,30,"hello"]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyvsp[-1].exprValue),newexpr_constnum(i),(yyval.exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                }                                                
                                }
#line 2095 "parser.c"
    break;

  case 70: /* objectdef: LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET  */
#line 496 "parser.y"
                             { //[{"x" : (fuction(s){print(s);})} ]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_indexed_length((yyvsp[-1].indexedValue));
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyvsp[-1].indexedValue)->value,(yyvsp[-1].indexedValue)->indexedelem,(yyval.exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                }   
        
        }
#line 2109 "parser.c"
    break;

  case 71: /* indexed: moreindexedelem  */
#line 507 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2115 "parser.c"
    break;

  case 72: /* moreindexedelem: indexedelem  */
#line 511 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2121 "parser.c"
    break;

  case 73: /* moreindexedelem: moreindexedelem COMMA indexedelem  */
#line 513 "parser.y"
                    {
                                (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2130 "parser.c"
    break;

  case 74: /* indexedelem: LEFT_CURLY_BRACKET expr COLON expr RIGHT_CURLY_BRACKET  */
#line 520 "parser.y"
                                 {
                                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2138 "parser.c"
    break;

  case 75: /* $@1: %empty  */
#line 526 "parser.y"
                           { 
                                CURR_SCOPE++;   
                        }
#line 2146 "parser.c"
    break;

  case 76: /* block: LEFT_CURLY_BRACKET $@1 stmts RIGHT_CURLY_BRACKET  */
#line 528 "parser.y"
                                                  {
                                if(CURR_SCOPE!=0)
                                        hide(CURR_SCOPE);
                                (yyval.stmtValue) = ((yyvsp[-1].stmtValue))  ?  (yyvsp[-1].stmtValue) : stmt_constractor(0,0);
                                CURR_SCOPE--;
                }
#line 2157 "parser.c"
    break;

  case 77: /* block: LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET  */
#line 534 "parser.y"
                                               {
                if(CURR_SCOPE!=0)
                        hide(CURR_SCOPE--); 
                (yyval.stmtValue) = stmt_constractor(0,0); 
        }
#line 2167 "parser.c"
    break;

  case 78: /* funcname: ID  */
#line 543 "parser.y"
                        {(yyval.stringValue) = (yyvsp[0].stringValue);}
#line 2173 "parser.c"
    break;

  case 79: /* funcname: %empty  */
#line 544 "parser.y"
                        {(yyval.stringValue) = newemptyfuncname();}
#line 2179 "parser.c"
    break;

  case 80: /* funcprefix: FUNCTION funcname  */
#line 548 "parser.y"
                          { 
                        (yyval.symbolEntry) = function_insert((yyvsp[0].stringValue));               //yylval.stringValue
                        (yyval.symbolEntry)->fuctionAddress = nextquadlabel();
                        //gia kapoio logo to lvalue_expr() den douleuei
                        expr* e = newexpr(programfunc_e);       e->sym = (yyval.symbolEntry);
                        emit(jump,NULL,NULL,NULL,0,yylineno); //pou teliwnei to fuct
                        emit(funcstart,e, NULL, NULL,0,yylineno);
                        push(save_fuctionlocals,currscopeoffset());
                        enterscopespace();                      // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2195 "parser.c"
    break;

  case 81: /* funcargs: LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 562 "parser.y"
                                          {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2205 "parser.c"
    break;

  case 82: /* funcargs: LEFT_PARENTHESIS moreidilist RIGHT_PARENTHESIS  */
#line 567 "parser.y"
                                                        {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2215 "parser.c"
    break;

  case 83: /* funcbody: block  */
#line 575 "parser.y"
                        {
                        delete_last_fuction_scope();
                        (yyval.intValue) = currscopeoffset();
                        printf("%d\n\n",(yyval.intValue));     
                        exitscopespace();
        }
#line 2226 "parser.c"
    break;

  case 84: /* funcblockstart: %empty  */
#line 583 "parser.y"
                 {      push(loopcounterStack,loopcounter);
                        loopcounter = 0;
                        fuctioncounter++;}
#line 2234 "parser.c"
    break;

  case 85: /* funcblockend: %empty  */
#line 587 "parser.y"
                {       pop(loopcounterStack);
                        fuctioncounter--;}
#line 2241 "parser.c"
    break;

  case 86: /* $@2: %empty  */
#line 591 "parser.y"
                        { fuction_scope_insert(CURR_SCOPE++); }
#line 2247 "parser.c"
    break;

  case 87: /* funcdef: funcprefix $@2 funcargs funcblockstart funcbody funcblockend  */
#line 592 "parser.y"
                                                        {
                        exitscopespace();
                        (yyvsp[-5].symbolEntry)->totalLocals = (yyvsp[-1].intValue);
                        int offset = pop(save_fuctionlocals);
                        restorecurrscopeoffset(offset);
                        (yyval.symbolEntry) = (yyvsp[-5].symbolEntry);
                        emit(funcend, lvalue_expr((yyvsp[-5].symbolEntry)), NULL, NULL,0,yylineno);
                        patchlabel((yyvsp[-5].symbolEntry)->fuctionAddress,nextquadlabel());
        }
#line 2261 "parser.c"
    break;

  case 88: /* const: INTEGER  */
#line 603 "parser.y"
                                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2267 "parser.c"
    break;

  case 89: /* const: REAL  */
#line 604 "parser.y"
                                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2273 "parser.c"
    break;

  case 90: /* const: STRING  */
#line 605 "parser.y"
                                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2279 "parser.c"
    break;

  case 91: /* const: NILL  */
#line 606 "parser.y"
                                { (yyval.exprValue) = newexpr_nil();}
#line 2285 "parser.c"
    break;

  case 92: /* const: TRUE  */
#line 607 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool(1);}
#line 2291 "parser.c"
    break;

  case 93: /* const: FALSE  */
#line 608 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool(0);}
#line 2297 "parser.c"
    break;

  case 94: /* idlist: ID  */
#line 611 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2303 "parser.c"
    break;

  case 95: /* idlist: COMMA ID  */
#line 612 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2309 "parser.c"
    break;

  case 98: /* ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 620 "parser.y"
                                                        {
                emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);   
                (yyval.intValue) = nextquadlabel(); 
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2319 "parser.c"
    break;

  case 99: /* elseprefix: ELSE  */
#line 626 "parser.y"
                        {
                //emit(if_eq, $expr, newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);  
                (yyval.intValue) = nextquadlabel();
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2329 "parser.c"
    break;

  case 100: /* ifstmt: ifprefix stmt  */
#line 632 "parser.y"
                              {
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                 (yyval.stmtValue) = ((yyvsp[0].stmtValue))  ?  (yyvsp[0].stmtValue) : stmt_constractor(0,0);
        }
#line 2338 "parser.c"
    break;

  case 101: /* ifstmt: ifprefix stmt elseprefix stmt  */
#line 636 "parser.y"
                                                 {
                patchlabel((yyvsp[-3].intValue), (yyvsp[-1].intValue) + 1);
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                (yyval.stmtValue) = stmt_constractor(0,0);
                /*an exoun kai ta 2 value ftiakse ta break/cont list (oustiaka an exoun stmt k to if k to else)*/
                if((yyvsp[-2].stmtValue) && (yyvsp[0].stmtValue)){
                        (yyval.stmtValue)->breaklist = mergelist((yyvsp[-2].stmtValue)->breaklist, (yyvsp[0].stmtValue)->breaklist);
                        (yyval.stmtValue)->contlist = mergelist((yyvsp[0].stmtValue)->contlist, (yyvsp[0].stmtValue)->contlist);
                }
                else if((yyvsp[-2].stmtValue))
                        (yyval.stmtValue) = (yyvsp[-2].stmtValue);
                else if((yyvsp[0].stmtValue))
                        (yyval.stmtValue) = (yyvsp[0].stmtValue); 
        }
#line 2357 "parser.c"
    break;

  case 102: /* loopstart: %empty  */
#line 653 "parser.y"
                { ++loopcounter; }
#line 2363 "parser.c"
    break;

  case 103: /* loopend: %empty  */
#line 654 "parser.y"
                { --loopcounter; }
#line 2369 "parser.c"
    break;

  case 104: /* loopstmt: loopstart stmt loopend  */
#line 657 "parser.y"
                                  {(yyval.stmtValue) = (yyvsp[-1].stmtValue);}
#line 2375 "parser.c"
    break;

  case 105: /* whilestart: WHILE  */
#line 660 "parser.y"
                   { (yyval.intValue) = nextquadlabel();
}
#line 2382 "parser.c"
    break;

  case 106: /* whilecond: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 663 "parser.y"
                                                   {
        emit(if_eq,(yyvsp[-1].exprValue),newexpr_constbool(1),NULL,nextquadlabel()+2,yylineno);
        (yyval.intValue) = nextquadlabel();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 2392 "parser.c"
    break;

  case 107: /* whilestmt: whilestart whilecond loopstmt  */
#line 671 "parser.y"
                                         {
        emit(jump, NULL, NULL,NULL, (yyvsp[-2].intValue),yylineno);
        patchlabel((yyvsp[-1].intValue), nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->contlist, (yyvsp[-2].intValue));
        (yyval.stmtValue) = stmt_constractor(0,0) ; 
}
#line 2404 "parser.c"
    break;

  case 108: /* N: %empty  */
#line 680 "parser.y"
    { (yyval.intValue) = nextquadlabel(); emit(jump,NULL,NULL,NULL,0,yylineno); }
#line 2410 "parser.c"
    break;

  case 109: /* M: %empty  */
#line 681 "parser.y"
    { (yyval.intValue) = nextquadlabel(); }
#line 2416 "parser.c"
    break;

  case 110: /* forprefix: FOR LEFT_PARENTHESIS moreElist SEMICOLON M expr SEMICOLON  */
#line 683 "parser.y"
{       
        (yyval.forValue) = for_prefix_constractor((yyvsp[-2].intValue),nextquadlabel());
        emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL,0,yylineno);
}
#line 2425 "parser.c"
    break;

  case 111: /* forstmt: forprefix N elist RIGHT_PARENTHESIS N loopstmt N  */
#line 689 "parser.y"
                                                            {
        patchlabel((yyvsp[-6].forValue)->enter, (yyvsp[-2].intValue)+1); 
        patchlabel((yyvsp[-5].intValue), nextquadlabel()); 
        patchlabel((yyvsp[-2].intValue), (yyvsp[-6].forValue)->test); 
        patchlabel((yyvsp[0].intValue), (yyvsp[-5].intValue)+1); 
        patchlist((yyvsp[-1].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[-1].stmtValue)->contlist, (yyvsp[-5].intValue)+1);
        (yyval.stmtValue) =   stmt_constractor(0,0);}
#line 2438 "parser.c"
    break;

  case 112: /* returnstart: RETURN  */
#line 700 "parser.y"
                     {
        (yyval.intValue) = nextquadlabel()+1;
}
#line 2446 "parser.c"
    break;

  case 113: /* returnstmt: returnstart SEMICOLON  */
#line 704 "parser.y"
                                  {
                if(fuctioncounter == 0)
                        yyerror("return w/o function");
                else
                       { 
                        emit(ret,NULL,NULL,NULL,0,yylineno);
                        emit(jump,NULL,NULL,NULL,0,yylineno);
                        patchlabel((yyvsp[-1].intValue) , nextquadlabel());
                }
                (yyval.stmtValue) = stmt_constractor(0,0);
        }
#line 2462 "parser.c"
    break;

  case 114: /* returnstmt: returnstart expr SEMICOLON  */
#line 715 "parser.y"
                                   {
                if(fuctioncounter == 0)
                        yyerror("return w/o function");
                else
                        {
                emit(ret,NULL,NULL,(yyvsp[-1].exprValue),0,yylineno);
                emit(jump,NULL,NULL,NULL,0,yylineno);
                patchlabel((yyvsp[-2].intValue) , nextquadlabel());
                }
                (yyval.stmtValue) = stmt_constractor(0,0);
}
#line 2478 "parser.c"
    break;


#line 2482 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 727 "parser.y"


int yyerror(char* yaccProvidedMessage){
                //provide error message example:
        fprintf(stderr,"%s: error at line %d , before token %s\n",yaccProvidedMessage,yylineno,yytext);
        error_flag = 1;
        //fprintf(stderr,"INVALID INPUT\n");
}


extern int curr_i;
extern instruction* instructions ;
extern Stack* userfunctions_Stack;

/*-----------------------------MAIN-----------------------*/
int main(int argc, char** argv){
        if(argc>1){
                if(!(yyin=fopen(argv[1],"r"))){
                        fprintf(stderr,"Error%s\n",argv[1]);
                        return 1;
                }
        }
	else yyin=stdin;
                
        
        save_fuctionlocals = createStack(150); 
        loopcounterStack = createStack(200);
        userfunctions_Stack = createStack(200);
        init_lib_func();
        malloc_all_lists();
        //yyset_in(input_file); // set the input stream for the lexer
        yyparse(); // call the parser function
        if(error_flag != 0)
                printf("/-------------   ERRORS     -------------------/\n");
        print_format(); //print scopes
        print_all_quads(); //print quads
        generateInstructions();
        //print_all_i(instructions,curr_i);
        write_all_i(TXT_FILE);
        writeBinaryFile(BINARY_FILE);
        return 0;
}
