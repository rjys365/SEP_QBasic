#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QString>
#include <QMap>
#include "statement.h"
#include <memory>

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

    //clear the program.
    void clear();

    //load a program from file
    void loadFile(QString filename);

    //modify a single line
    void modifyLine(int lineno,QString line);

    //convert all the lines to a string
    QString dispLines();

    void parseCommand(QString cmd);

    //run the program until it stops / wait for input.
    void run();

    //reset the status of the program.
    void reset();

    //return the parsed sementic tree
    QString updSemTrees();

    bool isInputting()const;

    bool inputStr(const QString &str);

private:
    QMap<int,QString> lines;
    QMap<int,std::shared_ptr<Statement>> statements;
    QMap<int,QString> semTrees;
    VariablePool vPool;
    bool inputting=false;
    QString inputVarName;
    int pc=0;



signals:
    void refresh();
    void lineOutput(QString line);
    void askForInput();
    void error(QString reason);
    void helpWindow();
    void chooseFile();
};

#endif // PROGRAM_H
