#include "tokenizer.h"

error::error(string initString)
{
    this->errorString = initString;
}

string error::printString()
{
    cout << errorString << endl;
    return errorString;
}


Tokenizer::Tokenizer(string inputString)
{
    buffer = inputString;
    formerToken = "=";
    posi = 0;
}

Tokenizer::Tokenizer(string fileName,int mark)
{

}

//跳过空格
void Tokenizer::SkipBlankSpace()
{
    for(int i = posi; i < buffer.length(); ++i){
        if(isblank(buffer[i]))
            ++posi;
        else
            break;
    }
}

//判断是否是前6个operator的工具函数
bool Tokenizer::isOp(char c)
{
    string cstr;
    cstr += c;
    for(int i = 0; i < 6; ++i){
        if(cstr == Ops[i])
            return true;
    }
    return false;
}

//用一对引号引起来的是一个string
bool Tokenizer::isString()
{
    int tmp = posi;
    if(buffer[tmp] == '"' && buffer[buffer.length() -1] == '"')
        return true;
    else
        return false;
}

string Tokenizer::getBuffer()
{
    return buffer.substr(posi);
}

bool Tokenizer::hasMoreTokens()
{
    SkipBlankSpace();
    return posi < buffer.length();
}

//读取字符、数字等
string Tokenizer::ReadToken()
{
    SkipBlankSpace();
    string TokenWant;
    if(posi >= buffer.length())
        return "";

    //此处默认如果是减号的话前面一定没有括号或者是等于号，有括号是负数
    if(buffer[posi] == '-'){
        TokenWant += buffer[posi];
        ++posi;
        while(posi < buffer.length() && (posi - 2 >= 0) && (formerToken == "(" || formerToken == "=")){
            TokenWant += buffer[posi];
            ++posi;
            if(buffer[posi] == ')')
                break;
        }
        formerToken = TokenWant;
        return TokenWant;
    }

    //处理“**”的情况
    if(posi < buffer.length() - 1){
        if(buffer[posi] == '*' && buffer[posi + 1] == '*'){
            posi = posi + 2;
            TokenWant = "**";
            formerToken = TokenWant;
            return TokenWant;
        }
    }

    //如果是一个字母或者是数字的话，说明可能是一个word,所以可能要读到最后
    if(isalnum(buffer[posi])){
        TokenWant += buffer[posi];
        ++posi;
        while(posi < buffer.length() && isalnum(buffer[posi])){
            TokenWant += buffer[posi];
            ++posi;
        }
        formerToken = TokenWant;
        return TokenWant;
    }

    else{
        TokenWant += buffer[posi++];
        formerToken = TokenWant;
        return TokenWant;
    }
}

int Tokenizer::getPosition()
{
    return posi;
}

//判断是否有所希望找到的字符
bool Tokenizer::verifyToken(string expected)
{
    //在verify的过程中没有对buffer进行实质性的读取
    SkipBlankSpace();
    int tmp = posi;
    for(int i = 0; i < expected.length(); ++i){
        if(buffer[tmp+i] != expected[i])
            return false;
    }
    return true;
}

//获取字符类型
TokenType Tokenizer::getTokenType(string token)
{
    if(token == "(" ||token == ")")
        return PARENTHESES;
    for(int i = 0; i < 6; ++i){
        if(Ops[i] == token)
            return OPERATOR;
    }
    for(int j = 0; j < 7; ++j){
        if(State[j] == token)
            return STATEMENT;
    }
    for(int j = 0; j < 6; ++j){
        if(Control[j] == token)
            return CONTROL;
    }
    int i;
    if(token[0] == '-' || isdigit(token[0])){
        for(i = 1; i < token.length(); ++i){
            if(!isdigit(token[i]))
                break;
        }
        if(i == token.length())
            return NUMBER;
    }
    int j;
    for(j = 0; j < token.length(); ++j){
        if(!isalnum(token[j]))
            break;
    }
    if(j == token.length())
        return WORD;

    throw error("invalid token type!");
}
