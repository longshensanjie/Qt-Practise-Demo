#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineE_pwd->setEchoMode(QLineEdit::Password);

    connect(ui->btn_1, &QPushButton::clicked, this, &Widget::setStyle);
    connect(ui->btn_2, &QPushButton::clicked, this, &Widget::setStyle);
    connect(ui->btn_3, &QPushButton::clicked, this, &Widget::setStyle);
    connect(ui->btn_4, &QPushButton::clicked, this, &Widget::setStyle);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::setStyle()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    QString filePath;
    if("btn_1" == btn->objectName())
        filePath = ":/res/qss/style-1.qss";
    else if("btn_2" == btn->objectName())
        filePath = ":/res/qss/style-2.qss";
    else if("btn_3" == btn->objectName())
        filePath = ":/res/qss/style-3.qss";
    else if("btn_4" == btn->objectName())
        filePath = ":/res/qss/style-4.qss";

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream fileText(&file);
    QString stlyleSheet = fileText.readAll();
    this->setStyleSheet(stlyleSheet);
    file.close();
}

