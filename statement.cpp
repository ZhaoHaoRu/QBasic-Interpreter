#include "statement.h"

Statement::Statement(){}

void clear(Treenode *root){
    while(!root->children.empty()){
        Treenode *child = root->children.back();
        root->children.pop_back();
        clear(child);
    }
    delete root;
}


RemStmt::RemStmt(Tokenizer &scanner)
{
    root = new Treenode("REM");
    scanner.SkipBlankSpace();
    note = scanner.getBuffer();
}

//注释，不用处理任何语句，直接return
void RemStmt::Handling(EvalState &context, Program &program)
{
    return;
}

Treenode *RemStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " REM";
    root->data = newRoot;
    Treenode *child = new Treenode(note);
    root->children.push_back(child);
    return root;
}

RemStmt::~RemStmt()
{
    clear(root);
}

LetStmt::LetStmt(Tokenizer &scanner)
{
    root = new Treenode("LET =");
    string str1 = scanner.ReadToken();
    string str2 = scanner.ReadToken();
    if(str2 != "="){
        name = "";
        equalOp = "";
        exp = nullptr;
        throw error("Invalid Statement! Let don't have equal sign!");
        return;
    }
    else{
        name = str1;
        equalOp = str2;
        string buffer = scanner.getBuffer();
        Parser parser(buffer);
        exp = parser.readExp();
    }
}

void LetStmt::Handling(EvalState &context, Program &program)
{
    int val = exp->eval(context);
    context.setValue(name,val);
}

Treenode *LetStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " LET = ";
    root->data = newRoot;
    Treenode *child = new Treenode(name);
    root->children.push_back(child);
    Treenode *Expnode = exp->toExpTree(exp);
    root->children.push_back(Expnode);
    return root;
}

LetStmt::~LetStmt()
{
    clear(root);
}

PrintStmt::PrintStmt(Tokenizer &scanner)
{
    if(scanner.isString()){
        string tmpString = scanner.getBuffer();
        string printString = tmpString.substr(1,tmpString.size()-1);
        exp = new IdentifierExp(printString);
        istring = true;
        return;
    }
    else{
        root = new Treenode("PRINT");
        Parser parser(scanner.getBuffer());
        exp = parser.readExp();
        istring = false;
    }

}

void PrintStmt::Handling(EvalState &context, Program &program)
{
    string outString;
    if(istring == true)
        outString = exp->toString();
    else
        outString = to_string(exp->eval(context));
    program.setOutput(outString);
    return;
}

//生成print的表达式树
Treenode *PrintStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " PRINT";
    root->data = newRoot;
    Treenode *Expnode = exp->toExpTree(exp);
    root->children.push_back(Expnode);
    return root;
}

PrintStmt::~PrintStmt()
{
    clear(root);
    delete exp;
}

InputStmt::InputStmt(Tokenizer &scanner, int value)
{
    indentfier = scanner.ReadToken();
    val = value;
    root = new Treenode("INPUT");
}


Treenode *InputStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " INPUT";
    root->data = newRoot;
    Treenode *newNode = new Treenode(indentfier);
    root->children.push_back(newNode);
    return root;
}

void InputStmt::Handling(EvalState &context, Program &program)
{
   context.setValue(indentfier,val);
}

InputStmt::~InputStmt()
{
    clear(root);
}

GotoStmt::GotoStmt(Tokenizer &scanner)
{
    root = new Treenode("GOTO");
    string lNum = scanner.ReadToken();
    int tmpNum = 0;
    try {
        tmpNum = stoi(lNum.c_str());
        //处理行号无效的情况
    } catch (std::invalid_argument) {
        throw error("Invalid Statement! Goto don't have a lineLabel behind!");
    }
    lineNum = tmpNum;
}

void GotoStmt::Handling(EvalState &context, Program &program)
{
    if(program.lineStore.find(lineNum) == program.lineStore.end())
        throw error("Goto an invalid line!");
    program.setNextLine(lineNum);
}

