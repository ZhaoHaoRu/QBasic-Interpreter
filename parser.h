#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "expression.h"
#include <stack>

using namespace std;
class Parser
{
private:
    stack<string> Operators;
    stack<Expression*> Operands;
    Tokenizer *monitor;
    string formerString;
    int nowPrec;


public:
    Parser(string inputString);
    int precedence(string token);
    void HandleOps(string op);
    Expression *readExp();
    ~Parser();
};

#endif // PARSER_H
