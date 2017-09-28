
#include "inc_FProject/FMediaData.h"

FMediaData::FMediaData(Type type)
{
    m_type = type;
}

int FMediaData::GetType()
{
    return m_type;
}

QString FMediaData::Name()
{
    return m_name;
}

void FMediaData::SetName(const QString &name)
{
    m_name = name;
}

//////////////////////////////////////////////////////////////////////////
FMediaFolder::FMediaFolder()
:FMediaData(Folder)
{
   // m_datas = new FMediaDatas();
}



void FMediaFolder::Add(FMediaData *data)
{
    m_datas.push_back(data);
}

void FMediaFolder::Remove(FMediaData *data)
{
    Q_ASSERT(m_datas.contains(data));
    m_datas.removeOne(data);
}

FMediaDatas* FMediaFolder::GetDatas()
{
    return &m_datas;
}

//////////////////////////////////////////////////////////////////////////

FMedia::FMedia()
: FMediaData(Timeline)
{
   
}

void FMedia::SetTimeline(FNLETimelinePtr timeline)
{
    m_timeline = timeline;
}

FNLETimelinePtr FMedia::GetTimeline()
{
    return m_timeline;
}

void FMedia::SetPath(const QString &path)
{
    m_path = path;
}

QString FMedia::GetPath()
{
    return m_path;
}

