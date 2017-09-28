#include "stdafx.h"
#include "AudioWaveMgr.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "TVControl.h"


#define DataCount_PerFrame	15		//每一帧去多少个点的数据
/*
获取音频数据
pData： 音频数据块地址
nSize:  数据块大小
return: 代表改数据块的音频值
*/
BYTE GetValue(BYTE* pData, INT nSize)
{
	SHORT *pEnd = (SHORT *)(pData + nSize);
	LONGLONG llTotalValue = 0, llCount = 0;
	for (SHORT *pCur = (SHORT *)pData; pCur <= pEnd; pCur++, llCount++)
	{
		LONGLONG llValue = abs(*pCur);//正负值 
		llTotalValue += BYTE(llValue * 256.0 / 32768.0);
	}
	return (BYTE)(llTotalValue * 1.0 / llCount);
}

BYTE GetMaxValue(BYTE* pData, INT nSize)
{
	SHORT *pEnd = (SHORT *)(pData + nSize);
	LONGLONG llMax = 0;
	for (SHORT *pCur = (SHORT *)pData; pCur <= pEnd; pCur++)
	{
		LONGLONG llValue = abs(*pCur) * 256.0 / 32768.0;//正负值 
		llMax = qMax(llMax, llValue);
	}
	return llMax;
}


QList<int> GetListValue(BYTE* pData, INT nSize,int fetchCnt)
{
	if (fetchCnt <= 0 || nSize < fetchCnt)
		return QList<int>();
	QList<int> res;
	SHORT *pEnd = (SHORT *)(pData + nSize);
	LONGLONG llCount = 0;
	int unitCnt = nSize / 2 / fetchCnt;
	int max = 0;
	for (SHORT *pCur = (SHORT *)pData; pCur <= pEnd; pCur++,llCount++)
	{
		int value = abs(*pCur) * 256.0 / 32768.0;//正负值 

		if (llCount % unitCnt == 0)
			max = 0;
		max = qMax(value, max);

		if (llCount % unitCnt == unitCnt - 1)
			res.push_back(max);
	}
	return res;
}


AudioWaveMgr::AudioWaveMgr(QObject *parent)
	: QObject(parent)
{

}

AudioWaveMgr::~AudioWaveMgr()
{

}


AudioWaveMgr::Result* AudioWaveMgr::GetTheSameOne(FSNLETimeline *anotherTL)
{
	for (auto result : m_lstResults)
	{
		if (result->IsSameOne(anotherTL))
			return result;
	}
	return nullptr;
}

AudioWaveMgr::Result* AudioWaveMgr::GetTheCopySrcOne(FSNLETimeline *anotherTL)
{
	QString guid = anotherTL->IsRecordTimeline() ? anotherTL->GetRecordAncestralGuidString() : anotherTL->GetAncestralGuidString();
	for (auto result : m_lstResults)
	{
		if (!result->IsSameTimeline(anotherTL) &&
			result->IsSameScale(anotherTL->GetRenderScale()) &&
			result->IsSameGuid(guid) &&
			result->IsSameDenoiseLevel(anotherTL->GetDenoiseLevel()))
			return result;
	}
	return nullptr;
}

QList<AudioWaveMgr::Result*> AudioWaveMgr::AllResults()
{
	return m_lstResults;
}

AudioWaveMgr* AudioWaveMgr::GetInstance()
{
	static AudioWaveMgr ins;
	return &ins;
}

void AudioWaveMgr::Add(FSNLETimeline *timeline)
{
	if (GetTheSameOne(timeline)) return;
	Result* fetchingRes = GetTheCopySrcOne(timeline);

	QString guid = timeline->IsRecordTimeline() ? timeline->GetRecordAncestralGuidString() : timeline->GetAncestralGuidString();
	auto cloneTimeline = fetchingRes ? fetchingRes->timeline : timeline->Clone();
	if (!fetchingRes) cloneTimeline->SetRange(0, cloneTimeline->GetTotalLength() - 1);
	auto thisResult = new Result(timeline,cloneTimeline);
	thisResult->ancesGuid = guid;
	if (fetchingRes)
	{
		thisResult->finish = fetchingRes->finish;
		thisResult->datas = fetchingRes->datas;
	}
	m_lstResults.push_back(thisResult);
	if (!fetchingRes)
	{
		thisResult->datas = new QList<int>();
		auto fetcher = new Fetcher(thisResult, 0);
		auto thread = new QThread(this);
		fetcher->moveToThread(thread);
		thread->start();
		fetcher->FetchData();
	}
}

