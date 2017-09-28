#ifndef FEXPORTSETTINGWND_H
#define FEXPORTSETTINGWND_H

#include "FPathSelectEdit.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "FSExportDataDef.h"
#include "FSExportHelper.h"


class FExportSettingControl;
class FLable;
class FButton;
class FLineEdit;
class FParameterSettingWnd;
class FSExportHelperManager;

class FExportSettingWnd : public FBaseWidget
{
	Q_OBJECT

public:
	FExportSettingWnd(QWidget *parent);
	~FExportSettingWnd();

private:
	void initUI();
	void initConnections();
	void initData();

public Q_SLOTS:
	void slotOnItemSelect(QString strType);
	void slotOnConfirmSetting(QMap<QString, QString> UserSelectParameter);
	void slotOnExport();
	void slotOnExportStateChange(FSExportHelper::FSEXPORT_STATE eState, int nProgress);

private:
	int getVedioBitrate(QString strBitrate);
	float getVedioFramerate(QString strFramerate);
	int  getAudioBitrate(QString strBitrate);
	int  getAudioSampleRate(QString strSamplerate);

protected:
	void paintEvent(QPaintEvent *event)override;

private:
	FLabel*  m_HeadTitle;
	FLabel*  m_ExportName;
	FLineEdit* m_ExportNameEdit;
	FLabel*  m_SavePath;
	FPathSelectEdit* m_PathSelectEdit;
	FLabel* m_ResolutionLabel;
	FLabel* m_ResolutionValue;
	FButton* m_SettingBtn;
	FLabel* m_ExportFileSize;
	FLabel* m_ExportFileSizeValue;
	FLabel* m_ExportFileTime;
	FLabel* m_ExportFileTimeValue;
	FButton* m_ExportBtn;
	FExportSettingControl* m_ExportSettingControl;//配置控制中心
	FSExportHelperManager* m_ExportHelper;
	ExportSettingInfo  m_ExportInfo; //导出的设置参数
	FParameterSettingWnd* m_exportSetting;
	QString m_CurType; //用户当前选中的格式类型
};

#endif // FEXPORTSETTINGWND_H
