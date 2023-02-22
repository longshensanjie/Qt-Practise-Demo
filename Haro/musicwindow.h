#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include <QWidget>

class QSlider;
class QPushButton;
class QPlainTextEdit;
class QMediaPlayer;
class QAudioOutput;
class QRegularExpression;

namespace Ui {
class MusicWindow;
}

class MusicWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MusicWindow(QWidget *parent = nullptr);
    ~MusicWindow();

private:
    void initBtn();
    void initMedia();
    void musicPlayFromIndex();

private:
    Ui::MusicWindow *ui;
    int musicIndex;

    QPushButton *nextBtn;
    QPushButton *lastBtn;
    QPushButton *pauseBtn;
    QPushButton *playBtn;
    QPushButton *addBtn;
    QPlainTextEdit *musicName;
    QPlainTextEdit *musicList;
    QMediaPlayer *player;
    QAudioOutput *audioOutPut;
    QSlider *volume;
    QStringList fileList;
    static QRegularExpression re;

private slots:
    void playBtnClicked();
    void pauseBtnClicked();
    void nextBtnClicked();
    void lastBtnClicked();
    void addBtnClicked();
    void volumeChanged(int);
};

#endif // MUSICWINDOW_H
