The PL/0 Compiler Implementation based on the given grammer.

This PL/0 compiler is the implementation of "Scanner", "Parser", and "VirtualMachine".
It has all the header files and a "CompilerDriver.c" file, which has main function to run
the program.The project can be compiled using the command "gcc CompilerDriver.c".
After a program is compiled, it creates an executable file.

There are different options available to run this program after it has been compiled.
There has to be atleast 2 command lines arguments passed in to execute the program,one being
the executable file and second one being the ".txt" file. For example,
To compiler the program
"gcc CompilerDriver.c"
To run or execute the program
"./a.out SouceCode.txt"
I have  shown with a sample "SouceCode.txt" file which has PL/0 souce code. There are other
".txt" test files that can be used to test the working of PL/0 compiler
 
There are also other commads like "-l", "-a", "-v" available to be passed in as a command 
line argument to print out desired output to the console.
One thing to note is that if the grammer is incorrent, the possible errors will be printed
to the console for the errors to be fixed. It does not print out anything to the corresponding 
output files if the PL/0 grammer is incorrent. Once the PL/0 is correct, the results will be 
printed to the corresponding output files.
 