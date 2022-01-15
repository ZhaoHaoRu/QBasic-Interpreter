#include "expression.h"

Expression::Expression()
{
}

//生成表达式部分的树
Treenode *Expression::toExpTree(Expression *exp)
{
    Treenode *root = new Treenode(exp->toString());
    if(exp->type() == COMPOUND && exp->getLHS() != nullptr){
        Treenode *child = toExpTree(exp->getLHS());
        root->children.push_back(child);
    }
    if(exp->type() == COMPOUND && exp->getRHS() != nullptr){
        Treenode *child = toExpTree(exp->getRHS());
        root->children.push_back(child);
    }
    return root;
}

ConstantExp::ConstantExp(int val)
{
    value = val;
}

int ConstantExp::eval(EvalState &context)
{
    return value;
}

string ConstantExp::toString()
{
    return  to_string(value);
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

IdentifierExp::IdentifierExp(string name)
{
    this->name = name;
}

int IdentifierExp::eval(EvalState &context)
{
    if(!context.isDefined(name))
        throw error(name + " is undefined");
    return context.getValue(name);
}

string IdentifierExp::toString()
{
    return name;
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}

string IdentifierExp::getIdentifierName()
{
    return name;
}

CompoundExp::CompoundExp(string op, Expression *lhs, Expression *rhs)
{
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

//计算表达式值
int CompoundExp::eval(EvalState &context)
{
    int right = rhs->eval(context);
    if(op == "="){
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->eval(context);
    if(op == "+") return left + right;
    if(op == "-") return left - right;
    if(op == "*") return left * right;
    if(op == "**") return int(pow(double(left),double(right)));
    if(op == "/") {
        if(right == 0)
            throw error("Division by 0");
        return left / right;
    }
    throw error("Illegal operator in expreession");
    return 0;
}

string CompoundExp::toString()
{
    return op;
}



ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

string CompoundExp::getOperator()
{
    return op;
}

//得到左表达式
Expression *CompoundExp::getLHS()
{
    return lhs;
}

//得到右表达式
Expression *CompoundExp::getRHS()
{
    return rhs;
}