Treenode *GotoStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " GOTO";
    root->data = newRoot;
    Treenode *child = new Treenode(to_string(this->lineNum));
    root->children.push_back(child);
    return root;
}

GotoStmt::~GotoStmt()
{
    clear(root);
}

IfStmt::IfStmt(Tokenizer &scanner,string OriginString)
{

    root = new Treenode("IF THEN");
    int Tposi = OriginString.find("THEN");
    if(Tposi == std::string::npos){
       throw error("Invalid Statement! If is not in Specified format!");
       return;
    }

    //寻找操作符，并判断是否找到
    int posi[3] = {0};
    posi[0] = OriginString.find('=');
    posi[1] = OriginString.find('<');
    posi[2] = OriginString.find('>');
    for(int i = 0;i < 3;++i){
        if(posi[i] != std::string::npos){
            cout << posi[i]<< endl;
            if(i == 0)
                CmpOp = "=";
            else if(i == 1)
                CmpOp = "<";
            else
                CmpOp = ">";
            string expString1 = OriginString.substr(3,posi[i] - 3);
            if(expString1.find('=') != std::string::npos || expString1.find('<') != std::string::npos || expString1.find('>') != std::string::npos){
                throw error("exp1:Invalid Statement! If is not in Specified format!");
                return;
            }
            //分别计算操作符两侧的表达式
            int count = Tposi - posi[i] - 1;
            string expString2 = OriginString.substr(posi[i] + 1,count);
            if(expString2.find('=') != std::string::npos || expString2.find('<') != std::string::npos || expString2.find('>') != std::string::npos){
                throw error("exp2:Invalid Statement! If is not in Specified format!");
                return;
            }
            Parser parser1(expString1);
            exp1 = parser1.readExp();
            Parser parser2(expString2);
            exp2 = parser2.readExp();
            Tokenizer scannerTmp(OriginString.substr(Tposi + 4));
            int tmpLine = 0;
            try{
                tmpLine = stoi(scannerTmp.ReadToken().c_str());
                lineNum = tmpLine;
                //如果没有行号
            }catch (std::invalid_argument){
                throw error("Invalid Statement! If don't have a lineLabel behind!");
            }
            break;
        }
    }
}

void IfStmt::Handling(EvalState &context, Program &program)
{
    int leftVal = exp1->eval(context);
    int rightVal = exp2->eval(context);
    if(CmpOp == "=" && (leftVal == rightVal)){
        if(program.lineStore.find(lineNum) == program.lineStore.end())
            throw error("If control go to an invalid line!");
        program.setNextLine(lineNum);
    }
    else if(CmpOp == "<" && (leftVal < rightVal)){
        if(program.lineStore.find(lineNum) == program.lineStore.end())
            throw error("If control go to an invalid line!");
        program.setNextLine(lineNum);
    }
    else if(CmpOp == ">" && (leftVal > rightVal)){
        if(program.lineStore.find(lineNum) == program.lineStore.end())
            throw error("If control go to an invalid line!");
        program.setNextLine(lineNum);
    }
}

Treenode *IfStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " IF THEN";
    root->data = newRoot;
    Treenode *Expnode1 = exp1->toExpTree(exp1);
    root->children.push_back(Expnode1);
    Treenode *Opnode = new Treenode(CmpOp);
    root->children.push_back(Opnode);
    Treenode *Expnode2 = exp2->toExpTree(exp2);
    root->children.push_back(Expnode2);
    Treenode *Linenode = new Treenode(to_string(this->lineNum));
    root->children.push_back(Linenode);
    return root;
}

IfStmt::~IfStmt()
{
    clear(root);
}

EndStmt::EndStmt(Tokenizer &scanner)
{
    root = new Treenode("END");
}

void EndStmt::Handling(EvalState &context, Program &program)
{
     program.setNextLine(-1);
}

Treenode *EndStmt::CreateTree(int lineNum)
{
    string Num = to_string(lineNum);
    string newRoot = Num + " END";
    root->data = newRoot;
    return root;
}

EndStmt::~EndStmt()
{
    delete root;
}

