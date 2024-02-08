#include "../avm.h"
#include "../vm.h"

typedef unsigned char(*tobool_func_t)(avm_memcell*);
unsigned char execute_compare (instruction* instr);

/*tranform to bool*/
unsigned char number_tobool (avm_memcell* m){ return m->data.numVal != 0; }
unsigned char string_tobool (avm_memcell* m){ return m->data.strVal[0] != 0; }
unsigned char bool_tobool (avm_memcell* m){ return m->data.boolVal; }
unsigned char table_tobool (avm_memcell* m){ return 1; }
unsigned char userfunc_tobool (avm_memcell* m){ return 1; }
unsigned char libfunc_tobool (avm_memcell* m){ return 1; }
unsigned char nil_tobool (avm_memcell* m) { return 0; }
unsigned char undef_tobool (avm_memcell* m){ return 0; }

tobool_func_t toboolFuncs[] = {
	number_tobool,
	string_tobool,
	bool_tobool,
	table_tobool,
	userfunc_tobool,
	libfunc_tobool,
	nil_tobool,
	undef_tobool
};

unsigned char avm_tobool (avm_memcell* m){
	assert(m->type >= 0 && m->type < undef_m);
	return (*toboolFuncs[m->type])(m);
}


char* typeStrings[] = {
	"number",
	"string",
	"bool",
	"table",
	"userfunc",
	"libfunc",
	"nil",
	"undef"
};

unsigned char compare_avm (avm_memcell *rv1,avm_memcell* rv2);
void execute_bool (instruction* instr) {
	
	assert(instr->result.type == label_a);

	avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

	unsigned char result = 0;

	if(rv1->type == undef_m || rv2->type == undef_m){
		printf("'undef' involved in equality!");
	}
    else if(rv1->type == bool_m || rv2->type == bool_m)
			result = (avm_tobool(rv1) == avm_tobool(rv2));
	else if(rv1->type == nil_m || rv2->type == nil_m)
			result = rv1->type == nil_m && rv2->type == nil_m;
	else if(rv1->type != rv2->type)
			printf("%s == %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
	else if(rv1->type == number_m && rv2->type == number_m){
		/* dispatching!!!*/
        result = execute_compare(instr);
		//printf("************%u****************",result);
		/**********************************8*/
	}	
    else{
        if(instr->opcode == jeq_v)
            result = compare_avm(rv1,rv2);  
        else if(instr->opcode == jnoteq_v)
            result = compare_avm(rv1,rv2);
    }	
	if(!executionFinished && result){
		pc = instr->result.val;
	}
}


unsigned char compare_avm(avm_memcell *rv1,avm_memcell* rv2){
    switch(rv1->type){
        case number_m: return number_tobool(rv1) == number_tobool(rv2);
        case string_m: return strcmp(rv1->data.strVal,rv2->data.strVal);
        case table_m: return 1;
        case userfunc_m: return 1;
        case libfunc_m: return 1;
        default : return 0;
    }
}

typedef unsigned char (*bool_func_t)(double x,double y);


unsigned char jeq_impl (double x,double y) {return x==y;}
unsigned char jne_impl (double x,double y) {return x!=y;}
unsigned char jle_impl (double x,double y) {return x<=y;}
unsigned char jge_impl (double x,double y) {return x>=y;}
unsigned char jlt_impl (double x,double y) {return x<y;}
unsigned char jgt_impl (double x,double y) {return x>y;}

/* Bool functions dispatcher */
bool_func_t BoolFuncs[] = {
    jeq_impl,
    jne_impl,
	jle_impl,
	jge_impl,
	jlt_impl,
    jgt_impl
};

unsigned char execute_compare (instruction* instr){
    assert(instr->result.type == label_a);

	avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

	assert(rv1 && rv2);

	if(rv1->type != number_m || rv2->type != number_m){
		printf("not a number in bool!");
		executionFinished = 1;
	}else{
		bool_func_t op = BoolFuncs[instr->opcode - jeq_v];
        return ((*op)(rv1->data.numVal, rv2->data.numVal));		    
	}
    return 0;
}

void execute_jump(instruction *instr) {
    assert(instr->result.type == label_a);
    pc = instr->result.val;
}