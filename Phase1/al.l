%option prefix = "DMLS_yy"
%option yylineno
%option noyywrap


/*definitions section*/
%{
	#include <stdio.h>
	#include <stdlib.h>
    #include <string.h>

	#define YY_DECL int alpha_yylex (void *ylval)
    
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
   		printf(": %d ", token->numline);
    	printf("# %d ", token->numToken);
		printf("\"");
    	printf(" %s ", token->content);
		printf("\"");
    	printf(" %s ", token->type);
		printf(" %s ", token->extra_type);
		printf(" %s\n", token->category);
	}
	
	void do_the_job(char*type,char*Etype,char*category){
		tokenaki *t = tokenConstractor( yylineno,token_count++,Etype,type,yytext,category);
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
<<<<<<< HEAD
=======
LINE_COMMENT "//"
BLOCK_COMMENT "/*"
>>>>>>> refs/remotes/origin/main

/*INTEGER NUMERIC*/
INTEGER [0-9]+

/*REAL NUMERIC*/
REAL [0-9]+"."[0-9]+

/*STRINGS*/
STRING " \" /" "

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

/*NOT DEFINED SYMBOLS*/
EXTRA_CHARS [!@#$^&~?|_]* 

/*ISWS TA SVISUMEEEE*/

%%
\n { yylineno; } // update yylineno every new line
[\t ' '] {};	// ignore whitespace char

{IF} {do_the_job("KEYWORD","IF","enumerated");}
{ELSE} {do_the_job("KEYWORD","ELSE","enumerated");}
{WHILE} {do_the_job("KEYWORD","WHILE","enumerated");}
{FOR} {do_the_job("KEYWORD","FOR","enumerated");}
{FUNCTION} {do_the_job("KEYWORD","FUNCTION","enumerated");}
{RETURN} {do_the_job("KEYWORD","RETURN","enumerated");}
{BREAK} {do_the_job("KEYWORD","BREAK","enumerated");}
{CONTINUE} {do_the_job("KEYWORD","CONTINUE","enumerated");}
{AND} {do_the_job("KEYWORD","AND","enumerated");}
{NOT} {do_the_job("KEYWORD","NOT","enumerated");}
{OR} {do_the_job("KEYWORD","OR","enumerated");}
{LOCAL} {do_the_job("KEYWORD","LOCAL","enumerated");}
{TRUE} {do_the_job("KEYWORD","TRUE","enumerated");}
{FALSE} {do_the_job("KEYWORD","FALSE","enumerated");}
{NILL} {do_the_job("KEYWORD","NILL","enumerated");}

{ASSIGNMENT} {do_the_job("OPERATOR","ASSIGNMENT","enumerated");}
{ADDITION} {do_the_job("OPERATOR","ADDITION","enumerated");}
{SUBTRACTION} {do_the_job("OPERATOR","SUBTRACTION","enumerated");}
{MULTI} {do_the_job("OPERATOR","MULTI","enumerated");}
{DIVISION} {do_the_job("OPERATOR","DIVISION","enumerated");}
{MODULUS} {do_the_job("OPERATOR","MODULUS","enumerated");}
{EQUAL} {do_the_job("OPERATOR","EQUAL","enumerated");}
{NOTEQUAL} {do_the_job("OPERATOR","NOTEQUAL","enumerated");}
{INCREMENT} {do_the_job("OPERATOR","INCREMENT","enumerated");}
{DECREMENT} {do_the_job("OPERATOR","DECREMENT","enumerated");}
{GRETER_THAN} {do_the_job("OPERATOR","GRETER_THAN","enumerated");}
{LESS_THAN} {do_the_job("OPERATOR","LESS_THAN","enumerated");}
{GRE_EQUAL} {do_the_job("OPERATOR","GRE_EQUAL","enumerated");}
{LES_EQUAL} {do_the_job("OPERATOR","LES_EQUAL","enumerated");}


{INTEGER} { do_the_job("CONST_INT",yytext,"integer"); } //number
{REAL} {do_the_job("REAL_NUMBER",yytext,"real");}

{LEFT_CURLY_BRACKET} { do_the_job("PUNCTUATION","LEFT_CURLY_BRACKET","enumerated");}
{RIGHT_CURLY_BRACKET} { do_the_job("PUNCTUATION","RIGHT_CURLY_BRACKET","enumerated");}
{LEFT_SQUARE_BRACKET} { do_the_job("PUNCTUATION","LEFT_SQUARE_BRACKET","enumerated");}
{RIGHT_SQUARE_BRACKET} { do_the_job("PUNCTUATION","RIGHT_SQUARE_BRACKET","enumerated");}
{LEFT_PARENTHESIS} { do_the_job("PUNCTUATION","LEFT_PARENTHESIS","enumerated");}
{RIGHT_PARENTHESIS} { do_the_job("PUNCTUATION","RIGHT_PARENTHESIS","enumerated");}
{SEMICOLON} { do_the_job("PUNCTUATION","SEMICOLON","enumerated");}
{COMMA} { do_the_job("PUNCTUATION","COMMA","enumerated");}
{COLON} { do_the_job("PUNCTUATION","COLON","enumerated");}
{SCOPE_RESOLUTION} { do_the_job("PUNCTUATION","SCOPE_RESOLUTION","enumerated");}
{FULL_STOP} { do_the_job("PUNCTUATION","FULL_STOP","enumerated");}
{DOUBLE_FULL_STOP} { do_the_job("PUNCTUATION","DOUBLE_FULL_STOP","enumerated");}

{IDENTIFICATION} { do_the_job(yytext,"ID","char*");}

{EXTRA_CHARS} { do_the_job("EXTRA_CHARS","EXTRA_CHARS","EXTRA_CHARS");
				printf("Undefined chars:*\n");}

{LINE_COMMENT} {
	int i ;
	//TO EOF EIXE BUG
	while((i = input()) >=1 || i=='\n')
	{
		if(i == '\n'){
			unput(i);
			do_the_job("","COMMENT LINE_COMMENT","enumerated");
			break;
		}
		else if(i=='/'){
<<<<<<< HEAD
			i = input();
			int tmp = i;
			if(tmp == '/')
			   do_the_job("","NESTED_COMMENT LINE_COMMENT","enumerated");
			else if(tmp == '*'){
				if(star_comment_checker(i) == 1)
=======
			if((i = input()) == '/')  
			   do_the_job("","NESTED_COMMENT LINE_COMMENT","enumerated");
			else if(i == '*'){
				if(star_comment_checker(i,1) == 1)
>>>>>>> refs/remotes/origin/main
					printf("ERROR : UNCLOSED STAR_COMMENT LINE : %d\n",yylineno);		
			}
		    else
			   unput(i);			   
		}
	}
	if(i <= 1)
		do_the_job("","COMMENT LINE_COMMENT","enumerated");
}


{STRING} {

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
			   		do_the_job("","NESTED_COMMENT LINE_COMMENT","enumerated");
		}
		else if(i == '*'){
			if((i = input()) == '/'){
				
			   do_the_job(create_string(line,yylineno),"COMMENT BLOCK?_COMMENT","enumerated");
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

<<<<<<< HEAD
int star_comment_checker(int i){
	while((i = input()) != '\n' && i <=1){
				if(i == '*')
					{if(i = input() == '/'){
						do_the_job("","NESTED_COMMENT STAR_COMMENT","enumerated");
						return 0;
					}
					else
						unput(i);}					
=======
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
				do_the_job(create_string(line,yylineno),"NESTED_COMMENT STAR_COMMENT","enumerated");
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
			   	do_the_job("","NESTED_COMMENT LINE_COMMENT","enumerated");
			else
				unput(i);
		}					
>>>>>>> refs/remotes/origin/main
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





