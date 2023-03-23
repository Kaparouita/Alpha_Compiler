#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101
#define BUCKETS 512

typedef enum var_type {var,fuction} var_type;
typedef enum var_id{global, formal, local,user_func, lib_func} var_id;


/**
 * Struct var:
 * 
*/
typedef struct var{

    var_type type;
    var_id id;          
    char* name;
    int scope;
    int hide;
    int line;
    struct var *next;    // gia to next var an pesei se idio bucket sto table
    struct var *scope;   // gia same scope mporei na xrhsimepsei
}var;

typedef struct SymTab {
    var **buckets;
    int num_buckets;
    var *scope_head;
} SymTab;


struct vars { /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name; /* variable name */
    int value; /* variable value */
};

hash_table_t *create_hash_table() {
    hash_table_t *ht = malloc(sizeof(hash_table_t));

    ht->buckets = calloc(NUM_BUCKETS, sizeof(var_t *));
    ht->head = NULL;

    return ht;
}

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