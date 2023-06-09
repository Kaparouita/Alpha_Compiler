#define AVM_STACKENV_SIZE	4
avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned	top, topsp;

//reverse translation for constants

double consts_getnumber (unsigned index);
char* consts_getstring (unsigned index);
char* libfuncs_getused (unsigned index);
userfunc* userfuncs_getfunc (unsigned index);


struct avm_table;

struct avm_memcell {
	avm_memcell_t type;

	union {
		double	numVal;
		char*	strVal;
		unsigned char boolVal;
		avm_table*	tableVal;
		unsigned	funcVal;
		char*	libfuncVal;
	} data;
};

enum avm_memcell_t {
	number_m	= 0,
	string_m	= 1,
	bool_m		= 2,
	table_m		= 3,
	userfunc_m	= 4,
	libfunc_m	= 5,
	nil_m		= 6,
	undef_m		= 7
};

avm_memcell* avm_translate_operand (vmarg* arg, avm_memcell* reg){

	switch(arg->type){

		/*Variables*/

		case global_a:	return &stack[AVM_STACKSIZE - 1 - arg->val];
		case local_a:	return &stack[topsp - arg->val];
		case formal_a:	return &stack[tpsp + AVM_STACKENV_SIZE + 1 + arg->val];

		case retval_a:	return &retval;
		


		case number_a: {
			reg->type = number_m;
			reg->data.numVal = consts_getnumber(arg->val);
			return reg;
		}

		case string_a: {
			reg->type =  string_m;
			reg->data.strVal = strdup(consts_getstring(arg->val));
			return reg;
		}

		case bool_a: {
			reg->type = bool_m;
			reg->data.boolVal = arg->val;
			return reg;
		}

		case nil_a: reg->type = nil_m; return reg;

		case userfunc_a: {
			reg->type = userfunc_m;
			/*if function address is directly stored*/
			reg->data.funcVal = arg->val;
			/*if function index table is stored*/
			reg->data.funcVal = userfuncs_getfunc(arg->val)->address;
			return reg;
		}

		case libfunc_a: {
			reg->type = libfunc_m;
			reg->data.libfuncVal = libfuncs_getused(arg->val);
			return reg;
		}

		default: assert(0);

	}	
}

memclear_func_t memclearFuncs[] = {
	0,	/*number*/
	memclear_string,
	0,	/*bool*/
	memclear_table,
	0,	/*userfunc*/
	0,	/*libfunc*/
	0,	/* nil*/
	0,	/*undef*/
};

void avm_memcellclear (avm_memcell* m){
	if(m->type != undef_m){
		memclear_func_t f = memclearFuncs[m->type];
		if(f){
			(*f)(m);
		}
		m->type = undef_m;
	}
}

typedef void (*memclear_func_t)(avm_memcell*);

extern void memclear_string (avm_memcell* m){
	assert(m->data.strVal);
	free(m->data.strVal);
}

extern void memclear_table (avm_memcell* m){
	assert(m->data.tableVal);
	avm_tabledecrefcounter(m->data.tableVal);
} 

extern void avm_warning(char* format/*,...*/);
extern avm_assign(avm_memcell* lv, avm_memcell* rv);

void execute_assign (instruction* instr){
	avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

	assert(lv && (&stack[N-1] >= lv && lv > &stack[top] || lv = &retval));
	assert(rv);

	avm_assign(lv,rv);
}




/*mexri kai slide 18*/

void avm_assign (avm_memcell* lv, avm_memcell* rv){
	if(lv == rv){
		return; /*same cell*/
	}

	if(lv->type == table_m && rv->type == table_m && lv->data.tableVal == rv->data.tableVal){
		return;	/*same table, no need for assign*/
	}

	if(rv->type == undef_m){		/*from undefined rv*/
		avm_warning("assigning from 'undef' content!")
	}

	avm_memcellclear(lv);		/*clear old cell contents*/

	memcpy(lv, rv, sizeof(avm_memcell));	/*in c++ dispatch instead*/

	/*taking care of copied calues or reference counting*/

	if(lv->type == string_m){
		lv->data.strVal = strdup(rv->data.strVal);
	}else{											/*MPOREI NA XW KANEI PATATIA*/
		if(lv->type == table_m){
			avm_tableincrefcounter(lv->data.tableVal);
		}
	}
}

//slide 20

