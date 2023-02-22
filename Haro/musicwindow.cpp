#include "musicwindow.h"
#include "ui_musicwindow.h"

#include <QSlider>
#include <QPainter>
#include <QFileDialog>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QRegularExpression>

QRegularExpression MusicWindow::re;

MusicWindow::MusicWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicWindow)
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

    re.setPattern("[/.]");

    this->setWindowIcon(QIcon(":/images/icon/music.png")); //设置窗口图标
    initBtn();
    initMedia();
}

MusicWindow::~MusicWindow()
{
    delete ui;
}

void MusicWindow::initBtn()
{
    setStyleSheet("QPushButton{border: none;"
                  "background-color:rgb(255,255,255);border-radius: 35px;}"
                  "QPushButton::hover{background-color:rgb(200,200,200);}"
                  "QPushButton:pressed{background-color:rgb(160,160,160);}");

    nextBtn = new QPushButton(this);
    lastBtn = new QPushButton(this);
    pauseBtn = new QPushButton(this);
    playBtn = new QPushButton(this);
    addBtn = new QPushButton(this);
    //图标载入
    nextBtn->setIcon(QIcon(":/images/icon/next.png"));
    lastBtn->setIcon(QIcon(":/images/icon/last.png"));
    pauseBtn->setIcon(QIcon(":/images/icon/pause.png"));
    playBtn->setIcon(QIcon(":/images/icon/play.png"));
    addBtn->setIcon(QIcon(":/images/icon/add.png"));

    //图标大小
    int btnSize = 80;
    QSize tempSize(btnSize,btnSize);
    nextBtn->setIconSize(tempSize);
    lastBtn->setIconSize(tempSize);
    pauseBtn->setIconSize(tempSize);
    playBtn->setIconSize(tempSize);
    addBtn->setIconSize(tempSize);

    //按钮位置
    int x=60;
    int y=300;
    nextBtn->setGeometry(x+180,y,btnSize,btnSize);
    lastBtn->setGeometry(x,y,btnSize,btnSize);
    pauseBtn->setGeometry(x+85,y-5,btnSize+10,btnSize+10);
    playBtn->setGeometry(x+85,y-5,btnSize+10,btnSize+10);
    addBtn->setGeometry(x+265,y+5,btnSize-10,btnSize-10);
    playBtn->hide();

    //连接按钮信号与对应槽函数
    connect(playBtn,&QPushButton::clicked,this,&MusicWindow::playBtnClicked);
    connect(pauseBtn,&QPushButton::clicked,this,&MusicWindow::pauseBtnClicked);
    connect(nextBtn,&QPushButton::clicked,this,&MusicWindow::nextBtnClicked);
    connect(lastBtn,&QPushButton::clicked,this,&MusicWindow::lastBtnClicked);
    connect(addBtn,&QPushButton::clicked,this,&MusicWindow::addBtnClicked);
}

void MusicWindow::initMedia()
{
    musicIndex = -1;
    player = new QMediaPlayer;
    audioOutPut = new QAudioOutput;

    musicName = new QPlainTextEdit(this);
    musicList = new QPlainTextEdit(this);

//    QPalette palette  = musicName->palette();
//    palette.setBrush(QPalette::Base, QBrush(QColor(0, 0, 0)));
//    musicName->setPalette(palette);
//    musicList->setPalette(palette);

    musicList->setGeometry(50,50,350,120);
    musicList->setReadOnly(true);
    musicName->setGeometry(50,180,350,100);
    musicName->setReadOnly(true);

    musicList->setPlainText("播放队列:");
    musicName->setPlainText("当前播放");

    volume = new QSlider(this);
    volume->setMaximum(100);//设置最大值
    volume->setMinimum(0);//设置最小值
    volume->setValue(60);//设置初始值
    volume->setSingleStep(5);//设置步长
    volume->setOrientation(Qt::Horizontal);//水平样式
    volume->setTickPosition(QSlider::TicksAbove);//刻度线
    volume->setGeometry(60,400,330,30);//坐标和尺寸

    //当滑块值改变时，同步改变音量↓
    connect(volume, &QSlider::valueChanged,this, &MusicWindow::volumeChanged);
}

void MusicWindow::musicPlayFromIndex()
{
    player->setSource(fileList.at(musicIndex));
    player->play();
    playBtn->show();
    pauseBtn->hide();
    player->play();
    musicName->setPlainText("当前播放：");
    musicName->appendPlainText(fileList.at(musicIndex).section(re,-2,-2));
}

void MusicWindow::playBtnClicked()
{
    playBtn->hide();
    pauseBtn->show();
    player->pause();
}

void MusicWindow::pauseBtnClicked()
{
    if(player->playbackState() == QMediaPlayer::PausedState)
    {
        pauseBtn->hide();
        playBtn->show();
        player->play();
    }
}

void MusicWindow::nextBtnClicked()
{
    if(player->playbackRate() == QMediaPlayer::StoppedState || QMediaPlayer::PausedState)
    {
        ++musicIndex;
        if(musicIndex < fileList.size())
        {
            musicPlayFromIndex();
        }
        else
        {
            musicIndex = 0;
            musicPlayFromIndex();
        }
    }
}

void MusicWindow::lastBtnClicked()
{
    if(player->playbackRate() == QMediaPlayer::StoppedState || QMediaPlayer::PausedState)
    {
        --musicIndex;
        if(musicIndex > 0)
        {
            musicPlayFromIndex();
        }
        else
        {
            musicIndex = fileList.size() - 1;
            musicPlayFromIndex();
        }
    }
}

void MusicWindow::addBtnClicked()
{
    //打开文件夹选择音频文件
    QString curPash = "./music";
    QString dlgTitle="选择音乐";
    QString filter="音频文件(*.mp3 *.wav *.wma)mp3文件(*.mp3);;wav文件(*.wav);;wma文件(*.wma);;所有文件(*.*)";
    QStringList fileListTemp = QFileDialog::getOpenFileNames(this,dlgTitle,curPash,filter);

    //    QDir dir( "./music");
    //    QStringList nameFilters;
    //    nameFilters << "*.mp3";
    //    QStringList fileListTemp = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    if(fileListTemp.count()<1)
        return;

    for(int i = 0;i<fileListTemp.count();i++){
        QString file = fileListTemp.at(i);
        fileList<<file;//将文件路径加入文件名列表
        musicList->appendPlainText(QString::number(fileList.count())+"."+(fileListTemp.at(i).section(re,-2,-2)));
        //fileListTemp.at(i).section(QRegExp("[/.]"),-2,-2))能对文件路径进行分割，将音乐名分割出来
    }

    if(player->playbackState()==QMediaPlayer::StoppedState){
        musicIndex = 0;
        player->setSource(fileList.at(musicIndex));
        audioOutPut->setVolume(60);
        player->setAudioOutput(audioOutPut);
        playBtn->show();
        pauseBtn->hide();
        player->play();
        musicName->setPlainText("当前播放：");
        musicName->appendPlainText(fileList.at(musicIndex).section(re,-2,-2));
    }
}


void MusicWindow::volumeChanged(int value)
{
    audioOutPut->setVolume(value);
}


