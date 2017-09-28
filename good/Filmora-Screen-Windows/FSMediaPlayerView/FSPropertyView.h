#ifndef FSPROPERTYVIEW_H
#define FSPROPERTYVIEW_H
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "Include/NLECaptionDefine.h" 

class QDoubleSpinBox;
class ToolBarSlider;
class IFSEditHelper;
class FSTextEdit;

class IDragValueChangedHelper;
class ColorSelecterComboBox;
class AngleDoubleSpinBox :public FSDoubleSpinBox{
    Q_OBJECT
public:
    AngleDoubleSpinBox(QWidget *pParent);
    ~AngleDoubleSpinBox();


protected:
    void keyPressEvent(QKeyEvent *event);
};

class FSCaptionSvgItem :public FSBaseWidget{
    Q_OBJECT

signals:
    void CurrentColorChanged(QColor,int index);
public:
    FSCaptionSvgItem(QWidget *pParent);
    ~FSCaptionSvgItem();

    QColor GetCurrentColor();
    void SetCurrentColor(QColor color);

    bool IsResetable();
    void SetIndex(int index);
    int GetIndex();

    void SetDedaultColor(QColor color);
    QColor GetDedaultColor();

    virtual void RetranslateUi();

private:
    void CreareUI();

    
private:
    ColorSelecterComboBox *m_pColorSelecter;
    FSButton              *m_pButtonColorReset;
    FSLabel               *m_pLabelColor;

    QColor                 m_DefaultColor;
    int                    m_IndexColor;
};

class FSPropertyTitle :public QWidget{
    Q_OBJECT
    enum BtnID{
        ID_TITLE,
        ID_DELETE,
        ID_RESET
    };
signals:
    void Reset();
    void Delete();
    void ShowView(bool bShow);

public:
    FSPropertyTitle(QWidget *pParent);
    ~FSPropertyTitle();

    void SetTitleName(const QString &strName);
    void SetCandelete(bool bCandelete);
    void SetResetable(bool bResetable);
    void SetTitleHeight(int height);

private:
    void CreateUI();
    void ConnectSignals();

    void resizeEvent(QResizeEvent *);

    private slots:
    void ButtonClickedSlot(int id);

private:
    QButtonGroup *m_pButtonGroup;
    QPushButton  *m_pTitleBtn;
    FSButton     *m_pResetBtn;
    FSButton     *m_pDeleteBtn;
    FSLabel      *m_pTitleLabel;

    bool          m_bCandelete;
    bool          m_bEdited;
    bool          m_bShow;
};


class IFSOptionView :public FSBaseWidget{
    Q_OBJECT

signals:
    void ParmaChanged();
    void DeletePropoty();

public:
    enum OptionType{
        TypeVisualBase,
        TypeVideoAudioSpeed,
        TypeAudioBase,
        TypeAudioDenoise,
        TypeAudioSpeed,
        TypeTextBase,
        TypeCaptionSvgOption,
        TypeAnotation,
        TypeTextBorder,
        TypeTextShadow,
        TypeBackground1,
        TypeCursorBase,
        TypeCursorHighlight,
        TypeCursorClickSound,
        TypeCursorClickRing,
        TypeHotKeyTip,
    };
public:
    IFSOptionView(OptionType type, QWidget *pParent = 0);
    ~IFSOptionView();

    OptionType GetOptionType();
    void       SetOptionType(OptionType type);

    void SetTitle(const QString &title);

    void SetCandelete(bool bCandelete);

    virtual void UpdataParma(IFSEditHelper *pHelper){};

protected:
    virtual void OnReset();

    virtual bool IsTitleResetable(){ return true; };
    void showEvent(QShowEvent *event);

private:
    void ConnectSignals();
    void OnShowViewSlot(bool bShow);
    void OnDeleteSlot();


protected:
    QWidget         *m_pOptionWidget;
    FSPropertyTitle *m_pTitleWidget;

private:
    OptionType mType;
    
};


class FSVisualBaseOption :public IFSOptionView
{
    Q_OBJECT
public:
    FSVisualBaseOption(QWidget *pParent = 0);
    ~FSVisualBaseOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    void GetPosition(double & positionX, double &positionY);
    void GetRotation(double & rotation);
    void GetScale(double &x, double &y);

