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
        #include "Symbol_Table.h"
        #include "quads.h"
        #include "Stack.h"

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

        int curr_anonymous = 0; //keep track for anonymous
        int if_flag = 0;
        int for_flag = 0;
        int error_flag = 0;     // check if there were errors


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
                //check if anonymous and insert if true
                /*if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                        hash_insert(myfuction,table);
                        //print_var(myfuction); //na ftiaksw ta print
                        return myfuction;
                }*/
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

#line 225 "parser.c"

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
  YYSYMBOL_stmt_list = 54,                 /* stmt_list  */
  YYSYMBOL_brk_stm = 55,                   /* brk_stm  */
  YYSYMBOL_56_1 = 56,                      /* $@1  */
  YYSYMBOL_cnt_stm = 57,                   /* cnt_stm  */
  YYSYMBOL_58_2 = 58,                      /* $@2  */
  YYSYMBOL_stmt = 59,                      /* stmt  */
  YYSYMBOL_expr = 60,                      /* expr  */
  YYSYMBOL_term = 61,                      /* term  */
  YYSYMBOL_assignexpr = 62,                /* assignexpr  */
  YYSYMBOL_primary = 63,                   /* primary  */
  YYSYMBOL_lvalue = 64,                    /* lvalue  */
  YYSYMBOL_member = 65,                    /* member  */
  YYSYMBOL_call = 66,                      /* call  */
  YYSYMBOL_callsuffix = 67,                /* callsuffix  */
  YYSYMBOL_normcall = 68,                  /* normcall  */
  YYSYMBOL_methodcall = 69,                /* methodcall  */
  YYSYMBOL_elist = 70,                     /* elist  */
  YYSYMBOL_moreElist = 71,                 /* moreElist  */
  YYSYMBOL_objectdef = 72,                 /* objectdef  */
  YYSYMBOL_indexed = 73,                   /* indexed  */
  YYSYMBOL_moreindexedelem = 74,           /* moreindexedelem  */
  YYSYMBOL_indexedelem = 75,               /* indexedelem  */
  YYSYMBOL_block = 76,                     /* block  */
  YYSYMBOL_77_3 = 77,                      /* $@3  */
  YYSYMBOL_funcname = 78,                  /* funcname  */
  YYSYMBOL_funcprefix = 79,                /* funcprefix  */
  YYSYMBOL_funcargs = 80,                  /* funcargs  */
  YYSYMBOL_funcbody = 81,                  /* funcbody  */
  YYSYMBOL_funcdef = 82,                   /* funcdef  */
  YYSYMBOL_83_4 = 83,                      /* $@4  */
  YYSYMBOL_const = 84,                     /* const  */
  YYSYMBOL_idlist = 85,                    /* idlist  */
  YYSYMBOL_moreidilist = 86,               /* moreidilist  */
  YYSYMBOL_ifprefix = 87,                  /* ifprefix  */
  YYSYMBOL_elseprefix = 88,                /* elseprefix  */
  YYSYMBOL_ifstmt = 89,                    /* ifstmt  */
  YYSYMBOL_90_5 = 90,                      /* $@5  */
  YYSYMBOL_whilestmt = 91,                 /* whilestmt  */
  YYSYMBOL_92_6 = 92,                      /* $@6  */
  YYSYMBOL_forstmt = 93,                   /* forstmt  */
  YYSYMBOL_94_7 = 94,                      /* $@7  */
  YYSYMBOL_returnstmt = 95,                /* returnstmt  */
  YYSYMBOL_96_8 = 96                       /* $@8  */
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   620

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  104
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  185

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
       0,   228,   228,   231,   232,   236,   236,   237,   237,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   269,   270,   276,   282,   295,   309,
     322,   336,   339,   362,   363,   364,   365,   369,   372,   373,
     374,   375,   378,   380,   387,   388,   391,   393,   395,   405,
     413,   414,   417,   422,   429,   436,   439,   440,   448,   452,
     462,   474,   478,   479,   486,   493,   493,   503,   504,   508,
     519,   527,   535,   535,   546,   547,   548,   549,   550,   551,
     554,   555,   558,   559,   563,   569,   574,   574,   577,   584,
     584,   588,   588,   591,   591
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
  "stmt_list", "brk_stm", "$@1", "cnt_stm", "$@2", "stmt", "expr", "term",
  "assignexpr", "primary", "lvalue", "member", "call", "callsuffix",
  "normcall", "methodcall", "elist", "moreElist", "objectdef", "indexed",
  "moreindexedelem", "indexedelem", "block", "$@3", "funcname",
  "funcprefix", "funcargs", "funcbody", "funcdef", "$@4", "const",
  "idlist", "moreidilist", "ifprefix", "elseprefix", "ifstmt", "$@5",
  "whilestmt", "$@6", "forstmt", "$@7", "returnstmt", "$@8", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-92)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -92,     3,   184,   -92,   -33,   -92,   -26,   -20,    11,   -92,
     -92,   -92,   312,    17,   -92,   312,   -12,   -12,   -92,   -92,
     -92,   -92,   -92,   -92,   -92,   216,     5,   -92,    25,   -92,
     -92,   -92,   341,   -92,   -92,   -92,   260,   -92,     8,   -92,
     -92,   -92,   -92,   -92,   -92,   184,   -92,   -92,   -92,   -92,
     312,   -92,   -92,   -92,   -92,   312,   -11,    24,   -92,   -92,
     -92,    81,   -13,     8,   -13,   -92,   312,   -92,   534,   -92,
     -35,    51,    52,   -92,   404,    53,   -92,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     -92,   312,   -92,   -92,   312,   248,    63,    64,   -92,   -92,
     -92,   312,   280,    66,    60,   184,   -92,   424,   312,   312,
     362,   -92,   -92,    62,   141,    96,   -92,   312,   -92,    65,
     -92,    67,   562,   548,    37,    37,   -92,   -92,   -92,   576,
     576,   590,   590,   590,   590,   534,   464,   -92,   -17,   -92,
      69,   488,   -92,    10,   -92,   -92,    68,   -92,   -92,   444,
      28,   -92,    67,   -92,   312,   -92,   -92,   312,   -92,   -92,
     312,   -92,   -92,   -28,   -92,   -92,   184,   312,   512,    18,
      21,   -92,   -92,    84,   -92,   -92,   383,   -92,   -92,   -92,
     -92,   312,    26,   184,   -92
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,    95,     0,     0,    78,   103,
       5,     7,     0,     0,    87,     0,     0,     0,    84,    85,
      86,    49,    88,    89,    75,     0,     0,    18,     0,    14,
      15,     3,     0,    33,    19,    41,    43,    48,    44,    45,
      16,    82,    17,    47,    96,     0,    10,    11,    12,    13,
       0,    99,   101,    77,    79,     0,     0,     0,    36,    50,
      35,     0,    37,     0,    39,     4,     0,    68,    65,    66,
       0,     0,    71,    72,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       9,     0,    38,    40,     0,     0,     0,     0,    58,    60,
      61,     0,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     6,     8,     0,     0,     0,    69,     0,    70,     0,
      34,    46,    31,    32,    20,    21,    22,    23,    24,    29,
      30,    25,    27,    26,    28,    42,     0,    63,     0,    52,
       0,     0,    56,     0,    54,    93,     0,    97,    94,     0,
       0,   104,     0,    76,     0,    67,    73,     0,    53,    62,
       0,    55,    57,     0,    81,    83,     0,     0,     0,     0,
       0,    90,    80,     0,    92,   100,     0,    74,    59,    64,
      91,     0,     0,     0,   102
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -92,   -92,    46,   -92,   -92,   -92,   -92,   -44,    -2,   -92,
     -92,   -92,    57,   -92,    77,   -92,   -92,   -92,     4,   -91,
     -92,   -92,   -92,     9,   -16,   -92,   -92,   -92,   -92,   -92,
     -19,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,   -92,   -92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    29,    56,    30,    57,    31,    68,    33,
      34,    35,    36,    37,    38,    98,    99,   100,    69,    70,
      39,    71,    72,    73,    40,    65,    54,    41,   146,   165,
      42,   104,    43,   174,   163,    44,    45,    46,   105,    47,
     108,    48,   109,    49,    55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,   106,    13,     3,   138,   116,   171,    75,    50,   117,
      58,   143,     8,    60,   172,    51,   173,    12,   150,    13,
      14,    52,    21,    15,    74,   159,    94,   117,    95,    61,
      16,    17,   111,    28,    96,    97,    18,    19,    20,    21,
      22,    23,   113,    32,    25,    53,    26,   101,   107,   102,
      28,    59,   162,   110,   117,   103,    81,    82,    83,    76,
     178,   147,   117,   179,   115,   117,   169,   112,   183,   170,
     117,   167,   117,    62,    64,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     8,   135,
     182,   118,   136,    63,    63,   121,   119,   139,   140,   141,
     144,   145,    66,    32,   152,    24,   149,    77,   157,    78,
     160,   114,    32,    79,    80,    81,    82,    83,   180,    84,
      85,   155,   175,    86,    87,    88,    89,     0,   156,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,   184,
       0,     0,   154,     0,     4,     5,     6,     7,     8,     9,
      10,    11,   168,    12,     0,    13,    14,     0,     0,    15,
       0,     0,     0,     0,    32,   176,    16,    17,     0,     0,
       0,     0,    18,    19,    20,    21,    22,    23,    24,   153,
      25,    32,    26,     0,    27,     0,    28,     4,     5,     6,
       7,     8,     9,    10,    11,     0,    12,     0,    13,    14,
       0,     0,    15,     0,     0,     0,     0,     0,     0,    16,
      17,     0,     0,     0,     0,    18,    19,    20,    21,    22,
      23,    24,     0,    25,     0,    26,     0,    27,    12,    28,
      13,    14,     0,     0,    15,     0,     0,     0,     0,     0,
       0,    16,    17,     0,     0,     0,     0,    18,    19,    20,
      21,    22,    23,    66,     0,    25,    67,    26,     0,     0,
      12,    28,    13,    14,     0,     0,    15,     0,     0,     0,
       0,     0,     0,    16,    17,     0,    91,     0,     0,    18,
      19,    20,    21,    22,    23,    92,    93,    25,     0,    26,
     137,     0,    12,    28,    13,    14,     0,     0,    15,    94,
       0,    95,     0,     0,     0,    16,    17,    96,    97,     0,
       0,    18,    19,    20,    21,    22,    23,     0,     0,    25,
       0,    26,   142,     0,    12,    28,    13,    14,     0,     0,
      15,     0,     0,     0,     0,     0,     0,    16,    17,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,     0,
       0,    25,    77,    26,    78,     0,     0,    28,    79,    80,
      81,    82,    83,     0,    84,    85,     0,     0,    86,    87,
      88,    89,     0,    77,     0,    78,     0,     0,     0,    79,
      80,    81,    82,    83,    90,    84,    85,     0,     0,    86,
      87,    88,    89,     0,    77,     0,    78,     0,     0,     0,
      79,    80,    81,    82,    83,   151,    84,    85,     0,     0,
      86,    87,    88,    89,     0,    77,     0,    78,     0,     0,
       0,    79,    80,    81,    82,    83,   181,    84,    85,     0,
       0,    86,    87,    88,    89,    77,     0,    78,     0,     0,
       0,    79,    80,    81,    82,    83,   120,    84,    85,     0,
       0,    86,    87,    88,    89,    77,     0,    78,     0,     0,
       0,    79,    80,    81,    82,    83,   148,    84,    85,     0,
       0,    86,    87,    88,    89,    77,     0,    78,     0,     0,
       0,    79,    80,    81,    82,    83,   166,    84,    85,     0,
       0,    86,    87,    88,    89,     0,     0,     0,     0,    77,
       0,    78,     0,     0,   158,    79,    80,    81,    82,    83,
       0,    84,    85,     0,     0,    86,    87,    88,    89,     0,
       0,     0,     0,    77,     0,    78,     0,     0,   161,    79,
      80,    81,    82,    83,     0,    84,    85,     0,     0,    86,
      87,    88,    89,     0,     0,    77,     0,    78,     0,     0,
     177,    79,    80,    81,    82,    83,     0,    84,    85,    77,
       0,    86,    87,    88,    89,    79,    80,    81,    82,    83,
       0,    84,    85,     0,     0,    86,    87,    88,    89,    79,
      80,    81,    82,    83,     0,    84,    85,     0,     0,    86,
      87,    88,    89,    79,    80,    81,    82,    83,     0,    -1,
      -1,     0,     0,    86,    87,    88,    89,    79,    80,    81,
      82,    83,     0,     0,     0,     0,     0,    -1,    -1,    -1,
      -1
};

