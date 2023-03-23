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
}var;

struct var* new_var(var_type type, var_id id, char* name, int scope, int hide, int line, struct var* next) {
    struct var* v = (struct var*) malloc(sizeof(struct var));
    v->type = type;
    v->id = id;
    v->name = strdup(name);
    v->scope = scope;
    v->hide = hide;
    v->line = line;
    v->next = next;
    return v;
}

/**
 * hash-table struct
*/
typedef struct hash_table_t {
    var **buckets;
    int num_buckets;
    var *scope_head;
} hash_table_t;


/**
 * hash-table constractor
*/
hash_table_t *create_hash_table() {
    hash_table_t *ht = malloc(sizeof(hash_table_t));

    ht->buckets = calloc(BUCKETS, sizeof(var_t *));
    ht->head = NULL;

    return ht;
}
/**
 * hash fuction
*/
unsigned int hash(char *str) {
    unsigned int hashval;
    for (hashval = 0; *str != '\0'; str++) {
        hashval = *str + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

int hash_insert(Table *table, char *name, int type, int scope) {
    // Calculate hash value
    int hash_val = hash(name) % table->num_buckets;

    // Traverse bucket list to check for existing variable
    VarNode *var = table->buckets[hash_val];
    while (var != NULL) {
        if (strcmp(var->name, name) == 0 && var->scope == scope) {
            // Variable already exists in scope, return error
            return 0;
        }
        var = var->next;
    }

    // Create new variable node
    var *new_var = (var*)malloc(sizeof(var));
    new_var->name = name;
    new_var->type = type;
    new_var->scope = scope;
    new_var->next = NULL;

    // Insert node at beginning of bucket list
    new_var->next = table->buckets[hash_val];
    table->buckets[hash_val] = new_var;

    // Insert node at beginning of scope list
    new_var->next_scope = table->scopes[scope];
    table->scopes[scope] = new_var;

    // Increment count
    table->count++;

    return 1;
}


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