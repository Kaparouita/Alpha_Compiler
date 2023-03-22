typedef enum types_var {global, typical, local} var_Type;
typedef enum types_func {user_func, lib_func} func_Type;


/*** LOG ***
* den eginan polla, ligo ksekatharisma kai sxolia, exw kollhsei sto pos tha einai h domh symtable.
*/


typedef struct domhvar{
    char* name;
    var_Type var_type;
    int scope;
    int lineno;
    int hidden = 0;     //me 0 den einai krymmeno, 1 einai 
    struct* var_struct next_same_scope;
} var_struct;


typedef struct domhfunc{
    char* name;
    func_Type func_type;
    
    /*  prepei na mpoun ta orismata, pou apo oti fantazomai
    *   einai apla          var_struct me ->var_type == typical;
    */

    int scope;
    int lineno;
    int hidden = 0;     //me 0 den einai krymmeno, 1 einai 
    struct* func_struct next_same_scope;
} func_struct;


int insert(){
    return 0;
}


int lookup(char* key_name){
    return 0;
}

/*  front3@16 leei na kanoume mia lookup gia sygkekrimeno scope, kai mia oxi*/
int lookup(char* key_name, int key_scope){
    
    /*  find a way to iterate through the hashtable and the whole 
    *   collision lists until the first symbol with target_scope is found.
    *   after that iterate through it's scope link
    *   and hide every node.
    */

    //temp = 
    while(temp != NULL){    /*Simple search*/
        if((strcmp(temp->name,key_name) == 0) && temp->scope == target_scope){
            //epishs prepei na frontisw na prospelasw oloklhra ta buckets (dhladh kai ta collision lists tous) 
            //ki edw arxizw na kanw iterate th lista me ta idia scopes.kapws.
        }
        temp = temp->next;
    }
}


int hide (int target_scope){
    //prepei na brw kai sta 2 struct 
    //ta elements me scope == target
    // tha xrhsimopoihthei lookup
    return 0;
}