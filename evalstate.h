#pragma once
#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <iostream>
#include <string>
#include <map>
#include "tokenizer.h"

using namespace std;
//EvalState类维护的唯一信息是一个符号表，它将变量名映射到它们的值中。
class EvalState
{
private:
    map<string,int> symbolTable;

public:
    EvalState();
    void setValue(string Var,int value);
    int getValue(const string Var);
    bool isDefined(string Var);

};



#endif // EVALSTATE_H
