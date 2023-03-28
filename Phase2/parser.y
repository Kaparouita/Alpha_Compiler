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
        extern var_table* table ;
        var* myvar; // global variable to insert to STable
        var_id Vtype; // for switch the type of the ids


        /*function for check ids and insert to STable*/
        void Id_check(char* name,var_id type){
                int retValue=lookup_globaly(table,name);
                if (retValue==0 && type!=local ){ // insert ids globaly 
                        myvar =new_var(varr,type,name,CURR_SCOPE,1,yylineno); 
                        hash_insert(myvar,table);
                        print_var(myvar);
                }else if (retValue==0 && type==local){  // insert ids in scope 0 but they have local keyword so we switch the type
                        retValue=lookup_scope(CURR_SCOPE ,name);
                        if(retValue==1) //if we found the id as global var
                                exit(0);
                        else if(CURR_SCOPE==0 && retValue==0){
                                Vtype= switch_enum(type);
                                myvar =new_var(varr,Vtype,name,CURR_SCOPE,1,yylineno); 
                                hash_insert(myvar,table);
                                print_var(myvar);
                        }else if (retValue==0){
                                // NOTE THIS den mporw na apofasisw ti ennow se front3 slide19 me ta collsioin se liub func 
                                myvar =new_var(varr,type,name,CURR_SCOPE,1,yylineno); 
                                hash_insert(myvar,table);
                                print_var(myvar);
                        }     

                }

                if (lookup_globaly(table,name)==1) //::(global) ids
                        exit(0);
                else 
                        yyerror("ID not found");
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

lvalue:     ID {if (CURR_SCOPE ==0 )
	                Id_check(yylval.stringValue,global);
                 else {
                        Vtype=switch_enum(global);
	                Id_check(yylval.stringValue,Vtype);
                        }       
                 }
            |LOCAL ID {Id_check(yylval.stringValue,local);}
            |SCOPE_RESOLUTION ID { Id_check(yylval.stringValue,global);}
            |member
            ;             
            
member:     lvalue FULL_STOP ID
            |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
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


block:  LEFT_CURLY_BRACKET {CURR_SCOPE++;int i=max_scope(0,1); printf("the max is %d",i);} stmt_list RIGHT_CURLY_BRACKET{ CURR_SCOPE--;}
        ;

funcdef:    FUNCTION ID LEFT_PARENTHESIS moreidilist  RIGHT_PARENTHESIS block
            |FUNCTION  LEFT_PARENTHESIS moreidilist  RIGHT_PARENTHESIS block   /*anonymous functions here */
            ;    

const:  INTEGER
        |REAL
        |STRING
        |NILL
        |TRUE
        |FALSE
        ;

idlist: ID
        |COMMA ID
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
        fprintf(stderr,"INVALID INPUT\n");
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
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    print_scope(CURR_SCOPE);
    return 0;
}
