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
        struct Stack * loopcounterStack;

        int curr_anonymous = 0; //keep track for anonymous
        int loopcounter =0;
        int error_flag = 0;     // check if there were errors
        int fuctioncounter = 0;

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
                        //print_var(myvar);
                        inccurrscopeoffset();              
                } 
                /*an yparxei hdh */
                else{ 
                        //aneferomaste kai menei na doume an exoume prosvash
                        //printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name); //TESTING PRINT
                        //check if we have access
                        if(check_access(myvar) == 0 && myvar->scope != 0){
                                yyerror("Cannot access var ");
                        }   
                } // einai hdh sto table
                return myvar;
        }

        /*check if global variable exist p.x. ::x (global x)*/
        var * check_global(char *name){
                //kanoume lookup sto global scope 0
                var *myvar = lookup_scope(0,name);
                //an einai NULL den iparxei
                if (myvar == NULL ) {
                        yyerror("Global var not found");
                        return NULL;
                }
                //alliws anaferomaste ekei
                //printf("Anafora sto %s : %s \n",enum_type(myvar->type) ,myvar->name);  //TESTING PRINT
                return myvar;
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
                //print_var(myvar);
        }

        /*insert a new fuction to the table*/
        var* function_insert(char* name){
                //check if anonymous and insert if true
                /*if(check_anonymous(name) != NULL){
                        curr_anonymous--;
                        name = check_anonymous(name);
                        var *myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                        hash_insert(myfuction,table);
                        //print_var(myfuction); //na ftiaksw ta print
                        return myfuction;
                }*/
                //kanoume lookuop sto trexon scope
                var* myfuction = lookup_scope(CURR_SCOPE,name);
                
                // an yparxei collision me lib_fuct einai error
                if(check_collisions(name) == 1){
                        yyerror("This is a lib_fuct");
                        return myfuction;
                }
                //an vrethei metavlhth h synarthsh einai error
                if(myfuction != NULL){ 
                        yyerror("Already defined");
                        return myfuction;
                }
                //alliws thn kanoume insert
                myfuction = new_var(fuction,user_func,name,CURR_SCOPE,currscopespace(),currscopeoffset(),1,yylineno); 
                hash_insert(myfuction,table);
                return myfuction;
                //print_var(myfuction);
        }

        /*Insert a local var with name = name */
        var* insert_local(char* name){
                /*koita sto trexon scope*/
                var* currVar = lookup_scope(CURR_SCOPE,name);
                var_id curr_id= local;
                /*an vrethei metavlhth sto table aneferomaste ekei*/
                if(currVar != NULL){  
                        //printf("Anafora sto %s : %s \n",enum_type(currVar->type) ,currVar->name);  //TESTING PRINT
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
                //print_var(currVar);
                inccurrscopeoffset();
                return currVar;         
        }

        /*check if the curr string is '_' to create the next anonymous fuction*/
        char *newemptyfuncname(){
                char* str = malloc(sizeof(char) * 30);
		sprintf(str,"_f%d",curr_anonymous++);
		return str;
        }
%}


/* Bison declarations and definitions */
%union {
        char*   stringValue;
        int     intValue;
        unsigned int UintValue;
        double  realValue;
        char    boolValue;
        struct  stmt_t *stmtValue;
        struct  var *symbolEntry;
        struct  for_prefix *forValue;
        struct  expr *exprValue;
        struct  indexed *indexedValue;
        struct  call_s *callValue;
}

/*KEYWORDS*/
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE
AND NOT OR LOCAL NILL 

/*OPERATORS*/   
%token ASSIGNMENT ADDITION SUBTRACTION MULTI
%token DIVISION  MODULUS UMINUS EQUAL NOTEQUAL INCREMENT DECREMENT GRETER_THAN LESS_THAN   GRE_EQUAL   LES_EQUAL  

/*INTEGER NUMERIC && BOOL*/
%token <intValue> INTEGER 
/*REAL NUMERIC*/
%token <realValue> REAL
/*STRINGS*/
%token <stringValue> STRING ID

%token <boolValue> TRUE FALSE


/*PUNCTUATIONS MARK*/
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_PARENTHESIS 
RIGHT_PARENTHESIS SEMICOLON COMMA SCOPE_RESOLUTION COLON FULL_STOP DOUBLE_FULL_STOP


