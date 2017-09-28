#ifndef NLETIMELINE_H
#define NLETIMELINE_H


#include <QtCore/QObject>
//#include <QImage>
//#include "windows.h"
#include "NLEComPtr.h"
#include "NLEPropComPtr/NLEKeyShortcut.h"
#include "inc_CommonLib/commonlib_global.h"

enum ExactType
{
	ExactType_Unknown = 0,
	ExactType_Video,
	ExactType_Audio,
	ExactType_Image,
	ExactType_VideoFilter,
	ExactType_AudioFilter,
	ExactType_Montage,
	ExactType_Caption,
};


class INLETimelineBase;
class INLETimelineClipControl;
class INLERender;
class COMMONLIB_EXPORT NLETimeline : public QObject
{
	Q_OBJECT

public:
	NLETimeline(NLEComPtr<INLETimelineBase> pSrcTimeline, ExactType exacttype = ExactType_Unknown, QObject *parent = nullptr);
	~NLETimeline();
	// Ù–‘…Ë÷√
	virtual NLETimeline* Clone();

	void SetName(const QString &strName);
	QString GetName() const;
	int GetTotalLength();
	GUID GetGuid();
	QString GetGuidString();

	void SetLevel(int nLevel);
	void SetPosition(int nPost);
	int GetPosition();
	void SetTrimin(int nTrimin);
	int GetTrimin();
	void SetTrimout(int nTrimout);
	int GetTrimout();
	QPoint GetRange();
	void SetRange(int nTrimin, int nTrimout);
	int GetPlayLen();
	int GetSrcLen();
	bool GetRender(INLERender** ppVideoRender, INLERender** ppAudioRender);
	void SetRenderSize(int nDx, int nDy);
	QSize GetRenderSize();

	void SetCropRegion(const QRectF  &rc);

	QRectF GetCropRegion();

	void GetTimelineBase(INLETimelineBase **ppTimelineBase);

	NLEComPtr<INLETimelineBase> GetSrc();
	void SetAncestralGuid(const QString &strGuid);
	QString GetAncestralGuid() const;
	float GetSourceFramerate();
	float GetRenderFramerate();

	bool IsFrameIn(int nFrame);
	int GetCurrentFrame();
	QImage GetCurrentFrameImg(int nFrameW, int nFrameH);

	QPointF GetRegionCenter();
	void SetRegionCenter(QPointF center);

	double GetRegionRotateAngle();
	void SetRegionRotateAngle(double angle);

	QSizeF GetRegionScale();
	void SetRegionScale(QSizeF size);

	QPixmap GetFramePixmap(int nFrame, int nW, int nH);

	void SetRenderFrameRate(float frameRate);
	float GetRenderFrameRate();
	ExactType GetExactType() const;

	int GetDuration(); //s

	//≤Ÿ◊˜clip
	void AddClip(NLETimeline* pClip,int nStartPos = 0,int nLevel = 0);
	void RemoveClip(NLETimeline* pClip);

	void SetClipLevel(NLETimeline* pClip,int nLevel);
	void SetClipPos(NLETimeline* pClip,int nStartPos);
	void SetClipRange(NLETimeline* pClip,int nTrimin, int nTrimout);

	int GetClipCount();
	

    int GetVolume();
    void SetVolume(int volume);
	
    NLEPropComPtr GetProp();
    int     GetLevel();
    QSizeF    GetSourceSize();

    QRectF GetRenderRegion();
    void   SetRenderRegion(const QRectF  &rc);
    QRectF GetRealRenderRegion(QRectF  baseRc);
	void    SetRenderResizeScale(QSizeF scale);
	QSizeF    GetRenderResizeScale();
    void    SetRenderMode(NLERenderMode mode);


	void SetHiden(bool bHide);

	bool IsRenderScaleEnable();
	void SetRenderScaleEnable(bool bEnable);
	float GetRenderScale();
	void SetRenderScale(float scale);

    void SetRenderBackgroundColor(QColor color);
    QColor GetRenderBackgroundColor();
protected:
	bool IsClipExist(NLETimeline* pClip);
protected:
	NLEComPtr<INLETimelineBase> m_pSrcTimeLine;
	NLEPropComPtr m_pProp;
	QString m_strAncestralGuid;
	ExactType m_ExactType;
};

#endif // NLETIMELINE_H
