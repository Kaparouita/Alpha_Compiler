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
        

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);


        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
        int CURR_SCOPE=0;
        int myfuctions[150];
        extern var_table* table ;
        var *curr = NULL;

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
                        myvar =new_var(varr,curr_id,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);                    
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
                myvar = new_var(varr,formal,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myvar,table);
                print_var(myvar);
        }

        /*insert a new fuction to the table*/
        void function_insert(char* name){
                //check if anonymous and insert if true
                if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,1,yylineno); 
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
                myVar = new_var(fuction,user_func,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myVar,table);
                print_var(myVar);
        }

        /*Insert a local var with name = name */
        void insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
                        return;
                }
                /*tsekare an yparxoun collisions me lib fuction*/
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                /*an eimaste sto scope 0 tote thn kanoume insert san global*/
                if(CURR_SCOPE == 0)
                        curr_id = global;
                        //printf("GT %d\n",currVar->hide);
                currVar = new_var(varr,curr_id,name,CURR_SCOPE,1,yylineno); 
                hash_insert(currVar,table);
                print_var(currVar);          
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

#line 216 "parser.c"

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
    MULTI = 275,
    DIVISION = 276,
    MODULUSEQUAL = 277,
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
    LEFT_CURLY_BRACKET = 288,
    RIGHT_CURLY_BRACKET = 289,
    RIGHT_SQUARE_BRACKET = 290,
    RIGHT_PARENTHESIS = 291,
    SEMICOLON = 292,
    COMMA = 293,
    SCOPE_RESOLUTION = 294,
    COLON = 295,
    FULL_STOP = 296,
    DOUBLE_FULL_STOP = 297,
    ID = 298,
    EXTRA_CHARS = 299,
    LINE_COMMENT = 300,
    BLOCK_COMMENT = 301,
    EQUAL = 302,
    SUBTRACTION = 303,
    MODULUS = 304,
    LEFT_SQUARE_BRACKET = 305,
    LEFT_PARENTHESIS = 306
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
#define MULTI 275
#define DIVISION 276
#define MODULUSEQUAL 277
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
#define LEFT_CURLY_BRACKET 288
#define RIGHT_CURLY_BRACKET 289
#define RIGHT_SQUARE_BRACKET 290
#define RIGHT_PARENTHESIS 291
#define SEMICOLON 292
#define COMMA 293
#define SCOPE_RESOLUTION 294
#define COLON 295
#define FULL_STOP 296
#define DOUBLE_FULL_STOP 297
#define ID 298
#define EXTRA_CHARS 299
#define LINE_COMMENT 300
#define BLOCK_COMMENT 301
#define EQUAL 302
#define SUBTRACTION 303
#define MODULUS 304
#define LEFT_SQUARE_BRACKET 305
#define LEFT_PARENTHESIS 306

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 149 "parser.y"

        char* stringValue;
        int intValue;
        double realValue;
        struct var *exprNode;

