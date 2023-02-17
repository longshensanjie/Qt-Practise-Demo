#include "picanimationwid.h"
#include "protreeitem.h"

#include <QTimer>
#include <QPainter>

PicAnimationWid::PicAnimationWid(QWidget *parent)
    : QWidget{parent}, _factor(0.0), _curItem(nullptr), _bstart(false)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &PicAnimationWid::slotTimeOut);
}

PicAnimationWid::~PicAnimationWid()
{
    _timer->stop();
}

void PicAnimationWid::start()
{
    emit sigStart();
    emit sigStartMusic();
    _factor = 0;
    _timer->start(25);
    _bstart  = true;
}

void PicAnimationWid::stop()
{
    emit sigStop();
    emit sigStopMusic();
    _timer->stop();
    _factor = 0;
    _bstart  = false;
}

void PicAnimationWid::slidePre()
{
    stop();
    if(!_curItem)
        return;
    ProTreeItem *curProItem = dynamic_cast<ProTreeItem*>(_curItem);
    ProTreeItem *preItem = curProItem->getPreItem();
    if(!preItem){
        return;
    }

    setPixmap(preItem);
    update();
}

void PicAnimationWid::slideNext()
{
    stop();
    if(!_curItem)
        return;
    ProTreeItem *curProItem = dynamic_cast<ProTreeItem*>(_curItem);
    ProTreeItem *nextItem = curProItem->getNextItem();
    if(!nextItem){
        return;
    }

    setPixmap(nextItem);
    update();
}

void PicAnimationWid::setPixmap(QTreeWidgetItem *item)
{
    if(!item)
        return;

    auto *treeItem = dynamic_cast<ProTreeItem *>(item);
    auto path = treeItem->getPath();
    _pixmap1.load(path);
    _curItem = treeItem;
    if(_itemsMap.find(path) == _itemsMap.end())
    {
        _itemsMap[path] = treeItem;
        emit sigUpPreList(item);
    }

    emit sigSelectItem(item);

    auto *nextItem = treeItem->getNextItem();
    if(!nextItem)
        return;

    auto nextPath = nextItem->getPath();
    _pixmap2.load(nextPath);
    if(_itemsMap.find(nextPath) == _itemsMap.end())
    {
        _itemsMap[nextPath] = nextItem;
        emit sigUpPreList(nextItem);
    }
}

void PicAnimationWid::upSelectPixmap(QTreeWidgetItem *item)
{
    if(!item)
        return;

    ProTreeItem * treeItem = dynamic_cast<ProTreeItem*>(item);
    QString path = treeItem->getPath();
    _pixmap1.load(path);

    _curItem = treeItem;
    if(_itemsMap.find(path) == _itemsMap.end()){
        _itemsMap[path] = treeItem;
    }

    ProTreeItem * nextItem = treeItem->getNextItem();
    if(!nextItem){
        return;
    }

    auto nextPath = nextItem->getPath();
    _pixmap2.load(nextPath);
    if(_itemsMap.find(nextPath) == _itemsMap.end()){
        _itemsMap[nextPath] = nextItem;
    }
}

void PicAnimationWid::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(_pixmap1.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = geometry();

    int width = rect.width();
    int height = rect.height();
    _pixmap1 = _pixmap1.scaled(width, height, Qt::KeepAspectRatio);
    int alpha = 255 * (1.0f - _factor);
    QPixmap alphaPixmap(_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);

    QPainter p1(&alphaPixmap);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, _pixmap1);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
    p1.end();

    int x = (width - _pixmap1.width()) / 2;
    int y = (height - _pixmap1.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap);

    if(_pixmap2.isNull()){
         return;
    }

    _pixmap2=_pixmap2.scaled(width,height,Qt::KeepAspectRatio);
    alpha = 255 * (_factor);
    QPixmap alphaPixmap2(_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, _pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    p2.end();
    x = (width - _pixmap2.width()) / 2;
    y = (height - _pixmap2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap2);
}

void PicAnimationWid::slotStartOrStop()
{
    if(!_bstart ){
        _factor = 0;
        _timer->start(25);
        _bstart = true;
        emit sigStartMusic();
    }else{
        _timer->stop();
        _factor = 0;
        update();
        _bstart = false;
        emit sigStopMusic();
    }
}

void PicAnimationWid::slotUpSelectShow(const QString &path)
{
    auto iter = _itemsMap.find(path);
    if(iter == _itemsMap.end()){
        return;
    }

    upSelectPixmap(iter.value());
    update();
}

void PicAnimationWid::slotTimeOut()
{
    if(!_curItem)
    {
        stop();
        update();
        return;
    }

    _factor = _factor + 0.01;

    if(_factor >= 1)
    {
        _factor = 0;
        auto *curProItem = dynamic_cast<ProTreeItem *>(_curItem);
        auto *nextProItem = curProItem->getNextItem();
        if(!nextProItem)
        {
            stop();
            update();
            return;
        }
        setPixmap(nextProItem);
        update();
        return;
    }
    update();
}
