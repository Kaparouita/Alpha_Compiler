%{
#define YY_DECL int alpha_yylex(void* ylval)
	
	struct alpha_token_t {
	  unsigned int numline;
	  unsigned int numToken;
	  char *content;
	  char *type;
	  char *sub_type;
	  char *category;
	  struct alpha_token_t *next;
	};

	
	
	struct alpha_token_t *head = NULL;
	
	int counterToken=0;
	
	void add_token(char* content, char* typeOfToken,char* sub,char* cat){
		struct alpha_token_t *new_token=malloc(sizeof(struct alpha_token_t));
		struct alpha_token_t *temp_token;		
	
		new_token->content = malloc(strlen(content)+1);
		
		new_token->type = malloc(strlen(typeOfToken)+1);
		new_token->sub_type = malloc(strlen(sub)+1);
		new_token->category = malloc(strlen(cat)+1);
		
		if ( head==NULL){
			head=new_token;
			new_token->next=NULL;	
			new_token->numline=yylineno;
						
            strcpy(new_token->content,content);
			strcpy(new_token->type,typeOfToken);
			strcpy(new_token->sub_type, sub);
			strcpy(new_token->category,cat);
			
			if (strcmp(new_token->type, "UNDEFINED")==0){
				  head->numToken=counterToken;
			}else{
				counterToken++;
				new_token->numToken=counterToken;
			}
			
			
		}else{
			temp_token=head;
			while(temp_token->next!=NULL){
				temp_token=temp_token->next;
			}
			temp_token->next=new_token;
			new_token->next=NULL;
			new_token->numline=yylineno;
			
			
			strcpy(new_token->content,content);
			strcpy(new_token->type,typeOfToken);
			strcpy(new_token->sub_type, sub);
			strcpy(new_token->category,cat);
			
			if (strcmp(new_token->type, "UNDEFINED")==0){
				new_token->numToken=counterToken;
			}else{
				counterToken++;
				new_token->numToken=counterToken;
			}
		}
		
		
	}

	
	
	void print_token(){
		struct alpha_token_t *temp=head;
		printf("\n-------------------------------   LEXICAL ANALYSIS   -------------------------------\n\n");
		while(temp!=NULL){
			if(strcmp(temp->type, "ID")==0 || strcmp(temp->type, "STRING")==0){
				printf("%d: #%d    \"%s\"     %s    \"%s\"     <-%s \n", temp->numline, temp->numToken, temp->content, temp->type, temp->sub_type, temp->category);
			}else if (strcmp(temp->type, "UNDEFINED")==0){
				printf("%d: Error: undefined character %s \n",temp->numline,temp->content);
			}else {
				printf("%d: #%d    \"%s\"     %s    %s     <-%s \n", temp->numline, temp->numToken, temp->content, temp->type, temp->sub_type, temp->category);
			}
			temp=temp->next;
		
		}
	}
	void fprint_token(FILE *output){
        
        struct alpha_token_t *temp=head;
		fprintf(output,"\n-------------------------------   LEXICAL ANALYSIS   -------------------------------\n\n");
		while(temp!=NULL){
			if(strcmp(temp->type, "ID")==0 || strcmp(temp->type, "STRING")==0){
				fprintf(output,"%d: #%d    \"%s\"     %s    \"%s\"     <-%s \n", temp->numline, temp->numToken, temp->content, temp->type, temp->sub_type, temp->category);
			}else if (strcmp(temp->type, "UNDEFINED")==0){
				fprintf(output,"%d: Error: undefined character %s \n",temp->numline,temp->content);
			}else{
				fprintf(output,"%d: #%d    \"%s\"     %s    %s     <-%s \n", temp->numline, temp->numToken, temp->content, temp->type, temp->sub_type, temp->category);
			}
			temp=temp->next;
		
		}
        fclose(output);
    }
	
%}

%option yylineno
%option noyywrap

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

