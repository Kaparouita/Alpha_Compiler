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
    UMINUS = 277,
    EQUAL = 278,
    NOTEQUAL = 279,
    INCREMENT = 280,
    DECREMENT = 281,
    GRETER_THAN = 282,
    LESS_THAN = 283,
    GRE_EQUAL = 284,
    LES_EQUAL = 285,
    INTEGER = 286,
    REAL = 287,
    STRING = 288,
    ID = 289,
    TRUE = 290,
    FALSE = 291,
    LEFT_CURLY_BRACKET = 292,
    RIGHT_CURLY_BRACKET = 293,
    LEFT_SQUARE_BRACKET = 294,
    RIGHT_SQUARE_BRACKET = 295,
    LEFT_PARENTHESIS = 296,
    RIGHT_PARENTHESIS = 297,
    SEMICOLON = 298,
    COMMA = 299,
    SCOPE_RESOLUTION = 300,
    COLON = 301,
    FULL_STOP = 302,
    DOUBLE_FULL_STOP = 303,
    EXTRA_CHARS = 304,
    LINE_COMMENT = 305,
    BLOCK_COMMENT = 306
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
#define UMINUS 277
#define EQUAL 278
#define NOTEQUAL 279
#define INCREMENT 280
#define DECREMENT 281
#define GRETER_THAN 282
#define LESS_THAN 283
#define GRE_EQUAL 284
#define LES_EQUAL 285
#define INTEGER 286
#define REAL 287
#define STRING 288
#define ID 289
#define TRUE 290
#define FALSE 291
#define LEFT_CURLY_BRACKET 292
#define RIGHT_CURLY_BRACKET 293
#define LEFT_SQUARE_BRACKET 294
#define RIGHT_SQUARE_BRACKET 295
#define LEFT_PARENTHESIS 296
#define RIGHT_PARENTHESIS 297
#define SEMICOLON 298
#define COMMA 299
#define SCOPE_RESOLUTION 300
#define COLON 301
#define FULL_STOP 302
#define DOUBLE_FULL_STOP 303
#define EXTRA_CHARS 304
#define LINE_COMMENT 305
#define BLOCK_COMMENT 306

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
        struct  stmt_t *stmtValue;
        struct  var *symbolEntry;
        struct  for_prefix *forValue;
        struct  expr *exprValue;
        struct  indexed *indexedValue;
        struct  call_s *callValue;

#line 392 "parser.c"

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
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   721

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  198

#define YYUNDEFTOK  2
#define YYMAXUTOK   306


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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   233,   233,   236,   240,   246,   250,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     284,   290,   293,   294,   300,   306,   319,   333,   346,   360,
     363,   386,   387,   388,   389,   393,   396,   397,   398,   399,
     402,   404,   411,   412,   415,   418,   420,   434,   439,   447,
     448,   451,   455,   460,   464,   469,   472,   473,   481,   485,
     495,   507,   511,   512,   519,   526,   526,   536,   545,   546,
     550,   564,   569,   577,   584,   588,   592,   592,   604,   605,
     606,   607,   608,   609,   612,   613,   616,   617,   621,   627,
     633,   640,   651,   652,   655,   658,   661,   669,   678,   679,
     680,   687,   698,   702,   713
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
  "UMINUS", "EQUAL", "NOTEQUAL", "INCREMENT", "DECREMENT", "GRETER_THAN",
  "LESS_THAN", "GRE_EQUAL", "LES_EQUAL", "INTEGER", "REAL", "STRING", "ID",
  "TRUE", "FALSE", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
  "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_PARENTHESIS",
  "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "SCOPE_RESOLUTION", "COLON",
  "FULL_STOP", "DOUBLE_FULL_STOP", "EXTRA_CHARS", "LINE_COMMENT",
  "BLOCK_COMMENT", "$accept", "program", "stmts", "brk_stm", "cnt_stm",
  "stmt", "expr", "term", "assignexpr", "primary", "lvalue", "member",
  "call", "callsuffix", "normcall", "methodcall", "elist", "moreElist",
  "objectdef", "indexed", "moreindexedelem", "indexedelem", "block", "$@1",
  "funcname", "funcprefix", "funcargs", "funcbody", "funcblockstart",
  "funcblockend", "funcdef", "$@2", "const", "idlist", "moreidilist",
  "ifprefix", "elseprefix", "ifstmt", "loopstart", "loopend", "loopstmt",
  "whilestart", "whilecond", "whilestmt", "N", "M", "forprefix", "forstmt",
  "returnstart", "returnstmt", YY_NULLPTR
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
     305,   306
};
# endif

