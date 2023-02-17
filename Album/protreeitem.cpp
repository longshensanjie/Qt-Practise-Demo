#include "protreeitem.h"
#include "const.h"

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view, type),_name(name), _path(path), _root(nullptr),
      _preItem(nullptr), _nextItem(nullptr)
{

}
ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent, type), _name(name), _path(path), _root(root),
      _preItem(nullptr), _nextItem(nullptr)
{

}

const QString ProTreeItem::getPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::getRoot()
{
    return _root;
}

void ProTreeItem::setNextItem(QTreeWidgetItem *item)
{
    _nextItem = item;
}

void ProTreeItem::setPreItem(QTreeWidgetItem *item)
{
    _preItem = item;
}

ProTreeItem *ProTreeItem::getPreItem()
{
    return dynamic_cast<ProTreeItem *>(_preItem) ;
}

ProTreeItem *ProTreeItem::getNextItem()
{
    return dynamic_cast<ProTreeItem *>(_nextItem);
}

ProTreeItem *ProTreeItem::getFirstPicChild()
{
    if(this->type() == TreeItemPic)
    {
        return nullptr;
    }

    auto childCount = this->childCount();
    if(childCount == 0)
        return nullptr;

    for(auto i = 0; i != childCount - 1; ++i)
    {
        auto *firstChild = this->child(i);
        auto *firstTreeChild = dynamic_cast<ProTreeItem *>(firstChild);
        if(firstTreeChild->type() == TreeItemPic)
            return firstTreeChild;

        firstChild = firstTreeChild->getFirstPicChild();
        if(!firstChild)
            continue;

        firstTreeChild = dynamic_cast<ProTreeItem *>(firstChild);
        return firstTreeChild;
    }
    return nullptr;
}

ProTreeItem *ProTreeItem::getLastPicChild()
{
    if(this->type() == TreeItemPic)
    {
        return nullptr;
    }

    auto childCount = this->childCount();
    if(childCount == 0)
        return nullptr;

    for(auto i = childCount - 1; i != 0; --i)
    {
        auto *lastChild = this->child(i);
        auto *lastTreeChild = dynamic_cast<ProTreeItem *>(lastChild);
        if(lastTreeChild->type() == TreeItemPic)
            return lastTreeChild;

        lastChild = lastTreeChild->getLastPicChild();
        if(!lastChild)
            continue;

        lastTreeChild = dynamic_cast<ProTreeItem *>(lastChild);
        return lastTreeChild;
    }
    return nullptr;
}

