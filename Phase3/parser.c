/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

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
        var * check_global(char *name){
                //kanoume lookup sto global scope 0
                var *myvar = lookup_scope(0,name);
                //an einai NULL den iparxei
                if (myvar == NULL ) {
                        yyerror("Global var not found");
                        return NULL;
                }
                //alliws anaferomaste ekei
                printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name);  //TESTING PRINT
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

#line 226 "parser.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    FOR = 261,
    FUNCTION = 262,
    RETURN = 263,
    BREAK = 264,
    CONTINUE = 265,
    AND = 266,
    NOT = 267,
    OR = 268,
    LOCAL = 269,
    TRUE = 270,
    FALSE = 271,
    NILL = 272,
    ASSIGNMENT = 273,
    ADDITION = 274,
    SUBTRACTION = 275,
    MULTI = 276,
    DIVISION = 277,
    MODULUS = 278,
    EQUAL = 279,
    NOTEQUAL = 280,
    INCREMENT = 281,
    DECREMENT = 282,
    GRETER_THAN = 283,
    LESS_THAN = 284,
    GRE_EQUAL = 285,
    LES_EQUAL = 286,
    INTEGER = 287,
    REAL = 288,
    STRING = 289,
    LEFT_CURLY_BRACKET = 290,
    RIGHT_CURLY_BRACKET = 291,
    LEFT_SQUARE_BRACKET = 292,
    RIGHT_SQUARE_BRACKET = 293,
    LEFT_PARENTHESIS = 294,
    RIGHT_PARENTHESIS = 295,
    SEMICOLON = 296,
    COMMA = 297,
    SCOPE_RESOLUTION = 298,
    COLON = 299,
    FULL_STOP = 300,
    DOUBLE_FULL_STOP = 301,
    ID = 302,
    EXTRA_CHARS = 303,
    LINE_COMMENT = 304,
    BLOCK_COMMENT = 305
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define WHILE 260
#define FOR 261
#define FUNCTION 262
#define RETURN 263
#define BREAK 264
#define CONTINUE 265
#define AND 266
#define NOT 267
#define OR 268
#define LOCAL 269
#define TRUE 270
#define FALSE 271
#define NILL 272
#define ASSIGNMENT 273
#define ADDITION 274
#define SUBTRACTION 275
#define MULTI 276
#define DIVISION 277
#define MODULUS 278
#define EQUAL 279
#define NOTEQUAL 280
#define INCREMENT 281
#define DECREMENT 282
#define GRETER_THAN 283
#define LESS_THAN 284
#define GRE_EQUAL 285
#define LES_EQUAL 286
#define INTEGER 287
#define REAL 288
#define STRING 289
#define LEFT_CURLY_BRACKET 290
#define RIGHT_CURLY_BRACKET 291
#define LEFT_SQUARE_BRACKET 292
#define RIGHT_SQUARE_BRACKET 293
#define LEFT_PARENTHESIS 294
#define RIGHT_PARENTHESIS 295
#define SEMICOLON 296
#define COMMA 297
#define SCOPE_RESOLUTION 298
#define COLON 299
#define FULL_STOP 300
#define DOUBLE_FULL_STOP 301
#define ID 302
#define EXTRA_CHARS 303
#define LINE_COMMENT 304
#define BLOCK_COMMENT 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 159 "parser.y"

        char* stringValue;
        int intValue;
        double realValue;
        struct var *exprNode;
        struct expr *exprValue;
        struct indexed *indexedValue;

#line 387 "parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYLAST   513

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  187

