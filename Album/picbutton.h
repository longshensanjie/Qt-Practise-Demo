#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = nullptr);
    void setIcons(const QString &normal, const QString &hover, const QString &pressed);

    // QObject interface
protected:
    bool event(QEvent *event);

private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();

private:
    QString _normal;
    QString _hover;
    QString _pressed;
};

#endif // PICBUTTON_H
