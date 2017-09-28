#include "stdafx.h"

#include <QtXml/QtXml>


#include "MVListItemData.h"
#include "inc_CommonLib/commonlib_global.h"
#include "inc_FSCommonlib/FSNLETimeline.h"

#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/XmlOperation.h"

#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"

#include "inc_FSCommonlib/ExternalAPI.h"


//////////////////////////////////////////////////////////////////////////
MediaData::MediaData(MVItemDataType type, QObject *parent /*= 0*/)
	:ItemDataBase(type, parent)
{

}

void MediaData::SetSourceFile(const QString &file)
{
	m_strSourceFile = file;
}

QString MediaData::GetSourceFile()
{
	return m_strSourceFile;
}

//////////////////////////////////////////////////////////////////////////
VideoData::VideoData(QObject *parent /*= 0*/)
	: MediaData(MVItemDataType_Video, parent)
{
}

//////////////////////////////////////////////////////////////////////////
AudioData::AudioData(QObject *parent /*= 0*/)
	: MediaData(MVItemDataType_Audio, parent)
{
}

//////////////////////////////////////////////////////////////////////////
ImageData::ImageData(QObject *parent /*= 0*/)
	: MediaData(MVItemDataType_Imgage, parent)
{
}

//////////////////////////////////////////////////////////////////////////
RecordData::RecordData(QObject *parent /*= 0*/)
	: MediaData(MVItemDataType_Record, parent)
{

}

//////////////////////////////////////////////////////////////////////////
AnnotationItemData::AnnotationItemData(AnnotationData *data, AnnotationDataHelper *helper, QObject *parent /*= 0*/)
:MediaData(MVItemDataType_Annotation, parent)
    , m_data(data)
    ,m_helper(helper)
{
    Q_ASSERT(data);
}


QString AnnotationItemData::GetCatalog()
{
    return m_data->catalog;
}

QString AnnotationItemData::GetAlias()
{
    return m_data->alias;
}


FSNLETimeline* AnnotationItemData::GetTimeline()
{
    // 测试Annotation使用
    bool test = FSGetSettingsTest("AnnotationTest", false).toInt();
    if (test)
    {
        if (m_pTimeline)
            delete m_pTimeline;
        m_pTimeline = m_helper->CreateFSNLETimeline(this);
    }
    else
    {
        // 动态加载
        if (m_pTimeline == nullptr)
        {
            m_pTimeline = m_helper->CreateFSNLETimeline(this);
        }
    }

    
    return m_pTimeline;
}

AnnotationDataHelper *AnnotationItemData::GetHelper()
{
    return m_helper;
}

AnnotationData* AnnotationItemData::GetData()
{
    return m_data;
}

/////////////////////////////////////////////////////////////////////////
CursorHightlineData::CursorHightlineData(QObject *parent /*= 0*/)
	:ItemDataBase(MVItemDataType_CursorHightline, parent)
{

}

//////////////////////////////////////////////////////////////////////////
ClickRingData::ClickRingData(QObject *parent /*= 0*/)
	:ItemDataBase(MVItemDataType_ClickRing, parent)
{

}

//////////////////////////////////////////////////////////////////////////
ClickSoundData::ClickSoundData(QObject *parent /*= 0*/)
	:ItemDataBase(MVItemDataType_ClickSound,parent)
{

}

QString ClickSoundData::GetSoundFile()
{
	return m_strSoundFile;
}

void ClickSoundData::SetSoundFile(const QString &file)
{
	m_strSoundFile = file;
}


//////////////////////////////////////////////////////////////////////////
AnnotationDataHelper::AnnotationDataHelper(QObject *parent)
:QObject(parent)
{
	RetranslateText();
}

QList<std::string> AnnotationDataHelper::GetCatagoryStrings()
{
    QList<std::string> lst;
	lst.push_back(m_favoriteString.toStdString());
	for (auto c : m_catalogs)
	{
		lst.push_back(c.untrtext);
	}
	return lst;
}

