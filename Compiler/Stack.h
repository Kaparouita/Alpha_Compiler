#ifndef Stack_H
#define Stack_H
// C program for array implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// A structure to represent a stack
typedef struct Stack {
	int top;
	unsigned capacity;
	int* array;
}Stack;

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack* createStack(unsigned capacity);

// Stack is full when top is equal to the last index
int isFull(struct Stack* stack);
// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack);
// Function to add an item to stack. It increases top by 1
void push(struct Stack* stack, int item);

// Function to remove an item from stack. It decreases top by 1
int pop(struct Stack* stack);

// Function to return the top from stack without removing it
int peek(struct Stack* stack);

#endif /*Stack_H*/