%option yylineno
%option noyywrap


/*definitions section*/
%{ 
	#include "parser.h"
	
	#define YY_DECL int alpha_yylex (void *ylval)
	#define INIT_BUFFER_SIZE 256
    
    /*creates a string with 2 integers for block comments printing*/
	char *create_string(int a,int b){
		char* str = malloc(sizeof(char) * 30);
		sprintf(str,"%d - %d",a,b);
		return str;
	}
	
	/*
	 * return 0 if succed
	 * return 1 if fails(not nested comment found)
	*/ 
	int star_comment_checker(int i,int what);
%}

/*KEYWORDS*/
IF "if"
ELSE "else"
WHILE "while"
FOR "for"
FUNCTION "function"
RETURN "return"
BREAK "break"
CONTINUE "continue"
AND "and"
NOT "not"
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
STRINGS "\""

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
ID [A-Za-z][A-Za-z_0-9]*

/*COMMENTS*/
LINE_COMMENT "//"
BLOCK_COMMENT "/*"

/*NOT DEFINED SYMBOLS*/
EXTRA_CHARS [!@#$^&~?|_]* 


%%
\n { yylineno; } // update yylineno every new line
[\t ' '] {};	// ignore whitespace char

{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
{FOR} {return FOR;}
{FUNCTION} {return FUNCTION;}
{RETURN} {return RETURN;}
{BREAK} {return BREAK;}
{CONTINUE} {return CONTINUE;}
{AND} {return AND;}
{NOT} {return NOT;}
{OR} {return OR;}
{LOCAL} {return LOCAL;}
{TRUE} {return TRUE;}
{FALSE} {return FALSE;}
{NILL} {return NILL;}

{ASSIGNMENT} {return ASSIGNMENT;}
{ADDITION} {return ADDITION;}
{SUBTRACTION} {return SUBTRACTION;}
{MULTI} {return MULTI;}
{DIVISION} {return DIVISION;}
{MODULUS} {return MODULUS;}
{EQUAL} {return EQUAL;}
{NOTEQUAL} {return NOTEQUAL;}
{INCREMENT} {return INCREMENT;}
{DECREMENT} {return DECREMENT;}
{GRETER_THAN} {return GRETER_THAN;}
{LESS_THAN} {return LESS_THAN;}
{GRE_EQUAL} {return GRE_EQUAL;}
{LES_EQUAL} {return LES_EQUAL;}


{INTEGER} {/*DEN KSERW AKOMA TI VAZUME*/ } //number
{REAL} {/*DEN KSERW AKOMA TI VAZUME*/}

{LEFT_CURLY_BRACKET} { return LEFT_CURLY_BRACKET;}
{RIGHT_CURLY_BRACKET} { return RIGHT_CURLY_BRACKET;}
{LEFT_SQUARE_BRACKET} { return LEFT_SQUARE_BRACKET;}
{RIGHT_SQUARE_BRACKET} { return RIGHT_SQUARE_BRACKET;}
{LEFT_PARENTHESIS} { return LEFT_PARENTHESIS;}
{RIGHT_PARENTHESIS} { return RIGHT_PARENTHESIS;}
{SEMICOLON} { return SEMICOLON;}
{COMMA} { return COMMA;}
{COLON} { return COLON;}
{SCOPE_RESOLUTION} { return SCOPE_RESOLUTION;}
{FULL_STOP} { return FULL_STOP;}
{DOUBLE_FULL_STOP} { return DOUBLE_FULL_STOP;}

{ID} { return ID;}

{EXTRA_CHARS} { return EXTRA_CHARS;}

{STRINGS} {
	int i ,buffer_counter = 0,starter_line = yylineno;
	int buffer_size = INIT_BUFFER_SIZE;
    char *buffer = (char*)malloc(sizeof(char*)*buffer_size),prev;
	while((i = input()) >= 1){	
		if(buffer_counter >= buffer_size - 1){
			buffer_size *= 2;
			buffer = (char*) realloc(buffer, buffer_size * sizeof(char));
		}
		if(prev == '\\' && ( i == 't' || i == 'n' ))
			buffer[buffer_counter-1] = (i == 't') ? '\t' : '\n';
	    else if(i == '\"' && prev != '\\')
			return STRING;
		else
			buffer[buffer_counter++] = i;
		prev = i;	  
	}
	if(i <= 1)
		printf("ERROR : UNCLOSED STRING REACHED EOF ");	

}

{LINE_COMMENT} {
	int i ;
	//TO EOF EIXE BUG
	while((i = input()) >=1 || i=='\n'){
		if(i == '\n'){
			unput(i);
			return LINE_COMMENT;
		}else if(i=='/'){
			if((i = input()) == '/')  
			  return LINE_COMMENT;
			else if(i == '*'){
				if(star_comment_checker(i,1) == 1)
					printf("ERROR : UNCLOSED STAR_COMMENT LINE : %d\n",yylineno);		
			}
		    else
			   unput(i);			   
		}
	}
	if(i <= 1)
		return LINE_COMMENT;
}

{BLOCK_COMMENT} {
	int i , line = yylineno;
	while((i = input()) >= 1){	
	    if(i == '/'){
			if((i = input()) == '*'){
				if(star_comment_checker(i,0) == 1)
					break;	
				else
			   		unput(i);
			}else if(i == '/')  
			   		return LINE_COMMENT;
		}else if(i == '*'){
			if((i = input()) == '/')			
			   return BLOCK_COMMENT;
			else
			   unput(i);
		}		   	   
	}
	if(i <= 1)
		printf("ERROR : UNCLOSED STAR_COMMENT REACHED EOF ");	
}


%%

/* Checks if the comment
 * what = 1 check for line
 * else check for multiline
 */
int star_comment_checker(int i,int what){
	int line = yylineno;
	while((i = input()) >= 1){
		if(what == 1 && i == '\n'){
			unput(i);
			return 1;
		}
		if(i == '*'){
			if((i = input()) == '/')				
				return 0;
			else
				unput(i);
		}else if(i == '/'){
            if((i = input()) == '*'){
				if(what == 1)
					star_comment_checker(i,1);
			    else
					star_comment_checker(i,0);		
			}else if(i == '/')  
			   	return 0;
			else
				unput(i);
		}					
	}

	if(i >= 1 && what != 1)
	  exit(0);
	  
	unput(i);
	return 1; 	
}




