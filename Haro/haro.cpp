
#include "haro.h"
#include "ui_haro.h"
#include "const.h"
#include "dresswindow.h"
#include "musicwindow.h"
#include "setwindow.h"

#include <QDir>
#include <QPair>
#include <QFile>
#include <QTimer>
#include <QString>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QCalendarWidget>
#include <QSystemTrayIcon>
#include <QRandomGenerator>


Haro::Haro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Haro)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowStaysOnTopHint);

    QFile file("./file/file.dat");
    file.open(QFile::ReadOnly);
    QDataStream in(&file);
    int cordX, cordY;
    if(file.isOpen())
    {
        in >> size >> bodyIndex >> earsIndex >> cordX >> cordY;
    }
    else
    {
        size = 400;
        bodyIndex = 0;
        earsIndex = 0;
        cordX = x();
        cordY = y();
    }
    file.close();
    move(cordX, cordY);

    timer = new QTimer(this);
    timer->start(40);
    connect(timer, &QTimer::timeout, this, &Haro::eyesMovement);

    loadBodyPic();
    loadEyesMovePic();
    loadSpeMovePic();

    bodyImage = new QLabel(this);
    eyesImage = new QLabel(this);
    stripeImage = new QLabel(this);
    earsImage = new QLabel(this);

    setImage(bodyImage, bodyVec[bodyIndex]);
    setImage(eyesImage, eyesPixmap);

    if(size > 140)
    {
        setImage(stripeImage, stripePixmap);
        stripeImage->show();
    }
    else
        stripeImage->hide();

    setImage(earsImage, ears1Vec[earsIndex]);

    initBtn();
    initSystemTray();

}

Haro::~Haro()
{
    delete ui;
    delete earsImage;
    delete bodyImage;
    delete eyesImage;
    delete stripeImage;

    delete closeBtn;
    delete dressBtn;
    delete moreBtn;
    delete minBtn;
    delete setBtn;
    delete musicBtn;
    delete gameBtn;
    delete calenBtn;

    delete dressWindow;
    delete setWindow;
    delete calenWidget;
    delete musicWindow;
    delete systemTray;
}

void Haro::loadBodyPic()
{
    bodyVec.emplaceBack(":/images/appearance/body/def_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/blue_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/pink_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/icefire_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/cat_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/Gundam_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/drill_body.png");
    bodyVec.emplaceBack(":/images/appearance/body/angel_body.png");

    ears1Vec.emplaceBack(":/images/appearance/ears/def_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/blue_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/pink_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/icefire_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/cat_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/Gundam_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/drill_ears1.png");
    ears1Vec.emplaceBack(":/images/appearance/ears/angel_ears1.png");

    ears2Vec.emplaceBack(":/images/appearance/ears/def_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/blue_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/pink_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/icefire_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/cat_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/Gundam_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/drill_ears2.png");
    ears2Vec.emplaceBack(":/images/appearance/ears/angel_ears2.png");

    eyesPixmap.load(":/images/appearance/eyes/def_eyes.png");
    stripePixmap.load(":/images/appearance/stripe.png");

}

