#ifndef SETWINDOW_H
#define SETWINDOW_H

#include <QWidget>

class QLabel;

namespace Ui {
class SetWindow;
}

class SetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SetWindow(QWidget *parent = nullptr);
    ~SetWindow();

    void setSize(int);
    int getSize();

private slots:
    void on_sizeSlider_valueChanged(int value);

private:
    Ui::SetWindow *ui;

    int haroSize;
    QLabel *sizeLabel;
};

#endif // SETWINDOW_H