    void ResetChangeFlag();
    bool isPosChanged();
    bool isScaleChanged();
    bool isAngleChanged();

protected:
    virtual void OnReset();
    virtual bool IsTitleResetable();
    bool UpdateScale(double x, double y);
    bool UpdatePosition(double x, double y);
    bool UpdateRotation(double rotation);

    

protected:
    void CreateUI();
    void RetranslateUi();

    void ConnectScaleValueChanged(bool bConnect);
    void ConnectPositionXValueChanged(bool bConnect);
    void ConnectPositionYValueChanged(bool bConnect);
    void ConnectRotationValueChanged(bool bConnect);

    void InitRange();


private slots:
    void OnScaleValueChangedSlot(double value);
    void OnPositionXValueChangedSlot(double value);
    void OnPositionYValueChangedSlot(double value);
    void OnRotationValueChangedSlot(double value);
    void OnButtonClieckedSlot(int id);

private:
    FSLabel *m_pLabelScale;
    FSLabel *m_pLabelRotation;
    FSLabel *m_pLabelPosition;
    FSLabel *m_pLabelPositionX;
    FSLabel *m_pLabelPositionY;

    QButtonGroup *m_pGroupButton;
    FSButton *m_pButtonScale;
    FSButton *m_pButtonRotation;
    FSButton *m_pButtonPositionX;
    FSButton *m_pButtonPositionY;

    FSDoubleSpinBox *m_pDoubleSpinBoxScale;
    AngleDoubleSpinBox *m_pDoubleSpinBoxRotation;
    FSDoubleSpinBox *m_pDoubleSpinBoxPositionX;
    FSDoubleSpinBox *m_pDoubleSpinBoxPositionY;


    double m_ScaleX;
    double m_ScaleY;

    double m_ScaleDisplay;

    double m_defaultPosX;
    double m_defaultPosY;

    double m_defaultScaleX;
    double m_defaultScaleY;

    double m_defaultAngle;

    int m_bChangeFlag;
};

class FSCursorClickSoundOption :public IFSOptionView{
    Q_OBJECT
public:
    FSCursorClickSoundOption(QWidget *pParent);
    ~FSCursorClickSoundOption();
    
    virtual void UpdataParma(IFSEditHelper *pHelper);

    void GetVoluem(double & Voluem);
protected:
    virtual void OnReset();
    virtual bool IsTitleResetable();

    void ConnectVoluemValueChanged(bool bConnect);
    int TranslateValueX(int x);
    int TranslateValueY(int y);

    void RetranslateUi();

protected slots:
    void OnVoluemValueChangedSlot(double value);

private:
    void CreateUI();


protected:
    ToolBarSlider  *m_pSliderVoluem;
    FSDoubleSpinBox *m_pDoubleSpinBoxVoluem;
    FSButton       *m_pButtonVoluem;
    FSLabel        *m_pLabelVoluem;

    QVBoxLayout    *m_pVL;

    int m_defaultVolume;
};

class ColorSelecterComboBox;
class FSBackground1Option :public IFSOptionView{
    Q_OBJECT
public:
    FSBackground1Option(QWidget *pParent);
    ~FSBackground1Option();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    virtual QColor GetColor();

    

    void InitColorList(QStringList colorList);

    bool isBgColorChanged();

    void ResetChangeFlag();
protected:
    virtual void OnReset();
    virtual bool IsTitleResetable();
    virtual void OnColorResetBtnClieck();
    virtual void OnColorChanged(QColor color);
    void RetranslateUi();

private:
    void CreateUI();


protected:
    FSLabel                 *m_pLabelColor;
    ColorSelecterComboBox   *m_pComBoxColor;
    FSButton                *m_pButtonColor;
    QVBoxLayout             *m_pVL;

    QColor       m_CurrentColor;

    QColor       m_defaultColor;

    int          m_bChangedFlag;

};


class FSAnotationOption :public IFSOptionView{
    Q_OBJECT

signals:
    void startValueChanged(double);
public:
    FSAnotationOption(QWidget *pParent);
    ~FSAnotationOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    QColor GetShapeColor();
    QColor GetBoderShapeColor();
    int GetBorderWidth();

    bool IsShapeColorChanged();
    bool IsBorderColorChanged();
    bool IsBorderWidthChanged();

