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
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   615

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  189

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
     284,   288,   291,   292,   298,   304,   317,   331,   344,   358,
     361,   384,   385,   386,   387,   391,   394,   395,   396,   397,
     400,   402,   409,   410,   413,   415,   417,   427,   435,   436,
     439,   444,   451,   458,   461,   462,   470,   474,   484,   496,
     500,   501,   508,   515,   515,   526,   527,   531,   542,   550,
     558,   558,   569,   570,   571,   572,   573,   574,   577,   578,
     581,   582,   586,   592,   598,   605,   611,   614,   614,   623,
     632,   633,   634,   641,   651,   651
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
  "funcname", "funcprefix", "funcargs", "funcbody", "funcdef", "$@2",
  "const", "idlist", "moreidilist", "ifprefix", "elseprefix", "ifstmt",
  "whilestart", "whilecond", "$@3", "whilestmt", "N", "M", "forprefix",
  "forstmt", "returnstmt", "$@4", YY_NULLPTR
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

#define YYPACT_NINF (-167)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     200,   -36,  -167,   -29,    -3,  -167,    -4,     6,   328,    23,
    -167,   328,   116,   116,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,   232,    84,  -167,    26,    67,   200,  -167,  -167,  -167,
     115,  -167,  -167,  -167,    11,  -167,   -32,  -167,  -167,  -167,
    -167,  -167,   200,  -167,    54,  -167,  -167,  -167,  -167,   328,
     328,  -167,  -167,   328,  -167,  -167,  -167,  -167,  -167,    82,
      14,   -32,    14,   200,   328,  -167,   529,  -167,     0,    57,
      63,  -167,   399,    59,  -167,  -167,  -167,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
    -167,   328,  -167,  -167,   328,   264,    78,    79,  -167,  -167,
    -167,   328,   296,    87,    83,   123,  -167,   200,   328,   419,
      41,   357,    89,   159,     5,  -167,   328,  -167,   100,  -167,
     105,   557,   543,    44,    44,  -167,  -167,  -167,   571,   571,
     585,   585,   585,   585,   529,   459,  -167,   -34,  -167,   106,
     483,  -167,     4,  -167,  -167,   104,  -167,   200,   328,  -167,
     107,  -167,  -167,  -167,   105,  -167,   328,  -167,  -167,   328,
    -167,  -167,   328,  -167,  -167,   -23,  -167,  -167,  -167,   439,
    -167,   328,   507,    24,    48,  -167,  -167,   118,  -167,  -167,
     200,   378,  -167,  -167,  -167,  -167,  -167,  -167,  -167
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    96,     0,    76,   104,     0,     0,     0,     0,
      85,     0,     0,     0,    82,    83,    84,    47,    86,    87,
      73,     0,     0,    16,     0,     0,     2,    12,    13,     4,
       0,    31,    17,    39,    41,    46,    42,    43,    14,    80,
      15,    45,     0,     8,     0,     9,   100,    10,    11,     0,
       0,    75,    77,     0,     5,     6,    34,    48,    33,     0,
      35,     0,    37,     0,     0,    66,    63,    64,     0,     0,
      69,    70,     0,     0,    49,     1,     3,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     0,    36,    38,     0,     0,     0,     0,    56,    58,
      59,     0,     0,     0,     0,    94,    97,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    32,
      44,    29,    30,    18,    19,    20,    21,    22,    27,    28,
      23,    25,    24,    26,    40,     0,    61,     0,    50,     0,
       0,    54,     0,    52,    91,     0,    93,     0,     0,    99,
       0,    92,   101,   105,     0,    74,     0,    65,    71,     0,
      51,    60,     0,    53,    55,     0,    79,    81,    95,     0,
     100,     0,     0,     0,     0,    88,    78,     0,    90,    98,
       0,     0,    72,    57,    62,    89,   100,   102,   103
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -167,  -167,    90,  -167,  -167,   -25,    -8,  -167,  -167,  -167,
      91,  -167,    93,  -167,  -167,  -167,   -78,   -48,  -167,  -167,
    -167,    38,    15,  -167,  -167,  -167,  -167,  -167,   -16,  -167,
    -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,  -167,
    -166,  -167,  -167,  -167,  -167,  -167
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    98,    99,   100,    67,    68,    37,    69,
      70,    71,    38,    63,    52,    39,   145,   167,    40,   104,
      41,   178,   165,    42,   147,    43,    44,   107,   148,    45,
     108,   171,    46,    47,    48,    53
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,    76,   110,    58,   180,    49,    73,   101,   161,   102,
     116,   175,    50,    66,    72,   103,    77,   105,    78,   176,
     188,   177,    79,    80,    81,    82,    83,    91,    84,    85,
     150,    51,    86,    87,    88,    89,    92,    93,   157,    54,
     115,   109,    66,   112,   116,   111,   164,   137,   116,    55,
      94,   156,    95,    94,   142,    95,   114,    57,    96,    97,
      74,    96,    97,    81,    82,    83,   183,    75,   116,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   149,   134,   152,   116,   135,    66,    76,     4,
     184,     4,   116,   140,    66,   106,     8,   117,     9,    10,
      66,   120,    11,    60,    62,    61,    61,   118,    66,    12,
      13,   173,   138,   139,   174,    14,    15,    16,    17,    18,
      19,   143,   168,    21,   144,    22,    77,   146,    78,    24,
       9,   154,    79,    80,    81,    82,    83,    64,    84,    85,
     169,    20,    86,    87,    88,    89,   159,   162,   172,   170,
      17,    66,   185,   113,    66,   186,   158,    59,    90,     0,
     166,    24,     1,   181,     2,     3,     4,     5,     6,     7,
       0,     8,     0,     9,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,    12,    13,     0,     0,     0,     0,
      14,    15,    16,    17,    18,    19,    20,   155,    21,     0,
      22,     0,    23,     1,    24,     2,     3,     4,     5,     6,
       7,     0,     8,     0,     9,    10,     0,     0,    11,     0,
       0,     0,     0,     0,     0,    12,    13,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,     0,    21,
       0,    22,     0,    23,     8,    24,     9,    10,     0,     0,
      11,     0,     0,     0,     0,     0,     0,    12,    13,     0,
       0,     0,     0,    14,    15,    16,    17,    18,    19,    64,
       0,    21,    65,    22,     0,     0,     8,    24,     9,    10,
       0,     0,    11,     0,     0,     0,     0,     0,     0,    12,
      13,     0,     0,     0,     0,    14,    15,    16,    17,    18,
      19,     0,     0,    21,     0,    22,   136,     0,     8,    24,
       9,    10,     0,     0,    11,     0,     0,     0,     0,     0,
       0,    12,    13,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,     0,     0,    21,     0,    22,   141,     0,
       8,    24,     9,    10,     0,     0,    11,     0,     0,     0,
       0,     0,     0,    12,    13,     0,     0,     0,     0,    14,
      15,    16,    17,    18,    19,     0,     0,    21,    77,    22,
      78,     0,     0,    24,    79,    80,    81,    82,    83,     0,
      84,    85,     0,     0,    86,    87,    88,    89,     0,    77,
       0,    78,     0,     0,     0,    79,    80,    81,    82,    83,
     153,    84,    85,     0,     0,    86,    87,    88,    89,     0,
      77,     0,    78,     0,     0,     0,    79,    80,    81,    82,
      83,   187,    84,    85,     0,     0,    86,    87,    88,    89,
      77,     0,    78,     0,     0,     0,    79,    80,    81,    82,
      83,   119,    84,    85,     0,     0,    86,    87,    88,    89,
      77,     0,    78,     0,     0,     0,    79,    80,    81,    82,
      83,   151,    84,    85,     0,     0,    86,    87,    88,    89,
      77,     0,    78,     0,     0,     0,    79,    80,    81,    82,
      83,   179,    84,    85,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,    77,     0,    78,     0,     0,   160,
      79,    80,    81,    82,    83,     0,    84,    85,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    77,     0,
      78,     0,     0,   163,    79,    80,    81,    82,    83,     0,
      84,    85,     0,     0,    86,    87,    88,    89,     0,     0,
      77,     0,    78,     0,     0,   182,    79,    80,    81,    82,
      83,     0,    84,    85,    77,     0,    86,    87,    88,    89,
      79,    80,    81,    82,    83,     0,    84,    85,     0,     0,
      86,    87,    88,    89,    79,    80,    81,    82,    83,     0,
      84,    85,     0,     0,    86,    87,    88,    89,    79,    80,
      81,    82,    83,     0,    -1,    -1,     0,     0,    86,    87,
      88,    89,    79,    80,    81,    82,    83,     0,     0,     0,
       0,     0,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
       8,    26,    50,    11,   170,    41,    22,    39,    42,    41,
      44,    34,    41,    21,    22,    47,    11,    42,    13,    42,
     186,    44,    17,    18,    19,    20,    21,    16,    23,    24,
     108,    34,    27,    28,    29,    30,    25,    26,   116,    43,
      40,    49,    50,    59,    44,    53,    42,    95,    44,    43,
      39,    46,    41,    39,   102,    41,    64,    34,    47,    48,
      34,    47,    48,    19,    20,    21,    42,     0,    44,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,   107,    91,    43,    44,    94,    95,   113,     7,
      42,     7,    44,   101,   102,    41,    12,    40,    14,    15,
     108,    42,    18,    12,    13,    12,    13,    44,   116,    25,
      26,   159,    34,    34,   162,    31,    32,    33,    34,    35,
      36,    34,   147,    39,    41,    41,    11,     4,    13,    45,
      14,    42,    17,    18,    19,    20,    21,    37,    23,    24,
     148,    37,    27,    28,    29,    30,    41,    41,   156,    42,
      34,   159,    34,    63,   162,   180,   118,    41,    43,    -1,
     145,    45,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      41,    -1,    43,     3,    45,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    -1,    39,
      -1,    41,    -1,    43,    12,    45,    14,    15,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      -1,    39,    40,    41,    -1,    -1,    12,    45,    14,    15,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    41,    42,    -1,    12,    45,
      14,    15,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    41,    42,    -1,
      12,    45,    14,    15,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    11,    41,
      13,    -1,    -1,    45,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    -1,    27,    28,    29,    30,    -1,    11,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      43,    23,    24,    -1,    -1,    27,    28,    29,    30,    -1,
      11,    -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    43,    23,    24,    -1,    -1,    27,    28,    29,    30,
      11,    -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    42,    23,    24,    -1,    -1,    27,    28,    29,    30,
      11,    -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    42,    23,    24,    -1,    -1,    27,    28,    29,    30,
      11,    -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    42,    23,    24,    -1,    -1,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    40,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    -1,    -1,    -1,    11,    -1,
      13,    -1,    -1,    40,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    -1,    27,    28,    29,    30,    -1,    -1,
      11,    -1,    13,    -1,    -1,    38,    17,    18,    19,    20,
      21,    -1,    23,    24,    11,    -1,    27,    28,    29,    30,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    28,    29,    30,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    -1,    27,    28,    29,    30,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    -1,    27,    28,
      29,    30,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     6,     7,     8,     9,    10,    12,    14,
      15,    18,    25,    26,    31,    32,    33,    34,    35,    36,
      37,    39,    41,    43,    45,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    70,    74,    77,
      80,    82,    85,    87,    88,    91,    94,    95,    96,    41,
      41,    34,    76,    97,    43,    43,    58,    34,    58,    41,
      62,    64,    62,    75,    37,    40,    58,    68,    69,    71,
      72,    73,    58,    80,    34,     0,    57,    11,    13,    17,
      18,    19,    20,    21,    23,    24,    27,    28,    29,    30,
      43,    16,    25,    26,    39,    41,    47,    48,    65,    66,
      67,    39,    41,    47,    81,    57,    41,    89,    92,    58,
      69,    58,    80,    54,    58,    40,    44,    40,    44,    42,
      42,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    42,    69,    34,    34,
      58,    42,    69,    34,    41,    78,     4,    86,    90,    57,
      68,    42,    43,    43,    42,    38,    46,    68,    73,    41,
      40,    42,    41,    40,    42,    84,    74,    79,    57,    58,
      42,    93,    58,    69,    69,    34,    42,    44,    83,    42,
      92,    58,    38,    42,    42,    34,    57,    43,    92
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    61,    61,    61,    61,    61,    62,    62,    62,    62,
      63,    63,    63,    63,    64,    64,    64,    64,    65,    65,
      66,    66,    67,    68,    69,    69,    70,    70,    70,    71,
      72,    72,    73,    75,    74,    76,    76,    77,    78,    79,
      81,    80,    82,    82,    82,    82,    82,    82,    83,    83,
      84,    84,    85,    86,    87,    87,    88,    90,    89,    91,
      92,    93,    94,    95,    97,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     2,     2,
       3,     4,     3,     4,     3,     4,     2,     6,     1,     1,
       3,     2,     5,     1,     1,     3,     2,     3,     3,     1,
       1,     3,     5,     0,     4,     1,     0,     2,     3,     1,
       0,     4,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     4,     1,     2,     4,     1,     0,     4,     3,
       0,     0,     7,     7,     0,     4
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
#line 1796 "parser.c"
    break;

  case 4:
