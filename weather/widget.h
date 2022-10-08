#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QLabel;
class QPushButton;
class QNetworkReply;
class QNetworkAccessManager;

#include <QNetworkRequest>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void mSleep(int msec);
    void setWeatherPic(QString weather);
    void up();
    void down();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void doProcessFinished(QNetworkReply *reply);
    void slotUp();

private:
    Ui::Widget *ui;

    QLabel *tempLabel;
    QLabel *currentWeatherLabel;
    QPushButton *foldBtn;
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    QPoint mousePoint;
    QPoint movePoint;
    bool mousePress;

};
#endif // WIDGET_H