    void ResetChangeFlag();

protected:
    virtual void OnReset();

private:
    virtual void RetranslateUi();
    void CreateUI();
    void BlockSignals(bool bBlock);

    int Alpha2Opacity(int alpha);
    int Opacity2Alpha(int Opacity);

    bool IsResetable();
    bool IsShapeResetable();
    bool IsBorderWidthResetable();
    bool IsBorderShapeResetable();

    void ConnectSignals();

private slots:
    void SlotShapeOpacityChanged(QString v);
    void SlotBorderOpacityChanged(QString v);
    void SlotBorderWidthChanged(QString w);

private:
    FSLabel *m_pLabelShapeOpacity;
    FSLabel *m_pLabelBorder;
    FSLabel *m_pLabelBorderOpacity;

    FSButton *m_pButtonShapeOpacityReset;
    FSButton *m_pButtonBorderReset;
    FSButton *m_pButtonBorderOpacityReset;

    FSDoubleSpinBox *m_pSpinBoxShapeOpacity;
    ColorSelecterComboBox *m_pSelecterShapeColor;
    FSDoubleSpinBox *m_pSpinBoxBorder;

    FSDoubleSpinBox *m_pSpinBoxBorderOpacity;
    ColorSelecterComboBox *m_pSelecterBorderColor;

    QColor m_currentShapeColor;
    QColor m_currentBorderColor;

    QColor m_defaultShapeColor;
    QColor m_defaultBorderColor;
    int    m_defaultBorderWidht;

    int    m_bChangeFlag;
};

class FSScrollArea :public QScrollArea{
    Q_OBJECT
public:
    FSScrollArea(QWidget *pParent);
    ~FSScrollArea();

    void AddWidget(QWidget *pParent,QString Name);
    QList<QWidget *> GetWidgets();

    void SortWidget(QStringList WidgetNameList);

private:
    QWidget     *m_pWidget;
    QVBoxLayout *m_pVL;
    QMap<QString, QWidget *> mWidgetMap;
};

class FSTextBaseOption :public IFSOptionView{
    Q_OBJECT

signals:
    void sigCharSelecetChanged(int start, int length);
    void sigKeyPress(QKeyEvent *e);
    void sigInputMethod(QInputMethodEvent *e);

public:
    FSTextBaseOption(QWidget *pParent);
    ~FSTextBaseOption();
    virtual void UpdataParma(IFSEditHelper *pHelper);

    QString GetFont();
    void    SetFont(QString font);

    int GetFontSize();
    void SetFontSize(int size);

    QColor GetTextColor();
    void SetTextColor(QColor color);

    NLEAlignType GetTextAlign();
    void SetTextAlign(NLEAlignType type);

    bool IsFontBold();
    void SetFontBold(bool bBold);
    bool IsFontItalic();
    void SetFontItalic(bool bItalic);


    bool isFontNameChanged();
    bool isFontSizeChanged();
    bool isFontColorChanged();
    bool isTextAlignChanged();

    void ResetChangeFlag();

    void SetText(QString text);
    FSTextEdit *GetTextEdit();

protected:
    virtual void OnReset();
    virtual bool IsTitleResetable();

    bool IsStyleResetable();


private:
    void CreateUI();
    void ConnectSignals();
    void InitUIText();
    void RetranslateUi();

    void BlockSignals(bool bBlock);

    
    
private slots:
    void OnButtonClikedSlot(int id);
    void OnButtonGroup1ClikedSlot(int id);
    void OnFontChangedSlot(QString text);
    void OnSizeChangedSlot(QString text);

private:
    FSLabel *m_pLabelFont;
    FSLabel *m_pLabelSize;
    FSLabel *m_pLabelColor;
    FSLabel *m_pLabelAlign;
    FSLabel *m_pLabelContent;

    FSComboBox *m_pComboBoxFont;
    FSComboBox *m_pComboBoxSize;
    ColorSelecterComboBox *m_pComboBoxColor;

    FSButton *m_pButtonBold;
    FSButton *m_PButtonItalic;


    FSButton *m_pButtonAlignLeft;
    FSButton *m_PButtonAlignCenter;
    FSButton *m_pButtonAlignRight;
    FSButton *m_pButtonAlign2Side;