#line 240 "parser.y"
              {
                (yyvsp[0].stmtValue) = stmt_constractor(0,0);
                (yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1804 "parser.c"
    break;

  case 5:
#line 246 "parser.y"
                        {       (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->breaklist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,nextquadlabel(),yylineno); 
                                (yyval.stmtValue) = stmt_constractor(0,0);}
#line 1813 "parser.c"
    break;

  case 6:
#line 250 "parser.y"
                           {    (yyval.stmtValue) = stmt_constractor(0,0);
                                (yyval.stmtValue)->contlist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,nextquadlabel(),yylineno); 
                                (yyval.stmtValue) = stmt_constractor(0,0);}
#line 1822 "parser.c"
    break;

  case 7:
#line 255 "parser.y"
                       {(yyval.stmtValue) = stmt_constractor(0,0);}
#line 1828 "parser.c"
    break;

  case 8:
#line 256 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1834 "parser.c"
    break;

  case 9:
#line 257 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1840 "parser.c"
    break;

  case 10:
#line 258 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1846 "parser.c"
    break;

  case 11:
#line 259 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1852 "parser.c"
    break;

  case 12:
#line 260 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1858 "parser.c"
    break;

  case 13:
#line 261 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1864 "parser.c"
    break;

  case 14:
#line 262 "parser.y"
                     {(yyval.stmtValue) = (yyvsp[0].stmtValue);}
#line 1870 "parser.c"
    break;

  case 15:
#line 263 "parser.y"
                     {(yyval.stmtValue) = stmt_constractor(0,0);}
#line 1876 "parser.c"
    break;

  case 16:
#line 264 "parser.y"
                   {resettemp(); (yyval.stmtValue) = stmt_constractor(0,0);}
#line 1882 "parser.c"
    break;

  case 17:
#line 268 "parser.y"
                                {(yyval.exprValue) =  (yyvsp[0].exprValue);}
#line 1888 "parser.c"
    break;

  case 18:
#line 269 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),add);}
#line 1894 "parser.c"
    break;

  case 19:
#line 270 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),sub);}
#line 1900 "parser.c"
    break;

  case 20:
#line 271 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mul);}
#line 1906 "parser.c"
    break;

  case 21:
#line 272 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),n_div);}
#line 1912 "parser.c"
    break;

  case 22:
#line 273 "parser.y"
                                {(yyval.exprValue) =  arithop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),mod);}
