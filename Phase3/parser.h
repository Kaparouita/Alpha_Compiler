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
    EQUAL = 277,                   /* EQUAL  */
    NOTEQUAL = 278,                /* NOTEQUAL  */
    INCREMENT = 279,               /* INCREMENT  */
    DECREMENT = 280,               /* DECREMENT  */
    GRETER_THAN = 281,             /* GRETER_THAN  */
    LESS_THAN = 282,               /* LESS_THAN  */
    GRE_EQUAL = 283,               /* GRE_EQUAL  */
    LES_EQUAL = 284,               /* LES_EQUAL  */
    INTEGER = 285,                 /* INTEGER  */
    REAL = 286,                    /* REAL  */
    STRING = 287,                  /* STRING  */
    ID = 288,                      /* ID  */
    TRUE = 289,                    /* TRUE  */
    FALSE = 290,                   /* FALSE  */
    LEFT_CURLY_BRACKET = 291,      /* LEFT_CURLY_BRACKET  */
    RIGHT_CURLY_BRACKET = 292,     /* RIGHT_CURLY_BRACKET  */
    LEFT_SQUARE_BRACKET = 293,     /* LEFT_SQUARE_BRACKET  */
    RIGHT_SQUARE_BRACKET = 294,    /* RIGHT_SQUARE_BRACKET  */
    LEFT_PARENTHESIS = 295,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 296,       /* RIGHT_PARENTHESIS  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    SCOPE_RESOLUTION = 299,        /* SCOPE_RESOLUTION  */
    COLON = 300,                   /* COLON  */
    FULL_STOP = 301,               /* FULL_STOP  */
    DOUBLE_FULL_STOP = 302,        /* DOUBLE_FULL_STOP  */
    EXTRA_CHARS = 303,             /* EXTRA_CHARS  */
    LINE_COMMENT = 304,            /* LINE_COMMENT  */
    BLOCK_COMMENT = 305            /* BLOCK_COMMENT  */
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
        struct  call_s *callValue;

#line 179 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