#line 377 "parser.c"

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
#define YYLAST   631

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  184

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
       0,   205,   205,   208,   209,   213,   213,   214,   214,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   246,   247,   248,   249,   250,   251,
     252,   253,   256,   256,   260,   261,   262,   263,   264,   267,
     268,   269,   270,   273,   274,   275,   276,   279,   280,   281,
     285,   286,   289,   292,   295,   298,   299,   300,   303,   304,
     307,   311,   312,   315,   319,   319,   328,   331,   328,   335,
     339,   335,   344,   345,   346,   347,   348,   349,   352,   353,
     356,   357,   361,   361,   362,   365,   365,   369,   369,   372,
     372
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "TRUE",
  "FALSE", "NILL", "ASSIGNMENT", "ADDITION", "MULTI", "DIVISION",
  "MODULUSEQUAL", "NOTEQUAL", "INCREMENT", "DECREMENT", "GRETER_THAN",
  "LESS_THAN", "GRE_EQUAL", "LES_EQUAL", "INTEGER", "REAL", "STRING",
  "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "RIGHT_SQUARE_BRACKET",
  "RIGHT_PARENTHESIS", "SEMICOLON", "COMMA", "SCOPE_RESOLUTION", "COLON",
  "FULL_STOP", "DOUBLE_FULL_STOP", "ID", "EXTRA_CHARS", "LINE_COMMENT",
  "BLOCK_COMMENT", "EQUAL", "SUBTRACTION", "MODULUS",
  "LEFT_SQUARE_BRACKET", "LEFT_PARENTHESIS", "$accept", "program",
  "stmt_list", "brk_stm", "$@1", "cnt_stm", "$@2", "stmt", "expr", "term",
  "assignexpr", "$@3", "primary", "lvalue", "member", "call", "callsuffix",
  "normcall", "methodcall", "elist", "moreElist", "objectdef", "indexed",
  "moreindexedelem", "indexedelem", "block", "$@4", "funcdef", "$@5",
  "$@6", "$@7", "$@8", "const", "idlist", "moreidilist", "ifstmt", "$@9",
  "whilestmt", "$@10", "forstmt", "$@11", "returnstmt", "$@12", YY_NULLPTR
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

