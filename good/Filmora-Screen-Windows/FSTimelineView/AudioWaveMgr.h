#ifndef AUIDOWAVEMGR_H
#define AUIDOWAVEMGR_H

#include <QObject>
#include "NLEComPtr.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
class FSNLETimeline;
class AudioWaveMgr : public QObject
{
	Q_OBJECT

public:
	struct Result
	{
		FSNLETimeline* timeline;
		FSNLETimeline* clone;
		QString ancesGuid;
		QList<int> *datas;
		bool finish;
		QPixmap audioWave;
		bool needReflesh;
		float srcscale;
		int denoiseLevel;

		bool IsSameTimeline(FSNLETimeline *anotherTL) 
		{ 
			return timeline == anotherTL;
		}
		bool IsSameGuid(QString anotherGuid)
		{ 
			return ancesGuid == anotherGuid; 
		}
		bool IsSameScale(float anotherScale)
		{ 
			return qAbs(srcscale - anotherScale) < 0.01;
		}
		bool IsSameDenoiseLevel(int anotherDL)
		{
			return denoiseLevel == anotherDL;
		}
		bool IsSameOne(FSNLETimeline *anotherTL)
		{ 
			return IsSameTimeline(anotherTL) && IsSameScale(anotherTL->GetRenderScale()) && IsSameDenoiseLevel(anotherTL->GetDenoiseLevel());
		}
		Result(FSNLETimeline* tl, FSNLETimeline* clonetl):timeline(tl), clone(clonetl), finish(false), needReflesh(true), datas(nullptr)
		{
			srcscale = tl->GetRenderScale();
			denoiseLevel = tl->GetDenoiseLevel();
		}
	};
	static AudioWaveMgr* GetInstance();
	~AudioWaveMgr();
	QList<Result*> AllResults();
	void Add(FSNLETimeline *timeline);
	QPixmap GetPixmap(FSNLETimeline *timeline, int pixelWidth, int pixelHight, int startFrame, int endFrame, int pixelDistance);

	void SetNeedReflesh(bool bNeeded);
	bool IsNeedReflesh(FSNLETimeline *timeline);
Q_SIGNALS:
	void sigFinish(QString ancestralGuid);
protected:
	AudioWaveMgr(QObject *parent = 0);

	Result* GetTheSameOne(FSNLETimeline *anotherTL);
	Result* GetTheCopySrcOne(FSNLETimeline *anotherTL);
private:
	QList<Result*> m_lstResults;
};

class INLEFrame;
class Fetcher : public QObject
{
	Q_OBJECT
public:
	Fetcher(AudioWaveMgr::Result* result, QObject *parent);
	void FetchData();

protected:
	Q_INVOKABLE void _FetchData();

	void DealWithData(NLEComPtr<INLEFrame> pFrame);

private:
	AudioWaveMgr::Result* m_pResult;
};

#endif // AUIDOWAVEMGR_H