QPixmap AudioWaveMgr::GetPixmap(FSNLETimeline *timeline, int pixelWidth, int pixelHight, int startFrame, int endFrame, int pixelDistance)
{
	auto res = GetTheSameOne(timeline);
	if (!res || !res->finish)
		return QPixmap();

	if (!res->needReflesh) return res->audioWave;

	res->needReflesh = false;
	auto lstData = res->datas;
	QRect rcWave(0, 0, pixelWidth, pixelHight);

	QPixmap px(pixelWidth, pixelHight);
	px.fill(Qt::transparent);
	QPainter painter(&px);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPainterPath path;
	QPoint startPt(2, pixelHight - 1);
	QPoint endPt(pixelWidth - 1, pixelHight - 1);
	path.moveTo(startPt);

	int cnt = lstData->size();

	auto ruler = TrackBuilder::GetInstance()->GetRuler();
	auto offset = ruler->Frames2PositionF(startFrame);

	int nDistance = pixelDistance;
	int nPrePixelX = 0;
	int nPixelX = 0;
	int nextPixelX = 0;
	int nPixelY = 0;
	for (int i = startFrame; i <= endFrame; ++i)
	{
		nPixelX = ruler->Frames2PositionF(i) - offset;
		if (nPixelX < 0)
			continue;
		else if (nPixelX > pixelWidth)
			break;

		nextPixelX = ruler->Frames2PositionF(i + 1) - offset;


		QList<int> frameDatas = lstData->mid(i * DataCount_PerFrame, DataCount_PerFrame);
		for (int k = 0; k < frameDatas.size(); k++)
		{
			if (nPixelX < nPrePixelX + nDistance)
				continue;
			if (nPixelX > nextPixelX - nDistance) 
				break;
			int res = frameDatas.at(k);
			nPixelY = pixelHight - 1.0 * (pixelHight * 0.8) / 256 * res;
			path.lineTo(nPixelX, nPixelY - 2);

			nPrePixelX = nPixelX;
			nPixelX += nDistance;
		}

		
	}
	path.lineTo(endPt);
	painter.fillPath(path, QBrush(QColor(42, 219, 140)));
	res->audioWave = px;
	painter.setRenderHint(QPainter::Antialiasing, false);
	return res->audioWave;
}

void AudioWaveMgr::SetNeedReflesh(bool bNeeded)
{
	for (auto result : m_lstResults)
	{
		result->needReflesh = bNeeded;
	}
}

bool AudioWaveMgr::IsNeedReflesh(FSNLETimeline *timeline)
{
	for (auto res : m_lstResults)
	{
		if (res->timeline == timeline)
			return res->needReflesh;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
Fetcher::Fetcher(AudioWaveMgr::Result* result, QObject *parent)
	:QObject(parent)
{
	m_pResult = result;
}

void Fetcher::FetchData()
{
	QMetaObject::invokeMethod(this, "_FetchData", Qt::QueuedConnection);
}

Q_INVOKABLE void Fetcher::_FetchData()
{
	//return;
	auto timeline = m_pResult->clone;
	int frames = timeline->GetTotalLength();

	INLERender *pVideoRender = NULL;
	INLERender *pAudioRender = NULL;
	NLEWraper::GetInstance().GetPlayerMaterProvider()->GetMediaRender(timeline->GetSrc(), nullptr, &pAudioRender);
	if (!pAudioRender) return;
	NLEComPtr<INLEFrame> pFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateAudioFrame(timeline->GetRenderFrameRate());
	if (!pFrame) return;

	HRESULT hr = pAudioRender->Seek(0);
	Q_ASSERT(hr == 0);
	hr = pAudioRender->Prepare(pFrame);
	Q_ASSERT(hr == 0);
	hr = pAudioRender->Render(pFrame);
	Q_ASSERT(hr == 0);
	if (!pFrame) return;
	DealWithData(pFrame);
	for (int i = 0; i < frames - 1; ++i)
	{
		int realIndex;
		hr = pAudioRender->Forward(1, &realIndex);
		Q_ASSERT(hr == 0);
		hr = pAudioRender->Prepare(pFrame);
		Q_ASSERT(hr == 0);
		hr = pAudioRender->Render(pFrame);
		Q_ASSERT(hr == 0);
		if (!pFrame) return;
		DealWithData(pFrame);
		pFrame->Reset();

	}
	m_pResult->finish = true;
	for (auto res : AudioWaveMgr::GetInstance()->AllResults())
	{
		if (!res->IsSameTimeline(m_pResult->timeline) &&
			res->IsSameScale(m_pResult->srcscale) &&
			res->IsSameGuid(m_pResult->ancesGuid) &&
			res->IsSameDenoiseLevel(m_pResult->denoiseLevel))
		{
			res->datas = m_pResult->datas;
			res->finish = true;
		}	
	}
	emit AudioWaveMgr::GetInstance()->sigFinish(m_pResult->ancesGuid);
}

void Fetcher::DealWithData(NLEComPtr<INLEFrame> pFrame)
{
	void* pData = pFrame->GetData();
	int nDataLen = pFrame->GetDataSize();
	Q_ASSERT(pData);
	Q_ASSERT(nDataLen > 0);
	auto res = GetListValue((BYTE*)pData, nDataLen, DataCount_PerFrame);
	m_pResult->datas->append(res);
}