#line 1918 "parser.c"
    break;

  case 23:
#line 274 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_greater); emit_relop((yyval.exprValue),if_greater);}
#line 1924 "parser.c"
    break;

  case 24:
#line 275 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_geatereq); emit_relop((yyval.exprValue), if_geatereq);}
#line 1930 "parser.c"
    break;

  case 25:
#line 276 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_less);    emit_relop((yyval.exprValue),if_less);}
#line 1936 "parser.c"
    break;

  case 26:
#line 277 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_lesseq);  emit_relop((yyval.exprValue),if_lesseq);}
#line 1942 "parser.c"
    break;

  case 27:
#line 278 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_eq);      emit_relop((yyval.exprValue),if_eq);}
#line 1948 "parser.c"
    break;

  case 28:
#line 279 "parser.y"
                                {(yyval.exprValue) =  relop((yyvsp[-2].exprValue),(yyvsp[0].exprValue),if_noteq);   emit_relop((yyval.exprValue),if_noteq);}
#line 1954 "parser.c"
    break;

  case 29:
#line 280 "parser.y"
                                { (yyval.exprValue) =   newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),and)) ; 
                                (yyval.exprValue)->sym = newtemp();     
                                emit(and,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno);\
                                }
#line 1963 "parser.c"
    break;

  case 30:
