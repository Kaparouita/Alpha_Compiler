#include "../avm.h"
#include "../vm.h"


void avm_assign(avm_memcell* lv, avm_memcell* rv);

void execute_assign (instruction* instr){

	avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv == &retval));
	assert(rv);

	
	avm_assign(lv,rv);
}




/*mexri kai slide 18*/

void avm_assign(avm_memcell* lv, avm_memcell* rv){
	if(lv == rv){
		return; /*same cell*/
	}

	if(lv->type == table_m && rv->type == table_m && lv->data.tableVal == rv->data.tableVal){
		return;	/*same table, no need for assign*/
	}

	if(rv->type == undef_m){		/*from undefined rv*/
		printf("assigning from 'undef' content!");
	}

	avm_memcellclear(lv);		/*clear old cell contents*/
//printf("hi %2.f \n",rv->data.numVal);
	memcpy(lv, rv, sizeof(avm_memcell));	/*in c++ dispatch instead*/

	/*taking care of copied calues or reference counting*/

	if(lv->type == string_m){
		lv->data.strVal = strdup(rv->data.strVal);
	}else if(lv->type == table_m){
			avm_table_increfcounter(lv->data.tableVal);
	}
}

void execute_nop (instruction *){

}