#define YYUNDEFTOK  2
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
       0,   225,   225,   228,   229,   233,   233,   234,   234,   236,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   266,   267,   268,   269,   270,
     271,   272,   273,   276,   296,   297,   298,   299,   300,   303,
     304,   305,   306,   309,   310,   311,   312,   315,   316,   317,
     320,   321,   324,   327,   330,   333,   334,   342,   345,   354,
     365,   369,   370,   376,   382,   382,   391,   397,   398,   391,
     405,   411,   411,   405,   417,   418,   419,   420,   421,   422,
     425,   426,   429,   430,   434,   434,   435,   438,   438,   442,
     442,   445,   445
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "TRUE",
  "FALSE", "NILL", "ASSIGNMENT", "ADDITION", "SUBTRACTION", "MULTI",
  "DIVISION", "MODULUS", "EQUAL", "NOTEQUAL", "INCREMENT", "DECREMENT",
  "GRETER_THAN", "LESS_THAN", "GRE_EQUAL", "LES_EQUAL", "INTEGER", "REAL",
  "STRING", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
  "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_PARENTHESIS",
  "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "SCOPE_RESOLUTION", "COLON",
  "FULL_STOP", "DOUBLE_FULL_STOP", "ID", "EXTRA_CHARS", "LINE_COMMENT",
  "BLOCK_COMMENT", "$accept", "program", "stmt_list", "brk_stm", "$@1",
  "cnt_stm", "$@2", "stmt", "$@3", "expr", "term", "assignexpr", "primary",
  "lvalue", "member", "call", "callsuffix", "normcall", "methodcall",
  "elist", "moreElist", "objectdef", "indexed", "moreindexedelem",
  "indexedelem", "block", "$@4", "funcdef", "$@5", "$@6", "$@7", "$@8",
  "$@9", "$@10", "const", "idlist", "moreidilist", "ifstmt", "$@11",
  "whilestmt", "$@12", "forstmt", "$@13", "returnstmt", "$@14", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

