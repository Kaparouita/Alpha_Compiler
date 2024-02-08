void avm_initialize (void) {
	avm_initstack();

	avm_registerlibfunc("print", libfunc_print);
	avm_registerlibfunc("typeof", libfunc_typeof);

	/* SAME FOR THE REST LIBS FUNCS*/

	avm_registerlibfunc("input", libfunc_input);
	avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
	avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
	avm_registerlibfunc("totalarguments", libfunc_totalarguments);
	avm_registerlibfunc("argument", libfunc_argument);
	avm_registerlibfunc("strtonum", libfunc_strtonum);
	avm_registerlibfunc("sqrt", libfunc_sqrt);
	avm_registerlibfunc("cos", libfunc_cos);
	avm_registerlibfunc("sin", libfunc_sin);
}


/* totalarguments   -lib func
    @returns the amount of arguments in a function
*/
void libfunc_totalarguments(void){
	/* Get topsp of previous activation record*/
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);

	if(!p_topsp){		/* if 0, its the first activation*/
		avm_error("'totalarguments' called outside a function");
		retval.type = nil_m;
	}else{
		/* Extract the number of actual arguments for the previous activation record*/
		retval.type = number_m;
		retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
	}
}


/*	print   -lib func 
     @displays every argument to the console
*/

void libfunc_print(void){
	unsigned n = avm_totalactuals();
	for(unsigned i = 0;i < n; ++i){
		char* s = avm_tostring(avm_getactual(i));
		puts(s);
		free(s);
	}
}

/*	typeof   -lib func 
     @returns the typeof of the arg
*/
void libfunc_typeof (void) {
	unsigned n = avm_totalactuals();

	if(n != 1){
		avm_error("one argument (not %u) expected in 'typeof'!", n);
	}else{
		/* a library function only has to set the 'retval' register !*/
		avm_memcellclear(&retval);
		retval.type = string_m;
		retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
	}
}

/*  sin     -lib func
    @returns sin(x) 
*/

void libfunc_sin (void){
    unsigned n = avm_totalactuals();

    if(n != 1){
        avm_error("one argument (not %u) expected in 'typeof'!", n);
    }else{ 
        if()
    }
}