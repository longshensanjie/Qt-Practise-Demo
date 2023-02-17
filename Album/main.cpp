#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/style/style.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        a.setStyleSheet(qss);
        file.close();
    }
    else
    {
        qDebug("open file failed");
        return -1;
    }

    MainWindow w;
    w.setWindowTitle("Ablum");
    w.showMaximized();
    return a.exec();
}
