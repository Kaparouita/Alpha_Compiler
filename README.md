# Alpha_Compiler

## Description

This is a compiler for the Alpha language. 
I still need to write a proper description for this project.  
But for now, you can get an idea of how the language looks like by looking at the `Compiler/tests` directory and the `Compiler/scanner.l` file.

## Basic Idea

You first generate instructions from the a file written in the Alpha language or straight from the command line.  
Then you execute the instructions using the Alpha Virtual Machine.

## Generate Instructions

To generate instructions from a file, you can use the following commands:

```bash
make calc
./calc "file.txt"
```
You can check how the instructions look like in the `maou.txt` file.  
Aslo binary.abc is the default binary file.  

To clean the generated files, you can use the following command:

```bash
make clean
```

## Read Binary and Run VM

To read the binary file and run the virtual machine, you can use the following commands:

```bash
make main
./main "binary.abc"
```