#define YYPACT_NINF (-141)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-43)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -141,     5,   183,  -141,   -26,   183,   -24,    -1,    18,  -141,
    -141,  -141,   213,    24,  -141,  -141,  -141,    -6,    -6,  -141,
    -141,  -141,  -141,  -141,    39,  -141,   213,    79,     4,  -141,
    -141,  -141,   246,  -141,  -141,  -141,   -11,  -141,    15,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,    37,   213,    55,    61,  -141,  -141,   107,     7,
      15,     7,  -141,  -141,    21,   213,   516,  -141,    33,    71,
      83,  -141,   277,    87,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,  -141,   213,   213,   213,  -141,  -141,
      81,    82,   213,   213,   108,  -141,  -141,  -141,    85,   213,
     213,   213,   213,   213,    80,  -141,   308,  -141,  -141,    96,
     134,   339,  -141,   213,  -141,   101,  -141,    84,   558,   527,
      21,  -141,  -141,   569,   582,   582,   582,   582,   569,    21,
    -141,  -141,   109,   370,   -32,   213,  -141,   381,     8,   412,
     443,    64,  -141,   -21,  -141,    84,  -141,   213,  -141,  -141,
     213,   213,  -141,  -141,   516,  -141,  -141,   183,   183,   213,
      26,  -141,    93,  -141,  -141,   454,    51,    69,  -141,  -141,
     485,  -141,   114,  -141,  -141,  -141,  -141,   213,   114,  -141,
      77,  -141,   183,  -141
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,    79,    99,
       5,     7,     0,     0,    86,    87,    85,     0,     0,    82,
      83,    84,    74,    18,     0,    49,     0,    67,     0,    14,
      15,     3,     0,    33,    19,    41,    44,    52,    45,    46,
      16,    17,    48,    10,    11,    12,    13,    92,    94,    95,
      97,    76,     0,     0,     0,     0,    36,    50,     0,    37,
       0,    39,     4,    51,    35,     0,    64,    65,     0,     0,
      70,    71,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,    38,    40,
       0,     0,     0,    67,     0,    58,    60,    61,     0,     0,
      67,     0,     0,    67,     0,    91,     0,     6,     8,     0,
       0,     0,    68,     0,    69,     0,    34,    47,    31,    32,
      20,    22,    23,    30,    25,    27,    26,    28,    29,    21,
      24,    53,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,    91,     0,   100,     0,    75,     0,    66,    72,
      67,    67,    54,    62,    43,    56,    57,     0,     0,     0,
       0,    80,     0,    88,    90,     0,     0,     0,    93,    96,
       0,    77,     0,    89,    73,    59,    63,    67,     0,    81,
       0,    78,     0,    98
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,    90,  -141,  -141,  -141,  -141,    -4,    -2,  -141,
    -141,  -141,  -141,    99,  -141,   102,  -141,  -141,  -141,    48,
     -91,  -141,  -141,  -141,    47,  -140,  -141,    -5,  -141,  -141,
    -141,  -141,  -141,  -141,    27,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    29,    54,    30,    55,    31,    66,    33,
      34,    94,    35,    36,    37,    38,    95,    96,    97,    67,
      68,    39,    69,    70,    71,    40,    62,    41,   104,   178,
      52,   172,    42,   164,   143,    43,   101,    44,   102,    45,
     103,    46,    53
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    48,   134,    32,   153,     3,   113,   -42,    13,   138,
      56,     8,   141,    88,    89,   161,    12,   162,    13,    14,
      15,    16,   163,    73,    64,    47,    72,    49,    17,    18,
      90,    91,   179,    24,    19,    20,    21,    25,   181,    92,
      93,    77,    78,    24,   156,    58,   113,    25,    90,    91,
      50,   106,    26,   109,    27,    28,    98,    92,    93,   166,
     167,    51,   171,   111,   162,    99,   100,    57,   112,   163,
      87,   113,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    63,   128,   129,   130,   180,   175,   105,   113,
     133,    12,   107,    13,    14,    15,    16,   137,   108,   139,
     140,   159,   113,    17,    18,   176,   114,   113,    32,    19,
      20,    21,    65,   182,     8,   113,    59,    61,    24,    60,
      60,   115,    25,   117,   131,   132,   135,    26,   136,    27,
      28,   142,   145,   154,    65,   150,   173,     4,     5,     6,
       7,     8,     9,    10,    11,   165,    12,    22,    13,    14,
      15,    16,   110,   168,   169,    32,    32,   170,    17,    18,
     151,   148,   149,     0,    19,    20,    21,    22,   146,   160,
       0,    23,     0,    24,     0,     0,     0,    25,   183,     0,
      32,     0,    26,     0,    27,    28,     4,     5,     6,     7,
       8,     9,    10,    11,     0,    12,     0,    13,    14,    15,
      16,     0,     0,     0,     0,     0,     0,    17,    18,     0,
       0,     0,     0,    19,    20,    21,    22,     0,     0,     0,
      23,     0,    24,     0,     0,    12,    25,    13,    14,    15,
      16,    26,     0,    27,    28,     0,     0,    17,    18,     0,
       0,     0,     0,    19,    20,    21,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,    25,    74,     0,    75,
       0,    26,     0,    27,    28,    76,    77,    78,     0,    79,
       0,     0,    80,    81,    82,    83,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,     0,     0,    74,     0,
      75,     0,     0,    85,    86,    87,    76,    77,    78,     0,
      79,     0,     0,    80,    81,    82,    83,     0,     0,     0,
       0,     0,     0,   116,     0,     0,     0,     0,     0,    74,
       0,    75,     0,     0,    85,    86,    87,    76,    77,    78,
       0,    79,     0,     0,    80,    81,    82,    83,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,     0,     0,
      74,     0,    75,     0,     0,    85,    86,    87,    76,    77,
      78,     0,    79,     0,     0,    80,    81,    82,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
       0,    74,     0,    75,     0,     0,    85,    86,    87,    76,
      77,    78,    74,    79,    75,     0,    80,    81,    82,    83,
      76,    77,    78,     0,    79,   152,     0,    80,    81,    82,
      83,     0,     0,     0,     0,     0,   155,    85,    86,    87,
       0,     0,     0,    74,     0,    75,     0,     0,    85,    86,
      87,    76,    77,    78,     0,    79,     0,     0,    80,    81,
      82,    83,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,     0,     0,    74,     0,    75,     0,     0,    85,
      86,    87,    76,    77,    78,    74,    79,    75,     0,    80,
      81,    82,    83,    76,    77,    78,     0,    79,     0,   158,
      80,    81,    82,    83,     0,     0,     0,     0,   174,     0,
      85,    86,    87,     0,     0,     0,    74,     0,    75,     0,
       0,    85,    86,    87,    76,    77,    78,     0,    79,     0,
       0,    80,    81,    82,    83,     0,     0,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,    74,     0,    75,
       0,     0,    85,    86,    87,    76,    77,    78,    74,    79,
       0,     0,    80,    81,    82,    83,    76,    77,    78,     0,
      79,     0,     0,    80,    81,    82,    83,     0,     0,     0,
       0,     0,     0,    85,    86,    87,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    76,    77,    78,
       0,    79,     0,     0,    80,    81,    82,    83,    76,    77,
      78,     0,   -43,     0,     0,    80,    81,    82,    83,     0,
       0,    76,    77,    78,     0,    85,    86,    87,   -43,   -43,
     -43,   -43,     0,     0,     0,     0,   -43,    86,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87
};

