#include "protreethread.h"
#include "protreeitem.h"
#include "const.h"

#include <QDir>
#include <QTreeWidget>
#include <QTreeWidgetItem>

ProTreeThread::ProTreeThread(const QString &srcPath, const QString &distPath, QTreeWidgetItem *parentItem, int &fileCount, QTreeWidget *self, QTreeWidgetItem *root, QObject *parent)
    :QThread(parent), _srcPath(srcPath), _distPath(distPath), _parentItem(parentItem), _fileCount(fileCount), _self(self), _root(root), _bstop(false)
{

}

void ProTreeThread::run()
{
    createProTree(_srcPath, _distPath, _parentItem, _fileCount, _self, _root);
    if(_bstop)
    {
        QString path = dynamic_cast<ProTreeItem *>(_root)->getPath();
        int index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit sigFinishProgress(_fileCount);
}

void ProTreeThread::createProTree(const QString &srcPath, const QString &distPath, QTreeWidgetItem *parentItem, int &fileCount, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *preItem)
{
    if(_bstop)
        return;
    bool needCopy = true;

    if(srcPath == distPath)
        needCopy = false;

    QDir importDir(srcPath);
    importDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    importDir.setSorting(QDir::Name);

    QFileInfoList list = importDir.entryInfoList();

    for(auto i = 0; i != list.size(); ++i)
    {
        if(_bstop)
            return;

        QFileInfo info = list.at(i);
        bool isDir = info.isDir();
        if(isDir)
        {
            if(_bstop)
                return;

            fileCount++;
            emit sigUpdateProgress(fileCount);

            QDir distDir(distPath);
            QString subDistPath = distDir.absoluteFilePath(info.fileName());
            QDir subDistDir(subDistPath);
            if(!subDistDir.exists())
            {
                bool ok = subDistDir.mkpath(subDistPath);
                if(!ok)
                {
                    qDebug()<< "sub_dist_dir mkpath failed";
                    continue;
                }
            }

            ProTreeItem *item = new ProTreeItem(parentItem, info.fileName(), subDistPath, root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, subDistPath);

            createProTree(info.absoluteFilePath(), subDistPath, item, fileCount, self, root, preItem);
        }
        else
        {
            if(_bstop)
                return;

            const QString &suffix = info.completeSuffix();
            if(suffix != "png" && suffix != "jpg" && suffix != "jpeg")
            {
                continue;
            }
            fileCount++;
            emit sigUpdateProgress(fileCount);

            if(!needCopy)
                continue;

            QDir distDir(distPath);
            QString distFilePath = distDir.absoluteFilePath(info.fileName());
            if(!QFile::copy(info.absoluteFilePath(), distFilePath))
            {
                continue;
            }

            auto * item = new ProTreeItem(parentItem, info.fileName(),
                                                    distFilePath, root,TreeItemPic);
                      item->setData(0,Qt::DisplayRole, info.fileName());
                      item->setData(0,Qt::DecorationRole, QIcon(":/icon/pic.png"));
                      item->setData(0,Qt::ToolTipRole, distFilePath);

            if(preItem)
            {
                ProTreeItem *preProItem = dynamic_cast<ProTreeItem *>(preItem);
                preProItem->setNextItem(item);

            }

            item->setPreItem(preItem);
            preItem = item;
        }

    }
    parentItem->setExpanded(true);
}

void ProTreeThread::slotCancelProgress()
{
    _bstop = true;
}
