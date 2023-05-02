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
                        print_var(myvar);
                        inccurrscopeoffset();              
                } 
                /*an yparxei hdh */
                else{ 
                        //aneferomaste kai menei na doume an exoume prosvash
                        printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name); //TESTING PRINT
                        //check if we have access
                        if(check_access(myvar) == 0 && myvar->scope != 0){
                                yyerror("Cannot access var ");
                        }   
                } // einai hdh sto table
                return myvar;
        }

        /*check if global variable exist p.x. ::x (global x)*/
        void check_global(char *name){
                //kanoume lookup sto global scope 0
                var *myvar = lookup_scope(0,name);
                //an einai NULL den iparxei
                if (myvar == NULL ) {
                        yyerror("Global var not found");
                        return;
                }
                //alliws anaferomaste ekei
                printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name);  //TESTING PRINT
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
                print_var(myvar);
        }

        /*insert a new fuction to the table*/
        void function_insert(char* name){
                //check if anonymous and insert if true
                if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                        hash_insert(myfuction,table);
                        print_var(myfuction); //na ftiaksw ta print
                        return;
                }
                //kanoume lookuop sto trexon scope
                var* myVar = lookup_scope(CURR_SCOPE,name);
                
                // an yparxei collision me lib_fuct einai error
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                //an vrethei metavlhth h synarthsh einai error
                if(myVar != NULL){ 
                        yyerror("Already defined");
                        return;
                }
                //alliws thn kanoume insert
                myVar = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                hash_insert(myVar,table);
                print_var(myVar);
        }

        /*Insert a local var with name = name */
        var* insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
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
                print_var(currVar);
                //inccurrscopeoffset();
                return currVar;         
        }

        /*check if the curr string is '_' to create the next anonymous fuction*/
        char *check_anonymous(char *name){
                if(strcmp(name,"_") == 0){
                        char* str = malloc(sizeof(char) * 30);
		        sprintf(str,"_f%d",curr_anonymous++);
		        return str;
                }
                return NULL;
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
  YYSYMBOL_TRUE = 15,                      /* TRUE  */
  YYSYMBOL_FALSE = 16,                     /* FALSE  */
  YYSYMBOL_NILL = 17,                      /* NILL  */
  YYSYMBOL_ASSIGNMENT = 18,                /* ASSIGNMENT  */
  YYSYMBOL_ADDITION = 19,                  /* ADDITION  */
  YYSYMBOL_SUBTRACTION = 20,               /* SUBTRACTION  */
  YYSYMBOL_MULTI = 21,                     /* MULTI  */
  YYSYMBOL_DIVISION = 22,                  /* DIVISION  */
  YYSYMBOL_MODULUS = 23,                   /* MODULUS  */
  YYSYMBOL_EQUAL = 24,                     /* EQUAL  */
  YYSYMBOL_NOTEQUAL = 25,                  /* NOTEQUAL  */
  YYSYMBOL_INCREMENT = 26,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 27,                 /* DECREMENT  */
  YYSYMBOL_GRETER_THAN = 28,               /* GRETER_THAN  */
  YYSYMBOL_LESS_THAN = 29,                 /* LESS_THAN  */
  YYSYMBOL_GRE_EQUAL = 30,                 /* GRE_EQUAL  */
  YYSYMBOL_LES_EQUAL = 31,                 /* LES_EQUAL  */
  YYSYMBOL_INTEGER = 32,                   /* INTEGER  */
  YYSYMBOL_REAL = 33,                      /* REAL  */
  YYSYMBOL_STRING = 34,                    /* STRING  */
  YYSYMBOL_LEFT_CURLY_BRACKET = 35,        /* LEFT_CURLY_BRACKET  */
  YYSYMBOL_RIGHT_CURLY_BRACKET = 36,       /* RIGHT_CURLY_BRACKET  */
  YYSYMBOL_LEFT_SQUARE_BRACKET = 37,       /* LEFT_SQUARE_BRACKET  */
  YYSYMBOL_RIGHT_SQUARE_BRACKET = 38,      /* RIGHT_SQUARE_BRACKET  */
  YYSYMBOL_LEFT_PARENTHESIS = 39,          /* LEFT_PARENTHESIS  */
  YYSYMBOL_RIGHT_PARENTHESIS = 40,         /* RIGHT_PARENTHESIS  */
  YYSYMBOL_SEMICOLON = 41,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 42,                     /* COMMA  */
  YYSYMBOL_SCOPE_RESOLUTION = 43,          /* SCOPE_RESOLUTION  */
  YYSYMBOL_COLON = 44,                     /* COLON  */
  YYSYMBOL_FULL_STOP = 45,                 /* FULL_STOP  */
  YYSYMBOL_DOUBLE_FULL_STOP = 46,          /* DOUBLE_FULL_STOP  */
  YYSYMBOL_ID = 47,                        /* ID  */
  YYSYMBOL_EXTRA_CHARS = 48,               /* EXTRA_CHARS  */
  YYSYMBOL_LINE_COMMENT = 49,              /* LINE_COMMENT  */
  YYSYMBOL_BLOCK_COMMENT = 50,             /* BLOCK_COMMENT  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_program = 52,                   /* program  */
  YYSYMBOL_stmt_list = 53,                 /* stmt_list  */
  YYSYMBOL_brk_stm = 54,                   /* brk_stm  */
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_cnt_stm = 56,                   /* cnt_stm  */
  YYSYMBOL_57_2 = 57,                      /* $@2  */
  YYSYMBOL_stmt = 58,                      /* stmt  */
  YYSYMBOL_expr = 59,                      /* expr  */
  YYSYMBOL_term = 60,                      /* term  */
  YYSYMBOL_assignexpr = 61,                /* assignexpr  */
  YYSYMBOL_primary = 62,                   /* primary  */
  YYSYMBOL_lvalue = 63,                    /* lvalue  */
  YYSYMBOL_member = 64,                    /* member  */
  YYSYMBOL_call = 65,                      /* call  */
  YYSYMBOL_callsuffix = 66,                /* callsuffix  */
  YYSYMBOL_normcall = 67,                  /* normcall  */
  YYSYMBOL_methodcall = 68,                /* methodcall  */
  YYSYMBOL_elist = 69,                     /* elist  */
  YYSYMBOL_moreElist = 70,                 /* moreElist  */
  YYSYMBOL_objectdef = 71,                 /* objectdef  */
  YYSYMBOL_72_3 = 72,                      /* $@3  */
  YYSYMBOL_indexed = 73,                   /* indexed  */
  YYSYMBOL_moreindexedelem = 74,           /* moreindexedelem  */
  YYSYMBOL_indexedelem = 75,               /* indexedelem  */
  YYSYMBOL_block = 76,                     /* block  */
  YYSYMBOL_77_4 = 77,                      /* $@4  */
  YYSYMBOL_funcdef = 78,                   /* funcdef  */
  YYSYMBOL_79_5 = 79,                      /* $@5  */
  YYSYMBOL_80_6 = 80,                      /* $@6  */
  YYSYMBOL_81_7 = 81,                      /* $@7  */
  YYSYMBOL_82_8 = 82,                      /* $@8  */
  YYSYMBOL_83_9 = 83,                      /* $@9  */
  YYSYMBOL_84_10 = 84,                     /* $@10  */
  YYSYMBOL_const = 85,                     /* const  */
  YYSYMBOL_idlist = 86,                    /* idlist  */
  YYSYMBOL_moreidilist = 87,               /* moreidilist  */
  YYSYMBOL_ifstmt = 88,                    /* ifstmt  */
  YYSYMBOL_89_11 = 89,                     /* $@11  */
  YYSYMBOL_whilestmt = 90,                 /* whilestmt  */
  YYSYMBOL_91_12 = 91,                     /* $@12  */
  YYSYMBOL_forstmt = 92,                   /* forstmt  */
  YYSYMBOL_93_13 = 93,                     /* $@13  */
  YYSYMBOL_returnstmt = 94,                /* returnstmt  */
  YYSYMBOL_95_14 = 95                      /* $@14  */
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
#define YYLAST   528

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  186

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   220,   220,   223,   224,   228,   228,   229,   229,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   261,   262,   263,   264,   265,   266,
     267,   268,   271,   290,   291,   292,   293,   294,   297,   298,
     299,   300,   304,   305,   306,   307,   310,   311,   312,   316,
     317,   320,   323,   326,   329,   330,   331,   334,   334,   337,
     340,   344,   345,   348,   352,   352,   361,   367,   368,   361,
     375,   381,   381,   375,   387,   388,   389,   390,   391,   392,
     395,   396,   399,   400,   404,   404,   405,   408,   408,   412,
     412,   415,   415
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
  "LOCAL", "TRUE", "FALSE", "NILL", "ASSIGNMENT", "ADDITION",
  "SUBTRACTION", "MULTI", "DIVISION", "MODULUS", "EQUAL", "NOTEQUAL",
  "INCREMENT", "DECREMENT", "GRETER_THAN", "LESS_THAN", "GRE_EQUAL",
  "LES_EQUAL", "INTEGER", "REAL", "STRING", "LEFT_CURLY_BRACKET",
  "RIGHT_CURLY_BRACKET", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET",
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA",
  "SCOPE_RESOLUTION", "COLON", "FULL_STOP", "DOUBLE_FULL_STOP", "ID",
  "EXTRA_CHARS", "LINE_COMMENT", "BLOCK_COMMENT", "$accept", "program",
  "stmt_list", "brk_stm", "$@1", "cnt_stm", "$@2", "stmt", "expr", "term",
  "assignexpr", "primary", "lvalue", "member", "call", "callsuffix",
  "normcall", "methodcall", "elist", "moreElist", "objectdef", "$@3",
  "indexed", "moreindexedelem", "indexedelem", "block", "$@4", "funcdef",
  "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "const", "idlist",
  "moreidilist", "ifstmt", "$@11", "whilestmt", "$@12", "forstmt", "$@13",
  "returnstmt", "$@14", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-130)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -130,    21,   156,  -130,   -11,   156,   -10,    -6,    16,  -130,
    -130,  -130,   190,    36,  -130,  -130,  -130,   190,    -8,    -8,
    -130,  -130,  -130,  -130,    55,    10,  -130,    44,  -130,  -130,
    -130,  -130,   245,  -130,  -130,  -130,   174,  -130,    -5,  -130,
    -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,
    -130,  -130,     2,   190,     4,    52,  -130,  -130,    33,    88,
     -26,    -5,   -26,  -130,   190,   190,    58,    57,  -130,   314,
      60,  -130,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,  -130,   190,  -130,  -130,   190,
     190,    54,    56,  -130,  -130,  -130,   190,   190,    68,   190,
     190,   190,    63,  -130,   268,  -130,  -130,    64,   104,   219,
     443,  -130,   -24,  -130,    55,  -130,    66,   471,   458,    33,
      33,  -130,  -130,  -130,   484,   484,   497,   497,   497,   497,
     443,   380,     6,  -130,    78,   401,    18,  -130,   336,   358,
     -33,  -130,   -40,  -130,    66,  -130,   190,  -130,   190,  -130,
     190,  -130,  -130,   190,  -130,  -130,   156,   156,   190,   -40,
      75,  -130,    83,  -130,   422,  -130,    19,    24,  -130,  -130,
     291,    85,  -130,  -130,  -130,  -130,  -130,   190,  -130,    91,
      27,    91,  -130,   156,  -130,  -130
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,    80,   101,
       5,     7,     0,     0,    88,    89,    87,     0,     0,     0,
      85,    84,    86,    74,    67,     0,    18,     0,    49,    14,
      15,     3,     0,    33,    19,    41,    43,    48,    44,    45,
      16,    17,    47,    10,    11,    12,    13,    94,    96,    97,
      99,    76,     0,     0,     0,     0,    36,    50,    35,     0,
      37,     0,    39,     4,     0,    66,     0,    70,    71,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,     0,    38,    40,     0,
      66,     0,     0,    57,    59,    60,     0,    66,     0,     0,
       0,    66,     0,    93,     0,     6,     8,     0,     0,     0,
      63,    64,     0,    69,     0,    34,    46,    31,    32,    20,
      21,    22,    23,    24,    29,    30,    25,    27,    26,    28,
      42,     0,     0,    52,     0,     0,     0,    54,     0,     0,
       0,    93,    81,   102,     0,    75,     0,    68,     0,    72,
      66,    53,    61,    66,    55,    56,     0,     0,     0,    77,
       0,    90,     0,    92,     0,    65,     0,     0,    95,    98,
       0,     0,    91,    82,    73,    58,    62,    66,    78,     0,
       0,     0,    83,     0,    79,   100
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -130,  -130,    65,  -130,  -130,  -130,  -130,    -4,    -2,  -130,
    -130,  -130,    67,  -130,    70,  -130,  -130,  -130,   -19,   -85,
    -130,  -130,  -130,  -130,    13,  -129,  -130,   -21,  -130,  -130,
    -130,  -130,  -130,  -130,  -130,  -130,    -9,  -130,  -130,  -130,
    -130,  -130,  -130,  -130,  -130
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    29,    54,    30,    55,    31,   110,    33,
      34,    35,    36,    37,    38,    93,    94,    95,   111,   112,
      39,    65,    66,    67,    68,    40,    63,    41,   102,   171,
     181,    52,   162,   179,    42,   163,   142,    43,    99,    44,
     100,    45,   101,    46,    53
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    48,   160,    32,    70,   132,    13,   161,   158,   148,
      56,    89,   136,    90,   147,    58,   140,     8,   148,    91,
      92,     3,    12,    69,    13,    14,    15,    16,    47,    49,
      17,    59,    96,    50,    97,    27,    18,    19,   107,    28,
      98,   103,    20,    21,    22,   105,   152,    24,   148,    25,
     182,   104,   184,    27,    76,    77,    78,    28,   155,   175,
     148,   148,   109,    51,   176,   166,   148,   183,   167,   148,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    57,   130,    60,    62,   131,    61,    61,
      64,    71,   180,   106,   135,     8,   113,   138,   139,   114,
     116,   133,   141,   134,   144,   150,    32,     4,     5,     6,
       7,     8,     9,    10,    11,   137,    12,   153,    13,    14,
      15,    16,   172,   173,    17,   178,    23,   149,   108,   165,
      18,    19,   159,     0,     0,     0,    20,    21,    22,    23,
     145,    24,     0,    25,   164,    26,     0,    27,     0,     0,
       0,    28,   168,   169,    32,    32,   170,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,     0,    12,     0,
      13,    14,    15,    16,     0,     0,    17,     0,     0,   185,
       0,    32,    18,    19,     0,     0,     0,     0,    20,    21,
      22,    23,    86,    24,     0,    25,     0,    26,     0,    27,
      87,    88,    12,    28,    13,    14,    15,    16,     0,     0,
      17,    89,     0,    90,     0,     0,    18,    19,     0,    91,
      92,     0,    20,    21,    22,     0,     0,    24,     0,    25,
      72,     0,    73,    27,     0,     0,     0,    28,    74,    75,
      76,    77,    78,    79,    80,     0,     0,    81,    82,    83,
      84,     0,     0,     0,     0,     0,    72,     0,    73,     0,
       0,     0,     0,   146,    74,    75,    76,    77,    78,    79,
      80,     0,     0,    81,    82,    83,    84,     0,     0,    72,
       0,    73,     0,     0,     0,     0,    85,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
       0,     0,    72,     0,    73,     0,     0,     0,     0,   143,
      74,    75,    76,    77,    78,    79,    80,     0,     0,    81,
      82,    83,    84,     0,     0,    72,     0,    73,     0,     0,
       0,     0,   177,    74,    75,    76,    77,    78,    79,    80,
       0,     0,    81,    82,    83,    84,     0,    72,     0,    73,
       0,     0,     0,     0,   115,    74,    75,    76,    77,    78,
      79,    80,     0,     0,    81,    82,    83,    84,     0,    72,
       0,    73,     0,     0,     0,     0,   156,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
       0,    72,     0,    73,     0,     0,     0,     0,   157,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,    72,     0,    73,     0,     0,     0,   151,     0,
      74,    75,    76,    77,    78,    79,    80,     0,     0,    81,
      82,    83,    84,    72,     0,    73,     0,     0,     0,   154,
       0,    74,    75,    76,    77,    78,    79,    80,     0,     0,
      81,    82,    83,    84,    72,     0,    73,     0,   174,     0,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    72,
       0,    81,    82,    83,    84,     0,     0,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
      74,    75,    76,    77,    78,    79,    80,     0,     0,    81,
      82,    83,    84,    74,    75,    76,    77,    78,    -1,    -1,
       0,     0,    81,    82,    83,    84,    74,    75,    76,    77,
      78,     0,     0,     0,     0,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
       2,     5,    42,     5,    25,    90,    14,    47,    41,    42,
      12,    37,    97,    39,    38,    17,   101,     7,    42,    45,
      46,     0,    12,    25,    14,    15,    16,    17,    39,    39,
      20,    39,    37,    39,    39,    43,    26,    27,    59,    47,
      45,    39,    32,    33,    34,    41,    40,    37,    42,    39,
     179,    53,   181,    43,    21,    22,    23,    47,    40,    40,
      42,    42,    64,    47,    40,   150,    42,    40,   153,    42,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    47,    86,    18,    19,    89,    18,    19,
      35,    47,   177,    41,    96,     7,    38,    99,   100,    42,
      40,    47,    39,    47,    40,    39,   108,     3,     4,     5,
       6,     7,     8,     9,    10,    47,    12,    39,    14,    15,
      16,    17,    47,    40,    20,    40,    35,   114,    63,   148,
      26,    27,   141,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,   146,    41,    -1,    43,    -1,    -1,
      -1,    47,   156,   157,   156,   157,   158,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    20,    -1,    -1,   183,
      -1,   183,    26,    27,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    18,    37,    -1,    39,    -1,    41,    -1,    43,
      26,    27,    12,    47,    14,    15,    16,    17,    -1,    -1,
      20,    37,    -1,    39,    -1,    -1,    26,    27,    -1,    45,
      46,    -1,    32,    33,    34,    -1,    -1,    37,    -1,    39,
      11,    -1,    13,    43,    -1,    -1,    -1,    47,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,
      -1,    -1,    -1,    44,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    -1,    11,
      -1,    13,    -1,    -1,    -1,    -1,    41,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    -1,    11,    -1,    13,    -1,    -1,    -1,    -1,    41,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    -1,    11,    -1,    13,    -1,    -1,
      -1,    -1,    41,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    11,    -1,    13,
      -1,    -1,    -1,    -1,    40,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    28,    29,    30,    31,    -1,    11,
      -1,    13,    -1,    -1,    -1,    -1,    40,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    11,    -1,    13,    -1,    -1,    -1,    -1,    40,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    28,    29,
      30,    31,    11,    -1,    13,    -1,    -1,    -1,    38,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    11,    -1,    13,    -1,    -1,    -1,    38,
      -1,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    11,    -1,    13,    -1,    36,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    11,
      -1,    28,    29,    30,    31,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    52,    53,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    12,    14,    15,    16,    17,    20,    26,    27,
      32,    33,    34,    35,    37,    39,    41,    43,    47,    54,
      56,    58,    59,    60,    61,    62,    63,    64,    65,    71,
      76,    78,    85,    88,    90,    92,    94,    39,    58,    39,
      39,    47,    82,    95,    55,    57,    59,    47,    59,    39,
      63,    65,    63,    77,    35,    72,    73,    74,    75,    59,
      78,    47,    11,    13,    19,    20,    21,    22,    23,    24,
      25,    28,    29,    30,    31,    41,    18,    26,    27,    37,
      39,    45,    46,    66,    67,    68,    37,    39,    45,    89,
      91,    93,    79,    39,    59,    41,    41,    78,    53,    59,
      59,    69,    70,    38,    42,    40,    40,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    70,    47,    47,    59,    70,    47,    59,    59,
      70,    39,    87,    41,    40,    36,    44,    38,    42,    75,
      39,    38,    40,    39,    38,    40,    40,    40,    41,    87,
      42,    47,    83,    86,    59,    69,    70,    70,    58,    58,
      59,    80,    47,    40,    36,    40,    40,    41,    40,    84,
      70,    81,    76,    40,    76,    58
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    55,    54,    57,    56,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    62,    62,    62,    62,    62,    63,    63,
      63,    63,    64,    64,    64,    64,    65,    65,    65,    66,
      66,    67,    68,    69,    70,    70,    70,    72,    71,    71,
      73,    74,    74,    75,    77,    76,    79,    80,    81,    78,
      82,    83,    84,    78,    85,    85,    85,    85,    85,    85,
      86,    86,    87,    87,    89,    88,    88,    91,    90,    93,
      92,    95,    94
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       2,     2,     3,     4,     3,     4,     4,     2,     6,     1,
       1,     3,     5,     1,     1,     3,     0,     0,     4,     3,
       1,     1,     3,     5,     0,     4,     0,     0,     0,     9,
       0,     0,     0,     8,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     0,     6,     2,     0,     6,     0,
      10,     0,     4
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
#line 228 "parser.y"
              {if(for_flag == 0)yyerror("break w/o loop");}
