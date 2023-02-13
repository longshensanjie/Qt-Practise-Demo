#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>

class QGridLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    static ColorWidget *Instance();
    explicit ColorWidget(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void showColorValue();

private:
    static ColorWidget *instance;
    QClipboard *cp;
    bool pressed;
    QTimer *timer;

    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labColor;
    QLabel *label;
    QLabel *labWeb;
    QLineEdit *txtWeb;
    QLabel *labRgb;
    QLineEdit *txtRgb;
    QLabel *labPoint;
    QLineEdit *txtPoint;
};

#endif // COLORWIDGET_H