/*NOT DEFINED SYMBOLS*/
%token EXTRA_CHARS

/*COMMENTS*/
%token LINE_COMMENT  
%token BLOCK_COMMENT 



/*proteraiothtes*/
%right ASSIGNMENT         
%left OR                
%left AND   
%right IF   
%left ELSE        
%nonassoc EQUAL NOTEQUAL 
%nonassoc GRETER_THAN GRE_EQUAL LESS_THAN LES_EQUAL 
%left ADDITION SUBTRACTION								
%left MULTI DIVISION MODULUS 							
%right UMINUS NOT INCREMENT DECREMENT  		
%left FULL_STOP DOUBLE_FULL_STOP                       		
%left LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET       	
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS		


%type <exprValue> lvalue expr member  elist assignexpr const term primary objectdef moreElist call 
%type <indexedValue> indexed moreindexedelem indexedelem                                      
%type <callValue> methodcall callsuffix normcall

%type <symbolEntry> funcprefix funcdef
%type <intValue> funcbody ifprefix elseprefix whilestart whilecond N M returnstart
%type <stringValue> funcname
%type <stmtValue> stmt ifstmt whilestmt forstmt returnstmt brk_stm cnt_stm block stmts loopstmt
%type <forValue> forprefix
%start program  /*specify the start symbol of the grammar*/



%%
program:   stmts
        ;

stmts: stmts  stmt {
                $$->breaklist = mergelist($1->breaklist, $stmt->breaklist);
                $$->contlist = mergelist($1->contlist, $stmt->contlist);
                        }
        |stmt {
                $$ = ($1) ? $1 : stmt_constractor(0,0);
        }
        ;


brk_stm:BREAK SEMICOLON {       $brk_stm = stmt_constractor(0,0);
                                $brk_stm->breaklist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        };
cnt_stm:CONTINUE SEMICOLON {    $cnt_stm = stmt_constractor(0,0);
                                $cnt_stm->contlist = newlist(nextquadlabel()); 
                                emit(jump,NULL,NULL,NULL,0,yylineno); 
                        };

stmt:   expr SEMICOLON {$$ = stmt_constractor(0,0); resettemp();}
        |ifstmt      {$$ = $1;}   
        |whilestmt   {$$ = $1;} 
        |forstmt     {$$ = $1;} 
        |returnstmt  {$$ = $1;} 
        |brk_stm     {$$ = $1;} 
        |cnt_stm     {$$ = $1;} 
        |block       {$$ = $1;} 
        |funcdef     {$$ = stmt_constractor(0,0);} 
        |SEMICOLON {resettemp(); $$ = stmt_constractor(0,0);}
        ;


