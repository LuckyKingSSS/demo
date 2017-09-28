#include "stdafx.h"
#include "PlayerMaterProvider.h"
#include "PlayerMaterProviderPrivate.h"
#include "INLEMarkdown.h"
#include "Interface/INLEDocumentSerialize.h"
PlayerMaterProvider::PlayerMaterProvider(QObject *parent)
	: QObject(parent)
{
	m_p = new PlayerMaterProviderPrivate(this);
}

PlayerMaterProvider::~PlayerMaterProvider()
{

}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateEmptyTimeline()
{
	return m_p->CreateEmptyTimeLine();
}

//bool PlayerMaterProvider::GetMediaStreamFromPath(const QString &path, IMediaStream** ppVideoStream, IMediaStream** ppAudioStream)
//{
//	return m_p->GetMediaStreamFromPath(path, ppVideoStream, ppAudioStream);
//}

NLEComPtr<INLEFrame> PlayerMaterProvider::CreateVideoFrame(int nW, int nH)
{
	return m_p->CreateVideoFrame(nW,nH);
}

NLEComPtr<INLEFrame> PlayerMaterProvider::CreateAudioFrame(int frameRate)
{
	return m_p->CreateAudioFrame(frameRate);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateMediaTimeline(const QString &path)
{
	return m_p->CreateMediaTimeline(path);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateVideoFilterTimeline(const QString &filterName)
{
	return m_p->CreateVideoFilterTimeline(filterName);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateAudioFilterTimeline(const QString &filterName)
{
	return m_p->CreateAudioFilterTimeline(filterName);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateCaptionTimeline(const QString &path, QString name)
{
	return m_p->CreateCaptionTimeline(path, name);
}

NLEComPtr<INLEMediaEncoder> PlayerMaterProvider::CreateMediaEncoder()
{
	return m_p->CreateMediaEncoder();
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateMontageTimeline(const QString &montageTemplateFiePath, const QString &montageName)
{
	return m_p->CreateMontageTimeline(montageTemplateFiePath, montageName);
}

NLEComPtr<INLEMarkBook> PlayerMaterProvider::CreateMarkBook(const QString &path)
{
	return m_p->CreateMarkBook(path);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateTimelineFromDirectory(const QString &dir)
{
	return m_p->CreateTimelineFromDirectory(dir);

}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CreateTimelineForScreenRecording(const QString &path)
{
	return m_p->CreateTimelineForScreenRecording(path);
}

NLEComPtr<INLEDocumentSerialize> PlayerMaterProvider::CreateDocumentSerialize()
{
	return m_p->CreateDocumentSerialize();
}

QList<NLEComPtr<INLETimelineBase>> PlayerMaterProvider::GetSubTimelines(NLEComPtr<INLETimelineBase> rootTimeline)
{
	return m_p->GetSubTimelines(rootTimeline);
}

NLEComPtr<INLETimelineBase> PlayerMaterProvider::CloneTimeline(NLEComPtr<INLETimelineBase> srcTimeline)
{
	return m_p->CloneTimeline(srcTimeline);
}

NLEComPtr<INLEProperties> PlayerMaterProvider::NLEGetProperties(IUnknown * pUnk)
{
	return m_p->NLEGetProperties(pUnk);
}

int PlayerMaterProvider::GetTimelineLength(NLEComPtr<INLETimelineBase> pTimeline)
{
	return m_p->GetTimelineLength(pTimeline);
}

int PlayerMaterProvider::GetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline)
{
	return m_p->GetTimelineFrameRate(pTimeline);
}

void PlayerMaterProvider::SetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline, int nFrameRate)
{
	return m_p->SetTimelineFrameRate(pTimeline, nFrameRate);
}

bool PlayerMaterProvider::GetMediaRender(NLEComPtr<INLETimelineBase> mediaTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender)
{
	bool bSucc = m_p->GetMediaRender(mediaTimeline, ppVideoRender, ppAudioRender);
	return bSucc;
}

bool PlayerMaterProvider::GetMediaCombineTimeline(NLEComPtr<INLETimelineBase> mediaTimeline, INLETimelineBase **ppTimeline)
{
	return m_p->GetMediaCombineTimeline(mediaTimeline,ppTimeline);
}

bool PlayerMaterProvider::GetVideaFilterCombineRender(NLEComPtr<INLETimelineBase> filterTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender)
{
	return m_p->GetVideaFilterCombineRender(filterTimeline, ppVideoRender, ppAudioRender);
}

bool PlayerMaterProvider::GetVideoFilterCombineTimeline(NLEComPtr<INLETimelineBase> filterTimeline, INLETimelineBase **ppTimeline)
{
	return m_p->GetVideoFilterCombineTimeline(filterTimeline,ppTimeline);
}