#define YYPACT_NINF (-171)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     173,   -38,  -171,   -32,   -14,  -171,   -29,     1,   413,     7,
    -171,   413,    21,    21,  -171,  -171,  -171,  -171,  -171,  -171,
       9,   221,    -2,  -171,    19,    63,   173,  -171,  -171,  -171,
     442,  -171,  -171,  -171,    20,  -171,    13,  -171,  -171,  -171,
    -171,  -171,   173,  -171,    64,  -171,  -171,  -171,   253,  -171,
     413,   413,  -171,  -171,  -171,  -171,  -171,  -171,  -171,    86,
     -20,    13,   -20,  -171,   173,   413,  -171,   635,  -171,    29,
      58,    62,  -171,   505,    66,  -171,  -171,  -171,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,  -171,   413,  -171,  -171,   413,   285,    75,    76,  -171,
    -171,  -171,   413,   317,    77,    71,   109,   413,  -171,   413,
    -171,   463,   525,    57,    72,   129,   202,  -171,   413,  -171,
      79,  -171,    78,   663,   649,    51,    51,  -171,  -171,  -171,
     677,   677,   691,   691,   691,   691,   635,   565,  -171,    32,
    -171,    80,   589,  -171,    33,  -171,    81,  -171,  -171,   173,
     545,   173,  -171,    82,  -171,  -171,  -171,    78,  -171,   413,
    -171,  -171,   349,  -171,  -171,   381,  -171,  -171,  -171,   -27,
      85,  -171,  -171,  -171,  -171,   413,   613,  -171,    52,  -171,
      55,  -171,  -171,    84,  -171,  -171,  -171,  -171,  -171,   484,
    -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171
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
      50,     0,     0,    54,     0,    52,    97,    84,    99,     0,
       0,     0,   107,     0,   114,    98,   109,     0,    76,     0,
      67,    73,     0,    51,    61,     0,    53,    55,    82,     0,
       0,   101,   106,   103,   108,     0,     0,    57,     0,    64,
       0,    94,    81,     0,    96,    83,    85,   104,   102,     0,
      74,    58,    63,    95,    87,   108,   110,   111
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -171,  -171,    65,  -171,  -171,   -24,     0,  -171,  -171,  -171,
      44,  -171,    91,  -171,  -171,  -171,   -69,   -45,  -171,  -171,
    -171,     6,   -42,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
     -21,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
     -58,  -171,  -171,  -171,  -170,  -171,  -171,  -171,  -171,  -171
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    67,    31,    32,    33,
      34,    35,    36,    99,   100,   101,    68,    69,    37,    70,
      71,    72,    38,    64,    53,    39,   147,   186,   170,   194,
      40,   105,    41,   184,   169,    42,   149,    43,   151,   187,
     152,    44,   108,    45,   109,   175,    46,    47,    48,    49
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      30,    74,    77,    50,   188,     4,   113,   181,    56,    51,
       8,    58,     9,    10,    54,   182,    11,   183,   106,    95,
      52,    96,    73,    12,    13,   197,    30,    97,    98,    14,
      15,    16,    17,    18,    19,     9,    92,    21,   114,    22,
     153,    57,    30,    24,    55,    93,    94,    63,   111,   160,
     112,   139,   102,    75,   103,    17,    60,    62,   144,    95,
     104,    96,    59,    76,    30,   116,    24,    97,    98,   117,
      82,    83,    84,   118,   164,   167,   118,   118,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    77,   136,     4,   191,   137,   118,   192,   119,   118,
     156,   118,   142,    61,    61,   107,   120,   150,   122,   140,
     141,   145,   146,   148,   157,    30,    65,   178,   193,   162,
     180,   165,    20,   168,   174,   171,   161,   173,   185,   115,
     195,     0,     1,     0,     2,     3,     4,     5,     6,     7,
       0,     8,     0,     9,    10,     0,     0,    11,     0,    30,
       0,    30,     0,     0,    12,    13,     0,     0,     0,   176,
      14,    15,    16,    17,    18,    19,    20,   158,    21,     0,
      22,     0,    23,     0,    24,   189,     1,     0,     2,     3,
       4,     5,     6,     7,     0,     8,     0,     9,    10,     0,
       0,    11,     0,     0,     0,     0,     0,     0,    12,    13,
       0,     0,     0,     0,    14,    15,    16,    17,    18,    19,
      20,     0,    21,    78,    22,    79,    23,     0,    24,    80,
      81,    82,    83,    84,     0,    85,    86,     0,     0,    87,
      88,    89,    90,     8,     0,     9,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,    12,    13,   159,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    65,     0,
      21,    66,    22,     0,     0,     8,    24,     9,    10,     0,
       0,    11,     0,     0,     0,     0,     0,     0,    12,    13,
       0,     0,     0,     0,    14,    15,    16,    17,    18,    19,
       0,     0,    21,     0,    22,     0,   110,     8,    24,     9,
      10,     0,     0,    11,     0,     0,     0,     0,     0,     0,
      12,    13,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,     0,     0,    21,     0,    22,   138,     0,     8,
      24,     9,    10,     0,     0,    11,     0,     0,     0,     0,
       0,     0,    12,    13,     0,     0,     0,     0,    14,    15,
      16,    17,    18,    19,     0,     0,    21,     0,    22,   143,
       0,     8,    24,     9,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,    12,    13,     0,     0,     0,     0,
      14,    15,    16,    17,    18,    19,     0,     0,    21,     0,
      22,   177,     0,     8,    24,     9,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,    12,    13,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,     0,     0,
      21,     0,    22,   179,     0,     8,    24,     9,    10,     0,
       0,    11,     0,     0,     0,     0,     0,     0,    12,    13,
       0,     0,     0,     0,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    78,    22,    79,     0,     0,    24,    80,
      81,    82,    83,    84,     0,    85,    86,     0,     0,    87,
      88,    89,    90,     0,    78,     0,    79,     0,     0,     0,
      80,    81,    82,    83,    84,    91,    85,    86,     0,     0,
      87,    88,    89,    90,     0,    78,     0,    79,     0,     0,
       0,    80,    81,    82,    83,    84,   154,    85,    86,     0,
       0,    87,    88,    89,    90,     0,    78,     0,    79,     0,
       0,     0,    80,    81,    82,    83,    84,   196,    85,    86,
       0,     0,    87,    88,    89,    90,    78,     0,    79,     0,
       0,     0,    80,    81,    82,    83,    84,   121,    85,    86,
       0,     0,    87,    88,    89,    90,    78,     0,    79,     0,
       0,     0,    80,    81,    82,    83,    84,   155,    85,    86,
       0,     0,    87,    88,    89,    90,    78,     0,    79,     0,
       0,     0,    80,    81,    82,    83,    84,   172,    85,    86,
       0,     0,    87,    88,    89,    90,     0,     0,     0,     0,
      78,     0,    79,     0,     0,   163,    80,    81,    82,    83,
      84,     0,    85,    86,     0,     0,    87,    88,    89,    90,
       0,     0,     0,     0,    78,     0,    79,     0,     0,   166,
      80,    81,    82,    83,    84,     0,    85,    86,     0,     0,
      87,    88,    89,    90,     0,     0,    78,     0,    79,     0,
       0,   190,    80,    81,    82,    83,    84,     0,    85,    86,
      78,     0,    87,    88,    89,    90,    80,    81,    82,    83,
      84,     0,    85,    86,     0,     0,    87,    88,    89,    90,
      80,    81,    82,    83,    84,     0,    85,    86,     0,     0,
      87,    88,    89,    90,    80,    81,    82,    83,    84,     0,
      -1,    -1,     0,     0,    87,    88,    89,    90,    80,    81,
      82,    83,    84,     0,     0,     0,     0,     0,    -1,    -1,
      -1,    -1
};