expr:   assignexpr              {$$ =  $assignexpr;}
        |expr ADDITION expr     {$$ =  arithop($1,$3,add);}//arithop emits
        |expr SUBTRACTION expr  {$$ =  arithop($1,$3,sub);}
        |expr MULTI expr        {$$ =  arithop($1,$3,mul);}
        |expr DIVISION expr     {$$ =  arithop($1,$3,n_div);}
        |expr MODULUS expr      {$$ =  arithop($1,$3,mod);}
        |expr GRETER_THAN expr  {$$ =  relop($1,$3,if_greater); emit_relop($$,if_greater);}
        |expr GRE_EQUAL expr    {$$ =  relop($1,$3,if_geatereq); emit_relop($$, if_geatereq);}
        |expr LESS_THAN expr    {$$ =  relop($1,$3,if_less);    emit_relop($$,if_less);}
        |expr LES_EQUAL expr    {$$ =  relop($1,$3,if_lesseq);  emit_relop($$,if_lesseq);}
        |expr EQUAL expr        {$$ =  relop($1,$3,if_eq);      emit_relop($$,if_eq);}
        |expr NOTEQUAL expr     {$$ =  relop($1,$3,if_noteq);   emit_relop($$,if_noteq);}
        |expr AND expr          {       $$ =   newexpr_constbool(boolo($1,$3,and)) ; 
                                        $$->sym = newtemp();     
                                        emit(and,$1,$3,$$,0,yylineno);
                                }
        |expr OR expr           {       $$ = newexpr_constbool(boolo($1,$3,or)); 
                                        $$->sym = newtemp();       
                                        emit(or,$1,$3,$$,0,yylineno); // epishs auto htan nextquad +2
                                        /*emit(jump,NULL,NULL,NULL,nextquadlabel()+3,yylineno);
                                        emit_relop($$,or); //htan to idio kai sthn and/not nmzw apla ta vgazoume k gg */
                                }
        |term                   {$$ = $1;}
        ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {$$ = $2;}
        |SUBTRACTION expr %prec UMINUS       { //%prec: priority of uminus operator 
                check_arith($expr);
                $term = newexpr(arithexpr_e);
                $term->sym = istempexpr($expr) ? $expr->sym : newtemp();
                emit(uminus,$expr,NULL,$term,0,yylineno);
        }
        |NOT expr               {//not a
                $$ = newexpr(constbool_e);
                $term->sym = newtemp();
                $term->boolConst = !(check_if_bool($2));
                emit(not,NULL,$2,$$,0,yylineno);
        }
        |INCREMENT lvalue       { //++lvalue
                check_arith($lvalue);
                if($lvalue->type == tableitem_e){
                        $term = emit_iftableitem($lvalue);
                        emit(add, $$, newexpr_constnum(1), $$, 0, yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, $$, 0, yylineno);
                }else{
                        emit(add, $lvalue, newexpr_constnum(1), $lvalue, 0, yylineno);
                        $term = newexpr(arithexpr_e);
                        $term->sym = newtemp();
                        emit(assign, $lvalue, NULL, $$, 0, yylineno);
                }                                     //edw exw balei ayta xwris na kserw an einai
        }
        |lvalue INCREMENT       {    //lvalue++
                check_arith($lvalue);
                $term = newexpr(var_e);
                $term->sym = newtemp();
                if($lvalue->type == tableitem_e){
                        expr* val = emit_iftableitem($1);
                        emit(assign, val, NULL, $$, 0, yylineno);
                        emit(add, val, newexpr_constnum(1), val,0,yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, val,0,yylineno);
                }else{
                        emit(assign, $lvalue, NULL, $$,0,yylineno);
                        emit(add, $lvalue, newexpr_constnum(1), $lvalue,0,yylineno);
                }
        }
        |DECREMENT lvalue       { //--lvalue
                check_arith($lvalue);
                if($lvalue->type == tableitem_e){
                        $$ = emit_iftableitem($lvalue);
                        emit(sub, $$, newexpr_constnum(1), $$, 0, yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, $$, 0, yylineno);
                }else{
                        emit(sub, $lvalue, newexpr_constnum(1), $lvalue, 0, yylineno);
                        $term = newexpr(arithexpr_e);
                        $term->sym = newtemp();
                        emit(assign, $lvalue, NULL, $$, 0, yylineno);
                } 
        }
        |lvalue DECREMENT       { //lvalue--
                check_arith($lvalue);
                $term = newexpr(var_e);
                $term->sym = newtemp();
                if($lvalue->type == tableitem_e){
                        expr* val = emit_iftableitem($1);
                        emit(assign, val, NULL, $$, 0, yylineno);
                        emit(sub, val, newexpr_constnum(1), val,0,yylineno);
                        emit(tablesetelem, $lvalue, $lvalue->index, val,0,yylineno);
                }else{
                        emit(assign, $lvalue, NULL, $$,0,yylineno);
                        emit(sub, $lvalue, newexpr_constnum(1), $lvalue,0,yylineno);
                }
        }
        |primary                {$$ = $1;}
        ;        

assignexpr: lvalue ASSIGNMENT expr{
				{
					//check_if_fuction($1); //check gia to table
                                        //lvalue[i] = expr
					if($1->type == tableitem_e) {
						emit(tablesetelem, $1, $1->index, $3, 0, yylineno);
						$assignexpr = emit_iftableitem($1);
						$assignexpr->type = assignexpr_e;
					} 
                                        // lvalue = expr
                                        else {
						emit(assign, $3, NULL,$1, 0, yylineno); //paizei na thelei ta arg anapoda
						$assignexpr = newexpr(assignexpr_e);
						$assignexpr->sym = newtemp();
                                                $1->type = $3->type;
                                               // copy_value($1,$3);
						emit(assign ,$1, NULL, $assignexpr,0, yylineno); //omoiws me to apo panw
                                                //copy_value($assignexpr,$1);
                                        }
				}               
        }
        ;    

