#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QString>
#include <QMap>

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

    void clear();
    void loadFile(QString filename);
    void modifyLine(int lineno,QString line);
    QString dispLines();

    void parseCommand(QString cmd);

private:
    QMap<int,QString> lines;

signals:
    void lineOutput(QString line);
    void error(QString reason);
    void helpWindow();
    void chooseFile();
};

#endif // PROGRAM_H
