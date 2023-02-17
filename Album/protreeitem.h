#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget *view, const QString &name, const QString &path , int type = Type);
    ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path ,
                QTreeWidgetItem *root, int type = Type);

    const QString getPath();
    QTreeWidgetItem* getRoot();

    void setNextItem(QTreeWidgetItem *item);
    void setPreItem(QTreeWidgetItem *item);
    ProTreeItem* getPreItem();
    ProTreeItem* getNextItem();
    ProTreeItem* getFirstPicChild();
    ProTreeItem* getLastPicChild();

private:
    QString _name;
    QString _path;
    QTreeWidgetItem *_root;
    QTreeWidgetItem *_preItem;
    QTreeWidgetItem *_nextItem;

};

#endif // PROTREEITEM_H
