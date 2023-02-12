#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);


private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonOpenDir_clicked();

    void on_pushButtonClear_clicked();


private:
    void initForm();

    bool checkFile(const QString& fileName);
    void countPathFileCount(const QString& path);
    void countFileRows(const QStringList& files);
    void countCodeRows(const QString& fileName, int& lineCode, int& lineNote, int& lineBlank);

private:
    Ui::Widget *ui;
    QStringList listFiles;

    // QWidget interface

};
#endif // WIDGET_H
