#include "program.h"
#include <QFile>
#include <QTextStream>

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

void Program::loadFile(QString filename){
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))return;
    clear();
    QTextStream fileIn(&file);
    while(!fileIn.atEnd()){
        QString line=fileIn.readLine();
        parseCommand(line);
    }
    file.close();
}

void Program::parseCommand(QString cmd){
    cmd=cmd.simplified();
    if(cmd.length()==0)return;
    if(cmd[0].isNumber()){
        auto numStr=cmd.left(cmd.indexOf(' '));
        bool ok=true;
        int num=numStr.toInt(&ok);
        if(!ok){
            emit error(tr("Unable to resolve line number!"));
            return;//TODO: error message
        }
        auto insStr=cmd.mid(cmd.indexOf(' '));
        modifyLine(num,insStr);
    }
    else{
        if(cmd=="RUN"){
            //TODO
        }
        else if(cmd=="LOAD"){
            emit chooseFile();
        }
        else if(cmd=="CLEAR"){
            clear();
        }
        else if(cmd=="HELP"){
            emit helpWindow();
        }
        else if(cmd=="QUIT"){
            exit(0);
        }
        else{
            emit error("Unknown command.");
        }
    }
}
