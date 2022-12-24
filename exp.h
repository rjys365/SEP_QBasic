#ifndef EXP_H
#define EXP_H

#include <QString>
#include <memory>
#include "variablepool.h"
#include <QHash>

enum OperationType{PLUS=0,MINUS=1,MULTIPLY=2,DIVIDE=3,POWER=4,LBRACKET=5,RBRACKET=6};
const QString OPERATOR_STR[]={"+","-","*","/","**","(",")"};
const int OP_PRECEDENCE[]={1,1,2,2,3,0,0};


class Exp
{
    friend std::unique_ptr<Exp> resolveExp(QStringList str);
protected:
    std::shared_ptr<Exp> left=nullptr,right=nullptr;
public:
    //get the value of the expression
    virtual int eval()const=0;

    //get part of the sementic tree, the argument is the previous indentation
    virtual QString semTree(int indentation)const=0;
    virtual ~Exp();
};

class ConstantExp : public Exp{
protected:
    int value;
public:
    int eval()const{return value;}
    QString semTree(int indentation)const{return QString("    ").repeated(indentation)+QString::number(value);}
    ~ConstantExp()=default;
    ConstantExp(int v):value(v){}
};

class IdentifierExp : public Exp{
protected:
    VariablePool *vPool;
    QString vName;
public:
    IdentifierExp(VariablePool *vp,QString vn):vPool(vp),vName(vn){}
    int eval()const{return vPool->getVal(vName);}
    QString semTree(int indentation)const{return QString("    ").repeated(indentation)+vName;}
};

class CompoundExp : public Exp{
protected:
    OperationType op;
public:
    CompoundExp(OperationType opr,std::shared_ptr<Exp> l,std::shared_ptr<Exp> r):op(opr){
        left=l,right=r;
    }
    int eval()const;
    QString semTree(int indentation)const;
};

std::shared_ptr<Exp> parseExp(QStringList str,VariablePool *vPool);

#endif // EXP_H
