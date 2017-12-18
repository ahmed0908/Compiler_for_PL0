//Muhammad Ahmed
//Systems Software COP3402, Summer 2017
//Implementation of the Scanner


//All the necessary libraries are imported
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeaderFile.h"


//declaring global variables
int tokenCounter = 0;
int lineScanner = 1;
int flag = 1;


//*******************************Function prototypes*****************
void programScanner(char nameTextFile[], int lexemeFlag);
void executingScanner(void);
void printLexemeTable( int lFlag);
void printLexemeList( int lFlag);
//********************************Function prototypes*****************


//implementation of the function
void programScanner(char nameTextFile[], int lexemeFlag)
{
	sourceCodeFile = fopen(nameTextFile, "r");
	tableOutputFile = fopen("LexemeTable.txt", "w");
	listOutputFile = fopen("LexemeList.txt", "w");
	
	if(sourceCodeFile == NULL)
	{
		printf("File has not been read read\n");
		return;
	}
	//calling functions to get this parser  working
	executingScanner();
	printLexemeTable(lexemeFlag);
	printLexemeList(lexemeFlag);
	
	fclose(sourceCodeFile);
	fclose(tableOutputFile);
	fclose(listOutputFile);
}

//the implementation scanner functions
void executingScanner(void)
{
	//declaring variables
	char numHolder[numMax];
	char varHolder[identMax];
	char scanner;
	int  temp = 0;
	int counter = 0;
	
	// scanner = fgetc(sourceCodeFile);
	// printf("%c\n", scanner);
	scanner = fgetc(sourceCodeFile);
	while(scanner != EOF)
	{
		
	 	if(scanner == '/') //dealing with the comments
		{
			
			scanner = fgetc(sourceCodeFile);
			if(scanner == '*')
			{
				while((scanner = fgetc(sourceCodeFile)) != '*')
				{
					if(scanner == '\n')
					{
						tokenList[tokenCounter++].type = newlinesym;
						lineScanner++;
					}
				}
				
				scanner = fgetc(sourceCodeFile);
				if(scanner == '/' )
				{
					scanner = fgetc(sourceCodeFile);
				}
			}
			scanner = fgetc(sourceCodeFile);
		}
		
		else if( scanner == '\n' || scanner == ';') //dealing with line and ;
		{
			if(scanner == '\n')
			{
				
				tokenList[tokenCounter++].type = newlinesym;
				lineScanner++;
			}
			if(scanner == ';')
			{
				
				strcpy(tokenList[tokenCounter].name, ";");
				tokenList[tokenCounter++].type = semicolonsym;
			}
			scanner = fgetc(sourceCodeFile);
		}
		
		else if((scanner >= '!' && scanner <= '/') || (scanner >= ':' && scanner <= '?'))
		{
			switch(scanner)
			{
				case '+':
					strcpy(tokenList[tokenCounter].name, "+");
					tokenList[tokenCounter++].type = plussym;
					break;
				case '-':
					strcpy(tokenList[tokenCounter].name, "-");
					tokenList[tokenCounter++].type = minussym;
					break;
				case '*':
					strcpy(tokenList[tokenCounter].name, "*");
					tokenList[tokenCounter++].type = multsym;
					break;
				case '/':
					strcpy(tokenList[tokenCounter].name, "/");
					tokenList[tokenCounter++].type = slashsym;
					break;
				case '=':
					strcpy(tokenList[tokenCounter].name, "=");
					tokenList[tokenCounter++].type = eqlsym;
					break;
				case ',':
					strcpy(tokenList[tokenCounter].name, ",");
					tokenList[tokenCounter++].type = commasym;
					break;
				case '.':
					strcpy(tokenList[tokenCounter].name, ".");
					tokenList[tokenCounter++].type = periodsym;
					break;
				case '>':
					scanner = fgetc(sourceCodeFile);
					if(scanner == '=')
					{
						strcpy(tokenList[tokenCounter].name, ">=");
						tokenList[tokenCounter++].type = geqsym;
					}
					else
					{
						strcpy(tokenList[tokenCounter].name, ">");
						tokenList[tokenCounter++].type = gtrsym;
					}
					break;
				case '<':
					scanner = fgetc(sourceCodeFile);
					if(scanner == '=')
					{
						strcpy(tokenList[tokenCounter].name, "<=");
						tokenList[tokenCounter++].type = leqsym;
					}
					else if(scanner == '>')
					{
						strcpy(tokenList[tokenCounter].name, "<>");
						tokenList[tokenCounter++].type = neqsym;
					}
					else
					{
						strcpy(tokenList[tokenCounter].name, "<");
						tokenList[tokenCounter++].type = lessym;
					}
					break;
				case '(':
					strcpy(tokenList[tokenCounter].name, "(");
					tokenList[tokenCounter++].type = lparentsym;
					break;
				case ')':
					strcpy(tokenList[tokenCounter].name, ")");
					tokenList[tokenCounter++].type = rparentsym;
					break;
				case ':':
					scanner = fgetc(sourceCodeFile);
					if(scanner == '=')
					{
						strcpy(tokenList[tokenCounter].name, ":=");
						tokenList[tokenCounter++].type = becomessym;
					}
					else
					{
						printf("Error: Line:%d :: ", lineScanner);
						printf("invalid symbol\n");
						flag = 0;
					}
					break;
				default:
					printf("Error: Line:%d :: ", lineScanner);
					printf("invalid symbol\n");
					flag = 0;
			}
			scanner = fgetc(sourceCodeFile);
		}
		
		else if((scanner >= 'A' && scanner <= 'Z') || (scanner >= 'a' && scanner <= 'z'))
		{
			counter = 0;
			varHolder[counter++] = scanner;
			while((scanner = fgetc(sourceCodeFile)) > ' ')
			{
				
				if(counter < identMax -1)
				{
					if((scanner >= '0' && scanner <= '9') || (scanner >= 'a' && scanner <= 'z'))
					{
						varHolder[counter++] = scanner;
					}
					else
					{
						break;
					}
					
				}
				else
				{
					printf("Error: Line:%d :: ", lineScanner);
					printf("the variable is too long\n");
					flag = 0;
					break;
				}
			}
			varHolder[counter] = '\0';
			
			//take several cases in thi manner
			if(strcmp(varHolder, "begin") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "begin");
                tokenList[tokenCounter++].type = beginsym;
			}
            else if(strcmp(varHolder, "call") == 0)
			{   
				strcpy(tokenList[tokenCounter].name, "call");
                tokenList[tokenCounter++].type = callsym;
            }
            else if(strcmp(varHolder, "const") == 0)
			{   
				strcpy(tokenList[tokenCounter].name, "const");
                tokenList[tokenCounter++].type = constsym;
            }
            else if(strcmp(varHolder, "var") == 0)
			{   
				strcpy(tokenList[tokenCounter].name, "var");
                tokenList[tokenCounter++].type = varsym;
            }
            else if(strcmp(varHolder, "do") == 0)
			{   
				strcpy(tokenList[tokenCounter].name, "do");
                tokenList[tokenCounter++].type = dosym;
            }
 			else if (strcmp(varHolder, "else") == 0)
			{
                temp = 1;
                while(tokenList[tokenCounter-temp].type == newlinesym)
				{
                    temp++;
				}
                if(tokenList[tokenCounter-temp].type != semicolonsym)
				{
                    strcpy(tokenList[tokenCounter].name, ";");
                    tokenList[tokenCounter++].type = semicolonsym;
                }
                strcpy(tokenList[tokenCounter].name, "else");
                tokenList[tokenCounter++].type = elsesym;
            }
            else if (strcmp(varHolder, "end") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "end");
                tokenList[tokenCounter++].type = endsym;
            }
            else if (strcmp(varHolder, "if") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "if");
                tokenList[tokenCounter++].type = ifsym;
            }
			else if (strcmp(varHolder, "write") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "write");
                tokenList[tokenCounter++].type = writesym;
			}
             else if (strcmp(varHolder, "procedure") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "procedure");
                tokenList[tokenCounter++].type = procsym;
            }
            else if (strcmp(varHolder, "then") == 0)
			{
				strcpy(tokenList[tokenCounter].name, "then");
                tokenList[tokenCounter++].type = thensym;
            }
            else if (strcmp(varHolder, "while") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "while");
                tokenList[tokenCounter++].type = whilesym;
            }
			else if (strcmp(varHolder, "read") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "read");
				tokenList[tokenCounter++].type = readsym;
			}
            else if (strcmp(varHolder, "odd") == 0)
			{
                strcpy(tokenList[tokenCounter].name, "odd");
                tokenList[tokenCounter++].type = oddsym;
            }
            else
			{
				strcpy(tokenList[tokenCounter].name,varHolder);
				tokenList[tokenCounter++].type = identsym;
			}
		}
		
		else if(scanner >= '0' && scanner <= '9')
		{
			counter = 0;
			numHolder[counter++] = scanner;
			while((scanner = fgetc(sourceCodeFile)) > ' ')
			{
				if(counter < identMax -1)
				{
					if(scanner >= 'a' && scanner <= 'z' )
					{
						printf("Error: Line:%d :: ", lineScanner);
						printf("variable does not start with a number\n");
						flag = 0;
						break;
					}
					else if(scanner >= '0' && scanner <= '9')
					{
						numHolder[counter++] = scanner;
					}
					else
					{
						break;
					}
				}
				else
				{
					printf("Error: Line:%d :: ", lineScanner);
					printf("number is too long\n");
					flag = 0;
					break;
				}
			}
			numHolder[counter] = '\0';
			strcpy(tokenList[tokenCounter].name, numHolder);
			tokenList[tokenCounter++].type = numbersym;
		}
		
		else
		{
			scanner = fgetc(sourceCodeFile);
		} 
	}
}

