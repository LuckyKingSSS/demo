#include "stdafx.h"
#include "Filmora.h"
#include <QtWidgets/QApplication>

#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FNLETimelineFactory.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Filmora filmora(0);
    filmora.showMaximized();

    return a.exec();
}
