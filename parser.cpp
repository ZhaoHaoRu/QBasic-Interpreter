#include "parser.h"

Parser::Parser(string inputString)
{
    formerString = inputString;
    monitor = new Tokenizer(inputString);
    nowPrec = 0;
}

//operator的优先级
int Parser::precedence(string token)
{
    if(token == "*" || token == "/")
        return 3;
    else if(token == "**")
        return 4;
    else if(token == "+" || token == "-")
        return 2;
    else if(token == "-")
        return 1;
    //不是合法的操作符
    else
        return -1;
}

//处理表达式具体运算
void Parser::HandleOps(string op)
{
    Expression *exp1,*exp2;
    if(Operands.empty()){
        cout << "formerString: "<< formerString << endl;
        throw error("Don't have enough operands!");
    }
    else{
        exp2 = Operands.top();
        Operands.pop();
    }
    if(Operands.empty()){
        cout << "formerString: "<< formerString << endl;
        throw error("Don't have enough operands!");
    }
    else{
        exp1 = Operands.top();
        Operands.pop();
    }

    Expression *exp = new CompoundExp(op,exp1,exp2);
    Operands.push(exp);

}

Expression *Parser::readExp()
{
    //依次从表达式中读取出一个合法的符号，直到表达式结束
    while(monitor->hasMoreTokens())
    {
        string token = monitor->ReadToken();
        TokenType type = monitor->getTokenType(token);
        //如果是字符，设为indentifier，数字，设为constant，存入操作数栈
        if(type == WORD) Operands.push(new IdentifierExp(token));
        else if(type == NUMBER) Operands.push(new ConstantExp(atoi(token.c_str())));
        else if(type == PARENTHESES){
            //开括号进运算符栈
            if(token == "(")
                Operators.push(token);
            //开括号和闭括号之间的所有运算都可以进行
            else{
                while(!Operators.empty() && (Operators.top() != "(")){
                   HandleOps(Operators.top());
                   Operators.pop();
                }
                if(Operators.top() != "("){
                    throw error("Unbalance parentheses!");
                    return nullptr;
                }
                else
                    Operators.pop();
            }
        }
        else if(type == OPERATOR){
            nowPrec = precedence(token);
            //乘方运算符进运算符栈
            if(nowPrec == 4)
               Operators.push(token);
            //运算符栈中的"/","*","**"退栈执行，当前运算符进栈
            else if(nowPrec == 3){
                while(!Operators.empty()){
                    string preOp = Operators.top();
                    int pastPrec = precedence(preOp);
                    if(pastPrec >= 3){
                        HandleOps(preOp);
                        Operators.pop();
                    }
                    else
                        break;
                }
                Operators.push(token);
            }
            //运算符栈中的运算符依次出栈执行，直到栈为空或者遇到开括号，当前运算符进栈
            else if(nowPrec == 2){
                while(!Operators.empty()){
                    string preOp = Operators.top();
                    if(preOp != "("){
                        HandleOps(preOp);
                        Operators.pop();
                    }
                    else
                        break;
                }
                Operators.push(token);

            }
        }
    }
    //运算符栈中的所有运算符出栈执行
    while(!Operators.empty()){
         HandleOps(Operators.top());
         Operators.pop();
    }
    //运算数栈为空，无结果
    if(Operands.empty()){
        throw error("Don't have result");
        return nullptr;
    }

    Expression *resultExp = Operands.top();
    Operands.pop();
    //运算数栈非空，出错
    if(!Operands.empty()){
        cout << "formerString: "<< formerString << endl;
        cout << "type:" << Operands.top()->type() << endl;
        throw error("Don't have enough operator!");
        return nullptr;
    }

    return resultExp;
}



Parser::~Parser()
{
    delete monitor;
}
