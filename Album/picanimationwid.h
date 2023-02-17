#ifndef PICANIMATIONWID_H
#define PICANIMATIONWID_H

#include <QWidget>
#include <QPixmap>

class QTreeWidgetItem;


class PicAnimationWid : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWid(QWidget *parent = nullptr);
    ~PicAnimationWid();
    void start();
    void stop();
    void slidePre();
    void slideNext();
    void setPixmap(QTreeWidgetItem *);
    void upSelectPixmap(QTreeWidgetItem *);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    float _factor;
    QTimer *_timer;
    QTreeWidgetItem *_curItem;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    bool _bstart;
    QMap<QString, QTreeWidgetItem*> _itemsMap;

signals:
    void sigStart();
    void sigStop();
    void sigStartMusic();
    void sigStopMusic();
    void sigUpPreList(QTreeWidgetItem *);
    void sigSelectItem(QTreeWidgetItem *);

public slots:
    void slotStartOrStop();
    void slotUpSelectShow(const QString &);

private slots:
    void slotTimeOut();


};

#endif // PICANIMATIONWID_H