static const yytype_int16 yycheck[] =
{
       2,    45,    14,     0,    95,    40,    34,    26,    41,    44,
      12,   102,     7,    15,    42,    41,    44,    12,   109,    14,
      15,    41,    34,    18,    26,    42,    39,    44,    41,    41,
      25,    26,    43,    45,    47,    48,    31,    32,    33,    34,
      35,    36,    61,    45,    39,    34,    41,    39,    50,    41,
      45,    34,    42,    55,    44,    47,    19,    20,    21,    34,
      42,   105,    44,    42,    66,    44,   157,    43,    42,   160,
      44,    43,    44,    16,    17,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,     7,    91,
     181,    40,    94,    16,    17,    42,    44,    34,    34,   101,
      34,    41,    37,   105,    42,    37,   108,    11,    41,    13,
      41,    65,   114,    17,    18,    19,    20,    21,    34,    23,
      24,   117,   166,    27,    28,    29,    30,    -1,   119,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    46,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,   154,    12,    -1,    14,    15,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   166,   167,    25,    26,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   183,    41,    -1,    43,    -1,    45,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    -1,    39,    -1,    41,    -1,    43,    12,    45,
      14,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    -1,    39,    40,    41,    -1,    -1,
      12,    45,    14,    15,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    16,    -1,    -1,    31,
      32,    33,    34,    35,    36,    25,    26,    39,    -1,    41,
      42,    -1,    12,    45,    14,    15,    -1,    -1,    18,    39,
      -1,    41,    -1,    -1,    -1,    25,    26,    47,    48,    -1,
      -1,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    41,    42,    -1,    12,    45,    14,    15,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    11,    41,    13,    -1,    -1,    45,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    -1,    27,    28,
      29,    30,    -1,    11,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    43,    23,    24,    -1,    -1,    27,
      28,    29,    30,    -1,    11,    -1,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    43,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    43,    23,    24,    -1,
      -1,    27,    28,    29,    30,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    42,    23,    24,    -1,
      -1,    27,    28,    29,    30,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    42,    23,    24,    -1,
      -1,    27,    28,    29,    30,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    42,    23,    24,    -1,
      -1,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    11,
      -1,    13,    -1,    -1,    40,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    -1,    27,    28,    29,    30,    -1,
      -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    40,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    -1,    27,
      28,    29,    30,    -1,    -1,    11,    -1,    13,    -1,    -1,
      38,    17,    18,    19,    20,    21,    -1,    23,    24,    11,
      -1,    27,    28,    29,    30,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    -1,    27,    28,    29,    30,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    -1,    27,
      28,    29,    30,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    -1,    27,    28,    29,    30,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    53,    54,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    12,    14,    15,    18,    25,    26,    31,    32,
      33,    34,    35,    36,    37,    39,    41,    43,    45,    55,
      57,    59,    60,    61,    62,    63,    64,    65,    66,    72,
      76,    79,    82,    84,    87,    88,    89,    91,    93,    95,
      41,    41,    41,    34,    78,    96,    56,    58,    60,    34,
      60,    41,    64,    66,    64,    77,    37,    40,    60,    70,
      71,    73,    74,    75,    60,    82,    34,    11,    13,    17,
      18,    19,    20,    21,    23,    24,    27,    28,    29,    30,
      43,    16,    25,    26,    39,    41,    47,    48,    67,    68,
      69,    39,    41,    47,    83,    90,    59,    60,    92,    94,
      60,    43,    43,    82,    54,    60,    40,    44,    40,    44,
      42,    42,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    42,    71,    34,
      34,    60,    42,    71,    34,    41,    80,    59,    42,    60,
      71,    43,    42,    38,    46,    70,    75,    41,    40,    42,
      41,    40,    42,    86,    76,    81,    42,    43,    60,    71,
      71,    34,    42,    44,    85,    59,    60,    38,    42,    42,
      34,    43,    71,    42,    59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    56,    55,    58,    57,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    61,    62,    63,    63,    63,    63,    63,    64,    64,
      64,    64,    65,    65,    65,    65,    66,    66,    66,    66,
      67,    67,    68,    68,    69,    70,    71,    71,    72,    72,
      72,    73,    74,    74,    75,    77,    76,    78,    78,    79,
      80,    81,    83,    82,    84,    84,    84,    84,    84,    84,
      85,    85,    86,    86,    87,    88,    90,    89,    89,    92,
      91,    94,    93,    96,    95
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       2,     2,     3,     4,     3,     4,     3,     4,     2,     6,
       1,     1,     3,     2,     5,     1,     1,     3,     2,     3,
       3,     1,     1,     3,     5,     0,     4,     1,     0,     2,
       3,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     4,     1,     0,     3,     2,     0,
       6,     0,    10,     0,     4
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
  case 5: /* $@1: %empty  */
#line 236 "parser.y"
              {if(for_flag == 0)yyerror("break w/o loop");}
#line 1530 "parser.c"
    break;

  case 7: /* $@2: %empty  */
#line 237 "parser.y"
                 {if(for_flag == 0)yyerror("continue w/o loop");}
#line 1536 "parser.c"
    break;

  case 9: /* stmt: expr SEMICOLON  */
#line 239 "parser.y"
                       {resettemp();}
#line 1542 "parser.c"
    break;

  case 18: /* stmt: SEMICOLON  */
#line 248 "parser.y"
                   {resettemp();}
#line 1548 "parser.c"
    break;

  case 19: /* expr: assignexpr  */
#line 252 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1554 "parser.c"
    break;

  case 20: /* expr: expr ADDITION expr  */
#line 253 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1560 "parser.c"
    break;

  case 21: /* expr: expr SUBTRACTION expr  */
#line 254 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1566 "parser.c"
    break;

  case 22: /* expr: expr MULTI expr  */
#line 255 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1572 "parser.c"
    break;

  case 23: /* expr: expr DIVISION expr  */
#line 256 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1578 "parser.c"
    break;

  case 24: /* expr: expr MODULUS expr  */
#line 257 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1584 "parser.c"
    break;

  case 25: /* expr: expr GRETER_THAN expr  */
#line 258 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater); emit_relop((yyval.exprValue),if_greater);}
#line 1590 "parser.c"
    break;

  case 26: /* expr: expr GRE_EQUAL expr  */
#line 259 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_geatereq); emit_relop((yyval.exprValue), if_geatereq);}
#line 1596 "parser.c"
    break;

  case 27: /* expr: expr LESS_THAN expr  */
#line 260 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_less);    emit_relop((yyval.exprValue),if_less);}
#line 1602 "parser.c"
    break;

  case 28: /* expr: expr LES_EQUAL expr  */
#line 261 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_lesseq);  emit_relop((yyval.exprValue),if_lesseq);}
#line 1608 "parser.c"
    break;

  case 29: /* expr: expr EQUAL expr  */
#line 262 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_eq);      emit_relop((yyval.exprValue),if_eq);}
#line 1614 "parser.c"
    break;

  case 30: /* expr: expr NOTEQUAL expr  */
#line 263 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_noteq);   emit_relop((yyval.exprValue),if_noteq);}
#line 1620 "parser.c"
    break;

  case 31: /* expr: expr AND expr  */
#line 264 "parser.y"
                                {(yyval.exprValue) =  boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),and);}