static const yytype_int16 yycheck[] =
{
       0,    22,    26,    41,   174,     7,    51,    34,     8,    41,
      12,    11,    14,    15,    43,    42,    18,    44,    42,    39,
      34,    41,    22,    25,    26,   195,    26,    47,    48,    31,
      32,    33,    34,    35,    36,    14,    16,    39,    59,    41,
     109,    34,    42,    45,    43,    25,    26,    38,    48,   118,
      50,    96,    39,    34,    41,    34,    12,    13,   103,    39,
      47,    41,    41,     0,    64,    65,    45,    47,    48,    40,
      19,    20,    21,    44,    42,    42,    44,    44,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   115,    92,     7,    42,    95,    44,    42,    40,    44,
      43,    44,   102,    12,    13,    41,    44,   107,    42,    34,
      34,    34,    41,     4,    42,   115,    37,   162,    34,    41,
     165,    41,    37,    42,    42,   149,   120,   151,   170,    64,
     188,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    18,    -1,   149,
      -1,   151,    -1,    -1,    25,    26,    -1,    -1,    -1,   159,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      41,    -1,    43,    -1,    45,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    -1,    39,    11,    41,    13,    43,    -1,    45,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    -1,    27,
      28,    29,    30,    12,    -1,    14,    15,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    46,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    -1,
      39,    40,    41,    -1,    -1,    12,    45,    14,    15,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    41,    -1,    43,    12,    45,    14,
      15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    41,    42,    -1,    12,
      45,    14,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    41,    42,
      -1,    12,    45,    14,    15,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      41,    42,    -1,    12,    45,    14,    15,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    41,    42,    -1,    12,    45,    14,    15,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    11,    41,    13,    -1,    -1,    45,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    -1,    27,
      28,    29,    30,    -1,    11,    -1,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    43,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    11,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    43,    23,    24,    -1,
      -1,    27,    28,    29,    30,    -1,    11,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    43,    23,    24,
      -1,    -1,    27,    28,    29,    30,    11,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    42,    23,    24,
      -1,    -1,    27,    28,    29,    30,    11,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    42,    23,    24,
      -1,    -1,    27,    28,    29,    30,    11,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    42,    23,    24,
      -1,    -1,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    -1,    40,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    -1,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    40,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    -1,    11,    -1,    13,    -1,
      -1,    38,    17,    18,    19,    20,    21,    -1,    23,    24,
      11,    -1,    27,    28,    29,    30,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    -1,    27,    28,    29,    30,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    -1,    27,    28,    29,    30,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
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
      68,    73,    41,    40,    42,    41,    40,    42,    42,    86,
      80,    57,    42,    57,    42,    97,    58,    42,    69,    42,
      69,    34,    42,    44,    85,    74,    79,    91,    96,    58,
      38,    42,    42,    34,    81,    92,    43,    96
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     2,     2,
       3,     4,     3,     4,     3,     4,     2,     5,     6,     1,
       1,     3,     2,     5,     4,     1,     1,     3,     2,     3,
       3,     1,     1,     3,     5,     0,     4,     2,     1,     0,
       2,     3,     2,     1,     0,     0,     0,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     4,     1,
       2,     4,     0,     0,     3,     1,     3,     3,     0,     0,
       7,     7,     1,     2,     3
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
  case 3:
#line 236 "parser.y"
                   {
                (yyval.stmtValue)->breaklist = mergelist((yyvsp[-1].stmtValue)->breaklist, (yyvsp[0].stmtValue)->breaklist);
                (yyval.stmtValue)->contlist = mergelist((yyvsp[-1].stmtValue)->contlist, (yyvsp[0].stmtValue)->contlist);
                        }
#line 1825 "parser.c"
    break;

  case 4:
#line 240 "parser.y"
              {
                (yyval.stmtValue) = ((yyvsp[0].stmtValue)) ? (yyvsp[0].stmtValue) : stmt_constractor(0,0);
        }
#line 1833 "parser.c"
    break;

  case 5:
#line 246 "parser.y"
                        {       (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->breaklist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        }
#line 1842 "parser.c"
    break;

  case 6:
#line 250 "parser.y"
                           {    (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->contlist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        }
#line 1851 "parser.c"
    break;

  case 7:
#line 255 "parser.y"
                       {(yyval.stmtValue) = stmt_constractor(0,0); resettemp();}
#line 1857 "parser.c"
    break;

  case 8:
#line 256 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1863 "parser.c"
    break;

  case 9:
#line 257 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1869 "parser.c"
    break;

  case 10:
#line 258 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1875 "parser.c"
    break;

  case 11:
#line 259 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1881 "parser.c"
    break;

  case 12:
#line 260 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1887 "parser.c"
    break;

  case 13:
#line 261 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1893 "parser.c"
    break;

  case 14:
#line 262 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1899 "parser.c"
    break;

  case 15:
#line 263 "parser.y"
                     {(yyval.stmtValue) = stmt_constractor(0,0);}
#line 1905 "parser.c"
    break;

  case 16:
#line 264 "parser.y"
                   {resettemp(); (yyval.stmtValue) = stmt_constractor(0,0);}
#line 1911 "parser.c"
    break;

  case 17:
#line 268 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1917 "parser.c"
    break;

  case 18:
#line 269 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1923 "parser.c"
    break;

  case 19:
#line 270 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1929 "parser.c"
    break;

  case 20:
#line 271 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1935 "parser.c"
    break;

  case 21:
#line 272 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1941 "parser.c"
    break;

  case 22:
#line 273 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1947 "parser.c"
    break;

  case 23:
#line 274 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater); emit_relop((yyval.exprValue),if_greater);}
#line 1953 "parser.c"
    break;

  case 24:
#line 275 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_geatereq); emit_relop((yyval.exprValue), if_geatereq);}
#line 1959 "parser.c"
    break;

  case 25:
#line 276 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_less);    emit_relop((yyval.exprValue),if_less);}
#line 1965 "parser.c"
    break;

  case 26:
#line 277 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_lesseq);  emit_relop((yyval.exprValue),if_lesseq);}
#line 1971 "parser.c"
    break;

  case 27:
#line 278 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_eq);      emit_relop((yyval.exprValue),if_eq);}
#line 1977 "parser.c"
    break;

  case 28:
#line 279 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_noteq);   emit_relop((yyval.exprValue),if_noteq);}
#line 1983 "parser.c"
    break;

  case 29:
#line 280 "parser.y"
                                {       (yyval.exprValue) =   newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),and)) ; 
                                        (yyval.exprValue)->sym = newtemp();     
                                        emit(and,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno);
                                }
