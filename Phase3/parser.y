%{
        #include <stdio.h>
        //#include "lex.yy.h" // alphayylex?
        #include "Symbol_Table.h"
        #include "quads.h"
        #include "Stack.h"

        int yyerror (char* yaccProvidedMessage);
        int yylex (void);

        extern unsigned scopeSpaceCounter;
        extern int yylineno;
        extern char* yytext;
        extern FILE* yyin;
        extern int CURR_SCOPE;
        extern unsigned functionLocalOffset;
        extern var_table* table ;
        extern int currQuad;
        var *curr = NULL;
        struct Stack * save_fuctionlocals;

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
                        myvar =new_var(varr,curr_id,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);
                        inccurrscopeoffset();              
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
                myvar = new_var(varr,formal,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                inccurrscopeoffset();
                hash_insert(myvar,table);
                print_var(myvar);
        }

        /*insert a new fuction to the table*/
        void function_insert(char* name){
                //check if anonymous and insert if true
                if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
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
                myVar = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                hash_insert(myVar,table);
                print_var(myVar);
        }

        /*Insert a local var with name = name */
        var* insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
                        if(currVar->type == fuction)
                                printf("Warning line 124 parser.y");
                        return currVar;
                }
                /*tsekare an yparxoun collisions me lib fuction*/
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return currVar;
                }
                /*an eimaste sto scope 0 tote thn kanoume insert san global*/
                if(CURR_SCOPE == 0)
                        curr_id = global;
                        //printf("GT %d\n",currVar->hide);
                currVar = new_var(varr,curr_id,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                hash_insert(currVar,table);
                print_var(currVar);
                //inccurrscopeoffset();
                return currVar;         
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
        struct expr *exprValue;
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

//%type <intValue>  expr
%type <exprValue> lvalue expr member call elist assignexpr const term primary objectdef moreElist

%start program  /*specify the start symbol of the grammar*/



%%
program:   stmt_list 
        ;

stmt_list: stmt_list  stmt
        |
        ;


brk_stm:BREAK {if(for_flag == 0)yyerror("break w/o loop");} SEMICOLON ;
cnt_stm:CONTINUE {if(for_flag == 0)yyerror("continue w/o loop");} SEMICOLON ;

stmt: {resettemp();}  expr SEMICOLON
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
        |expr ADDITION expr     {$$ =  do_maths($1,$3,add);}
        |expr SUBTRACTION expr  {$$ =  do_maths($1,$3,sub);}
        |expr MULTI expr        {$$ =  do_maths($1,$3,mul);}
        |expr DIVISION expr     {$$ =  do_maths($1,$3,n_div);}
        |expr MODULUS expr      {$$ =  do_maths($1,$3,mod);}
        |expr GRETER_THAN expr  {$$ =  do_maths($1,$3,if_greater);}
        |expr GRE_EQUAL expr    {$$ =  do_maths($1,$3,if_greater);}
        |expr LESS_THAN expr    {$$ =  do_maths($1,$3,if_greater);}
        |expr LES_EQUAL expr    {$$ =  do_maths($1,$3,if_greater);}
        |expr EQUAL expr        {$$ =  do_bool($1,$3,if_greater);}
        |expr NOTEQUAL expr     {$$ =  do_bool($1,$3,if_greater);}
        |expr AND expr          {$$ =  do_bool($1,$3,if_greater);}
        |expr OR expr           {$$ =  do_bool($1,$3,if_greater);}
        |term                   {}
        ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {$$ = $2;}
        |SUBTRACTION expr {check_arith($2); $term = newexpr(arithexpr_e);$term->sym = newtemp(); emit(uminus,$expr,NULL,$term,currQuad,yylineno);} //-a
        |NOT expr         {check_arith($2);} //not a
        |INCREMENT lvalue {check_arith($2);} //++a
        |lvalue INCREMENT {check_arith($1);} //a++
        |DECREMENT lvalue {check_arith($2);} //--a
        |lvalue DECREMENT {check_arith($1);} //a--
        |primary          {$$ = $1;}
        ;        

assignexpr: lvalue ASSIGNMENT expr{
				{
					check_if_fuction($1); //check gia to table
                                        //lvalue[i] = expr
					if($1->type == tableitem_e) {
						emit(tablesetelem, $1, $1->index, $3, 0, yylineno);
						$assignexpr = emit_iftableitem($1);
						$assignexpr->type = assignexpr_e;
					} 
                                        // lvalue = expr
                                        else {
						emit(assign, $1, $3, NULL, currQuad, yylineno);
						$assignexpr = newexpr(assignexpr_e);
						$assignexpr->sym = newtemp();
						emit(assign, $assignexpr, $1, NULL, currQuad, yylineno);
                                        }
				}               
        }
        ;    

primary: lvalue         {$$ = $1;}
        |call           {$$ = $1;}
        |objectdef      {$$ = $1;}
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{}
        |const          {$$ = $1;}
        ;

lvalue: member                  {  $lvalue = $1;} 
        |ID                     {  $lvalue = lvalue_expr(insert_ID(yylval.stringValue)); }
        |LOCAL ID               {  $lvalue = lvalue_expr(insert_local(yylval.stringValue));   }
        |SCOPE_RESOLUTION ID    { check_global(yylval.stringValue);
                                } //::
        ;             

member:  lvalue FULL_STOP ID
        |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET // []
        |call   FULL_STOP ID
        |call   LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
        ;

call:    call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS
        |lvalue callsuffix
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS{}
        ;

callsuffix: normcall
        |methodcall
        ;

normcall:   LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS 
        ;

methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS 
        ;    

elist:  expr   //elist polla expr
        ;

moreElist: elist        
        |moreElist COMMA elist
        |  {$$ = $$;}//?
        ;     

objectdef:  
        LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
        LEFT_SQUARE_BRACKET  moreElist  {  // []
        //emit(tablecreate,newexpr(tableitem_e),NULL,NULL,currQuad,yylineno);
        } RIGHT_SQUARE_BRACKET   {}
        |LEFT_SQUARE_BRACKET   indexed  RIGHT_SQUARE_BRACKET {}
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
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert(yylval.stringValue);  // insert to fuction
                fuction_scope_insert(CURR_SCOPE++);   // gia na kratame to teleutaio scope
        }LEFT_PARENTHESIS 
                moreidilist {enterscopespace(); }
                RIGHT_PARENTHESIS{
                        CURR_SCOPE--;
        } block {
                delete_last_fuction_scope();
                exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);
        }
        |FUNCTION {
                //no name fuct
                push(save_fuctionlocals,functionLocalOffset);
                enterscopespace();   // auksanoume to counter gia to ti var einai kata 1
                function_insert("_");  //regognize anonymous fuctions
                fuction_scope_insert(CURR_SCOPE++);  
        }LEFT_PARENTHESIS moreidilist{enterscopespace(); }  RIGHT_PARENTHESIS {
                CURR_SCOPE--;
        }block{ delete_last_fuction_scope(); exitscopespace();
                functionLocalOffset = pop(save_fuctionlocals);} /*anonymous functions here */
        ;    

const:  INTEGER { $$ = newexpr_constnum($INTEGER);}
        |REAL   { $$ = newexpr_constdouble($REAL);}
        |STRING { $$ = newexpr_conststring($STRING);}
        |NILL   { $$ = newexpr_nil();}
        |TRUE   { $$ = newexpr_constbool(1);}
        |FALSE  { $$ = newexpr_constbool(0);}
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
	 
    
    save_fuctionlocals = createStack(150); 
    init_lib_func();
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
   // print_format(); //print scopes
   print_all_quads(); //print quads
    return 0;
}