#line 1509 "parser.c"
    break;

  case 7: /* $@2: %empty  */
#line 229 "parser.y"
                 {if(for_flag == 0)yyerror("continue w/o loop");}
#line 1515 "parser.c"
    break;

  case 19: /* expr: assignexpr  */
#line 244 "parser.y"
                                {}
#line 1521 "parser.c"
    break;

  case 20: /* expr: expr ADDITION expr  */
#line 245 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1527 "parser.c"
    break;

  case 21: /* expr: expr SUBTRACTION expr  */
#line 246 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1533 "parser.c"
    break;

  case 22: /* expr: expr MULTI expr  */
#line 247 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1539 "parser.c"
    break;

  case 23: /* expr: expr DIVISION expr  */
#line 248 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1545 "parser.c"
    break;

  case 24: /* expr: expr MODULUS expr  */
#line 249 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1551 "parser.c"
    break;

  case 25: /* expr: expr GRETER_THAN expr  */
#line 250 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1557 "parser.c"
    break;

  case 26: /* expr: expr GRE_EQUAL expr  */
#line 251 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1563 "parser.c"
    break;

  case 27: /* expr: expr LESS_THAN expr  */
#line 252 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1569 "parser.c"
    break;

  case 28: /* expr: expr LES_EQUAL expr  */