void execute_call (instruction* instr) {
	avm_memcell* func = avm_translate_operand(&instr->result, &ax);
	assert(func);
	switch(func->type){

		case userfunc_m: {
			avm_callsaveenvironment();
			pc = func->data.funcVal;
			assert(pc < AVM_ENDING_PC);
			assert(code[pc].opcode == funcenter_v);
			break;
		}

		case string_m: avm_calllibfunc(func->data.strVal);	break;
		case libfunc_m: avm_calllibfunc(func->data.libVal); break;
		case table_m: avm_call_functor(func->data.tableVal);	break;
		
		default: {
			char* s = avm_tostring(func);
			avm_error("call: cannot bind '%s' to function",s);
			free(s);
			executionFinished = 1;
		}
	}
	
}

extern void avm_error(char* format/*,*/);
extern char* avm_tostring (avm_memcell*);	/*caller frees*/
extern void avm_calllibfunc (char* funcName);
extern void avm_callsaveenvironment (void);
extern void avm_call_functor (avm_table* t);
extern void avm_push_table_arg(avm_table* t); /*latter*/

void avm_call_functor (avm_table* t){
	cx.type = string_m;
	cx.data.strVal = "()";
	avm_memcell* f = avm_tablegetelem (t, &cx);

	if(!f){
		avm_error("in calling table: no '()' element found!");
	}else{
		if(f->type == table_m){
			avm_call_functor(f->data.tableVal);
		}else{
			if(f->type == userfunc_a){
				avm_push_table_arg(t);
				avm_callsaveenvironment();
				pc = f->data.funcVal;
				assert(pc < AVM_ENDING_PC && code[pc].opcode == funcenter_v);
			}else{
				avm_error("in calling table: illegal '()' element value!");
			}
		}
	}
}

unsigned totalActuals = 0;

void avm_dec_top(void){
	if(!top) {	/*stack overflow*/	
		avm_error("stack overflow");
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
	assert(code[pc].opcode == call_v);		/*EDW O DIKOS MAS PROTEINEI MANUAL CHECK?*/
	avm_push_envvalue(pc + 1);
	avm_push_envvalue(top + totalActuals + 2);
	avm_push_envvalue(topsp);
}








extern userfunc* avm_getfuncinfo(unsigned address);

void execute_funcenter(instruction* instr){
	avm_memcell* func = avm_translate_operand(&instr->result, &ax);
	assert(func);
	assert(pc == func->data.funcVal); /* Func address should match PC */

	/* Callee actions here */
	totalActuals = 0;
	userfunc* funcInfo = avm_getfuncinfo(pc);
	topsp = top;
	top = top - funcInfo->localSize;
}



#define AVM_NUMACTUALS_OFFSET + 4
#define AVM_SAVEDPC_OFFSET + 3
#define AVM_SAVEDTOP_OFFSET + 2
#define AVM_SAVEDTOPSP_OFFSET + 1

unsigned avm_get_envvalue(unsigned i){
	assert(stack[i].type = number_m);
	unsigned val = (unsigned) stack[i].data.numVal;
	assert(stack[i].data.numVal == ((double) val));
	return val;
}

void execute_funcexit(instruction* unused){
	unsigned oldTop = top;
	top 	= avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc 		= avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET)
	topsp 	= avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

	while(++oldTop <= top){
		avm_memcellclear(&stack[oldTop]);
	}
}



void avm_calllibfunc(char* id){
	library_func_t f = avm_getlibraryfunc(id);
	if(!f){
		avm_error("unsupported lib func '%s' called!", id);
		executionFinished = 1;
	}else{
		/* Notice that enter function and exit function are called manually*/
		avm_callsaveenvironment();
		topsp = top;		/* enter function seq. No stack locals*/
		totalActuals = 0;
		(*f)();		/* Call library function */
		if(!executionFinished){					/* An error may naturally occur inside */
			execute_funcexit((instruction*) 0); /* Return sequence */
		}
	}	
}





unsigned avm_totalactuals(void){
	return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i){
	assert(i < avm_totalactuals());
	return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}







/* With the following every library function is manually
   added in the VM library funtion resolution map*/

void avm_registerlibfunc(char* id, library_func_t addr);









extern void avm_push_table_arg(avm_table* t){
	stack[top].type = table_m;
	avm_tableincrefcounter(stack[top].data.tableVal = t);
	++totalActuals;
	avm_dec_top;
}