void Haro::loadEyesMovePic()
{
    int index = 0;
    for(int i = 1; i <= BLANK_PIC_FRAME_NUM; ++i)
        eyeMovePicVec.emplaceBack(QString(":/images/movement/blink/%1.png").arg(i));
    faceFramesVec.push_back(qMakePair(index, BLANK_PIC_FRAME_NUM));
    index += BLANK_PIC_FRAME_NUM;
    for(int i = 1; i <= HEART_PIC_FRAME_NUM; ++i)
        eyeMovePicVec.emplaceBack(QString(":/images/movement/heart/%1.png").arg(i));
    faceFramesVec.push_back(qMakePair(index, HEART_PIC_FRAME_NUM));
    index += HEART_PIC_FRAME_NUM;
    for(int i = 1; i <= QUESTION_PIC_FRAME_NUM; ++i)
        eyeMovePicVec.emplaceBack(QString(":/images/movement/question/%1.png").arg(i));
    faceFramesVec.push_back(qMakePair(index, QUESTION_PIC_FRAME_NUM));
    index += QUESTION_PIC_FRAME_NUM;
    for(int i = 1; i <= CLOSE_EYES_PIC_FRAME_NUM; ++i)
        eyeMovePicVec.emplaceBack(QString(":/images/movement/closeEyes/%1.png").arg(i));
    faceFramesVec.push_back(qMakePair(index, CLOSE_EYES_PIC_FRAME_NUM));
    index += CLOSE_EYES_PIC_FRAME_NUM;
    for(int i = 1; i <= WINK_PIC_FRAME_NUM; ++i)
        eyeMovePicVec.emplaceBack(QString(":/images/movement/wink/%1.png").arg(i));
    faceFramesVec.push_back(qMakePair(index, WINK_PIC_FRAME_NUM));

    faceIndex = -1;
}

void Haro::loadSpeMovePic()
{
    for(int i = 1; i <= ERROR_PIC_FRAME_NUM; ++i)
        speMovePicVec.emplaceBack(QString(":/images/movement/error/%1.png").arg(i));
    for(int i = 1; i <= FLY_PIC_FRAME_NUM; ++i)
        speMovePicVec.emplaceBack(QString(":/images/movement/fly/%1.png").arg(i));
    speMoveIndex = -1;
}

void Haro::setImage(QLabel *image, QPixmap pixmap)
{
    image->setPixmap(pixmap.scaled(size, size));
    image->setScaledContents(true);
    image->resize(size, size);
    image->move(this->frameGeometry().width() / 2 - size / 2,
                this->frameGeometry().height() / 2 - size/ 2);
}

void Haro::initBtn()
{
    closeBtn = new QPushButton(this);
    closeBtn->setIcon(QIcon(":/images/icon/close.png"));

    dressBtn = new QPushButton(this);
    dressBtn->setIcon(QIcon(":/images/icon/dress.png"));

    moreBtn = new QPushButton(this);
    moreBtn->setIcon(QIcon(":/images/icon/more.png"));

    minBtn = new QPushButton(this);
    minBtn->setIcon(QIcon(":/images/icon/min.png"));

    setBtn = new QPushButton(this);
    setBtn->setIcon(QIcon(":/images/icon/setting.png"));

    musicBtn = new QPushButton(this);
    musicBtn->setIcon(QIcon(":/images/icon/music.png"));

    gameBtn = new QPushButton(this);
    gameBtn->setIcon(QIcon(":/images/icon/game.png"));

    calenBtn = new QPushButton(this);
    calenBtn->setIcon(QIcon(":/images/icon/calendar.png"));

    reInitBtn();

    setStyleSheet("QPushButton{border:4px solid black;"
                  "background-color:rgb(200,210,255);border-radius: 10px;}"
                  "QPushButton::hover{background-color:rgb(170,200,255);}"
                  "QPushButton:pressed{background-color:rgb(60,70,200);}");

    dressWindow = new DressWindow;
    dressWindow->accept(bodyVec, ears1Vec, bodyIndex, earsIndex);

    musicWindow = new MusicWindow;

    setWindow = new SetWindow;
    setWindow->setSize(size);

    calenWidget = new QCalendarWidget;
    calenWidget->setWindowFlags(Qt::FramelessWindowHint);
    calenWidget->setWindowIcon(QIcon(":/images/icon/calendar.png"));
    calenWidget->resize(600, 400);

    connect(closeBtn, &QPushButton::clicked, this, &Haro::closeBtnClicked);
    connect(dressBtn, &QPushButton::clicked, this, &Haro::dressBtnClicked);
    connect(moreBtn, &QPushButton::clicked, this, &Haro::moreBtnClicked);
    connect(minBtn, &QPushButton::clicked, this, &Haro::minBtnClicked);
    connect(setBtn, &QPushButton::clicked, this, &Haro::setBtnClicked);
    connect(musicBtn, &QPushButton::clicked, this, &Haro::musicBtnClicked);
    connect(gameBtn, &QPushButton::clicked, this, &Haro::gameBtnClicked);
    connect(calenBtn, &QPushButton::clicked, this, &Haro::calenBtnClicked);

    btnSwitch1 = 0;
    btnSwitch2 = 0;
    btnSwitchRole();
}

