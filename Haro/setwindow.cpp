#include "setwindow.h"
#include "ui_setwindow.h"

#include <QLabel>
#include <QPainter>

SetWindow::SetWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWindow)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();

    QPainter painter(&bmp);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(bmp.rect(), 50, 50);
    setMask(bmp);
    setWindowOpacity(0.95);
    setStyleSheet("background-color:white");

    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowStaysOnTopHint);

    this->setWindowIcon(QIcon(":/images/icon/setting.png"));
}

SetWindow::~SetWindow()
{
    delete ui;
}

void SetWindow::setSize(int size)
{
    haroSize = size;

    QFont font;
    font.setPointSize(12);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::blue);

    sizeLabel = new QLabel(this);
    sizeLabel->move(120, 100);
    sizeLabel->resize(120, 20);
    sizeLabel->setFont(font);
    sizeLabel->setPalette(pa);
    sizeLabel->setNum(haroSize);
    sizeLabel->show();

    ui->sizeSlider->setValue(haroSize);
}

int SetWindow::getSize()
{
    return haroSize;
}

void SetWindow::on_sizeSlider_valueChanged(int value)
{
    haroSize = value;
    setSize(haroSize);
}