ASSIGN "="
EQUAL "=="
PLUS "+"
PLUS_PLUS "++"
MINUS "-"
MINUS_MINUS "--"
MULTI "*"
DIV "/"
MOD "%"
NOT_EQUAL "!="
LESS_THAN "<"
GREATER_THAN ">"
LESS_EQUAL "<="
GREATER_EQUAL ">="

INTEGER [0-9]+
REAL [0-9]+"."[0-9]+

STRING \"

OPENED_BRACES "{"
CLOSED_BRACES "}"
OPENED_BRACKETS "["
CLOSED_BRACKETS "]"
OPENED_PARENTHESES "("
CLOSED_PARENTHESES ")"
SEMICOLON ";"
COMMA ","
COLON ":"
DOUBLE_COLON "::"
DOT "."
DOUBLE_DOT ".."

ID [a-zA-Z][a-zA-Z_0-9]*

UNDEFINED_CHAR [$!@#&_$~?^|]*

COMMENT1 "//".*
COMMENT2 "/*"



%%
{UNDEFINED_CHAR} {add_token(yytext,"UNDEFINED","UND","UND");}

{IF} {add_token(yytext,"KEYWORD","IF","enumerated");}  
{ELSE} {add_token(yytext,"KEYWORD","ELSE","enumerated");}  
{WHILE} {add_token(yytext,"KEYWORD","WHILE","enumerated");}  
{FOR} {add_token(yytext,"KEYWORD","WHILE","enumerated");}  
{FUNCTION} {add_token(yytext,"KEYWORD","FUNCTION","enumerated");}  
{RETURN} {add_token(yytext,"KEYWORD","RETURN","enumerated");}  
{BREAK} {add_token(yytext,"KEYWORD","BREAK","enumerated");}  
{CONTINUE} {add_token(yytext,"KEYWORD","CONTINUE","enumerated");}  
{AND} {add_token(yytext,"KEYWORD","AND","enumerated");}  
{NOT} {add_token(yytext,"KEYWORD","NOT","enumerated");}  
{OR} {add_token(yytext,"KEYWORD","OR","enumerated");}  
{LOCAL} {add_token(yytext,"KEYWORD","LOCAL","enumerated");}  
{TRUE} {add_token(yytext,"KEYWORD","TRUE","enumerated");}  
{FALSE} {add_token(yytext,"KEYWORD","FALSE","enumerated");}  
{NILL} {add_token(yytext,"KEYWORD","NILL","enumerated");}  


{ASSIGN} {add_token(yytext,"OPERATOR","ASSIGN","enumerated");}  
{EQUAL} {add_token(yytext,"OPERATOR","EQUAL","enumerated");}  
{PLUS} {add_token(yytext,"OPERATOR","PLUS","enumerated");} 
{PLUS_PLUS} {add_token(yytext,"OPERATOR","PLUS_PLUS","enumerated");}  
{MINUS} {add_token(yytext,"OPERATOR","MINUS","enumerated");}  
{MINUS_MINUS} {add_token(yytext,"OPERATOR","MINUS_MINUS","enumerated");}  
{MULTI} {add_token(yytext,"OPERATOR","MULTI","enumerated");}  
{DIV} {add_token(yytext,"OPERATOR","DIV","enumerated");} 
{MOD} {add_token(yytext,"OPERATOR","MOD","enumerated");} 
{NOT_EQUAL} {add_token(yytext,"OPERATOR","NOT_EQUAL","enumerated");} 
{LESS_THAN} {add_token(yytext,"OPERATOR","LESS_THAN","enumerated");} 
{GREATER_THAN} {add_token(yytext,"OPERATOR","GREATER_THAN","enumerated");} 
{LESS_EQUAL} {add_token(yytext,"OPERATOR","LESS_EQUAL","enumerated");} 
{GREATER_EQUAL} {add_token(yytext,"OPERATOR","GREATER_EQUAL","enumerated");} 

{INTEGER} {add_token(yytext,"INTCONST",yytext,"int");} 
{REAL} {add_token(yytext,"REALCONST",yytext,"real");} 



"\"" {

	 
	 int c, counter=0,quotecnt=1;
	 char* string;
	 int buff_size=100;
	 string = malloc(sizeof(char*)*buff_size);
		while((c=input())!=-1 && c!=0){
			if(c=='\\'){
				c=input();
				if(c!=EOF && c=='n'){
					string[counter]='\n';
					counter++;
				}
				else if(c!=EOF && c=='t'){
					string[counter]='\t';
					counter++;
				}
				else if(c!=EOF && c=='\\'){
					string[counter]='\\';
					counter++;
				}
				else if(c!=EOF && c=='\"'){
					string[counter]='"';
					counter++;
				}
			}
			else if (c=='\"'){
				quotecnt++;
				add_token(string,"STRING",string,"char*");
				break;
				}
			else{
				string[counter]=c;
				counter++;
			}	
			if(buff_size<counter){
				buff_size=counter*2;
				string= realloc(string,buff_size);
			}
		}
		if((quotecnt % 2 !=0) && (c==0 || c==-1)){
			print_token();
			printf("ERROR:Missing quotes\n");
			exit(0);
		}
	}


{OPENED_BRACES} {add_token(yytext,"PUNCTUATION","LEFT_BRACES","enumerated");}
{CLOSED_BRACES} {add_token(yytext,"PUNCTUATION","RIGHT_BRACES","enumerated");}
{OPENED_BRACKETS} {add_token(yytext,"PUNCTUATION","LEFT_BRACKETS","enumerated");}
{CLOSED_BRACKETS} {add_token(yytext,"PUNCTUATION","RIGHT_BRACKETS","enumerated");}
{OPENED_PARENTHESES} {add_token(yytext,"PUNCTUATION","LEFT_PARENTHESES","enumerated");}
{CLOSED_PARENTHESES} {add_token(yytext,"PUNCTUATION","RIGHT_PARENTHESES","enumerated");} 
{SEMICOLON} {add_token(yytext,"PUNCTUATION","SEMICOLON","enumerated");}
{COMMA} {add_token(yytext,"PUNCTUATION","COMMA","enumerated");} 
{COLON} {add_token(yytext,"PUNCTUATION","COLON","enumerated");} 
{DOUBLE_COLON} {add_token(yytext,"PUNCTUATION","DOUBLE_COLON","enumerated");} 
{DOT} {add_token(yytext,"PUNCTUATION","DOT","enumerated");} 
{DOUBLE_DOT} {add_token(yytext,"PUNCTUATION","DOUBLE_DOT","enumerated");} 

{ID} {add_token(yytext,"ID",yytext,"char*");}

"//" {
	int c;
	c=input();
	add_token("","COMMENT","LINE_COMMENT","enumerated");
	while(c!=0 && c!=-1){
		c=input();
		if(c=='\n' || c==-1 || c==0){
			break;
		}
	}
}

"/*" {
	int c, cnt=1;
	
	while((c=input())!= 0 && c!=-1) {
		if(c=='/'){
			if((c=input())=='*'){
			cnt++;
			}
			else{
				unput(c);
			}
		}
		else if(c=='*'){
			if((c=input())=='/'){
			cnt--;
			if(cnt>0){
				add_token("","NESTED_COMMENT","BLOCK_COMMENT","enumerated");	
			}else{
				add_token("","COMMENT","BLOCK_COMMENT","enumerated");
				break;
			}
			}else{
				unput(c);
			}
		}
	}

	if(cnt>0){
		print_token();
		printf("ERROR:Missing */\n");
		exit(0);
	}
}

\n { yylineno; }
[\n\t ' '] {};

%%
int main(int argc,char** argv){	
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
		
			fprintf(stderr,"Error%s\n",argv[1]);
			
			return 1;
		}
	}else{
		yyin=stdin;
	}	
	
	alpha_yylex(NULL);
	
	if(argc==2){
        print_token();
    }else{
		FILE *output;
        output = fopen (argv[2], "w");
		fprint_token(output);
    }
	
	return 0;
}
