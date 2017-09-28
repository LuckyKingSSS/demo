#ifndef FEXPORTLOCALPANEL_H
#define FEXPORTLOCALPANEL_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"
class FTypeSettingWnd;
class FExportSettingWnd;
class FExportLocalPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FExportLocalPanel(QWidget *parent);
	~FExportLocalPanel();
	void initUI();
	void initConnections();

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	FTypeSettingWnd* m_pStyleSettingWnd;
	FExportSettingWnd* m_pExportSettingWnd;
};

#endif // FEXPORTLOCALPANEL_H
