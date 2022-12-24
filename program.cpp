#include "program.h"
#include <QFile>
#include <QTextStream>
#include "statement.h"
#include "exceptions.h"
#include "tokenizer.h"

Program::Program(QObject *parent)
    : QObject{parent}
{

}

void Program::clear(){
    lines.clear();
    statements.clear();
    semTrees.clear();
    vPool.clear();
    inputting=false;
    pc=0;
    inputVarName="";
}

void Program::reset(){
    vPool.clear();
    inputting=false;
    pc=0;
    inputVarName="";
}

void Program::modifyLine(int lineno,QString line){
    try{
        if(line.simplified()==""){lines.remove(lineno);semTrees.remove(lineno);statements.remove(lineno);return;}
        else if(lines.contains(lineno))
            statements[lineno]=parseStatement(tokenize(line),line,&vPool);
        else
            statements.insert(lineno,parseStatement(tokenize(line),line,&vPool));
    }
    catch(ResolveExpFailure){
        emit error(QString("Failed to resolve expression at line ")+QString::number(lineno)+".");
        return;
    }
    catch(IllegalLetStatement){
        emit error(QString("Illegal LET statement found at line ")+QString::number(lineno)+".");
        return;
    }
    catch(IllegalInputStatement){
        emit error(QString("Illegal INPUT statement found at line ")+QString::number(lineno)+".");
        return;
    }
    catch(IllegalGotoStatement){
        emit error(QString("Illegal GOTO statement found at line ")+QString::number(lineno)+".");
        return;
    }
    catch(IllegalIfStatement){
        emit error(QString("Illegal IF statement found at line ")+QString::number(lineno)+".");
        return;
    }
    catch(IllegalEndStatement){
        emit error(QString("Illegal END statement found at line ")+QString::number(lineno)+".");
        return;
    }
    catch(UnknownStatement){
        emit error(QString("Unknown statement found at line ")+QString::number(lineno)+".");
        return;
    }
    if(lines.contains(lineno)){
        lines[lineno]=line;
        semTrees[lineno]=statements[lineno]->semTree();
    }
    else{
        lines.insert(lineno,line);
        semTrees.insert(lineno,statements[lineno]->semTree());
    }
    updSemTrees();
}

QString Program::dispLines(){
    QString ret;
    for(auto it=lines.cbegin();it!=lines.cend();it++){
        ret=ret+QString::number(it.key())+it.value()+'\n';
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
    emit refresh();
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
        if(cmd.simplified()==QString::number(num))modifyLine(num,"");
        else{
            auto insStr=cmd.mid(cmd.indexOf(' '));
            modifyLine(num,insStr);
        }
    }
    else{
        if(cmd=="RUN"){
            run();
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

QString Program::updSemTrees(){
    QString str;
    for(auto it=semTrees.cbegin();it!=semTrees.cend();it++){
        str.append(QString::number(it.key())+" "+it.value()+"\n");
    }
    return str;
}

bool Program::isInputting()const{
    return inputting;
}

bool Program::inputStr(const QString &str){
    bool ok=true;
    int val=str.toInt(&ok);
    if(!ok){
        emit error("Unable to resolve input");
        return false;
    }
    vPool.setVal(inputVarName,val);
    inputting=false;
    inputVarName="";
    run();
    return true;
}

void Program::run(){
    auto curIt=statements.lowerBound(pc);
    while(1){
        if(curIt==statements.end()){
            reset();
            break;
        }
        pc=curIt.key();
        auto curSt=curIt.value();
        QString vn;int nextins;
        int status=-1;
        try{
            status=curSt->eval(vn,nextins);
        }
        catch(VariableNotExist){
            emit error(QString("Variable not exist at line ")+QString::number(pc));
            reset();
            return;
        }
        catch(DivideByZero){
            emit error(QString("Divide by zero at line ")+QString::number(pc));
            reset();
            return;
        }

        switch(status){
        case 0:
            curIt++;
            break;
        case 1:
            reset();
            return;
        case 2:
            pc+=1;
            inputVarName=vn;
            inputting=true;
            emit askForInput();
            return;
        case 3:
            curIt=statements.lowerBound(nextins);
            if(curIt!=statements.end()){
                pc=curIt.key();
            }
            else{
                reset();
                return;
            }
            break;
        case 4:
            emit lineOutput(QString::number(nextins));
            curIt++;
            continue;
            //TODO
        }

    }
}
