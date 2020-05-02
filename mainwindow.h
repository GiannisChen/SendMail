#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    QString fileDir = "H:/FormEmails";

private:
    void sendEmail(Ui::MainWindow *);
    void getFormEmails(Ui::MainWindow *);
    void getFormEmailContent(QTreeWidgetItem *,int);
    void save(Ui::MainWindow *);
};
#endif // MAINWINDOW_H
