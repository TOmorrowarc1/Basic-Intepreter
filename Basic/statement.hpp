/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();
    virtual void kill();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */
    //这些执行对EVAL与Pro起作用。
    virtual void execute(EvalState& state, Program& program) = 0;

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement. If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class Command :public Statement {
private:
    enum type {
        RUN,LIST,CLEAR
    };
    type type;
public:
    Command(TokenScanner& token);
    virtual void execute(EvalState& state, Program& program);
};

class Control :public Statement {
private:
    int object_pointer_;
public:
    Control();
    ~Control();
    void Set(int a);
    virtual void kill();
    virtual void jump(Program& program);
};

class GOTO :public Control {
private:
public:
    GOTO(TokenScanner& token);
    virtual void execute(EvalState& state, Program& program);
};

class IF :public Control {
private:
    Expression* lhs;
    Expression* rhs;
    char compare;
public:
    IF(TokenScanner& token);
    ~IF();
    virtual void kill();
    virtual void execute(EvalState& state, Program& program);
};

class END :public Control {
private:
public:
    END(TokenScanner& token);
    virtual void execute(EvalState& state, Program& program);
};

class Sequential :public Statement {
private:
    enum type1 {
        REM, LET, INPUT, PRINT
    };
    type1 type;
    std::string input;
    Expression* exp;
public:
    Sequential(TokenScanner& token);
    ~Sequential();
    virtual void kill();
    virtual void execute(EvalState& state, Program& program);
};

#endif