static const yytype_int16 yycheck[] =
{
       2,     5,    93,     5,    36,     0,    38,    18,    14,   100,
      12,     7,   103,    24,    25,    36,    12,    38,    14,    15,
      16,    17,    43,    28,    26,    51,    28,    51,    24,    25,
      41,    42,   172,    39,    30,    31,    32,    43,   178,    50,
      51,    20,    21,    39,    36,    51,    38,    43,    41,    42,
      51,    53,    48,    58,    50,    51,    41,    50,    51,   150,
     151,    43,    36,    65,    38,    50,    51,    43,    35,    43,
      49,    38,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    43,    85,    86,    87,   177,    36,    51,    38,
      92,    12,    37,    14,    15,    16,    17,    99,    37,   101,
     102,    37,    38,    24,    25,    36,    35,    38,   110,    30,
      31,    32,    33,    36,     7,    38,    17,    18,    39,    17,
      18,    38,    43,    36,    43,    43,    18,    48,    43,    50,
      51,    51,    36,   135,    33,    51,    43,     3,     4,     5,
       6,     7,     8,     9,    10,   147,    12,    33,    14,    15,
      16,    17,    62,   157,   158,   157,   158,   159,    24,    25,
      51,   113,   115,    -1,    30,    31,    32,    33,    34,   142,
      -1,    37,    -1,    39,    -1,    -1,    -1,    43,   182,    -1,
     182,    -1,    48,    -1,    50,    51,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    -1,    -1,    -1,
      37,    -1,    39,    -1,    -1,    12,    43,    14,    15,    16,
      17,    48,    -1,    50,    51,    -1,    -1,    24,    25,    -1,
      -1,    -1,    -1,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    43,    11,    -1,    13,
      -1,    48,    -1,    50,    51,    19,    20,    21,    -1,    23,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    11,    -1,
      13,    -1,    -1,    47,    48,    49,    19,    20,    21,    -1,
      23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    13,    -1,    -1,    47,    48,    49,    19,    20,    21,
      -1,    23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    -1,    47,    48,    49,    19,    20,
      21,    -1,    23,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    11,    -1,    13,    -1,    -1,    47,    48,    49,    19,
      20,    21,    11,    23,    13,    -1,    26,    27,    28,    29,
      19,    20,    21,    -1,    23,    35,    -1,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    35,    47,    48,    49,
      -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    47,    48,
      49,    19,    20,    21,    -1,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    47,
      48,    49,    19,    20,    21,    11,    23,    13,    -1,    26,
      27,    28,    29,    19,    20,    21,    -1,    23,    -1,    36,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    34,    -1,
      47,    48,    49,    -1,    -1,    -1,    11,    -1,    13,    -1,
      -1,    47,    48,    49,    19,    20,    21,    -1,    23,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    11,    -1,    13,
      -1,    -1,    47,    48,    49,    19,    20,    21,    11,    23,
      -1,    -1,    26,    27,    28,    29,    19,    20,    21,    -1,
      23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    49,    19,    20,    21,
      -1,    23,    -1,    -1,    26,    27,    28,    29,    19,    20,
      21,    -1,    23,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    19,    20,    21,    -1,    47,    48,    49,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    53,    54,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    12,    14,    15,    16,    17,    24,    25,    30,
      31,    32,    33,    37,    39,    43,    48,    50,    51,    55,
      57,    59,    60,    61,    62,    64,    65,    66,    67,    73,
      77,    79,    84,    87,    89,    91,    93,    51,    59,    51,
      51,    43,    82,    94,    56,    58,    60,    43,    51,    65,
      67,    65,    78,    43,    60,    33,    60,    71,    72,    74,
      75,    76,    60,    79,    11,    13,    19,    20,    21,    23,
      26,    27,    28,    29,    37,    47,    48,    49,    24,    25,
      41,    42,    50,    51,    63,    68,    69,    70,    41,    50,
      51,    88,    90,    92,    80,    51,    60,    37,    37,    79,
      54,    60,    35,    38,    35,    38,    36,    36,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    43,    43,    60,    72,    18,    43,    60,    72,    60,
      60,    72,    51,    86,    37,    36,    34,    40,    71,    76,
      51,    51,    35,    36,    60,    35,    36,    36,    36,    37,
      86,    36,    38,    43,    85,    60,    72,    72,    59,    59,
      60,    36,    83,    43,    34,    36,    36,    37,    81,    77,
      72,    77,    36,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    56,    55,    58,    57,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    61,    63,    62,    64,    64,    64,    64,    64,    65,
      65,    65,    65,    66,    66,    66,    66,    67,    67,    67,
      68,    68,    69,    70,    71,    72,    72,    72,    73,    73,
      74,    75,    75,    76,    78,    77,    80,    81,    79,    82,
      83,    79,    84,    84,    84,    84,    84,    84,    85,    85,
      86,    86,    88,    87,    87,    90,    89,    92,    91,    94,
      93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     0,     4,     1,     1,     1,     3,     1,     1,
       2,     2,     1,     3,     4,     3,     4,     4,     2,     6,
       1,     1,     3,     5,     1,     1,     3,     0,     3,     3,
       1,     1,     3,     5,     0,     4,     0,     0,     8,     0,
       0,     7,     1,     1,     1,     1,     1,     1,     1,     2,
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
#line 213 "parser.y"
              {if(for_flag == 0)yyerror("break w/o loop");}
#line 1781 "parser.c"
    break;

  case 7:
#line 214 "parser.y"
                 {if(for_flag == 0)yyerror("continue w/o loop");}
#line 1787 "parser.c"
    break;

  case 19:
#line 229 "parser.y"
                                {}
#line 1793 "parser.c"
    break;

  case 20:
#line 230 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) + (yyvsp[0].intValue);}
#line 1799 "parser.c"
    break;

  case 21:
#line 231 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) - (yyvsp[0].intValue);}
#line 1805 "parser.c"
    break;

  case 22:
#line 232 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) * (yyvsp[0].intValue);}
#line 1811 "parser.c"
    break;

  case 23:
#line 233 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) / (yyvsp[0].intValue);}
#line 1817 "parser.c"
    break;

  case 24:
#line 234 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) % (yyvsp[0].intValue);}
#line 1823 "parser.c"
    break;

  case 25:
#line 235 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) > (yyvsp[0].intValue);}
#line 1829 "parser.c"
    break;

  case 26:
#line 236 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) >= (yyvsp[0].intValue);}
#line 1835 "parser.c"
    break;

  case 27:
#line 237 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) < (yyvsp[0].intValue);}
#line 1841 "parser.c"
    break;

  case 28:
#line 238 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) <= (yyvsp[0].intValue);}
#line 1847 "parser.c"
    break;

  case 29:
#line 239 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) == (yyvsp[0].intValue);}
#line 1853 "parser.c"
    break;

  case 30:
#line 240 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) != (yyvsp[0].intValue);}
#line 1859 "parser.c"
    break;

  case 31:
#line 241 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) && (yyvsp[0].intValue);}
#line 1865 "parser.c"
    break;

  case 32:
#line 242 "parser.y"
                                {(yyval.intValue) =  (yyvsp[-2].intValue) || (yyvsp[0].intValue);}
#line 1871 "parser.c"
    break;

  case 33:
#line 243 "parser.y"
                                {}
#line 1877 "parser.c"
    break;

  case 35:
#line 247 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1883 "parser.c"
    break;

  case 36:
#line 248 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1889 "parser.c"
    break;

  case 37:
#line 249 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1895 "parser.c"
    break;

  case 38:
#line 250 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1901 "parser.c"
    break;

  case 39:
#line 251 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1907 "parser.c"
    break;

  case 40:
#line 252 "parser.y"
                          {check_if_fuction(yylval.exprNode);}
#line 1913 "parser.c"
    break;

  case 42:
#line 256 "parser.y"
                        {check_if_fuction(yylval.exprNode);}
#line 1919 "parser.c"
    break;

  case 49:
#line 267 "parser.y"
                                { yylval.exprNode = insert_ID(yylval.stringValue);  }
#line 1925 "parser.c"
    break;

  case 50:
#line 268 "parser.y"
                                { insert_local(yylval.stringValue);}
#line 1931 "parser.c"
    break;

  case 51:
#line 269 "parser.y"
                                { check_global(yylval.stringValue);}
#line 1937 "parser.c"
    break;

  case 52:
#line 270 "parser.y"
                                {}
#line 1943 "parser.c"
    break;

  case 74:
#line 319 "parser.y"
                           { 
                CURR_SCOPE++;   
                }
#line 1951 "parser.c"
    break;

  case 75:
#line 321 "parser.y"
                                              {
                        if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
#line 1960 "parser.c"
    break;

  case 76:
#line 328 "parser.y"
                   {
                function_insert(yylval.stringValue);  // insert to fuction
                fuction_scope_insert(CURR_SCOPE++);   // gia na kratame to teleutaio scope
        }
#line 1969 "parser.c"
    break;

  case 77:
#line 331 "parser.y"
                                                       {
                CURR_SCOPE--;
        }
#line 1977 "parser.c"
    break;

  case 78:
#line 333 "parser.y"
                {delete_last_fuction_scope();}
#line 1983 "parser.c"
    break;

  case 79:
#line 335 "parser.y"
                  {
                //no name fuct
                function_insert("_");  //regognize anonymous fuctions
                fuction_scope_insert(CURR_SCOPE++);  
        }
#line 1993 "parser.c"
    break;

  case 80:
#line 339 "parser.y"
                                                         {
                CURR_SCOPE--;
        }
#line 2001 "parser.c"
    break;

  case 81:
#line 341 "parser.y"
              { delete_last_fuction_scope();}
#line 2007 "parser.c"
    break;

  case 88:
#line 352 "parser.y"
           {insert_formal(yylval.stringValue);}
#line 2013 "parser.c"
    break;

  case 89:
#line 353 "parser.y"
                 {insert_formal(yylval.stringValue);}
#line 2019 "parser.c"
    break;

  case 92:
#line 361 "parser.y"
                           {if_flag = 1;}
#line 2025 "parser.c"
    break;

  case 93:
#line 361 "parser.y"
                                                                       { if_flag = 0;}
#line 2031 "parser.c"
    break;

  case 95:
#line 365 "parser.y"
                                 {for_flag = 1;}
#line 2037 "parser.c"
    break;

  case 96:
#line 365 "parser.y"
                                                                             {for_flag = 0;}
#line 2043 "parser.c"
    break;

  case 97:
#line 369 "parser.y"
                               {for_flag = 1;}
#line 2049 "parser.c"
    break;

  case 98:
#line 369 "parser.y"
                                                                                                                   {for_flag = 0;}
#line 2055 "parser.c"
    break;

  case 99:
#line 372 "parser.y"
                   {if(CURR_SCOPE == 0)yyerror("return w/o function");}
#line 2061 "parser.c"
    break;

  case 100:
#line 372 "parser.y"
                                                                                      {}
#line 2067 "parser.c"
    break;


#line 2071 "parser.c"

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
#line 374 "parser.y"


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
	 
    
    
    init_lib_func();
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
    print_format();
    return 0;
}
