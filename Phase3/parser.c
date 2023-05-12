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

#line 224 "parser.c"

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
    NILL = 270,
    ASSIGNMENT = 271,
    ADDITION = 272,
    SUBTRACTION = 273,
    MULTI = 274,
    DIVISION = 275,
    MODULUS = 276,
    EQUAL = 277,
    NOTEQUAL = 278,
    INCREMENT = 279,
    DECREMENT = 280,
    GRETER_THAN = 281,
    LESS_THAN = 282,
    GRE_EQUAL = 283,
    LES_EQUAL = 284,
    INTEGER = 285,
    REAL = 286,
    STRING = 287,
    ID = 288,
    TRUE = 289,
    FALSE = 290,
    LEFT_CURLY_BRACKET = 291,
    RIGHT_CURLY_BRACKET = 292,
    LEFT_SQUARE_BRACKET = 293,
    RIGHT_SQUARE_BRACKET = 294,
    LEFT_PARENTHESIS = 295,
    RIGHT_PARENTHESIS = 296,
    SEMICOLON = 297,
    COMMA = 298,
    SCOPE_RESOLUTION = 299,
    COLON = 300,
    FULL_STOP = 301,
    DOUBLE_FULL_STOP = 302,
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
#define NILL 270
#define ASSIGNMENT 271
#define ADDITION 272
#define SUBTRACTION 273
#define MULTI 274
#define DIVISION 275
#define MODULUS 276
#define EQUAL 277
#define NOTEQUAL 278
#define INCREMENT 279
#define DECREMENT 280
#define GRETER_THAN 281
#define LESS_THAN 282
#define GRE_EQUAL 283
#define LES_EQUAL 284
#define INTEGER 285
#define REAL 286
#define STRING 287
#define ID 288
#define TRUE 289
#define FALSE 290
#define LEFT_CURLY_BRACKET 291
#define RIGHT_CURLY_BRACKET 292
#define LEFT_SQUARE_BRACKET 293
#define RIGHT_SQUARE_BRACKET 294
#define LEFT_PARENTHESIS 295
#define RIGHT_PARENTHESIS 296
#define SEMICOLON 297
#define COMMA 298
#define SCOPE_RESOLUTION 299
#define COLON 300
#define FULL_STOP 301
#define DOUBLE_FULL_STOP 302
#define EXTRA_CHARS 303
#define LINE_COMMENT 304
#define BLOCK_COMMENT 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 157 "parser.y"

        char*   stringValue;
        int     intValue;
        unsigned int UintValue;
        double  realValue;
        char    boolValue;
        struct  var *symbolEntry;
        struct  expr *exprValue;
        struct  indexed *indexedValue;

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
#define YYLAST   551

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  181

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
       0,   226,   226,   229,   230,   234,   234,   235,   235,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   267,   268,   274,   280,   293,   307,
     320,   334,   337,   357,   358,   359,   360,   364,   367,   368,
     369,   370,   373,   375,   382,   383,   386,   387,   397,   404,
     405,   408,   415,   423,   426,   427,   435,   439,   449,   461,
     465,   466,   473,   480,   480,   491,   492,   496,   507,   515,
     523,   523,   564,   565,   566,   567,   568,   569,   572,   573,
     576,   577,   581,   581,   582,   585,   585,   589,   589,   592,
     592
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "NILL",
  "ASSIGNMENT", "ADDITION", "SUBTRACTION", "MULTI", "DIVISION", "MODULUS",
  "EQUAL", "NOTEQUAL", "INCREMENT", "DECREMENT", "GRETER_THAN",
  "LESS_THAN", "GRE_EQUAL", "LES_EQUAL", "INTEGER", "REAL", "STRING", "ID",
  "TRUE", "FALSE", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
  "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_PARENTHESIS",
  "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "SCOPE_RESOLUTION", "COLON",
  "FULL_STOP", "DOUBLE_FULL_STOP", "EXTRA_CHARS", "LINE_COMMENT",
  "BLOCK_COMMENT", "$accept", "program", "stmt_list", "brk_stm", "$@1",
  "cnt_stm", "$@2", "stmt", "expr", "term", "assignexpr", "primary",
  "lvalue", "member", "call", "callsuffix", "normcall", "methodcall",
  "elist", "moreElist", "objectdef", "indexed", "moreindexedelem",
  "indexedelem", "block", "$@3", "funcname", "funcprefix", "funcargs",
  "funcbody", "funcdef", "$@4", "const", "idlist", "moreidilist", "ifstmt",
  "$@5", "whilestmt", "$@6", "forstmt", "$@7", "returnstmt", "$@8", YY_NULLPTR
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

