#include <QtWidgets>
#include <QtNetwork>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setStyleSheet("QLabel{color：#292929}");

    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);

    tempLabel = new QLabel(this);
    QFont tempFont("Arial", 50, 75);
    tempLabel->setFont(tempFont);

    currentWeatherLabel = new QLabel(this);
    QFont weatherFont("Arial", 20, 75);
    currentWeatherLabel->setFont(weatherFont);
    currentWeatherLabel->setStyleSheet("color:#ffffff");

    /*当前城市和日期文字的字体颜色*/
    ui->label_curr_city->setStyleSheet("color:#ffffff");
    ui->label_curr_date->setStyleSheet("color:#ffffff");

    tempLabel->move(180, 80);
    currentWeatherLabel->move(220, 200);

    foldBtn = new QPushButton(this);
    foldBtn->setFixedSize(50, 50);
    foldBtn->setText(tr("折叠"));
    foldBtn->setStyleSheet("QPushButton{font:Arial;color:#ffffff;font-size:20px;background-color:rgba(0,0,0,0);border:none}"
                            "QPushButton:hover{font-size:22px;}");
    connect(foldBtn, &QPushButton::clicked, this, &Widget::slotUp);
    foldBtn->move(380,0);

    QPushButton *closeBtn = new QPushButton(this);
    closeBtn->setFixedSize(50, 50);
    closeBtn->setText(tr("关闭"));
    closeBtn->setStyleSheet("QPushButton{font:Arial;color:#ffffff;font-size:20px;background-color:rgba(0,0,0,0);border:none}"
                            "QPushButton:hover{font-size:22px;}");
    connect(closeBtn, &QPushButton::clicked, this, &Widget::close);
    closeBtn->move(440, 0);

    QString str = "https://www.mxnzp.com/api/weather/forecast/成都市?app_id=rgihdrm0kslojqvm&app_secret=WnhrK251TWlUUThqaVFWbG5OeGQwdz09";
    QUrl url(str);
    manager = new QNetworkAccessManager(this);
    request.setUrl(url);
    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, &Widget::doProcessFinished);

    QDateTime dt;
    QDate date;
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("MM-dd ddd");
    ui->label_curr_date->setText(currentDate);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::doProcessFinished(QNetworkReply *reply)
{
    QByteArray array = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(array);
    if(doc.isNull())
        return;

    QJsonObject obj = doc.object();
    if(false == obj.contains("data"))
        return;

    QJsonValue val = obj.value("data");
    if(!val.isObject())
        return;

    QJsonObject jsonDate = val.toObject();
    QString currentTemp = jsonDate.value("wendu").toString();
    QString city = jsonDate.value("city").toString();

    QJsonValue fore = jsonDate.value("forecasts");
    if(fore.isArray())
    {
        QJsonArray jsonArray = fore.toArray();
        for(auto i = 0; i < 4; ++i) {
            QJsonObject weatherObj = jsonArray[i].toObject();

            QString high = weatherObj.value("dayTemp").toString();
            QString low = weatherObj.value("nightTemp").toString();
            QString week = weatherObj.value("dayOfWeek").toString();
            QString weather = weatherObj.value("dayWeather").toString();

            switch (week.toInt()) {
            case 1:
                week = tr("星期一");
                break;
            case 2:
                week = tr("星期二");
                break;
            case 3:
                week = tr("星期三");
                break;
            case 4:
                week = tr("星期四");
                break;
            case 5:
                week = tr("星期五");
                break;
            case 6:
                week = tr("星期六");
                break;
            case 7:
                week = tr("星期天");
                break;
            default:
                break;
            }

            if(i == 0) {
                tempLabel->setText(high);
                tempLabel->adjustSize();

                currentWeatherLabel->setText(weather);
                currentWeatherLabel->adjustSize();

                ui->label_curr_city->setText(city);
                ui->label_curr_city->adjustSize();
                setWeatherPic(weather);
            }

            if(i == 1) {
                ui->label_future_day_1->setText(week);
                ui->label_future_weather_1->setText(weather);
                ui->label_future_temp_1->setText(low + "/" + high);
            }

            if(i == 2) {
                ui->label_future_day_2->setText(week);
                ui->label_future_weather_2->setText(weather);
                ui->label_future_temp_2->setText(low + "/" + high);
            }

            if(i == 3) {
                ui->label_future_day_3->setText(week);
                ui->label_future_weather_3->setText(weather);
                ui->label_future_temp_3->setText(low + "/" + high);
            }
        }
    }
}

