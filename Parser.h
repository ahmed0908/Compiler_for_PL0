//Muhammad Ahmed
//Systems Software COP3402, Summer 2017
//Implementation of the Parser


//All the necessary libraries are imported
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeaderFile.h"


//Declaring necessary structs
//Token tokenList[MAX_CODE_LENGTH];
Token currentToken;
symTable symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction MCode[MAX_CODE_LENGTH];

//global variables
char currentProc[identMax];
int procedures[500][2], procPos = 0; //0=level 1= line
int counted = 0, numProcedures = 0;
int varLevel = 0, constLevel = 0, varNum = 0, constNum = 0;
int symTableCounter = 0, tokenTableCounter = 0;
int machineCodeCounter = 0, currentMachineCode = 0, lexLevel = 0;
int lineParser = 1, flag2 = 1;
 
//*********************Function Prototypes*******************************///
//void createTokenList(void);
void programParser(int assemblyFlag);
void runBlock(void);
void constFound(void);
void varFound(void);
void statementFound(void);
void expressionFound(void);
void termFound(void);
void factorFound(void);
void operationFound(void);
void procedureFound(void);
void fetchToken(void);
void voidSyms(int level);
int searchSym(char *name, int level);
void toCode(int OP, int L, int M);
int toInt(char *num);
void pushSymTable(int kind, Token t, int num, int L, int M);
void printMachineCode(int aFlag);
//***********************************************************************/////


//starting the parser implementations
void programParser(int assemblyFlag)
{
	//First creating a token list by reading tokens from the "LexemeTable.txt"
	//createTokenList();
	
	machineCodeFile = fopen("MachineCode.txt", "w");
	
	
	fetchToken(); //grabbing the first token
	runBlock(); //running the whole program block here
	if(currentToken.type != periodsym)
	{
		printf("\n There is an Error: Line %d: Period expected\n", lineParser - 1);
		flag2 = 0;
	}
	
	printMachineCode(assemblyFlag);
	
	fclose(machineCodeFile);
	
}

/* //all the function implementations here
void createTokenList(void)
{
	//declaring variables
	int type;
	int i = 0;
	
	char *tokenSplitter;
	char eachLine[50];
	
	
	while(fgets(eachLine, 50, tableOutputFile))
	{
		tokenSplitter = strtok(eachLine, " ");
		while(tokenSplitter != NULL)
		{
			if(tokenSplitter[0] >= '0' && tokenSplitter[0] <= '9')
			{
				type = atoi(tokenSplitter);
				tokenList[tokenCounter2].type = type;
			}
			else
			{
				strcpy(tokenList[tokenCounter2].name, tokenSplitter);
			}
			tokenSplitter = strtok(NULL, " "); //keep spliting
		}
		tokenCounter2++;
	}
	
 	//fprintf(tableOutputFile,"lexeme      token type\n");
	for( i=0; i<tokenCounter2; i++)
	{
			
		printf("%-11s %d\n", tokenList[i].name, tokenList[i].type);
	}	 
} */

//Fetches the next token to use
void fetchToken()
{
    currentToken = tokenList[tokenTableCounter];
    tokenTableCounter++;
    if(currentToken.type == newlinesym)
	{
        while(currentToken.type == newlinesym)
		{
            lineParser++;
            fetchToken();
        }
    }
}

