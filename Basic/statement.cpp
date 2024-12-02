/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"

class Program;
class Statement;

/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;
void Statement::kill(){
    return;
}

Command::Command(TokenScanner& token) {
    std::string order = token.nextToken();
    if (order == "RUN") {
        type = RUN;
    }
    else if (order == "LIST") {
        type = LIST;
    }
    else if (order == "CLEAR") {
        type = CLEAR;
    }
    else {
        error("SYNAXERROR");
    }
}
void Command::execute(EvalState& state ,Program& program) {
    switch (type) {
    case RUN: {
        program.run_program_(state);
        break;
    }
    case LIST: {
        program.list_program_();
        break;
    }
    case CLEAR:{
        program.clear(state);
        break;
     }
    }
    return;
}

Control::Control() {
    object_pointer_ = 0;
}
Control::~Control() = default;

void Control::Set(int a) {
    object_pointer_ = a;
}
void Control::kill(){
    return;
}
void Control::jump(Program& program) {
    if (!program.set_pointer(object_pointer_)) {
        error("LINE NUMBER ERROR");
    }
    return;
}


GOTO::GOTO(TokenScanner& token) {
    token.nextToken();
    std::string next = token.nextToken();
    int a = stringToInteger(next);
    Set(a);
}
void GOTO::execute(EvalState& state, Program& program) {
    jump(program);
    return;
}


IF::IF(TokenScanner& token) {
    std::string expression,next;
    int object = 0;
    next=token.nextToken();
    while (1) {
        next = token.nextToken();
        if (next == "THEN") {
            break;
        }
        else {
            expression += next;
        }
    }
    next = token.nextToken();
    object = stringToInteger(next);
    Set(object);
    TokenScanner new_token(expression);
    new_token.ignoreWhitespace();
    new_token.scanNumbers();
    lhs = readE(new_token,1);
    next=new_token.nextToken();
    if(next=="<"){
        compare='<';
    }
    else if(next=="="){
        compare='=';
    }
    else{
        compare='>';
    }
    rhs=parseExp(new_token);
}
IF::~IF() {
    delete lhs;
    delete rhs;
    lhs = nullptr;
    rhs=nullptr;
}
void IF::kill(){
    delete lhs;
    delete rhs;
    lhs = nullptr;
    rhs=nullptr;
}
void IF::execute(EvalState& state, Program& program) {
    bool flag=0;
    int a = (*lhs).eval(state);
    int b=(*rhs).eval(state);
    if(compare=='>'&& a>b){
        flag=1;
    }
    else if(compare=='='&& a==b){
        flag=1;
    }
    else if(compare=='<'&&a<b){
        flag=1;
    }
    if(flag==1){
        jump(program);
    }
    return;
}

END::END(TokenScanner& token) {
    Set(-1);
}
void END::execute(EvalState& state, Program& program) {
    jump(program);
    return;
}



Sequential::Sequential(TokenScanner& token) {
    std::string order = token.nextToken();
    if (order == "REM") {
        type = REM;
        input="comment";
    }
    else if (order == "LET") {
        type = LET;
        std::string expression, next;
        while (token.hasMoreTokens()) {
            next = token.nextToken();
            expression += next;
        }
        TokenScanner new_token(expression);
        new_token.ignoreWhitespace();
        new_token.scanNumbers();
        exp = parseExp(new_token);
    }
    else if (order == "INPUT") {
        type = INPUT;
        input = token.nextToken();
    }
    else if (order == "PRINT") {
        type = PRINT;
        std::string expression, next;
        while (token.hasMoreTokens()) {
            next = token.nextToken();
            expression += next;
        }
        TokenScanner new_token(expression);
        new_token.ignoreWhitespace();
        new_token.scanNumbers();
        exp = parseExp(new_token);
    }
}
Sequential::~Sequential() {
    if(type!=REM && type!=INPUT ){
        delete exp;
    }
    exp = nullptr;
}
void Sequential::kill(){
    if(type!=REM){
        delete exp;
    }
    exp = nullptr;
}
void Sequential::execute(EvalState& state, Program& program) {
    switch (type) {
    case REM: {
        break;
    }
    case LET: {
        (*exp).eval(state);
        break;
    }
    case INPUT: {
        while(1){
            std::cout<<' '<<'?'<<' ';
            std::string in;
            getline(std::cin,in);
            int pointer=0,flag=1;
            char check=in[0];
            if(check!='-'&&check!='+'&&(check>'9'||check<'0')){
                std::cout<<"INVALID NUMBER"<<'\n';
                continue;
            }
            else{
                ++pointer;
                while(in[pointer]!=0&&flag){
                    check=in[pointer];
                    if(check>'9'||check<'0'){
                        std::cout<<"INVALID NUMBER"<<'\n';
                        flag=0;
                    }
                    ++pointer;
                }
                if(flag==0){
                    continue;
                }
                else{
                   int in_number=0,ten=1;
                   --pointer;
                   while(pointer>=0&&in[pointer]!='-'&&in[pointer]!='+'){
                    in_number+=ten*(in[pointer]-'0');
                    ten=ten*10;
                    --pointer;
                   }
                   if(in[0]=='-'){
                    in_number=-in_number;
                   }
                   state.setValue(input, in_number);
                   break;
                }
            }
        }
        break;
    }
    case PRINT: {
        int outcome=(*exp).eval(state);
        std::cout << outcome << '\n';
        break;
    }
    }
    return;
}

