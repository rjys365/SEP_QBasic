#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&this->program,&Program::chooseFile,this,&MainWindow::chooseFile);
    connect(&this->program,&Program::error,this,&MainWindow::errDialog);
    connect(&this->program,&Program::helpWindow,this,&MainWindow::helpDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseFile(){
    QString filename;
    bool ok=true;
    do{
        ok=true;
        try{
            filename=QFileDialog::getOpenFileName(nullptr,tr("选择打开文件"),".",tr("All Files(*.*)"));
            if(filename.isEmpty())return;
            program.loadFile(filename);
        }catch(...){
            ok=false;
            if(filename.isEmpty())break;
        }
    }while(!ok);
}

void MainWindow::errDialog(QString reason){
    QMessageBox::critical(this,"Error",reason);
}

void MainWindow::helpDialog(){
    QMessageBox::information(this,"Help Info","QBasic, a mini Basic language interpretor.\nSee the word document for more info.");
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

