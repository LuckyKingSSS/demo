#ifndef FMEDIADATA_H
#define FMEDIADATA_H

#include "inc_FProject/fproject_global.h"

#include <QtCore/QString>
#include <QtCore/QList>

#include "inc_FProject/FNLETimeline.h"

class FNLETimeline;
class FMediaData;
typedef QList<FMediaData*> FMediaDatas;
//typedef FMediaDatas *PFMediaDatas;



class FPROJECT_EXPORT FMediaData
{
public:
    enum Type{Folder, Timeline};

    int GetType();

    QString Name();
    void SetName(const QString &name);

protected:
    int m_type;
    QString m_name;

    FMediaData(Type type);
};


class FPROJECT_EXPORT FMediaFolder : public FMediaData
{
public:
    void Add(FMediaData *data);
    void Remove(FMediaData *data);

    FMediaDatas* GetDatas();

    FMediaFolder();

protected:
    FMediaDatas m_datas;
};


class FPROJECT_EXPORT FMedia : public FMediaData
{
public:
    FMedia();

    void SetTimeline(FNLETimelinePtr timeline);
    FNLETimelinePtr GetTimeline();

    void SetPath(const QString &path);
    QString GetPath();

protected:
    FNLETimelinePtr m_timeline;
    QString m_path;
};




#endif // FNLETIMELINE_H
