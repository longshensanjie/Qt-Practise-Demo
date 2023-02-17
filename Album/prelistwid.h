#ifndef PRELISTWID_H
#define PRELISTWID_H

#include <QListWidget>
class QTreeWidgetItem;
class QListWidgetItem;

class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    PreListWid(QWidget *parent = nullptr);

    void addListItem(const QString &);

private:
    QMap<QString, QListWidgetItem *>_itemsMap;
    int _global;
    int _lastIndex;
    QPoint _originPox;

public slots:
    void slotUpPreList(QTreeWidgetItem *);
    void slotUpSelect(QTreeWidgetItem *);

private slots:
    void slotItemPressed(QListWidgetItem *);

signals:
    void sigUpSelectRow(const QString &);

};

#endif // PRELISTWID_H
