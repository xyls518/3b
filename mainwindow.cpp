#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include "print.h"
#include "tool.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   QMessageBox::information(this,tr("信息提示"),tr("开始打印"),QMessageBox::Ok);

   Print print;

   print.isDebug = true;

   if(ui->checkBox->checkState() == Qt::Checked){
       print.turn = true;
   }else{
       print.turn = false;
   }
   print.fileName = ui->textEdit->toPlainText() == ""?ui->textEdit->placeholderText():ui->textEdit->toPlainText();
   if(!isFilesExist(print.fileName)){
      QMessageBox::critical(this,tr("信息提示"),tr("图片不存在"),QMessageBox::Ok);
      return;
   }


   if(print.beginPrint()){
       QMessageBox::information(this,tr("信息提示"),tr("成功"),QMessageBox::Ok);
   }else{
       QMessageBox::critical(this,tr("信息提示"),tr("失败"),QMessageBox::Ok);
   }

}