#line 284 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool(boolo((yyvsp[-2].exprValue),(yyvsp[0].exprValue),or)); 
                                (yyval.exprValue)->sym = newtemp();       
                                emit(and,(yyvsp[-2].exprValue),(yyvsp[0].exprValue),(yyval.exprValue),0,yylineno);
                                }
#line 1972 "parser.c"
    break;

  case 31:
#line 288 "parser.y"
                                {}
#line 1978 "parser.c"
    break;

  case 32:
#line 291 "parser.y"
                                                {(yyval.exprValue) = (yyvsp[-1].exprValue);}
#line 1984 "parser.c"
    break;

  case 33:
#line 292 "parser.y"
                                             { //%prec: priority of uminus operator 
                check_arith((yyvsp[0].exprValue));
                (yyval.exprValue) = newexpr(arithexpr_e);
                (yyval.exprValue)->sym = istempexpr((yyvsp[0].exprValue)) ? (yyvsp[0].exprValue)->sym : newtemp();
                emit(uminus,(yyvsp[0].exprValue),NULL,(yyval.exprValue),0,yylineno);
        }
#line 1995 "parser.c"
    break;

  case 34:
#line 298 "parser.y"
                                {//not a
                (yyval.exprValue) = newexpr(constbool_e);
                (yyval.exprValue)->sym = newtemp();
                (yyval.exprValue)->boolConst = !(check_if_bool((yyvsp[0].exprValue)));
                emit(not, (yyvsp[0].exprValue), NULL, (yyval.exprValue),0, yylineno);
        }
