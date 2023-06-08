#ifndef Symbol_Table_H
#define Symbol_Table_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quads.h"


#define TABLE_SIZE 100
#define BUCKETS 512

typedef enum var_type {varr,fuction} var_type;
typedef enum var_id{global, formal, local,user_func, lib_func} var_id;
typedef enum scopespace_t{programvar,functionlocal,formalarg}scopespace_t;


/**
 * @brief 
 * 
 */
typedef struct var{
    var_type type;
    var_id id;          
    char* name;
    unsigned offset;
	scopespace_t space;
    int fuctionAddress ;    // 0 gia var alliws to adress tou fuction
    int totalLocals;       // an einai fuction posa locals exei
    int scope;
    int hide;             // 0 gia hidden 1 gia visable
    int line;
    struct var *next;    // gia to next var an pesei se idio bucket sto table
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
var* new_var(var_type type, var_id id, char* name, int scope,scopespace_t space,unsigned offset, int hide, int line);
    
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


typedef struct last_fuction_scope {
    int scope;
    struct last_fuction_scope *next;// keep track of the scope of the last fuction
} last_fuction_scope;


void return_check();
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
 * @return return var if var is in the scope NULL otherwise 
 */
var* lookup_scope(int scope,char*vname);


/**
 * @brief Get the scope var object
 * 
 * @return the first element of the scope or NULL if scope doesnt exist 
 */
var *get_scope_var(int scope);
/**
 * @brief look up all the table
 * 
 * @param table 
 * @param v 
 * @return 1 if var already on the table 0 otherwise
 */
var *lookup_globaly( char*vname);
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
 * @brief check if its the same with the lib_fucts
 * 
 * @param str 
 * @return 1 if its a lib_fuct name 0 otherwise 
 */
int check_collisions(char *str);

var* lookup_in_out(int scope,char* vname);
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

/**
 * @brief gia ta checks tou last fuction
 * ousiastika gia ta access einai
 */
void delete_last_fuction_scope();
void fuction_scope_insert(int scope);

/**
 * @brief check if the fuction is anonymous
 * if the name equals '_' it is
 * 
 * @param name 
 * @return the fuction name 
 */
char *check_anonymous(char *name);

var *lookup_var(char* vname);
/**
 * @brief check where the last fuction was and if my var has access
 * 
 * @param myvar 
 * @return 1 if it has access 0 otherwess
 */
int check_access(var *myvar);

/**
 * @brief init all lib fuctions
 * 
 * @param lib_func 
 */
void init_lib_func();

/**
 * @brief printing opws tou savvidh
 * 
 */
void print_format();
/**
 * @brief print olo to table
 * 
 * @param oSymTable 
 */
void print_table(var_table *oSymTable);
const char* enum_scospace_t_print(enum scopespace_t myenum) ;

/**
 * @brief create a name for my hidden tmp
 * 
 * @return char* 
 */
char *newtempname();
/**
 * @brief create a tmp
 * 
 * @return var* 
 */
var *newtemp();
/**
 * @brief resets tmp count
 * 
 */
void resettemp();
/**
 * @brief checkarei an kati einai mesa se fuction
 * 
 * @return int 
 */
int check_if_in_fuction();


char **get_lib_fuctions();

#endif /*Symbol_talbe_H*/