void Haro::reInitBtn()
{
    btnSize = size;
    if(btnSize < 300)
        btnSize = 300;

    int btnX = this->frameGeometry().width() / 2 - btnSize * 3 / 5 - 5;
    int btnY = this->frameGeometry().height() / 2 - btnSize / 4;
    int btnWidth = btnSize / 5;
    int btnHeight = btnSize / 8;

    closeBtn->setGeometry(btnX, btnY, btnWidth, btnHeight);
    dressBtn->setGeometry(btnX, btnY + btnSize / 6, btnWidth, btnHeight);
    moreBtn->setGeometry(btnX, btnY + 2 * btnSize / 6, btnWidth, btnHeight);
    minBtn->setGeometry(btnX, btnY + 3 * btnSize / 6, btnWidth, btnHeight);

    setBtn->setGeometry(btnX - btnWidth * 1.2, btnY, btnWidth, btnHeight);
    musicBtn->setGeometry(btnX - btnWidth * 1.2, btnY + btnSize / 6, btnWidth, btnHeight);
    gameBtn->setGeometry(btnX -  btnWidth * 1.2, btnY + 2 * btnSize / 6, btnWidth, btnHeight);
    calenBtn->setGeometry(btnX -  btnWidth * 1.2, btnY + 3 * btnSize / 6, btnWidth, btnHeight);

    QSize tempSize(btnSize / 8, btnSize / 8);
    closeBtn->setIconSize(tempSize);
    dressBtn->setIconSize(tempSize);
    moreBtn->setIconSize(tempSize);
    minBtn->setIconSize(tempSize);
    setBtn->setIconSize(tempSize);
    musicBtn->setIconSize(tempSize);
    gameBtn->setIconSize(tempSize);
    calenBtn->setIconSize(tempSize);
}

void Haro::initSystemTray()
{
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/images/icon/haro_icon.ico"));
    systemTray->setToolTip("Hello, I'm haro.");
    systemTray->show();
    connect(systemTray, &QSystemTrayIcon::activated, this, &Haro::systemTrayActivated);
}

void Haro::btnSwitchRole()
{
    closeBtn->setVisible(btnSwitch1);
    dressBtn->setVisible(btnSwitch1);
    moreBtn->setVisible(btnSwitch1);
    minBtn->setVisible(btnSwitch1);
    setBtn->setVisible(btnSwitch2);
    musicBtn->setVisible(btnSwitch2);
    gameBtn->setVisible(btnSwitch2);
    calenBtn->setVisible(btnSwitch2);

    musicWindow->move(x()+frameGeometry().width()/2
                          -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-musicWindow->frameGeometry().width(),
                      y()+frameGeometry().height()/2-size/5
                          -musicWindow->frameGeometry().height()/2);
    //移动窗口坐标↓
    calenWidget->move(x()+frameGeometry().width()/2
                          -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-calenWidget->frameGeometry().width(),
                      y()+frameGeometry().height()/2-size/5
                          -calenWidget->frameGeometry().height()/2);
    setWindow->move(x()+frameGeometry().width()/2
                        -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-setWindow->frameGeometry().width(),
                    y()+frameGeometry().height()/2-size/5
                        -setWindow->frameGeometry().height()/2);
}

void Haro::specialMovement()
{
    static int flag = 0;
    if(speMoveIndex == 0)
    {
        if(flag % 20 == 0 && flag <= 200)
            eyesImage->setPixmap(speMovePicVec[flag / 20].scaled(size, size));
        else if(flag > 300)
        {
            eyesImage->setPixmap(eyesPixmap.scaled(size, size));
            flag = 0;
            speMoveIndex = -1;
            return;
        }
    }
    flag++;
}