#line 1992 "parser.c"
    break;

  case 30:
#line 284 "parser.y"
                                {       (yyval.exprValue) = newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),or)); 
                                        (yyval.exprValue)->sym = newtemp();       
                                        emit(or,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno); // epishs auto htan nextquad +2
                                        /*emit(jump,NULL,NULL,NULL,nextquadlabel()+3,yylineno);
                                        emit_relop($$,or); //htan to idio kai sthn and/not nmzw apla ta vgazoume k gg */
                                }
#line 2003 "parser.c"
    break;

  case 31:
#line 290 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2009 "parser.c"
    break;

  case 32:
#line 293 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 2015 "parser.c"
    break;

  case 33:
#line 294 "parser.y"
                                             { //%prec: priority of uminus operator 
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = istempexpr((yyvsp[0].exprValue)) ? (yyvsp[0].exprValue)->sym : newtemp();
                emit(uminus,(yyvsp[0].exprValue),NULL,(yyval.exprValue),0,yylineno);
        }
#line 2026 "parser.c"
    break;

  case 34:
#line 300 "parser.y"
                                {//not a
                (yyval.exprValue) = newexpr(constbool_e);
                (yyval.exprValue)->sym = newtemp();
                (yyval.exprValue)->boolConst = !(check_if_bool((yyvsp[0].exprValue)));
                emit(not,NULL,(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno);
        }
#line 2037 "parser.c"
    break;

  case 35:
#line 306 "parser.y"
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
#line 2055 "parser.c"
    break;

  case 36:
#line 319 "parser.y"
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
#line 2074 "parser.c"
    break;

  case 37:
#line 333 "parser.y"
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
#line 2092 "parser.c"
    break;

  case 38:
#line 346 "parser.y"
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
#line 2111 "parser.c"
    break;

  case 39:
#line 360 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2117 "parser.c"
    break;

  case 40:
#line 363 "parser.y"
                                  {
				{
					//check_if_fuction($1); //check gia to table
                                        //lvalue[i] = expr
					if((yyvsp[-2].exprValue)->type == tableitem_e) {
						emit(tablesetelem, (yyvsp[-2].exprValue), (yyvsp[-2].exprValue)->index, (yyvsp[0].exprValue), 0, yylineno);
						(yyval.exprValue) = emit_iftableitem((yyvsp[-2].exprValue));
						(yyval.exprValue)->type = assignexpr_e;
					} 
                                        // lvalue = expr
                                        else {
						emit(assign, (yyvsp[0].exprValue), NULL,(yyvsp[-2].exprValue), 0, yylineno); //paizei na thelei ta arg anapoda
						(yyval.exprValue) = newexpr(assignexpr_e);
						(yyval.exprValue)->sym = newtemp();
                                                (yyvsp[-2].exprValue)->type = (yyvsp[0].exprValue)->type;
                                               // copy_value($1,$3);
						emit(assign ,(yyvsp[-2].exprValue), NULL, (yyval.exprValue),0, yylineno); //omoiws me to apo panw
                                                //copy_value($assignexpr,$1);
                                        }
				}               
        }
#line 2143 "parser.c"
    break;

  case 41:
#line 386 "parser.y"
                                {(yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));}
