#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include "protreewidget.h"


SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem *firstItem, QTreeWidgetItem *lastItem)
    :    QDialog(parent), ui(new Ui::SlideShowDlg), _firstItem(firstItem), _lastItem(lastItem)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->playBtn->setIcons(":/icon/play.png",":/icon/play_hover.png",":/icon/play_press.png",
                             ":/icon/pause.png",":/icon/pause_hover.png",":/icon/pause_press.png");
    ui->closeBtn->setIcons(":/icon/closeshow.png",":/icon/closeshow_hover.png",
                             ":/icon/closeshow_press.png");
    ui->slidpreBtn->setIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");
    ui->slidenextBtn->setIcons(":/icon/next.png",
                               ":/icon/next_hover.png",
                               ":/icon/next_press.png");


    connect(ui->playBtn, &PicStateBtn::clicked, ui->picAnimation, &PicAnimationWid::slotStartOrStop);
    connect(ui->closeBtn, &QPushButton::clicked, this, &SlideShowDlg::close);
    connect(ui->slidpreBtn, &PicButton::clicked, this, &SlideShowDlg::slotSlidePre);
    connect(ui->slidenextBtn, &PicButton::clicked, this, &SlideShowDlg::slotSlideNext);

    PreListWid *prelistWid = dynamic_cast<PreListWid *>(ui->preListWidget);

    connect(ui->picAnimation, &PicAnimationWid::sigUpPreList, prelistWid, &PreListWid::slotUpPreList);
    connect(ui->picAnimation, &PicAnimationWid::sigSelectItem, prelistWid, &PreListWid::slotUpSelect);
    connect(prelistWid, &PreListWid::sigUpSelectRow, ui->picAnimation, &PicAnimationWid::slotUpSelectShow);

    ProTreeWidget * proTreeWidget = dynamic_cast<ProTreeWidget*>(parent);

    connect(ui->picAnimation, &PicAnimationWid::sigStartMusic, proTreeWidget, &ProTreeWidget::slotStartMusic);
    connect(ui->picAnimation, &PicAnimationWid::sigStopMusic, proTreeWidget, &ProTreeWidget::slotStopMusic);

    ui->picAnimation->setPixmap(_firstItem);
    ui->picAnimation->start();

    connect(ui->picAnimation, &PicAnimationWid::sigStart, ui->playBtn, &PicStateBtn::slotStart);
    connect(ui->picAnimation, &PicAnimationWid::sigStop, ui->playBtn, &PicStateBtn::slotStop);

}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::closeEvent(QCloseEvent *event)
{
    ui->picAnimation->stop();
    QDialog::closeEvent(event);
}

void SlideShowDlg::slotSlidePre()
{
    ui->picAnimation->slidePre();
}

void SlideShowDlg::slotSlideNext()
{
    ui->picAnimation->slideNext();
}
