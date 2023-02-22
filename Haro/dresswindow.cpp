#include "dresswindow.h"
#include "ui_dresswindow.h"
#include "const.h"

#include <QPainter>
#include <QWheelEvent>
#include <QPushButton>
#include <QButtonGroup>

DressWindow::DressWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DressWindow)
{
    ui->setupUi(this);

    QBitmap bmp(this->size());//设置圆角边框
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),50,50);
    setMask(bmp);
    setWindowOpacity(0.95);//设置透明度
    setStyleSheet("background-color:white;");

    Qt::WindowFlags m_flags = windowFlags();//保持窗口置顶1
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint);//保持窗口置顶2

    this->setWindowIcon(QIcon(":/images/icon/dress.png")); //设置窗口图标

    posY = 0;

    initBtn();
}

DressWindow::~DressWindow()
{
    delete ui;
}

void DressWindow::accept(QVector<QPixmap> &bodyVec, QVector<QPixmap> &earsVec, int bodyIndex, int earsIndex)
{
    this->bodyVec = bodyVec;
    this->earsVec = earsVec;
    this->bodyIndex = bodyIndex;
    this->earsIndex = earsIndex;
    bodyBtnArr[bodyIndex]->setChecked(true);
    earsBtnArr[earsIndex]->setChecked(true);
}

int DressWindow::getBodyIndex()
{
    return bodyIndex;
}

int DressWindow::getEarsIndex()
{
    return earsIndex;
}

void DressWindow::paintEvent(QPaintEvent *event)
{
    static QPixmap dummy(":/images/appearance/body/dummy.png");
    QPainter painter(this);


    for(int i =0; i < DRESS_NUM; ++i)
    {
        painter.drawPixmap(0, posY + i *240, 240, 240, bodyVec[i]);
        painter.drawPixmap(200, posY + i * 240, 240, 240, dummy);
        painter.drawPixmap(200, posY + i * 240, 240, 240, earsVec[i]);
    }
}

void DressWindow::wheelEvent(QWheelEvent *event)
{
    static int speed = 40;
    if(event->angleDelta().y() > 0)
    {
        if(posY < 0)
            posY += speed;
    }
    else
    {
        if(posY > -230 * DRESS_NUM + 460)
            posY -= speed;
    }

    for(int i = 0;i < DRESS_NUM; i++){
        bodyBtnArr[i]->move(80, posY + i * 240 + 210);
        earsBtnArr[i]->move(280,posY + i * 240 + 210);
    }
    update();
}

void DressWindow::initBtn()
{
    setStyleSheet("QPushButton{border:none;"
                  "background-color:rgb(200,200,200)"
                  ";border-radius: 15px;"
                  "border-style:outset;}"
                  "QPushButton::hover{background-color:rgb(170,200,255);}"
                  "QPushButton:checked{background-color:rgb(100,120,230);}");

    bodyBox = new QButtonGroup;
    bodyBox->setExclusive(true);
    earsBox = new QButtonGroup;
    earsBox->setExclusive(true);
    for(int i = 0; i < DRESS_NUM; ++i)
    {
        bodyBtnArr[i] = new QPushButton(this);
        bodyBtnArr[i]->setFixedSize(80, 40);
        bodyBtnArr[i]->move(80, posY + i* 230 + 210);
        bodyBtnArr[i]->setIcon(QIcon(":/images/icon/choose.png"));
        bodyBtnArr[i]->setCheckable(true);
        bodyBox->addButton(bodyBtnArr[i], i);

        earsBtnArr[i] = new QPushButton(this);
        earsBtnArr[i]->setFixedSize(80,40);
        earsBtnArr[i]->move(280,posY + i*230 + 210);
        earsBtnArr[i]->setIcon(QIcon(":/images/icon/choose.png"));
        earsBtnArr[i]->setCheckable(true);
        earsBox->addButton(earsBtnArr[i],i);

    }

    connect(bodyBox, &QButtonGroup::buttonClicked,
            this,&DressWindow::bodyChange);
    connect(earsBox,
            &QButtonGroup::buttonClicked,
            this,&DressWindow::earsChange);
}


void DressWindow::bodyChange(QAbstractButton *btn)
{
    bodyIndex = bodyBox->id(btn);
}

void DressWindow::earsChange(QAbstractButton *btn)
{
    earsIndex = earsBox->id(btn);
}