    FSButton *m_pButtonFontReset;
    FSButton *m_pButtonSizeReset;
    FSButton *m_pButtonColorReset;
    FSButton *m_pButtonAlignReset;

    FSTextEdit *m_pTextEdit;

    QButtonGroup *m_pButtonGroup;
    QButtonGroup *m_pButtonGroup1;

    QMap<QString,QString> m_FontMap;
    QStringList m_FontSizeList;


    QString m_DefaultFontName;
    int     m_DefaultFontSize;
    QColor  m_DefaultFontColor;
    bool    m_bDefaultBold;
    bool    m_bDefaultItalic;
    int     m_shadowDerection;

    NLEAlignType m_DefaultType;

    int     m_bChangedFlag;
};

class FSCursorBaseOption :public IFSOptionView{
    Q_OBJECT
public:
    FSCursorBaseOption(QWidget *pParent);
    ~FSCursorBaseOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    double GetMouseScale();
    double GetMouseOpacity();

protected:
    virtual void OnReset();
    virtual bool IsTitleResetable();

protected slots:
    void OnScaleValueChangedSlot(double value);
    void OnOpacityValueChangedSlot(double value);

private:
    void CreateUI();
    void ConnectSignals();
    void ConnectScaleChangedSignals(bool bConnect);
    void ConnectOpacityChangedSignals(bool bConnect);
    void RetranslateUi();

private:
    FSLabel *m_pLabelScale;
    FSLabel *m_pLabelOpacity;

    ToolBarSlider  *m_pSliderScale;
    ToolBarSlider  *m_pSliderOpacity;

    FSDoubleSpinBox *m_pDoubleSpinBoxScale;
    FSDoubleSpinBox *m_pDoubleSpinBoxOpacity;

    FSButton *m_pButtonScale;
    FSButton *m_pButtonOpacity;

    double m_DefaultScale;
    double m_DefaultOpacity;
};

class FSCursorHighlightOption :public FSBackground1Option{
    Q_OBJECT
public:
    FSCursorHighlightOption(QWidget *pParent);
    ~FSCursorHighlightOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);


};

class FSAudioBaseOption :public FSCursorClickSoundOption{
    Q_OBJECT
public:
    FSAudioBaseOption(QWidget *pParent);
    ~FSAudioBaseOption();
    
    virtual void UpdataParma(IFSEditHelper *pHelper);
    virtual void RetranslateUi();

    bool isMix2MonoChecked();

private:
    void ConnectMix2MononStateChnage(bool bConnect);
    void OnMix2MonoStateChnaged(int checkState);

private:
    QCheckBox *m_pCheckBox;

};

class FSDenoiseOption :public IFSOptionView{
    Q_OBJECT

public:
    enum DeNoiseLevel
    {
        DeNoise_LOW_LEVEL=1,
        DeNoise_MID_LEVEL,
        DeNoise_HIGH_LEVEL
    };

public:
    FSDenoiseOption(QWidget *pParent);
    ~FSDenoiseOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);
    virtual void RetranslateUi();

    virtual void OnReset();

    bool IsDenoiseEnable();

    DeNoiseLevel GetDenoiseLevel();
    int GetLastDenoiseLevel();

private:
    void ConnectDenoiseEnableStateChanged(bool bConnect);
    void OnDenoiseEnableStateChanged(int checkState);

    void ConnectDenoiseLevelChanged(bool bConnect);
    void CreateUI();
    void connectSignals();

    void ClearGroupButtonCheckState();
    void EnableWidget(bool bEnable);
    void BlockSignals(bool bBlock);

private slots:
    void OnDenoiseLevelChanged(int id,bool bChecked);


private:
    FSLabel   *m_pEnableLabel;
    FSLabel   *m_pLevelLable;
    QCheckBox *m_pCheckBox;

    QRadioButton *m_pLowRadioBtn;
    QRadioButton *m_pMidRadioBtn;
    QRadioButton *m_pHighRadioBtn;

    QButtonGroup *m_pButtonGroup;

    int m_DefaultDenoiseLevel;
    int m_LastDenoiseLevel;
};


class FSCursorClickRingOption :public FSBackground1Option{
    Q_OBJECT
public:
    FSCursorClickRingOption(QWidget *pParent);
    ~FSCursorClickRingOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);
};