void Widget::setWeatherPic(QString weather)
{
    qDebug() << weather;
    QString path;
    if(weather.contains("雨"))
        path = ":/res/pic/day-rain.png";
    else if(weather.contains("晴"))
        path = ":/res/pic/day-sun.png";
    else if(weather.contains("雪"))
        path = ":/res/pic/day-snow.png";
    else if(weather.contains("多云"))
        path = ":/res/pic/cloudy.png";
    else
        path = ":/res/pic/all.png";

    tempLabel->setStyleSheet("color:#ffffff");

    ui->frame_pic->setAutoFillBackground(true);
    QPalette palette = ui->frame_pic->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap(path).scaled(ui->frame_pic->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    ui->frame_pic->setPalette(palette);
}


void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        mousePress = true;
    movePoint = event->globalPos() - pos();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mousePress = false;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress) {
        QPoint movePos = event->globalPos();
        move(movePos - movePoint);
    }
}

void Widget::slotUp()
{
    static bool status = true;
    if(status)
        up();
    else
        down();
    status = !status;
}

void Widget::up()
{
    foldBtn->setText(tr("展开"));

    QPropertyAnimation *leftTempTextAnimation = new QPropertyAnimation(tempLabel, "pos");
    leftTempTextAnimation->setStartValue(tempLabel->pos());
    leftTempTextAnimation->setEndValue(QPoint(ui->frame_pic->pos().x() + 70, tempLabel->pos().y() - 80));
    leftTempTextAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    leftTempTextAnimation->setDuration(500);

    QPropertyAnimation *leftWeatherTextAnimation = new QPropertyAnimation(currentWeatherLabel, "pos");
    leftWeatherTextAnimation->setStartValue(currentWeatherLabel->pos());
    leftWeatherTextAnimation->setEndValue(QPoint(ui->frame_pic->pos().x() + 70, currentWeatherLabel->pos().y() - 100));
    leftWeatherTextAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    leftWeatherTextAnimation->setDuration(500);

    QPropertyAnimation *futureUpAnimation = new QPropertyAnimation(ui->frame_future, "pos");
    futureUpAnimation->setStartValue(ui->frame_future->pos());
    futureUpAnimation->setEndValue(QPoint(ui->frame_future->pos().x(), ui->frame_future->pos().y() - ui->frame_future->height() - 400));
    futureUpAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    futureUpAnimation->setDuration(300);

    QPropertyAnimation *picUpAnimation = new QPropertyAnimation(ui->frame_pic, "pos");
    picUpAnimation->setStartValue(ui->frame_pic->pos());
    picUpAnimation->setEndValue(QPoint(ui->frame_pic->pos().x(), ui->frame_pic->pos().y() - 400));
    picUpAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    picUpAnimation->setDuration(300);

    QSequentialAnimationGroup *posGroup = new QSequentialAnimationGroup(this);
    posGroup->addAnimation(leftTempTextAnimation);
    posGroup->addAnimation(leftWeatherTextAnimation);
    posGroup->addAnimation(futureUpAnimation);
    posGroup->addAnimation(picUpAnimation);
    posGroup->start(QAbstractAnimation::DeleteWhenStopped);

}

