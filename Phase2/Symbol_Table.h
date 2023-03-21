


typedef enum types_var {global, typical, local} var_Type;
typedef enum types_func {user_func, lib_func} func_Type;


typedef struct domhvar{
    char* name;
    var_Type var_type;
    char* scope;
    int lineno;
} var_struct;


typedef struct domhfunc{
    char* name;
    func_Type func_type;
    //prepei na mpoun ta orismata, de jkserw types oute plithos
    int scope;
    int lineno;
} func_struct;


void insert(){
    //
}

void lookup(){
    //ksekiname apo front3@14 3 bullet  scope link giati an kanw iterate to hash 
    //table tha mporw na brw ta idia scopes se alla stoixeio xwris na to psaksw olo to table.
    if(temp->scope == target_scope)
}

void hide (int target_scope){
    //prepei na brw kai sta 2 struct 
    //ta elements me scope == target
    //iterate kai bres to scope
    var_struct temp;
}