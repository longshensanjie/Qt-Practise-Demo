#include "opentreethread.h"
#include "protreeitem.h"
#include "const.h"

#include <QDir>

OpenTreeThread::OpenTreeThread(const QString &srcPath, int &fileCount, QTreeWidget *self, QObject *parent)
    : QThread{parent}, _srcPath(srcPath), _fileCount(fileCount), _self(self), _bstop(false), _root(nullptr)
{

}

void OpenTreeThread::run()
{
    openProTree(_srcPath, _fileCount, _self);
    if(_bstop && _root)
    {
        auto path = dynamic_cast<ProTreeItem *>(_root)->getPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit sigFinishProgress(_fileCount);
}


void OpenTreeThread::openProTree(const QString &srcPath, int &fileCount, QTreeWidget *self)
{
    QDir srcDir(srcPath);
    QString name = srcDir.dirName();
    ProTreeItem *item = new ProTreeItem(self, name, srcPath, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, srcPath);

    _root = item;
    recursiveProTree(srcPath, fileCount, self, _root, item, nullptr);
}

void OpenTreeThread::recursiveProTree(const QString &srcPath, int &fileCount, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    QDir srcDir(srcPath);
    //设置文件过滤器
    QStringList nameFilters;
    srcDir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);//除了目录或文件，其他的过滤掉
    srcDir.setSorting(QDir::Name);//优先显示名字
    QFileInfoList list = srcDir.entryInfoList();
    for(int i = 0; i < list.size(); i++){
        if(_bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if (bIsDir)
        {
            if(_bstop){
                return;
            }
            fileCount++;
            emit sigUpdateProgress(fileCount);
            ProTreeItem * item = new ProTreeItem(_root, fileInfo.fileName(),
                                          fileInfo.absoluteFilePath(), _root,TreeItemDir);
            item->setData(0,Qt::DisplayRole, fileInfo.fileName());
            item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole, fileInfo.absoluteFilePath());

            recursiveProTree(fileInfo.absoluteFilePath(), fileCount,
                             self,root,item, preItem);

         }else{
            if(_bstop){
                return;
            }

            const QString &suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg"){
                continue;
            }

            fileCount++;
             emit sigUpdateProgress(fileCount);

            ProTreeItem *item = new ProTreeItem(parent, fileInfo.fileName(),
                                          fileInfo.absoluteFilePath(), root,TreeItemPic);
            item->setData(0,Qt::DisplayRole, fileInfo.fileName());
            item->setData(0,Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole, fileInfo.absoluteFilePath());

            if(preItem){
                ProTreeItem* preProItem = dynamic_cast<ProTreeItem*>(preItem);
                preProItem->setNextItem(item);
            }

            item->setPreItem(preItem);
            preItem = item;
        }
    }

    emit sigFinishProgress(fileCount);
}

