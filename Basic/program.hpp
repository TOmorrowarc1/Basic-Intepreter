/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 */

#ifndef _program_h
#define _program_h

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "statement.hpp"

class Statement;
/*
 * This class stores the lines in a BASIC program.  Each line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 */

class Program {

public:

/*
 * Constructor: Program
 * Usage: Program program;
 * -----------------------
 * Constructs an empty BASIC program.
 */

    Program();

/*
 * Destructor: ~Program
 * Usage: usually implicit
 * -----------------------
 * Frees any heap storage associated with the program.
 */

    ~Program();

/*
 * Method: clear
 * Usage: program.clear();
 * -----------------------
 * Removes all lines from the program.
 */

    void clear(EvalState&);

/*
 * Method: addSourceLine
 * Usage: program.addSourceLine(lineNumber, line);
 * -----------------------------------------------
 * Adds a source line to the program with the specified line number.
 * If that line already exists, the text of the line replaces
 * the text of any existing line and the parsed representation
 * (if any) is deleted.  If the line is new, it is added to the
 * program in the correct sequence.
 */

    void addSourceLine(int lineNumber, const std::string& line, Statement& info);

/*
 * Method: removeSourceLine
 * Usage: program.removeSourceLine(lineNumber);
 * --------------------------------------------
 * Removes the line with the specified number from the program,
 * freeing the memory associated with any parsed representation.
 * If no such line exists, this method simply returns without
 * performing any action.
 */

    void removeSourceLine(int lineNumber);

/*
 * Method: getSourceLine
 * Usage: string line = program.getSourceLine(lineNumber);
 * -------------------------------------------------------
 * Returns the program line with the specified line number.
 * If no such line exists, this method returns the empty string.
 */
    //获取某行程序陈述。
    std::string getSourceLine(int lineNumber);

/*
 * Method: setParsedStatement
 * Usage: program.setParsedStatement(lineNumber, stmt);
 * ----------------------------------------------------
 * Adds the parsed representation of the statement to the statement
 * at the specified line number.  If no such line exists, this
 * method raises an error.  If a previous parsed representation
 * exists, the memory for that statement is reclaimed.
 */

    //更改某行程序。
    void setParsedStatement(int lineNumber,Statement& stmt);

/*
 * Method: getParsedStatement
 * Usage: Statement *stmt = program.getParsedStatement(lineNumber);
 * ----------------------------------------------------------------
 * Retrieves the parsed representation of the statement at the
 * specified line number.  If no value has been set, this method
 * returns NULL.
 */
    //获取某行程序,若不存在，返回nullptr。
    Statement* getParsedStatement(int lineNumber);

/*
 * Method: getFirstLineNumber
 * Usage: int lineNumber = program.getFirstLineNumber();
 * -----------------------------------------------------
 * Returns the line number of the first line in the program.
 * If the program has no lines, this method returns -1.
 */

    int getFirstLineNumber();
    int getLastLineNumber();

/*
 * Method: getNextLineNumber
 * Usage: int nextLine = program.getNextLineNumber(lineNumber);
 * ------------------------------------------------------------
 * Returns the line number of the first line in the program whose
 * number is larger than the specified one, which must already exist
 * in the program.  If no more lines remain, this method returns -1.
 */

    int getNextLineNumber(int lineNumber);

    //依序列出程序
    void list_program_();

    //依序运行程序，直至pointer=-1(end)或 pointer>max_line.
    //输入变量库，每条程序的执行可以对变量库进行更改。
    void run_program_(EvalState&);

    //更改pointer,成功返回1，未成功返回0（包括设置为-1）
    bool set_pointer(int object);

private:
    int max_line;
    int pointer;
    std::set<int> line_numbers_;
    std::unordered_map<int, std::string> storage;
    std::unordered_map<int, Statement*> ram;
};

#endif
