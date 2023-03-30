#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100
#define BUCKETS 512

typedef enum var_type {varr,fuction} var_type;
typedef enum var_id{global, formal, local,user_func, lib_func} var_id;

/**
 * @brief 
 * 
 */
typedef struct var{
    var_type type;
    var_id id;          
    char* name;
    int scope;
    int hide;             // 0 gia hidden 1 gia visable
    int line;
    struct var *next;    // gia to next var an pesei se idio bucket sto table
    struct var *same_scope_next; // gia to same scope
    struct var *s_next;   // scope next
}var;


/**
 * @brief Constractor gia ena var
 * 
 * @param type 
 * @param id 
 * @param name 
 * @param scope 
 * @param hide 
 * @param line 
 * @param next 
 * @return struct var* 
 */
var* new_var(var_type type, var_id id, char* name, int scope, int hide, int line);
    
/**
 * @brief hash table 
 *
 */
typedef struct var_table {
    var **buckets;
    int num_buckets;
    int size;
    unsigned hs; /*hash multiplier*/
} var_table;


/**
 * @brief Create a table object
 * 
 * @param hs hash multiplayer
 * @return var_table* 
 */
var_table *create_table(unsigned hs);
/**
 * @brief Create a hash table object
 * 
 * @return hash_table_t* 
 */
var_table *create_hash_table() ;

/**
 * @brief insert sto scope list
 * 
 * @param v 
 * @return * void 
 */
void scope_insert(var *v);
/**
 * @brief hash fuction
 * 
 * @param str to name
 * @return unsigned int 
 */
unsigned int hash(char *str) ;

/**
 * @brief insert ena var sto hash table
 * 
 * @param v 
 * @param table 
 */
void hash_insert( var *v, var_table *table);

/**
 * @brief lookup for a var in spesific scope
 * 
 * @param scope 
 * @return return 1 if var is in the scope 0 otherwise 
 */
int lookup_scope(int scope,char*vname);


/**
 * @brief Get the scope var object
 * 
 * @return the first element of the scope or NULL if scope doesnt exist 
 */
var *get_scope_var(int scope);
/**
 * @brief 
 * 
 * @param table 
 * @param v 
 * @return 1 if var already on the table 0 otherwise
 */
int lookup_globaly(var_table *table, char*vname);
/**
 * print gia enum(id)
*/
char *enum_id(var_type id);
/**
 * Print gia enum(type)
*/
char *enum_type(var_type type);
var_id switch_enum(var_id id );
/**
 * @brief hide all variables withing scope
 * 
 * @param scope 
 * @return int 
 */
int hide (int scope);

/**
 * @brief check if its the same with the lib_fucts
 * 
 * @param str 
 * @return 1 if its a lib_fuct name 0 otherwise 
 */
int check_collisions(char *str);

/**
 * @brief print var v
 * 
 * @param v 
 */
void print_var(var *v);
/**
 * @brief print all vars in scope
 * 
 * @param scope 
 */
void print_scope(int scope);

char *check_anonymous(char *name);

var *lookup_var(var_table *table, char* vname);
int check_access(char *name);

int max_scope(int current_scope,int previos_scope);
/**
 * @brief init all lib fuctions
 * 
 * @param lib_func 
 */
void init_lib_func();