AnnotationItems AnnotationDataHelper::GetItems()
{
    return m_items;
}

AnnotationItems AnnotationDataHelper::GetItems(QString catalog)
{
    AnnotationItems items;
    for (auto temp : m_items)
    {
        if (temp->GetCatalog() == catalog)
            items.push_back(temp);
    }
    return items;
}

bool AnnotationDataHelper::BelongCatalog(AnnotationItemData *item, QString catalog)
{
	if (item->GetCatalog() == catalog)
	{
		return true;
	}
	else if (catalog == m_favoriteString && m_favorites.contains(item))
	{
		return true;
	}
	else
		return false;
}

bool AnnotationDataHelper::IsFavorite(AnnotationItemData *item)
{
    return m_favorites.contains(item);
}

QString AnnotationDataHelper::GetFavoriteString()
{
    return m_favoriteString;
}

void AnnotationDataHelper::AddItem2Favorite(AnnotationItemData *item)
{
    if (!m_favorites.contains(item))
    {
        m_favorites.push_back(item);
    }
}

void AnnotationDataHelper::RemoveItemFromFavorite(AnnotationItemData *item)
{
    m_favorites.removeOne(item);

}

FSNLETimeline* AnnotationDataHelper::CreateFSNLETimeline(AnnotationItemData *item)
{
    auto catalog = item->GetCatalog();
    auto data = item->GetData();
    FSNLETimeline *fstimeline = nullptr;
    NLEComPtr<INLETimelineBase> timeline;
    if (data->type == 0)		//caption
    {
        timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateCaptionTimeline(data->resfile, data->displayName);
        if (timeline == nullptr)
            return fstimeline;
        fstimeline = new FSNLETimeline(timeline);
        fstimeline->SetType(TimelineType_Annotaions_Caption, false);
    }
	else if (data->type == 1)	// overlay
    {
        timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateTimelineForScreenRecording(data->resfile);
		if (timeline == nullptr)
			return fstimeline;
		fstimeline = new FSNLETimeline(timeline);
		fstimeline->SetType(TimelineType_Annotaions_Overlay, false);

    }
	else if (data->type == 2) //element
	{
		timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateCaptionTimeline(data->resfile, data->displayName);
		if (timeline == nullptr)
			return fstimeline;
		fstimeline = new FSNLETimeline(timeline);
		fstimeline->SetType(TimelineType_Annotaions_Element, false);
	}
    else
    {
        Q_ASSERT(false);
        return fstimeline;
    }
    fstimeline->SetAncestralGuidString(fstimeline->GetGuidString());
    fstimeline->SetDisplayName(data->displayName);
    fstimeline->SetThumbnail(item->GetThumbnail());
    fstimeline->SetRange(0, 5 * 25 -1 );
    item->SetTimeline(fstimeline);

    return fstimeline;
}

bool AnnotationDataHelper::LoadCategorys(QString path)
{
	QDomDocument doc;

	if (!XmlOperation::OpenXmlFile(doc, path))
		return false;

	auto rootElem = doc.documentElement();
	if (rootElem.isNull())
		return false;

	auto categoryElem = rootElem.firstChildElement("Category");

	while (!categoryElem.isNull())
	{
		CategoryData data;

		auto elem = categoryElem.firstChildElement("type");
		if (!elem.isNull())
			data.type = elem.text().toInt();

		elem = categoryElem.firstChildElement("text");
		if (!elem.isNull())
		{
			data.untrtext = elem.text().toStdString();
			data.text = tr(data.untrtext.c_str());
		}

		AddCatagory(data);

		categoryElem = categoryElem.nextSiblingElement();
	}
	return true;
}

