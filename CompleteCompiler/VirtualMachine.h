//Muhammad Ahmed
//Systems Software COP3402, Summer 2017
//Implementation of the virtual machine


//All the necessary libraries are imported
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeaderFile.h"


//initial values for PL/0 CPU registers
int BP = 1;
int SP = 0;
int PC = 0;

int stackAssembly[MAX_STACK_HEIGHT] = {0};
instruction codeAssembly[MAX_CODE_LENGTH];
instruction IR;
int codeLength = 0, callCounter = 0;
FILE *inputFile = NULL;


//all the function prototypes 
void virtualMachine(int virtualMachineFlag); 
void processMachineCodeFile(int vmFlag);
void printCodeAssembly(int vmFlag);
void processCodeAssembly(int vmFlag);
void printStackAssembly(int vmFlag);
void initiateCycle(void);
void executeCycle(void);
void runOperation(void);
int base(int level, int base);



//Implementation of the virtual machine
void virtualMachine(int virtualMachineFlag)
{
	//opening the file
	inputFile = fopen("MachineCode.txt", "r");
	stackAssemblyFile =fopen("StackAssembly.txt", "w");
	
	if(inputFile == NULL)
		return;
	
	//calling function to process the file
	processMachineCodeFile(virtualMachineFlag);
	//calling a fuction which prints the assembly code
	printCodeAssembly(virtualMachineFlag);
	//calling a function that process the code assembly
	processCodeAssembly(virtualMachineFlag);
	
	fclose(inputFile);
	fclose(stackAssemblyFile);
	
	return;
}

//processing file
void processMachineCodeFile(int vmFlag)
{
	//declaring variables
	int index = 0, OP, L, M;
	
	//reading integers format of assembly code from the input text file
	while(fscanf(inputFile,"%d",&OP) != EOF)
	{
		if(index > MAX_CODE_LENGTH)
		{
			break;
		}
		
		fscanf(inputFile, "%d", &L);
        fscanf(inputFile, "%d", &M);

        codeAssembly[index].OP = OP;
        codeAssembly[index].L = L;
        codeAssembly[index].M = M;

        index++;
	}
	codeLength = index;
	return;
}

//printing assembly code to th output file
void printCodeAssembly(int vmFlag)
{
	//declaring variables
	int index;
	
	//how do we solve certain problems using some different situations here to 
	//allocate some memory here to solve certain issues
	
	
	fprintf(stackAssemblyFile,"Line OP   L   M\n");
	if(vmFlag)
	{
		printf("Line OP   L   M\n");
	}
	
	for(index = 0; index < codeLength; index++)
	{
		fprintf(stackAssemblyFile," %3d %s %2d %3d\n", index, opcodeKeyword[codeAssembly[index].OP], codeAssembly[index].L, codeAssembly[index].M);
		if(vmFlag)
		{
			printf(" %3d %s %2d %3d\n", index, opcodeKeyword[codeAssembly[index].OP], codeAssembly[index].L, codeAssembly[index].M);
		}
	}
	
	fprintf(stackAssemblyFile,"\n\n");
	fprintf(stackAssemblyFile,"                pc  bp  sp  stack\nInitial values  %2d   %d  %2d\n", PC, BP, SP);
	if(vmFlag)
	{
		printf("\n\n");
		printf("                pc  bp  sp  stack\nInitial values  %2d   %d  %2d\n", PC, BP, SP);
	}
	
	return;
}

//processiong code assembly
void processCodeAssembly(int vmFlag)
{
	//declaring variables
	 while (BP > 0)
	 {
        if (PC < codeLength)
		{
            fprintf(stackAssemblyFile,"%3d %s %2d %3d ", PC, opcodeKeyword[codeAssembly[PC].OP], codeAssembly[PC].L, codeAssembly[PC].M);
			if(vmFlag)
			{
				printf("%3d %s %2d %3d ", PC, opcodeKeyword[codeAssembly[PC].OP], codeAssembly[PC].L, codeAssembly[PC].M);
			}
		
			//calling a function that initiates cycle
			initiateCycle();
			executeCycle();
			
			 fprintf(stackAssemblyFile,"%3d %3d %3d  ", PC, BP, SP);
			 if(vmFlag)
			 {
				printf("%3d %3d %3d  ", PC, BP, SP);
			 }
			 
			 //call a function
			 printStackAssembly(vmFlag);
			 
			 fprintf(stackAssemblyFile, "\n");
			 if(vmFlag)
			 {
				printf("\n");
			 }
		}
		else 
			return;
	 }
}

//function that initiates a instruction
void initiateCycle(void)
{
	IR = codeAssembly[PC];
	PC++;
}

