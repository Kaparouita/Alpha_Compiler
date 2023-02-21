test: test.o 
	$ (gcc) -o test test.c

test.c: test.l
	flex --outfile=test.c test.l

clean:
	rm -f al.c
	rm -f *.o al