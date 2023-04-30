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

#line 166 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