#line 2149 "parser.c"
    break;

  case 42:
#line 387 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2155 "parser.c"
    break;

  case 43:
#line 388 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2161 "parser.c"
    break;

  case 44:
#line 389 "parser.y"
                                                   {
                (yyval.exprValue) = newexpr(programfunc_e);
                (yyval.exprValue)->sym = (yyvsp[-1].symbolEntry); 
        }
#line 2170 "parser.c"
    break;

  case 45:
#line 393 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2176 "parser.c"
    break;

  case 46:
#line 396 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2182 "parser.c"
    break;

  case 47:
#line 397 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 2188 "parser.c"
    break;

  case 48:
#line 398 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 2194 "parser.c"
    break;

  case 49:
#line 399 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 2200 "parser.c"
    break;

  case 50:
#line 402 "parser.y"
                                {// a.x;
                                (yyval.exprValue) = member_item((yyvsp[-2].exprValue),yylval.stringValue);}
#line 2207 "parser.c"
    break;

  case 51:
#line 404 "parser.y"
                                                              { // a[3]
                                (yyvsp[-3].exprValue) = emit_iftableitem((yyvsp[-3].exprValue));
                                (yyval.exprValue) = newexpr(tableitem_e);
                                (yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
                                (yyval.exprValue)->index = (yyvsp[-1].exprValue); //index of expr
        }
#line 2218 "parser.c"
    break;

  case 54:
#line 416 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-2].exprValue),NULL);
                                }
