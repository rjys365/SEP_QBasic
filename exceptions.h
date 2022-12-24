//This file contains various exceptions that will be used in the program.

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

//exceptions that may be thrown during the execution of programs.
struct VariableNotExist{};
struct DivideByZero{};
struct ResolveExpFailure{};

//exceptions that may be thrown when parsing statements.
struct IllegalLetStatement{};
struct IllegalInputStatement{};
struct IllegalGotoStatement{};
struct IllegalIfStatement{};
struct IllegalEndStatement{};
struct UnknownStatement{};

#endif // EXCEPTIONS_H