#line 2006 "parser.c"
    break;

  case 35:
#line 304 "parser.y"
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
#line 2024 "parser.c"
    break;

  case 36:
#line 317 "parser.y"
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
#line 2043 "parser.c"
    break;

  case 37:
#line 331 "parser.y"
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
#line 2061 "parser.c"
    break;

  case 38:
#line 344 "parser.y"
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
#line 2080 "parser.c"
    break;

  case 39:
#line 358 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2086 "parser.c"
    break;

  case 40:
#line 361 "parser.y"
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
						emit(assign, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, 0, yylineno); //paizei na thelei ta arg anapoda
						(yyval.exprValue) = newexpr(assignexpr_e);
						(yyval.exprValue)->sym = newtemp();
                                                (yyvsp[-2].exprValue)->type = (yyvsp[0].exprValue)->type;
                                               // copy_value($1,$3);
						emit(assign, (yyval.exprValue), (yyvsp[-2].exprValue), NULL, 0, yylineno); //omoiws me to apo panw
                                                //copy_value($assignexpr,$1);
                                        }
				}               
        }
#line 2112 "parser.c"
    break;

  case 41:
#line 384 "parser.y"
                                {(yyval.exprValue) = emit_iftableitem((yyvsp[0].exprValue));}
#line 2118 "parser.c"
    break;

  case 42:
#line 385 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2124 "parser.c"
    break;

  case 43:
#line 386 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2130 "parser.c"
    break;

  case 44:
#line 387 "parser.y"
                                                   {
                (yyval.exprValue) = newexpr(programfunc_e);
                (yyval.exprValue)->sym = (yyvsp[-1].symbolEntry); 
        }
#line 2139 "parser.c"
    break;

  case 45:
#line 391 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2145 "parser.c"
    break;

  case 46:
