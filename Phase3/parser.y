%{
        #include <stdio.h>
        //#include "lex.yy.h" // alphayylex?
        #include "Symbol_Table.h"
        #include "quads.h"

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);

        extern unsigned scopeSpaceCounter;
        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
        int CURR_SCOPE=0;
        int myfuctions[150];
        extern var_table* table ;
        var *curr = NULL;

        int curr_anonymous = 0; //keep track for anonymous
        int if_flag = 0;
        int for_flag = 0;
        int error_flag = 0;     // check if there were errors


        /*function for check ids and insert to STable*/
        var *insert_ID(char* name){
                //kanoume lookup apo mesa pros ta eksw
                var *myvar = lookup_in_out(CURR_SCOPE,name);
                var_id curr_id = local;
                //An den vrethei tpt kanthn insert sto Curr_scope
                if(myvar == NULL){
                        if(CURR_SCOPE == 0) // an einai global
                                curr_id = global;
                        myvar =new_var(varr,curr_id,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);                    
                } 
                /*an yparxei hdh */
                else{ 
                        //aneferomaste kai menei na doume an exoume prosvash
                        printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name); //TESTING PRINT
                        //check if we have access
                        if(check_access(myvar) == 0 && myvar->scope != 0){
                                yyerror("Cannot access var ");
                        }
                } // einai hdh sto table
                return myvar;
        }

        /*check if global variable exist p.x. ::x (global x)*/
        void check_global(char *name){
                //kanoume lookup sto global scope 0
                var *myvar = lookup_scope(0,name);
                //an einai NULL den iparxei
                if (myvar == NULL ) {
                        yyerror("Global var not found");
                        return;
                }
                //alliws anaferomaste ekei
                printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name);  //TESTING PRINT
        }

        /*function to insert formal variables*/
        void insert_formal(char* name){
                var *myvar = lookup_scope(CURR_SCOPE,name);
                /*an yparxei sto idio scope error p.x. fuction (x,y,x) */
                if(myvar != NULL){
                        yyerror("Already defined ");
                        return;
                }
                /*an exei collision me libfuction error */
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                /*alliws insert sto syble table san formal */
                myvar = new_var(varr,formal,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myvar,table);
                print_var(myvar);
        }

        /*insert a new fuction to the table*/
        void function_insert(char* name){
                //check if anonymous and insert if true
                if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myfuction,table);
                        print_var(myfuction); //na ftiaksw ta print
                        return;
                }
                //kanoume lookuop sto trexon scope
                var* myVar = lookup_scope(CURR_SCOPE,name);
                
                // an yparxei collision me lib_fuct einai error
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                //an vrethei metavlhth h synarthsh einai error
                if(myVar != NULL){ 
                        yyerror("Already defined");
                        return;
                }
                //alliws thn kanoume insert
                myVar = new_var(fuction,user_func,name,CURR_SCOPE,1,yylineno); 
                hash_insert(myVar,table);
                print_var(myVar);
        }

        /*Insert a local var with name = name */
        void insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
                        return;
                }
                /*tsekare an yparxoun collisions me lib fuction*/
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return;
                }
                /*an eimaste sto scope 0 tote thn kanoume insert san global*/
                if(CURR_SCOPE == 0)
                        curr_id = global;
                        //printf("GT %d\n",currVar->hide);
                currVar = new_var(varr,curr_id,name,CURR_SCOPE,1,yylineno); 
                hash_insert(currVar,table);
                print_var(currVar);          
        }

        /*check if the curr string is '_' to create the next anonymous fuction*/
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
        struct var *exprNode;
}

/*KEYWORDS*/
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE
AND NOT OR LOCAL TRUE FALSE NILL

/*OPERATORS*/   
%token ASSIGNMENT ADDITION SUBTRACTION MULTI
%token DIVISION  MODULUS EQUAL NOTEQUAL INCREMENT DECREMENT GRETER_THAN LESS_THAN   GRE_EQUAL   LES_EQUAL  


/*INTEGER NUMERIC*/
%token <intValue> INTEGER

/*REAL NUMERIC*/
%token <realValue> REAL

/*STRINGS*/
%token <stringValue> STRING

