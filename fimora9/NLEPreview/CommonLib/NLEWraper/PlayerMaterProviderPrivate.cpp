#include "stdafx.h"
#include "PlayerMaterProviderPrivate.h"

#include "Interface/INLEFactory.h"
#include "Interface/INLETimelineBase.h"
#include "WSCreateInstance.h"
#include "Include/NLEPropComPtr/NLEKeyShortcut.h"
#include "Include/NLEPropComPtr/NLESourcePropComPtr.h"
#include "Interface/INLEClonable.h"
#include "Interface/INLEDocumentSerialize.h"
#include "PathOperation.h"
#include "FileOperation.h"
#include "INLECaption.h"
#include "cCaptureStream.h"

PlayerMaterProviderPrivate::PlayerMaterProviderPrivate(QObject *parent)
: QObject(parent)
{
	WSCreateInstance::Instance()->wsCreateInstance(CLSID_NLEFactory, nullptr, 0, IID_INLEFactory, (void**)&m_pNLEFactory);
	m_pNLEFactory->Init(NULL);
	Q_ASSERT(m_pNLEFactory);
	m_EmptyTimeline = CreateEmptyTimeLine();
	//m_FilterSourceTimelime = CreateMediaTimeline(PathOperation::MakeWorkPath("ResConfig\\Video\\filter.mp4"));

	// 延后创建
	m_FilterSourceTimelime = nullptr;
}

PlayerMaterProviderPrivate::~PlayerMaterProviderPrivate()
{

}

NLEComPtr<INLEFrame> PlayerMaterProviderPrivate::CreateVideoFrame(int nW, int nH)
{
	NLEComPtr<INLEFrame> pFrame = NULL;
	pFrame =  m_pNLEFactory->CreateNLEVideoFrame(nW, nH);
	return pFrame;
}

