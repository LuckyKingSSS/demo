#ifndef FSNLETIMELINE_H
#define FSNLETIMELINE_H

#include "inc_CommonLib/NLETimeline.h"


#include "inc_FSCommonLib/fscommonlib_global.h"
#include <QtGui/QPixmap>
#include <memory>


static LPCWSTR Key_MouseFilterType = L"Key_MouseFilterType";
#define Value_MouseFilterType_Highlight		1
#define Value_MouseFilterType_Rings			2


static LPCWSTR Key_AncestralGuid = L"Key_AncestralGuid";
static LPCWSTR Key_RecordAncestralGuid = L"Key_RecordAncestralGuid";
static LPCWSTR Key_DisplayName = L"Key_DisplayName";
static LPCWSTR Key_TimelineType = L"Key_TimelineType";
static LPCWSTR Key_CaptionThumnnail = L"Key_CaptionThumnnail";
static LPCWSTR Key_BackGroundColor = L"Key_BackGroundColor";
static LPCWSTR Key_BackGroundColorChangerByUser = L"Key_BackGroundColorChangerByUser";
static LPCWSTR Key_ClipMarkers = L"Key_ClipMarkers";
static LPCWSTR Key_TrackMarkers = L"Key_TrackMarkers";
static LPCWSTR Key_HidenTracks = L"Key_HidenTracks";
static LPCWSTR Key_FreezenTracks = L"Key_FreezenTracks";
enum TimelineType
{
	TimelineType_Unknown = 0,
	TimelineType_Complex,
	TimelineType_Video,
	TimelineType_Audio,
	TimelineType_VideoAudio,
	TimelineType_Image,
	TimelineType_Annotaions_Caption,
	TimelineType_Annotaions_Overlay,
	TimelineType_Annotaions_Element,
	TimelineType_MouseHightlineFilter,
	TimelineType_MouseClickRingsFilter,
	
};

class FSCOMMONLIB_EXPORT MarkPoint : public QObject
{
	Q_OBJECT
public:
	MarkPoint(QObject *parent = nullptr);
	MarkPoint(int frame, QColor color, QObject *parent = nullptr);
	MarkPoint& operator=(const MarkPoint &mp);

	void SetFrame(int nFrame);
	int GetFrame();
	void SetColor(QColor clr);
	QColor GetColor();
	void SetSelected(bool bSelected);
	bool IsSelected();
private:
	int m_frame;
	bool m_selected;
	QColor m_bkcolor;
};
typedef std::shared_ptr<MarkPoint> MarkPointPtr;


class FSCOMMONLIB_EXPORT FSNLETimeline : public NLETimeline
{
	Q_OBJECT
public:
	FSNLETimeline(NLEComPtr<INLETimelineBase> pSrcTimeline, ExactType exacttype = ExactType_Unknown, QObject *parent = nullptr);
	~FSNLETimeline();
	FSNLETimeline* Clone() override;

    QString GetFilePath();
    void    SetFilePath(QString path);

    void    SetAnchorPointF(QPointF anchor);
    void    SetRenderMode(NLERenderMode mode);

    NLERenderMode GetRenderMode();


    bool    IsContain(QPoint pt, QRectF  baseRc);

    void SetMaskPath(const QString &pathPoint);
    QString GetMaskPath();

    void SetMix2Mono(bool bEnable);
    bool GetMix2Mono();


    void SetDenoiseLevel(int level);
    int  GetDenoiseLevel();


	TimelineType GetType();
	void SetType(TimelineType type,bool bGetThumbnail = true);
	
	void SetThumbnail(QPixmap px);
	QPixmap GetThumbnail();
    QPixmap GetThumbnail(int nWidth, int nHeight);
    QPixmap GetThumbnail(int frames, int nWidth, int nHeight);
	void _FetchThumbnail(int nFrame = 5 * 25);
	QPixmap ExtractImage(int nFrame, int nWidth, int nHeight);
	QString GetDurationString(); //hh:mm:ss

	int GetFrameAudio(int nFrame);

	TimelineType GetNLETimelineType();

	bool IsRecordTimeline();
	bool IsDesktopVideo();
    bool IsGameVideo();
    bool IsCameraVideo();
	bool IsMicrophoneAudio();
	bool IsSystemAudio();
	bool IsPureAudio();
    int GetRecordingStreamType();

    QString GetAncestralGuidString();
	void SetAncestralGuidString(const QString &str);

	QString GetRecordAncestralGuidString();
	void SetRecordAncestralGuidString(const QString &str);

	void SetDisplayName(const QString &name);
	QString GetDisplayName();

	void SetDependency(FSNLETimeline *pDen);
	FSNLETimeline* GetDependency();

	//按照srcSize的比例缩放至dstSize,
	static QSize GetRatioSize(QSize srcSize, QSize dstSize);  
	QString GetSourceFilePath();

	void SetBackgroundColor(QColor clr,bool bDefaultSet = true);
	QColor GetBackgroundColor();
	bool IsBkColorChangeByUser();

	void SetMarkers(const QList<int> &markers);
	QList<int> GetMarkers();
	void AddMarker(int frame);
	void RemoveMarker(int frame);
	void SelectMarker(int frame, bool bSelected);
	int GetSelectedMarker();
	void UnselectMarker();
	bool CanAddMarker();
	int ClipMarkPos2TrackMarkPos(int pos);
	int TrackMarkPos2ClipMarkPos(int pos);
	QList<int> GetTrackMarkPos();

	//track mark point
	void UpdateTrackMarks(QList<MarkPointPtr> mps);
	QList<MarkPointPtr> CreateTrackMarks();


	//
	void SetHidenTracks(QList<int> tracks);
	QList<int> GetHidenTracks();
	void SetFreezenTracks(QList<int> tracks);
	QList<int> GetFreezenTracks();
Q_SIGNALS:
	void sigFetchedThumbnail(FSNLETimeline *tl);
	void sigBeforeDelete();
protected:
	NLEFileType _GetNLEFileType();
private:
	QPixmap m_thumbnail;
	FSNLETimeline *m_pDependency;  // 摄像头依附的desktop或游戏视频
	int m_selectedMarker;
};

#endif // FSCOMMONLIB_H