//The function that deals with the complexity of the whole block
void runBlock()
{
    counted = 0;
    int tempBlockPos = machineCodeCounter, temPos;
    currentMachineCode = 0;

    toCode(7,0,0); //Start.

    while(currentToken.type == constsym || currentToken.type == varsym)
	{
		if(currentToken.type == constsym)
			constFound();
		if(currentToken.type == varsym)
			varFound();
		counted++;
    }

    temPos = currentMachineCode; //Store current MCode pos

    while(currentToken.type == procsym)
        procedureFound();

    MCode[tempBlockPos].M = machineCodeCounter;

    toCode(6,0,temPos + 4);
    statementFound();
    if(currentToken.type != periodsym && currentToken.type == semicolonsym)
	{
        toCode(2,0,0); //return proc
        voidSyms(lexLevel);
    }
    else
        toCode(9,0,3);
}
//deal with consts
void constFound()
{
    Token tempT;
    int returner = 0;
    if(constNum >= 1)
	{
        if(constLevel == lexLevel)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Var or const detected more than once\n");
			flag2 = 0;
        }
    }
    constNum++;
    constLevel = lexLevel;
    do
	{
        fetchToken();
        if(currentToken.type != identsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("const, var, and procedure must be followed by an identifier\n");
			flag2 = 0;
        }
        returner = searchSym(currentToken.name, lexLevel);
        if(returner != -1 && symbolTable[returner].level == lexLevel)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Ident name declared twice\n");
			flag2 = 0;
        }

        strcpy(tempT.name, currentToken.name); //copy into temp

        fetchToken();
        if(currentToken.type != eqlsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("""="" expected after const declaration\n");
			flag2 = 0;
        }

        fetchToken();
        if(currentToken.type != numbersym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Number expected after ""="" with const\n");
			flag2 = 0;
        }

        pushSymTable(1, tempT, constLevel, -5, toInt(currentToken.name));
        fetchToken();
    } while(currentToken.type == commasym);

    if(currentToken.type != semicolonsym)
	{
        printf("\nError: Line:%d,  :: ",lineParser - 1);
        printf("Semicolon needed between statements.\n");
		flag2 = 0;
    }

    fetchToken();
}
//Deal with vars
void varFound()
{
    int returner = 0;
    if(varNum >= 1)
	{
        if(varLevel == lexLevel)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Var or const detected more than once\n");
			flag2 = 0;
        }
    }
    varNum++;
    varLevel = lexLevel;

    do
	{
        fetchToken();
        if(currentToken.type != identsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("const, var, and procedure must be followed by an identifier\n");
			flag2 = 0;
        }//Test to see if it exists already

        returner = searchSym(currentToken.name, lexLevel);
        if(returner != -1 && symbolTable[returner].level == lexLevel)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Ident name declared twice\n");
			flag2 = 0;
        }
        pushSymTable(2, currentToken, lexLevel, currentMachineCode+4, 0);
        fetchToken();
    } while(currentToken.type == commasym);

    if(currentToken.type != semicolonsym)
	{
        printf("\nError: Line:%d,  :: ",lineParser - 1);
        printf("Semicolon needed between statements.\n");
		flag2 = 0;
    }

    fetchToken();
}
//Deal with a statement line
void statementFound()
{
    int symPos, identPos, tempBPos, temPos, temPos2;

    if(currentToken.type == identsym)
	{
        symPos = searchSym(currentToken.name, lexLevel);

        if(symPos == -1)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Identifier '%s': ", currentToken.name);
            printf("Undeclared identifier detected\n");
			flag2 = 0;
        }
        else if(symbolTable[symPos].kind == 1)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Assignment to constants and procedures not allowed\n");
			flag2 = 0;
        }

        identPos = symbolTable[symPos].addr;

        fetchToken();
        if(currentToken.type != becomessym)
		{
            if(currentToken.type == eqlsym)
			{
               printf("\nError: Line:%d,  :: ",lineParser);
               printf("Use "":="", not ""=""\n");
			   flag2 = 0;
            }
            else
			{
                printf("\nError: Line:%d,  :: ",lineParser);
                printf(""":="" expected after identifier\n");
				flag2 = 0;
            }
        }

        fetchToken();
        expressionFound();

        if(currentToken.type != semicolonsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser - 1);
            printf("Semicolon needed between statements.\n");
			flag2 = 0;
        }

        toCode(4, lexLevel-symbolTable[symPos].level, identPos);
    }
    else if(currentToken.type == callsym)
	{
        fetchToken();

        if(currentToken.type != identsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Ident expected after ""call""\n");
			flag2 = 0;
        }

        symPos = searchSym(currentToken.name, lexLevel);

        if(symPos == -1)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Identifier '%s': ", currentToken.name);
            printf("Undeclared identifier detected\n");
			flag2 = 0;
        }
        else if(symbolTable[symPos].kind == 1)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Assignment to constants and procedures not allowed\n");
			flag2 = 0;
        }

        fetchToken();

        toCode(5, lexLevel, symbolTable[symPos].addr);

    }
    else if(currentToken.type == beginsym)
	{
        fetchToken();
        statementFound();

        while(currentToken.type == semicolonsym)
		{
            fetchToken();
            statementFound();
        }
        if(currentToken.type != endsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Cannot begin new statement with this symbol\n");
			flag2 = 0;
        }

        fetchToken();
    }
    else if(currentToken.type == ifsym)
	{
        fetchToken();
        operationFound();
        if(currentToken.type != thensym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("""then"" expected after ""if""\n");
			flag2 = 0;
        }

        fetchToken();
        tempBPos = machineCodeCounter;

        toCode(8,0,0);

        statementFound();
        MCode[tempBPos].M = machineCodeCounter;

        fetchToken();

        if(currentToken.type != elsesym)
		{
            tokenTableCounter--;
            tokenTableCounter--;
            currentToken.type = tokenList[tokenTableCounter].type;
            strcpy(currentToken.name,tokenList[tokenTableCounter].name);
            while(currentToken.type == newlinesym)
			{
                tokenTableCounter--;
                currentToken.type = tokenList[tokenTableCounter].type;
                strcpy(currentToken.name,tokenList[tokenTableCounter].name);
            }
        }

        if(currentToken.type == elsesym)
		{
            MCode[tempBPos].M = machineCodeCounter+1;

            tempBPos = machineCodeCounter;

            toCode(7,0,0);

            fetchToken();
            statementFound();
            MCode[tempBPos].M = machineCodeCounter;
        }
    }
    else if(currentToken.type == whilesym)
	{
        temPos = machineCodeCounter;

        fetchToken();
        operationFound();

        temPos2 = machineCodeCounter;

        toCode(8,0,0);

        if(currentToken.type != dosym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("""do"" expected after ""while""\n");
			flag2 = 0;
        }

        fetchToken();
        statementFound();

        toCode(7,0,temPos);

        MCode[temPos2].M = machineCodeCounter;
    }
    else if(currentToken.type == readsym)
	{
        fetchToken();

        if(currentToken.type == identsym)
		{
            symPos = searchSym(currentToken.name, lexLevel);
            if(symPos == -1)
			{
                printf("\nError: Line:%d,  :: ",lineParser);
                printf("Identifier '%s': ", currentToken.name);
                printf("Undeclared identifier detected\n");
				flag2 = 0;
            }
            fetchToken();

            toCode(9,0,2); //read from screen

            toCode(4,0,symbolTable[symPos].addr); //increment mcode
        }
    }
    else if(currentToken.type == writesym)
	{
        fetchToken();

        if(currentToken.type == identsym)
		{
            symPos = searchSym(currentToken.name, lexLevel);
            if(symPos == -1)
			{
                printf("\nError: Line:%d,  :: ",lineParser);
                printf("Identifier '%s': ", currentToken.name);
                printf("Undeclared identifier detected\n");
				flag2 = 0;
            }
            fetchToken();
            if(symbolTable[symPos].kind == 1)
                toCode(1,0,symbolTable[symPos].val); //if constant
            else
                toCode(3,0,symbolTable[symPos].addr); //read from screen

            toCode(9,0,1); //output
        }
        else
		{
            printf("\nError: Line:%d,  :: ",lineParser);
			printf("Ident expected after ""call""\n");
			flag2 = 0;
        }
    }
}
//Expression work
void expressionFound()
{
    int thisOp;

    if(currentToken.type == plussym || currentToken.type == minussym)
	{
        thisOp = currentToken.type;

        if(thisOp == minussym)
		{
            fetchToken();
            termFound();
            toCode(2,0,1);
        }
    }
    else
	{
        termFound();
    }

    while(currentToken.type == plussym || currentToken.type == minussym)
	{
        thisOp = currentToken.type;
        fetchToken();
        termFound();

        if(thisOp == plussym)
            toCode(2,0,2);
        else
            toCode(2,0,3);
    }
}
//terms and things
void termFound()
{
    int thisOp;

    factorFound();

    while(currentToken.type == multsym || currentToken.type == slashsym)
	{
        thisOp = currentToken.type;
        fetchToken();
        factorFound();

        if(thisOp == multsym)
            toCode(2,0,4);
        else
            toCode(2,0,5);
    }
}
//factors and fun
void factorFound()
{
    int symPos;

    if(currentToken.type == identsym)
	{
        symPos = searchSym(currentToken.name, lexLevel);

        if(symPos == -1)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Identifier '%s': ", currentToken.name);
            printf("Undeclared identifier detected\n");
			flag2 = 0;
        }

        if(symbolTable[symPos].kind == 1)
            toCode(1, 0, symbolTable[symPos].val);
        else
            toCode(3, lexLevel-symbolTable[symPos].level, symbolTable[symPos].addr);

        fetchToken();
    }
    else if(currentToken.type == numbersym)
	{
        toCode(1, 0, toInt(currentToken.name));
        fetchToken();
    }
    else if (currentToken.type == lparentsym)
	{
        fetchToken();
        expressionFound();

        if(currentToken.type != rparentsym)
		{
            printf("\nError: Line:%d,  :: ",lineParser);
            printf("Unclosed parenthesis detected\n");
			flag2 = 0;
        }

        fetchToken();
    }
    else
	{
        printf("\nError: Line:%d,  :: '%s'",lineParser,currentToken.name);
        printf("Cannot begin new statement with this symbol\n");
		flag2 = 0;
    }
}
//Mathematical operation
void operationFound()
{
    int thisOp;
    if(currentToken.type == oddsym)
	{
        toCode(2,0,6);
        fetchToken();
        expressionFound();
    }
    else
	{
        expressionFound();
        thisOp = currentToken.type;

        switch (currentToken.type) 
		{
            case becomessym:
                printf("\nError: Line:%d,  :: ",lineParser);
                printf("Use ""="", not "":=""\n");
				flag2 = 0;
                break;
            case eqlsym:
                thisOp = 8;
                break;
            case neqsym:
                thisOp = 9;
                break;
            case lessym:
                thisOp = 10;
                break;
            case leqsym:
                thisOp = 11;
                break;
            case gtrsym:
                thisOp = 12;
                break;
            case geqsym:
                thisOp = 13;
                break;
            default:
                printf("\nError: Line:%d,  :: ",lineParser);
                printf("Relational operator expected\n");
				flag2 = 0;
                break;
        }
        fetchToken();
        expressionFound();
        toCode(2,0,thisOp);
    }
}
//deal with procedure declarations
void procedureFound()
{
    numProcedures++;
    fetchToken();
    if(currentToken.type != identsym)
	{
        printf("\nError: Line:%d,  :: ",lineParser);
        printf("const, var, and procedure must be followed by an identifier\n");
		flag2 = 0;
    }
    strcpy(currentProc,currentToken.name);

    pushSymTable(3, currentToken, lexLevel, machineCodeCounter, -1);

    lexLevel++;
    if(lexLevel > MAX_LEXI_LEVELS)
	{
        printf("\nError: :: ");
        printf("Compiler has run out of memory\n");
		flag2 = 0;
    }
    numProcedures++;
    varNum = 0;

    fetchToken();
    if(currentToken.type != semicolonsym)
	{
        printf("\nError: Line:%d,  :: ",lineParser);
		printf("Semicolon needed between statements.\n");
		flag2 = 0;
    }

    fetchToken();
    runBlock(); //run for the proc's insides
    lexLevel--;

    if(currentToken.type != semicolonsym)
	{
        printf("\nError: Line:%d,  :: ",lineParser);
        printf("Semicolon needed between statements.\n");
		flag2 = 0;
    }
    strcpy(currentProc," ");
    fetchToken();
}
//find a variable in the symbol table
int searchSym(char *name, int level)
{
    int i;
    while(level != -1)
	{
        for(i=symTableCounter-1; i >= 0; i--)
		{
            if((strcmp(name,symbolTable[i].name) == 0) && (symbolTable[i].addr != -1) && (symbolTable[i].level == level))
			{
                return i;
            }
        }
        level--;
    }
    return -1; //not found :(
}
//mark old syms as invalid with -1
void voidSyms(int level)
{
    int i;
    for(i=symTableCounter-1; i >= 0; i--)
	{
        if(symbolTable[i].level == level && symbolTable[i].kind != 3 && symbolTable[i].addr != -1)
		{
            symbolTable[i].addr = -1;
        }
    }
}
//Add to sym table
void pushSymTable(int kind, Token t, int L, int M, int num)
{
    symbolTable[symTableCounter].kind = kind;
    strcpy(symbolTable[symTableCounter].name,t.name);
    symbolTable[symTableCounter].level = L;
    symbolTable[symTableCounter].addr = M;
    if(kind == 1)
        symbolTable[symTableCounter].val = num;
    else if (kind == 2)
        currentMachineCode++;
    else if (kind == 3)
	{
        procedures[procPos][0] = M;
        procedures[procPos][1] = L+1;
        procPos++;
    }
    symTableCounter++;
}

//convert to integer
int toInt(char *num)
{
    int returner = 0, i = 0;
    while(num[i] != '\0')
	{
        returner *= 10;
        returner += num[i] - '0';
        i++;
    }
    return returner;
}
//push the code
void toCode(int OP, int L, int M)
{
    MCode[machineCodeCounter].OP = OP;
    MCode[machineCodeCounter].M = M;
    MCode[machineCodeCounter].L = L;
    machineCodeCounter++;
}

void printMachineCode(int aFlag)
{
    int i;
	
	//printf("========================\nGenerated Machine Code\n========================\n");
	if(flag2)
	{	
		printf("\nThe program is syntatically correct\n\n");
		for(i=0; i< machineCodeCounter; i++)
		{
			fprintf(machineCodeFile,"%d %d %d\n",MCode[i].OP, MCode[i].L, MCode[i].M);
			if(aFlag)
			{
				printf("%d %d %d\n",MCode[i].OP, MCode[i].L, MCode[i].M);
			}
		}
		printf("\n");
	}
}
