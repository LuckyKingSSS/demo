#ifndef PLAYERMATERPROVIDERPRIVATE_H
#define PLAYERMATERPROVIDERPRIVATE_H

#include <QObject>
//#include "windows.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLETimelineBase.h"
#include "Interface/INLEMediaEncoder.h"
struct IMediaStreamFactory;
struct IMediaStream;

class INLEFactory;
class INLERender;
class WSCreateInstance;
class INLEMarkBook;
class INLEDocumentSerialize;
class PlayerMaterProviderPrivate : public QObject
{
	Q_OBJECT

public:
	PlayerMaterProviderPrivate(QObject *parent);
	~PlayerMaterProviderPrivate();

	//新的接口
	NLEComPtr<INLEFrame> CreateVideoFrame(int nW, int nH);
	NLEComPtr<INLEFrame> CreateAudioFrame(int frameRate);
	NLEComPtr<INLETimelineBase> CreateMediaTimeline(const QString &path);
	NLEComPtr<INLETimelineBase> CreateVideoFilterTimeline(const QString &filterName);
	NLEComPtr<INLETimelineBase> CreateAudioFilterTimeline(const QString &filterName);
	NLEComPtr<INLETimelineBase> CreateCaptionTimeline(const QString &path, QString name);
	NLEComPtr<INLEMediaEncoder> CreateMediaEncoder();
	NLEComPtr<INLETimelineBase> CloneTimeline(NLEComPtr<INLETimelineBase> srcTimeline);
	NLEComPtr<INLETimelineBase> CreateMontageTimeline(const QString &montageTemplateFiePath, const QString &montageName);
	NLEComPtr<INLEMarkBook> CreateMarkBook(const QString &path);
	NLEComPtr<INLETimelineBase> CreateTimelineFromDirectory(const QString &dir); //通过图片序列创建timeline
	NLEComPtr<INLETimelineBase> CreateTimelineForScreenRecording(const QString &path);
	NLEComPtr<INLEDocumentSerialize> CreateDocumentSerialize();

	QList<NLEComPtr<INLETimelineBase>> GetSubTimelines(NLEComPtr<INLETimelineBase> rootTimeline);
	int GetTimelineLength(NLEComPtr<INLETimelineBase> pTimeline);
	int GetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline);
	void SetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline, int nFrameRate);
	bool GetMediaRender(NLEComPtr<INLETimelineBase> mediaTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender);

	bool GetMediaCombineTimeline(NLEComPtr<INLETimelineBase> mediaTimeline,INLETimelineBase **ppTimeline);

	bool GetVideaFilterCombineRender(NLEComPtr<INLETimelineBase> filterTimeline,INLERender** ppVideoRender, INLERender** ppAudioRender);
	bool GetVideoFilterCombineTimeline(NLEComPtr<INLETimelineBase> filterTimeline, INLETimelineBase **ppTimeline);


	NLEComPtr<INLETimelineBase> CreateEmptyTimeLine();
	NLEComPtr<INLEProperties> NLEGetProperties(IUnknown * pUnk);
	NLEComPtr<INLETimelineBase> m_EmptyTimeline;
	NLEComPtr<INLETimelineBase> m_FilterSourceTimelime;
	INLEFactory *m_pNLEFactory;

};

#endif // PLAYERMATERPROVIDERPRIVATE_H
