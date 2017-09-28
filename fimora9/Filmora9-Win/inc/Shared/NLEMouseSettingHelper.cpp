#include "stdafx.h"

#include "NLEMouseSettingHelper.h"
#include "NLECommon.h"

#include "tinyxml2.cpp"

static const CHAR *TAG_mouseInfos = "mouseInfos";
static const CHAR *TAG_SegmentMouseInfo = "SegmentMouseInfo";
static const CHAR *TAG_range = "range";
static const CHAR *TAG_Range_Start = "range_start";
static const CHAR *TAG_Range_End = "range_end";
static const CHAR *TAG_iconDirectory = "iconDirectory";
static const CHAR *TAG_mouseImgPath = "mouseImgPath";
static const CHAR *TAG_mouseImgHotspot = "mouseImgHotspot";
static const CHAR *TAG_mouseImgHotspot_X = "mouseImgHotspot_X";
static const CHAR *TAG_mouseImgHotspot_Y = "mouseImgHotspot_Y";
static const CHAR *TAG_clickedEffectImgPath = "clickedEffectImgPath";
static const CHAR *TAG_doubleClickedEffectImgPath = "doubleClickedEffectImgPath";
static const CHAR *TAG_rightClickedEffectImgPath = "rightClickedEffectImgPath";
static const CHAR *TAG_dragEffectImgPath = "dragEffectImgPath";

CNLEMouseSettingHelper::CNLEMouseSettingHelper()
{

}

CNLEMouseSettingHelper::~CNLEMouseSettingHelper()
{

}

std::wstring CNLEMouseSettingHelper::GetPropertyValue()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* mouseInfosElement = doc.NewElement(TAG_mouseInfos);
	doc.InsertFirstChild(mouseInfosElement);

	std::wstring str;
	for (std::vector<CNLESegmentMouseInfo>::iterator it = m_segmentMouseInfoVec.begin();
		it != m_segmentMouseInfoVec.end(); it++)
	{
		CNLESegmentMouseInfo info = (*it);
		
		tinyxml2::XMLElement* segmentInfoElement = doc.NewElement(TAG_SegmentMouseInfo);		
		mouseInfosElement->InsertEndChild(segmentInfoElement);

		//range
		tinyxml2::XMLElement* element = doc.NewElement(TAG_range);
		segmentInfoElement->InsertEndChild(element);
		element->SetAttribute(TAG_Range_Start, info.range.Start);
		element->SetAttribute(TAG_Range_End, info.range.End);

		//iconDirectory
		element = doc.NewElement(TAG_iconDirectory);
		char text[MAX_PATH] = { 0 };
		NLECommon::WChar2Char(info.iconDirectory.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);

		//mouseImgPath
		element = doc.NewElement(TAG_mouseImgPath);
		NLECommon::WChar2Char(info.mouseImgPath.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);

		//mouseImgHotspot
		element = doc.NewElement(TAG_mouseImgHotspot);
		segmentInfoElement->InsertEndChild(element);
		element->SetAttribute(TAG_mouseImgHotspot_X, info.mouseImgHotspot.X);
		element->SetAttribute(TAG_mouseImgHotspot_Y, info.mouseImgHotspot.Y);

		//clickedEffectImgPath
		element = doc.NewElement(TAG_clickedEffectImgPath);
		NLECommon::WChar2Char(info.clickedEffectImgPath.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);

		//doubleClickedEffectImgPath
		element = doc.NewElement(TAG_doubleClickedEffectImgPath);
		NLECommon::WChar2Char(info.doubleClickedEffectImgPath.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);

		//rightClickedEffectImgPath
		element = doc.NewElement(TAG_rightClickedEffectImgPath);
		NLECommon::WChar2Char(info.rightClickedEffectImgPath.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);

		//dragEffectImgPath
		element = doc.NewElement(TAG_dragEffectImgPath);
		NLECommon::WChar2Char(info.dragEffectImgPath.c_str(), text, MAX_PATH);
		element->SetText(text);
		segmentInfoElement->InsertEndChild(element);
	}

	tinyxml2::XMLPrinter streamer;
	doc.Print(&streamer);

	std::string xmlStr = streamer.CStr();
	int length = xmlStr.length();
	WCHAR *pBuf = new WCHAR[length + MAX_PATH];
	memset(pBuf, 0, sizeof(WCHAR)* (length + MAX_PATH));
	NLECommon::Char2WChar(xmlStr.c_str(), pBuf, length);
	str = pBuf;
	delete pBuf;

	return str;
}

