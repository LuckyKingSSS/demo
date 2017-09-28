#pragma once
#include "commonlib_global.h"
#include <QObject>
//#include "COM\NLEInterface.h"
#include "Interface/INLETimelineBase.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLEMediaEncoder.h"

struct IMediaStream;
class INLERender;
class PlayerMaterProviderPrivate;
class INLEMarkBook;
class INLEDocumentSerialize;
class COMMONLIB_EXPORT PlayerMaterProvider : public QObject
{
	Q_OBJECT

public:
	PlayerMaterProvider(QObject *parent);
	~PlayerMaterProvider();

	//bool GetMediaStreamFromPath(const QString &path, IMediaStream** ppVideoStream, IMediaStream** ppAudioStream);

	NLEComPtr<INLEFrame> CreateVideoFrame(int nW,int nH);
	NLEComPtr<INLEFrame> CreateAudioFrame(int frameRate);
	NLEComPtr<INLETimelineBase> CreateEmptyTimeline();
	NLEComPtr<INLETimelineBase> CreateMediaTimeline(const QString &path);
	NLEComPtr<INLETimelineBase> CreateVideoFilterTimeline(const QString &filterName);
	NLEComPtr<INLETimelineBase> CreateAudioFilterTimeline(const QString &filterName);
	NLEComPtr<INLETimelineBase> CreateCaptionTimeline(const QString &path, QString name);
	NLEComPtr<INLEMediaEncoder> CreateMediaEncoder();
	NLEComPtr<INLETimelineBase> CreateMontageTimeline(const QString &montageTemplateFiePath, const QString &montageName);
	NLEComPtr<INLEMarkBook> CreateMarkBook(const QString &path);
	NLEComPtr<INLETimelineBase> CreateTimelineFromDirectory(const QString &path);
	NLEComPtr<INLETimelineBase> CreateTimelineForScreenRecording(const QString &path);
	NLEComPtr<INLEDocumentSerialize> CreateDocumentSerialize();

	QList<NLEComPtr<INLETimelineBase>> GetSubTimelines(NLEComPtr<INLETimelineBase> rootTimeline);
	NLEComPtr<INLETimelineBase> CloneTimeline(NLEComPtr<INLETimelineBase> srcTimeline);
	NLEComPtr<INLEProperties> NLEGetProperties(IUnknown * pUnk);
	int GetTimelineLength(NLEComPtr<INLETimelineBase> pTimeline);
	int GetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline);
	void SetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline,int nFrameRate);
	bool GetMediaRender(NLEComPtr<INLETimelineBase> mediaTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender);
	bool GetMediaCombineTimeline(NLEComPtr<INLETimelineBase> mediaTimeline, INLETimelineBase **ppTimeline);


	bool GetVideaFilterCombineRender(NLEComPtr<INLETimelineBase> filterTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender);
	bool GetVideoFilterCombineTimeline(NLEComPtr<INLETimelineBase> filterTimeline, INLETimelineBase **ppTimeline);

private:
	PlayerMaterProviderPrivate *m_p;
};

