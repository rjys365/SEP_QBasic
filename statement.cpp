#include "statement.h"
#include "exceptions.h"
/*
Statement::Statement()
{

}
*/

LetStatement::LetStatement(const QString &vn,std::shared_ptr<Exp> ex,VariablePool *vp):varName(vn),exp(ex),vPool(vp){}

//the return value of eval function indicates the status of program.
//0 is for normally executing the next instruction
//1 is for exiting the program
//2 is for waiting for input
//3 is for jumping
//4 is for outputting the value (reusing nextins)

int LetStatement::eval(QString &,int &){
    vPool->setVal(varName,exp->eval());
    return 0;
}

QString LetStatement::semTree()const{
    return QString("LET =\n    ")+varName+"\n"+exp->semTree(1);
}

RemStatement::RemStatement(const QString &s):str(s){}

QString RemStatement::semTree()const{
    return QString("REM\n    ")+str;
}

int RemStatement::eval(QString &,int &){
    return 0;
}

PrintStatement::PrintStatement(std::shared_ptr<Exp> ex,VariablePool *):exp(ex){}

QString PrintStatement::semTree()const{
    return QString("PRINT\n")+exp->semTree(1);
}

int PrintStatement::eval(QString &,int &nextins){
    nextins=exp->eval();
    return 4;
}

InputStatement::InputStatement(const QString &vn):varName(vn){}

QString InputStatement::semTree()const{
    return QString("INPUT\n    ")+varName;
}

int InputStatement::eval(QString &inVarName,int &){
    inVarName=varName;
    return 2;
}

GotoStatement::GotoStatement(int d):dest(d){}

int GotoStatement::eval(QString &,int &nextins){
    nextins=dest;
    return 3;
}

QString GotoStatement::semTree()const{
    return QString("GOTO\n    ")+QString::number(dest);
}

IfStatement::IfStatement(std::shared_ptr<Exp> le,std::shared_ptr<Exp> re,CompOperator o,int dst):lExp(le),rExp(re),op(o),dest(dst){}

int IfStatement::eval(QString &,int &nextins){
    bool jump;
    switch(op){
    case EQUAL:
        jump=(lExp->eval()==rExp->eval());
        break;
    case LESS:
        jump=(lExp->eval()<rExp->eval());
        break;
    case GREATER:
        jump=(lExp->eval()>rExp->eval());
        break;
    }
    if(jump){
        nextins=dest;
        return 3;
    }
    return 0;
}

QString IfStatement::semTree()const{
    return QString("IF THEN\n    ")+COMP_STR[op]+"\n"+lExp->semTree(2)+"\n"+rExp->semTree(2)+"\n    "+QString::number(dest);
}

int EndStatement::eval(QString &,int &){
    return 1;
}

QString EndStatement::semTree()const{
    return QString("END");
}

std::shared_ptr<Statement> parseStatement(QStringList str,QString rawStr,VariablePool *vPool){
    if(str[0]=="LET"){
        int eqPos=str.indexOf("="),eqPos1=str.lastIndexOf("=");
        if(eqPos!=eqPos1)throw IllegalLetStatement();//multiple equal marks
        if(eqPos!=2)throw IllegalLetStatement();//equal mark appears in the wrong place
        if(!str[1][0].isLetter())throw IllegalLetStatement();//illegal variable name
        auto varName=str[1];
        auto exp=parseExp(str.mid(3),vPool);
        return std::make_shared<LetStatement>(LetStatement(varName,exp,vPool));
    }
    else if(str[0]=="REM"){
        return std::make_shared<RemStatement>(RemStatement(rawStr.mid(4)));
    }
    else if(str[0]=="PRINT"){
        auto exp=parseExp(str.mid(1),vPool);
        return std::make_shared<PrintStatement>(PrintStatement(exp,vPool));
    }
    else if(str[0]=="INPUT"){
        if(str.size()>2)throw IllegalInputStatement();
        QString vn=str[1];
        if(!vn[0].isLetter())throw IllegalInputStatement();
        return std::make_shared<InputStatement>(InputStatement(vn));
    }
    else if(str[0]=="GOTO"){
        if(str.size()>2)throw IllegalGotoStatement();
        bool ok=true;
        int dest=str[1].toInt(&ok);
        if(!ok||dest<0)throw IllegalGotoStatement();
        return std::make_shared<GotoStatement>(GotoStatement(dest));
    }
    else if(str[0]=="IF"){
        if(str.count("<")+str.count("=")+str.count(">")!=1)throw IllegalIfStatement();
        if(str.count("THEN")!=1)throw IllegalIfStatement();
        int thenPos=str.indexOf("THEN");
        if(str.length()!=thenPos+2)throw IllegalIfStatement();
        bool ok=true;
        int dest=str[str.length()-1].toInt(&ok);
        if(!ok)throw IllegalIfStatement();
        std::shared_ptr<Exp> lExp,rExp;
        int opPos;CompOperator op;
        if(str.contains("=")){
            opPos=str.indexOf("=");
            op=EQUAL;
        }
        else if(str.contains("<")){
            opPos=str.indexOf("<");
            op=LESS;
        }
        else {
            opPos=str.indexOf(">");
            op=GREATER;
        }
        if(opPos==1||opPos==thenPos-1)throw IllegalIfStatement();
        auto lStr=str.mid(1,opPos-1);
        auto rStr=str.mid(opPos+1,thenPos-opPos-1);
        lExp=parseExp(lStr,vPool);
        rExp=parseExp(rStr,vPool);
        return std::make_shared<IfStatement>(IfStatement(lExp,rExp,op,dest));
    }
    else if(str[0]=="END"){
        if(str.size()!=1)throw IllegalEndStatement();
        return std::make_shared<EndStatement>(EndStatement());
    }
    throw UnknownStatement();
}
