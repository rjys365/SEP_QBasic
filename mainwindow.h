#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //prompt to choose a file to load.
    void chooseFile();

    //raise an error dialog.
    void errDialog(const QString &reason);

    //raise a help dialog.
    void helpDialog();

    //output a line to output area.
    void outputLine(const QString &line);

    //ask for Input in the command area.
    void askForInput();

    void refresh();

private slots:
    void on_cmdLineEdit_editingFinished();

    void on_btnClearCode_clicked();

    void on_btnRunCode_clicked();

    void on_btnLoadCode_clicked();

private:
    Ui::MainWindow *ui;
    Program program;
};
#endif // MAINWINDOW_H
