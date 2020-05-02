#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <exception>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <string>
#include <SendMail.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDockNestingEnabled(true);

//----------------------------toolBar----------------------------
    //send
    QAction *toolSend = ui->toolBar->addAction("send");
    connect(toolSend,&QAction::triggered,this,
            [=](){
                sendEmail(ui);
            });

    //save
    QAction *toolSave = ui->toolBar->addAction("save");
    connect(toolSave,&QAction::triggered,this,
            [=](){
                save(ui);
            });

    ui->toolBar->addSeparator();

    //exit
    QAction *toolExit = ui->toolBar->addAction("exit");
    connect(toolExit,&QAction::triggered,this,
            [=](){
                int ret = QMessageBox::question(this,"Warning","Do you want to exit?",QMessageBox::Yes | QMessageBox::No);
                switch(ret)
                {
                    case QMessageBox::Yes:
                        MainWindow::close();
                        break;
                    case QMessageBox::No:
                        break;
                    default:
                        break;
                }
            });


//----------------------------toolBar----------------------------
    getFormEmails(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//function to send e-mail
void MainWindow::sendEmail(Ui::MainWindow *ui)
{
    QString content = ui->emailContent->toPlainText();
    if(0==content.length())
    {
        QMessageBox::warning(this,"Warning","empty email content...");
        return;
    }
    QString title = ui->emailTitle->text();
    if(0==title.length())
    {
        QMessageBox::warning(this,"Warning","empty email title...");
        return;
    }

    std::string emailTo = (ui->sALineEdit->text()).toStdString();
    std::string emailFrom = (ui->sALineEdit->text()).toStdString();
    std::string password = (ui->sPLineEdit->text()).toStdString();

    if(emailTo.empty() || emailFrom.empty() || password.empty())
    {
        QMessageBox::warning(this,"Warning","missing email details...");
        return;
    }
    //char EmailTo[] = "chenhaoyuan981215@163.com";    //此处是送达的邮箱, 需和EmailContents里的保持一致
    //char tpassword[] = "yaxfycdaufzdbffi";
    //char temailFrom[] = "1065619715@qq.com";

    std::string EmailContents = "From: \"Send\"<" + emailFrom + ">\r\n"
                                + "To: \"Receive\"<" + emailTo + ">\r\n"
                                + "Subject: " + title.toStdString() + "\r\n\r\n"
                                + content.toStdString() + "\n";

    qDebug()<<QString::fromStdString(EmailContents);

    if(EmailContents.length()>=10000)
    {
        QMessageBox::warning(this,"Warning","email too long...");
        return;
    }
    qDebug()<<QString::fromStdString(emailTo)<<endl;
    qDebug()<<QString::fromStdString(emailFrom)<<endl;
    qDebug()<<QString::fromStdString(password)<<endl;
    bool re = SendMail(emailTo, emailFrom, password, EmailContents.c_str());
    if(!re)
    {
        QMessageBox::warning(this,"Warning","send error, check something...");
        return;
    }
}

//function to get form e-mail list
void MainWindow::getFormEmails(Ui::MainWindow *ui)
{
    ui->emailTree->clear();
    QDir dir(fileDir);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);

    QFileInfoList list = dir.entryInfoList();

    QTreeWidgetItem *containerItem = new QTreeWidgetItem();
    containerItem->setText(0,"FormEmails");
    ui->emailTree->addTopLevelItem(containerItem);
    if(list.size() > 0)
    {
        for(int i=0;i<list.size();i++)
        {
            QFileInfo fileInfo = list.at(i);
            if(".txt"==fileInfo.fileName().right(4))
            {
                QTreeWidgetItem *newItem = new QTreeWidgetItem();
                newItem->setText(0,fileInfo.fileName().left(fileInfo.fileName().length()-4));

                containerItem->addChild(newItem);
                qDebug()<<fileInfo.baseName();
            }
        }
        connect(ui->emailTree,&QTreeWidget::itemDoubleClicked,this,&MainWindow::getFormEmailContent);
    }
}

//function to get e-mail content
void MainWindow::getFormEmailContent(QTreeWidgetItem *item, int col)
{
    qDebug()<<item->text(col);
    QFile *emailFile = new QFile(fileDir+"/"+item->text(col)+".txt");
    if(!emailFile->exists())
    {
        QMessageBox::warning(this,"Warning","this email does not exit...");
        return;
    }
    if(!emailFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Warning","this email can not read...");
        return;
    }
    //qDebug()<<item->text(col);

    QString content = emailFile->readAll();

    ui->emailTitle->setText(item->text(col));
    ui->emailContent->setPlainText(content);

    //qDebug()<<content;
    emailFile->close();
    delete emailFile;
    return;
}

//function to save e-mail
void MainWindow::save(Ui::MainWindow *ui)
{
    QString title = ui->emailTitle->text();
    if(title.length()==0)
    {
        QMessageBox::warning(this,"Warning","you must fill the e-mail title...");
        return;
    }

    QString content = ui->emailContent->toPlainText();
    QFile *file = new QFile(fileDir+"/"+title+".txt");
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Warning","unknow error...");
        return;
    }
    file->write(content.toUtf8());

    file->close();
    qDebug()<<"save";

    delete file;
    getFormEmails(ui);
}
