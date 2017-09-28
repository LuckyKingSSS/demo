#ifndef FPROPERTYPANEL_H
#define FPROPERTYPANEL_H


#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FNLETimeline;
class FPROPERTYPANEL_EXPORT FPropertyPanel : public FUpdateBaseWidget
{
    Q_OBJECT
public:
    FPropertyPanel(QWidget *parent);

    FNLETimeline* GetTimeline();
    FNLETimeline* GetRootTimeline();
    
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
    
signals:
    // 通知外部更新数据（时间线，EditPanel等）
    void sigUpdate();
    // 通知EditPanel更新数据
    void sigUpdateEditPanel();
	// OK按钮被点击
	void sigOk();

//private:
public:
    void SetTimeline(FNLETimeline *timeline);
    void SetRootTimeline(FNLETimeline *timeline);

private:
    FNLETimeline *m_timeline;
    FNLETimeline *m_rootTimeline;
};

#endif // FPROPERTYPANEL_H
