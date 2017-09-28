#ifndef FPHOTOPANEL_H
#define FPHOTOPANEL_H

#include "FPropertyPanel.h"
#include "FPropertyCommon.h"
#include "FPhotoPanelMgr.h"
#include "FPropertySlider.h"
#include "FPhotoPanelMgr.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FPropertySpinBox;

class FPROPERTYPANEL_EXPORT FPhotoPanel : public FPropertyPanel
{
	Q_OBJECT

public:
	FPhotoPanel(QWidget *parent = 0);
	~FPhotoPanel();

	// 窗口初始化
	void Init() override;
	// 窗口将显示
	void Show() override;
	// 窗口将隐藏
	void Hide() override;
	// 窗口将销毁
	void Destory() override;
	// 窗口更新数据
	void Update() override;
	// 多语言翻译
	void TranslateText() override;
	void RetranslateUi() override;
	// 窗口enable操作
	void SetEnabled(bool) override;

protected:
	void resizeEvent(QResizeEvent *) override;

private slots:
	void ConnectSignals();
	void Init3DLUTMenu();
	void slotRotateChanged(FPropertyRotate::RotateMode);
	void slotContrastChanged(int);
	void slotSaturationChanged(int);
	void slotBrightnessChanged(int);
	void slotTintChanged(int);
	void slot3DLUTChanged(QAction*);
	void slotDefault3DLUT();
	void slotEnhanceChanged(bool);
	void slotDurationChanged(int);
	void slotDurationApplyToAll();
	void slotReset();
	void slotAdvanced();
	void slotShowMenu();

private:
	FPhotoPanelMgr *m_pPanelMgr;
	FPropertyRotate *m_pRotate;
	FLabel *m_pAdjust;
	FPropertySlider *m_pContrast;
	FPropertySlider *m_pSaturation;
	FPropertySlider *m_pBrightness;
	FPropertySlider *m_pTint;
	FPropertyButton *m_p3DLUT;
	FPropertyCheckBox *m_pEnhance;
	FPropertySpinBox *m_pDuration;
	BottomButtonBar *m_pButtonBar;
	QList<QAction*> m_lstActions;
	FMenu *m_pMenu;
};

class FPropertySpinBox : public FBaseWidget
{
	Q_OBJECT

public:
	FPropertySpinBox(QWidget *parent = 0);
	~FPropertySpinBox();
	void SetPropertyName(QString);
	QString GetPropertyName();
	void SetButtonText(QString);
	int GetValue();
	void SetValue(int);
	FSpinBox *GetSpinBox();

signals:
	void sigValueChanged(int);
	void sigValueChanged(const QString&);
	void sigClicked();

protected:
	void resizeEvent(QResizeEvent *) override;

private slots:
	void slotValueChanged(int);
	void slotValueChanged(const QString&);

private:
	FLabel *m_pPropertyLbl;
	FSpinBox *m_pSpinBox;
	FButton *m_pTextBtn;
};

#endif // FPHOTOPANEL_H



