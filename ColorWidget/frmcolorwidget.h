#ifndef FRMCOLORWIDGET_H
#define FRMCOLORWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class frmColorWidget; }
QT_END_NAMESPACE

class frmColorWidget : public QWidget
{
    Q_OBJECT

public:
    frmColorWidget(QWidget *parent = nullptr);
    ~frmColorWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::frmColorWidget *ui;
};
#endif // FRMCOLORWIDGET_H