class FSHotKetTipOption :public IFSOptionView{
    Q_OBJECT
public:
    FSHotKetTipOption(QWidget *pParent);
    ~FSHotKetTipOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    QString GetHotKeySize();
    bool    HotKeyEnable();
    QString GetHotKeyPosition();
    QString GetHotKeyStyle();
    QColor  GetHotKeyColor();
    int     GetHotKeyOpacity();
    int     GetFliterType();


protected:
    virtual void RetranslateUi();
    virtual void OnReset();

private:
    void CreateUI();
    void ConnectSignals();

    void OnCheckBoxStateChanged(int state);


    void EnableWidget(bool bEnable);
    void BlockSignals(bool bBlock);

    void InitDefaultParma(IFSEditHelper *pHelper);
    void UpdataUI(IFSEditHelper *pHelper);
    void UpadaResetUI();

    int Alpha2Opacity(int alpha);
    int Opacity2Alpha(int Opacity);

    bool IsSizeResetable();
    bool IsPositionResetable();
    bool IsColorResetable();
    bool IsStyleResetable();
    bool IsOpacityResetable();
    bool IsFliterResetable();
    bool IsResetable();
    void OnColorChanged(QColor color);

protected slots :

    void OnHotKeySizeChanged(int id, bool bChecked);  
    void OnHotKeyFliterChanged(int id, bool bChecked);
    void OnStyleChanged(QString);
    void OnPositionChanged(QString);
    void OnOpacityChanged(double);

private:
    FSLabel *m_pLabelEnable;
    FSLabel *m_pLabelSize;
    FSLabel *m_pLabelFliter;
    FSLabel *m_pLabelPosition;
    FSLabel *m_pLabelStyle;
    FSLabel *m_pLabelColor;
    FSLabel *m_pLabelOpacity;

    FSButton *m_pButtonResetSize;
    FSButton *m_pButtonResetFliter;
    FSButton *m_pButtonResetPosition;
    FSButton *m_pButtonResetStyle;
    FSButton *m_pButtonResetColor;
    FSButton *m_pButtonResetOpacity;



    QCheckBox *m_pCheckBoxEnable;

    QButtonGroup *m_pButtonGroup;
    QRadioButton *m_pButtonSmall;
    QRadioButton *m_pButtonMid;
    QRadioButton *m_pButtonLarge;

    QButtonGroup *m_pButtonGroup1;
    QRadioButton *m_pButtonHotKeyFliter;
    QRadioButton *m_pButtonAllHotKey;

    FSComboBox   *m_pComboBoxPosition;
    FSComboBox   *m_pComboBoxStyle;

    ColorSelecterComboBox *m_pColorSelecter;

    FSDoubleSpinBox *m_pSpinBoxOpacity;

    bool    m_bDefaultEnable;
    QString m_nDefaultSize;
    QString m_DefaultPoint;
    QString m_DefaultStyle;
    QColor  m_DefaultColor;
    int     m_nDefaultOpacity;
    int     m_nFliter;
};

class FSSVGCaptionOption :public IFSOptionView{
    Q_OBJECT
public:
    FSSVGCaptionOption(QWidget *pParent);
    ~FSSVGCaptionOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);
    
    QMap<int, QPair<bool, QColor>> &GetColors();
protected:
    virtual void OnReset();
    virtual void RetranslateUi();
    void UpdataUI(int count);


private:
    QList<FSCaptionSvgItem *> m_pItemList;
    QMap<int, QPair<bool, QColor>> m_ColorMap;
    QVBoxLayout                    *m_pVL;
};


class FSSpeedOption :public IFSOptionView{
    Q_OBJECT

signals:
    void speedChanged();

public:
    FSSpeedOption(QWidget *pParent);
    ~FSSpeedOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    float GetSpeedScale();

protected:
    virtual void OnReset();
    virtual void RetranslateUi();

private:
    void CreateUI();
    void ConnectSignals();
    void setDurationText(QString text);

private slots:
    void OnSpinBoxValueChanged(QString text);


private:
    FSLabel  *m_pLabelSpeed;
    FSLabel  *m_pLabelDuration;

    FSDoubleSpinBox *m_pSpeedSpinBox;
    FSLabel         *m_pLabelDurationText;

    float           m_frameRate;
};