#line 253 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1575 "parser.c"
    break;

  case 29: /* expr: expr EQUAL expr  */
#line 254 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1581 "parser.c"
    break;

  case 30: /* expr: expr NOTEQUAL expr  */
#line 255 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1587 "parser.c"
    break;

  case 31: /* expr: expr AND expr  */
#line 256 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1593 "parser.c"
    break;

  case 32: /* expr: expr OR expr  */
#line 257 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1599 "parser.c"
    break;

  case 33: /* expr: term  */
#line 258 "parser.y"
                                {}
#line 1605 "parser.c"
    break;

  case 35: /* term: SUBTRACTION expr  */
#line 262 "parser.y"
                          {check_if_fuction((yyvsp[0].exprValue));}
#line 1611 "parser.c"
    break;

  case 36: /* term: NOT expr  */
#line 263 "parser.y"
                          {check_if_fuction((yyvsp[0].exprValue));}
#line 1617 "parser.c"
    break;

  case 37: /* term: INCREMENT lvalue  */
#line 264 "parser.y"
                          {check_if_fuction((yyvsp[0].exprValue));}
#line 1623 "parser.c"
    break;

  case 38: /* term: lvalue INCREMENT  */
#line 265 "parser.y"
                          {check_if_fuction((yyvsp[-1].exprValue));}
