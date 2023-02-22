%option prefix = "DMLS_yy"
%option yylineno
%option noyywrap


/*definitions section*/ 
%{
	#include <stdlib.h>
    #include <string.h>

	#define YY_DECL int alpha_yylex (void *ylval)
    
    typedef struct alpha_token_t tokenaki;

	struct alpha_token_t {
      unsigned int     numline;
      unsigned int     numToken;
      char          *content;
      char          *type;
      struct alpha_token_t *next;
	  };

      /*CREATE A TOKEN*/
	tokenaki *tokenConstractor(int numline,int numToken,char* type,char *content){
		if (type == NULL || content == NULL) {
        fprintf(stderr, "Error: lathos TYPE , CONTENT\n");
        return NULL;
    	}

		tokenaki *token = (tokenaki*)malloc(sizeof(tokenaki));
		token->numline = numline;
		token->numToken = numToken;
		token->content = strdup(content);
		token->type = strdup(type);
		token->next = NULL;
		return token;
	  }

	/*HEAD OF TOKENS*/ 
    tokenaki *first= NULL;
	/*TOKENS COUNTER*/	
	int token_count = 0;
	  
    /*ADD A NEW TOKEN TO THE LIST*/
	void insert_token(tokenaki *new_token) {
    	if (first == NULL) {
        first = new_token;
   		} 
		else {
        tokenaki *current = first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_token;
    	}
	}
	/*TESTING PRINT FUCTION*/
	void print_token(tokenaki* token){
    	printf("Token number: %d\n", token->numToken);
    	printf("Token type: %s\n", token->type);
   		printf("Line number: %d\n", token->numline);
    	printf("Content: %s\n", token->content);
	}
	  
%}

/*rules section*/ 
%{
%}

/*KEYWORDS*/
IF "if"
ELSE "else"
WHILE "while"
FOR "for"
FUNCTION "function"
RETUNR "return"
BREAK "break"
CONTINUE "continue"
AND "and"
OR "or"
LOCAL "local"
TRUE "true"
FALSE "false"
NILL "nill"

/*OPERATORS*/
ASSIGNMENT "="
ADDITION "+"
SUBTRACTION "-"
MULTI "*"
DIVISION "/"
MODULUS "%"
EQUAL "=="
NOTEQUAL "!="
INCREMENT "++"
DECREMENT "--"
GRETER_THAN ">"
LESS_THAN "<"
GRE_EQUAL ">="
LES_EQUAL "<="

/*INTEGER NUMERIC*/
INTEGER [0-9]+

/*REAL NUMERIC*/
REAL [0-9]+"."[0-9]+

/*STRINGS*/

/*STHN POREIA*/

/*PUNCTUATIONS MARK*/
LEFT_CURLY_BRACKET "{"
RIGHT_CURLY_BRACKET "}"
LEFT_SQUARE_BRACKET "["
RIGHT_SQUARE_BRACKET "]"
LEFT_PARENTHESIS "("
RIGHT_PARENTHESIS ")"
SEMICOLON ";"
COMMA ","
COLON ":"
SCOPE_RESOLUTION "::"
FULL_STOP "."
DOUBLE_FULL_STOP ".."

/*IDENTIFICATION NAME*/
IDENTIFICATION [A-Za-z][A-Za-z_0-9]*

/*COMMENTS*/

/*STHN POREIA*/


/*NOT DEFINED SYMBOLS*/
EXTRA_CHARS [!@#$^&~?|_]* 

/*ISWS TA SVISUMEEEE*/

%%
\n { yylineno; } // update yylineno every new line
[\t ' '] {};	// ignore whitespace char

[0-9]+      { tokenaki *t = tokenConstractor( yylineno,token_count++,"INTEGER",yytext);
              insert_token(t);
			  print_token(t); } //number


[0-9]+[^0-9]+ {printf("hello");} //number followed by char
		  
%%



int main(int argc,char** argv){	
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){		
			fprintf(stderr,"Something went wrong bro <3%s\n",argv[1]);			
			return 1;
		}
	}else
		yyin=stdin;
	
	
	alpha_yylex(NULL);
		
	return 0;
}





