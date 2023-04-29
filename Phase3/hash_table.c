#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Symbol_Table.h"

extern int yyerror (char* yaccProvidedMessage);

/**
 * @brief VARIABLES
 * 
 */

var* first; // first var for scope_list
var_table* table ; //table
last_fuction_scope *fuction_scope ;

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
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = v;
        scope_insert(v); //insert sto scope tou 
    }
    table->size++;
}

void scope_insert(var *v){
    if(first == NULL){
        first = v;
        return;
    }
    var *curr = first;
    //phgene sto scope tou
    while(curr->scope != v->scope && curr->s_next != NULL){
        curr = curr->s_next;
    }
    //proxwra mexri to last element kai kantw insert sthn list
    while(curr->s_next != NULL && curr->s_next->scope == v->scope){
        curr = curr->s_next;        
    }
    var *tmp = curr->s_next;
    curr->s_next = v;
    v->s_next = tmp;
}

void fuction_scope_insert(int scope){
    if(fuction_scope == NULL){
        fuction_scope = malloc(sizeof(last_fuction_scope *));
        fuction_scope->scope = scope;
        fuction_scope->next = NULL;
        return;
    }
    last_fuction_scope *curr = fuction_scope;
    //create new 
    last_fuction_scope *new = malloc(sizeof(last_fuction_scope *));
    new->next = NULL;
    new->scope = scope;
    while(curr->next != NULL)
        curr = curr->next;
    //insert new
    curr->next = new;
}

void delete_last_fuction_scope(){
    if(fuction_scope == NULL)
        return;
    last_fuction_scope *curr = fuction_scope;
    last_fuction_scope *prev = curr;
    while(curr->next != NULL){
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
   // free(curr);
}

var *lookup_globaly(char*vname){
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


var *lookup_var(char* vname){
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

var* lookup_scope(int scope,char* vname){
    var *curr = get_scope_var(scope);
    while(curr!=NULL && curr->scope == scope){
            if (strcmp(curr->name,vname) == 0 && curr->hide == 1) /*if its the same return 1*/
                return curr;
            curr = curr->s_next;
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
        curr = curr->s_next;
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
        curr = curr->s_next;
    }
    return 0;
}

int check_access(var *myVar){
    if(fuction_scope == NULL)
        return 1;
    last_fuction_scope *curr = fuction_scope;
    while(curr->next != NULL)
        curr = curr->next;
    if(myVar->scope > curr->scope)
        return 1; 
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

void print_scope(int scope){
    printf("\n/-------------   SCOPE #%d   -------------------/\n",scope);
    if(first == NULL)
        return;
    var *curr = get_scope_var(scope);
    while(curr != NULL && curr->scope == scope){
        
            print_var(curr);
        curr = curr->s_next;
    }
    if(curr != NULL && curr->scope != scope)
        {}
}

void print_table(var_table *oSymTable)
{
    int i = 0;
    printf("START PRINTING\n/----------------------------------------/\n");

    for (i = 0; i < oSymTable->num_buckets; i++)
    {
        var *curr = oSymTable->buckets[i];
        if (curr != NULL)
        {
            printf("TABLE INDEX : %d ,", i);
            while(curr!=NULL){
                print_var(curr);
                curr=curr->next;
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

int check_collisions(char *str){
    char lib_fuctions[11][20] = {{"print"},{"input"},{"objectmemberkeys"},{"objecttotalmembers"},{"totalarguments"}
    ,{"argument"},{"typeof"},{"strtonum"},{"sqrt"},{"cos"},{"sin"}};
    for(int i=0;i<11;i++){
        if(strcmp(str,lib_fuctions[i]) == 0)
            return 1;
    }
    return 0;
}

void print_format(){
    int scope = 0;
    var* myvar = get_scope_var(scope);
    while(myvar != NULL){
        print_scope(scope);
        myvar = get_scope_var(++scope);
    }
}

void check_if_fuction(var *v){
    if(v == NULL)
        return;
    if(v->type == fuction)
        yyerror("Illegal operation with fuction");
    return;
}
/*
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
   
}*/