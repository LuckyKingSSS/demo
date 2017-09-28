#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "inc_FSExport/IFSExport.h"
#include "inc_FSCommonLib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "NLEType.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyleSheet("");
    qApp->setStyleSheet(gGetStyleSheet(":/FSExport/Resources/styles_light.qss") + gGetCommonStyleSheet());
    NLEComPtr<INLETimelineBase> pEmptyTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
    Q_ASSERT(pEmptyTimeline);
    NLEComPtr<INLETimelineBase> pTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline("C:/Users/ws/Desktop/My Video.mp4");
    Q_ASSERT(pTimeline);
    pEmptyTimeline->AddClip(pTimeline);
    QList<FSNLETimeline *> list;
    list.push_back(new FSNLETimeline(pEmptyTimeline, ExactType::ExactType_Video));
    list.first()->SetFilePath("C:/Users/ws/Desktop/My Video.mp4");
    IFSExport w(list, NULL);
    w.show();
   /* IFSEditMediaPlayer w(NULL);
    w.show();
    NLEComPtr<INLETimelineBase> pEmptyTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
    Q_ASSERT(pEmptyTimeline);
    NLEComPtr<INLETimelineBase> pTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline("C:\\Users\\Public\\Videos\\Sample Videos\\123.wmv");
    Q_ASSERT(pTimeline);
    pEmptyTimeline->AddClip(pTimeline);
    w.Open(1, new FSNLETimeline(pEmptyTimeline, ExactType::ExactType_Video));*/

    return a.exec();
}