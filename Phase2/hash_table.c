#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Symbol_Table.h"


/**
 * @brief VARIABLES
 * 
 */

var* first; // first var for scope_list
var_table* table ; //table

var* new_var(var_type type, var_id id, char* name, int scope, int hide, int line){
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

var_table *create_table(unsigned hs){
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
        if(strcmp(curr->name,v->name) != 0){
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

int lookup_globaly(var_table *table, char*vname){
    var *curr;
    int index = hash(vname); /*get the key*/
    if ((table->buckets[index]) != NULL){
        curr = table->buckets[index];
        /*tsekare thn alysida tou*/
        while (curr != NULL)
        {
            if (strcmp(curr->name,vname) == 0 && curr->hide == 1) /*if its the same return 1*/
                return 1;
            curr = curr->next;
        }
    }
    return 0;
}


var *lookup_var(var_table *table, char* vname){
    var *curr;
    int index = hash(vname); /*get the key*/
    if ((table->buckets[index]) != NULL){
        curr = table->buckets[index];
        /*tsekare thn alysida tou*/
        while (curr != NULL)
        {
            if (strcmp(curr->name,vname) == 0 && curr->hide == 1) /*if its the same return 1*/
                return curr;
            curr = curr->next;
        }
    }
    return NULL;
}


int lookup_global(char* vname){
    var *myvar=lookup_var(table,vname);
    if(myvar == NULL)
        return 1;
    if(myvar->scope==0)
        return 0;
    return 1;
}

var* lookup_scope(int scope,char* vname){
    var *curr = get_scope_var(scope);
    if(curr == NULL)
        return NULL;
    while(curr!=NULL && curr->scope == scope){
            if (strcmp(curr->name,vname) == 0 && curr->hide == 1) /*if its the same return 1*/
                return curr;
            curr = curr->next;
    }
    return NULL;
}

var* lookup_in_out(int scope,char* vname){
    var *curr = get_scope_var(scope);
    while(curr == NULL && scope >= 0){
        scope--;
        curr = get_scope_var(scope);
    }
    while(scope >= 0){
        if (strcmp(curr->name,vname) == 0 && curr->hide == 1) /*if its the same return 1*/
            return curr;
        curr = curr->next;
        while(curr == NULL && scope >= 0){
            scope--;
            curr = get_scope_var(scope);
        }
    }
    return NULL;
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
    while(curr != NULL && curr->scope != 0){
        curr->hide = 0;
        curr = curr->same_scope_next;
    }
    return 0;
}

void print_var(var *v) {
    
    printf("\"%s\" ", v->name);
    printf("[%s] ",   enum_id(v->id));
    printf("(line %d) ", v->line);
    printf("(scope %d) \n", v->scope);
    
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
var_id switch_enum(var_id id ){
    switch(id){
        case global:      return local;
        case local :      return global;
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

unsigned int hash(char *str) {
    unsigned int hashval;
    for (hashval = 0; *str != '\0'; str++) {
        hashval = *str + 31 * hashval;
    }
    return hashval % TABLE_SIZE;
}

void init_lib_func(){
    table = create_table(5381);
    
    hash_insert(new_var(fuction,lib_func,"print",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"input",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"objectmemberkeys",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"objecttotalmembers",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"totalarguments",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"argument",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"typeof",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"strtonum",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"sqrt",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"cos",0,1,0),table);
    hash_insert(new_var(fuction,lib_func,"sin",0,1,0),table);
}

/**
 * @brief Get max scope
 * 
 * @param current_scope 
 * @param previous_scope 
 * @return int 
 */
int max_scope(int current_scope,int previous_scope){
    return ((current_scope > previous_scope) ?  current_scope : previous_scope);
}

int check_collisions(char *str){
    char lib_fuctions[11][20] = {{"print"},{"input"},{"objectmemberkeys"},{"objecttotalmembers"},{"totalarguments"}
    ,{"argument"},{"typeof"},{"strtonum"},{"sqrt"},{"cos"},{"sin"}};
    for(int i=0;i<11;i++){
        if(strcmp(str,lib_fuctions[i]) == 0)
            return 1;
    }
    return 0;
}

void hide_all(int Curr_scope){
    Curr_scope--;
    var *curr = get_scope_var(Curr_scope);
    while(Curr_scope != 0){
        
         while(curr == NULL){
            Curr_scope--;
            curr = get_scope_var(Curr_scope);
            if(Curr_scope == 0)
                return;
        }
        if(curr->type != fuction && curr->id != formal){
            curr->hide = 0;
        }
        curr = curr->same_scope_next;
    } 
}


int main()
{
    
    table = create_table(5381);

    var *x= new_var(fuction,global,"x",1,1,0);
    var *y=new_var(varr,global,"y",1,1,8);
    var *k=new_var(fuction,lib_func,"k",2,1,0);
    hash_insert(y,table);
   // hash_insert(new_var(varr,global,"x",1,1,3),table);
   hash_insert(new_var(varr,local,"t",2,1,3),table);
   hash_insert(new_var(varr,local,"Y",3,1,3),table);
   hash_insert(new_var(varr,local,"U",3,1,3),table);

    hash_insert(k,table);
    hash_insert(x,table);
    init_lib_func();

    var *new = lookup_in_out(3,"t");
    if(new!=NULL)
        printf("YO %s\n",new->name);
    hide_all(2);
    //hide(1);
    print_scope(2);   
   
}