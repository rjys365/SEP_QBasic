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
    void chooseFile();
    void errDialog(QString reason);
    void helpDialog();

private slots:
    void on_cmdLineEdit_editingFinished();

    void on_btnClearCode_clicked();

private:
    Ui::MainWindow *ui;
    Program program;
};
#endif // MAINWINDOW_H
