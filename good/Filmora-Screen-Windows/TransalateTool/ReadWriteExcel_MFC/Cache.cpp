#include "stdafx.h"
#include "Cache.h"
#include "Utils\StringUtils.h"
#include "Utils\XMLUtil.h"



CCache::CCache(wstring strCachePath)
{
	m_XmlDoc = new CXMLUtil();
	LoadXmlData(strCachePath);
}

CCache::~CCache()
{
}

bool CCache::SaveTranslateCache(string strExcelPath, string strProjectPath, string strFilter, bool bIsForceReplace)
{
	//获取Root节点
	LoadXmlData(m_CachePath);
	tinyxml2::XMLElement* rootNode = NULL;
	m_XmlDoc->GetNodePointerByName(string("root"), rootNode);
	tinyxml2::XMLElement* elemnt = (tinyxml2::XMLElement*)m_XmlDoc->InsertLeftNode(rootNode,"Translate");
	strExcelPath = CStringUtils::trim(strExcelPath);
	strExcelPath = CStringUtils::string_To_UTF8(strExcelPath);
	strProjectPath = CStringUtils::trim(strProjectPath);
	strProjectPath = CStringUtils::string_To_UTF8(strProjectPath);
	strFilter = CStringUtils::trim(strFilter);
	strFilter = CStringUtils::string_To_UTF8(strFilter);
	elemnt->SetAttribute("ExcelPath", strExcelPath.c_str());
	elemnt->SetAttribute("ProjectPath", strProjectPath.c_str());
	elemnt->SetAttribute("FilterValue", strFilter.c_str());
	elemnt->SetAttribute("IsForceReplace", (bIsForceReplace) ? "true" : "false");
	m_XmlDoc->SaveFile();
	return true;
}

pTranslateCacheInfo CCache::GetTranslateCache()
{
	pTranslateCacheInfo info = new TraslateCacheInfo();
	tinyxml2::XMLElement* Node = NULL;
	string strNodeNmae("Translate");
	if (m_XmlDoc != nullptr)
	{
		m_XmlDoc->GetNodePointerByName(strNodeNmae, Node);
		if (Node)
		{
			info->strExcelPath = Node->Attribute("ExcelPath");
			info->strProjectPath = Node->Attribute("ProjectPath");
			info->strFilter = Node->Attribute("FilterValue");
			info->strIsForceReplace = Node->Attribute("IsForceReplace");
		}	
	}
	return info;
}

pExtactCacheInfo CCache::GetExtractCache()
{
	pExtactCacheInfo info = new ExtactCacheInfo();
	tinyxml2::XMLElement* Node = NULL;
	string strNodeNmae("Extract");
	if (m_XmlDoc != nullptr)
	{
		m_XmlDoc->GetNodePointerByName(strNodeNmae, Node);
		if (Node)
		{
			info->strResultPath = Node->Attribute("ExcelPath");
			info->strProjectPath = Node->Attribute("ProjectPath");
			info->strFilter = Node->Attribute("FilterValue");
			info->bIsExtractAll = Node->Attribute("IsExtractAll");
		}
	}
	return info;
}

void CCache::LoadXmlData(wstring wstrPath)
{
	string strPath;
	CStringUtils::WStringToString(wstrPath, strPath);
	strPath = CStringUtils::trim(strPath);
	m_XmlDoc->CreateXML(strPath.c_str());
	m_CachePath = wstrPath;
}

bool CCache::SaveExtractCache(string strResultPath, string strProjectPath, string strFilter, bool bIsExtractAll)
{
	//需要考虑同步文档问题，故重新加载doc
	LoadXmlData(m_CachePath);
	tinyxml2::XMLElement* rootNode = NULL;
	m_XmlDoc->GetNodePointerByName(string("root"), rootNode);
	tinyxml2::XMLElement* elemnt = (tinyxml2::XMLElement*)m_XmlDoc->InsertRightNode(rootNode, "Extract");

	strResultPath = CStringUtils::trim(strResultPath);
	strResultPath = CStringUtils::string_To_UTF8(strResultPath);

	strProjectPath = CStringUtils::trim(strProjectPath);
	strProjectPath = CStringUtils::string_To_UTF8(strProjectPath);

	strFilter = CStringUtils::trim(strFilter);
	strFilter = CStringUtils::string_To_UTF8(strFilter);

	elemnt->SetAttribute("ExcelPath", strResultPath.c_str());
	elemnt->SetAttribute("ProjectPath", strProjectPath.c_str());
	elemnt->SetAttribute("FilterValue", strFilter.c_str());
	elemnt->SetAttribute("IsExtractAll", (bIsExtractAll) ? "true" : "false");
	m_XmlDoc->SaveFile();
	return true;
}
