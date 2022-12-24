#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include "exp.h"
#include <memory>

enum CompOperator{EQUAL=0,LESS=1,GREATER=2};
const QString COMP_STR[]={"=","<",">"};

class Statement
{
protected:
public:
    virtual ~Statement()=default;
    //the return value of eval function indicates the status of program.
    //0 is for normally executing the next instruction
    //1 is for exiting the program
    //2 is for waiting for input
    //3 is for jumping
    //4 is for outputting the value (reusing nextins)
    //inVarName is for INPUT statements
    //nextins is used for jump statements or PRINT statement
    virtual int eval(QString &inVarName,int &nextins)=0;
    virtual QString semTree()const=0;
};

class LetStatement : public Statement{
protected:
    QString varName;
    std::shared_ptr<Exp> exp;
    VariablePool *vPool;
public:
    LetStatement(const QString &vn,std::shared_ptr<Exp> ex,VariablePool *vp);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class RemStatement : public Statement{
protected:
    QString str;
public:
    RemStatement(const QString &s);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class PrintStatement : public Statement{
protected:
    std::shared_ptr<Exp> exp;
public:
    PrintStatement(std::shared_ptr<Exp> ex,VariablePool *vp);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class InputStatement : public Statement{
protected:
    QString varName;
public:
    InputStatement(const QString &vn);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class GotoStatement : public Statement{
protected:
    int dest;
public:
    GotoStatement(int dst);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class IfStatement : public Statement{
protected:
    std::shared_ptr<Exp>lExp,rExp;
    CompOperator op;
    int dest;
public:
    IfStatement(std::shared_ptr<Exp> lExp,std::shared_ptr<Exp> rExp,CompOperator op,int dst);
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

class EndStatement : public Statement{
public:
    EndStatement()=default;
    int eval(QString &inVarName,int &nextins);
    QString semTree()const;
};

//parse a statement as a string list and return a shared_ptr to the resolved statement.
//rawStr is used specifically for REM instructions.
std::shared_ptr<Statement> parseStatement(QStringList str,QString rawStr,VariablePool *vPool);



#endif // STATEMENT_H
