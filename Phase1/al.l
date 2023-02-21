/*definitions section*/ 
%{
	#define YY_DECL int alpha_yylex (void *lvalp)
%}

/*rules section*/ 
%{
%}

%option yylineno
%option noyywrap

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





