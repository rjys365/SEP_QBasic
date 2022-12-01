#include "program.h"

Program::Program(QObject *parent)
    : QObject{parent}
{

}

void Program::clear(){
    lines.clear();
}

void Program::modifyLine(int lineno,QString line){
    if(lines.contains(lineno)){
        lines[lineno]=line;
    }
    else{
        lines.insert(lineno,line);
    }
}

QString Program::dispLines(){
    QString ret;
    for(auto it=lines.cbegin();it!=lines.cend();it++){
        ret=ret+QString::number(it.key())+' '+it.value()+'\n';
    }
    return ret;
}

void Program::parseCommand(QString cmd){
    cmd=cmd.simplified();
    if(cmd[0].isNumber()){
        auto numStr=cmd.left(cmd.indexOf(' '));
        bool ok=true;
        int num=numStr.toInt(&ok);
        if(!ok)return;//TODO: error message
        auto insStr=cmd.mid(cmd.indexOf(' '));
        modifyLine(num,insStr);
    }
    else{

    }
}
