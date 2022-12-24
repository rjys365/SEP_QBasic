#ifndef VARIABLEPOOL_H
#define VARIABLEPOOL_H

#include <QString>
#include <QHash>

class VariablePool
{
private:
    QHash<QString,int> vars;
public:
    VariablePool();
    int getVal(QString vName)const;
    void setVal(QString vName, int val);
    void clear();
};

#endif // VARIABLEPOOL_H
