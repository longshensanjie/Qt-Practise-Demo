#ifndef DRESSWINDOW_H
#define DRESSWINDOW_H

#include <QWidget>
#include "const.h"

class QPushButton;
class QButtonGroup;
class QAbstractButton;

namespace Ui {
class DressWindow;
}

class DressWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DressWindow(QWidget *parent = nullptr);
    ~DressWindow();
    void accept(QVector<QPixmap> &bodyVec, QVector<QPixmap> &earsVec, int bodyIndex, int earsIndex);
    int getBodyIndex();
    int getEarsIndex();


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void initBtn();

private:
    Ui::DressWindow *ui;

    int posY;
    int bodyIndex, earsIndex;
    QButtonGroup *bodyBox, *earsBox;
    QPushButton *bodyBtnArr[DRESS_NUM], *earsBtnArr[DRESS_NUM];
    QVector<QPixmap> bodyVec, earsVec;

private slots:
    void bodyChange(QAbstractButton *);
    void earsChange(QAbstractButton *);

};

#endif // DRESSWINDOW_H
