%option yylineno
%option noyywrap


/*definitions section*/
%{ 
	#include <stdio.h>
	#include <stdlib.h>
    #include <string.h>
	#include "parser.h"
	
	#define YY_DECL int alpha_yylex (void *ylval)
	#define INIT_BUFFER_SIZE 256
    
    typedef struct alpha_token_t tokenaki;

	struct alpha_token_t {
      unsigned int     numline;
      unsigned int     numToken;
      char          *content;
      char          *type;
	  char          *extra_type;
	  char          *category;
      struct alpha_token_t *next;
	  };

      /*CREATE A TOKEN*/
	tokenaki *tokenConstractor(int numline,int numToken,char* extra_type,char* type,char *content,char* category){
		if (type == NULL || content == NULL) {
			fprintf(stderr, "Error: lathos TYPE , CONTENT\n");
			return NULL;
    	}

		tokenaki *token = (tokenaki*)malloc(sizeof(tokenaki));
		token->numline = numline;
		token->numToken = numToken;
		token->extra_type = strdup(extra_type);
		token->type = strdup(type);
		token->content = strdup(content);
		token->category = strdup(category);
		token->next = NULL;
		return token;
	}

	/*HEAD OF TOKENS*/ 
    tokenaki *first= NULL;
	/*TOKENS COUNTER*/	
	int token_count = 0;
	  
    /*ADD A NEW TOKEN TO THE LIST*/
	void insert_token(tokenaki *new_token) {
    	if (first == NULL) 
        	first = new_token;
		else{
			tokenaki *current = first;
			while (current->next != NULL) 
				current = current->next;
			
        	current->next = new_token;
    	}
	}

	/*TESTING PRINT FUCTION*/
	void print_token(tokenaki* token){
   		printf("%d:"" ", token->numline);
    	printf("# %d ", token->numToken);
		printf("\"");
    	printf("%s", token->content);
		printf("\"");
    	printf(" %s ", token->type);
		printf(" %s ", token->extra_type);
		printf(" <-%s\n", token->category);
	}
	

	/*add line for comments/strings */
	void do_the_job(char* text,char*type,char*Etype,char*category){
		tokenaki *t = tokenConstractor( yylineno,token_count++,Etype,type,text,category);
		if(t == NULL)
		 { token_count--;
		  return;}
        insert_token(t);
		print_token(t);
	}

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
IDENTIFICATION [A-Za-z][A-Za-z_0-9]*

/*COMMENTS*/
LINE_COMMENT "//"
BLOCK_COMMENT "/*"

/*NOT DEFINED SYMBOLS*/
EXTRA_CHARS [!@#$^&~?|_]* 


%%
\n { yylineno; } // update yylineno every new line
[\t ' '] {};	// ignore whitespace char

{IF} {do_the_job(yytext,"KEYWORD","IF","enumerated");}
{ELSE} {do_the_job(yytext,"KEYWORD","ELSE","enumerated");}
{WHILE} {do_the_job(yytext,"KEYWORD","WHILE","enumerated");}
{FOR} {do_the_job(yytext,"KEYWORD","FOR","enumerated");}
{FUNCTION} {do_the_job(yytext,"KEYWORD","FUNCTION","enumerated");}
{RETURN} {do_the_job(yytext,"KEYWORD","RETURN","enumerated");}
{BREAK} {do_the_job(yytext,"KEYWORD","BREAK","enumerated");}
{CONTINUE} {do_the_job(yytext,"KEYWORD","CONTINUE","enumerated");}
{AND} {do_the_job(yytext,"KEYWORD","AND","enumerated");}
{NOT} {do_the_job(yytext,"KEYWORD","NOT","enumerated");}
{OR} {do_the_job(yytext,"KEYWORD","OR","enumerated");}
{LOCAL} {do_the_job(yytext,"KEYWORD","LOCAL","enumerated");}
{TRUE} {do_the_job(yytext,"KEYWORD","TRUE","enumerated");}
{FALSE} {do_the_job(yytext,"KEYWORD","FALSE","enumerated");}
{NILL} {do_the_job(yytext,"KEYWORD","NILL","enumerated");}

{ASSIGNMENT} {do_the_job(yytext,"OPERATOR","ASSIGNMENT","enumerated");}
{ADDITION} {do_the_job(yytext,"OPERATOR","ADDITION","enumerated");}
{SUBTRACTION} {do_the_job(yytext,"OPERATOR","SUBTRACTION","enumerated");}
{MULTI} {do_the_job(yytext,"OPERATOR","MULTI","enumerated");}
{DIVISION} {do_the_job(yytext,"OPERATOR","DIVISION","enumerated");}
{MODULUS} {do_the_job(yytext,"OPERATOR","MODULUS","enumerated");}
{EQUAL} {do_the_job(yytext,"OPERATOR","EQUAL","enumerated");}
{NOTEQUAL} {do_the_job(yytext,"OPERATOR","NOTEQUAL","enumerated");}
{INCREMENT} {do_the_job(yytext,"OPERATOR","INCREMENT","enumerated");}
{DECREMENT} {do_the_job(yytext,"OPERATOR","DECREMENT","enumerated");}
{GRETER_THAN} {do_the_job(yytext,"OPERATOR","GRETER_THAN","enumerated");}
{LESS_THAN} {do_the_job(yytext,"OPERATOR","LESS_THAN","enumerated");}
{GRE_EQUAL} {do_the_job(yytext,"OPERATOR","GRE_EQUAL","enumerated");}
{LES_EQUAL} {do_the_job(yytext,"OPERATOR","LES_EQUAL","enumerated");}


{INTEGER} { do_the_job(yytext,"CONST_INT",yytext,"integer"); } //number
{REAL} {do_the_job(yytext,"REAL_NUMBER",yytext,"real");}

{LEFT_CURLY_BRACKET} { do_the_job(yytext,"PUNCTUATION","LEFT_CURLY_BRACKET","enumerated");}
{RIGHT_CURLY_BRACKET} { do_the_job(yytext,"PUNCTUATION","RIGHT_CURLY_BRACKET","enumerated");}
{LEFT_SQUARE_BRACKET} { do_the_job(yytext,"PUNCTUATION","LEFT_SQUARE_BRACKET","enumerated");}
{RIGHT_SQUARE_BRACKET} { do_the_job(yytext,"PUNCTUATION","RIGHT_SQUARE_BRACKET","enumerated");}
{LEFT_PARENTHESIS} { do_the_job(yytext,"PUNCTUATION","LEFT_PARENTHESIS","enumerated");}
{RIGHT_PARENTHESIS} { do_the_job(yytext,"PUNCTUATION","RIGHT_PARENTHESIS","enumerated");}
{SEMICOLON} { do_the_job(yytext,"PUNCTUATION","SEMICOLON","enumerated");}
{COMMA} { do_the_job(yytext,"PUNCTUATION","COMMA","enumerated");}
{COLON} { do_the_job(yytext,"PUNCTUATION","COLON","enumerated");}
{SCOPE_RESOLUTION} { do_the_job(yytext,"PUNCTUATION","SCOPE_RESOLUTION","enumerated");}
{FULL_STOP} { do_the_job(yytext,"PUNCTUATION","FULL_STOP","enumerated");}
{DOUBLE_FULL_STOP} { do_the_job(yytext,"PUNCTUATION","DOUBLE_FULL_STOP","enumerated");}

{IDENTIFICATION} { do_the_job(yytext,"ID",yytext,"char*");}

{EXTRA_CHARS} { do_the_job(yytext,"UNDEF","UNDEF","UNDEF");}

{STRINGS} {
	int i ,buffer_counter = 0,starter_line = yylineno;
	int buffer_size = INIT_BUFFER_SIZE;
    char *buffer = (char*)malloc(sizeof(char*)*buffer_size),prev;
	while((i = input()) >= 1)
	{	
		
		if(buffer_counter >= buffer_size - 1){
			buffer_size *= 2;
			buffer = (char*) realloc(buffer, buffer_size * sizeof(char));
		}
		if(prev == '\\' && ( i == 't' || i == 'n' )){
			buffer[buffer_counter-1] = (i == 't') ? '\t' : '\n';
		}
	    else if(i == '\"' && prev != '\\'){
			do_the_job(buffer,"STRING",buffer,"<-char*");
			break;
		}
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
	while((i = input()) >=1 || i=='\n')
	{
		if(i == '\n'){
			unput(i);
			do_the_job("","COMMENT","LINE_COMMENT","enumerated");
			break;
		}
		else if(i=='/'){
			if((i = input()) == '/')  
			   do_the_job("","COMMENT","NESTED_LINE_COMMENT","enumerated");
			else if(i == '*'){
				if(star_comment_checker(i,1) == 1)
					printf("ERROR : UNCLOSED STAR_COMMENT LINE : %d\n",yylineno);		
			}
		    else
			   unput(i);			   
		}
	}
	if(i <= 1)
		do_the_job("","COMMENT","LINE_COMMENT","enumerated");
}



{BLOCK_COMMENT} {
	int i , line = yylineno;
	while((i = input()) >= 1)
	{	
	    if(i == '/'){
			if((i = input()) == '*')  
			{
				if(star_comment_checker(i,0) == 1)
					break;	
				else
			   		unput(i);
			} 
			else if(i == '/')  
			   		do_the_job("","COMMENT","NESTED_LINE_COMMENT","enumerated");
		}
		else if(i == '*'){
			if((i = input()) == '/'){				
			   do_the_job(create_string(line,yylineno),"COMMENT" ,"BLOCK_COMMENT","enumerated");
			   break;
			   }
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
		if(what == 1 && i == '\n')
		    {   unput(i);
				return 1;}
		if(i == '*'){
			if((i = input()) == '/'){
				do_the_job(create_string(line,yylineno),"COMMENT","NESTED_STAR_COMMENT","enumerated");
					return 0;
			}
			else
				unput(i);
		}
		else if(i == '/'){
            if((i = input()) == '*'){
				if(what == 1)
					star_comment_checker(i,1);
			    else
					star_comment_checker(i,0);		
			}
			else if(i == '/')  
			   	do_the_job("","COMMENT","NESTED_LINE_COMMENT","enumerated");
			else
				unput(i);
		}					
	}
	if(i >= 1 && what != 1)
	  exit(0);
	unput(i);
	return 1; 	
}


int main(int argc,char** argv){	
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){		
			fprintf(stderr,"Something went wrong bro <3%s\n",argv[1]);			
			return 1;
		}
	}else
		yyin=stdin;
		printf("----------------------   Lexical Analysis  --------------------\n");
	
	alpha_yylex(first);//nmzw prepei na einai etsi <---
	return 0;
}
