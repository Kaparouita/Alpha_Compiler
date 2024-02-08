#include "../avm.h"
#include "../vm.h"

extern char* number_tostring (avm_memcell*);
extern char* string_tostring (avm_memcell*);
extern char* bool_tostring (avm_memcell*);
extern char* table_tostring (avm_memcell*);
extern char* userfunc_tostring (avm_memcell*);
extern char* libfunc_tostring (avm_memcell*);
extern char* nil_tostring (avm_memcell*);
extern char* undef_tostring (avm_memcell*);


typedef double (*arithmetic_func_t)(double x, double y);

double add_impl (double x, double y) {return x+y;}
double sub_impl (double x, double y) {return x-y;}
double mul_impl (double x, double y) {return x*y;}
double div_impl (double x, double y) {if(y) return x/y; return 0; /*assert(y != 0);avm_error("y is 0!"); leei se comment error check?*/}
double mod_impl (double x, double y) {
	if(y)
	    return ((unsigned) x) % ((unsigned) y);
    return 0;
}



/* Arithmetic functions dispatcher */
arithmetic_func_t arithmeticFuncs[] = {
	add_impl,
	sub_impl,
	mul_impl,
	div_impl,
	mod_impl
};

void execute_arithmetic (instruction* instr){
	avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

	assert(lv && (&stack[N-1] >= lv && lv > &stack[top] || lv == &retval));
	assert(rv1 && rv2);
	//printf("===%u==%u==%u=",instr->result.type,instr->arg1.type,instr->arg2.type);
	if(rv1->type != number_m || rv2->type != number_m){
		printf("not a number in arithmetic!");
		executionFinished = 1;
	}else{
		arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
		avm_memcellclear(lv);
		lv->type = number_m;
		lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
	}
}