primary: lvalue                 {$$ = emit_iftableitem($1);} 
        |call                   {$$ = $1;}
        |objectdef              {$$ = $1;}
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{
                $primary = newexpr(programfunc_e);
                $primary->sym = $funcdef; 
        }
        |const                  {$$ = $1;}
        ;

lvalue: member                  {  $lvalue = $member;} 
        |ID                     {  $lvalue = lvalue_expr(insert_ID(yylval.stringValue));}
        |LOCAL ID               {  $lvalue = lvalue_expr(insert_local(yylval.stringValue));}
        |SCOPE_RESOLUTION ID    {  $lvalue = lvalue_expr(check_global(yylval.stringValue));} //::
        ;             

member:  lvalue FULL_STOP ID    {// a.x;
                                $member = member_item($lvalue,yylval.stringValue);}
        |lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET { // a[3]
                                $lvalue = emit_iftableitem($lvalue);
                                $member = newexpr(tableitem_e);
                                $member->sym = $lvalue->sym;
                                $member->index = $expr; //index of expr
        }
        //FUCTIONS calls
        |call   FULL_STOP ID  //a..fuction;
        |call   LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
        ;

call:   call LEFT_PARENTHESIS RIGHT_PARENTHESIS
                                {$$ = make_call($1,NULL);
                                }       /* !den kserw an xreiazete*/
        |call LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS       //call (elist)
                                {$$ = make_call($1, $moreElist);}
        |lvalue callsuffix
                                {
                                $lvalue = emit_iftableitem($lvalue);            //se periptwsh p htan table item
                                if($callsuffix->method){
                                        expr* t = $lvalue;
                                        $lvalue = emit_iftableitem(member_item(t, $callsuffix->name));
                                        if($callsuffix)//!EDW isws prepei na to treksw mexri to telos k meta na to kanw insert?
                                                $callsuffix->elist->next = t;           // insert san prwto arg (reversed,so last)
                                        else
                                                $callsuffix->elist = t;
                                }
                                $call = make_call($lvalue, $callsuffix->elist);
        }
                /*AN TO ELIST = NULL */
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS  RIGHT_PARENTHESIS{
                                expr* func = newexpr(programfunc_e);
				func->sym = $2;
				$$ = make_call(func, NULL);
        }
        |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS // (funcdef) (elist)
                                {
                                expr* func = newexpr(programfunc_e);
                                func->sym = $funcdef;
                                $call = make_call(func, $moreElist);                
        }
        ;

callsuffix: normcall            {$callsuffix = $normcall; }
         |methodcall            {$callsuffix = $methodcall;}
        ;

normcall:   LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS  {
                                $normcall = call_constractor($moreElist,0,NULL);
        }
        /*AN TO ELIST = NULL */
        |  LEFT_PARENTHESIS RIGHT_PARENTHESIS{
                                $normcall = call_constractor(NULL,0,NULL);
        }
        ;

methodcall: DOUBLE_FULL_STOP ID LEFT_PARENTHESIS moreElist RIGHT_PARENTHESIS{
                                $methodcall = call_constractor($moreElist,1,$ID);
        }
        /*AN TO ELIST = NULL */
        | DOUBLE_FULL_STOP ID LEFT_PARENTHESIS RIGHT_PARENTHESIS{
                                $methodcall = call_constractor(NULL,1,$ID);
        }
        ;    

elist:  expr                    {$$ = $1;}   //elist polla expr
        ;

moreElist: elist                {$$ = $1;}
        |moreElist COMMA elist  {
                                $elist->next = $1;      //expr->next = me moreElist
                                $$ = $3;                //moreElist = expr;        
                                }
        // | <--- !edw eixe kai to keno alla m petaei shift reduce
        ;     

