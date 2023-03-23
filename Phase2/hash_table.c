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
    } else {
        // If there's already a variable in this bucket,
        // append this one to the end of the list.
        var *curr = table->buckets[index];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = v;
    }
    table->size++;
}


void print_var(var *v) {
    printf("Type: %s ,", v->type);
    printf("ID: %s ,", v->id);
    printf("Name: %s ,", v->name);
    printf("Scope: %d ,", v->scope);
    printf("Hide: %d ,", v->hide);
    printf("Line: %d \n", v->line);
}

void print_table(var_table *oSymTable)
{
    int i = 0;
    printf("START PRINTING\n");

    for (i = 0; i < oSymTable->num_buckets; i++)
    {
        if (oSymTable->buckets[i] != NULL)
        {
            printf("TABLE INDEX : %d", i);
            while(oSymTable->buckets[i]!=NULL){
                print_var(oSymTable->buckets[i]);
                oSymTable->buckets[i]=oSymTable->buckets[i]->next;
            }
        }
    }
    printf("END PRINTING\n/----------------------------------------/\n");
    return;
}

var_table* table ;


int main()
{
    
    table = create_table(5381);
    
    hash_insert(new_var(varr,global,"x",1,0,0),table);
    hash_insert(new_var(varr,global,"x",1,0,8),table);
    hash_insert(new_var(varr,global,"x",1,0,3),table);
    hash_insert(new_var(fuction,lib_func,"k",1,0,0),table);

    print_table(table);
}