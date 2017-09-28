#include "inc_FProject/FProjectData.h"

#include "inc_FProject/FMediaData.h"


FProjectData::FProjectData()
{
    
}

FMediaData* FProjectData::GetMediaData()
{
    return &m_mediaData;
}