class FSAudioSpeedOption :public FSSpeedOption{
    Q_OBJECT
public:
    FSAudioSpeedOption(QWidget *pParent);
    ~FSAudioSpeedOption();

};


class FSTextBorderOption :public IFSOptionView{
    Q_OBJECT
public:
    FSTextBorderOption(QWidget *pParent);
    ~FSTextBorderOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);


    QColor GetTextBorderColor();
    int    GetTextBorderOpacity();
    int    GetTextBorderSize();
    int    GetTextBorderBlur();

    bool IsColorChanged();
    bool IsOpacityChanged();
    bool IsBlurChanged();
    bool IsSizeChanged();
    void ResetChangeFlag();


protected:
    virtual void OnReset();
    virtual void RetranslateUi();

private slots:
    void OnOpacityValueChanged(double);
    void OnBlurValueChanged(double);
    void OnSizeValueChanged(double);
    void OnCurrentColorChanged(QColor);

private:
    void CreateUI();
    void ConnectSignals();
    void BlockSignals(bool bBlock);

    

    void InitDefaultParma(IFSEditHelper *pHelper);
    void UpdataUI(IFSEditHelper *pHelper);
    void UpadaResetUI();

    bool IsResetable();
    bool IsOpacityResetable();
    bool IsBlurResetable();
    bool IsSizeResetable();
    bool IsColorResetable();

    int Alpha2Opacity(int alpha);
    int Opacity2Alpha(int Opacity);

private:
    ColorSelecterComboBox *m_pColorSelecter;
    FSDoubleSpinBox       *m_pSpinBoxOpacity;
    FSDoubleSpinBox       *m_pSpinBoxSize;
    FSDoubleSpinBox       *m_pSpinBoxBlur;

    FSLabel               *m_pLabelColor;
    FSLabel               *m_pLabelOpacity;
    FSLabel               *m_pLabelBlur;
    FSLabel               *m_pLabelSize;

    FSButton              *m_pButtonResetSize;
    FSButton              *m_pButtonResetBlur;
    FSButton              *m_pButtonResetColor;
    FSButton              *m_pButtonResetOpacity;  

    QColor                 m_DefaultColor;
    int                    m_DefaultOpacity;
    int                    m_DefaultBlur;
    int                    m_DefaultSize;

    int                    m_ChangeFlag;
};



class FSTextShadowSelecter;
class FSTextShadowOption :public IFSOptionView{
    Q_OBJECT
public:
    FSTextShadowOption(QWidget *pParent);
    ~FSTextShadowOption();

    virtual void UpdataParma(IFSEditHelper *pHelper);

    QColor GetTextShadowColor();
    int    GetTextShadowOpacity();
    int    GetTextShadowDistance();
    int    GetTextShadowBlur();
    int    GetTextShadowDerection();

    bool IsColorChanged();
    bool IsOpacityChanged();
    bool IsBlurChanged();
    bool IsTypeChanged();
    bool IsDistanceChanged();
    void ResetChangeFlag();

protected:
    virtual void OnReset();
    virtual void RetranslateUi();

    private slots:
    void OnOpacityValueChanged(double);
    void OnBlurValueChanged(double);
    void OnTypeValueChanged(int);
    void OnCurrentColorChanged(QColor);
    void OnDistanceChanged(double);

private:
    void CreateUI();
    void ConnectSignals();
    void BlockSignals(bool bBlock);

    void InitDefaultParma(IFSEditHelper *pHelper);
    void UpdataUI(IFSEditHelper *pHelper);
    void UpadaResetUI();

    int Alpha2Opacity(int alpha);
    int Opacity2Alpha(int Opacity);

    bool IsResetable();
    bool IsOpacityResetable();
    bool IsBlurResetable();
    bool IsTypeResetable();
    bool IsColorResetable();
    bool IsDistanceResetable();


private:
    ColorSelecterComboBox *m_pColorSelecter;
    FSDoubleSpinBox       *m_pSpinBoxOpacity;
    FSDoubleSpinBox       *m_pSpinBoxDistance;
    FSDoubleSpinBox       *m_pSpinBoxBlur;

    FSTextShadowSelecter  *m_pTextShadowSelecter;

    FSLabel               *m_pLabelColor;
    FSLabel               *m_pLabelOpacity;
    FSLabel               *m_pLabelBlur;
    FSLabel               *m_pLabelDistance;
    FSLabel               *m_pLabelDerection;

