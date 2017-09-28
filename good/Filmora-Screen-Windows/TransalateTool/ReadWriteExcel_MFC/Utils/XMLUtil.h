#pragma once
#include "stdafx.h"
#include "tinyxml2.h"
#include <string>

typedef void(*pFun)(PVOID pParam);

class CXMLUtil
{
public:
	CXMLUtil();
	~CXMLUtil();

	//获取xml特定节点元素
	static bool GetNodePointerByName(tinyxml2::XMLElement* pRootEle, std::string &strNodeName, tinyxml2::XMLElement* &Node);
	//遍历整个xml表  --采用深度优先遍历
	static bool TravelXMLTree(std::string strXmlPath, pFun func);
    //创建xml  --如果当前xml已经存在则做加载操作
	bool CreateXML(const char* xmlPath);
	//添加新节点(在根节点上添加新的子节点)
	tinyxml2::XMLNode* InsertNode(const char* elementName);
    //在特定的节点添加子节点
	tinyxml2::XMLNode* InsertRightNode(tinyxml2::XMLElement* parentElement, const char* elementName);

	tinyxml2::XMLNode* InsertLeftNode(tinyxml2::XMLElement* parentElement, const char* elementName);

	//查询节点信息-- 根据节点的属性名称查询属性对应的值
	std::string QueryNodeInfo(const char* nodeName,const char* attributeName);
	//修改节点信息
	bool UpdatePathData(const char* xmlPath, const char* newElementValue);
	//删除特定节点的信息（根结点下的子节点）
	bool DeleteNodeInfo(const char* nodeName);
	//删除特定节点的信息（非根结点下的子节点）
	bool DeleteNodeInfo(tinyxml2::XMLElement* parentElement, const char* nodeName);
	//保存对配置文件的修改
	void SaveFile(const char* strFilepath = "");
	//通过节点名称过去对应的元素(--获取一级节点)
	bool GetNodePointerByName(std::string &strNodeName, tinyxml2::XMLElement* &Node);

private:
	tinyxml2::XMLDocument m_doc; //当前加载的xml文档
	tinyxml2::XMLElement* m_RootElement; //文档的根节点元素

	tinyxml2::XMLElement* m_RootNode;//顶级节点
	const char* m_xmlPath;
};

