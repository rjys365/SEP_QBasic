#include "exp.h"
#include "exceptions.h"
#include <cmath>
#include <QStringList>
#include <QStack>

Exp::~Exp(){
    //delete left;
    //delete right;
}

int CompoundExp::eval()const{
    switch(op){
    case PLUS:
        if(left==nullptr)return right->eval();
        return left->eval()+right->eval();
    case MINUS:
        if(left==nullptr)return -right->eval();
        return left->eval()-right->eval();
    case MULTIPLY:
        return left->eval()*right->eval();
    case DIVIDE:{
        int leftv=left->eval();
        int rightv=right->eval();
        if(rightv==0)throw DivideByZero();
        return leftv/rightv;
    }
    case POWER:
        return pow(left->eval(),right->eval());
    default:
        return 0;
    }
}

QString CompoundExp::semTree(int indentation)const{
    if(left==nullptr)return QString("    ").repeated(indentation)+OPERATOR_STR[op]+"\n"
            +right->semTree(indentation+1);
    return QString("    ").repeated(indentation)+OPERATOR_STR[op]+"\n"
            +left->semTree(indentation+1)+"\n"
            +right->semTree(indentation+1);
}



std::shared_ptr<Exp> parseExp(QStringList str,VariablePool *vPool){
    static const QHash<QString,OperationType> OP_MAP={{"+",PLUS},{"-",MINUS},{"*",MULTIPLY},{"/",DIVIDE},{"**",POWER},{"(",LBRACKET},{")",RBRACKET}};
    QStack<OperationType> opStack;
    QStack<std::shared_ptr<Exp>> numStack;
    try{
        for(auto it=str.cbegin();it!=str.cend();it++){
            const QString &curToken=*it;
            if(OP_MAP.contains(curToken)){
                auto curOp=OP_MAP[curToken];
                //left bracket, push into stack
                if(curOp==LBRACKET){opStack.push(curOp);}
                else if(curOp==RBRACKET){
                    while(opStack.top()!=LBRACKET){
                        auto topOp=opStack.top();opStack.pop();
                        std::shared_ptr<Exp> opr1,opr2;
                        opr2=numStack.top();numStack.pop();
                        if(numStack.empty()){//specially proceed "+x" pattern.
                            if(topOp==PLUS||topOp==MINUS)numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,nullptr,opr2)));
                            else throw ResolveExpFailure();
                        }
                        else{
                            opr1=numStack.top();numStack.pop();
                            numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,opr1,opr2)));
                        }
                    }
                    opStack.pop();
                }
                else{//ordinary operators
                    while(!(opStack.empty()||opStack.top()==LBRACKET||OP_PRECEDENCE[opStack.top()]<OP_PRECEDENCE[curOp])){
                        auto topOp=opStack.top();opStack.pop();
                        std::shared_ptr<Exp> opr1,opr2;
                        opr2=numStack.top();numStack.pop();
                        if(numStack.empty()){
                            if(topOp==PLUS||topOp==MINUS)numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,nullptr,opr2)));
                            else throw ResolveExpFailure();
                        }
                        else{
                            opr1=numStack.top();numStack.pop();
                            numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,opr1,opr2)));
                        }
                    }
                    opStack.push(curOp);
                }
            }
            else if(curToken[0].isLetter()){
                numStack.push(std::make_shared<IdentifierExp>(IdentifierExp(vPool,curToken)));
            }
            else if(curToken[0].isDigit()){
                bool ok=true;
                int num=curToken.toInt(&ok);
                if(!ok)throw ResolveExpFailure();
                numStack.push(std::make_shared<ConstantExp>(ConstantExp(num)));
            }
            else throw ResolveExpFailure();
        }
        while(!opStack.empty()){
            auto topOp=opStack.top();
            opStack.pop();
            std::shared_ptr<Exp> opr1,opr2;
            opr2=numStack.top();numStack.pop();
            if(numStack.empty()){
                if(topOp==PLUS||topOp==MINUS)numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,nullptr,opr2)));
                else throw ResolveExpFailure();
            }
            else{
                opr1=numStack.top();numStack.pop();
                numStack.push(std::make_shared<CompoundExp>(CompoundExp(topOp,opr1,opr2)));
            }
        }
    }
    catch(...){
        throw ResolveExpFailure();
    }
    if(numStack.size()!=1)throw ResolveExpFailure();
    return numStack.top();
}