//another function
void printLexemeTable(int lFlag)
{
	// print the table here
	int i;
	if(flag)
	{
		fprintf(tableOutputFile,"lexeme      token type\n");
		if(lFlag)
		{
			printf("lexeme      token type\n");
		}
		
		for(i=0; i<tokenCounter; i++)
		{
			if(tokenList[i].type == newlinesym)
			{
				continue;
			}
			
			fprintf(tableOutputFile,"%-11s %d\n", tokenList[i].name, tokenList[i].type);
			
			if(lFlag)
			{				
				printf("%-11s %d\n", tokenList[i].name, tokenList[i].type);
			}
		}	
	}
}

//another function here
void printLexemeList(int lFlag)
{
	int i;
	
	if(flag)
	{	
		fprintf(listOutputFile,"\n==============\nLexeme List:\n==============\n");
		if(lFlag)
		{
			printf("\n==============\nLexeme List:\n==============\n");
		}
		
		for(i=0; i<tokenCounter; i++)
		{
			if(tokenList[i].type == newlinesym)
			{
				continue;
			}
		
			fprintf(listOutputFile,"%d ", tokenList[i].type);
			if(lFlag)
			{
				printf("%d ", tokenList[i].type);
			}
			
			if(tokenList[i].type == identsym || tokenList[i].type == numbersym)
			{
				fprintf(listOutputFile,"%s ", tokenList[i].name);
				if(lFlag)
				{
					printf("%s ", tokenList[i].name);
				}
			}
			
		}
		fprintf(listOutputFile,"\n");
		if(lFlag)
		{
			printf("\n");
		}
	}
}