bool AnnotationDataHelper::LoadAnnotation(QString path)
{
    QDomDocument doc;

    if (!XmlOperation::OpenXmlFile(doc, path)) 
        return false;

    auto rootElem = doc.documentElement();
    auto captionArrayElem = rootElem.firstChildElement("CaptionArray");
    if (captionArrayElem.isNull()) 
        return false;
  
    auto captionResouseElem = captionArrayElem.firstChildElement("CaptionResource");

    while (!captionResouseElem.isNull())
    {
        AnnotationData *data = new AnnotationData;

		auto elem = captionResouseElem.firstChildElement("GUID");
		if (!elem.isNull())
			data->guid = elem.text();

		elem = captionResouseElem.firstChildElement("type");
		if (!elem.isNull())
			data->type = elem.text().toInt();

		elem = captionResouseElem.firstChildElement("Category");
		if (!elem.isNull())
			data->catalog = GetCatagoryString(elem.text().toInt());

        elem = captionResouseElem.firstChildElement("Alias");
        if (!elem.isNull())
            data->alias = elem.text();
        elem = captionResouseElem.firstChildElement("ResFile");
        if (!elem.isNull())
            data->resfile = PathOperation::MakeWorkPath("Captions/") + elem.text();
        elem = captionResouseElem.firstChildElement("BackImage");
        if (!elem.isNull())
            data->thumbnailfile = PathOperation::MakeWorkPath("Captions/") + elem.text();
        elem = captionResouseElem.firstChildElement("CanEditText");
        if (!elem.isNull())
            data->displayName = elem.text().toInt() == 0 ? data->alias : "Text";

        m_datas.push_back(data);
        captionResouseElem = captionResouseElem.nextSiblingElement();
    }


    return true;
}

bool AnnotationDataHelper::LoadOverlay(QString path)
{
    QDomDocument doc;
    if (!XmlOperation::OpenXmlFile(doc, path))
        return false;

    auto rootElem = doc.documentElement();
    auto overlaysElem = rootElem.firstChildElement("OverlayArray");
    if (overlaysElem.isNull()) 
        return false;

    auto overlayElem = overlaysElem.firstChildElement("Overlay");
    while (!overlayElem.isNull())
    {
        AnnotationData *data = new AnnotationData;

		auto elem = overlayElem.firstChildElement("GUID");
		if (!elem.isNull())
			data->guid = elem.text();

		elem = overlayElem.firstChildElement("type");
		if (!elem.isNull())
			data->type = elem.text().toInt();

		elem = overlayElem.firstChildElement("Category");
		if (!elem.isNull())
			data->catalog = GetCatagoryString(elem.text().toInt());

        elem = overlayElem.firstChildElement("Alias");
        if (!elem.isNull())
            data->alias = elem.text();
        elem = overlayElem.firstChildElement("ResFile");
        if (!elem.isNull())
            data->resfile = PathOperation::MakeWorkPath("Captions/") + elem.text();
        elem = overlayElem.firstChildElement("BackImage");
        if (!elem.isNull())
            data->thumbnailfile = PathOperation::MakeWorkPath("Captions/") + elem.text();

		
        m_datas.push_back(data);
        overlayElem = overlayElem.nextSiblingElement();
    }

    return true;
}

bool AnnotationDataHelper::LoadElement(QString path)
{
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, path))
		return false;

	auto rootElem = doc.documentElement();
	auto overlaysElem = rootElem.firstChildElement("ElementArray");
	if (overlaysElem.isNull())
		return false;

	auto overlayElem = overlaysElem.firstChildElement("Element");
	while (!overlayElem.isNull())
	{
		AnnotationData *data = new AnnotationData;

		auto elem = overlayElem.firstChildElement("GUID");
		if (!elem.isNull())
			data->guid = elem.text();

		elem = overlayElem.firstChildElement("type");
		if (!elem.isNull())
			data->type = elem.text().toInt();

		elem = overlayElem.firstChildElement("Category");
		if (!elem.isNull())
			data->catalog = GetCatagoryString(elem.text().toInt());

		elem = overlayElem.firstChildElement("Alias");
		if (!elem.isNull())
			data->alias = elem.text();
		elem = overlayElem.firstChildElement("ResFile");
		if (!elem.isNull())
			data->resfile = PathOperation::MakeWorkPath("Captions/") + elem.text();

		elem = overlayElem.firstChildElement("BackImage");
		if (!elem.isNull())
			data->thumbnailfile = PathOperation::MakeWorkPath("Captions/") + elem.text();

		m_datas.push_back(data);
		overlayElem = overlayElem.nextSiblingElement();
	}

	return true;
}