void execute_pusharg(instruction* instr){
	avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
	assert(arg);

	/* This is actually stack[top] = arg, but we have to use avm_assign*/
	avm_assign(&stack[top], arg);
	++totalActuals;
	avm_dec_top();
}

typedef char* (*tostring_func_t)(avm_memcell*);

extern char* number_tostring (avm_memcell*);
extern char* string_tostring (avm_memcell*);
extern char* bool_tostring (avm_memcell*);
extern char* table_tostring (avm_memcell*);
extern char* userfunc_tostring (avm_memcell*);
extern char* libfunc_tostring (avm_memcell*);
extern char* nil_tostring (avm_memcell*);
extern char* undef_tostring (avm_memcell*);


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









#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

typedef double (*arithmetic_func_t)(doublex, double y);

double add_impl (double x, double y) {return x+y;}
double sub_impl (double x, double y) {return x-y;}
double mul_impl (double x, double y) {return x*y;}
double div_impl (double x, double y) {return x/y; /*assert(y != 0);avm_error("y is 0!"); leei se comment error check?*/}
double mod_impl (double x, double y) {
	//assert(y != 0); //diko mou error check
	return ((unsigned) x) % ((unsigned) y);
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

	if(rv1->type != number_m || rv2->type != number_m){
		avm_error("not a number in arithmetic!");
		executionFinished = 1;
	}else{
		arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
		avm_memcellclear(lv);
		lv->type = number_m;
		lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
	}
}





/*		JGE JGT JLE JLT		*/









typedef unsigned char(*tobool_func_t)(avm_memcell*);

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

unsigned char sym_tobool (avm_memcell* m){
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


void execute_jeq (instruction* instr) {
	
	assert(instr->result.type == label_a);

	avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

	unsigned char result = 0;

	if(rv1->type == undef_m || rv2->type = undef_m){
		avm_error("'undef' involved in equality!");
	}else{
		if(rv1->type == bool_m || rv2->type == bool_m){
			result = (avm_tobool(rv1) == avm_tobool(rv2));
		}else{
			if(rv1->type == nil_m || rv2->type == nil_m){
				result = rv1->type == nil_m && rv2->type == nil_m;
			}else{
				if(rv1->type != rv2->type){
					avm_error("%s == %s is illegal!", typeStrings[rv1->type], typeStrings[tv2->type]);
				}else{
					/* Equality Check with deispatching!!!*/

					/**********************************8*/
				}
			}
		}
	}

	if(!executionFinished && result){
		pc = instr->result.val;
	}
}


void execute_newtable (instruction* instr){
	avm_memcell* lv = avm_translate_operand(&instr->result, (&avm_memcell*) 0);
	assert(lv && (&stack[N-1] >= lv && lv > &stack[top] || lv == &retval));

	avm_memcellclear(lv);

	lv->type			= table_m;
	lv->data.tableVal	= avm_tablenew();
	avm_tableincrefcounter(lv->data.tableVal);
}

avm_memcell* avm_tablegetelem (
		avm_table* table,
		avm_memcell* index	
);

void avm_tablesetelem(
		avm_table*	table,
		avm_memcell* index,
		avm_memcell* content
);





void execute_tablegetelem (instruction* instr){
	avm_memcell* lv	= avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* t	= avm_translate_operand(&instr->arg1, (avm_memcell*) 0);
	avm_memcell* i	= avm_translate_operand(&instr->arg2, &ax);

	assert(lv && &stack[N-1] >= lv && lv > &stack[top] || lv == &retval);
	assert(t && &stack[N-1] >= t && t > &stack[top]);
	assert(i);

	avm_memcellclear(lv);
	lv->type = nil_m; /* Default */

	if(t->type != table_m) {
		avm_error("illegal use of type %s as table!", typeStrings[t->type]);
	}else{
		avm_memcell* content = avm_tablegetelem(t->data.tableVal, i);
		if(content){
			avm_assign(lv, content);
		}else{
			char* ts = avm_tostring(t);
			char* is = avm_tostring(i);
			avm_warning("%s[%s] not found!", ts, is);
			free(ts);
			free(is);
		}
	}
}


void execute_tablesetelem (instruction* instr){
	avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);

	assert(t && &stack[N-1] >= t && t > &stack[top]);
	assert(i && c);

	if(t->type != table_m){
		avm_error("illegal use of type %s as table!", typeStrings[t->type]);
	}else{
		avm_tablesetelem(t->data.tableVal, i , c);
	}
}


