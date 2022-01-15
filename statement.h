#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H


#include "parser.h"
#include "program.h"

using namespace std;

class Program;
//EvalState evalTable;


//注意动态变量最后的析构
class Statement
{
public:
    Treenode *root;
    Statement();
    virtual void Handling(EvalState &context, Program &program) = 0;
    virtual Treenode *CreateTree(int lineNum) = 0;
    virtual ~Statement(){}
};

class RemStmt:public Statement
{
private:
    string note;

public:
    RemStmt(Tokenizer &scanner);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~RemStmt();
};

class LetStmt:public Statement
{
private:
    string name;
    string equalOp;
    Expression *exp;

public:
    //the constructor for the LetStmt class reads an identifier name, an equal
    //sign, and an expression from the scanner and then stores that information in instance
    //variables.
    LetStmt(Tokenizer &scanner);
    // the execute method has to evaluate the expression that appeared on the right side of the
    //equal sign and then store that value in the variable that appears on the left side
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~LetStmt();
};

class PrintStmt:public Statement
{
private:
    Expression *exp;
    bool istring;

public:
    PrintStmt(Tokenizer &scanner);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    //string handleString(EvalState &context);
    ~PrintStmt();
};

class InputStmt:public Statement
{
private:
   string indentfier;
   int val;

public:
    InputStmt(Tokenizer &scanner, int value);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~InputStmt();

};



class GotoStmt:public Statement
{
private:
    int lineNum;
public:
    //the GotoStmt constructor reads a line number and stores that in a different
    //collection of instance variables, since each of the subclasses defines its own structure
    GotoStmt(Tokenizer &scanner);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~GotoStmt();
};

class IfStmt:public Statement
{
private:
    Expression *exp1,*exp2;
    string CmpOp;
    int lineNum;

public:
    IfStmt(Tokenizer &scanner, string OriginString);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~IfStmt();
};

class EndStmt:public Statement
{
public:
    EndStmt(Tokenizer &scanner);
    void Handling(EvalState &context, Program &program) override;
    Treenode *CreateTree(int lineNum) override;
    ~EndStmt();
};



#endif // STATEMENT_H
