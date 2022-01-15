#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <cmath>
#include <vector>
#include "evalstate.h"
using namespace std;

struct Treenode
{
    string data;
    //用vector是因为子节点可能不止一个
    vector<Treenode*> children;

    Treenode(string str = ""){
        data = str;
    }
};

enum ExpressionType{ CONSTANT, IDENTIFIER, COMPOUND};

class Expression
{
public:
    Expression();
    Treenode *toExpTree(Expression *exp);
    virtual ~Expression(){};
    virtual int eval(EvalState &context) = 0;
    virtual string toString() = 0;
    virtual ExpressionType type() = 0;

    //virtual int getConstantVal(){}
    virtual string getIdentifierName(){return "";}
    virtual string getOperator(){return "";}
    virtual Expression *getLHS(){return nullptr;}
    virtual Expression *getRHS(){return nullptr;}

};

class ConstantExp: public Expression {
private:
    int value;

public:
    ConstantExp(int val);
    //对于常量表达式，该函数只是存储在节点中的常量的值。
    int eval(EvalState &context) override;
    string toString() override;
    ExpressionType type() override;

};

class IdentifierExp: public Expression {
private:
    string name;

public:
    IdentifierExp(string name);
    // the value is determined by looking up the identifier name in a symbol table and
    //returning the corresponding value
    int eval(EvalState &context) override;
    string toString() override;
    ExpressionType type() override;
    string getIdentifierName() override;

};

class CompoundExp: public Expression {
private:
    string op;
    Expression *lhs,*rhs;

public:
    CompoundExp(string op, Expression *lhs, Expression *rhs);
    //the value must be computed by recursively evaluating the subexpressions and
    //then applying the appropriate operator.
    int eval(EvalState &context) override;
    string toString() override;
    ExpressionType type() override;
    string getOperator() override;
    Expression *getLHS() override;
    Expression *getRHS() override;
};

#endif // EXPRESSION_H
