#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("app.qss");//路径为应用程序所在目录开始
            file.open(QFile::ReadOnly);
            QTextStream file_text(&file);
            QString str_ret= file_text.readAll();
    a.setStyleSheet(str_ret);
    MainWindow w;
    w.show();

    return a.exec();
}
