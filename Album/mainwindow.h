#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_proTree;
    QWidget *_picShow;

private slots:
    void slotCreatePro(bool);
    void slotOpenPro(bool);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void sigOpenPro(const QString &);
};
#endif // MAINWINDOW_H