#line 1629 "parser.c"
    break;

  case 39: /* term: DECREMENT lvalue  */
#line 266 "parser.y"
                          {check_if_fuction((yyvsp[0].exprValue));}
#line 1635 "parser.c"
    break;

  case 40: /* term: lvalue DECREMENT  */
#line 267 "parser.y"
                          {check_if_fuction((yyvsp[-1].exprValue));}
#line 1641 "parser.c"
    break;

  case 42: /* assignexpr: lvalue ASSIGNMENT expr  */
#line 272 "parser.y"
                         {
				{
					check_if_fuction((yyvsp[-2].exprValue)); //check gia to table
					/*if($1->type == tableitem_e) {
						emit(tablesetelem, $1, $1->index, $3, 0, yylineno);
						$$ = emit_iftableitem($1);
						$$->type = assignexpr_e;
					} else {
                                        */
						emit(assign, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, 0, yylineno);
						(yyval.exprValue) = newexpr(assignexpr_e);
						(yyval.exprValue)->sym = newtemp();
						emit(assign, (yyval.exprValue), (yyvsp[-2].exprValue), NULL, 0, yylineno);
					
				}               
        }
#line 1662 "parser.c"
    break;

  case 48: /* lvalue: member  */
#line 297 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1668 "parser.c"
    break;

  case 49: /* lvalue: ID  */
