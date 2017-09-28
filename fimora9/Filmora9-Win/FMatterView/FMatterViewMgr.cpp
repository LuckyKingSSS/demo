#include "stdafx.h"
#include "FMatterViewMgr.h"
#include "inc_FProject/FMediaData.h"

FMatterViewMgr::FMatterViewMgr(QObject *parent /* = 0 */ )
	: QObject(parent)
	, m_MatterType(MatterType::Matter_Media)
{

}

FMatterViewMgr::~FMatterViewMgr()
{

}

void FMatterViewMgr::Load(FMediaData* pMediaData)
{
	Q_ASSERT(pMediaData);
	m_MatterType = Matter_Media;
	m_pMediaData = pMediaData;
	InitMediaCategory();
}

void FMatterViewMgr::SetMatterType(MatterType matterType)
{
	m_MatterType = matterType;
}

QList<QPair<QString, int>>* FMatterViewMgr::GetCategory()
{
	if (m_MatterType == Matter_Media){
		return &m_MediaCategory;
	}
	else if (m_MatterType == Matter_Music){
		return &m_MusicCategory;
	}
	else if (m_MatterType == Matter_Text){
		return &m_TextCategory;
	}
	else if (m_MatterType == Matter_Transition){
		return &m_TransitionCategory;
	}
	else if (m_MatterType == Matter_Filter){
		return &m_FilterCategory;
	}
	else if (m_MatterType == Matter_Overlay){
		return &m_OverlayCategory;
	}
	else if (m_MatterType == Matter_Element){
		return &m_ElementCategory;
	}
	else if (m_MatterType == Matter_SplitScreen){
		return &m_SplitScreenCategory;
	}
	return NULL;
}

void FMatterViewMgr::InitMediaCategory()
{
	m_MediaCategory.clear();
	if (m_pMediaData->GetType() == FMediaData::Folder){
		FMediaFolder *pFolder = (FMediaFolder *)(m_pMediaData);
		FMediaDatas* pMediaDatas = pFolder->GetDatas();
		for (auto pData : *pMediaDatas){
			FMediaFolder *Folder = (FMediaFolder *)pData;
			m_MediaCategory.append(QPair<QString, int>(Folder->Name(), Folder->GetDatas()->count()));
		}
	}
	else if (m_pMediaData->GetType() == FMediaData::Timeline){

	}
}
