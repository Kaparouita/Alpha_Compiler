#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Symbol_Table.h"


var* new_var(var_type type, var_id id, char* name, int scope, int hide, int line) {
    var* v = (var*) malloc(sizeof(var));
    v->type = type;
    v->id = id;
    v->name = strdup(name);
    v->scope = scope;
    v->hide = hide;
    v->line = line;
    v->next = NULL;
    v->s_next = NULL;
    v->same_scope_next = NULL;
    return v;
}

var_table *create_table(unsigned hs)
{
    var_table *new_table = malloc(sizeof(var_table *));
    new_table->buckets = malloc(BUCKETS * sizeof(struct var *));
    new_table->size = 0;
    new_table->num_buckets = BUCKETS;
    new_table->hs = hs;
    return new_table;
}

void hash_insert(var *v,var_table *table) {
    int index = hash(v->name);
    if (table->buckets[index] == NULL) {
        table->buckets[index] = v;
        scope_insert(v);
    } else {
        // If there's already a variable in this bucket,
        // append this one to the end of the list.
        // kai den exoume vrei var me to idio name
        var *curr = table->buckets[index];
        while (curr->next != NULL &&(strcmp(curr->name,v->name) != 0) ) {
            curr = curr->next;
        }
        if(strcmp(curr->name,v->name) == 0)
            printf("VAR ALREADY IN THE TABLE!\n");
        else{  
            curr->next = v;
            scope_insert(v); //insert sto scope tou    
        }
    }
    table->size++;
}

void scope_insert(var *v){
    if(first == NULL){
        first = v;
        return;
    }
    var *curr = first;
    while(curr->scope != v->scope && curr->s_next != NULL){
        curr = curr->s_next;
    }
    if(curr->scope == v->scope){
        //proxwra mexri to last element kai kantw insert sthn list
        while(curr->same_scope_next != NULL){
            curr = curr->same_scope_next;
        }
        curr->same_scope_next = v;
    }
    else{
        curr->s_next = v;
    }
}

int lookup_globaly(var_table *table, var *v){
    var *curr;
    int index = hash(v->name); /*get the key*/
    if ((table->buckets[index]) != NULL){
        curr = table->buckets[index];
        /*tsekare thn alysida tou*/
        while (curr != NULL)
        {
            if (strcmp(curr->name,v->name) == 0) /*if its the same return 1*/
                return 1;
            curr = curr->next;
        }
    }
    return 0;
}

int lookup_scope(int scope,var *v){
    var *curr = get_scope_var(scope);
    if(curr == NULL)
        return 0;
    while(curr!=NULL && curr->scope == scope){
            if (strcmp(curr->name,v->name) == 0) /*if its the same return 1*/
                return 1;
            curr = curr->next;
    }
    return 0;
}


var *get_scope_var(int scope){
    if(first == NULL){
        return NULL;
    }
    var *curr = first;
    while(curr->scope != scope && curr->s_next != NULL){
        curr = curr->s_next;
    }
    if(curr->scope == scope)
        return curr;
    else
        return NULL;
}

int hide(int scope){
    var *curr = get_scope_var(scope);
    if(curr == NULL)
        return 1;//nothing within the scope
    while(curr != NULL){
        curr->hide = 0;
        curr = curr->same_scope_next;
    }
    return 0;
}

void print_var(var *v) {
    printf("Type: %s ,", enum_type(v->type));
    printf("ID: %s ,",   enum_id(v->id));
    printf("Name: %s ,", v->name);
    printf("Scope: %d ,", v->scope);
    printf("Hide: %d ,", v->hide);
    printf("Line: %d \n", v->line);
}

char *enum_type(var_type type){
    switch(type){
        case varr:      return "Variable";
        case fuction:   return "Fuction";
        default : return "ERROR";
    }
}

char *enum_id(var_type id){
    switch(id){
        case global:      return "Global_var";
        case formal:      return "Formal_var";
        case local:       return "Local_var";  
        case user_func:   return "User_Fuction";  
        case lib_func:    return "Library_Fuction";
        default : return "ERROR";
    }
}


void print_scope(int scope){
    if(first == NULL)
        return;
    var *curr = first;
    while(curr->scope != scope && curr->s_next != NULL){
        curr = curr->s_next;
    }
    while(curr != NULL && curr->scope == scope){
        if(curr->hide == 1)
            print_var(curr);
        curr = curr->same_scope_next;
    }
    if(curr != NULL && curr->scope != scope)
        printf("NO VAR/FUNC WITH THAT SCOPE FOUND\n");
}

void print_table(var_table *oSymTable)
{
    int i = 0;
    printf("START PRINTING\n");

    for (i = 0; i < oSymTable->num_buckets; i++)
    {
        if (oSymTable->buckets[i] != NULL)
        {
            printf("TABLE INDEX : %d ,", i);
            while(oSymTable->buckets[i]!=NULL){
                print_var(oSymTable->buckets[i]);
                oSymTable->buckets[i]=oSymTable->buckets[i]->next;
            }
        }
    }
    printf("END PRINTING\n/----------------------------------------/\n");
    return;
}



int main()
{
    
    table = create_table(5381);

    var *x= new_var(varr,global,"x",1,1,0);
    var *y=new_var(varr,global,"y",1,1,8);
    var *k=new_var(fuction,lib_func,"k",2,1,0);
    hash_insert(y,table);
    hash_insert(new_var(varr,global,"x",1,1,3),table);
    hash_insert(k,table);
    hash_insert(x,table);
    init_lid_func();

    //hide(1);
    print_scope(0);   
   
}