#line 298 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue)); }
#line 1674 "parser.c"
    break;

  case 50: /* lvalue: LOCAL ID  */
#line 299 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));   }
#line 1680 "parser.c"
    break;

  case 51: /* lvalue: SCOPE_RESOLUTION ID  */
#line 300 "parser.y"
                                { check_global(yylval.stringValue);
                                }
#line 1687 "parser.c"
    break;

  case 58: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  */
#line 312 "parser.y"
                                                                                                {}
#line 1693 "parser.c"
    break;

  case 67: /* $@3: %empty  */
#line 334 "parser.y"
                                {
        //emit(tablecreate,newexpr(tableitem_e),NULL,NULL,1,yylineno);
        }
#line 1701 "parser.c"
    break;

  case 74: /* $@4: %empty  */
#line 352 "parser.y"
                           { 
                CURR_SCOPE++;   
                }
#line 1709 "parser.c"
    break;

  case 75: /* block: LEFT_CURLY_BRACKET $@4 stmt_list RIGHT_CURLY_BRACKET  */
#line 354 "parser.y"
                                              {
                        if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
#line 1718 "parser.c"
    break;

  case 76: /* $@5: %empty  */
#line 361 "parser.y"
                   {
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert(yylval.stringValue);  // insert to fuction
                fuction_scope_insert(CURR_SCOPE++);   // gia na kratame to teleutaio scope
        }
#line 1729 "parser.c"
    break;

  case 77: /* $@6: %empty  */
#line 367 "parser.y"
                            {enterscopespace(); }
#line 1735 "parser.c"
    break;

  case 78: /* $@7: %empty  */
#line 368 "parser.y"
                                 {
                        CURR_SCOPE--;
        }
#line 1743 "parser.c"
    break;

  case 79: /* funcdef: FUNCTION ID $@5 LEFT_PARENTHESIS moreidilist $@6 RIGHT_PARENTHESIS $@7 block  */
#line 370 "parser.y"
                {
                delete_last_fuction_scope();
                exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);
        }
