#pragma once


#include "inc_CommonLib/commonlib_global.h"

#include <QtWidgets/QWidget>
#include <QtCore/QString>


#define MAKE_FOURCC(str) MAKEFOURCC(str[0],str[1],str[2],str[3])

COMMONLIB_EXPORT QWidget* gGetMainWindow();
COMMONLIB_EXPORT void gSetMainWindow(QWidget *w);

COMMONLIB_EXPORT QWidget* gGetMatterView();
COMMONLIB_EXPORT void gSetMatterView(QWidget *w);


COMMONLIB_EXPORT QString gFrame2String(int frame, float fps = 25.0f);
COMMONLIB_EXPORT QString gFrame2StringSecond(int frame, float fps = 25.0f);


COMMONLIB_EXPORT QString gGetStyleSheet(QString path);

COMMONLIB_EXPORT float gGetDevicePixelRatio();

COMMONLIB_EXPORT QString GetConfigfileDir();
COMMONLIB_EXPORT void CreateSettingsFile(const QString &filePath);
COMMONLIB_EXPORT QString GetSettingFilePath();

COMMONLIB_EXPORT QString gGetCommonLibStyles();

COMMONLIB_EXPORT void gSetArgLang(bool bArg = false);
COMMONLIB_EXPORT bool gIsArgLang();

COMMONLIB_EXPORT bool gIsH264Coder(int fourcc);
COMMONLIB_EXPORT QString gMakeFourCCString(int fouccc);

COMMONLIB_EXPORT quint64 gGetDiskFreeSpace(const QString &driver);  //·µ»ØMB



COMMONLIB_EXPORT int gGetRetranslateUiTextLenght(QFont font, QString text, int minWidth, int extraWidth = 0);


