#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include <QThread>
#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;

class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThread(const QString &srcPath, int &fileCount, QTreeWidget *self, QObject *parent = nullptr);

    void openProTree(const QString &srcPath, int &fileCount, QTreeWidget *self);

protected:
    void run();

private:
    void recursiveProTree(const QString &srcPath,
                          int &fileCount, QTreeWidget *self,
                          QTreeWidgetItem* root, QTreeWidgetItem* parent, QTreeWidgetItem* preItem);

private:
    QString _srcPath;
    int _fileCount;
    QTreeWidget *_self;
    bool _bstop;
    QTreeWidgetItem *_root;

signals:
    void sigFinishProgress(int);
    void sigUpdateProgress(int);
};

#endif // OPENTREETHREAD_H
