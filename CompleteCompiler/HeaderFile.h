//Muhammad Ahmed
//Systems Software COP3402, Summer 2017


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Macros defined constants
#define MAX_STACK_HEIGHT 2000
#define MAX_SYMBOL_TABLE_SIZE 2000
#define MAX_CODE_LENGTH 2000
#define MAX_LEXI_LEVELS 100
#define identMax 11
#define numMax 11

#ifndef GLB
#define GLB

typedef enum{
	nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym,
	oddsym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym,
	rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym,
	ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
	readsym, elsesym, errsym, newlinesym
} token;

//more comments. These global arrays will be later for te execution of the program
char *opcodeKeyword[] = {"FCH", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO"};
enum opcode{FCH, LIT, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO};
enum operation{RET, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ};

 //Lex table
typedef struct tokens{
    int type;
    char name[identMax];
} Token;
Token tokenList[MAX_CODE_LENGTH];

//symbol table
typedef struct symbol {
    int kind; // const = 1, var = 2, proc = 3
    char name[12]; // name up to 11 chars
    int val; // number (ASCII value)
    int level; // L level
    int addr; // M address
} symTable;


//Struct of 3 variables have been used to work with the format of the PL/0 assembly language
typedef struct instr{
	int OP; //For constants, you must store kind, name and value.
	int L; //For variables, you must store kind, name, L and M.
	int M; //For procedures, you must store kind, name, L and M.
} instruction;


//all the file pointers here defined globally here
//writing input and output file here
FILE *sourceCodeFile = NULL;
FILE *listOutputFile = NULL;
FILE *tableOutputFile = NULL;
FILE *machineCodeFile = NULL;
FILE *stackAssemblyFile =NULL;


#endif // GLB