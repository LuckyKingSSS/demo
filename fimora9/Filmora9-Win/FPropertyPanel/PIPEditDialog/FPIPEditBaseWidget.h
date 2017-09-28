#ifndef FPIPEDITBASEWIDGET_H
#define FPIPEDITBASEWIDGET_H

#include "FPropertyCommon.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "NLEType_c.h"

class FPropertySlider;
class MediaPlayerView;
class IMediaPlayerControl;
class FNLETimeline;
class ColorSelecterComboBox;
class MaskWidgetItem;

class MaskPanel : public FBaseWidget
{
	Q_OBJECT

public:
	MaskPanel(QWidget *parent = 0);
	~MaskPanel();

	void SetMaskThumbFile(QStringList);

signals:
	void sigItemDoubleClicked(int);
	void sigWidthChanged(float);
	void sigHeightChanged(float);
	void sigLeftChanged(float);
	void sigTopChanged(float);

protected:
	void resizeEvent(QResizeEvent *);

private:
	void Init();
	void ConnectSignals();
	void slotItemClicked(int);
	void slotItemDoubleClicked(QListWidgetItem*);

private:
	int m_CurrentMaskId;
	QList<MaskWidgetItem*> m_lstItems;
	FListWidget *m_pListWidget;
	FPropertySlider *m_pWidth;
	FPropertySlider *m_pHeight;
	FPropertySlider *m_pLeft;
	FPropertySlider *m_pTop;
};

class MaskWidgetItem : public FBaseWidget
{
	Q_OBJECT

public:
	enum State{Selected, UnSelected};

public:
	MaskWidgetItem(int id = 0, QString ImagePath = "", QWidget *parent = nullptr);
	~MaskWidgetItem();

	int GetID();
	void Select();
	void UnSelect();

signals:
	void sigItemClicked(int);

protected:
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;

private:
	int m_Id;
	State m_State;
	QString m_BGImagePath;
};

class FDirectionWidget : public FBaseWidget
{
	Q_OBJECT

public:
	enum State{UnSelected = 0, Selected = 1};

public:
	FDirectionWidget(int id = 0, QString imagepath = "", QWidget *parent = 0);  // pathÊÇÍ¼Æ¬Â·¾¶
	~FDirectionWidget();

	void SetState(State);
	void ClickButton();
	int GetId();

signals:
	void Clicked(int);
	void StateChanged(State);

protected:
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void enterEvent(QEvent *) override;

private:
	int m_Id;
	State m_State;
	QString m_ImagePath;
};

class FDirectionPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FDirectionPanel(QWidget *parent = 0);
	~FDirectionPanel();
	
	void SetDirection(DIRECTION);
	DIRECTION GetDirection();

signals:
	void sigDirectionChanged(DIRECTION);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void slotDirectionChanged(DIRECTION);

private:
	DIRECTION m_CurrentDirection;
	QList<FDirectionWidget*> m_lstBtns;
};

class FChromaKeyPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FChromaKeyPanel(QWidget *parent = 0);
	~FChromaKeyPanel();
	
signals:
	void sigIntensityLevelChanged(int);
	
protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *) override;

private:
	void slotStateChanged(int);

private:
	FCheckBox *m_pCheckBox;
	FLabel *m_pColorLbl;
	FLabel *m_pColorShowLbl;
	FLabel *m_pIntensityLbl;
	FPropertySlider *m_pSlider;
	FLabel *m_pInfoLbl;
};

class FShadowPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FShadowPanel(QWidget *parent = 0);
	~FShadowPanel();

signals:
	void sigDistanceChanged(int);
	void sigBlurChanged(int);
	void sigTransparencyChanged(int);
	void sigShadowColorChanged(QColor);
	void sigShadowDirectionChanged(DIRECTION);

protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *) override;

private:
	void ConnectSignals();
	void slotStateChanged(int);

private:
	FCheckBox *m_pCheckBox;
	FPropertySlider *m_pDistance;
	FPropertySlider *m_pBlur;
	FPropertySlider *m_pTransparency;
	FLabel *m_pShadowColorLbl;
	ColorSelecterComboBox *m_pColorSelecter;
	FLabel *m_pShadowDirectionLbl;
	FDirectionPanel *m_pShadowDirection;
};

class FBorderPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FBorderPanel(QWidget *parent = 0);
	~FBorderPanel();

signals:
	void sigDistanceChanged(int);
	void sigBlurChanged(int);
	void sigTransparencyChanged(int);
	void sigGradientColorStartChanged(QColor);
	void sigGradientColorEndChanged(QColor);
	void sigGradientDirectionChanged(DIRECTION);

protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *) override;

private:
	void ConnectSignals();
	void slotStateChanged(int);

private:
	FCheckBox *m_pCheckBox;
	FPropertySlider *m_pDistance;
	FPropertySlider *m_pBlur;
	FPropertySlider *m_pTransparency;
	FLabel *m_pGradientColorLbl;
	FLabel *m_pStartLbl;
	ColorSelecterComboBox *m_pStartColorSelecter;
	FLabel *m_pEndLbl;
	ColorSelecterComboBox *m_pEndColorSelecter;
	FLabel *m_pGradientDirectionLbl;
	FDirectionPanel *m_pGradientDirection;
};

