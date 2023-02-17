#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>

namespace Ui {
class PicShow;
}

class QPropertyAnimation;
class QPixmap;

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();

    void reloadPic();

    // QObject interface
protected:
    bool event(QEvent *event);

private:
    void showPreNextBtns(bool);

private:
    Ui::PicShow *ui;
    QPropertyAnimation *_preShowAnimation;
    QPropertyAnimation *_nextShowAnimation;
    bool _btnVisible;
    QString _selectedPath;
    QPixmap _pixmap;

public slots:
    void slotSelectItem(const QString &);
    void slotUpdatePic(const QString &);
    void slotDeleteItem();

signals:
    void sigPreBtnClicked();
    void sigNextBtnClicked();


};

#endif // PICSHOW_H
