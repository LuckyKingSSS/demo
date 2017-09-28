#ifndef FSSPLASHSCREEN_H
#define FSSPLASHSCREEN_H

#include <QtWidgets/QSplashScreen>

class FSSplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    FSSplashScreen(QWidget *parent = nullptr);
    ~FSSplashScreen();

private:
    void paintEvent(QPaintEvent *event);
    QString m_version;
};

#endif // FSSPLASHSCREEN_H
