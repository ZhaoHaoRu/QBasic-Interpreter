
#include "evalstate.h"

EvalState::EvalState()
{

}

void EvalState::setValue(string Var,int value)
{
    symbolTable[Var] = value;
}

//定义如果没有，返回-1
int EvalState::getValue(const string Var)
{
    if(isDefined(Var))
        return symbolTable[Var];
    else
        return -1;
}

bool EvalState::isDefined(string Var)
{
    if(symbolTable.find(Var) != symbolTable.end())
        return true;
    else
        return false;
}