void Haro::saveData()
{
    QFile file("./file/file.dat");
    file.open(QFile::WriteOnly);
    QDataStream out(&file);
    if(file.isOpen())
        out << size << bodyIndex << earsIndex << x() << y();
    file.close();
}

void Haro::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(event->globalPosition().toPoint()-moveLeftTop);
        dressWindow->move(x()+frameGeometry().width()/2-10
                              -btnSize*0.6-dressWindow->frameGeometry().width(),
                          y()+frameGeometry().height()/2-150
                              -dressWindow->frameGeometry().height()/2);

        setWindow->move(x()+frameGeometry().width()/2
                              -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-setWindow->frameGeometry().width(),
                          y()+frameGeometry().height()/2-size/5
                              -setWindow->frameGeometry().height()/2);

        musicWindow->move(x()+frameGeometry().width()/2
                              -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-musicWindow->frameGeometry().width(),
                          y()+frameGeometry().height()/2-size/5
                              -musicWindow->frameGeometry().height()/2);

        calenWidget->move(x()+frameGeometry().width()/2
                              -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-calenWidget->frameGeometry().width(),
                          y()+frameGeometry().height()/2-size/5
                              -calenWidget->frameGeometry().height()/2);
        saveData();
    }
}

void Haro::mousePressEvent(QMouseEvent *event)
{
    static int flag = 0;//触发特殊动作的计数变量
    if(event->button() == Qt::LeftButton){//鼠标左键事件
        moveLeftTop = event->pos();
        if(faceIndex < 0 && speMoveIndex < 0){//随机播放表情
            faceIndex = QRandomGenerator::global()->bounded(FACE_NUM);
            flag++;
            if(flag==10){//触发蓝屏
                flag = 0;
                speMoveIndex = 0;
                faceIndex = -1;
            }
        }
    }
    else if(event->button() == Qt::RightButton){//鼠标右键事件
        if(btnSwitch1){//隐藏按钮
            btnSwitch1=0;
            btnSwitch2=0;
        }
        else
            btnSwitch1=1;//显示按钮

        dressWindow->hide();
        btnSwitchRole();
    }
}

void Haro::eyesMovement()
{
    static int flag = 0, second1 = 0, second2 = 0, earSwitch = 1;
    int value  = QRandomGenerator::global()->bounded(200);
    if(faceIndex < 0 && speMoveIndex < 0)
    {
        second1++;
        if(second1 >= value && value > 100)
        {
            faceIndex = 0;
            second1 = 0;
        }
    }

    second2++;
    if(second2 > 40 && earSwitch)
    {
        earsImage->setPixmap(ears2Vec[earsIndex].scaled(size, size));
        earSwitch = 0;
        second2 = 0;
    }
    else if(second2 > 10 && !earSwitch)
    {
        earsImage->setPixmap(ears1Vec[earsIndex].scaled(size, size));
        earSwitch = 1;
        second1 = 0;
    }

    if(faceIndex != -1)
    {
        int start = faceFramesVec[faceIndex].first,  num = faceFramesVec[faceIndex].second;
        flag++;
        if(flag < num)
            eyesImage->setPixmap(eyeMovePicVec[start + flag].scaled(size, size));
        else
            eyesImage->setPixmap(eyeMovePicVec[start - flag + (num - 1) *2].scaled(size, size));

        if(flag >= (num - 1) *2)
        {
            flag = 0;
            faceIndex = -1;
            eyesImage->setPixmap(eyesPixmap.scaled(size, size));
        }
    }

    if(!dressWindow->isHidden())
    {
        if(bodyIndex != dressWindow->getBodyIndex())
        {
            bodyIndex = dressWindow->getBodyIndex();
            bodyImage->setPixmap(bodyVec[bodyIndex].scaled(size, size));
            saveData();
        }

        if(earsIndex != dressWindow->getEarsIndex())
        {
            earsIndex = dressWindow->getEarsIndex();
            earsImage->setPixmap(ears1Vec[earsIndex].scaled(size, size));
            saveData();
        }
    }

    if(!setWindow->isHidden())
    {
        if(size != setWindow->getSize())
        {
            size = setWindow->getSize();
            setImage(bodyImage, bodyVec[bodyIndex]);
            setImage(eyesImage, eyesPixmap);
            if(size > 140)
            {
                setImage(stripeImage, stripePixmap);
                stripeImage->show();
            }
            else
                stripeImage->hide();
            setImage(earsImage, ears1Vec[earsIndex]);

            saveData();
            reInitBtn();
        }
    }

    if(speMoveIndex > -1)
        specialMovement();
}

