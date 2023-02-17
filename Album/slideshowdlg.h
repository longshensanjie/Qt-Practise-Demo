#ifndef SLIDESHOWDLG_H
#define SLIDESHOWDLG_H

#include <QDialog>

namespace Ui {
class SlideShowDlg;
}

class ProTreeWidget;
class QTreeWidgetItem;

class SlideShowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDlg(QWidget *parent, QTreeWidgetItem *firstItem, QTreeWidgetItem *lastItem);
    ~SlideShowDlg();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::SlideShowDlg *ui;
    QTreeWidgetItem *_firstItem;
    QTreeWidgetItem *_lastItem;

private slots:
    void slotSlidePre();
    void slotSlideNext();


};

#endif // SLIDESHOWDLG_H
