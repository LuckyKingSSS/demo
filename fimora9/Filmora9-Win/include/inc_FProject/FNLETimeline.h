#ifndef FNLETIMELINE_H
#define FNLETIMELINE_H
#include "INLETimelineBase.h"

#include "inc_FProject/fproject_global.h"

#include "inc_CommonLib/NLETimeline.h"

#include "inc_FProject/FRef.h"

class FTrack;



class FPROJECT_EXPORT FNLETimeline : public NLETimeline, public FRef
{
    //Q_OBJECT
public:
    enum Type
    {
        Unknown = 0,
        Video,
        Audio,
        VideoAudio,
    };
public:

    Type GetType();

    void SetType(Type type);

    // 片段在轨道上位置
    virtual int GetStartAtTrack();

    virtual int GetEndAtTrack();

    //片段长度
    virtual int GetLenAtTrack();

    // 片段的trimin
    int GetStartAtSrc();

    // 片段原始时长
    int GetSrcLen();

    // 设置片段位置(此处只保存变量, 当片段被真正增加到NLETimeline时使用此变量设置位置)
    void SetStartAtTrack(int pos);

    // 片段是否选择
    bool GetSelected();

    void SetSelected(bool selected);


    // 获取片段所在轨道Index
    int GetIndex();

    // 下一个片段
    NLEComPtr<FNLETimeline> NextTimeline();

    // 上一个片段
    NLEComPtr<FNLETimeline> PreTimeline();

    // 基类的Clone返回不是智能指针，新FNLETimeline需要so重新定义clone函数
    virtual NLEComPtr<FNLETimeline> clone();

public:
    void SetTrack(FTrack* track);
    FTrack* GetTrack();

protected:
    FNLETimeline(NLEComPtr<INLETimelineBase> timeline, QObject *parent = nullptr);
    // 使用clone()进行克隆
    FNLETimeline* Clone() override;
private:
    Type m_type;
    bool m_selected;
    int m_index;
    FTrack *m_track;
private:
    friend class FNLETimelineFactory;
};

typedef NLEComPtr<FNLETimeline> FNLETimelinePtr;
typedef QList<NLEComPtr<FNLETimeline>> FNLETimelines;




class FPROJECT_EXPORT FNLEDragTimeline
{
public:
    FNLEDragTimeline(FNLETimelinePtr timeline);
    // 片段在轨道上位置
    int GetStartAtTrack();

    //片段长度
    int GetLenAtTrack();

    void SetStartAtTrack(int frames);

    int GetIndex();
    void SetIndex(int index);

    FNLETimelinePtr GetTimeline();

private:
    int m_startAtTrack;
    int m_lenAtTrack;
    int m_trackIndex;

    FNLETimelinePtr m_timeline;
};

typedef QList<FNLEDragTimeline> FNLEDragTimelines;




#endif // FNLETIMELINE_H
