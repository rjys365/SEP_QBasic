#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");
    program.parseCommand(cmd);
    ui->CodeDisplay->setText(program.dispLines());
    /*
    ui->CodeDisplay->append(cmd);
    ui->CodeDisplay->setText(program.dispLines());
    */

}

void MainWindow::on_btnClearCode_clicked()
{
    ui->CodeDisplay->clear();
}