#line 394 "parser.y"
                                {  (yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2151 "parser.c"
    break;

  case 47:
#line 395 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_ID(yylval.stringValue));}
#line 2157 "parser.c"
    break;

  case 48:
#line 396 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(insert_local(yylval.stringValue));}
#line 2163 "parser.c"
    break;

  case 49:
#line 397 "parser.y"
                                {  (yyval.exprValue) = lvalue_expr(check_global(yylval.stringValue));}
#line 2169 "parser.c"
    break;

  case 50:
#line 400 "parser.y"
                                {// a.x;
                                (yyval.exprValue) = member_item((yyvsp[-2].exprValue),yylval.stringValue);}
#line 2176 "parser.c"
    break;

  case 51:
#line 402 "parser.y"
                                                              { // a[3]
                                (yyvsp[-3].exprValue) = emit_iftableitem((yyvsp[-3].exprValue));
                                (yyval.exprValue) = newexpr(tableitem_e);
                                (yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
                                (yyval.exprValue)->index = (yyvsp[-1].exprValue); //index of expr
        }
#line 2187 "parser.c"
    break;

  case 54:
#line 414 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-2].exprValue),NULL);}
#line 2193 "parser.c"
    break;

  case 55:
#line 416 "parser.y"
                                {(yyval.exprValue) = make_call((yyvsp[-3].exprValue), (yyvsp[-1].exprValue));}
#line 2199 "parser.c"
    break;

  case 56:
#line 418 "parser.y"
                                {
                                (yyvsp[-1].exprValue) = emit_iftableitem((yyvsp[-1].exprValue));            //se periptwsh p htan table item
                                if((yyvsp[0].callValue)->method){
                                        expr* t = (yyvsp[-1].exprValue);
                                        (yyvsp[-1].exprValue) = emit_iftableitem(member_item(t, (yyvsp[0].callValue)->name));
                                        (yyvsp[0].callValue)->elist->next = t;           // insert san prwto arg (reversed,so last)
                                }
                                (yyval.exprValue) = make_call((yyvsp[-1].exprValue), (yyvsp[0].callValue)->elist);
        }
#line 2213 "parser.c"
    break;

  case 57:
#line 428 "parser.y"
                                {
                                expr* func = newexpr(programfunc_e);
                                func->sym = (yyvsp[-4].symbolEntry);
                                (yyval.exprValue) = make_call(func, (yyvsp[-1].exprValue));                
        }
#line 2223 "parser.c"
    break;

  case 58:
#line 435 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue); }
#line 2229 "parser.c"
    break;

  case 59:
#line 436 "parser.y"
                                {(yyval.callValue) = (yyvsp[0].callValue);}
#line 2235 "parser.c"
    break;

  case 60:
#line 439 "parser.y"
                                                          {
                                (yyval.callValue)->elist = (yyvsp[-1].exprValue);
                                (yyval.callValue)->method = 0;
                                (yyval.callValue)->name = NULL;
        }
#line 2245 "parser.c"
    break;

  case 61:
#line 444 "parser.y"
                                             {
                                (yyval.callValue)->elist =NULL;
                                (yyval.callValue)->method = 0;
                                (yyval.callValue)->name = NULL;
        }
#line 2255 "parser.c"
    break;

  case 62:
#line 451 "parser.y"
                                                                            {
                                (yyval.callValue)->elist = (yyvsp[-1].exprValue); 
                                (yyval.callValue)->method = 1;
                                (yyval.callValue)->name = (yyvsp[-3].stringValue);
        }
#line 2265 "parser.c"
    break;

  case 63:
#line 458 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2271 "parser.c"
    break;

  case 64:
#line 461 "parser.y"
                                {(yyval.exprValue) = (yyvsp[0].exprValue);}
#line 2277 "parser.c"
    break;

  case 65:
#line 462 "parser.y"
                                {
                                (yyvsp[0].exprValue)->next = (yyvsp[-2].exprValue);      //expr->next = me moreElist
                                (yyval.exprValue) = (yyvsp[0].exprValue);                //moreElist = expr;        
                                }
#line 2286 "parser.c"
    break;

  case 66:
#line 471 "parser.y"
                                {     //[]
                                (yyval.exprValue) = tablecreate_and_emit();
                                }
#line 2294 "parser.c"
    break;

  case 67:
#line 475 "parser.y"
                                {    // [20,30,"hello"]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_elist_length((yyvsp[-1].exprValue));  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),newexpr_constnum(i),(yyvsp[-1].exprValue),0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].exprValue) = (yyvsp[-1].exprValue)->next; // go to next expr
                                }                                                
                                }
#line 2308 "parser.c"
    break;

  case 68:
#line 485 "parser.y"
                             { //[{"x" : (fuction(s){print(s);})} ]
                                (yyval.exprValue) = tablecreate_and_emit();
                                int i = get_indexed_length((yyvsp[-1].indexedValue));
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,(yyval.exprValue),(yyvsp[-1].indexedValue)->indexedelem,(yyvsp[-1].indexedValue)->value,0,yylineno); // emit (op,temp,index,value)
                                        (yyvsp[-1].indexedValue) = (yyvsp[-1].indexedValue)->next; // go to next index
                                }   
        
        }
