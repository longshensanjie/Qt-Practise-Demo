#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;

class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString &srcPath, const QString &distPath, QTreeWidgetItem *parentItem, int &fileCount,
                  QTreeWidget *self, QTreeWidgetItem *root, QObject *parent);

protected:
    virtual void run();

private:
    void createProTree(const QString &srcPath, const QString &distPath,
                  QTreeWidgetItem *parentItem, int &fileCount,
                  QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem* preItem = nullptr);

private:
    QString _srcPath;
    QString _distPath;
    QTreeWidgetItem *_parentItem;
    int _fileCount;
    QTreeWidget *_self;
    QTreeWidgetItem *_root;
    bool _bstop;

public slots:
    void slotCancelProgress();

signals:
    void sigUpdateProgress(int);
    void sigFinishProgress(int);
};

#endif // PROTREETHREAD_H
