#include "widget.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/res/qss/style-1.qss");
    file.open(QIODevice::ReadOnly);

    QTextStream fileText(&file);
    QString styleSheet = fileText.readAll();
    app.setStyleSheet(styleSheet);
    file.close();

    Widget w;
    w.show();
    return app.exec();
}