#line 1626 "parser.c"
    break;

  case 32: /* expr: expr OR expr  */
#line 265 "parser.y"
                                {(yyval.exprValue) =  boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),or);}
#line 1632 "parser.c"
    break;

  case 33: /* expr: term  */
#line 266 "parser.y"
                                {}
#line 1638 "parser.c"
    break;

  case 34: /* term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 269 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 1644 "parser.c"
    break;

  case 35: /* term: SUBTRACTION expr  */
#line 270 "parser.y"
                                             { //%prec: priority of uminus operator 
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = istempexpr((yyvsp[0].exprValue)) ? (yyvsp[0].exprValue)->sym : newtemp();
                emit(uminus,(yyvsp[0].exprValue),NULL,(yyval.exprValue),0,yylineno);
        }
#line 1655 "parser.c"
    break;

  case 36: /* term: NOT expr  */
#line 276 "parser.y"
                                {//not a
                (yyval.exprValue) = newexpr(boolexpr_e);
                (yyval.exprValue)->sym = newtemp();
                (yyval.exprValue)->boolConst = !(check_if_bool((yyvsp[0].exprValue)));
                emit(not, (yyvsp[0].exprValue), NULL, (yyval.exprValue),0, yylineno);
        }
#line 1666 "parser.c"
    break;

  case 37: /* term: INCREMENT lvalue  */