//a funtion used to execute cycle
void executeCycle(void)
{
	//declaring variables
	int temp;
	
	//using the functionality of "if" statements to execute the instuction
	//push literal on the stack
	if(IR.OP == LIT)
	{
		SP++;
		stackAssembly[SP] = IR.M;
	}
	//do a certain operation based on the on the offset
	else if(IR.OP == OPR)
	{
		//function called to do specific operation based on the instruction
		runOperation();
	}
	//The LOD instruction will push on the stack
	else if(IR.OP == LOD)
	{
		SP++;
		if(IR.L != 0)
			stackAssembly[SP] = stackAssembly[base(IR.L, BP) + IR.M];
		else
			stackAssembly[SP] = stackAssembly[base(IR.L, BP) + IR.M];
	}
	//The store instruction will pop off the value and store at offset
	else if(IR.OP == STO)
	{
		if(IR.L != 0)
			stackAssembly[base(IR.L, BP) + IR.M] = stackAssembly[SP];
		else
			stackAssembly[base(IR.L, BP) + IR.M] = stackAssembly[SP];
		SP--;
	}
	//the instruction calling the procedure 
	else if(IR.OP == CAL)
	{
		callCounter++;
		stackAssembly[SP + 1] = 0; //subroutine return value
		stackAssembly[SP + 2] = base(IR.L, BP); //This is a static link
		stackAssembly[SP + 3] = BP; //This is a dynamic link
		stackAssembly[SP + 4] = PC; //return address 
		BP = SP + 1;
		PC = IR.M;
	}
	//the instruction INC increment stack pointer
	else if(IR.OP == INC)
	{
		SP = SP + IR.M;
	}
	//The JMP instruction will jump to the instruction at offset M
	else if(IR.OP == JMP)
	{
		PC = IR.M;
	}
	//The instruction JPC will jump to instruction if the top of the stack is 0
	else if(IR.OP == JPC)
	{
		if(stackAssembly[SP] == 0)
		{
			PC = IR.M;
			SP--;
		}
		else
			SP--;
	}
	else if(IR.OP == SIO)
	{
		if(IR.M == 1)
		{
			printf("****************\n Display: %d\n**************\n", stackAssembly[SP]);
			SP--;
		}
		else if(IR.M == 2)
		{
			printf("************\nInput Value: ");
			scanf("%d", &temp);
			printf("************\n");
			SP++;
			stackAssembly[SP] = temp;
		}
		else
		{
			BP = 0;
			PC = 0;
			SP = 0;
			return;
		}
	}
	else
		return;
}

//It will do the operation based on the offset M
void runOperation(void)
{
	//declaring variables
	if(IR.M == RET)
	{
		callCounter--;
		SP = BP - 1;
		PC = stackAssembly[SP + 4];
		BP = stackAssembly[SP + 3];
	}
	else if(IR.M == NEG)
	{
		stackAssembly[SP] = stackAssembly[SP] * -1;
	}
	else if(IR.M == ADD)
	{
		SP--;
		stackAssembly[SP] = stackAssembly[SP] + stackAssembly[SP + 1];
	}
	else if(IR.M == SUB)
	{
		SP--;
		stackAssembly[SP] = stackAssembly[SP] - stackAssembly[SP + 1];
	}
	else if(IR.M == MUL)
	{
		SP--;
		stackAssembly[SP] = stackAssembly[SP] * stackAssembly[SP + 1];
	}
	else if(IR.M == DIV)
	{
		SP--;
		stackAssembly[SP] = stackAssembly[SP] / stackAssembly[SP + 1];
	}
	else if(IR.M == MOD)
	{
		SP--;
		stackAssembly[SP] = stackAssembly[SP] % stackAssembly[SP + 1];
	}
	else if(IR.M == EQL)
	{
		SP--;
		if(stackAssembly[SP] == stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;
	}
	else if(IR.M == NEQ)
	{
		SP--;
		if(stackAssembly[SP] != stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;
	}
	else if(IR.M == LSS)
	{
		SP--;
		if(stackAssembly[SP] < stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;	
	}
	else if(IR.M == LEQ)
	{
		SP--;
		if(stackAssembly[SP] <= stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;
	}
	else if(IR.M == GTR)
	{
		SP--;
		if(stackAssembly[SP] > stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;
	}
	else if(IR.M == GEQ)
	{
		SP--;
		if(stackAssembly[SP] >= stackAssembly[SP + 1])
			stackAssembly[SP] = 1;
		else
			stackAssembly[SP] = 0;
	}
	else
		return;
}

//function that prints each stack in the stackAssemblyFile
void printStackAssembly(int vmFlag)
{
	int this_BP = BP;
	int num_BPs = 1;
	int BPs[MAX_LEXI_LEVELS];
	int i = 1;
	BPs[0] = 1;

	while (this_BP > 1)
	{
        BPs[i++] = this_BP;//Set to pos i in BP array
        this_BP = stackAssembly[this_BP + 2];//Advance to previous BP number
	}
    num_BPs = i-1;

	for(i = 1; i <= SP; i++) 
	{
		if (i == BPs[num_BPs] && i != 1) 
		{
			fprintf(stackAssemblyFile,"| ");
			if(vmFlag)
			{
				printf("| ");
			}
			
			num_BPs--;
		}
	
		fprintf(stackAssemblyFile,"%d ", stackAssembly[i]);
		if(vmFlag)
		{
			printf("%d ", stackAssembly[i]);
		}
	}
	return;
}

//other helper function
int base(int level, int base)
{
	while(level > 0)
	{
		base = stackAssembly[base + 1];
		level--;
	}
	return base;
}

