%{
    /*C code included here*/    
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
%token BREAK;
%token CONTINUE
%token AND 'and'
%token NOT  'not'
%token OR   'or'
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
program:    stmts 
            ;

stmt_list:  stmt
            |stmt_list SEMICOLON stmt
            ;

break_stmt: BREAK SEMICOLON;
continue_stmt: CONTINUE SEMICOLON ;

stmt:   expr SEMICOLON
        |ifstmt
        |whilestmt
        |forstmt
        |returnstmt
        |break_stmt
        |continue_stmt
        |block
        |fundef
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
            |LEFT_PARENTHESIS fundef RIGHT_PARENTHESIS
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
            |LEFT_PARENTHESIS fundef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
            ;

callsuffix:     normcall
                |methodcall
                ;

normcall:   LEFT_PARENTHESIS elist RIGHT_PARENTHESI 
            ;

methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
            ;