#line 282 "parser.y"
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
#line 1684 "parser.c"
    break;

  case 38: /* term: lvalue INCREMENT  */
#line 295 "parser.y"
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
#line 1703 "parser.c"
    break;

  case 39: /* term: DECREMENT lvalue  */
#line 309 "parser.y"
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
#line 1721 "parser.c"
    break;

  case 40: /* term: lvalue DECREMENT  */
#line 322 "parser.y"
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
#line 1740 "parser.c"
    break;

  case 41: /* term: primary  */
#line 336 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1746 "parser.c"
    break;

  case 42: /* assignexpr: lvalue ASSIGNMENT expr  */
#line 339 "parser.y"
                                  {
				{
					check_if_fuction((yyvsp[-2].exprValue)); //check gia to table
                                        //lvalue[i] = expr
					if((yyvsp[-2].exprValue)->type == tableitem_e) {
						emit(tablesetelem, (yyvsp[-2].exprValue), (yyvsp[-2].exprValue)->index, (yyvsp[0].exprValue), 0, yylineno);
						(yyval.exprValue) = emit_iftableitem((yyvsp[-2].exprValue));
						(yyval.exprValue)->type = assignexpr_e;
					} 
                                        // lvalue = expr
                                        else {
						emit(assign, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, currQuad, yylineno); //paizei na thelei ta arg anapoda
						(yyval.exprValue) = newexpr(assignexpr_e);
						(yyval.exprValue)->sym = newtemp();
                                                (yyvsp[-2].exprValue)->type = (yyvsp[0].exprValue)->type;
                                                copy_value((yyvsp[-2].exprValue),(yyvsp[0].exprValue));
						emit(assign, (yyval.exprValue), (yyvsp[-2].exprValue), NULL, currQuad, yylineno); //omoiws me to apo panw
                                                copy_value((yyval.exprValue),(yyvsp[-2].exprValue));
                                        }
				}               
        }
