#include "prelistwid.h"
#include "protreeitem.h"
#include "prelistitem.h"
#include "const.h"

#include <QGuiApplication>
#include <QPainter>

PreListWid::PreListWid(QWidget *parent)
    :QListWidget(parent), _global(0), _lastIndex(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);
    connect(this, &PreListWid::itemPressed, this, &PreListWid::slotItemPressed);
}

void PreListWid::slotUpPreList(QTreeWidgetItem *item)
{
    if(!item)
        return;

    auto *proItem = dynamic_cast<ProTreeItem *>(item);
    auto path = proItem->getPath();
    auto iter = _itemsMap.find(path);
    if(iter != _itemsMap.end())
    {
        return;
    }

    addListItem(path);
}

void PreListWid::slotUpSelect(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }

    auto * proItem = dynamic_cast<ProTreeItem*>(item);
    auto path = proItem->getPath();
    auto iter = _itemsMap.find(path);
    if(iter == _itemsMap.end()){
        return;
    }

    PreListItem * listItem = dynamic_cast<PreListItem*>(iter.value());
    int index = listItem->getIndex();

    if(index < 17)
    {
        auto pos_cur = this->pos();
        this->move(pos_cur.x() - (index - _lastIndex) *100, pos_cur.y());
        _lastIndex = index;
    }
    else
    {
        this->move(_originPox);
        _lastIndex = 17;
    }

    this->setCurrentItem(iter.value());
}

void PreListWid::slotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto * listItem = dynamic_cast<PreListItem*>(item);
    auto path = listItem->getPath();

    this->setCurrentItem(item);
    emit sigUpSelectRow(path);
}

void PreListWid::addListItem(const QString &path)
{
    QPixmap srcPixmap(path);
    srcPixmap = srcPixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dstPixmap(QSize(PREICON_SIZE, PREICON_SIZE));

    auto src_width = srcPixmap.width();
    auto src_height = srcPixmap.height();

    auto dst_width = dstPixmap.width();
    auto dst_height = dstPixmap.height();

    dstPixmap.fill(QColor(220, 220, 220, 50));
    QPainter painter(&dstPixmap);

    auto x = (dst_width-src_width)/2;
    auto y = (dst_height-src_height)/2;
    painter.drawPixmap(x,y,srcPixmap);

    _global++;
    PreListItem *preListItem = new PreListItem(QIcon(dstPixmap), path, _global, this);
    preListItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(preListItem);
    _itemsMap[path] = preListItem;

    if(_global == 1)
        _originPox = this->pos();
}