#line 2322 "parser.c"
    break;

  case 69:
#line 496 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2328 "parser.c"
    break;

  case 70:
#line 500 "parser.y"
                                {(yyval.indexedValue) = (yyvsp[0].indexedValue);}
#line 2334 "parser.c"
    break;

  case 71:
#line 502 "parser.y"
                    {
                                (yyvsp[0].indexedValue)->next = (yyvsp[-2].indexedValue);        //expr->next = me moreIndex
                                (yyval.indexedValue) = (yyvsp[0].indexedValue);                        //moreIndex = expr; 
        }
#line 2343 "parser.c"
    break;

  case 72:
#line 509 "parser.y"
                                 {
                                (yyval.indexedValue) = indexed_constractor((yyvsp[-3].exprValue),(yyvsp[-1].exprValue),NULL);
        }
#line 2351 "parser.c"
    break;

  case 73:
#line 515 "parser.y"
                           { 
                                CURR_SCOPE++;   
                        }
#line 2359 "parser.c"
    break;

  case 74:
#line 517 "parser.y"
                                                  {
                                if(CURR_SCOPE!=0)
                                        hide(CURR_SCOPE--);    
                                (yyval.stmtValue) = (yyvsp[-1].stmtValue);
                }
#line 2369 "parser.c"
    break;

  case 75:
#line 526 "parser.y"
                        {(yyval.stringValue) = (yyvsp[0].stringValue);}
#line 2375 "parser.c"
    break;

  case 76:
#line 527 "parser.y"
                        {(yyval.stringValue) = newemptyfuncname();}
#line 2381 "parser.c"
    break;

  case 77:
