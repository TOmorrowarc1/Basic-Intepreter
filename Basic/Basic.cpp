/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);
void tokenToString(TokenScanner&, std::string&);

/* Main program */

int main() {
    //给出变量记录state（string->int map)
    EvalState state;
    //给出程序记录program：int->string map
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string next = scanner.nextToken();
    TokenType type = scanner.getTokenType(next);
    switch (type) {
    case NUMBER: {
        int number = stringToInteger(next);
        if(!scanner.hasMoreTokens()){
            program.removeSourceLine(number);
            return;
        }
        next = scanner.nextToken();
        scanner.saveToken(next);
        std::string say;
        tokenToString(scanner, say);
        scanner.setInput(say);
        if (next == "REM") {
            Sequential* statement_1=new Sequential(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "LET") {
            Sequential* statement_1=new Sequential(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "PRINT") {
            Sequential* statement_1=new Sequential(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "INPUT") {
            Sequential* statement_1=new Sequential(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "GOTO") {
            GOTO* statement_1=new GOTO(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "END") {
            END* statement_1=new END(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else if (next == "IF") {
            IF* statement_1=new IF(scanner);
            program.addSourceLine(number, say, *statement_1);
        }
        else{
            error("SYNTAXERROR");
        }
        break;
    }
    case WORD: {
        if(next=="QUIT"){
            exit(0);
        }
        else if(next=="HELP"){
            std::cout<<"???";
        }
        else if(next=="LET"){
            scanner.saveToken(next);
            Sequential statement_1(scanner);
            statement_1.execute(state,program);
        }
        else if(next=="PRINT"){
            scanner.saveToken(next);
            Sequential statement_1(scanner);
            statement_1.execute(state,program);
        }
        else if(next=="INPUT"){
            scanner.saveToken(next);
            Sequential statement_1(scanner);
            statement_1.execute(state,program);
        }
        else if(next=="RUN"||next=="LIST"||next=="CLEAR"){
            scanner.saveToken(next);
            Command statement_1(scanner);
            statement_1.execute(state,program);
        }
        else{
            error("SYNTAXERROR");
        }
        break;
    }
    default: {
        error("SYNTAXERROR");
    }
    }
}

void tokenToString(TokenScanner& copies, std::string& object) {
    while (copies.hasMoreTokens()) {
        object += copies.nextToken();
        object += ' ';
    }
}
