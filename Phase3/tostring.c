#include "avm.h"
#include "vm.h"
#include <stdarg.h>
#define EMPTY " "


int asprintf(char **str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the size needed for the formatted string
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (size < 0) {
        return -1; // vsnprintf failed
    }

    // Allocate memory for the formatted string
    *str = (char *)malloc(size + 1);
    if (*str == NULL) {
        return -1; // Memory allocation failed
    }

    va_start(args, format);
    size = vsnprintf(*str, size + 1, format, args);
    va_end(args);

    if (size < 0) {
        free(*str);
        return -1; // vsnprintf failed
    }
    return size;
}

//TO STRING_FUNCTIONS
char * number_tostring(avm_memcell* m){
    if(sizeof(m->data.numVal) != sizeof(double))
        return EMPTY;
    char* str = NULL;
    //sprintf(str, "%.3lf", m->data.numVal);  
    if(asprintf(&str, "%3.f", m->data.numVal) != -1);
        return str;
    return EMPTY;
}
char * string_tostring(avm_memcell* m){
    if (sizeof(m->data.strVal) != sizeof(char*)) {
        printf("Error: Incorrect type passed to StringToString()\n");
        return EMPTY;
    }
    char* str = NULL;
    asprintf(&str, "%s", m->data.strVal);
    return str;
}

char* bool_tostring(avm_memcell* m){
    if (sizeof(m->data.boolVal) != sizeof(unsigned char)) {
        printf("Error: Incorrect type passed to BoolToString()\n");
        return EMPTY;
    }
    char *str = malloc(sizeof(char*));
    str =  (m->data.boolVal )?  "true" : "false";
    return str; 
}

char* table_tostring(avm_memcell* m){
    if (m->type != table_m || m->data.tableVal == NULL) {
        printf("Invalid input: Not a table.\n");
        return NULL;
    }
    avm_table* table = m->data.tableVal;
    const int BUFFER_SIZE = 1024;
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        printf("Failed to allocate memory for buffer.\n");
        return NULL;
    }
    memset(buffer, 0, BUFFER_SIZE);

    int offset = 0;
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "{");

    // Iterate over string-indexed buckets
    for (int i = 0; i < AVM_TABLE_HASHSIZE; i++) {
        avm_table_bucket* bucket = table->strIndexed[i];
        while (bucket != NULL) {
            if (bucket->key.type == number_m && bucket->value.type == string_m) {
                offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "'%2.f': %s, ", bucket->key.data.numVal, bucket->value.data.strVal);
            }
            bucket = bucket->next;
        }
    }
    // Iterate over number-indexed buckets
    for (int i = 0; i < AVM_TABLE_HASHSIZE; i++) {
        avm_table_bucket* bucket = table->numIndexed[i];
        while (bucket != NULL) {
            if (bucket->key.type == number_m && bucket->value.type == number_m) {
                offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "%2.f: %2.f,%d\n ", bucket->key.data.numVal, bucket->value.data.numVal,i);
            }
            bucket = bucket->next;
        }
    }
    // Remove the trailing comma and space
    if (offset >= 2) {
        buffer[offset - 2] = '\0';
    }
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "}");
    return buffer;
}

char* userfunc_tostring(avm_memcell* m){
        if (sizeof(m->data.funcVal) != sizeof(unsigned)) {
        printf("Error: Incorrect type passed to convertFuncToString()\n");
        return EMPTY;
    }
    char* str = NULL;
    asprintf(&str, "%u", m->data.funcVal);
    return str;
}
char* libfunc_tostring(avm_memcell* m){
    if (sizeof(m->data.libfuncVal) != sizeof(char*)) {
        printf("Error: Incorrect type passed to convertLibFuncToString()\n");
        return EMPTY;
    }
    char* str = NULL;
    asprintf(&str, "%s", m->data.libfuncVal);
    return str;
}

char* nil_tostring(avm_memcell* m){
    if(m->type != nil_m)
        return EMPTY;
        char* str = NULL;
    asprintf(&str, "%s", "nil");
    return str;
}
char* undef_tostring(avm_memcell* m){
    if(m->type != undef_m)
        return EMPTY;
    char* str = NULL;
    asprintf(&str, "%s", "un_def");
    return str;
}

typedef char* (*tostring_func_t)(avm_memcell*);
tostring_func_t tostringFuncs[] = {
	number_tostring,
	string_tostring,
	bool_tostring,
	table_tostring,
	userfunc_tostring,
	libfunc_tostring,
	nil_tostring,
	undef_tostring
};

char* avm_tostring (avm_memcell* m){
	assert(m->type >= 0 && m->type <= undef_m);
	return(*tostringFuncs[m->type])(m);
}