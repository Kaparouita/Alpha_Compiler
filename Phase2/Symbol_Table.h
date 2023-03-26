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
 * @brief VARIABLES
 * 
 */
int CURR_SCOPE = 0;
var* first; // first var for scope_list
var_table* table ; //table

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
unsigned int hash(char *str) {
    unsigned int hashval;
    for (hashval = 0; *str != '\0'; str++) {
        hashval = *str + 31 * hashval;
    }
    return hashval % TABLE_SIZE;
}

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
int lookup_scope(int scope,var *v);

int lookup(char* key_name){
    return 0;
}

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
int lookup_globaly(var_table *table, var *v);
/**
 * print gia enum(id)
*/
char *enum_id(var_type id);
/**
 * Print gia enum(type)
*/
char *enum_type(var_type type);

/**
 * @brief hide all variables withing scope
 * 
 * @param scope 
 * @return int 
 */
int hide (int scope);

/**
 * @brief init all lib fuctions
 * 
 * @param lib_func 
 */
void init_lid_func(){
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