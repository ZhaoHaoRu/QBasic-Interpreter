# QBasic
## Introduction 
This project builds a minimal BASIC interpreter.

The programming language BASIC—the name is an acronym for Beginner’s All-purpose Symbolic Instruction Code—was developed in the mid-1960s at Dartmouth College by John Kemeny and Thomas Kurtz. It was one of the first languages designed to be easy to use and learn. Although BASIC has now pretty much disappeared as a teaching language, its ideas live on in Microsoft’s Visual Basic system, which remains in widespread use. 

## Functions & Commands
* ### Table 1. Statements implemented in the minimal version of BASIC
    | Statements  | Description <img width=100/> |
    |--------- | --------------------------------------------------|
    | REM | This statement is used for comments. Any text on the line after the keyword REM is ignored.  |
    | LET | This statement is BASIC’s assignment statement. The LET keyword is followed by a variable name, an equal sign, and an expression. As in C++, the effect of this statement is to assign the value of the expression to the variable, replacing any previous value. In BASIC, assignment is not an operator and may not be nested inside other expressions. |
    | PRINT | In minimal BASIC, the PRINT statement has the form: 'PRINT exp', where exp is an expression. The effect of this statement is to print the value of the expression on the console and then print a newline character so that the output from the next PRINT statement begins on a new line. |
    |INPUT |In the minimal version of the BASIC interpreter, the INPUT statement has the form:'INPUT var', where var is a variable read in from the user. The effect of this statement is to print a prompt consisting of the string " ? " and then to read in a value to be stored in the variable. (The string " ? " should display in the command input edit box in GUI.)|
    |GOTO |This statement has the syntax: 'GOTO n', and forces an unconditional change in the control flow of the program. When the program hits this statement, the program continues from line n instead of continuing with the next statement. Your program should report an error if line n does not exist.|
    |IF	 |This statement provides conditional control. The syntax for this statement is: 'IF exp1 op exp2 THEN n ',where exp1 and exp2 are expressions and op is one of the conditional operators =, <, or >. If the condition holds, the program should continue from line n just as in the GOTO statement. If not, the program continues on to the next line.Note that the conditional operators (=, <, >) are not parts of expressions.|
    |END |Marks the end of the program. Execution halts when this line is reached. This statement is usually optional in BASIC programs because execution also stops if the program continues past the last numbered line. |

* ### Table 2. Commands to control the BASIC interpreter  
    | Commands  | Description <img width=100/> |
    |--------- | --------------------------------------------------|
    |RUN|This command starts program execution beginning at the lowest-numbered line. Unless the flow is changed by GOTO and IF commands, statements are executed in line-number order. Execution ends when the program hits the END statement or continues past the last statement in the program. |
    |LOAD |This command loads a file containing statements and commands. Statements and commands should be stored (also displayed in GUI) and executed respectively, as if they were entered into input box in order. A prompt window should be displayed when this command is entered. The window asks users to choose the file to load.|
    |LIST |This command lists the steps in the program in numerical sequence. It has been required to be implemented in the previous version of this project. In the new version, your interpreter should be able to display all the codes that have been entered in real time, so there is no need to implement this command.|
    |CLEAR|This command deletes the program so the user can start entering a new one.|
    |HELP	|This command provides a simple help message describing your interpreter.|
    |QUIT	|Typing QUIT exits from the BASIC interpreter.|

* ### Syntax tree display
    Syntax tree is one abstract representation of program. More specifically, every statement in the program can be represented as a tree. The structure of the syntax tree can be seen as the steps of the computation of the expression in the statement. The node of the tree can be identifier definition, assignment, function call, expression computation and conditional or unconditional branch.  In my interpreter implementation, the computation is made along the syntax tree from leaf node to root. Because the connection structures of the tree are determined by the computation rules, e.g. operator priority and association. In my implementation, the syntax tree is constructed in infix notation.  


## Contact 
Haoru Zhao - zhaohaoru@sjtu.edu.cn

## License 
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
