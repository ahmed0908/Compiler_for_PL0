//Muhammad Ahmed
//Systems Software COP3402, Summer 2017
//The main program here

//Importin all the header files to run the compiler 
#include "Scanner.h"
#include "Parser.h"
#include "VirtualMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//writing a main function 
int main(int argc, char **argv)
{
	//declaring variables
	int i = 0; 
	int lexemeFlag = 0;
	int assemblyFlag = 0; 
	int virtualMachineFlag = 0;
	
 	 if(argc < 2)
	 {
		 printf("Enter a txt file in the second argument \n");
		 return 0;
	 } 
		
	if(argc > 2)
	{
		for(i = 2; i < argc; i++)
		{
			if(strcmp(argv[i], "-l") == 0)
				lexemeFlag = 1;
			else if(strcmp(argv[i], "-a") == 0)
				assemblyFlag = 1;
			else if(strcmp(argv[i], "-v") == 0)
				virtualMachineFlag = 1;
			else
				continue;
		}
	}
	
	//calling the function to start the programScanner
	programScanner(argv[1], lexemeFlag);
	//Starting the parser
	programParser(assemblyFlag);
	//starting the virtual machine implementation heres
	virtualMachine(virtualMachineFlag); 
	
	
	return 1;
}