#define YYPACT_NINF (-127)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -127,     5,     3,  -127,   -15,   142,     8,    18,    38,  -127,
    -127,  -127,  -127,  -127,  -127,  -127,  -127,   188,  -127,  -127,
    -127,  -127,  -127,  -127,  -127,  -127,  -127,  -127,  -127,    51,
     188,    72,    73,  -127,   188,    48,  -127,  -127,  -127,   188,
     -10,   -10,  -127,  -127,  -127,   154,    92,    68,  -127,   243,
    -127,  -127,  -127,     9,  -127,   -17,  -127,  -127,   188,   188,
     188,    77,  -127,   266,  -127,  -127,    56,  -127,  -127,    66,
      24,   -20,   -17,   -20,   188,  -127,   441,  -127,    11,    12,
      78,  -127,   312,    81,  -127,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,  -127,   188,
    -127,  -127,   188,   188,    75,    76,  -127,  -127,  -127,   188,
     188,    80,   334,   356,    -1,  -127,   -26,  -127,  -127,    88,
     217,  -127,   188,  -127,    95,  -127,    93,   134,   456,    66,
      66,  -127,  -127,  -127,   469,   469,   482,   482,   482,   482,
     441,   378,    -8,  -127,    94,   399,    42,  -127,   142,   142,
     188,   -26,    89,  -127,    97,  -127,    93,   188,  -127,  -127,
     188,  -127,  -127,   188,  -127,  -127,  -127,  -127,   289,    98,
    -127,  -127,   420,    54,    58,   188,  -127,   105,  -127,  -127,
    -127,    61,   105,  -127,   142,  -127,  -127
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     9,     1,     0,     9,     0,     0,    80,   101,
       5,     7,    74,    19,    15,    16,     3,     0,    17,    18,
      11,    12,    13,    14,    94,    96,    97,    99,    76,     0,
       0,     0,     0,     4,     0,     0,    88,    89,    87,     0,
       0,     0,    84,    85,    86,     0,     0,     0,    50,     0,
      34,    20,    42,    44,    49,    45,    46,    48,     0,     0,
       0,     0,    93,     0,     6,     8,     9,    37,    51,    36,
       0,    38,     0,    40,     0,    67,    64,    65,     0,     0,
      70,    71,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,     0,
      39,    41,     0,     0,     0,     0,    58,    60,    61,     0,
       0,     0,     0,     0,     0,    93,    81,   102,    75,     0,
       0,    68,     0,    69,     0,    35,    47,    32,    33,    21,
      22,    23,    24,    25,    30,    31,    26,    28,    27,    29,
      43,     0,     0,    53,     0,     0,     0,    55,     9,     9,
       0,    77,     0,    90,     0,    92,     0,     0,    66,    72,
       0,    54,    62,     0,    56,    57,    95,    98,     0,     0,
      91,    82,     0,     0,     0,     0,    78,     0,    73,    59,
      63,     0,     0,    83,     9,    79,   100
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -127,  -127,   109,  -127,  -127,  -127,  -127,    -5,  -127,   -16,
    -127,  -127,  -127,    27,  -127,    70,  -127,  -127,  -127,    39,
     -58,  -127,  -127,  -127,    36,  -126,  -127,   -31,  -127,  -127,
    -127,  -127,  -127,  -127,  -127,  -127,    52,  -127,  -127,  -127,
    -127,  -127,  -127,  -127,  -127
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    14,    31,    15,    32,    16,    17,    76,
      50,    51,    52,    53,    54,    55,   106,   107,   108,    77,
      78,    56,    79,    80,    81,    18,    33,    19,    61,   169,
     182,    29,   154,   177,    57,   155,   116,    20,    58,    21,
      59,    22,    60,    23,    30
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    49,   114,    -2,    35,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    63,    83,   152,   102,    67,   103,
     109,   153,   110,    69,    24,   104,   105,    99,   111,    70,
      82,     8,   162,    47,   122,   100,   101,    48,    12,   119,
     150,   122,   112,   113,    13,   142,   102,    26,   103,   121,
     123,   183,   146,   122,   104,   105,   185,    27,   120,     4,
       5,     6,     7,     8,     9,    10,    11,    71,    73,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   165,   140,   122,    28,   141,    89,    90,    91,
      62,    12,   118,   145,   179,    68,   122,    13,   180,     8,
     122,   184,   173,   122,    34,   174,    35,    36,    37,    38,
      72,    72,    39,    64,    65,    84,   115,   181,    40,    41,
     124,   126,   143,   144,    42,    43,    44,   147,   156,    45,
      74,    46,   160,   163,   168,    47,   170,   171,   176,    48,
      12,   172,    66,   166,   167,     4,     5,     6,     7,     8,
       9,    10,    11,    87,    88,    89,    90,    91,    92,    93,
     159,   158,    94,    95,    96,    97,    34,   151,    35,    36,
      37,    38,     0,     0,    39,     0,     0,    12,     0,   186,
      40,    41,     0,    13,     0,     0,    42,    43,    44,    74,
       0,    45,    75,    46,     0,     0,     0,    47,     0,     0,
      34,    48,    35,    36,    37,    38,     0,     0,    39,     0,
       0,     0,     0,     0,    40,    41,     0,     0,     0,     0,
      42,    43,    44,     0,     0,    45,     0,    46,    85,     0,
      86,    47,     0,     0,     0,    48,    87,    88,    89,    90,
      91,    92,    93,     0,     0,    94,    95,    96,    97,     0,
       0,     0,     0,     0,    85,     0,    86,     0,     0,     0,
       0,   157,    87,    88,    89,    90,    91,    92,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    85,     0,    86,
       0,     0,     0,     0,    98,    87,    88,    89,    90,    91,
      92,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      85,     0,    86,     0,     0,     0,     0,   117,    87,    88,
      89,    90,    91,    92,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    85,     0,    86,     0,     0,     0,     0,
     175,    87,    88,    89,    90,    91,    92,    93,     0,     0,
      94,    95,    96,    97,     0,    85,     0,    86,     0,     0,
       0,     0,   125,    87,    88,    89,    90,    91,    92,    93,
       0,     0,    94,    95,    96,    97,     0,    85,     0,    86,
       0,     0,     0,     0,   148,    87,    88,    89,    90,    91,
      92,    93,     0,     0,    94,    95,    96,    97,     0,    85,
       0,    86,     0,     0,     0,     0,   149,    87,    88,    89,
      90,    91,    92,    93,     0,     0,    94,    95,    96,    97,
      85,     0,    86,     0,     0,     0,   161,     0,    87,    88,
      89,    90,    91,    92,    93,     0,     0,    94,    95,    96,
      97,    85,     0,    86,     0,     0,     0,   164,     0,    87,
      88,    89,    90,    91,    92,    93,     0,     0,    94,    95,
      96,    97,    85,     0,    86,     0,   178,     0,     0,     0,
      87,    88,    89,    90,    91,    92,    93,    85,     0,    94,
      95,    96,    97,     0,     0,    87,    88,    89,    90,    91,
      92,    93,     0,     0,    94,    95,    96,    97,    87,    88,
      89,    90,    91,    -3,    -3,     0,     0,    94,    95,    96,
      97,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      -3,    -3,    -3,    -3
};