HRESULT CNLEMouseSettingHelper::SetPropertyValue(std::wstring value)
{
	int length = value.length();
	CHAR *pBuf = new CHAR[length + MAX_PATH];
	memset(pBuf, 0, sizeof(CHAR)* (length + MAX_PATH));
	NLECommon::WChar2Char(value.c_str(), pBuf, length);
	HRESULT hr = S_OK;
	do 
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError err = doc.Parse(pBuf);
		if (tinyxml2::XML_NO_ERROR != err)
		{
			hr = E_FAIL;
			break;
		}

		m_segmentMouseInfoVec.clear();

		tinyxml2::XMLElement *pRootElement = doc.FirstChildElement(TAG_mouseInfos);
		if (NULL == pRootElement)
		{
			hr = E_FAIL;
			break;
		}

		for (tinyxml2::XMLElement *pInfoElement = pRootElement->FirstChildElement(TAG_SegmentMouseInfo); 
			pInfoElement;
			pInfoElement = pInfoElement->NextSiblingElement())
		{
			CNLESegmentMouseInfo info;

			for (tinyxml2::XMLElement *pElement = pInfoElement->FirstChildElement();
				pElement;
				pElement = pElement->NextSiblingElement())
			{
				WCHAR wcsBuf[MAX_PATH] = { 0 };
				if (0 == strcmp(pElement->Name(), TAG_range))
				{
					info.range.Start = pElement->IntAttribute(TAG_Range_Start);
					info.range.End = pElement->IntAttribute(TAG_Range_End);
				}
				else if (0 == strcmp(pElement->Name(), TAG_mouseImgHotspot))
				{
					info.mouseImgHotspot.X = pElement->IntAttribute(TAG_mouseImgHotspot_X);
					info.mouseImgHotspot.Y = pElement->IntAttribute(TAG_mouseImgHotspot_Y);
				}
				else if (0 == strcmp(pElement->Name(), TAG_iconDirectory))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.iconDirectory = wcsBuf;
				}
				else if (0 == strcmp(pElement->Name(), TAG_mouseImgPath))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.mouseImgPath = wcsBuf;
				}
				else if (0 == strcmp(pElement->Name(), TAG_clickedEffectImgPath))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.clickedEffectImgPath = wcsBuf;
				}
				else if (0 == strcmp(pElement->Name(), TAG_doubleClickedEffectImgPath))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.doubleClickedEffectImgPath = wcsBuf;
				}
				else if (0 == strcmp(pElement->Name(), TAG_rightClickedEffectImgPath))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.rightClickedEffectImgPath = wcsBuf;
				}
				else if (0 == strcmp(pElement->Name(), TAG_dragEffectImgPath))
				{
					const char *value = pElement->GetText();
					NLECommon::Char2WChar(value, wcsBuf, MAX_PATH);
					info.dragEffectImgPath = wcsBuf;
				}
			}
			m_segmentMouseInfoVec.push_back(info);
		}
	} while (FALSE);

	delete pBuf;

	return hr;
}

int CNLEMouseSettingHelper::SegmentCount()
{
	return m_segmentMouseInfoVec.size();
}

HRESULT CNLEMouseSettingHelper::AddSegmentMouseInfo(CNLESegmentMouseInfo info)
{
	HRESULT hr = S_OK;

	m_segmentMouseInfoVec.push_back(info);

	return hr;
}

HRESULT CNLEMouseSettingHelper::GetSegmentMouseInfo(int index, CNLESegmentMouseInfo *pInfo)
{
	if (index < 0 || index >= m_segmentMouseInfoVec.size() || NULL == pInfo)
		return E_INVALIDARG;

	*pInfo = m_segmentMouseInfoVec[index];

	return S_OK;
}

HRESULT CNLEMouseSettingHelper::RemoveSegmentMouseInfo(int index)
{
	if (index < 0 || index >= m_segmentMouseInfoVec.size())
		return E_INVALIDARG;

	std::vector<CNLESegmentMouseInfo>::iterator it = m_segmentMouseInfoVec.begin() + index;
	m_segmentMouseInfoVec.erase(it);
	
	return S_OK;
}

HRESULT CNLEMouseSettingHelper::Clear()
{
	m_segmentMouseInfoVec.clear();

	return S_OK;
}