/*PUNCTUATIONS MARK*/
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_PARENTHESIS 
RIGHT_PARENTHESIS SEMICOLON COMMA SCOPE_RESOLUTION COLON FULL_STOP DOUBLE_FULL_STOP

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

%type <intValue> expr
%type <exprNode> lvalue 

%start program  /*specify the start symbol of the grammar*/



%%
program:   stmt_list 
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


expr:   assignexpr              {}
        |expr ADDITION expr     {$$ =  $1 + $3;}
        |expr SUBTRACTION expr  {$$ =  $1 - $3;}
        |expr MULTI expr        {$$ =  $1 * $3;}
        |expr DIVISION expr     {$$ =  $1 / $3;}
        |expr MODULUS expr      {$$ =  $1 % $3;}
        |expr GRETER_THAN expr  {$$ =  $1 > $3;}
        |expr GRE_EQUAL expr    {$$ =  $1 >= $3;}
        |expr LESS_THAN expr    {$$ =  $1 < $3;}
        |expr LES_EQUAL expr    {$$ =  $1 <= $3;}
        |expr EQUAL expr        {$$ =  $1 == $3;}
        |expr NOTEQUAL expr     {$$ =  $1 != $3;}
        |expr AND expr          {$$ =  $1 && $3;}
        |expr OR expr           {$$ =  $1 || $3;}
        |term                   {}
        ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
        |SUBTRACTION expr {check_if_fuction(yylval.exprNode);}
        |NOT expr         {check_if_fuction(yylval.exprNode);}
        |INCREMENT lvalue {check_if_fuction(yylval.exprNode);}
        |lvalue INCREMENT {check_if_fuction(yylval.exprNode);}
        |DECREMENT lvalue {check_if_fuction(yylval.exprNode);}
        |lvalue DECREMENT {check_if_fuction(yylval.exprNode);}
        |primary
        ;        

assignexpr: lvalue      {check_if_fuction(yylval.exprNode);}
        ASSIGNMENT      expr
        ;    

primary: lvalue
        |call
        |objectdef
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
        |const
        ;

lvalue:  ID                     { yylval.exprNode = insert_ID(yylval.stringValue);  }
        |LOCAL ID               { insert_local(yylval.stringValue);}
        |SCOPE_RESOLUTION ID    { check_global(yylval.stringValue);} //::
        |member                 {}
        ;             

member:  lvalue FULL_STOP ID
        |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET // []
        |call   FULL_STOP ID
        |call   LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
        ;

call:    call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS
        |lvalue callsuffix
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS
        ;


callsuffix: normcall
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
                CURR_SCOPE++;   
                }stmt_list RIGHT_CURLY_BRACKET{
                        if(CURR_SCOPE!=0)
                                hide(CURR_SCOPE--);       
                }
        ;

funcdef:
        FUNCTION ID{
                scopeSpaceCounter++;   // auksanoume to counter gia to ti var einai kata 1
                function_insert(yylval.stringValue);  // insert to fuction
                fuction_scope_insert(CURR_SCOPE++);   // gia na kratame to teleutaio scope
        }LEFT_PARENTHESIS 
                moreidilist {
                        scopeSpaceCounter++;
                }
                RIGHT_PARENTHESIS{
                        CURR_SCOPE--;
        } block {
                delete_last_fuction_scope();
                scopeSpaceCounter-=2;
        }

        |FUNCTION {
                //no name fuct
                function_insert("_");  //regognize anonymous fuctions
                fuction_scope_insert(CURR_SCOPE++);  
        }LEFT_PARENTHESIS moreidilist  RIGHT_PARENTHESIS {
                CURR_SCOPE--;
        }block{ delete_last_fuction_scope();} /*anonymous functions here */
        ;    

const:  INTEGER
        |REAL
        |STRING
        |NILL
        |TRUE
        |FALSE
        ;

idlist: ID {insert_formal(yylval.stringValue);}
        |COMMA ID{insert_formal(yylval.stringValue);}
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

returnstmt: RETURN {if(CURR_SCOPE == 0)yyerror("return w/o function");} expr SEMICOLON{}
        ;   
%%

int yyerror(char* yaccProvidedMessage){
                //provide error message example:
        fprintf(stderr,"%s: error at line %d , before token %s\n",yaccProvidedMessage,yylineno,yytext);
        error_flag = 1;
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
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
    print_format();
    return 0;
}
