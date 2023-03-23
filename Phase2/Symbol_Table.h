
#include <string.h>

extern int yylineno;

typedef enum var_type {var,fuction} var_type;
typedef enum var_id{global, typical, local,user_func, lib_func} var_id;



typedef struct domhvar{
    char* name;
    int scope;
    int lineno;
    
    //var_Type var_type;
    //int hidden ;     //me 0 den einai krymmeno, 1 einai 
    //struct var_struct * next_same_scope;
} var_struct;


typedef struct values{

    var_type type;
    var_id id;          //ti eimai?
    char* name;
    


}*values;



typedef struct domhfunc{
    char* name;
    //func_Type func_type;
    
    /*  prepei na mpoun ta orismata, pou apo oti fantazomai
    *   einai apla          var_struct me ->var_type == typical;
    */

    int scope;
    int lineno;
    //int hidden;     //me 0 den einai krymmeno, 1 einai
    //struct func_struct * next_same_scope;
} func_struct;


typedef struct Sentrys{
        func_Type func_type;
    //var_Type var_type;
    /*  prepei na mpoun ta orismata, pou apo oti fantazomai
    *   einai apla          var_struct me ->var_type == typical;
    */

    int scope;
    int lineno;
    int hidden;     //me 0 den einai krymmeno, 1 einai 
    //struct func_struct * next_same_scope;
} Symbol_Entrys;


int insert(){
    return 0;
}


int lookup(char* key_name){
    return 0;
}

/*  front3@16 leei na kanoume mia lookup gia sygkekrimeno scope, kai mia oxi*/
int lookup_scope(char* key_name, int key_scope){
    
    /*  find a way to iterate through the hashtable and the whole 
    *   collision lists until the first symbol with target_scope is found.
    *   after that iterate through it's scope link
    *   and hide every node.
    */

    //temp = 

    /*POUKA COMMENTS*/
    /* thelei na kaneis declare kapu to temp KAI TO TARGET SCOPE*/
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
    //iterate kai bres to scope
    var_struct temp;
}

void init_lid_func(char * lib_func){
    //me thn insert kanume init oles tis lib funcs
}