#define YYPACT_NINF (-91)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -91,    18,   158,   -91,   -18,   158,   -14,    -9,    17,   -91,
     -91,   -91,   235,    40,   -91,   235,   -12,   -12,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   204,     5,   -91,    56,   -91,
     -91,   -91,   263,   -91,   -91,   -91,    80,   -91,   -13,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   235,    20,    55,   -91,   -91,    27,
      87,   -32,   -13,   -32,   -91,   235,   -91,   483,   -91,   -34,
      62,    65,   -91,   341,    82,   -91,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   -91,
     235,   -91,   -91,   235,   235,    91,    95,   -91,   -91,   -91,
     235,   235,    96,    93,   235,   235,   235,   289,   -91,   -91,
      89,   107,   186,   -91,   235,   -91,    98,   -91,   108,   509,
     496,    27,    27,   -91,   -91,   -91,   522,   522,    39,    39,
      39,    39,   483,   416,    10,   -91,   110,   439,    28,   -91,
     -91,    99,   366,   391,    12,   -91,   108,   -91,   235,   -91,
     -91,   235,   -91,   -91,   235,   -91,   -91,     1,   -91,   -91,
     158,   158,   235,   462,    29,    49,   -91,   -91,   103,   -91,
     -91,   -91,   315,   -91,   -91,   -91,   -91,   235,    52,   158,
     -91
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,    76,    99,
       5,     7,     0,     0,    85,     0,     0,     0,    82,    83,
      84,    49,    86,    87,    73,     0,     0,    18,     0,    14,
      15,     3,     0,    33,    19,    41,    43,    48,    44,    45,
      16,    80,    17,    47,    10,    11,    12,    13,    92,    94,
      95,    97,    75,    77,     0,     0,     0,    36,    50,    35,
       0,    37,     0,    39,     4,     0,    66,    63,    64,     0,
       0,    69,    70,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,    38,    40,     0,     0,     0,     0,    57,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     8,
       0,     0,     0,    67,     0,    68,     0,    34,    46,    31,
      32,    20,    21,    22,    23,    24,    29,    30,    25,    27,
      26,    28,    42,     0,     0,    52,     0,     0,     0,    54,
      91,     0,     0,     0,     0,   100,     0,    74,     0,    65,
      71,     0,    53,    61,     0,    55,    56,     0,    79,    81,
       0,     0,     0,     0,     0,     0,    88,    78,     0,    90,
      93,    96,     0,    72,    58,    62,    89,     0,     0,     0,
      98
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -91,   -91,    88,   -91,   -91,   -91,   -91,    -4,    -2,   -91,
     -91,   -91,    83,   -91,    90,   -91,   -91,   -91,    41,   -90,
     -91,   -91,   -91,    37,    13,   -91,   -91,   -91,   -91,   -91,
     -19,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    29,    55,    30,    56,    31,    67,    33,
      34,    35,    36,    37,    38,    97,    98,    99,    68,    69,
      39,    70,    71,    72,    40,    64,    53,    41,   141,   159,
      42,   103,    43,   169,   157,    44,   104,    45,   105,    46,
     106,    47,    54
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    49,    13,    32,   134,   113,    93,    74,    94,   114,
      57,   138,     8,    59,    95,    96,   144,    12,     3,    13,
      14,    21,    48,    15,    73,   100,    50,   101,    60,    16,
      17,    51,    28,   102,   166,    18,    19,    20,    21,    22,
      23,   110,   167,    25,   168,    26,    80,    81,    82,    28,
      52,   153,   107,   114,   162,   114,    78,    79,    80,    81,
      82,   164,   108,   112,   165,    -1,    -1,    -1,    -1,   156,
     174,   114,   114,    58,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   178,   132,    75,
     175,   133,   114,   179,     8,   114,    90,   109,   137,    61,
      63,   115,   142,   143,    91,    92,    62,    62,   116,    32,
       4,     5,     6,     7,     8,     9,    10,    11,    93,    12,
      94,    13,    14,   118,   135,    15,    95,    96,   136,   139,
     146,    16,    17,   140,    65,    24,   176,    18,    19,    20,
      21,    22,    23,    24,   147,    25,   163,    26,   151,    27,
     154,    28,   111,   150,   158,   149,   170,   171,    32,    32,
     172,     4,     5,     6,     7,     8,     9,    10,    11,     0,
      12,     0,    13,    14,     0,   180,    15,    32,     0,     0,
       0,     0,    16,    17,     0,     0,     0,     0,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    76,    26,    77,
      27,     0,    28,    78,    79,    80,    81,    82,    83,    84,
       0,     0,    85,    86,    87,    88,    12,     0,    13,    14,
       0,     0,    15,     0,     0,     0,     0,     0,    16,    17,
       0,   148,     0,     0,    18,    19,    20,    21,    22,    23,
      65,     0,    25,    66,    26,     0,     0,    12,    28,    13,
      14,     0,     0,    15,     0,     0,     0,     0,     0,    16,
      17,     0,     0,     0,     0,    18,    19,    20,    21,    22,
      23,     0,     0,    25,    76,    26,    77,     0,     0,    28,
      78,    79,    80,    81,    82,    83,    84,     0,     0,    85,
      86,    87,    88,     0,     0,     0,     0,     0,     0,     0,
      76,     0,    77,     0,     0,    89,    78,    79,    80,    81,
      82,    83,    84,     0,     0,    85,    86,    87,    88,     0,
       0,     0,     0,     0,     0,     0,    76,     0,    77,     0,
       0,   145,    78,    79,    80,    81,    82,    83,    84,     0,
       0,    85,    86,    87,    88,     0,     0,     0,     0,     0,
       0,     0,    76,     0,    77,     0,     0,   177,    78,    79,
      80,    81,    82,    83,    84,     0,     0,    85,    86,    87,
      88,     0,     0,     0,     0,     0,     0,    76,     0,    77,
       0,     0,   117,    78,    79,    80,    81,    82,    83,    84,
       0,     0,    85,    86,    87,    88,     0,     0,     0,     0,
       0,     0,    76,     0,    77,     0,     0,   160,    78,    79,
      80,    81,    82,    83,    84,     0,     0,    85,    86,    87,
      88,     0,     0,     0,     0,     0,     0,    76,     0,    77,
       0,     0,   161,    78,    79,    80,    81,    82,    83,    84,
       0,     0,    85,    86,    87,    88,     0,     0,     0,     0,
      76,     0,    77,     0,     0,   152,    78,    79,    80,    81,
      82,    83,    84,     0,     0,    85,    86,    87,    88,     0,
       0,     0,     0,    76,     0,    77,     0,     0,   155,    78,
      79,    80,    81,    82,    83,    84,     0,     0,    85,    86,
      87,    88,     0,     0,    76,     0,    77,     0,     0,   173,
      78,    79,    80,    81,    82,    83,    84,    76,     0,    85,
      86,    87,    88,    78,    79,    80,    81,    82,    83,    84,
       0,     0,    85,    86,    87,    88,    78,    79,    80,    81,
      82,    83,    84,     0,     0,    85,    86,    87,    88,    78,
      79,    80,    81,    82,    -1,    -1,     0,     0,    85,    86,
      87,    88
};

