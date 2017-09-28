#include "stdafx.h"
#include "FSSplashScreen.h"

FSSplashScreen::FSSplashScreen(QWidget *parent)
 : QSplashScreen(parent)
{
    setPixmap(QPixmap(":/FSRecorder/Resources/light/splash.png"));

    QString path = QApplication::applicationDirPath() + "/UpdateRegister.ini";
    QSettings settings(path, QSettings::IniFormat);
    m_version = settings.value("UpdateRegister/Version", "").toString();
    if (!m_version.isEmpty())
    {
        m_version = QString("%1:%2").arg(tr("Version")).arg(m_version);
    }
}

FSSplashScreen::~FSSplashScreen()
{

}

void FSSplashScreen::paintEvent(QPaintEvent *event)
{
    QSplashScreen::paintEvent(event);

    QPainter painter(this);
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    QPen pen = painter.pen();
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawText(QRect(0, 10, width() - 10, 40), Qt::AlignRight | Qt::AlignTop, m_version);
}
