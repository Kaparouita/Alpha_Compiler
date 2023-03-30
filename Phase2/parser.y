%{
        #include <stdio.h>
        //#include "lex.yy.h" // alphayylex?
        #include "Symbol_Table.h"
       

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);

        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
        int CURR_SCOPE=0;
        int PREV_SCOPE=0;
        int i;
        int myfuctions[150];
        extern var_table* table ;
        var* myvar; // global variable to insert to STable
        var_id Vtype; // for switch the type of the ids
        int curr_anonymous = 0; //keep track for anonymous


        /*function for check ids and insert to STable*/
        void Id_check(char* name,var_type var_type,var_id type){
                int retValue = lookup_globaly(table,name);
                if(retValue == 1){ 
                        //check if we have access
                        if(check_access(name) == 1){
                                yyerror("No access cause var is in another scope");
                        }
                        printf("To var %s einai hdh sto table line %d\n",name,yylineno);
                        return;
                } // einai hdh sto table
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                }
                if(CURR_SCOPE == 0){
                        //an einai mhden panta global
                        myvar =new_var(varr,global,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);
                }
                else{   //alliws local
                        myvar =new_var(varr,type,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);   
                }
        }

        int check_access(char *name){
                var* retVar = lookup_var(table,name);
                if(retVar == NULL)
                        return 1;
                if(retVar->scope != CURR_SCOPE && retVar->scope != 0)
                        return 1;
                return 0;
        }

        void check_global(char *name){
                if (lookup_scope(0,name) == 0 ) //::(global) ids
                        yyerror("ID not found");
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
        }

       /*function for check ids and insert to STable*/
        void formal_check(char* name,var_id type){
                if (lookup_globaly(table,name)==0 ){ // insert ids formal 
                        if(check_collisions(name) == 1){
                                yyerror("This is a lib_fuct");
                        }
                        myvar =new_var(varr,type,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);
                }else 
                        yyerror("The id already exist");
        }

        void function_insert(char* name){
                int retValue = lookup_globaly(table,name);
                if(retValue == 1){ 
                        printf("To fuction %s einai hdh sto table",name);
                        return;
                } // einai hdh sto table
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                }
                //check if anonymous
                if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                }
                var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myfuction,table);
                print_var(myfuction);
        }

        void insert_local(char* name){
                int retValue = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                if(retValue == 1){ 
                        printf("To var %s einai hdh sto table",name);
                        return;
                } // einai hdh sto table
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                }
                if(CURR_SCOPE == 0)
                        curr_id = global;
                var *myvar =new_var(varr,curr_id,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myvar,table);
                print_var(myvar);          
        }

        char *check_anonymous(char *name){
                if(strcmp(name,"_") == 0){
                        char* str = malloc(sizeof(char) * 30);
		        sprintf(str,"_f%d",curr_anonymous++);
		        return str;
                }
                return NULL;
        }
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


/*proteraiothtes*/
%right ASSIGNMENT         
%left OR                
%left AND               
%nonassoc EQUAL NOTEQUAL 
%nonassoc GRETER_THAN GRE_EQUAL LESS_THAN LES_EQUAL 
%left ADDITION SUBTRACTION								
%left MULTI DIVISION MODULUS 							
%right NOT INCREMENT DECREMENT  		
%left FULL_STOP DOUBLE_FULL_STOP                       		
%left LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET       	
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS		



%start program  /*specify the start symbol of the grammar*/



%%
program:    stmt_list 
            ;

stmt_list: stmt_list  stmt
           |
           ;


brk_stm:BREAK SEMICOLON;
cnt_stm:CONTINUE SEMICOLON ;
 
stmt:   expr SEMICOLON
        |ifstmt
        |whilestmt
        |forstmt
        |returnstmt
        |brk_stm
        |cnt_stm
        |block
        |funcdef
        |SEMICOLON
        ;