void Haro::systemTrayActivated()
{
    if(this->isHidden())
        this->show();
}

void Haro::closeBtnClicked()
{
    dressWindow->close();
    setWindow->close();
    musicWindow->close();
    calenWidget->close();
    this->close();
}

void Haro::dressBtnClicked()
{
    if(dressWindow->isHidden())
    {
        dressWindow->move(x()+frameGeometry().width()/2-10
                              -btnSize*0.6-dressWindow->frameGeometry().width(),
                          y()+frameGeometry().height()/2-150
                              -dressWindow->frameGeometry().height()/2);
        dressWindow->show();
        calenWidget->hide();
        setWindow->hide();
        musicWindow->hide();
        btnSwitch2 = 0;
        btnSwitchRole();
    }
    else
        dressWindow->hide();
}

void Haro::moreBtnClicked()
{
    if(btnSwitch2 == 0)
        btnSwitch2 = 1;
    else
        btnSwitch2 = 0;

    btnSwitchRole();
    dressWindow->hide();
}

void Haro::minBtnClicked()
{
    this->hide();
    dressWindow->hide();
    calenWidget->hide();
    setWindow->hide();
    musicWindow->hide();

    btnSwitch1 = 0;
    btnSwitch2 = 0;
    btnSwitchRole();

}

void Haro::setBtnClicked()
{
    if(setWindow->isHidden())
    {
        setWindow->move(x()+frameGeometry().width()/2
                            -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-setWindow->frameGeometry().width(),
                        y()+frameGeometry().height()/2-size/5
                            -setWindow->frameGeometry().height()/2);
        setWindow->show();
        calenWidget->hide();
        musicWindow->hide();
    }
    else
        setWindow->hide();
}

void Haro::musicBtnClicked()
{
    if(musicWindow->isHidden()){
        //移动窗口坐标↓
        musicWindow->move(x()+frameGeometry().width()/2
                              -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-musicWindow->frameGeometry().width(),
                          y()+frameGeometry().height()/2-size/5
                              -musicWindow->frameGeometry().height()/2);

        musicWindow->show();
        calenWidget->hide();
        setWindow->hide();
    }
    else
        musicWindow->hide();
}

void Haro::gameBtnClicked()
{
    this->hide();
    setWindow->hide();
    calenWidget->hide();
    musicWindow->hide();
    QDir dir( "./game/Sky_island/Release/Sky_island.exe.lnk");//获取相对路径
    QString temDir = dir.absolutePath();//通过相对路径获取绝对路径
    system(temDir.toLatin1());

    //↑通过cmd启动游戏，toLatin1()将QString类型转为char*类型
    //阻塞式启动，关闭后游戏窗口后才运行下面语段↓
    //隐藏按钮↓
    btnSwitch1=0;
    btnSwitch2=0;
    btnSwitchRole();

    this->show();
}

void Haro::calenBtnClicked()
{
    if(calenWidget->isHidden())
    {
        calenWidget->move(x()+frameGeometry().width()/2
                              -btnSize*(btnSwitch1+btnSwitch2+1.5)/4-calenWidget->frameGeometry().width(),
                          y()+frameGeometry().height()/2-size/5
                              -calenWidget->frameGeometry().height()/2);
        calenWidget->show();
        musicWindow->hide();
        setWindow->hide();
    }
    else
        calenWidget->hide();
}