    FSButton              *m_pButtonResetDistance;
    FSButton              *m_pButtonResetBlur;
    FSButton              *m_pButtonResetColor;
    FSButton              *m_pButtonResetOpacity;
    FSButton              *m_pButtonResetDerection;

    QColor                 m_DefaultColor;
    int                    m_DefaultOpacity;
    int                    m_DefaultBlur;
    int                    m_DefaultDistance;
    int                    m_DefaultDerection;
    int                    m_ChangeFlag;

};

class IFSEditHelper;
class IFSEditHelperManager;
class IPlayerEditorManager;
class UndoCommand;
class FSNLETimeline;
class FSPropertyView : public FSBaseWidget
{
    Q_OBJECT
signals:
    void sigShowPropertyView(bool bshow);
    void sigSpeedChanged(FSNLETimeline *timeline,float scale);
	void sigDenoise(FSNLETimeline *timeline);
public:
    enum MouseEffictType{
        Mouse_ClickSound=-1,
        Mouse_UnKown,
        Mouse_Highlight,
        Mouse_Rings        
    };

public:
    void UpdateScale(double x, double y);
    void UpdatePosition(double x, double y);
    void UpdateRotation(double rotation);

    void SetEditHelperManager(IFSEditHelperManager *);
    void SetPlayerEditManager(IPlayerEditorManager*);

    void SetEditHelper(IFSEditHelper *pHelper);
    void OnTransformInfoChangedSlot(IFSEditHelper *pHelper);
    void OnEndChangedSlot(UndoCommand *pCmd);
    

    void AddOptionWidget(const QString &Tiltle, const char *sztitle, bool bCanDelete, IFSOptionView::OptionType optionType);
    IFSOptionView * GetOptionView(IFSOptionView::OptionType optionType);

    void UpdataParam();
	void TransfromGaInfo(int iType);

public:
    FSPropertyView(QWidget *pParent = 0);
    ~FSPropertyView();

protected:
    void resizeEvent(QResizeEvent *);

protected slots:
    void OnButtonClickedSlot(int id);

    void OnParmaChangedSlot();

    void OnDeletePropotySlot();

    void OnMouseEffictAdded(int);

    void OnCmdUpdataSlot();

    void OnSpeedChanged();

    void OnTextChanged(QString);

    void OnButtonTogged(int,bool);

private:
    virtual void RetranslateUi();

    void CreateUI();
    
    void ConnectSignal();

    void SetVisibleView(int flag);

    void SetCursorViewVisible();

    void SetShowHideButtonState(bool showState);


private:

    IFSEditHelperManager      *m_pEditorHelperManager;
    IPlayerEditorManager      *m_pPlayerEditorManager;
    FSVisualBaseOption        *m_pVisualPropertyView;

    FSLabel                   *m_pWidgetMain;
    FSLabel                   *m_pStyleTitle;
    QWidget                   *m_pWidgetTitle;
    QStackedWidget            *m_pStachedWidget;

    FSScrollArea               *m_pScrollAreaVisual;
    FSScrollArea               *m_pScrollAreaAudio;
    FSScrollArea               *m_pScrollAreaAnnotation;
    FSScrollArea               *m_pScrollAreaCursor;
    FSScrollArea               *m_pScrollAreaHotKey;

    IFSEditHelper             *m_pEditHelper;

    QButtonGroup         *m_pButtonGroup;
    FSButton             *m_pButtonVisual;
    FSButton             *m_pButtonCursor;
    FSButton             *m_pButtonAudio;
    FSButton             *m_pButtonAnnotation;
    FSButton             *m_pButtonHotKey;
    FSStateBaseButton    *m_pButtonShowProperty;

    MouseEffictType      m_MouseEffictType;
};

class FSTextEdit :public QTextEdit{
    Q_OBJECT
    signals :
    void sigKeyPress(QKeyEvent *e);
    void sigInputMethod(QInputMethodEvent *e);
public:
    FSTextEdit(QWidget *pParent);
    ~FSTextEdit();


    void SetPos(int pos);
protected:
    void keyPressEvent(QKeyEvent *e);
    void inputMethodEvent(QInputMethodEvent *e);

private:
    int m_pos;
};

#endif // FSPROPERTYVIEW_H
