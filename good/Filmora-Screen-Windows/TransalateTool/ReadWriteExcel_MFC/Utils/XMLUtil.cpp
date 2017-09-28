#include "stdafx.h"
#include "XMLUtil.h"
#include "FileFinderHelper.h"

using namespace tinyxml2;
#pragma comment(lib,"tinyxml2.lib")

CXMLUtil::CXMLUtil()
{
}


CXMLUtil::~CXMLUtil()
{
	delete[]m_xmlPath;
}

bool CXMLUtil::GetNodePointerByName(tinyxml2::XMLElement* pRootEle, std::string &strNodeName, tinyxml2::XMLElement* &Node)
{
	// 假如等于根节点名，就退出   
	if (strNodeName == pRootEle->Name())
	{
		Node = pRootEle;
		return true;
	}
	XMLElement* pEle = pRootEle;
	for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		//递归处理子节点，获取节点指针   
		if (GetNodePointerByName(pEle, strNodeName, Node))
			return true;
	}
	return false;
}

bool CXMLUtil::GetNodePointerByName(std::string &strNodeName, tinyxml2::XMLElement* &Node)
{

	if (m_RootElement == NULL)
	{
		return false;
	}
	if (strNodeName == m_RootElement->Name())
	{
		Node = m_RootElement;
		return true;
	}
	XMLElement* pEle = m_RootElement;
	for (pEle = m_RootElement->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		//递归处理子节点，获取节点指针   
		if (GetNodePointerByName(pEle, strNodeName, Node))
			return true;
	}
	return false;
}

bool CXMLUtil::TravelXMLTree(std::string strXmlPath, pFun func)
{
	bool bRec = true;
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = doc->LoadFile(strXmlPath.c_str());
	if (XML_SUCCESS != error)
	{
		return false;
	}
	XMLElement* ele = doc->RootElement();
	if (ele != NULL)
	{
		XMLNode* firstEle = ele->FirstChild();
		while (firstEle != NULL)
		{

			firstEle = firstEle->NextSibling();
		}
		ele = ele->NextSiblingElement("context");
	}
	return bRec;
}

bool CXMLUtil::CreateXML(const char* xmlPath)
{
	if (CFileFinderHelper::IsFileExsit(xmlPath))
	{
		XMLError loadState = m_doc.LoadFile(xmlPath);
		if (loadState != XML_SUCCESS)
		{
			return false;
		}
		m_RootElement = m_doc.RootElement();
	}
	else
	{
		const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
		m_doc.Parse(declaration);//会覆盖xml所有内容
		//添加申明可以使用如下两行
		m_RootNode = m_doc.NewElement("root");
		m_doc.InsertEndChild(m_RootNode);
		XMLError createState = m_doc.SaveFile(xmlPath);
		if (XML_SUCCESS != createState)
		{
			//创建失败
			return false;
		}
	}
	m_RootElement = m_doc.RootElement();
	m_xmlPath = new char[MAX_PATH];
	memset((void*)m_xmlPath, 0, MAX_PATH);
	memcpy_s((void*)m_xmlPath, MAX_PATH, xmlPath, strlen(xmlPath));
	//m_xmlPath = xmlPath;
	return true;
}

XMLNode* CXMLUtil::InsertNode(const char* elementName)
{
	XMLElement* element = m_doc.FirstChildElement(elementName);
	if (element != nullptr)
	{
		//删除该节点
		m_doc.DeleteChild(element);
	}
	XMLElement* newNode = m_doc.NewElement(elementName);
	XMLNode* node = m_doc.InsertEndChild(newNode);
	m_doc.SaveFile(m_xmlPath);
	return node;
}

XMLNode* CXMLUtil::InsertRightNode(tinyxml2::XMLElement* parentElement, const char* elementName)
{
	XMLElement* element = parentElement->FirstChildElement(elementName);
	if (element != nullptr)
	{
		//删除该节点
		parentElement->DeleteChild(element);
	}
	XMLElement* newNode = m_doc.NewElement(elementName);
	XMLNode* node = parentElement->InsertEndChild(newNode);
	m_doc.SaveFile(m_xmlPath);
	return node;
}

tinyxml2::XMLNode* CXMLUtil::InsertLeftNode(tinyxml2::XMLElement* parentElement, const char* elementName)
{
	XMLElement* element = parentElement->FirstChildElement(elementName);
	if (element != nullptr)
	{
		//删除该节点
		parentElement->DeleteChild(element);
	}
	XMLElement* newNode = m_doc.NewElement(elementName);
	XMLNode* node = parentElement->InsertFirstChild(newNode);
	m_doc.SaveFile(m_xmlPath);
	return node;
}

std::string CXMLUtil::QueryNodeInfo(const char* nodeName, const char* attributeName)
{
	XMLElement* nodeElement = m_RootElement->FirstChildElement(nodeName);
	const char* value = nodeElement->Attribute(attributeName);
	std::string strAttribute(value);
	return strAttribute;
};

bool CXMLUtil::DeleteNodeInfo(const char* nodeName)
{
	XMLElement* elementNode = m_RootElement->FirstChildElement(nodeName);
	if (elementNode != nullptr)
	{
		m_RootElement->DeleteChild(elementNode);
		return true;
	}
	return false;
}

bool CXMLUtil::DeleteNodeInfo(tinyxml2::XMLElement* parentElement, const char* nodeName)
{
	XMLElement* elementNode = parentElement->FirstChildElement(nodeName);
	if (elementNode != nullptr)
	{
		parentElement->DeleteChild(elementNode);
		return true;
	}
	return false;
}

void CXMLUtil::SaveFile(const char* strFilepath)
{
	m_doc.SaveFile((strFilepath == "") ? m_xmlPath : strFilepath);
}

