/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "evalstate.hpp"

class Program;
class Statement;

Program::Program() = default;

Program::~Program() = default;

// Removes all lines from the program.
void Program::clear(EvalState& state) {
    max_line = 0;
    pointer = 0;
    line_numbers_.clear();
    storage.clear();
    for(auto iter=ram.begin();iter!=ram.end();++iter){
        (*iter).second->kill();
        delete (*iter).second;
    }
    ram.clear();
    state.Clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line, Statement& info) {
    auto iter = line_numbers_.find(lineNumber);
    if (iter == line_numbers_.end()) {
        line_numbers_.insert(lineNumber);
        storage[lineNumber] = line;
        ram[lineNumber] = &(info);
    }
    else {
        setParsedStatement(lineNumber, info);
        storage[lineNumber] = line;
    }
}

void Program::removeSourceLine(int lineNumber) {
    if(line_numbers_.find(lineNumber)!=line_numbers_.end()){
        line_numbers_.erase(lineNumber);
        storage.erase(lineNumber);
        ram[lineNumber]->kill();
        delete ram[lineNumber];
        ram.erase(lineNumber);
    }
    else{
        return;
    }
}

std::string Program::getSourceLine(int lineNumber) {
    std::string object = "\0";
    auto iter = storage.find(lineNumber);
    if (iter != storage.end()) {
        object = (*iter).second;
    }
    return object;
}

void Program::setParsedStatement(int lineNumber, Statement& new_info) {
    auto iter = ram.find(lineNumber);
    if (iter != ram.end()) {
        (*iter).second->kill();
        delete (*iter).second;
        (*iter).second = &new_info;
    }
    else {
        error("SYNTAX ERROR");
    }
}


Statement* Program::getParsedStatement(int lineNumber) {
    auto iter = ram.find(lineNumber);
    if (iter != ram.end()) {
        return (*iter).second;
    }
    else {
        return nullptr;
    }
}

//Returns the line number of the first line in the program.
//If the program has no lines, this method returns - 1.
int Program::getFirstLineNumber() {
    auto iter = line_numbers_.begin();
    if (iter == line_numbers_.end()) {
        return -1;
    }
    else {
        return (*iter);
    }
}

int Program::getLastLineNumber() {
    auto iter = line_numbers_.end();
    if (!line_numbers_.empty()) {
        --iter;
        return(*iter);
    }
    else {
        return -1;
    }
}

//Returns the line number of the first line in the program whose
//number is larger than the specified one, which must already exist
// in the program.If no more lines remain, this method returns - 1.
int Program::getNextLineNumber(int lineNumber) {
    auto iter = line_numbers_.find(lineNumber);
    ++iter;
    if (iter == line_numbers_.end()) {
        return -1;
    }
    else {
        return (*iter);
    }
}

void Program::list_program_() {
    if (Program::getFirstLineNumber() != -1) {
        for (auto iter = line_numbers_.begin();iter != line_numbers_.end();++iter) {
            std::cout << (*iter) << ' ' << storage[(*iter)] << '\n';
        }
    }
    return;
}

void Program::run_program_(EvalState& eval) {
    pointer = Program::getFirstLineNumber();
    max_line = Program::getLastLineNumber();
    while (pointer != -1) {
        int memory_now = pointer;
        (*ram[pointer]).execute(eval,*this);
        if (pointer == memory_now) {
            pointer = getNextLineNumber(pointer);
            continue;
        }
    }
    return;
}

bool Program::set_pointer(int object) {
    if (object == -1) {
        pointer = -1;
        return 1;
    }
    auto iter = line_numbers_.find(object);
    if (iter == line_numbers_.end()) {
        return 0;
    }
    else {
        pointer = object;
        return 1;
    }
}