#line 531 "parser.y"
                          { 
                        (yyval.symbolEntry) = function_insert((yyvsp[0].stringValue));               //yylval.stringValue
                        (yyval.symbolEntry)->fuctionAddress = nextquadlabel();
                        emit(funcstart,lvalue_expr((yyval.symbolEntry)), NULL, NULL,0,yylineno);
                        push(save_fuctionlocals,currscopeoffset());
                        enterscopespace();                      // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2394 "parser.c"
    break;

  case 78:
#line 542 "parser.y"
                                                       {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
#line 2404 "parser.c"
    break;

  case 79:
#line 550 "parser.y"
                        {
                        delete_last_fuction_scope();
                        (yyval.intValue) = currscopeoffset();
                        exitscopespace();
        }
#line 2414 "parser.c"
    break;

  case 80:
#line 558 "parser.y"
                        { fuction_scope_insert(CURR_SCOPE++); }
#line 2420 "parser.c"
    break;

  case 81:
#line 559 "parser.y"
                          {
                        exitscopespace();
                        (yyvsp[-3].symbolEntry)->totalLocals = (yyvsp[0].intValue);
                        int offset = pop(save_fuctionlocals);
                        restorecurrscopeoffset(offset);
                        (yyval.symbolEntry) = (yyvsp[-3].symbolEntry);
                        emit(funcend, lvalue_expr((yyvsp[-3].symbolEntry)), NULL, NULL,0,yylineno);
        }
#line 2433 "parser.c"
    break;

  case 82:
#line 569 "parser.y"
                                { (yyval.exprValue) = newexpr_constnum((yyvsp[0].intValue));}
#line 2439 "parser.c"
    break;

  case 83:
#line 570 "parser.y"
                                { (yyval.exprValue) = newexpr_constdouble((yyvsp[0].realValue));}
#line 2445 "parser.c"
    break;

  case 84:
#line 571 "parser.y"
                                { (yyval.exprValue) = newexpr_conststring((yyvsp[0].stringValue));}
#line 2451 "parser.c"
    break;

  case 85:
#line 572 "parser.y"
                                { (yyval.exprValue) = newexpr_nil();}
#line 2457 "parser.c"
    break;

  case 86:
#line 573 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('1');}
#line 2463 "parser.c"
    break;

  case 87:
#line 574 "parser.y"
                                { (yyval.exprValue) = newexpr_constbool('0');}
#line 2469 "parser.c"
    break;

  case 88:
#line 577 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2475 "parser.c"
    break;

  case 89:
#line 578 "parser.y"
                                {insert_formal(yylval.stringValue);}
#line 2481 "parser.c"
    break;

  case 92:
#line 586 "parser.y"
                                                        {
                emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);   
                (yyval.intValue) = nextquadlabel(); 
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2491 "parser.c"
    break;

  case 93:
#line 592 "parser.y"
                        {
                //emit(if_eq, $expr, newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);  
                (yyval.intValue) = nextquadlabel();
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
#line 2501 "parser.c"
    break;

  case 94:
#line 598 "parser.y"
                              {
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                if((yyvsp[0].stmtValue) != NULL)
                        (yyval.stmtValue) = (yyvsp[0].stmtValue);
                else
                        (yyval.stmtValue) = stmt_constractor(0,0); 
        }
#line 2513 "parser.c"
    break;

  case 95:
#line 605 "parser.y"
                                                 {
                patchlabel((yyvsp[-3].intValue), (yyvsp[-1].intValue) + 1);
                patchlabel((yyvsp[-1].intValue), nextquadlabel());
                (yyval.stmtValue) = stmt_constractor(0,0); 
        }
#line 2523 "parser.c"
    break;

  case 96:
#line 611 "parser.y"
                   { (yyval.intValue) = nextquadlabel();
}
#line 2530 "parser.c"
    break;

  case 97:
#line 614 "parser.y"
                            { //for_flag = 1;
}
#line 2537 "parser.c"
    break;

  case 98:
#line 615 "parser.y"
                        {
        emit(if_eq,(yyvsp[-1].exprValue),newexpr_constbool(1),NULL,nextquadlabel()+2,yylineno);
        (yyval.intValue) = nextquadlabel();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 2547 "parser.c"
    break;

  case 99:
#line 623 "parser.y"
                                     {
        emit(jump, NULL, NULL,NULL, (yyvsp[-2].intValue),yylineno);
        patchlabel((yyvsp[-1].intValue), nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[0].stmtValue)->contlist, (yyvsp[-2].intValue));
        (yyval.stmtValue) = ((yyvsp[0].stmtValue)) ?  (yyvsp[0].stmtValue) : stmt_constractor(0,0); 
}
#line 2559 "parser.c"
    break;

  case 100:
#line 632 "parser.y"
    { (yyval.intValue) = nextquadlabel(); emit(jump,NULL,NULL,NULL,0,yylineno); }
#line 2565 "parser.c"
    break;

  case 101:
#line 633 "parser.y"
    { (yyval.intValue) = nextquadlabel(); }
#line 2571 "parser.c"
    break;

  case 102:
#line 635 "parser.y"
{       
        (yyval.forValue) = for_prefix_constractor((yyvsp[-2].intValue),nextquadlabel());
        emit(if_eq, (yyvsp[-1].exprValue), newexpr_constbool(1), NULL,0,yylineno);
}
#line 2580 "parser.c"
    break;

  case 103:
#line 641 "parser.y"
                                                        {
        patchlabel((yyvsp[-6].forValue)->enter, (yyvsp[-2].intValue)+1); 
        patchlabel((yyvsp[-5].intValue), nextquadlabel()); 
        patchlabel((yyvsp[-2].intValue), (yyvsp[-6].forValue)->test); 
        patchlabel((yyvsp[0].intValue), (yyvsp[-5].intValue)+1); 
        patchlist((yyvsp[-1].stmtValue)->breaklist, nextquadlabel());
        patchlist((yyvsp[-1].stmtValue)->contlist, (yyvsp[-5].intValue)+1);
        (yyval.stmtValue) = ((yyvsp[-1].stmtValue)) ?  (yyvsp[-1].stmtValue) : stmt_constractor(0,0); }
#line 2593 "parser.c"
    break;

  case 104:
#line 651 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 2599 "parser.c"
    break;

  case 105:
#line 651 "parser.y"
                                                                                      {
        (yyval.stmtValue) = stmt_constractor(0,0);
}
#line 2607 "parser.c"
    break;


#line 2611 "parser.c"

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
#line 655 "parser.y"


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
    emit(0,NULL,NULL,NULL,0,0);
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
   //print_format(); //print scopes
   print_all_quads(); //print quads
    return 0;
}