void Widget::down()
{
    foldBtn->setText(tr("折叠"));

    /*背景图移动动画*/
    QPropertyAnimation * picDownAnimation = new QPropertyAnimation(ui->frame_pic,"pos");
    picDownAnimation->setDuration(300);
    picDownAnimation->setStartValue(QPoint(ui->frame_pic->pos().x(),ui->frame_pic->pos().y()));
    picDownAnimation->setEndValue(QPoint(ui->frame_pic->pos().x(),ui->frame_pic->pos().y() + 400));
    picDownAnimation->setDirection(QAbstractAnimation::Direction::Forward);

    /*未来天气背景框移动动画*/
    QPropertyAnimation * futureDownAnimation = new QPropertyAnimation(ui->frame_future,"pos");
    futureDownAnimation->setStartValue(QPoint(ui->frame_future->pos().x(),ui->frame_future->pos().y()));
    futureDownAnimation->setEndValue(QPoint(ui->frame_future->pos().x(),ui->frame_future->pos().y() + ui->frame_future->height() + 400));
    futureDownAnimation->setDirection(QAbstractAnimation::Direction::Forward);
//    animation_future_up_1->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
    futureDownAnimation->setDuration(500);

    QGraphicsOpacityEffect *btnOpacity1 = new QGraphicsOpacityEffect(this);
    btnOpacity1->setOpacity(0);
    ui->frame_weather_1->setGraphicsEffect(btnOpacity1);
    QPropertyAnimation *pOpacityAnimation_1 = new QPropertyAnimation(btnOpacity1, "opacity");
    pOpacityAnimation_1->setDuration(300);
    pOpacityAnimation_1->setStartValue(0);
    pOpacityAnimation_1->setEndValue(1);

    QGraphicsOpacityEffect *btnOpacity2 = new QGraphicsOpacityEffect(this);
    btnOpacity2->setOpacity(0);
    ui->frame_weather_2->setGraphicsEffect(btnOpacity2);
    QPropertyAnimation *pOpacityAnimation_2 = new QPropertyAnimation(btnOpacity2, "opacity");
    pOpacityAnimation_2->setDuration(300);
    pOpacityAnimation_2->setStartValue(0);
    pOpacityAnimation_2->setEndValue(1);

    /*未来第三天天气文字渐变动画*/
    QGraphicsOpacityEffect *btnOpacity3 = new QGraphicsOpacityEffect(this);
    btnOpacity3->setOpacity(0);
    ui->frame_weather_3->setGraphicsEffect(btnOpacity3);
    QPropertyAnimation *pOpacityAnimation_3 = new QPropertyAnimation(btnOpacity3, "opacity");
    pOpacityAnimation_3->setDuration(300);
    pOpacityAnimation_3->setStartValue(0);
    pOpacityAnimation_3->setEndValue(1);

    /*当前温度文字的移动动画*/
    QPropertyAnimation * rightTempTextAnimation = new QPropertyAnimation(tempLabel,"pos");
    rightTempTextAnimation->setStartValue(tempLabel->pos());
    rightTempTextAnimation->setEndValue(QPoint(180,80));
    rightTempTextAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    rightTempTextAnimation->setDuration(500);

    /*当前天气状况文字的移动动画*/
    QPropertyAnimation * rightWeatherTextAnimation = new QPropertyAnimation(currentWeatherLabel,"pos");
    rightWeatherTextAnimation->setStartValue(currentWeatherLabel->pos());
    rightWeatherTextAnimation->setEndValue(QPoint(220,200));
    rightWeatherTextAnimation->setDirection(QAbstractAnimation::Direction::Forward);
    rightWeatherTextAnimation->setDuration(500);

    QSequentialAnimationGroup *posGroup = new QSequentialAnimationGroup(this);
    posGroup->addAnimation(picDownAnimation);
    posGroup->addAnimation(futureDownAnimation);
    posGroup->addAnimation(pOpacityAnimation_1);
    posGroup->addAnimation(pOpacityAnimation_2);
    posGroup->addAnimation(pOpacityAnimation_3);

    posGroup->addAnimation(rightTempTextAnimation);
    posGroup->addAnimation(rightWeatherTextAnimation);
    posGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::mSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 200);
    }
}