NLEComPtr<INLEFrame> PlayerMaterProviderPrivate::CreateAudioFrame(int frameRate)
{
	NLEComPtr<INLEFrame> pFrame = NULL;
	pFrame = m_pNLEFactory->CreateNLEAudioFrame(NLERational(frameRate, 1));
	return pFrame;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateMediaTimeline(const QString &path)
{
	if (!FileOperation::IsFileExist(path)) return nullptr;

	std::wstring s = path.toStdWString();
	NLEComPtr<INLETimelineBase> pTimeline = m_pNLEFactory->CreateTimelineFromFile(s.c_str());

	return pTimeline;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateVideoFilterTimeline(const QString &filterName)
{
    
    //qDebug()<< filterName;
	std::wstring s = filterName.toStdWString();
    
	NLEComPtr<INLETimelineBase> pFielterTimeline = m_pNLEFactory->CreateVideoFilterTimeline(s.c_str());
	NLEPropComPtr propComp = NLEGetProperties(pFielterTimeline);
	propComp->Render()->SetRenderRange(NLERange(0,25 * 5));
	return pFielterTimeline;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateAudioFilterTimeline(const QString &filterName)
{
	std::wstring s = filterName.toStdWString();
	NLEComPtr<INLETimelineBase> pFielterTimeline = m_pNLEFactory->CreateAudioFilterTimeline(s.c_str());

	if (!pFielterTimeline) return NULL;

	NLEPropComPtr propComp = NLEGetProperties(pFielterTimeline);
	propComp->Render()->SetRenderRange(NLERange(0, 25 * 5));
	return pFielterTimeline;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateCaptionTimeline(const QString &path, QString name)
{
	auto captionTimeline = m_pNLEFactory->CreateCaptionTimeline();
	NLEComPtr<INLECaption> pCaption;
	captionTimeline->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
	Q_ASSERT(pCaption);
	if (!pCaption->LoadFromFile((BSTR)path.utf16()))
		return nullptr;

	NLEPropComPtr propCaption = NLEGetProperties(captionTimeline);
	propCaption->SetName((BSTR)name.utf16());

	return captionTimeline;
}

NLEComPtr<INLEMediaEncoder> PlayerMaterProviderPrivate::CreateMediaEncoder()
{
	return m_pNLEFactory->CreateMediaEncoder();
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateMontageTimeline(const QString &montageTemplateFiePath, const QString &montageName)
{
	std::wstring s = montageTemplateFiePath.toStdWString();
	return m_pNLEFactory->CreateMontageTimeline((LPWSTR)s.c_str(), (LPWSTR)montageName.utf16());
}

NLEComPtr<INLEMarkBook> PlayerMaterProviderPrivate::CreateMarkBook(const QString &path)
{
	if (!FileOperation::IsFileExist(path)) return nullptr;

	std::wstring s = path.toStdWString();
	NLEComPtr<INLEMarkBook> pTimeline = m_pNLEFactory->CreateMarkBook(s.c_str());

	return pTimeline;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateTimelineFromDirectory(const QString &dir)
{
	//NLEComPtr<INLEProperties> propTLapse;
	//auto hr = NLECommon::GetInterface(CLSID_NLEProperties, nullptr, 0, IID_INLEProperties, (void**)&propTLapse);
	//propTLapse->SetInt(NLEKey::Timelapse::kSequenceMode, 1);
	//propTLapse->SetString(NLEKey::Timelapse::kImageType, L".jpg");
	//propTLapse->SetRational(NLEKey::Timelapse::kFramerate, NLERational(5.0));
	//propTLapse->SetSize(NLEKey::Timelapse::kSize, NLESize(4000, 3000));
	//propTLapse->SetInt(NLEKey::Timelapse::kCache, 5);

	//auto stdwstring = dir.toStdWString();
	//auto timeline = m_pNLEFactory->CreateTimelineFromPath(stdwstring.c_str(), propTLapse);

	return nullptr;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateTimelineForScreenRecording(const QString &path)
{
	if (!FileOperation::IsFileExist(path)) 
		return nullptr;

	std::wstring s = path.toStdWString();

	NLEComPtr<INLETimelineBase> pEmptyTimeline = m_pNLEFactory->CreateTimeline(NULL);
	auto rootTimeline = m_pNLEFactory->CreateTimelineForScreenRecording(s.c_str());
	if (rootTimeline == nullptr)
		return nullptr;

    const wchar_t* pName = reinterpret_cast<const wchar_t*>(path.utf16());

    NLEPropComPtr propComp = NLEGetProperties(pEmptyTimeline);
    propComp->SetString(NLEKey::Base::kStrFileFullPath, pName);

	int childCount = 0;
	rootTimeline->GetClipCount(&childCount);
	if (rootTimeline && childCount > 0)
	{
		QList<NLEComPtr<INLETimelineBase>> timelines;
		NLEComPtr<INLEIterator> pIterator = NULL;
		rootTimeline->QueryInterface(IID_INLEIterator, (void**)&pIterator);
		for (pIterator->MoveToBegin(); pIterator->Current(); pIterator->MoveToNext())
		{
			NLEComPtr<INLETimelineBase> pTimelineTemp;
			pIterator->Current()->QueryInterface(IID_INLETimelineBase, (void**)&pTimelineTemp);
			NLE_ASSERT(pTimelineTemp);
			timelines.push_back(pTimelineTemp);
		}

		auto GetTimelineFromType = [&](int type)
		{
			NLEComPtr<INLETimelineBase> retTimeline;
			INT typeTemp = 0;
			for (auto temp : timelines)
			{
				NLEPropComPtr propComp = NLEGetProperties(temp);
				propComp->GetInt(kSource_ScreenRecordingStreamType, &typeTemp);
				if (typeTemp == type)
				{
					retTimeline = temp;
					break;
				}
			}
			return retTimeline;
		};

		std::vector<NLEComPtr<INLETimelineBase> > sortTimelines;
		auto AddTimeline2SortTimelines = [&](int type)
		{
			auto timelineTemp = GetTimelineFromType(type);
			if (timelineTemp)
				sortTimelines.push_back(timelineTemp);
		};
		
		//NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN = 0,
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO = 1, //桌面视频 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO = 2, //游戏视频 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO = 3, //系统声音 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO = 4,  //麦克风 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_MOUSE = 5, //鼠标流 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_KEYBOARD = 6, //键盘流 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO = 7, //摄像头视频 
		//	NLE_SCREEN_RECORDING_STREAM_TYPE_MARK = 8, //MARK数据 

		// 排序timeline
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_KEYBOARD);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_MOUSE);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO);
		AddTimeline2SortTimelines(NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO);


		for (int i = 0; i < sortTimelines.size(); ++i)
		{
			pEmptyTimeline->AddClip(sortTimelines[i]);
			NLEPropComPtr propComp = NLEGetProperties(sortTimelines[i]);
			propComp->SetLevel(i);
			propComp->SetPosition(0);
		}

		// kRender_Region 
		// 设置摄像头到右上角
		NLEComPtr<INLETimelineBase> webCamTimeline = GetTimelineFromType(NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO);
		NLEComPtr<INLETimelineBase> desktopCamTimeline = GetTimelineFromType(NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO);
		NLEComPtr<INLETimelineBase> gameCamTimeline = GetTimelineFromType(NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO);
		if (webCamTimeline && (desktopCamTimeline || gameCamTimeline))
		{
			// 设置摄像头比例
			NLESourcePropComPtr propComp = NLEGetProperties(webCamTimeline);
			NLERectF rectF(0.6f, 0, 0.4, 0.4);

			NLESize size;
			propComp->GetSourceSize(&size);
			
			NLESourcePropComPtr desktopPropComp;
			if(desktopCamTimeline)
				desktopPropComp = NLEGetProperties(desktopCamTimeline);
			else
				desktopPropComp = NLEGetProperties(gameCamTimeline);

			NLESize desktopSize;
			desktopPropComp->GetSourceSize(&desktopSize);

            float desktopRate = (float)desktopSize.Width / desktopSize.Height;
            float webCamRate = (float)size.Width / size.Height;
            int webCamWidth = desktopSize.Width * rectF.Width;
            int webCamHeight = desktopSize.Height * rectF.Height;
            int webCamRealWidth = 0;
            int webCamRealHeight = 0;
            if (desktopRate > webCamRate)
            {
                webCamRealWidth = webCamHeight * webCamRate;
                webCamRealHeight = webCamHeight;
            }
            else
            {
                webCamRealWidth = webCamWidth;
                webCamRealHeight = webCamWidth / webCamRate;
            }
            rectF.Width = (float)webCamRealWidth / desktopSize.Width;
            rectF.Height = (float)webCamRealHeight / desktopSize.Height;
            rectF.X = 1 - rectF.Width;
            rectF.Y = 0;

			NLEPropComPtr webPropComp = NLEGetProperties(webCamTimeline);
			webPropComp->SetRectF(kRender_Region, rectF);
			

		}

		// 设置rendersize为其中一视频的sourcesize
		NLESourcePropComPtr renderSizeTimeline;
		if (desktopCamTimeline)
			renderSizeTimeline = NLEGetProperties(desktopCamTimeline);
		else if (gameCamTimeline)
			renderSizeTimeline = NLEGetProperties(gameCamTimeline);
		else if (webCamTimeline)
			renderSizeTimeline = NLEGetProperties(webCamTimeline);

		if (renderSizeTimeline)
		{
			NLERenderPropComPtr pEmptyTimelinePro = NLEGetProperties(pEmptyTimeline);
			NLESize sizeTemp;
			renderSizeTimeline->GetSourceSize(&sizeTemp);
			pEmptyTimelinePro->SetRenderSize(sizeTemp);

		}
	}
	else
	{
		pEmptyTimeline = rootTimeline;
	}

	return pEmptyTimeline;
	
}

NLEComPtr<INLEDocumentSerialize> PlayerMaterProviderPrivate::CreateDocumentSerialize()
{
	return m_pNLEFactory->CreateDocumentSerialize();
}

QList<NLEComPtr<INLETimelineBase>> PlayerMaterProviderPrivate::GetSubTimelines(NLEComPtr<INLETimelineBase> rootTimeline)
{
	QList<NLEComPtr<INLETimelineBase>> timelines;
	NLEComPtr<INLEIterator> pIterator = NULL;
	rootTimeline->QueryInterface(IID_INLEIterator, (void**)&pIterator);

	for (pIterator->MoveToBegin(); pIterator->Current(); pIterator->MoveToNext())
	{
		NLEComPtr<INLETimelineBase> pTimelineTemp;
		pIterator->Current()->QueryInterface(IID_INLETimelineBase, (void**)&pTimelineTemp);
		NLE_ASSERT(pTimelineTemp);
		timelines.push_back(pTimelineTemp);
	}
	return timelines;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CloneTimeline(NLEComPtr<INLETimelineBase> srcTimeline)
{
	NLEComPtr<INLEClonable> pClonable = NULL;
	HRESULT hr = srcTimeline->QueryInterface(IID_INLEClonable, (LPVOID*)&pClonable);
	if (!SUCCEEDED(hr)) return NULL;

	NLEComPtr<INLETimelineBase> pNewTimeline = NULL;
	hr = pClonable->Clone((LPVOID*)&pNewTimeline);
	if (!SUCCEEDED(hr)) return NULL;

	return pNewTimeline;
}

int PlayerMaterProviderPrivate::GetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline)
{
	if (!pTimeline) return 0;
	NLEPropComPtr propComp = NLEGetProperties(pTimeline);
	int nLength = 0;
	NLERational rational;
	propComp->GetRenderFrameRate(&rational);
	return (int)rational.Value();
}

void PlayerMaterProviderPrivate::SetTimelineFrameRate(NLEComPtr<INLETimelineBase> pTimeline, int nFrameRate)
{
	if (!pTimeline) return;
	NLEPropComPtr propComp = NLEGetProperties(pTimeline);
	int nLength = 0;
	NLERational rational(nFrameRate, 1);
	propComp->SetRenderFrameRate(rational);
}

int PlayerMaterProviderPrivate::GetTimelineLength(NLEComPtr<INLETimelineBase> pTimeline)
{
	if (!pTimeline) return 0;
	NLEPropComPtr propComp = NLEGetProperties(pTimeline);
	int nLength = 0;
	propComp->GetRenderTotalFrameCount(&nLength);
	return nLength;
}

bool PlayerMaterProviderPrivate::GetMediaRender(NLEComPtr<INLETimelineBase> mediaTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender)
{
	HRESULT res = mediaTimeline->GetRender(ppAudioRender, ppVideoRender);
	return SUCCEEDED(res);
}

bool PlayerMaterProviderPrivate::GetMediaCombineTimeline(NLEComPtr<INLETimelineBase> mediaTimeline, INLETimelineBase **ppTimeline)
{
	//m_EmptyTimeline->RemoveAll();
	//if (!SUCCEEDED(m_EmptyTimeline->AddClip(mediaTimeline))) return false;
	//*ppTimeline = m_EmptyTimeline;

	*ppTimeline = mediaTimeline;
	return true;
}

bool PlayerMaterProviderPrivate::GetVideaFilterCombineRender(NLEComPtr<INLETimelineBase> filterTimeline, INLERender** ppVideoRender, INLERender** ppAudioRender)
{
	return SUCCEEDED(filterTimeline->GetRender(ppAudioRender, ppVideoRender));

	//GetVideoFilterCombineTimeline(filterTimeline,NULL);
	//return SUCCEEDED(m_EmptyTimeline->GetRender(ppAudioRender, ppVideoRender));
}

bool PlayerMaterProviderPrivate::GetVideoFilterCombineTimeline(NLEComPtr<INLETimelineBase> filterTimeline, INLETimelineBase **ppTimeline)
{
	//m_EmptyTimeline->RemoveAll();
	//if (!SUCCEEDED(m_EmptyTimeline->AddClip(m_FilterSourceTimelime))) return false;

	if (m_FilterSourceTimelime == nullptr)
	{
		m_FilterSourceTimelime = CreateMediaTimeline(PathOperation::MakeWorkPath("ResConfig\\Video\\filter.mp4"));
		Q_ASSERT(m_FilterSourceTimelime);
		
	}

	if (m_FilterSourceTimelime)
	{
		m_FilterSourceTimelime->RemoveAll();
		NLEComPtr<INLETimelineBase> pClone = CloneTimeline(filterTimeline);
		//if (!SUCCEEDED(m_EmptyTimeline->AddClip(pClone))) return false;
		m_FilterSourceTimelime->AddClip(pClone);

		int nCnt;
		NLEPropComPtr propSrc = NLEGetProperties(m_FilterSourceTimelime);
		propSrc->Render()->GetRenderTotalFrameCount(&nCnt);
		propSrc->SetLevel(0);
		propSrc->SetPosition(0);
		propSrc->SetRenderRange(NLERange(0, nCnt));

		NLEPropComPtr propFilter = NLEGetProperties(pClone);
		propFilter->SetLevel(1);
		propFilter->SetPosition(0);
		propFilter->SetRenderRange(NLERange(0, nCnt));

		*ppTimeline = m_FilterSourceTimelime;

		return true;
	}

	return false;
}

NLEComPtr<INLEProperties> PlayerMaterProviderPrivate::NLEGetProperties(IUnknown *pUnk)
{
	NLEComPtr<INLEProperties> pProperties;
	HRESULT hr = pUnk->QueryInterface(IID_INLEProperties, (LPVOID*)&pProperties);
	if (FAILED(hr))
	{
		return NULL;
	}
	return pProperties;
}

NLEComPtr<INLETimelineBase> PlayerMaterProviderPrivate::CreateEmptyTimeLine()
{
	NLEComPtr<INLETimelineBase> pEmptyTimeline = m_pNLEFactory->CreateTimeline(NULL);
	NLEPropComPtr propComp = NLEGetProperties(pEmptyTimeline);
	propComp->SetName(L"TopTimeline");
	//propComp->Render()->SetRenderSize(NLESize(640, 480));

	//propComp->Render()->SetRenderSampleRate(44100);
	//propComp->Render()->SetRenderChannels(2);
	//propComp->Render()->SetRenderBitsPerSample(16);

	return pEmptyTimeline;
}

//bool PlayerMaterProviderPrivate::GetMediaStreamFromPath(const QString &path, IMediaStream** ppVideoStream, IMediaStream** ppAudioStream)
//{
//	const wchar_t *wpath = reinterpret_cast<const wchar_t*>(path.utf16());
//	return SUCCEEDED(m_mediaStreamFactory->CreateMediaStreamByFilePath(wpath, ppAudioStream, ppVideoStream));
//}