#line 1772 "parser.c"
    break;

  case 43: /* primary: lvalue  */
#line 362 "parser.y"
                                {(yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));}
#line 1778 "parser.c"
    break;

  case 44: /* primary: call  */
#line 363 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1784 "parser.c"
    break;

  case 45: /* primary: objectdef  */
#line 364 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1790 "parser.c"
    break;

  case 46: /* primary: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS  */
#line 365 "parser.y"
                                                   {
                (yyval.exprValue) = newexpr(programfunc_e);
                (yyval.exprValue)->sym = (yyvsp[-1].symbolEntry); 
        }
#line 1799 "parser.c"
    break;

  case 47: /* primary: const  */
#line 369 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1805 "parser.c"
    break;

  case 48: /* lvalue: member  */
#line 372 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1811 "parser.c"
    break;

  case 49: /* lvalue: ID  */
#line 373 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 1817 "parser.c"
    break;

  case 50: /* lvalue: LOCAL ID  */
#line 374 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 1823 "parser.c"
    break;

  case 51: /* lvalue: SCOPE_RESOLUTION ID  */
#line 375 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 1829 "parser.c"
    break;

  case 52: /* member: lvalue FULL_STOP ID  */
#line 378 "parser.y"
                                {// a.x;
                                (yyval.exprValue) = member_item((yyvsp[-2].exprValue),yylval.stringValue);}