#line 1753 "parser.c"
    break;

  case 80: /* $@8: %empty  */
#line 375 "parser.y"
                  {
                //no name fuct
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert("_");  //regognize anonymous fuctions
                fuction_scope_insert(CURR_SCOPE++);  
        }
#line 1765 "parser.c"
    break;

  case 81: /* $@9: %empty  */
#line 381 "parser.y"
                                     {enterscopespace(); }
#line 1771 "parser.c"
    break;

  case 82: /* $@10: %empty  */
#line 381 "parser.y"
                                                                              {
                CURR_SCOPE--;
        }
#line 1779 "parser.c"
    break;

  case 83: /* funcdef: FUNCTION $@8 LEFT_PARENTHESIS moreidilist $@9 RIGHT_PARENTHESIS $@10 block  */
#line 383 "parser.y"
              { delete_last_fuction_scope(); exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);}
#line 1786 "parser.c"
    break;

  case 84: /* const: REAL  */
#line 387 "parser.y"
               { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 1792 "parser.c"
    break;

  case 85: /* const: INTEGER  */
#line 388 "parser.y"
                 { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 1798 "parser.c"
    break;

  case 86: /* const: STRING  */
#line 389 "parser.y"
                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 1804 "parser.c"
    break;

  case 87: /* const: NILL  */
#line 390 "parser.y"
                { (yyval.exprValue) = newexpr_nil();}
#line 1810 "parser.c"
    break;

  case 88: /* const: TRUE  */
#line 391 "parser.y"
                { (yyval.exprValue) = newexpr_constbool(1);}
#line 1816 "parser.c"
    break;

  case 89: /* const: FALSE  */
#line 392 "parser.y"
                { (yyval.exprValue) = newexpr_constbool(0);}
#line 1822 "parser.c"
    break;

  case 90: /* idlist: ID  */
#line 395 "parser.y"
           {insert_formal(yylval.stringValue);}
#line 1828 "parser.c"
    break;

  case 91: /* idlist: COMMA ID  */
#line 396 "parser.y"
                 {insert_formal(yylval.stringValue);}
#line 1834 "parser.c"
    break;

  case 94: /* $@11: %empty  */
#line 404 "parser.y"
                           {if_flag = 1;}
#line 1840 "parser.c"
    break;

  case 95: /* ifstmt: IF LEFT_PARENTHESIS $@11 expr RIGHT_PARENTHESIS stmt  */
#line 404 "parser.y"
                                                                       { if_flag = 0;}
#line 1846 "parser.c"
    break;

  case 97: /* $@12: %empty  */
#line 408 "parser.y"
                                 {for_flag = 1;}
#line 1852 "parser.c"
    break;

  case 98: /* whilestmt: WHILE LEFT_PARENTHESIS $@12 expr RIGHT_PARENTHESIS stmt  */
#line 408 "parser.y"
                                                                             {for_flag = 0;}
#line 1858 "parser.c"
    break;

  case 99: /* $@13: %empty  */
#line 412 "parser.y"
                               {for_flag = 1;}
#line 1864 "parser.c"
    break;

  case 100: /* forstmt: FOR LEFT_PARENTHESIS $@13 moreElist SEMICOLON expr SEMICOLON moreElist RIGHT_PARENTHESIS stmt  */
#line 412 "parser.y"
                                                                                                                   {for_flag = 0;}
#line 1870 "parser.c"
    break;

  case 101: /* $@14: %empty  */
#line 415 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 1876 "parser.c"
    break;

  case 102: /* returnstmt: RETURN $@14 expr SEMICOLON  */
#line 415 "parser.y"
                                                                                      {}
#line 1882 "parser.c"
    break;


#line 1886 "parser.c"

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

#line 417 "parser.y"


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
   // print_format();
    return 0;
}