bool AnnotationDataHelper::LoadItemData(QString catalog, AnnotationData *data)
{
    if (!QFile::exists(data->resfile))
        return false;

    auto *pData = new AnnotationItemData(data, this);

    pData->SetTitle(data->alias);
    pData->setProperty("QByteArray", data->alias.toStdString().c_str());
    pData->SetThumbnail(QFile::exists(data->thumbnailfile) ? QPixmap(data->thumbnailfile) : QPixmap());
    pData->SetSourceFile(data->resfile);

    m_items.push_back(pData);

    return true;
}

void AnnotationDataHelper::AddCatagory(CategoryData catalog)
{
	for (auto c : m_catalogs)
		if (c.type == catalog.type)
			return;
    m_catalogs.push_back(catalog);
}

QString AnnotationDataHelper::GetCatagoryString(int type)
{
	for (auto c : m_catalogs)
	{
		if (c.type == type)
		{
			return c.text;
		}
	}
	return "";
}

void AnnotationDataHelper::LoadFavorite()
{
	QString path = FSGetFilmoraScreenDirectory() + "/AnnotationsFavorite.xml";
    QDomDocument doc("AnnotationsFavorite");
    if (!XmlOperation::OpenXmlFile(doc, path))
        return ;

    auto rootElem = doc.documentElement();
    auto favoriteElem = rootElem.firstChildElement("GUID");
    QStringList favorites;
    while (!favoriteElem.isNull())
    {
        favorites.push_back(favoriteElem.text());
        favoriteElem = favoriteElem.nextSiblingElement();
    }


    for (auto temp : favorites)
    {
        for (auto item : m_items)
        {
            if (temp == item->GetData()->guid)
            {
                m_favorites.push_back(item);
                break;
            }
        }
    }

}

bool AnnotationDataHelper::Load()
{
	LoadCategorys(PathOperation::MakeWorkPath("Captions/Category.xml"));
    LoadAnnotation(PathOperation::MakeWorkPath("Captions/Annotations.xml"));
    LoadOverlay(PathOperation::MakeWorkPath("Captions/Overlays.xml"));
	LoadElement(PathOperation::MakeWorkPath("Captions/Elements.xml"));

    for (auto data : m_datas)
    {
        LoadItemData(data->catalog, data);    
    }

     LoadFavorite();

    return true;

}

bool AnnotationDataHelper::Save()
{
	QString path = FSGetFilmoraScreenDirectory() + "/AnnotationsFavorite.xml";
    if (!QFile::exists(path))
    {
        XmlOperation::CreateXmlFile(path, "AnnotationsFavorite");
    }
    

    QDomDocument doc;
    auto annotationsFavoriteElem = doc.createElement("AnnotationsFavorite");
    doc.appendChild(annotationsFavoriteElem);
    for (auto temp : m_favorites)
    {
        auto elem = doc.createElement("GUID");
        auto textNode = doc.createTextNode(temp->GetData()->guid);
        elem.appendChild(textNode);
        annotationsFavoriteElem.appendChild(elem);
    }

    XmlOperation::SaveXmlContent2File(doc, path);

    return true;
}

void AnnotationDataHelper::RetranslateText()
{
	m_favoriteString = tr("Favorite");
	for (int i = 0; i < m_catalogs.size(); ++i)
		m_catalogs[i].text = tr(m_catalogs[i].untrtext.c_str());
}

