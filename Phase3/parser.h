/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 173 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