expr:   assignexpr
        |expr ADDITION expr
        |expr SUBTRACTION expr
        |expr MULTI expr
        |expr DIVISION expr
        |expr MODULUS expr
        |expr GRETER_THAN expr
        |expr GRE_EQUAL expr
        |expr LESS_THAN expr
        |expr LES_EQUAL expr
        |expr EQUAL expr
        |expr NOTEQUAL expr
        |expr AND expr
        |expr OR expr
        |term
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

lvalue:     ID {if (CURR_SCOPE == 0 )
	                Id_check(yylval.stringValue,varr,global);
                 else 
	                Id_check(yylval.stringValue,varr,local);     
                 }
            |LOCAL ID { insert_local(yylval.stringValue);}
            |SCOPE_RESOLUTION ID { check_global(yylval.stringValue);} //::
            |member
            ;             
            
member:     lvalue FULL_STOP ID
            |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET // []
            |call FULL_STOP ID
            |call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
            ;

call:       call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS
            |lvalue callsuffix
            |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS
            ;


callsuffix:     normcall
                |methodcall
                ;

normcall:   LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS 
            ;

methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS 
            ;    

elist:  expr
        ;

moreElist: elist        
           |moreElist COMMA elist
           |
           ;     

objectdef: LEFT_SQUARE_BRACKET  moreElist RIGHT_SQUARE_BRACKET
           |LEFT_SQUARE_BRACKET   indexed  RIGHT_SQUARE_BRACKET
           ;     

indexed: moreindexedelem
           ; 

/*1 or + times for indexedelems*/
moreindexedelem:   indexedelem     
                   |moreindexedelem COMMA indexedelem
                   ;

indexedelem: LEFT_CURLY_BRACKET expr COLON expr RIGHT_CURLY_BRACKET 
             ;


block:  LEFT_CURLY_BRACKET {
                PREV_SCOPE = CURR_SCOPE; 
                CURR_SCOPE++;   
                }stmt_list RIGHT_CURLY_BRACKET{
                        hide(CURR_SCOPE--); PREV_SCOPE--;
                }
        ;

funcdef:
        FUNCTION ID{
                function_insert(yylval.stringValue);
                PREV_SCOPE=CURR_SCOPE;
                CURR_SCOPE++;
        }LEFT_PARENTHESIS moreidilist RIGHT_PARENTHESIS{
                if(PREV_SCOPE!=0){
                        PREV_SCOPE--;
                };
                CURR_SCOPE--;
        } block
        |FUNCTION {
                function_insert("_");
                PREV_SCOPE=CURR_SCOPE;
                CURR_SCOPE++;
        }LEFT_PARENTHESIS moreidilist  RIGHT_PARENTHESIS {
                if(PREV_SCOPE!=0){
                        PREV_SCOPE--;
                };
                CURR_SCOPE--;
        }block   /*anonymous functions here */
        ;    

const:  INTEGER
        |REAL
        |STRING
        |NILL
        |TRUE
        |FALSE
        ;

idlist: ID {formal_check(yylval.stringValue,formal);}
        |COMMA ID{formal_check(yylval.stringValue,formal);}
        ;

moreidilist: moreidilist idlist
             |
             ;


ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
        |ELSE stmt
        ;

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
           ;

/*note this for elist*/
forstmt:   FOR LEFT_PARENTHESIS moreElist SEMICOLON expr SEMICOLON moreElist RIGHT_PARENTHESIS stmt
           ;

returnstmt: RETURN expr SEMICOLON
            ;   
%%

int yyerror(char* yaccProvidedMessage){
                //provide error message example:
        fprintf(stderr,"%s: error at line %d , before token %s\n",yaccProvidedMessage,yylineno,yytext);
        //fprintf(stderr,"INVALID INPUT\n");
}





/*-----------------------------MAIN-----------------------*/
int main(int argc, char** argv){
        if(argc>1){
                if(!(yyin=fopen(argv[1],"r"))){
                        fprintf(stderr,"Error%s\n",argv[1]);
                        return 1;
                }
        }
	else yyin=stdin;
	 
    
    
    init_lib_func();
    print_scope(0);
    check_collisions("hi");
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    //print_scope(CURR_SCOPE);
    return 0;
}