#line 2225 "parser.c"
    break;

  case 55:
#line 419 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-3].exprValue), (yyvsp[-1].exprValue));}
#line 2231 "parser.c"
    break;

  case 56:
#line 421 "parser.y"
                                {
                                (yyvsp[-1].exprValue) = emit_iftableitem((yyvsp[-1].exprValue));            //se periptwsh p htan table item
                                if((yyvsp[0].callValue)->method){
                                        expr* t = (yyvsp[-1].exprValue);
                                        (yyvsp[-1].exprValue) = emit_iftableitem(member_item(t, (yyvsp[0].callValue)->name));
                                        if((yyvsp[0].callValue))//!EDW isws prepei na to treksw mexri to telos k meta na to kanw insert?
                                                (yyvsp[0].callValue)->elist->next = t;           // insert san prwto arg (reversed,so last)
                                        else
                                                (yyvsp[0].callValue)->elist = t;
                                }
                                (yyval.exprValue) = make_call((yyvsp[-1].exprValue), (yyvsp[0].callValue)->elist);
        }
#line 2248 "parser.c"
    break;

  case 57:
#line 434 "parser.y"
                                                                                       {
                                expr* func = newexpr(programfunc_e);
				func->sym = (yyvsp[-3].symbolEntry);
				(yyval.exprValue) = make_call(func, NULL);
        }
#line 2258 "parser.c"
    break;

  case 58:
#line 440 "parser.y"
                                {
                                expr* func = newexpr(programfunc_e);
                                func->sym = (yyvsp[-4].symbolEntry);
                                (yyval.exprValue) = make_call(func, (yyvsp[-1].exprValue));                
        }
#line 2268 "parser.c"
    break;

  case 59:
#line 447 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue); }
#line 2274 "parser.c"
    break;

  case 60:
#line 448 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue);}
#line 2280 "parser.c"
    break;

  case 61:
#line 451 "parser.y"
                                                          {
                                (yyval.callValue) = call_constractor((yyvsp[-1].exprValue),0,NULL);
        }
#line 2288 "parser.c"
    break;

  case 62:
#line 455 "parser.y"
                                             {
                                (yyval.callValue) = call_constractor(NULL,0,NULL);
        }
#line 2296 "parser.c"
    break;

  case 63:
#line 460 "parser.y"
                                                                            {
                                (yyval.callValue) = call_constractor((yyvsp[-1].exprValue),1,(yyvsp[-3].stringValue));
        }
#line 2304 "parser.c"
    break;

  case 64:
#line 464 "parser.y"
                                                                {
                                (yyval.callValue) = call_constractor(NULL,1,(yyvsp[-2].stringValue));
        }
#line 2312 "parser.c"
    break;

  case 65:
#line 469 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2318 "parser.c"
    break;

  case 66:
#line 472 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2324 "parser.c"
    break;

  case 67:
