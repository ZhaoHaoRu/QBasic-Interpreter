#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

//作用：divide a string into individual logic units
//还差一个结束符
enum TokenType {CONTROL, STATEMENT, WORD, NUMBER, OPERATOR, PARENTHESES};




class Tokenizer
{
private:
    string buffer;
    int posi;
    string Ops[8] = { "+", "-", "*", "/", "**", "=","(",")"};
    string  State[7] = {"REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "END"};
    string Control[6] = {"RUN", "LOAD", "LIST", "CLEAR", "HELP", "QIUT"};
    string formerToken;

public:
    Tokenizer(string inputString);
    Tokenizer(string fileName,int mark);
    void SkipBlankSpace();
    bool hasMoreTokens();
    //判断是否是一个string的类型
    bool isString();
    bool isOp(char c);
    string getBuffer();
    string ReadToken();
    int getPosition();
    bool verifyToken(string expected);
    TokenType getTokenType(string token);

};

class error
{
private:
    string errorString;

public:
    error(string initString);
    string printString();
};

#endif // TOKENIZER_H
