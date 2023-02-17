#include "picshow.h"
#include "ui_picshow.h"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

PicShow::PicShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShow), _selectedPath(""), _btnVisible(false)
{
    ui->setupUi(this);
    ui->preBtn->setIcons(":/icon/previous.png",
                         ":/icon/previous_hover.png",
                         ":/icon/previous_press.png");

    ui->nextBtn->setIcons(":/icon/next.png",
                          ":/icon/next_hover.png",
                          ":/icon/next_press.png");

    connect(ui->preBtn, &PicButton::clicked, this, &PicShow::sigPreBtnClicked);
    connect(ui->nextBtn, &PicButton::clicked, this, &PicShow::sigNextBtnClicked);

    QGraphicsOpacityEffect *preOpacity = new QGraphicsOpacityEffect(this);
    preOpacity->setOpacity(0);
    ui->preBtn->setGraphicsEffect(preOpacity);

    QGraphicsOpacityEffect *nextOpacity = new QGraphicsOpacityEffect(this);
    nextOpacity->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(nextOpacity);

    _preShowAnimation = new QPropertyAnimation(preOpacity, "opacity", this);
    _preShowAnimation->setEasingCurve(QEasingCurve::Linear);
    _preShowAnimation->setDuration(500);


    _nextShowAnimation = new QPropertyAnimation(nextOpacity, "opacity", this);
    _nextShowAnimation->setEasingCurve(QEasingCurve::Linear);
    _nextShowAnimation->setDuration(500);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::reloadPic()
{
    if(_selectedPath != "")
    {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();

        _pixmap.load(_selectedPath);
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pixmap);
    }
}

bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        showPreNextBtns(true);
        break;
    case QEvent::Leave:
        showPreNextBtns(false);
        break;
    default:
        break;
    }
    return QDialog::event(event);
}

void PicShow::showPreNextBtns(bool bShow)
{
    if(!bShow && _btnVisible)
    {
        _preShowAnimation->stop();
        _preShowAnimation->setStartValue(1);
        _preShowAnimation->setEndValue(0);
        _preShowAnimation->start();

        _nextShowAnimation->stop();
        _nextShowAnimation->setStartValue(1);
        _nextShowAnimation->setEndValue(0);
        _nextShowAnimation->start();
        _btnVisible = false;
        return;
    }

    if(_selectedPath == "")
    {
        return;
    }

    if(bShow && !_btnVisible)
    {
        _preShowAnimation->stop();
        _preShowAnimation->setStartValue(1);
        _preShowAnimation->setEndValue(0);
        _preShowAnimation->start();

        _nextShowAnimation->stop();
        _nextShowAnimation->setStartValue(1);
        _nextShowAnimation->setEndValue(0);
        _nextShowAnimation->start();
        _btnVisible = true;
        return;
    }
}

void PicShow::slotSelectItem(const QString &path)
{
    _selectedPath = path;
    _pixmap.load(path);
    auto width = this->width() - 20;
    auto height =this->height() - 20;
    _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(_pixmap);
}

void PicShow::slotUpdatePic(const QString &path)
{
    _selectedPath = path;
    if(_selectedPath != "")
    {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        _pixmap.load(_selectedPath);
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pixmap);
    }
}

void PicShow::slotDeleteItem()
{
    _selectedPath = "";
}
