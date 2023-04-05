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
        int access_scope = 0;
        int i;
        int myfuctions[150];
        extern var_table* table ;
        var* myvar; // global variable to insert to STable
        var_id Vtype; // for switch the type of the ids
        int curr_anonymous = 0; //keep track for anonymous
        int if_flag = 0;
        int for_flag = 0;
        int fuction_flag = 0;
        int return_flag = 0;


        /*function for check ids and insert to STable*/
        void Id_check(char* name,var_type var_type,var_id type){
                int retValue = lookup_globaly(table,name);
                if(retValue == 1){ 
                        //check if we have access
                        if(check_access(name) == 1){
                                yyerror("Cannot access var");
                        }
                        if(check_name(name) == 0){
                                printf("Cannot access %s\n",name);}
                        return;
                } // einai hdh sto table
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
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

        int check_name(char *name){
                return 1;
        }

        int check_access(char *name){
                var* retVar = lookup_var(table,name);
                if(retVar == NULL)
                        return 1;
                if(retVar->id == 2){
                        if(retVar->scope != CURR_SCOPE)
                                return 1;
                }
                else if(retVar->scope < access_scope && retVar->scope != 0)
                        return 1;
                return 0;
        }

        void check_global(char *name){
                if (lookup_global(name) == 1 ) //::(global) ids
                        {yyerror("ID not found");
                        return;}
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
        }

        /*function for check ids and insert to STable*/
        void formal_check(char* name,var_id type){
                var *var1 = lookup_var(table,name);
                if(var1!=NULL)
                {if((var1->scope == CURR_SCOPE ) && (var1->id == formal ))
                        { yyerror("Already defined");
                        return;}}
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                myvar =new_var(varr,type,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myvar,table);
                print_var(myvar);
        }

        void function_insert(char* name){
                int retValue = lookup_scope(CURR_SCOPE,name);
                if(retValue == 1){ 
                        printf("Cannot access %s\n",name);
                        return;
                } // einai hdh sto table
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
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
                var* retValue = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                if(retValue != NULL){  // einai hdh sto table
                        printf("To var %s einai hdh sto table",name);
                        return;
                }
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
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


brk_stm:BREAK {if(for_flag == 0)yyerror("break w/o loop");} SEMICOLON ;
cnt_stm:CONTINUE {if(for_flag == 0)yyerror("continue w/o loop");} SEMICOLON ;
 
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

lvalue:     ID {
                 if(return_flag == 1){
                        if(lookup_globaly(table,yylval.stringValue) == 0)
                                yyerror("No var\n");
                 }
                 else if (CURR_SCOPE == 0 )
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
                        if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);
                        PREV_SCOPE--;
                }
        ;

funcdef:
        FUNCTION ID{
                access_scope++;
                fuction_flag++;
                function_insert(yylval.stringValue);
                PREV_SCOPE=CURR_SCOPE;
                CURR_SCOPE++;
        }LEFT_PARENTHESIS moreidilist RIGHT_PARENTHESIS{
                if(PREV_SCOPE!=0){
                        PREV_SCOPE--;
                };
                CURR_SCOPE--;
        } block{fuction_flag--;}

        |FUNCTION {
                access_scope++;
                fuction_flag++;
                function_insert("_");
                PREV_SCOPE=CURR_SCOPE;
                CURR_SCOPE++;
        }LEFT_PARENTHESIS moreidilist  RIGHT_PARENTHESIS {
                if(PREV_SCOPE!=0){
                        PREV_SCOPE--;
                };
                CURR_SCOPE--;
        }block{fuction_flag--;access_scope--;} /*anonymous functions here */
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


ifstmt: IF LEFT_PARENTHESIS{if_flag = 1;} expr RIGHT_PARENTHESIS  stmt { if_flag = 0;}
        |ELSE stmt
        ;

whilestmt: WHILE LEFT_PARENTHESIS{for_flag = 1;} expr RIGHT_PARENTHESIS stmt {for_flag = 0;} 
           ;

/*note this for elist*/
forstmt:   FOR LEFT_PARENTHESIS{for_flag = 1;} moreElist SEMICOLON expr SEMICOLON moreElist RIGHT_PARENTHESIS stmt {for_flag = 0;} 
           ;

returnstmt: RETURN {if(fuction_flag == 0)yyerror("return w/o fuction");return_flag = 1;} expr SEMICOLON{return_flag = 0;}
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