#line 473 "parser.y"
                                {
                                (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 2333 "parser.c"
    break;

  case 68:
#line 482 "parser.y"
                                {     //[]
                                (yyval.exprValue) = tablecreate_and_emit();
                                }
#line 2341 "parser.c"
    break;

  case 69:
#line 486 "parser.y"
                                {    // [20,30,"hello"]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),newexpr_constnum(i),(yyvsp[-1].exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                }                                                
                                }
#line 2355 "parser.c"
    break;

  case 70:
#line 496 "parser.y"
                             { //[{"x" : (fuction(s){print(s);})} ]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_indexed_length((yyvsp[-1].indexedValue));
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),(yyvsp[-1].indexedValue)->indexedelem,(yyvsp[-1].indexedValue)->value,0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                }   
        
        }
#line 2369 "parser.c"
    break;

  case 71:
#line 507 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2375 "parser.c"
    break;

  case 72:
#line 511 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2381 "parser.c"
    break;

  case 73:
#line 513 "parser.y"
                    {
                                (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2390 "parser.c"
    break;

  case 74:
#line 520 "parser.y"
                                 {
                                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2398 "parser.c"
    break;

  case 75:
#line 526 "parser.y"
                           { 
                                CURR_SCOPE++;   
                        }
#line 2406 "parser.c"
    break;

  case 76:
#line 528 "parser.y"
                                                  {
                                if(CURR_SCOPE!=0)
                                        hide(CURR_SCOPE--);
                                if(!(yyvsp[-1].stmtValue))   
                                        (yyval.stmtValue) = stmt_constractor(0,0);
                                else 
                                        (yyval.stmtValue) = (yyvsp[-1].stmtValue);
                }
#line 2419 "parser.c"
    break;

  case 77:
#line 536 "parser.y"
                                               {
                if(CURR_SCOPE!=0)
                        hide(CURR_SCOPE--); 
                (yyval.stmtValue) = stmt_constractor(0,0); 
        }
#line 2429 "parser.c"
    break;

  case 78:
#line 545 "parser.y"
                        {(yyval.stringValue) = (yyvsp[0].stringValue);}
#line 2435 "parser.c"
    break;

  case 79:
#line 546 "parser.y"
                        {(yyval.stringValue) = newemptyfuncname();}
#line 2441 "parser.c"
    break;

  case 80:
#line 550 "parser.y"
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
#line 2457 "parser.c"
    break;

  case 81:
#line 564 "parser.y"
                                                       {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2467 "parser.c"
    break;

  case 82:
#line 569 "parser.y"
                                           {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2477 "parser.c"
    break;

  case 83:
#line 577 "parser.y"
                        {
                        delete_last_fuction_scope();
                        (yyval.intValue) = currscopeoffset();
                        exitscopespace();
        }
#line 2487 "parser.c"
    break;

  case 84:
#line 584 "parser.y"
                 {      push(loopcounterStack,loopcounter);
                        loopcounter = 0;
                        fuctioncounter++;}
#line 2495 "parser.c"
    break;

  case 85:
#line 588 "parser.y"
                {       pop(loopcounterStack);
                        fuctioncounter--;}
#line 2502 "parser.c"
    break;

  case 86:
#line 592 "parser.y"
                        { fuction_scope_insert(CURR_SCOPE++); }
#line 2508 "parser.c"
    break;

  case 87:
#line 593 "parser.y"
                                                        {
                        exitscopespace();
                        (yyvsp[-5].symbolEntry)->totalLocals = (yyvsp[-1].intValue);
                        int offset = pop(save_fuctionlocals);
                        restorecurrscopeoffset(offset);
                        (yyval.symbolEntry) = (yyvsp[-5].symbolEntry);
                        emit(funcend, lvalue_expr((yyvsp[-5].symbolEntry)), NULL, NULL,0,yylineno);
                        patchlabel((yyvsp[-5].symbolEntry)->fuctionAddress,nextquadlabel());
        }
#line 2522 "parser.c"
    break;

  case 88:
#line 604 "parser.y"
                                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2528 "parser.c"
    break;

  case 89:
#line 605 "parser.y"
                                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2534 "parser.c"
    break;

  case 90:
#line 606 "parser.y"
                                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2540 "parser.c"
    break;

  case 91:
#line 607 "parser.y"
                                { (yyval.exprValue) = newexpr_nil();}
#line 2546 "parser.c"
    break;

  case 92:
#line 608 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool(1);}
#line 2552 "parser.c"
    break;

  case 93:
#line 609 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool(0);}
#line 2558 "parser.c"
    break;

  case 94:
#line 612 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2564 "parser.c"
    break;

  case 95:
#line 613 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2570 "parser.c"
    break;

  case 98:
#line 621 "parser.y"
                                                        {
                emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);   
                (yyval.intValue) = nextquadlabel(); 
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2580 "parser.c"
    break;

  case 99:
#line 627 "parser.y"
                        {
                //emit(if_eq, $expr, newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);  
                (yyval.intValue) = nextquadlabel();
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2590 "parser.c"
    break;

  case 100:
#line 633 "parser.y"
                              {
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                if((yyvsp[0].stmtValue) != NULL)
                        (yyval.stmtValue) = (yyvsp[0].stmtValue);
                else
                        (yyval.stmtValue) = stmt_constractor(0,0); 
        }
#line 2602 "parser.c"
    break;

  case 101:
#line 640 "parser.y"
                                                 {
                patchlabel((yyvsp[-3].intValue), (yyvsp[-1].intValue) + 1);
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                if((yyvsp[-2].stmtValue) != NULL)
                        (yyval.stmtValue) = (yyvsp[-2].stmtValue);
                else
                        (yyval.stmtValue) = ((yyvsp[0].stmtValue)) ? (yyvsp[0].stmtValue) : stmt_constractor(0,0); 
        }
#line 2615 "parser.c"
    break;

  case 102:
#line 651 "parser.y"
                { ++loopcounter; }
#line 2621 "parser.c"
    break;

  case 103:
#line 652 "parser.y"
                { --loopcounter; }
#line 2627 "parser.c"
    break;

  case 104:
#line 655 "parser.y"
                                  {(yyval.stmtValue) = (yyvsp[-1].stmtValue);}
#line 2633 "parser.c"
    break;

  case 105:
#line 658 "parser.y"
                   { (yyval.intValue) = nextquadlabel();
}
#line 2640 "parser.c"
    break;

  case 106:
#line 661 "parser.y"
                                                   {
        emit(if_eq,(yyvsp[-1].exprValue),newexpr_constbool(1),NULL,nextquadlabel()+2,yylineno);
        (yyval.intValue) = nextquadlabel();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 2650 "parser.c"
    break;

  case 107:
#line 669 "parser.y"
                                         {
        emit(jump, NULL, NULL,NULL, (yyvsp[-2].intValue),yylineno);
        patchlabel((yyvsp[-1].intValue), nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->contlist, (yyvsp[-2].intValue));
        (yyval.stmtValue) = (yyvsp[0].stmtValue) ; 
}
#line 2662 "parser.c"
    break;

  case 108:
#line 678 "parser.y"
    { (yyval.intValue) = nextquadlabel(); emit(jump,NULL,NULL,NULL,0,yylineno); }
#line 2668 "parser.c"
    break;

  case 109:
#line 679 "parser.y"
    { (yyval.intValue) = nextquadlabel(); }
#line 2674 "parser.c"
    break;

  case 110:
#line 681 "parser.y"
{       
        (yyval.forValue) = for_prefix_constractor((yyvsp[-2].intValue),nextquadlabel());
        emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL,0,yylineno);
}
#line 2683 "parser.c"
    break;

  case 111:
#line 687 "parser.y"
                                                            {
        patchlabel((yyvsp[-6].forValue)->enter, (yyvsp[-2].intValue)+1); 
        patchlabel((yyvsp[-5].intValue), nextquadlabel()); 
        patchlabel((yyvsp[-2].intValue), (yyvsp[-6].forValue)->test); 
        patchlabel((yyvsp[0].intValue), (yyvsp[-5].intValue)+1); 
        patchlist((yyvsp[-1].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[-1].stmtValue)->contlist, (yyvsp[-5].intValue)+1);
        (yyval.stmtValue) =   (yyvsp[-1].stmtValue);}
#line 2696 "parser.c"
    break;

  case 112:
#line 698 "parser.y"
                     {
        (yyval.intValue) = nextquadlabel()+1;
}
#line 2704 "parser.c"
    break;

  case 113:
#line 702 "parser.y"
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
#line 2720 "parser.c"
    break;

  case 114:
#line 713 "parser.y"
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
#line 2736 "parser.c"
    break;


#line 2740 "parser.c"

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
#line 725 "parser.y"


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
    loopcounterStack = createStack(200);
    init_lib_func();
    emit(0,NULL,NULL,NULL,0,0);
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
   print_format(); //print scopes
   print_all_quads(); //print quads
    return 0;
}
