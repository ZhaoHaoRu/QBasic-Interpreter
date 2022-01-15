#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <iterator>
#include <list>
#include <QString>

#include "statement.h"

class Statement;
using namespace std;

struct Line{
    int lineNum;
    string code;

    Line(int l = 0,string str =""){
        lineNum = l;
        code = str;
    }
};

//实现程序语句的存储
class Program
{
private:
    list<Line> lineList;
    Statement *Prostate;
    int var;
    string output;
    bool jmp;
    int nextLine;
    string  Statelabel[7] = {"REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "END"};

public:
    map<int,string> lineStore;
    Program();
    //输入list操作时，按顺序显示代码
    string displayMap(int lineNum);
    void clear();
    void addNewLine(int lineNum,string line);
    void removeLine(int lineNum);
    //有啥用嘛？
    string getLine(int lineNum);
    void setNextLine(int lineNum);
    int getNextLine();
    void resetJmp();
    bool isJmp();
    Statement *getState();
    void getParsedState(int lineNum);
    void getInstantParsed(string input);
    void getInput(int var);
    void setOutput(string str);
    string getOutput();

};

#endif // PROGRAM_H