class FClickShowWidget : public FBaseWidget
{
	Q_OBJECT
	
public:
	enum State{Opened_State, Closed_State};

public:
	FClickShowWidget(int id = 0, QWidget *parent = 0);
	~FClickShowWidget();
	
	void SetShowWidget(QWidget*);
	QWidget* GetShowWidget();
	void SetTextAlignment(Qt::Alignment);
	void SetText(QString);
	bool Opened();
	void SetState(State);
	void SetTitleHeight(int);
	void SetBottomHeight(int);
	int Height();

signals:
	void Clicked(int);

protected:
	void resizeEvent(QResizeEvent *) override;
	void paintEvent(QPaintEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;

private:
	int m_id;
	State m_state;
	int m_titleHeight;
	int m_bottomHeight;
	FLabel *m_pTextLbl;
	QWidget *m_pShowWidget;
	QStackedWidget *m_pStackedWidget;
};

class EffectPanel : public FBaseWidget
{
	Q_OBJECT

public:
	EffectPanel(QWidget *parent = 0);
	~EffectPanel();
	void OpenItem(int);

signals:
	// Chroma
	void sigIntensityLevelChanged(int);
	// Shadow
	void sigShadowDistanceChanged(int);
	void sigShadowBlurChanged(int);
	void sigShadowTransparencyChanged(int);
	void sigShadowColorChanged(QColor);
	void sigShadowDirectionChanged(DIRECTION);
	// Border
	void sigBorderDistanceChanged(int);
	void sigBorderBlurChanged(int);
	void sigBorderTransparencyChanged(int);
	void sigBorderGradientColorStartChanged(QColor);
	void sigBorderGradientColorEndChanged(QColor);
	void sigBorderGradientDirectionChanged(DIRECTION);
	// Flip
	void sigRotateChanged(FPropertyRotate::RotateMode);

protected:
	void resizeEvent(QResizeEvent *);

private:
	void ConnectSignals();

private:
	int m_currentId;
	QList<FClickShowWidget*> m_lstItems;
	FChromaKeyPanel *m_pChromaKeyPanel;
	FShadowPanel *m_pShadowPanel;
	FBorderPanel *m_pBorderPanel;
	FPropertyRotate *m_pRotatePanel;
};

class FPIPEditSettingPanel : public FBaseWidget
{
	Q_OBJECT

public:
	FPIPEditSettingPanel(QWidget *parent = 0);
	~FPIPEditSettingPanel();
	
	void SetMaskThumbFile(QStringList);

signals:
	// Mask
	void sigMaskItemDoubleClicked(int);
	void sigMaskWidthChanged(int);
	void sigMaskHeightChanged(int);
	void sigMaskLeftChanged(int);
	void sigMaskTopChanged(int);

	/*Effect*/
	// Chroma
	void sigIntensityLevelChanged(int);
	// Shadow
	void sigShadowDistanceChanged(int);
	void sigShadowBlurChanged(int);
	void sigShadowTransparencyChanged(int);
	void sigShadowColorChanged(QColor);
	void sigShadowDirectionChanged(DIRECTION);
	// Border
	void sigBorderDistanceChanged(int);
	void sigBorderBlurChanged(int);
	void sigBorderTransparencyChanged(int);
	void sigBorderGradientColorStartChanged(QColor);
	void sigBorderGradientColorEndChanged(QColor);
	void sigBorderGradientDirectionChanged(DIRECTION);
	// Flip
	void sigRotateChanged(FPropertyRotate::RotateMode);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void ConnectSignals();

private:
	FButton *m_pMaskBtn;
	FButton *m_pEffectBtn;
	MaskPanel *m_pMaskPanel;
	EffectPanel *m_pEffectPanel;
	QStackedWidget *m_pStackedWidget;
};

class FPROPERTYPANEL_EXPORT FPIPEditPlayer : public FBaseWidget
{
	Q_OBJECT

public:
	FPIPEditPlayer(QWidget *parent = 0);
	~FPIPEditPlayer();
	bool Open(int bSource, FNLETimeline *timeline);
	void Play();
	bool IsPlaying();
	void Pause();
	void Stop();
	void Update();

signals:
	void sigOk();
	void sigCancel();

protected:
	void resizeEvent(QResizeEvent *) override;
	void RetranslateUi() override;
	
private:
	void ConnectSignals();

private:
	bool m_bPlaying;
	FNLETimeline *m_pTimeline;
	MediaPlayerView *m_pPlayerView;
	IMediaPlayerControl *m_pPlayerCtrl;
	ToolBarSlider *m_pDurationSlider;
	FStateButton *m_pPlayBtn;
	FImageButton *m_pStopBtn;
	FButton *m_pOkBtn;
	FButton *m_pCancelBtn;
};

#endif  // FPIPEDITBASEWIDGET_H
