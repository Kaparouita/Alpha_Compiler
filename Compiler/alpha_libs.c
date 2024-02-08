#include "avm.h"
#include "vm.h"


static void avm_initstack(void){
    for (unsigned i = 0; i<AVM_STACKSIZE;i++){
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

void avm_initialize (void) {
	avm_initstack();
	top = AVM_STACKSIZE - 1 - global_vars;
    topsp = top;
	//avm_registerlibfunc("print", libfunc_print);
	/*
	avm_registerlibfunc("typeof", libfunc_typeof);
	avm_registerlibfunc("totalarguments", libfunc_totalarguments);
		//avm_registerlibfunc("argument", libfunc_argument);

	
	avm_registerlibfunc("input", libfunc_input);
	avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
	avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
	avm_registerlibfunc("strtonum", libfunc_strtonum);
	avm_registerlibfunc("sqrt", libfunc_sqrt);
	avm_registerlibfunc("cos", libfunc_cos);
	avm_registerlibfunc("sin", libfunc_sin);
*/
}

unsigned avm_totalactuals(void){
	return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i){
	assert(i < avm_totalactuals());
	return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

/* totalarguments   -lib func
    @returns the amount of arguments in a function
*/
void libfunc_totalarguments(void){
	/* Get topsp of previous activation record*/
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);

	if(!p_topsp){		/* if 0, its the first activation*/
		printf("'totalarguments' called outside a function");
		retval.type = nil_m;
	}else{
		/* Extract the number of actual arguments for the previous activation record*/
		retval.type = number_m;
		retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
	}
}


void libfunc_typeof (void);
/*	print   -lib func 
     @displays every argument to the console
*/

void libfunc_print(void){
	unsigned n = avm_totalactuals();
	for(unsigned i = 0;i < n; ++i){
		 char* s = avm_tostring(avm_getactual(i));
		 printf("%s", s);
		 //printf("p : %f\n", avm_getactual(i)->data.numVal);

         //free(s);
    }
    avm_memcellclear(&retval);
    retval.type = nil_m;
}
typedef void (*lib_func_t)();

lib_func_t tolibFuncs[3] = {
	libfunc_print,
	//libfunc_input,
	//libfunc_objectmemberkeys,
	//libfunc_objecttotalmembers,
	libfunc_totalarguments,
	//libfunc_argument,
	libfunc_typeof,
	//libfunc_strtonum,
	//libfunc_sqrt,
	//libfunc_cos,
	//libfunc_sin,
};

char lib_fuctions[3][20] = {{"print"},{"totalarguments"},{"typeof"}};

lib_func_t avm_getlibraryfunc(char* id){
	for(int i = 0;i<3;i++){
		if(!strcmp(lib_fuctions[i],id))
			return tolibFuncs[i];
	}
	return 0;
}

void avm_calllibfunc(char* id){
	lib_func_t f = avm_getlibraryfunc(id);
	if(!f){
		printf("unsupported lib func '%s' called!", id);
		executionFinished = 1;
	}else{
		// Notice that enter function and exit function are called manually
		//printf("'%s' called!\n", id);
		avm_callsaveenvironment();
		topsp = top;		// enter function seq. No stack locals
		totalActuals = 0;
		(*f)();		// Call library function 
		if(!executionFinished){					// An error may naturally occur inside 
			execute_funcexit((instruction*) 0); // Return sequence 
		}
	}	
}

extern char* typeStrings[];
/*	typeof   -lib func 
     @returns the typeof of the arg
*/
void libfunc_typeof (void) {
	unsigned n = avm_totalactuals();

	if(n != 1){
		printf("one argument (not %u) expected in 'typeof'!", n);
	}else{
		/* a library function only has to set the 'retval' register !*/
		avm_memcellclear(&retval);
		retval.type = string_m;
		retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
	}
}
