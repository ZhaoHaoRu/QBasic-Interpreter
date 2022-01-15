#include "program.h"

Program::Program()
{
    var = 0;
    jmp = false;
    nextLine = -1;
}

string Program::displayMap(int lineNum)
{
    string lineContent = lineStore[lineNum];
    string lineDisplay = to_string(lineNum);
    lineDisplay.append(" ");
    lineDisplay += lineContent;
    return lineDisplay;
}

void Program::clear()
{
    lineList.clear();
    lineStore.clear();
    var = 0;
    jmp = false;
    nextLine = 0;
}

void Program::addNewLine(int lineNum,string line)
{
    lineStore[lineNum] = line;
    Line newLine = Line(lineNum,line);
    if(lineList.size() == 0){
        lineList.push_back(newLine);
        return;
    }
    list<Line>::iterator itr = lineList.begin(),itre = lineList.end();
    if(lineList.size() == 1){
        if(itr->lineNum < lineNum)
            lineList.push_back(newLine);
        else
            lineList.push_front(newLine);
        return;
    }
    else {
        for(;itr != itre; ++itr){
            if(itr->lineNum < lineNum){
                int nextLineNum = (++itr)->lineNum;
                if(nextLineNum >= lineNum){
                    lineList.insert(itr,newLine);
                    return;
                }
                else if(nextLineNum == lineNum){
                    lineList.erase(itr);
                    lineList.insert(itr,newLine);
                    return;
                }
                else --itr;
            }
            lineList.push_back(newLine);
        }
    }
    return;
}

void Program::removeLine(int lineNum)
{
    lineStore.erase(lineNum);
    list<Line>::iterator itr = lineList.begin();
    for(;itr != lineList.end(); ++itr){
        if(itr->lineNum == lineNum){
            lineList.erase(itr);
            break;
        }
    }
    return;
}

string Program::getLine(int lineNum)
{
    map<int,string>::iterator result = lineStore.find(lineNum);
    return result->second;
}

//存入input的值
void Program::getInput(int var)
{
    this->var = var;
}

void Program::setNextLine(int lineNum)
{
    jmp = true;
    nextLine = lineNum;
}

int Program::getNextLine()
{
    return nextLine;
}



bool Program::isJmp()
{
    return jmp;
}

void Program::resetJmp()
{
    jmp = false;
}

Statement *Program::getState()
{
    return Prostate;
}

void Program::setOutput(string str)
{
    output = str;
}

string Program::getOutput()
{
    return output;
}

void Program::getParsedState(int lineNum)
{
    Tokenizer scanner(lineStore[lineNum]);
    string stateMark = scanner.ReadToken();
    int i = 0;
    //根据statement的标识符判断不同的statement
    for(i = 0;i < 7;++i){
        if(Statelabel[i] == stateMark)
            break;
    }
    switch(i){
        case 0: Prostate = new RemStmt(scanner);
                break;
        case 1: Prostate = new LetStmt(scanner);
                break;
        case 2: Prostate = new PrintStmt(scanner);
                break;
        case 3: cout<<"get parsedStatement!"<<endl;
                Prostate = new InputStmt(scanner,var);
                break;
        case 4: Prostate = new GotoStmt(scanner);
                break;
        case 5: Prostate = new IfStmt(scanner,lineStore[lineNum]);
                break;
        case 6: Prostate = new EndStmt(scanner);
                break;
        default:Prostate = nullptr;
                throw error("invalid statement identifier!");
    }
    return ;
}

//无行号情况的处理
void Program::getInstantParsed(string input)
{
    Tokenizer scanner(input);
    string stateMark = scanner.ReadToken();
    int i = 0;
    for(i = 0;i < 7;++i){
        if(Statelabel[i] == stateMark)
            break;
    }
    switch(i){
        case 0: Prostate = new RemStmt(scanner);
                break;
        case 1: Prostate = new LetStmt(scanner);
                break;
        case 2: Prostate = new PrintStmt(scanner);
                break;
        case 3: Prostate = new InputStmt(scanner,var);
                break;
        case 4: Prostate = new GotoStmt(scanner);
                break;
        case 5: Prostate = new IfStmt(scanner,input);
                break;
        case 6: Prostate = new EndStmt(scanner);
                break;
        default:Prostate = nullptr;
                throw error("invalid statement identifier!");
    }
    return ;
}

