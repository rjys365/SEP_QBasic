#include "tokenizer.h"

QStringList tokenize(QString str){
    QStringList ret;
    str=str.simplified();
    //the string is simplified, so we can assume that space may only appear once at a time.
    int tokenStart=0,len=str.length();
    for(int i=0;i<len;i++){
        auto curChar=str[i];
        if(curChar.isSpace()){
            if(tokenStart<i){ret.append(str.mid(tokenStart,i-tokenStart));tokenStart=i+1;}
            else tokenStart=i+1;
        }
        else if(curChar=='*'){//specially process '*', since it may be a "**"
            if(i>tokenStart)ret.append(str.mid(tokenStart,i-tokenStart));
            if(str[i+1]=='*'){ret.append("**");tokenStart=i+2;i++;continue;}
            ret.append("*");tokenStart=i+1;
        }
// discard the special process for <= and >= since they don't exist.
//        else if(curChar=='<'||curChar=='>'){
//            if(i>tokenStart)ret.append(str.mid(tokenStart,i-tokenStart));
//            if(str[i+1]=='='){ret.append(curChar+"=");tokenStart=i+2;i++;continue;}
//            ret.append(QString(curChar));tokenStart=i+1;
//        }
        else if(curChar=='+'||curChar=='-'||curChar=='/'||curChar=='('||curChar==')'||curChar=='='||curChar=='<'||curChar=='>'){
            if(i>tokenStart)ret.append(str.mid(tokenStart,i-tokenStart));
            ret.append(QString(curChar));tokenStart=i+1;
        }
    }
    if(len>tokenStart)ret.append(str.mid(tokenStart));
    return ret;
}
