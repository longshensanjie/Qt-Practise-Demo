
#ifndef HARO_H
#define HARO_H

#include <QMainWindow>


class DressWindow;
class SetWindow;
class MusicWindow;

class QLabel;
class QPushButton;
class QSystemTrayIcon;
class QCalendarWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class Haro; }
QT_END_NAMESPACE

class Haro : public QMainWindow

{
    Q_OBJECT

public:
    Haro(QWidget *parent = nullptr);
    ~Haro();

    void loadBodyPic();
    void loadEyesMovePic();
    void loadSpeMovePic();
    void setImage(QLabel *, QPixmap);
    void initBtn();
    void reInitBtn();
    void initSystemTray();
    void btnSwitchRole();
    void specialMovement();
    void saveData();


    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);


private:
    Ui::Haro *ui;

    DressWindow *dressWindow;
    SetWindow *setWindow;
    MusicWindow *musicWindow;
    QCalendarWidget* calenWidget;

    int size;
    int bodyIndex;
    int earsIndex;
    int btnSize;
    int btnSwitch1, btnSwitch2;
    int faceIndex = 0;
    int speMoveIndex = 0;

    QPoint moveLeftTop;

    QTimer *timer;
    QVector<QPixmap> bodyVec, ears1Vec, ears2Vec;
    QPixmap eyesPixmap, stripePixmap;
    QVector<QPair<int, int>> faceFramesVec;
    QVector<QPixmap> eyeMovePicVec;
    QVector<QPixmap> speMovePicVec;

    QLabel *bodyImage, *eyesImage, *stripeImage, *earsImage;
    QPushButton *closeBtn, *dressBtn, *moreBtn, *minBtn, *setBtn, *musicBtn, *gameBtn, *calenBtn;
    QSystemTrayIcon *systemTray;

private slots:
    void eyesMovement();
    void systemTrayActivated();
    void closeBtnClicked();
    void dressBtnClicked();
    void moreBtnClicked();
    void minBtnClicked();
    void setBtnClicked();
    void musicBtnClicked();
    void gameBtnClicked();
    void calenBtnClicked();

};

#endif // HARO_H