objectdef:  
        LEFT_SQUARE_BRACKET 
        RIGHT_SQUARE_BRACKET    {     //[]
                                $$ = tablecreate_and_emit();
                                }
        |LEFT_SQUARE_BRACKET  moreElist  
        RIGHT_SQUARE_BRACKET    {    // [20,30,"hello"]
                                $$ = tablecreate_and_emit();
                                int i = get_elist_length($moreElist);  //find the length of the items for the table
                                //for each item check its type and insert it to the table
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,$$,newexpr_constnum(i),$moreElist,0,yylineno); // emit (op,temp,index,value)
                                        $moreElist = $moreElist->next; // go to next expr
                                }                                                
                                }
        |LEFT_SQUARE_BRACKET   indexed  
        RIGHT_SQUARE_BRACKET { //[{"x" : (fuction(s){print(s);})} ]
                                $$ = tablecreate_and_emit();
                                int i = get_indexed_length($indexed);
                                for(i; i >= 0; i--){
                                        emit(tablesetelem,$$,$indexed->indexedelem,$indexed->value,0,yylineno); // emit (op,temp,index,value)
                                        $indexed = $indexed->next; // go to next index
                                }   
        
        }
        ;     

indexed: moreindexedelem        {$$ = $1;}
        ; 

/*1 or + times for indexedelems*/
moreindexedelem:   indexedelem  {$$ = $1;}
        |moreindexedelem COMMA 
        indexedelem {
                                $indexedelem->next = $1;        //expr->next = me moreIndex
                                $$ = $3;                        //moreIndex = expr; 
        }
        ;

indexedelem: LEFT_CURLY_BRACKET expr COLON 
        expr RIGHT_CURLY_BRACKET {
                                $$ = indexed_constractor($2,$4,NULL);
        }
        ;


block:  LEFT_CURLY_BRACKET { 
                                CURR_SCOPE++;   
                        }stmts RIGHT_CURLY_BRACKET{
                                if(CURR_SCOPE!=0)
                                        hide(CURR_SCOPE--);
                                if(!$stmts)   
                                        $$ = stmt_constractor(0,0);
                                else 
                                        $block = $stmts;
                }
        |LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET{
                if(CURR_SCOPE!=0)
                        hide(CURR_SCOPE--); 
                $$ = stmt_constractor(0,0); 
        }
        ;


funcname:
        ID              {$funcname = $ID;}
        |               {$funcname = newemptyfuncname();}              //keep track if anonymous fuction
        ;

