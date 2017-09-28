#ifndef FOVERLAYPANEL_H
#define FOVERLAYPANEL_H

#include  "FPropertyPanel.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FPropertySlider;
class BottomButtonBar;
class FPROPERTYPANEL_EXPORT FOverlayPanel : public FPropertyPanel
{
	Q_OBJECT

public:
	FOverlayPanel(QWidget *parent);
	~FOverlayPanel();
	// 窗口初始化
	virtual void Init();
	// 窗口将显示
	virtual void Show();
	// 窗口将隐藏
	virtual void Hide();
	// 窗口enable操作
	virtual void SetEnabled(bool enabled);
	// 窗口将销毁
	virtual void Destory();
	// 窗口更新数据
	virtual void Update() override;

public Q_SLOTS:
	void slotAlphaValueChanged(int iValue);

Q_SIGNALS:
	void sigClickOkBtn();

private:
	void initUI();
private:
	FPropertySlider* m_AlphaSlider;
	BottomButtonBar* m_BottomBar;
};

#endif // FOVERLAYPANEL_H