static const yytype_int16 yycheck[] =
{
       5,    17,    60,     0,    14,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    30,    46,    42,    37,    34,    39,
      37,    47,    39,    39,    39,    45,    46,    18,    45,    39,
      46,     7,    40,    43,    42,    26,    27,    47,    35,    70,
      41,    42,    58,    59,    41,   103,    37,    39,    39,    38,
      38,   177,   110,    42,    45,    46,   182,    39,    74,     3,
       4,     5,     6,     7,     8,     9,    10,    40,    41,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    40,    99,    42,    47,   102,    21,    22,    23,
      39,    35,    36,   109,    40,    47,    42,    41,    40,     7,
      42,    40,   160,    42,    12,   163,    14,    15,    16,    17,
      40,    41,    20,    41,    41,    47,    39,   175,    26,    27,
      42,    40,    47,    47,    32,    33,    34,    47,    40,    37,
      35,    39,    39,    39,   150,    43,    47,    40,    40,    47,
      35,   157,    33,   148,   149,     3,     4,     5,     6,     7,
       8,     9,    10,    19,    20,    21,    22,    23,    24,    25,
     124,   122,    28,    29,    30,    31,    12,   115,    14,    15,
      16,    17,    -1,    -1,    20,    -1,    -1,    35,    -1,   184,
      26,    27,    -1,    41,    -1,    -1,    32,    33,    34,    35,
      -1,    37,    38,    39,    -1,    -1,    -1,    43,    -1,    -1,
      12,    47,    14,    15,    16,    17,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      32,    33,    34,    -1,    -1,    37,    -1,    39,    11,    -1,
      13,    43,    -1,    -1,    -1,    47,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    -1,
      -1,    44,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    -1,    11,    -1,    13,
      -1,    -1,    -1,    -1,    41,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    28,    29,    30,    31,    -1,    -1,
      11,    -1,    13,    -1,    -1,    -1,    -1,    41,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    -1,    11,    -1,    13,    -1,    -1,    -1,    -1,
      41,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    11,    -1,    13,    -1,    -1,
      -1,    -1,    40,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    11,    -1,    13,
      -1,    -1,    -1,    -1,    40,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    28,    29,    30,    31,    -1,    11,
      -1,    13,    -1,    -1,    -1,    -1,    40,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      11,    -1,    13,    -1,    -1,    -1,    38,    -1,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    11,    -1,    13,    -1,    -1,    -1,    38,    -1,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    28,    29,
      30,    31,    11,    -1,    13,    -1,    36,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    11,    -1,    28,
      29,    30,    31,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    28,    29,    30,    31,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      28,    29,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    52,    53,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    35,    41,    54,    56,    58,    59,    76,    78,
      88,    90,    92,    94,    39,    58,    39,    39,    47,    82,
      95,    55,    57,    77,    12,    14,    15,    16,    17,    20,
      26,    27,    32,    33,    34,    37,    39,    43,    47,    60,
      61,    62,    63,    64,    65,    66,    72,    85,    89,    91,
      93,    79,    39,    60,    41,    41,    53,    60,    47,    60,
      39,    64,    66,    64,    35,    38,    60,    70,    71,    73,
      74,    75,    60,    78,    47,    11,    13,    19,    20,    21,
      22,    23,    24,    25,    28,    29,    30,    31,    41,    18,
      26,    27,    37,    39,    45,    46,    67,    68,    69,    37,
      39,    45,    60,    60,    71,    39,    87,    41,    36,    78,
      60,    38,    42,    38,    42,    40,    40,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    71,    47,    47,    60,    71,    47,    40,    40,
      41,    87,    42,    47,    83,    86,    40,    44,    70,    75,
      39,    38,    40,    39,    38,    40,    58,    58,    60,    80,
      47,    40,    60,    71,    71,    41,    40,    84,    36,    40,
      40,    71,    81,    76,    40,    76,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    55,    54,    57,    56,    59,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    62,    63,    63,    63,    63,    63,    64,
      64,    64,    64,    65,    65,    65,    65,    66,    66,    66,
      67,    67,    68,    69,    70,    71,    71,    72,    72,    72,
      73,    74,    74,    75,    77,    76,    79,    80,    81,    78,
      82,    83,    84,    78,    85,    85,    85,    85,    85,    85,
      86,    86,    87,    87,    89,    88,    88,    91,    90,    93,
      92,    95,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     2,     2,     2,     2,
       2,     2,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     2,     2,     3,     4,     3,     4,     4,     2,     6,
       1,     1,     3,     5,     1,     1,     3,     2,     3,     3,
       1,     1,     3,     5,     0,     4,     0,     0,     0,     9,
       0,     0,     0,     8,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     0,     6,     2,     0,     6,     0,
      10,     0,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
  case 5:
#line 233 "parser.y"
              {if(for_flag == 0)yyerror("break w/o loop");}
#line 1767 "parser.c"
    break;

  case 7:
#line 234 "parser.y"
                 {if(for_flag == 0)yyerror("continue w/o loop");}
#line 1773 "parser.c"
    break;

  case 9:
#line 236 "parser.y"
      {resettemp();}
#line 1779 "parser.c"
    break;

  case 20:
#line 249 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1785 "parser.c"
    break;

  case 21:
#line 250 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1791 "parser.c"
    break;

  case 22:
#line 251 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1797 "parser.c"
    break;

  case 23:
#line 252 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1803 "parser.c"
    break;

  case 24:
#line 253 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1809 "parser.c"
    break;

  case 25:
#line 254 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1815 "parser.c"
    break;

  case 26:
#line 255 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1821 "parser.c"
    break;

  case 27:
#line 256 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1827 "parser.c"
    break;

  case 28:
#line 257 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1833 "parser.c"
    break;

  case 29:
#line 258 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1839 "parser.c"
    break;

  case 30:
#line 259 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1845 "parser.c"
    break;

  case 31:
#line 260 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1851 "parser.c"
    break;

  case 32:
#line 261 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1857 "parser.c"
    break;

  case 33:
#line 262 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1863 "parser.c"
    break;

  case 34:
#line 263 "parser.y"
                                {}
#line 1869 "parser.c"
    break;

  case 35:
#line 266 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 1875 "parser.c"
    break;

  case 36:
#line 267 "parser.y"
                          {check_arith((yyvsp[0].exprValue)); (yyval.exprValue) = newexpr(arithexpr_e);(yyval.exprValue)->sym = newtemp(); emit(uminus,(yyvsp[0].exprValue),NULL,(yyval.exprValue),currQuad,yylineno);}
#line 1881 "parser.c"
    break;

  case 37:
#line 268 "parser.y"
                          {check_arith((yyvsp[0].exprValue));}
#line 1887 "parser.c"
    break;

  case 38:
#line 269 "parser.y"
                          {check_arith((yyvsp[0].exprValue));}
#line 1893 "parser.c"
    break;

  case 39:
#line 270 "parser.y"
                          {check_arith((yyvsp[-1].exprValue));}
#line 1899 "parser.c"
    break;

  case 40:
#line 271 "parser.y"
                          {check_arith((yyvsp[0].exprValue));}
#line 1905 "parser.c"
    break;

  case 41:
#line 272 "parser.y"
                          {check_arith((yyvsp[-1].exprValue));}
#line 1911 "parser.c"
    break;

  case 42:
#line 273 "parser.y"
                          {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1917 "parser.c"
    break;

  case 43:
#line 276 "parser.y"
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
						emit(assign, (yyval.exprValue), (yyvsp[-2].exprValue), NULL, currQuad, yylineno); //omoiws me to apo panw
                                        }
				}               
        }
