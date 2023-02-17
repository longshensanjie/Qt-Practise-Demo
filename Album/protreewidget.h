#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <memory>
#include "protreethread.h"
#include "opentreethread.h"
#include "slideshowdlg.h"

class QTreeWidgetItem;
class QProgressDialog;
class QSoundEffect;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void addProToTree(const QString &name, const QString &path);

private:
    QSet<QString> _pathSet;
    QAction *_importAction;
    QAction *_setStartAction;
    QAction *_closeProAction;
    QAction *_slideShowAction;
    QTreeWidgetItem *_rightBtnItem;
    QTreeWidgetItem *_selectdItem;
    QTreeWidgetItem *_activeItem;
    QProgressDialog *_progressDialog;
    QProgressDialog *_openProProgressDlg;
    std::shared_ptr<ProTreeThread>  _createProThread;
    std::shared_ptr<OpenTreeThread> _openProThread;
    std::shared_ptr<SlideShowDlg> _slideShowDlg;
    QSoundEffect *_sound;

private slots:
    void slotItemPressed(QTreeWidgetItem *item, int column);
    void slotItemDoubleClicked(QTreeWidgetItem *item, int column);

    void slotImport();
    void slotSetActive();
    void slotClosePro();
    void slotSlideShow();
    void slotUpdateProgress(int);
    void slotFinishProgress();
    void slotCancelProgress();

    void slotUpOpenPgogress(int);
    void slotFinishOpenPgogress();

public slots:
    void slotPreShow();
    void slotNextShow();
    void slotOpenPro(const QString &);
    void slotSetMusic(bool);
    void slotStartMusic();
    void slotStopMusic();


signals:
    void sigCancelProgress();
    void sigUpdateSelected(const QString&);
    void sigUpdatePic(const QString&);
    void sigClearSelected();
};

#endif // PROTREEWIDGET_H