#line 1836 "parser.c"
    break;

  case 53: /* member: lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET  */
#line 380 "parser.y"
                                                              { // a[3]
                                (yyvsp[-3].exprValue) = emit_iftableitem((yyvsp[-3].exprValue));
                                (yyval.exprValue) = newexpr(tableitem_e);
                                (yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
                                (yyval.exprValue)->index = (yyvsp[-1].exprValue); //index of expr
        }
#line 1847 "parser.c"
    break;

  case 56: /* call: call LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 392 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-2].exprValue),NULL);}
#line 1853 "parser.c"
    break;

  case 57: /* call: call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 394 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-3].exprValue), (yyvsp[-1].exprValue));}
#line 1859 "parser.c"
    break;

  case 58: /* call: lvalue callsuffix  */
#line 396 "parser.y"
                                {
                                (yyvsp[-1].exprValue) = emit_iftableitem((yyvsp[-1].exprValue));            //se periptwsh p htan table item
                                if((yyvsp[0].callValue)->method){
                                        expr* t = (yyvsp[-1].exprValue);
                                        (yyvsp[-1].exprValue) = emit_iftableitem(member_item(t, (yyvsp[0].callValue)->name));
                                        (yyvsp[0].callValue)->elist->next = t;           // insert san prwto arg (reversed,so last)
                                }
                                (yyval.exprValue) = make_call((yyvsp[-1].exprValue), (yyvsp[0].callValue)->elist);
        }
#line 1873 "parser.c"
    break;

  case 59: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 406 "parser.y"
                                {
                                expr* func = newexpr(programfunc_e);
                                func->sym = (yyvsp[-4].symbolEntry);
                                (yyval.exprValue) = make_call(func, (yyvsp[-1].exprValue));                
        }
#line 1883 "parser.c"
    break;

  case 60: /* callsuffix: normcall  */
#line 413 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue); }
#line 1889 "parser.c"
    break;

  case 61: /* callsuffix: methodcall  */
#line 414 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue);}
#line 1895 "parser.c"
    break;

  case 62: /* normcall: LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 417 "parser.y"
                                                          {
                                (yyval.callValue)->elist = (yyvsp[-1].exprValue);
                                (yyval.callValue)->method = 0;
                                (yyval.callValue)->name = NULL;
        }
#line 1905 "parser.c"
    break;

  case 63: /* normcall: LEFT_PARENTHESIS RIGHT_PARENTHESIS  */
#line 422 "parser.y"
                                             {
                                (yyval.callValue)->elist =NULL;
                                (yyval.callValue)->method = 0;
                                (yyval.callValue)->name = NULL;
        }
#line 1915 "parser.c"
    break;

  case 64: /* methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 429 "parser.y"
                                                                            {
                                (yyval.callValue)->elist = (yyvsp[-1].exprValue); 
                                (yyval.callValue)->method = 1;
                                (yyval.callValue)->name = (yyvsp[-3].stringValue);
        }
#line 1925 "parser.c"
    break;

  case 65: /* elist: expr  */
#line 436 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1931 "parser.c"
    break;

  case 66: /* moreElist: elist  */
#line 439 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1937 "parser.c"
    break;

  case 67: /* moreElist: moreElist COMMA elist  */
#line 440 "parser.y"
                                {
                                (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 1946 "parser.c"
    break;

  case 68: /* objectdef: LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET  */
#line 449 "parser.y"
                                {     //[]
                                (yyval.exprValue) = tablecreate_and_emit();
                                }
#line 1954 "parser.c"
    break;

  case 69: /* objectdef: LEFT_SQUARE_BRACKET moreElist RIGHT_SQUARE_BRACKET  */
#line 453 "parser.y"
                                {    // [20,30,"hello"]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),newexpr_constnum(i),(yyvsp[-1].exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                }                                                
                                }
#line 1968 "parser.c"
    break;

  case 70: /* objectdef: LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET  */
#line 463 "parser.y"
                             { //[{"x" : (fuction(s){print(s);})} ]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_indexed_length((yyvsp[-1].indexedValue));
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),(yyvsp[-1].indexedValue)->indexedelem,(yyvsp[-1].indexedValue)->value,0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                }   
        
        }
#line 1982 "parser.c"
    break;

  case 71: /* indexed: moreindexedelem  */
#line 474 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 1988 "parser.c"
    break;

  case 72: /* moreindexedelem: indexedelem  */
#line 478 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 1994 "parser.c"
    break;

  case 73: /* moreindexedelem: moreindexedelem COMMA indexedelem  */
#line 480 "parser.y"
                    {
                                (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2003 "parser.c"
    break;

  case 74: /* indexedelem: LEFT_CURLY_BRACKET expr COLON expr RIGHT_CURLY_BRACKET  */
#line 487 "parser.y"
                                 {
                                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2011 "parser.c"
    break;

  case 75: /* $@3: %empty  */
#line 493 "parser.y"
                           { 
                                CURR_SCOPE++;   
                        }
#line 2019 "parser.c"
    break;

  case 76: /* block: LEFT_CURLY_BRACKET $@3 stmt_list RIGHT_CURLY_BRACKET  */
#line 495 "parser.y"
                                                      {
                                if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
#line 2028 "parser.c"
    break;

  case 77: /* funcname: ID  */
#line 503 "parser.y"
                        {(yyval.stringValue) = (yyvsp[0].stringValue);}
#line 2034 "parser.c"
    break;

  case 78: /* funcname: %empty  */
#line 504 "parser.y"
                        {(yyval.stringValue) = newemptyfuncname();}
#line 2040 "parser.c"
    break;

  case 79: /* funcprefix: FUNCTION funcname  */
#line 508 "parser.y"
                          { 
                        (yyval.symbolEntry) = function_insert((yyvsp[0].stringValue));               //yylval.stringValue
                        (yyval.symbolEntry)->fuctionAddress = nextquadlabel();
                        emit(funcstart,lvalue_expr((yyval.symbolEntry)), NULL, NULL,0,yylineno);
                        push(save_fuctionlocals,currscopeoffset());
                        enterscopespace();                      // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2053 "parser.c"
    break;

  case 80: /* funcargs: LEFT_PARENTHESIS moreidilist RIGHT_PARENTHESIS  */
#line 519 "parser.y"
                                                       {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2063 "parser.c"
    break;

  case 81: /* funcbody: block  */
#line 527 "parser.y"
                        {
                        delete_last_fuction_scope();
                        (yyval.intValue) = currscopeoffset();
                        exitscopespace();
        }
#line 2073 "parser.c"
    break;

  case 82: /* $@4: %empty  */
#line 535 "parser.y"
                        { fuction_scope_insert(CURR_SCOPE++); }
#line 2079 "parser.c"
    break;

  case 83: /* funcdef: funcprefix $@4 funcargs funcbody  */
#line 536 "parser.y"
                          {
                        exitscopespace();
                        (yyvsp[-3].symbolEntry)->totalLocals = (yyvsp[0].intValue);
                        int offset = pop(save_fuctionlocals);
                        restorecurrscopeoffset(offset);
                        (yyval.symbolEntry) = (yyvsp[-3].symbolEntry);
                        emit(funcend, lvalue_expr((yyvsp[-3].symbolEntry)), NULL, NULL,0,yylineno);
        }
#line 2092 "parser.c"
    break;

  case 84: /* const: INTEGER  */
#line 546 "parser.y"
                                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2098 "parser.c"
    break;

  case 85: /* const: REAL  */
#line 547 "parser.y"
                                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2104 "parser.c"
    break;

  case 86: /* const: STRING  */
#line 548 "parser.y"
                                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2110 "parser.c"
    break;

  case 87: /* const: NILL  */
#line 549 "parser.y"
                                { (yyval.exprValue) = newexpr_nil();}
#line 2116 "parser.c"
    break;

  case 88: /* const: TRUE  */
#line 550 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('1');}
#line 2122 "parser.c"
    break;

  case 89: /* const: FALSE  */
#line 551 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('0');}
#line 2128 "parser.c"
    break;

  case 90: /* idlist: ID  */
#line 554 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2134 "parser.c"
    break;

  case 91: /* idlist: COMMA ID  */
#line 555 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2140 "parser.c"
    break;

  case 94: /* ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 563 "parser.y"
                                                        {
                emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), 0, nextquadlabel()+2, yylineno);   // prin htan(warning):emit(if_eq, $expr, newexpr_constbool(1), nextquadlabel()+2, 0, yylineno);
                (yyval.exprValue)->numConst = nextquadlabel(); 
                emit(jump, NULL, NULL, 0, 0, yylineno);
        }
#line 2150 "parser.c"
    break;

  case 95: /* elseprefix: ELSE  */
#line 569 "parser.y"
                        {
                (yyval.exprValue)->numConst = nextquadlabel();
                emit(jump, NULL, NULL, 0, 0, yylineno);
        }
#line 2159 "parser.c"
    break;

  case 96: /* $@5: %empty  */
#line 574 "parser.y"
                    {(yyvsp[0].exprValue)=(yyvsp[0].exprValue);}
#line 2165 "parser.c"
    break;

  case 97: /* ifstmt: ifprefix $@5 stmt  */
#line 574 "parser.y"
                                        {
        patchlabel((int)(yyvsp[-2].exprValue)->numConst, nextquadlabel());
        }
#line 2173 "parser.c"
    break;

  case 98: /* ifstmt: elseprefix stmt  */
#line 577 "parser.y"
                         {
                patchlabel((int)(yyvsp[-1].exprValue)->numConst, (int)(yyvsp[-1].exprValue)->numConst + 1);
                patchlabel((int)(yyvsp[-1].exprValue)->numConst, nextquadlabel());
        }
#line 2182 "parser.c"
    break;

  case 99: /* $@6: %empty  */
#line 584 "parser.y"
                                 {for_flag = 1;}
#line 2188 "parser.c"
    break;

  case 100: /* whilestmt: WHILE LEFT_PARENTHESIS $@6 expr RIGHT_PARENTHESIS stmt  */
#line 584 "parser.y"
                                                                             {for_flag = 0;}
#line 2194 "parser.c"
    break;

  case 101: /* $@7: %empty  */
#line 588 "parser.y"
                               {for_flag = 1;}
#line 2200 "parser.c"
    break;

  case 102: /* forstmt: FOR LEFT_PARENTHESIS $@7 moreElist SEMICOLON expr SEMICOLON moreElist RIGHT_PARENTHESIS stmt  */
#line 588 "parser.y"
                                                                                                                   {for_flag = 0;}
#line 2206 "parser.c"
    break;

  case 103: /* $@8: %empty  */
#line 591 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 2212 "parser.c"
    break;

  case 104: /* returnstmt: RETURN $@8 expr SEMICOLON  */
#line 591 "parser.y"
                                                                                      {}
#line 2218 "parser.c"
    break;


#line 2222 "parser.c"

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

#line 593 "parser.y"


int yyerror(char* yaccProvidedMessage){
                //provide error message example:
        fprintf(stderr,"%s: error at line %d , before token %s\n",yaccProvidedMessage,yylineno,yytext);
        error_flag = 1;
        //fprintf(stderr,"INVALID INPUT\n");
}



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
    init_lib_func();
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
   //print_format(); //print scopes
   //print_all_quads(); //print quads
    return 0;
}
