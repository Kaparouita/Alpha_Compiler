%{
        #include <stdio.h>
        #include "Symbol_Table.h"
       

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);

        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
      
%}


/* Bison declarations and definitions */
%union {
    char* stringValue;
    int intValue;
    double realValue;
}

/*KEYWORDS*/
%token IF
%token ELSE
%token WHILE
%token FOR
%token FUNCTION
%token RETURN
%token BREAK
%token CONTINUE
%token AND 
%token NOT  
%token OR   
%token LOCAL
%token TRUE
%token FALSE
%token NILL

/*OPERATORS*/   
%token ASSIGNMENT 
%token ADDITION
%token SUBTRACTION 
%token MULTI
%token DIVISION 
%token MODULUS
%token EQUAL   
%token NOTEQUAL 
%token INCREMENT 
%token DECREMENT 
%token GRETER_THAN 
%token LESS_THAN   
%token GRE_EQUAL   
%token LES_EQUAL  


/*INTEGER NUMERIC*/
%token <intValue> INTEGER
/*REAL NUMERIC*/
%token <realValue> REAL

/*STRINGS*/
%token <stringValue> STRING

/*PUNCTUATIONS MARK*/
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET
%token LEFT_SQUARE_BRACKET
%token RIGHT_SQUARE_BRACKET
%token LEFT_PARENTHESIS
%token RIGHT_PARENTHESIS
%token SEMICOLON
%token COMMA
%token COLON
%token SCOPE_RESOLUTION
%token FULL_STOP
%token DOUBLE_FULL_STOP

/*IDENTIFICATION NAME*/
%token <stringValue> ID

/*NOT DEFINED SYMBOLS*/
%token EXTRA_CHARS


/*COMMENTS*/
%token LINE_COMMENT 
%token BLOCK_COMMENT 

%start program  /*specify the start symbol of the grammar*/



%%
program:    stmt_list 
            ;

stmt_list:  stmt        /*note thisn*/
            |stmt_list SEMICOLON stmt
            ;

break_stmt: BREAK SEMICOLON;
continue_stmt: CONTINUE SEMICOLON ;

stmt:   expr SEMICOLON
        |ifstmt
        |whilestmt { }
        |forstmt
        |returnstmt
        |break_stmt
        |continue_stmt
        |block
        |funcdef
        |SEMICOLON
        ;

expr:   assignexpr
        |expr op expr
        |term
        ;

op:     ADDITION
        |SUBTRACTION 
        |MULTI 
        |DIVISION 
        |MODULUS 
        |EQUAL    
        |NOTEQUAL  
        |GRETER_THAN 
        |LESS_THAN   
        |GRE_EQUAL   
        |LES_EQUAL   
        |AND
        |OR
        ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
        |SUBTRACTION expr
        |NOT expr
        |INCREMENT lvalue
        |lvalue INCREMENT
        |DECREMENT lvalue
        |lvalue DECREMENT
        |primary
        ;

assignexpr: lvalue ASSIGNMENT expr
            ;

primary:    lvalue
            |call
            |objectdef
            |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
            |const
            ;

lvalue:     ID
            |LOCAL ID
            |SCOPE_RESOLUTION ID
            |member
            ;

member:     lvalue FULL_STOP ID
            |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
            |call FULL_STOP ID
            |call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
            ;

call:       call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
            |lvalue callsuffix
            |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
            ;


callsuffix:     normcall
                |methodcall
                ;

normcall:   LEFT_PARENTHESIS elist RIGHT_PARENTHESIS 
            ;

methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS 
            ;    

elist:  expr
        |elist COMMA expr
        ;


objectdef: LEFT_SQUARE_BRACKET  elist RIGHT_SQUARE_BRACKET
           |LEFT_SQUARE_BRACKET   indexed  RIGHT_SQUARE_BRACKET
           ;     

indexed: LEFT_SQUARE_BRACKET  elist RIGHT_SQUARE_BRACKET
           ; 


block:  LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET
        ;

funcdef:    FUNCTION ID LEFT_PARENTHESIS  { }

const:  INTEGER
        |REAL
        |STRING
        |NILL
        |TRUE
        |FALSE
        ;

idlist:
ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt_list
        |ELSE stmt
        ;

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
           ;

/*note this for elist*/
forstmt:   FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
           ;

returnstmt: RETURN expr SEMICOLON
            ;

%%

int yyerror(char* yaccProvidedMessage){
                //provide error message example:
        fprintf(stderr,"%s: error at line %d , before token %s\n",yaccProvidedMessage,yylineno,yytext);
        fprintf(stderr,"INVALID INPUT\n");
}



/*-----------------------------MAIN-----------------------*/
int main(int argc, char **argv) {
    FILE *input_file;
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            printf("Error: failed to open input file '%s'\n", argv[1]);
            return 1;
        }
    } 
    else 
        input_file = stdin;

    printf("----------------------   Lexical Analysis  --------------------\n");
   // yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    fclose(input_file);
    return 0;
}