static const yytype_int16 yycheck[] =
{
       2,     5,    14,     5,    94,    39,    38,    26,    40,    43,
      12,   101,     7,    15,    46,    47,   106,    12,     0,    14,
      15,    33,    40,    18,    26,    38,    40,    40,    40,    24,
      25,    40,    44,    46,    33,    30,    31,    32,    33,    34,
      35,    60,    41,    38,    43,    40,    19,    20,    21,    44,
      33,    41,    54,    43,    42,    43,    17,    18,    19,    20,
      21,   151,    42,    65,   154,    26,    27,    28,    29,    41,
      41,    43,    43,    33,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,   177,    90,    33,
      41,    93,    43,    41,     7,    43,    16,    42,   100,    16,
      17,    39,   104,   105,    24,    25,    16,    17,    43,   111,
       3,     4,     5,     6,     7,     8,     9,    10,    38,    12,
      40,    14,    15,    41,    33,    18,    46,    47,    33,    33,
      41,    24,    25,    40,    36,    36,    33,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   148,    40,    40,    42,
      40,    44,    64,   116,   141,   114,   160,   161,   160,   161,
     162,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    -1,   179,    18,   179,    -1,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    11,    40,    13,
      42,    -1,    44,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    28,    29,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
      -1,    45,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    -1,    -1,    12,    44,    14,
      15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,
      25,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    -1,    -1,    38,    11,    40,    13,    -1,    -1,    44,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    -1,    42,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,
      -1,    42,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    13,    -1,    -1,    42,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,
      -1,    -1,    41,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    13,    -1,    -1,    41,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,
      -1,    -1,    41,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    -1,    39,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    39,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    -1,    11,    -1,    13,    -1,    -1,    37,
      17,    18,    19,    20,    21,    22,    23,    11,    -1,    26,
      27,    28,    29,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    28,    29,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    26,    27,    28,    29,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    26,    27,
      28,    29
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    52,    53,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    12,    14,    15,    18,    24,    25,    30,    31,
      32,    33,    34,    35,    36,    38,    40,    42,    44,    54,
      56,    58,    59,    60,    61,    62,    63,    64,    65,    71,
      75,    78,    81,    83,    86,    88,    90,    92,    40,    58,
      40,    40,    33,    77,    93,    55,    57,    59,    33,    59,
      40,    63,    65,    63,    76,    36,    39,    59,    69,    70,
      72,    73,    74,    59,    81,    33,    11,    13,    17,    18,
      19,    20,    21,    22,    23,    26,    27,    28,    29,    42,
      16,    24,    25,    38,    40,    46,    47,    66,    67,    68,
      38,    40,    46,    82,    87,    89,    91,    59,    42,    42,
      81,    53,    59,    39,    43,    39,    43,    41,    41,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    70,    33,    33,    59,    70,    33,
      40,    79,    59,    59,    70,    42,    41,    37,    45,    69,
      74,    40,    39,    41,    40,    39,    41,    85,    75,    80,
      41,    41,    42,    59,    70,    70,    33,    41,    43,    84,
      58,    58,    59,    37,    41,    41,    33,    42,    70,    41,
      58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    55,    54,    57,    56,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    62,    62,    62,    62,    62,    63,    63,
      63,    63,    64,    64,    64,    64,    65,    65,    65,    66,
      66,    67,    68,    69,    70,    70,    71,    71,    71,    72,
      73,    73,    74,    76,    75,    77,    77,    78,    79,    80,
      82,    81,    83,    83,    83,    83,    83,    83,    84,    84,
      85,    85,    87,    86,    86,    89,    88,    91,    90,    93,
      92
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       2,     2,     3,     4,     3,     4,     4,     2,     6,     1,
       1,     3,     5,     1,     1,     3,     2,     3,     3,     1,
       1,     3,     5,     0,     4,     1,     0,     2,     3,     1,
       0,     4,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     0,     6,     2,     0,     6,     0,    10,     0,
       4
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
#line 234 "parser.y"
              {if(for_flag == 0)yyerror("break w/o loop");}
#line 1775 "parser.c"
    break;

  case 7:
#line 235 "parser.y"
                 {if(for_flag == 0)yyerror("continue w/o loop");}
#line 1781 "parser.c"
    break;

  case 9:
#line 237 "parser.y"
                       {resettemp();}
#line 1787 "parser.c"
    break;

  case 18:
#line 246 "parser.y"
                   {resettemp();}
#line 1793 "parser.c"
    break;

  case 19:
#line 250 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1799 "parser.c"
    break;

  case 20:
#line 251 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1805 "parser.c"
    break;

  case 21:
#line 252 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1811 "parser.c"
    break;

  case 22:
#line 253 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1817 "parser.c"
    break;

  case 23:
#line 254 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1823 "parser.c"
    break;

  case 24:
#line 255 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1829 "parser.c"
    break;

  case 25:
#line 256 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater);}
#line 1835 "parser.c"
    break;

  case 26:
#line 257 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_geatereq);}
#line 1841 "parser.c"
    break;

  case 27:
#line 258 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_less);}
#line 1847 "parser.c"
    break;

  case 28:
#line 259 "parser.y"
                                {(yyval.exprValue) =  do_maths((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_lesseq);}
#line 1853 "parser.c"
    break;

  case 29:
#line 260 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_eq);}
#line 1859 "parser.c"
    break;

  case 30:
#line 261 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_noteq);}
#line 1865 "parser.c"
    break;

  case 31:
#line 262 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),and);}
#line 1871 "parser.c"
    break;

  case 32:
#line 263 "parser.y"
                                {(yyval.exprValue) =  do_bool((yyvsp[-2].exprValue),(yyvsp[0].exprValue),or);}
#line 1877 "parser.c"
    break;

  case 33:
#line 264 "parser.y"
                                {}
#line 1883 "parser.c"
    break;

  case 34:
#line 267 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 1889 "parser.c"
    break;

  case 35:
#line 268 "parser.y"
                                { //-a
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = newtemp();
                emit(uminus,(yyvsp[0].exprValue),NULL,(yyval.exprValue),currQuad,yylineno);
        }
#line 1900 "parser.c"
    break;

  case 36:
#line 274 "parser.y"
                                {//not a
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = newtemp();
                emit(not, (yyvsp[0].exprValue), NULL, (yyval.exprValue), currQuad, yylineno);
        }
#line 1911 "parser.c"
    break;

  case 37:
#line 280 "parser.y"
                                { /*//++a
                check_arith($2);
                if($lvalue->type == tableitem_e){
                        $$ = emit_iftableitem($lvalue);
                        emit(add, $$, newexpr_constnum(1), $$, currQuad, yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, $$, currQuad, yylineno);
                }else{
                        emit(add, $lvalue, newexpr_constnum(1), $lvalue, currQuad, yylineno);
                        $$ = newexpr(arithexpr_e);
                        $$->sym = newtemp();
                        emit(assign, $lvalue, NULL, $$, currQuad, yylineno);
                }                                     //edw exw balei ayta xwris na kserw an einai
        */}
#line 1929 "parser.c"
    break;

  case 38:
#line 293 "parser.y"
                                { /*//a++
                check_arith($1);
                $$ = newexpr(var_e);
                $$->sym = newtemp();
                if($lvalue->type == tableitem_e){
                        exper* val = emit_iftableitem($1);
                        emit(assign, val, NULL, $$, currQuad, yylineno);
                        emit(add, val, newexpr_constnum(1), val);
                        emit(tablesetelem, $lvalue, $lvalue->index, val);
                }else{
                        emit(assign, $lvalue, NULL, $$);
                        emit(add, $lvalue, newexpr_constnum(1), $lvalue);
                }
        */}
#line 1948 "parser.c"
    break;

  case 39:
#line 307 "parser.y"
                                { /*//--a
                check_arith($2);
                if($lvalue->type == tableitem_e){
                        $$ = emit_iftableitem($lvalue);
                        emit(sub, $$, newexpr_constnum(1), $$, currQuad, yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, $$, currQuad, yylineno);
                }else{
                        emit(sub, $lvalue, newexpr_constnum(1), $lvalue, currQuad, yylineno);
                        $$ = newexpr(arithexpr_e);
                        $$->sym = newtemp();
                        emit(assign, $lvalue, NULL, $$, currQuad, yylineno);
                } 
        */}
#line 1966 "parser.c"
    break;

  case 40:
#line 320 "parser.y"
                                { /*//a--
                check_arith($1);
                $$ = newexpr(var_e);
                $$->sym = newtemp();
                if($lvalue->type == tableitem_e){
                        exper* val = emit_iftableitem($1);
                        emit(assign, val, NULL, $$, currQuad, yylineno);
                        emit(sub, val, newexpr_constnum(1), val);
                        emit(tablesetelem, $lvalue, $lvalue->index, val);
                }else{
                        emit(assign, $lvalue, NULL, $$);
                        emit(sub, $lvalue, newexpr_constnum(1), $lvalue);
                }
        */}
#line 1985 "parser.c"
    break;

  case 41:
#line 334 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 1991 "parser.c"
    break;

  case 42:
#line 337 "parser.y"
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
#line 2014 "parser.c"
    break;

  case 43:
#line 357 "parser.y"
                                {(yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));}
#line 2020 "parser.c"
    break;

  case 45:
#line 359 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2026 "parser.c"
    break;

  case 46:
#line 360 "parser.y"
                                                   {
                (yyval.exprValue) = newexpr(programfunc_e);
                (yyval.exprValue)->sym = (yyvsp[-1].symbolEntry); 
        }
#line 2035 "parser.c"
    break;

  case 47:
#line 364 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2041 "parser.c"
    break;

  case 48:
#line 367 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2047 "parser.c"
    break;

  case 49:
#line 368 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 2053 "parser.c"
    break;

  case 50:
#line 369 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 2059 "parser.c"
    break;

  case 51:
#line 370 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 2065 "parser.c"
    break;

  case 52:
#line 373 "parser.y"
                                {// a.x;
                                (yyval.exprValue) = member_item((yyvsp[-2].exprValue),yylval.stringValue);}
#line 2072 "parser.c"
    break;

  case 53:
#line 375 "parser.y"
                                                              { // a[3]
                                (yyvsp[-3].exprValue) = emit_iftableitem((yyvsp[-3].exprValue));
                                (yyval.exprValue) = newexpr(tableitem_e);
                                (yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
                                (yyval.exprValue)->index = (yyvsp[-1].exprValue); //index of expr
        }
#line 2083 "parser.c"
    break;

  case 57:
#line 387 "parser.y"
                          {}
#line 2089 "parser.c"
    break;

  case 58:
#line 397 "parser.y"
                                                                                                 {}
#line 2095 "parser.c"
    break;

  case 63:
#line 423 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2101 "parser.c"
    break;

  case 64:
#line 426 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2107 "parser.c"
    break;

  case 65:
#line 427 "parser.y"
                                {
                                (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 2116 "parser.c"
    break;

  case 66:
#line 436 "parser.y"
                             {      //[]
                                (yyval.exprValue) = tablecreate_and_emit();
                                                }
#line 2124 "parser.c"
    break;

  case 67:
#line 440 "parser.y"
                                {    // [20,30,"hello"]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),newexpr_constnum(i),(yyvsp[-1].exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                }                                                
                                }
#line 2138 "parser.c"
    break;

  case 68:
#line 450 "parser.y"
                             { //[{"x" : (fuction(s){print(s);})} ]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_indexed_length((yyvsp[-1].indexedValue));
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),(yyvsp[-1].indexedValue)->indexedelem,(yyvsp[-1].indexedValue)->value,0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                }   
        
        }
#line 2152 "parser.c"
    break;

  case 69:
#line 461 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2158 "parser.c"
    break;

  case 70:
#line 465 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2164 "parser.c"
    break;

  case 71:
#line 467 "parser.y"
                    {
                                (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2173 "parser.c"
    break;

  case 72:
#line 474 "parser.y"
                                 {
                                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2181 "parser.c"
    break;

  case 73:
#line 480 "parser.y"
                           { 
                                CURR_SCOPE++;   
                        }
#line 2189 "parser.c"
    break;

  case 74:
#line 482 "parser.y"
                                                      {
                                if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
#line 2198 "parser.c"
    break;

  case 75:
#line 491 "parser.y"
                {(yyval.stringValue) = (yyvsp[0].stringValue);}
#line 2204 "parser.c"
    break;

  case 76:
#line 492 "parser.y"
                {(yyval.stringValue) = newemptyfuncname();}
#line 2210 "parser.c"
    break;

  case 77:
#line 496 "parser.y"
                          { 
                (yyval.symbolEntry) = function_insert((yyvsp[0].stringValue));               //yylval.stringValue
                (yyval.symbolEntry)->fuctionAddress = nextquadlabel();
                emit(funcstart,lvalue_expr((yyval.symbolEntry)), NULL, NULL,0,yylineno);
                push(save_fuctionlocals,currscopeoffset());
                enterscopespace();                      // auksanoume to counter gia to ti var einai kata 1
                resetformalargsoffset();
        }
#line 2223 "parser.c"
    break;

  case 78:
#line 507 "parser.y"
                                                       {
                CURR_SCOPE--;
                enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                resetformalargsoffset();
        }
#line 2233 "parser.c"
    break;

  case 79:
#line 515 "parser.y"
              {
                delete_last_fuction_scope();
                (yyval.intValue) = currscopeoffset();
                exitscopespace();
        }
#line 2243 "parser.c"
    break;

  case 80:
#line 523 "parser.y"
                   { fuction_scope_insert(CURR_SCOPE++); }
#line 2249 "parser.c"
    break;

  case 81:
#line 524 "parser.y"
                          {
                exitscopespace();
                (yyvsp[-3].symbolEntry)->totalLocals = (yyvsp[0].intValue);
                int offset = pop(save_fuctionlocals);
                restorecurrscopeoffset(offset);
                (yyval.symbolEntry) = (yyvsp[-3].symbolEntry);
                emit(funcend, lvalue_expr((yyvsp[-3].symbolEntry)), NULL, NULL,0,yylineno);
        }
#line 2262 "parser.c"
    break;

  case 82:
#line 564 "parser.y"
                                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2268 "parser.c"
    break;

  case 83:
#line 565 "parser.y"
                                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2274 "parser.c"
    break;

  case 84:
#line 566 "parser.y"
                                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2280 "parser.c"
    break;

  case 85:
#line 567 "parser.y"
                                { (yyval.exprValue) = newexpr_nil();}
#line 2286 "parser.c"
    break;

  case 86:
#line 568 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('1');}
#line 2292 "parser.c"
    break;

  case 87:
#line 569 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('0');}
#line 2298 "parser.c"
    break;

  case 88:
#line 572 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2304 "parser.c"
    break;

  case 89:
#line 573 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2310 "parser.c"
    break;

  case 92:
#line 581 "parser.y"
                           {if_flag = 1;}
#line 2316 "parser.c"
    break;

  case 93:
#line 581 "parser.y"
                                                                       { if_flag = 0;}
#line 2322 "parser.c"
    break;

  case 95:
#line 585 "parser.y"
                                 {for_flag = 1;}
#line 2328 "parser.c"
    break;

  case 96:
#line 585 "parser.y"
                                                                             {for_flag = 0;}
#line 2334 "parser.c"
    break;

  case 97:
#line 589 "parser.y"
                               {for_flag = 1;}
#line 2340 "parser.c"
    break;

  case 98:
#line 589 "parser.y"
                                                                                                                   {for_flag = 0;}
#line 2346 "parser.c"
    break;

  case 99:
#line 592 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 2352 "parser.c"
    break;

  case 100:
#line 592 "parser.y"
                                                                                      {}
#line 2358 "parser.c"
    break;


#line 2362 "parser.c"

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
#line 594 "parser.y"


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
   print_format(); //print scopes
   //print_all_quads(); //print quads
    return 0;
}
