#include "variablepool.h"
#include "exceptions.h"

VariablePool::VariablePool()
{

}

int VariablePool::getVal(QString vName)const{
    if(!vars.contains(vName))throw VariableNotExist();
    return vars[vName];
}

void VariablePool::setVal(QString vName,int val){
    vars[vName]=val;
}

void VariablePool::clear(){
    vars.clear();
}
