#include "../avm.h"
#include "../vm.h"

//extern void avm_error(char* format,...);
extern char* avm_tostring (avm_memcell*);	/*caller frees*/
extern void avm_calllibfunc (char* funcName);
extern void avm_callsaveenvironment (void);
extern void avm_call_functor (avm_table* t);
extern void avm_push_table_arg(avm_table* t); /*latter*/

void execute_call (instruction* instr) {
	avm_memcell* func = avm_translate_operand(&instr->result, &ax);
	assert(func);
	switch(func->type){

		case userfunc_m: {
			avm_callsaveenvironment();
			pc = func->data.funcVal + 1;//gt prosthesame to jump
			assert(pc < AVM_ENDING_PC);
			assert(code[pc].opcode == funcstart_v);
			break;
		}

		case string_m: avm_calllibfunc(func->data.strVal);	
						break;
		case libfunc_m: avm_calllibfunc(func->data.libfuncVal); 
						break;
		case table_m: avm_call_functor(func->data.tableVal);	break;
		
		default: {
			char* s = avm_tostring(func);
			printf("call: cannot bind '%s' to function",s);
			free(s);
			executionFinished = 1;
		}
	}
	
}


void avm_call_functor (avm_table* t){
	cx.type = string_m;
	cx.data.strVal = "()";
	avm_memcell* f = avm_tablegetelem (t, &cx);

	if(!f){
		printf("in calling table: no '()' element found!");
	}else{
		if(f->type == table_m){
			avm_call_functor(f->data.tableVal);
		}else{
			if(f->type == userfunc_a){
				avm_push_table_arg(t);
				avm_callsaveenvironment();
				pc = f->data.funcVal;
				assert(pc < AVM_ENDING_PC && code[pc].opcode == funcstart_v);
			}else{
				printf("in calling table: illegal '()' element value!");
			}
		}
	}
}
unsigned totalActuals = 0;

void avm_dec_top(void){
	if(!top) {	/*stack overflow*/	
		printf("stack overflow");
		executionFinished = 1;
	}else{
		--top;
	}
}

void avm_push_envvalue(unsigned val){
	stack[top].type = number_m;
	stack[top].data.numVal = val;
	avm_dec_top();
}

void avm_callsaveenvironment(void) {
	avm_push_envvalue(totalActuals);
	assert(code[pc].opcode == call_v);		
	avm_push_envvalue(pc + 1);
	avm_push_envvalue(top + totalActuals + 2);
	avm_push_envvalue(topsp);
}



extern userfunc* avm_getfuncinfo(unsigned address);

void execute_funcenter(instruction* instr){
	avm_memcell* func = avm_translate_operand(&instr->result, &ax);
	assert(func);
	assert(pc == func->data.funcVal + 1); /* Func address should match PC */
	printf("%d",func->data.funcVal);
	/* Callee actions here */
	totalActuals = 0;
	userfunc* funcInfo = avm_getfuncinfo(pc - 1);
	topsp = top;
	top = top - funcInfo->localSize;
}


unsigned avm_get_envvalue(unsigned i){
	assert(stack[i].type == number_m);
	unsigned val = (unsigned) stack[i].data.numVal;
	if(stack[i].data.numVal != ((double) val))
        return 0;
	return val;
}

void execute_funcexit(instruction* unused){
	unsigned oldTop = top;
	top 	= avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc 		= avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
	topsp 	= avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

	while(++oldTop <= top){
		avm_memcellclear(&stack[oldTop]);
	}
}






extern void avm_push_table_arg(avm_table* t){
	stack[top].type = table_m;
	avm_table_increfcounter(stack[top].data.tableVal = t);
	++totalActuals;
	avm_dec_top();
}

void execute_pusharg(instruction* instr){
	avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
	assert(arg);

	/* This is actually stack[top] = arg, but we have to use avm_assign*/
	avm_assign(&stack[top], arg);
	++totalActuals;
	avm_dec_top();
}



