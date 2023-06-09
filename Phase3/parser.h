/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    WHILE = 260,                   /* WHILE  */
    FOR = 261,                     /* FOR  */
    FUNCTION = 262,                /* FUNCTION  */
    RETURN = 263,                  /* RETURN  */
    BREAK = 264,                   /* BREAK  */
    CONTINUE = 265,                /* CONTINUE  */
    AND = 266,                     /* AND  */
    NOT = 267,                     /* NOT  */
    OR = 268,                      /* OR  */
    LOCAL = 269,                   /* LOCAL  */
    NILL = 270,                    /* NILL  */
    ASSIGNMENT = 271,              /* ASSIGNMENT  */
    ADDITION = 272,                /* ADDITION  */
    SUBTRACTION = 273,             /* SUBTRACTION  */
    MULTI = 274,                   /* MULTI  */
    DIVISION = 275,                /* DIVISION  */
    MODULUS = 276,                 /* MODULUS  */
    UMINUS = 277,                  /* UMINUS  */
    EQUAL = 278,                   /* EQUAL  */
    NOTEQUAL = 279,                /* NOTEQUAL  */
    INCREMENT = 280,               /* INCREMENT  */
    DECREMENT = 281,               /* DECREMENT  */
    GRETER_THAN = 282,             /* GRETER_THAN  */
    LESS_THAN = 283,               /* LESS_THAN  */
    GRE_EQUAL = 284,               /* GRE_EQUAL  */
    LES_EQUAL = 285,               /* LES_EQUAL  */
    INTEGER = 286,                 /* INTEGER  */
    REAL = 287,                    /* REAL  */
    STRING = 288,                  /* STRING  */
    ID = 289,                      /* ID  */
    TRUE = 290,                    /* TRUE  */
    FALSE = 291,                   /* FALSE  */
    LEFT_CURLY_BRACKET = 292,      /* LEFT_CURLY_BRACKET  */
    RIGHT_CURLY_BRACKET = 293,     /* RIGHT_CURLY_BRACKET  */
    LEFT_SQUARE_BRACKET = 294,     /* LEFT_SQUARE_BRACKET  */
    RIGHT_SQUARE_BRACKET = 295,    /* RIGHT_SQUARE_BRACKET  */
    LEFT_PARENTHESIS = 296,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 297,       /* RIGHT_PARENTHESIS  */
    SEMICOLON = 298,               /* SEMICOLON  */
    COMMA = 299,                   /* COMMA  */
    SCOPE_RESOLUTION = 300,        /* SCOPE_RESOLUTION  */
    COLON = 301,                   /* COLON  */
    FULL_STOP = 302,               /* FULL_STOP  */
    DOUBLE_FULL_STOP = 303,        /* DOUBLE_FULL_STOP  */
    EXTRA_CHARS = 304,             /* EXTRA_CHARS  */
    LINE_COMMENT = 305,            /* LINE_COMMENT  */
    BLOCK_COMMENT = 306            /* BLOCK_COMMENT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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
#line 153 "parser.y"

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

#line 183 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