funcprefix:      
        FUNCTION funcname { 
                        $funcprefix = function_insert($funcname);               //yylval.stringValue
                        $funcprefix->fuctionAddress = nextquadlabel();
                        //gia kapoio logo to lvalue_expr() den douleuei
                        expr* e = newexpr(programfunc_e);       e->sym = $$;
                        emit(jump,NULL,NULL,NULL,0,yylineno); //pou teliwnei to fuct
                        emit(funcstart,e, NULL, NULL,0,yylineno);
                        push(save_fuctionlocals,currscopeoffset());
                        enterscopespace();                      // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
        ;

funcargs:
        LEFT_PARENTHESIS moreidilist RIGHT_PARENTHESIS {
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }
        /*|LEFT_PARENTHESIS RIGHT_PARENTHESIS{
                        CURR_SCOPE--;
                        enterscopespace();              // auksanoume to counter gia to ti var einai kata 1
                        resetformalargsoffset();
        }*/
        ;

funcbody:
        block           {
                        delete_last_fuction_scope();
                        $funcbody = currscopeoffset();
                        exitscopespace();
        }
        ;

funcblockstart : {      push(loopcounterStack,loopcounter);
                        loopcounter = 0;
                        fuctioncounter++;}

funcblockend :  {       pop(loopcounterStack);
                        fuctioncounter--;}

funcdef:
        funcprefix      { fuction_scope_insert(CURR_SCOPE++); }   // gia na kratame to teleutaio scope
        funcargs funcblockstart  funcbody funcblockend  {
                        exitscopespace();
                        $funcprefix->totalLocals = $funcbody;
                        int offset = pop(save_fuctionlocals);
                        restorecurrscopeoffset(offset);
                        $funcdef = $funcprefix;
                        emit(funcend, lvalue_expr($funcprefix), NULL, NULL,0,yylineno);
                        patchlabel($funcprefix->fuctionAddress,nextquadlabel());
        }
        ;    

const:  INTEGER                 { $$ = newexpr_constnum($INTEGER);}
        |REAL                   { $$ = newexpr_constdouble($REAL);}
        |STRING                 { $$ = newexpr_conststring($STRING);}
        |NILL                   { $$ = newexpr_nil();}
        |TRUE                   { $$ = newexpr_constbool(1);}
        |FALSE                  { $$ = newexpr_constbool(0);}
        ;

idlist: ID                      {insert_formal(yylval.stringValue);}
        |COMMA ID               {insert_formal(yylval.stringValue);}
        ;

moreidilist: moreidilist idlist
        |
        ;


ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS    {
                emit(if_eq, $expr, newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);   
                $ifprefix = nextquadlabel(); 
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
        ;
elseprefix: ELSE        {
                //emit(if_eq, $expr, newexpr_constbool(1), NULL, nextquadlabel()+2, yylineno);  
                $elseprefix = nextquadlabel();
                emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
        ;
ifstmt: ifprefix %prec IF stmt{
                patchlabel($ifprefix, nextquadlabel());
                if($2 != NULL)
                        $$ = $stmt;
                else
                        $$ = stmt_constractor(0,0); 
        }
        | ifprefix %prec IF stmt elseprefix stmt {
                patchlabel($1, $3 + 1);
                patchlabel($3, nextquadlabel());
                if($2 != NULL)
                        $$ = $2;
                else
                        $$ = ($4) ? $4 : stmt_constractor(0,0); 
        }
        ;


loopstart :     { ++loopcounter; }
loopend :       { --loopcounter; }


loopstmt : loopstart stmt loopend {$loopstmt = $stmt;}


whilestart : WHILE { $whilestart = nextquadlabel();
}

whilecond : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{
        emit(if_eq,$expr,newexpr_constbool(1),NULL,nextquadlabel()+2,yylineno);
        $whilecond = nextquadlabel();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
}



whilestmt: whilestart whilecond loopstmt {
        emit(jump, NULL, NULL,NULL, $whilestart,yylineno);
        patchlabel($whilecond, nextquadlabel());
        patchlist($loopstmt->breaklist, nextquadlabel());
        patchlist($loopstmt->contlist, $whilestart);
        $$ = $loopstmt ; 
}
        ;

N : { $N = nextquadlabel(); emit(jump,NULL,NULL,NULL,0,yylineno); }
M : { $M = nextquadlabel(); }
forprefix : FOR LEFT_PARENTHESIS moreElist SEMICOLON  M expr SEMICOLON
{       
        $forprefix = for_prefix_constractor($M,nextquadlabel());
        emit(if_eq, $expr, newexpr_constbool(1), NULL,0,yylineno);
}

/*note this for elist*/
forstmt:   forprefix N elist RIGHT_PARENTHESIS N loopstmt N {
        patchlabel($forprefix->enter, $5+1); 
        patchlabel($2, nextquadlabel()); 
        patchlabel($5, $forprefix->test); 
        patchlabel($7, $2+1); 
        patchlist($loopstmt->breaklist, nextquadlabel());
        patchlist($loopstmt->contlist, $2+1);
        $$ =   $loopstmt;} 
        ;


returnstart : RETURN {
        $returnstart = nextquadlabel()+1;
}

returnstmt: returnstart SEMICOLON {
                if(fuctioncounter == 0)
                        yyerror("return w/o function");
                else
                       { 
                        emit(ret,NULL,NULL,NULL,0,yylineno);
                        emit(jump,NULL,NULL,NULL,0,yylineno);
                        patchlabel($returnstart , nextquadlabel());
                }
                $$ = stmt_constractor(0,0);
        } 
        |returnstart expr SEMICOLON{
                if(fuctioncounter == 0)
                        yyerror("return w/o function");
                else
                        {
                emit(ret,NULL,NULL,$expr,0,yylineno);
                emit(jump,NULL,NULL,NULL,0,yylineno);
                patchlabel($returnstart , nextquadlabel());
                }
                $$ = stmt_constractor(0,0);
}
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
    loopcounterStack = createStack(200);
    init_lib_func();
    emit(0,NULL,NULL,NULL,0,0);
    //yyset_in(input_file); // set the input stream for the lexer
    yyparse(); // call the parser function
    if(error_flag != 0)
        printf("/-------------   ERRORS     -------------------/\n");
   print_format(); //print scopes
   print_all_quads(); //print quads
    return 0;
}
