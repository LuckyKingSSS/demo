#include "stdafx.h"
#include "FPresentWndMgr.h"

FPresentWndMgr::FPresentWndMgr(QObject *parent)
	: QObject(parent)
{

}

FPresentWndMgr::~FPresentWndMgr()
{

}

QStringList FPresentWndMgr::GetTotalColors()
{
	QStringList strList;// = { "Dark Film", "Cool", "Warm", "Coolmax", "Boost Color", "Shadow Details", "Elegant", "Brighten", "Dark Film", "Cool", "Warm", "Coolmax", "Boost Color", "Shadow Details", "Elegant", "Brighten" };
	strList << "Dark Film" << "Cool" << "Warm" << "Coolmax" << "Boost Color" << "Shadow Details" << "Elegant" << "Brighten" << "Dark Film" << "Cool" << "Warm" << "Coolmax" << "Boost Color" << "Shadow Details" << "Elegant" << "Brighten";
	return strList;
}
