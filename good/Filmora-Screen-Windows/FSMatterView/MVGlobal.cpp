#include "stdafx.h"
#include "MVGlobal.h"

MVGlobal::MVGlobal(QObject *parent)
	: QObject(parent)
{

}


MVGlobal* MVGlobal::GetInstance()
{
	static MVGlobal ins;
	return &ins;
}


void MVGlobal::ImportMediaItems(QStringList files)
{
	emit sigImportMediaItems(files);
}