#line 1940 "parser.c"
    break;

  case 44:
#line 296 "parser.y"
                        {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1946 "parser.c"
    break;

  case 45:
#line 297 "parser.y"
                        {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1952 "parser.c"
    break;

  case 46:
#line 298 "parser.y"
                        {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1958 "parser.c"
    break;

  case 47:
#line 299 "parser.y"
                                                   {}
#line 1964 "parser.c"
    break;

  case 48:
#line 300 "parser.y"
                        {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1970 "parser.c"
    break;

  case 49:
#line 303 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1976 "parser.c"
    break;

  case 50:
#line 304 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 1982 "parser.c"
    break;

  case 51:
#line 305 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 1988 "parser.c"
    break;

  case 52:
#line 306 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 1994 "parser.c"
    break;

  case 59:
#line 317 "parser.y"
                                                                                                {}
#line 2000 "parser.c"
    break;

  case 64:
#line 330 "parser.y"
                                {                       (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2006 "parser.c"
    break;

  case 65:
#line 333 "parser.y"
                                {                       (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2012 "parser.c"
    break;

  case 66:
#line 334 "parser.y"
                                {
                                                        (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                                        (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 2021 "parser.c"
    break;

  case 67:
#line 342 "parser.y"
                                                 {      //[]
                                                        (yyval.exprValue) = tablecreate_and_emit();
                                                }
#line 2029 "parser.c"
    break;

  case 68:
#line 345 "parser.y"
                                                                 {    // [20,30,"hello"]
                                                        (yyval.exprValue) = tablecreate_and_emit();
                                                        int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                                        //for each item check its type and insert it to the table
                                                        for(i; i >= 0; i--){
                                                                emit(tablesetelem,(yyval.exprValue),newexpr_constnum(i),(yyvsp[-1].exprValue),0,yylineno); // emit (op,temp,index,value)
                                                                (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                                        }                                                
                                                }
#line 2043 "parser.c"
    break;

  case 69:
#line 354 "parser.y"
                                                             { //[{"x" : (fuction(s){print(s);})} ]
                                                        (yyval.exprValue) = tablecreate_and_emit();
                                                        int i = get_indexed_length((yyvsp[-1].indexedValue));
                                                        for(i; i >= 0; i--){
                                                                emit(tablesetelem,(yyval.exprValue),(yyvsp[-1].indexedValue)->indexedelem,(yyvsp[-1].indexedValue)->value,0,yylineno); // emit (op,temp,index,value)
                                                                (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                                        }   
        
        }
#line 2057 "parser.c"
    break;

  case 70:
#line 365 "parser.y"
                           {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2063 "parser.c"
    break;

  case 71:
#line 369 "parser.y"
                                           {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2069 "parser.c"
    break;

  case 72:
#line 370 "parser.y"
                                           {
                                                        (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                                        (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2078 "parser.c"
    break;

  case 73:
#line 376 "parser.y"
                                                                    {
                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2086 "parser.c"
    break;

  case 74:
#line 382 "parser.y"
                           { 
                CURR_SCOPE++;   
                }
#line 2094 "parser.c"
    break;

  case 75:
#line 384 "parser.y"
                                              {
                        if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
#line 2103 "parser.c"
    break;

  case 76:
#line 391 "parser.y"
                   {
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert(yylval.stringValue);  // insert to fuction
                fuction_scope_insert(CURR_SCOPE++);   // gia na kratame to teleutaio scope
        }
#line 2114 "parser.c"
    break;

  case 77:
#line 397 "parser.y"
                            {enterscopespace(); }
#line 2120 "parser.c"
    break;

  case 78:
#line 398 "parser.y"
                                 {
                        CURR_SCOPE--;
        }
#line 2128 "parser.c"
    break;

  case 79:
#line 400 "parser.y"
                {
                delete_last_fuction_scope();
                exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);
        }
#line 2138 "parser.c"
    break;

  case 80:
#line 405 "parser.y"
                  {
                //no name fuct
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert("_");  //regognize anonymous fuctions
                fuction_scope_insert(CURR_SCOPE++);  
        }
#line 2150 "parser.c"
    break;

  case 81:
#line 411 "parser.y"
                                     {enterscopespace(); }
#line 2156 "parser.c"
    break;

  case 82:
#line 411 "parser.y"
                                                                              {
                CURR_SCOPE--;
        }
#line 2164 "parser.c"
    break;

  case 83:
#line 413 "parser.y"
              { delete_last_fuction_scope(); exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);}
#line 2171 "parser.c"
    break;

  case 84:
#line 417 "parser.y"
                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2177 "parser.c"
    break;

  case 85:
#line 418 "parser.y"
                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2183 "parser.c"
    break;

  case 86:
#line 419 "parser.y"
                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2189 "parser.c"
    break;

  case 87:
#line 420 "parser.y"
                { (yyval.exprValue) = newexpr_nil();}
#line 2195 "parser.c"
    break;

  case 88:
#line 421 "parser.y"
                { (yyval.exprValue) = newexpr_constbool(1);}
#line 2201 "parser.c"
    break;

  case 89:
#line 422 "parser.y"
                { (yyval.exprValue) = newexpr_constbool(0);}
#line 2207 "parser.c"
    break;

  case 90:
#line 425 "parser.y"
           {insert_formal(yylval.stringValue);}
#line 2213 "parser.c"
    break;

  case 91:
#line 426 "parser.y"
                 {insert_formal(yylval.stringValue);}
#line 2219 "parser.c"
    break;

  case 94:
#line 434 "parser.y"
                           {if_flag = 1;}
#line 2225 "parser.c"
    break;

  case 95:
#line 434 "parser.y"
                                                                       { if_flag = 0;}
#line 2231 "parser.c"
    break;

  case 97:
#line 438 "parser.y"
                                 {for_flag = 1;}
#line 2237 "parser.c"
    break;

  case 98:
#line 438 "parser.y"
                                                                             {for_flag = 0;}
#line 2243 "parser.c"
    break;

  case 99:
#line 442 "parser.y"
                               {for_flag = 1;}
#line 2249 "parser.c"
    break;

  case 100:
#line 442 "parser.y"
                                                                                                                   {for_flag = 0;}
#line 2255 "parser.c"
    break;

  case 101:
#line 445 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 2261 "parser.c"
    break;

  case 102:
#line 445 "parser.y"
                                                                                      {}
#line 2267 "parser.c"
    break;


#line 2271 "parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
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
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 447 "parser.y"


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
   // print_format(); //print scopes
   print_all_quads(); //print quads
    return 0;
}
