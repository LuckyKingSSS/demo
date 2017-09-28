#ifndef FSDRAWRECTHELPER_H
#define FSDRAWRECTHELPER_H

#include <QObject>
#include <QTransform>
#include "FSPainterPath.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLECaption.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "Interface/INLECaptionBaseObj.h"
#include "Interface/INLECaptionTipTextObj.h"
#include "Interface/INLECaptionTextObj.h"
#include "Interface/INLECaptionShapeObj.h"
#include "Interface/INLECaptionSVGTextObj.h"
#include "INLEKeyboardStreamManager.h"

#include "Include/NLECaptionDefine.h" 
#include <QtXml/QDomDocument>

class QWidget;
class QMouseEvent;
class QKeyEvent;
class QInputMethodEvent;
class QResizeEvent;
class IMediaPlayerControl;
class IFSDragBoxControl;

#define MYRGB(r,g,b)  ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define MAX_PATHLENGTH 256

#define Default_Position         "default_Posiotion"
#define Default_Scale            "default_Scale"
#define Default_Angle            "default_Angle"
#define Default_AudioVolume      "default_AudioVolume"
#define Default_TextBgOptacity   "default_TextBgOptacity"
#define Default_TextBgColor      "default_TextBgColor"
#define Default_TextShapeBgColor   "default_TextShapeBgColor"
#define Default_TextBorderBgColor "default_TextBorderBgColor"
#define Default_TextBorderWidth  "default_TextBorderWidth"
#define Default_FontName         "default_FontName"
#define Default_FontSize         "default_FontSize"
#define Default_FontBold         "default_FontBold"
#define Default_FontItalic       "default_FontItalic"
#define Default_FontTextColor    "default_FontTextColor"
#define Default_ShadowDerection  "default_ShadowDerection"
#define Default_TextAlignType    "default_TextAlignType"
#define Default_CurssorScale     "default_CurssorScale"
#define Default_CurssorOptacity  "default_CurssorOptacity"
#define Default_CurssorVolume    "default_CurssorVolume"
#define Default_CurssorHighlight "default_CurssorHighlight"
#define Default_CurssorRingColor "default_CurssorRingColor"
#define MouseEffectOrder         "MouseEffectOrder"
#define DefaultDenoiseLevel      "default_DenoiseLevel"
#define HotKeyTipEnable          "default_HotKeyEnable"
#define HotKeySize               "default_HotKeySize"
#define HotKeyPoision            "default_HotKeyPosition"
#define HotKeyStyle              "default_HotKeyStyle"
#define HotKeyColor              "default_HotKeyColor"
#define HotKeyOpacity            "default_HotKeyOpacity"
#define HotKeyFliter             "default_HotKeyEnable"
#define Defualt_TextBorderColor  "defualt_TextBorderColor"
#define Default_TextBorderBlur   "default_TextBorderBlur"
#define Default_TextBorderSize   "default_TextBorderSize"
#define Default_TextBorderOpacity "default_TextBorderOpacity"

#define Defualt_TextShadowDerection  "defualt_TextShadowDerection"
#define Defualt_TextShadowColor      "defualt_TextShadowColor"
#define Default_TextShadowBlur       "default_TextShadowBlur"
#define Default_TextShadowDistance   "default_TextShadowDistance"
#define Default_TextShadowOpacity    "default_TextShadowOpacity"
#define Default_CaptionSvgColors     "default_CaptionSvgColors"




class IFSEditHelper :public QObject
{
    Q_OBJECT
public:
    enum EditType{
        Edit_Text,
        Edit_Picture,
        Edit_Video,
        Edit_Audio
    };

    enum SnapType{
        UnKnown,
        Roriate,
        ZoomInOut,
        Trans,
        CtrlLine,
        CtrlShape
    };

signals:
    void TransformChanged();
    void CmdUpdate();
    void sigRepaintWidget();

public:
    IFSEditHelper(FSNLETimeline *timeLine, IMediaPlayerControl *pPlayerControl, QObject *parent);
    ~IFSEditHelper();

    virtual bool DoMousePressEvent(QMouseEvent *e){ return true; };
    virtual void DoMouseMoveEvent(QMouseEvent *e){ };
    virtual void DoMouseUpEvent(QMouseEvent *e){};
    virtual void DoMouseDoubleClickEvent(QMouseEvent *e){};
    virtual void DoPaintEvent(QPainter &Paniter,QPointF Offset){};
    virtual void DoPaintControlBoxEvent(QPainter &Paniter, QPointF Offset){};
    virtual bool DoKeyPressEvent(QKeyEvent *e){ return false; };
    virtual bool DoinputMethodEvent(QInputMethodEvent *e){return false;};
    virtual void DoResizeEvent(QResizeEvent *e);

    virtual void SetPosWithDelta(QPointF OffSet){};
    virtual void DoMouseRealese(QPointF curPt){};
    virtual void GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt){};
    virtual void GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle){};
    virtual void SetScaleWithDelta(QPointF deltaScale,int index){};
    virtual void SetAngleWithDelta(double deltaAngle, bool bSnap){};
    virtual void SetCtlrPos(QPointF curPt, int index){};

    virtual void GetChangedTransformInfo(void * info){};
    virtual void UpdataTransformFromBrother(void * info){};

    virtual double GetAngle(){ return 0.0; };
    virtual void   SetAngle(double angle){};

    virtual void GetScale(double &x, double &y){};
    virtual void SetScale(double x, double y){};

    virtual void GetPosition(double &x, double &y){};
    virtual void SetPosition(double x, double y){};


    virtual bool IsSelected(){ return mbSelected; };
    virtual void SetSelected(bool bSelected){ mbSelected = bSelected; };

    virtual bool IsEditState(){ return false; };
    virtual void SetEditState(bool bSelected){return; };
    virtual bool Contains(QPointF pos, QPointF offSet, bool &bEdit){ return false; };

    virtual bool IsChanged(){ return true; };

    virtual void Updata();

    virtual bool IsScaleMode();
    virtual void EnableScaleMode(bool bEnable);

    virtual bool ControlBoxContains(QPointF pos, QPointF offSet,int &index);
    virtual SnapType GetSnapType();

    virtual void GetPath(QList<FSPainterPath> &listPath, QPointF offSet);

    virtual bool Intersects(QRect rc){ return false; };

    virtual QList<QPointF >GetGuidesPtList(){ return QList<QPointF>(); };
    virtual QList<QPointF>GetTargetPtList(){ return QList<QPointF>(); };
    virtual bool GetGuidesPt(int index,QPointF &pt){ return false; };
    virtual int GetKeyPointCount(){ return 0; };

    virtual void UpdataKeyPointNeedFillState(QPointF pt){};
    virtual void UpdataKeyPointPressedOnState(QPointF pt){};
    virtual void ClearKeyPointState(){};
    virtual void SetShiftFlag(bool bPressedShift){};

    virtual void SetLinePathColor(QColor color){};
    virtual void SetControlPathColor(QColor color){};
    virtual void SetControlBoxFillColor(QColor color){};
    virtual void SetRolationFillColor(QColor color){};
    virtual void SetLineWidth(float w){};
    virtual void SetLinePathStyle(Qt::PenStyle style){};

    virtual bool NeedToDrawPath(){ return IsSelected(); };

    bool Resetable();


    Qt::CursorShape GetCursor(){ return mCursorShape; };
    

    EditType GetEditType();
   

    FSNLETimeline  * GetTimeLine(){ return mpTimeLine; }


    bool GetDefaultPosiont(double &x, double &y);
    bool GetDefaultScale(double &x, double &y);
    bool GetDefaultAngle(double &angle);
    bool GetDefaultVolume(int &volume);
    bool GetDefaultTextBgColor(QColor &color);
    bool GetDefaultTextBgOptacity(int &optacity);

    bool GetDefaultTextShapeBgColor(QColor &color);
    bool GetDefaultTextBorderBgColor(QColor &color);
    bool GetDefaultTextBorderWidht(int &width);

    bool GetDefaultFontName(QString &fontName);
    bool GetDefaultFontSize(int &size);
    bool GetDefaultFontBold(bool &bBold);
    bool GetDefaultFontItalic(bool &bItalic);
    bool GetDefaultFontColor(QColor &color);
    bool GetDefaultShadowDerection(int &derection);
    bool GetDefaultTextAlignType(NLEAlignType &type);
    bool GetDefaultCursorScale(double &scale);
    bool GetDefaultCursorOptacity(double &optacity);
    bool GetDefaultCursorVolume(int &volume);
    bool GetDefaultCursorRingColor(QColor &color);
    bool GetDefaultCursorHighlight(QColor &color);
    bool GetDefaultDenoiseLevel(int &level);
    bool GetDefaultHotKeyEnable(bool &bEnable);
    bool GetDefaultHotKeySize(QString  &size);
    bool GetDefaultHotKeyPosition(QString &pos);
    bool GetDefaultHotKeyStyle(QString &style);
    bool GetDefaultHotKeyColor(QColor & color);
    bool GetDefaultHotKeyOpacity(int &Opacity);
    bool GetDefaultHotKeyFliter(int &type);
    bool GetDefualtTextBorderColor(QColor &color);
    bool GetDefualtTextBorderSize(int &size);
    bool GetDefualtTextBorderOpacity(int &Opacity);
    bool GetDefualtTextBorderBlur(int &blur);

    bool GetDefualtTextShadowDerection(int &derection);
    bool GetDefualtTextShadowColor(QColor &color);
    bool GetDefualtTextShadowDistance(int &size);
    bool GetDefualtTextShadowOpacity(int &Opacity);
    bool GetDefualtTextShadowBlur(int &blur);
    bool GetDefualtSvgCaptionColors(QStringList &list);

    void UpdateMouseEffectOrder(QString Effect);
    QStringList GetMouseEffectOrderList();
    virtual void InitDefaultPropertyValue();

    void SetBorderWidth(int width);

    void SetLastDenoiseLevel(int level);
    int GetLastDenoiseLevel();

    void SetPositionScale(double scaleX, double scaleY);
    void GetPositionScale(double &scaleX, double &scaleY);

    void SetDragingFlag(bool bDraging);

protected:
    QRectF GetBaseRect();
    QRectF GetRealRenderRectf(QRectF renderRect, QRectF baseRc);
    void   InitTransformInfo(QRectF renderRect, QRectF baseR, double angle, double& scaleX, double& scaleY, double& transx, double& transy);
    QRectF GetRectFromCenterPt(QPointF Center, int width);
    QTransform GetRotatedTransform(QRectF rc,double angle);

    double GetRolationAngle(QPointF ptCenter, QPointF ptLast, QPointF ptCurrent);
    bool   IsThreePointOnLine(QPointF destPt, QPointF leftPt, QPointF rightPt);
    qreal  Distance(QPointF Pt1, QPointF Pt2);
    qreal  GetScaleDistance(QPointF currentPt, QPointF destPt, QPointF destPt1);

protected:
    int             mControlItemWidht;
    int             mBorderWidth;
    int             m_LastDenoiseLevel;
    bool            m_bDragingFlag;
    double          mPositionScaleX;
    double          mPositionScaleY;
    Qt::CursorShape mCursorShape;
    QWidget             *mpParentWidget;
    FSNLETimeline       *mpTimeLine;
    IMediaPlayerControl *mpPlayerControl;

private:
    bool            mbSelected;
    bool            m_bScaleMode;

};


class INLETimelineBase;
class INLEFrame;
class INLECaption;
class FSTextEditHelper :public IFSEditHelper
{
    Q_OBJECT

signals:
    void TextChanged(const QString &textStr);
    void TextUnDoReDo();

public:
    FSTextEditHelper(FSNLETimeline *timeLine, IMediaPlayerControl *pControl, QObject *pParent);
    ~FSTextEditHelper();

    virtual bool DoMousePressEvent(QMouseEvent *e);
    virtual void DoMouseMoveEvent(QMouseEvent *e);
    virtual void DoMouseUpEvent(QMouseEvent *e);
    virtual void DoMouseDoubleClickEvent(QMouseEvent *e);
    virtual void DoPaintEvent(QPainter &Paniter, QPointF Offset);
    virtual void DoPaintControlBoxEvent(QPainter &Paniter, QPointF Offset);
    virtual bool DoKeyPressEvent(QKeyEvent *e);
    virtual bool DoinputMethodEvent(QInputMethodEvent *e);
    virtual void DoResizeEvent(QResizeEvent *e);

    virtual void SetPosWithDelta(QPointF OffSet);
    virtual void DoMouseRealese(QPointF curPt);
    virtual void GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt);
    virtual void GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle);
    virtual void SetScaleWithDelta(QPointF deltaScale, int index);
    virtual void SetAngleWithDelta(double deltaAngle,bool bSnap);
    virtual void SetCtlrPos(QPointF curPt, int index);

    virtual bool IsSelected();
    virtual void SetSelected(bool bSelected);

    virtual bool IsEditState();
    virtual void SetEditState(bool bSelected);
    virtual bool Contains(QPointF pos, QPointF offSet, bool &bEdit);

    virtual bool IsChanged();

    virtual double GetAngle();
    virtual void SetAngle(double angle);

    virtual void GetScale(double &x, double &y);
    virtual void SetScale(double x, double y);

    virtual void GetPosition(double &x, double &y);
    virtual void SetPosition(double x, double y);
    virtual void GetPath(QList<FSPainterPath> &listPath, QPointF offSet);
   
    virtual bool IsScaleMode(){ return true; };
    virtual bool ControlBoxContains(QPointF pos, QPointF offSet, int &index);
    virtual SnapType GetSnapType();
    virtual bool Intersects(QRect rc);

    virtual QList<QPointF >GetGuidesPtList();
    virtual QList<QPointF>GetTargetPtList();
    virtual bool GetGuidesPt(int index, QPointF &pt);
    virtual int GetKeyPointCount();
    virtual void UpdataKeyPointNeedFillState(QPointF pt);
    virtual void UpdataKeyPointPressedOnState(QPointF pt);
    virtual void ClearKeyPointState();

    virtual void SetLinePathColor(QColor color);
    virtual void SetControlPathColor(QColor color);
    virtual void SetControlBoxFillColor(QColor color);
    virtual void SetRolationFillColor(QColor color);
    virtual void SetLineWidth(float w);
    virtual void SetLinePathStyle(Qt::PenStyle style);
    virtual void SetShiftFlag(bool bPressedShift);


    void SetCaption(NLEComPtr<INLECaption> pCaption);
    void SetDocState(bool bEdit, bool bchanged);
    void GetDocState(bool &bEdit, bool &bTextEdit);


    bool GetLineCtrlPt(int index, QPointF &pt);
    void SetTextChangeCallBack();

    QString GetText();

    NLECaptionObjData      GetCaptionBaseObj();
    NLEComPtr<INLECaption> GetCaption();

    void   SetBackGroundColor(QColor color);
    QColor GetBackGroundColor();

    void   SetBorderBackGroundColor(QColor color);
    QColor GetBorderBackGroundColor();

    void SetBorderWidth(int w);
    int GetBorderWidth();

    void   SetTextColor(QColor color);
    QColor GetTextColor();

    void   SetTextBorderColor(QColor color);
    QColor GetTextBorderColor();

    void   SetTextBorderOpacity(int opacity);
    int    GetTextBorderOpacity();

    void   SetTextBorderBlur(int blur);
    int    GetTextBorderBlur();

    void   SetTextBorderSize(int size);
    int    GetTextBorderSize();

    void EnableTextBorder(bool bEnable);
    bool IsEnableTextBorder();


    void   SetTextShadowColor(QColor color);
    QColor GetTextShadowColor();

    void   SetTextShadowOpacity(int opacity);
    int    GetTextShadowOpacity();

    void   SetTextShadowBlur(int blur);
    int    GetTextShadowBlur();

    void   SetTextShadowDistance(int distance);
    int    GetTextShadowDistance();

    int GetTextShadowDerection();
    void SetTextShadowDerection(int derection);

    void EnableTextShadow(bool bEnable);
    bool IsEnableTextShadow();

    bool GetCaptionSvgCount(int &count);
    bool GetCaptionSvgColor(int index, QColor &color);
    bool SetCaptionSvgColor(int index, QColor color);

    void SetCharSelect(int start, int len, int posStart = 0);

    void         SetTextAlign(NLEAlignType type);
    NLEAlignType GetTextAlign();

    void    SetFont(QString fontStr);
    QString GetFont();

    void SetFontSize(int size);
    int  GetFontSize();

    void SetFontBold(bool bBold);
    bool GetFontBold();

    void SetFontItalic(bool bItalic);
    bool GetFontIatlic();

    void SetOpacity(int value);
    int  GetOpacity();

    void SetText(QString text);

    void EnableBorder(bool bEnable);
    
    virtual void InitDefaultPropertyValue();

    bool IsLineShape();
private:
    void ChangeCurrsor(NLEMouseType type);
    void InitTextChangeCallBack();
    void InitCaptionObjBeforeChangeCallBack();

    INLECaptionShapeObj   *GetCaptionShapeObj();
    INLECaptionTipTextObj *GetCaptionTipTextObj();  
    INLECaptionTextObj *GetCaptionTextObj();
    INLECaptionSVGTextObj *GetCaptionSvgTextObj();

    void InitKeyPointGroup();
    void UpdataKeyPointGroup();

    void RolationSnap(double &angle);


private:
    TEXTCHANGE_CALLBACK              m_pTextChangedCallBack;
    CAPTIONOBJ_BEFORECHANGE_CALLBACK m_pCaptionObjBeforeChangeCallBack;
    INLECaptionBaseObj    *m_pBaseObj;
    NLEComPtr<INLECaption> mpCaption;
    FSKeyPointGroup        m_KeyPointGroup;
    bool                   mbChanaged;

    bool                   m_bPressedShift;

    QColor          m_ColorLinePath;
    QColor          m_ColorControlPath;
    QColor          m_ColorFillControlBox;
    QColor          m_ColorFillRolationBox;
    float           m_PathLineWidth;
    Qt::PenStyle    m_PenStyle;
};




typedef QList<QPair<QString, QVariant>> HotKeyParms;

class FSScaleBoxControl;
class FSMaskBoxControl;
class UndoCommand;
class IMediaPlayerControl;
class FSVideoPicEditHelper : public IFSEditHelper
{
    Q_OBJECT

signals:
    void EndChanged(UndoCommand *command);

public:
    FSVideoPicEditHelper(FSNLETimeline *timeLine, IMediaPlayerControl *pControl, QObject *parent);
    ~FSVideoPicEditHelper();

    virtual bool DoMousePressEvent(QMouseEvent *e);
    virtual void DoMouseMoveEvent(QMouseEvent *e);
    virtual void DoMouseUpEvent(QMouseEvent *e);
    virtual void DoPaintEvent(QPainter &Paniter, QPointF Offset);
    virtual void DoPaintControlBoxEvent(QPainter &Paniter, QPointF Offset);
    virtual void DoResizeEvent(QResizeEvent *e);

    virtual void SetPosWithDelta(QPointF OffSetPt);
    virtual void DoMouseRealese(QPointF curPt);
    virtual void GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt);
    virtual void GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle);
    virtual void SetScaleWithDelta(QPointF deltaScale, int index);
    virtual void SetAngleWithDelta(double deltaAngle, bool bSnap);

    virtual void   SetAngle(double angle);
    virtual double GetAngle();

    virtual void SetPosition(double x, double y);
    virtual void GetPosition(double &x, double &y);

    virtual void SetScale(double x, double y);
    virtual void GetScale(double &x, double &y);

    virtual bool IsChanged();
    virtual void Updata();

    virtual bool Contains(QPointF pos, QPointF offSet, bool &bEdit);

    virtual void EnableScaleMode(bool bEnable);

    virtual void GetPath(QList<FSPainterPath> &listPath, QPointF offSet);
    virtual bool ControlBoxContains(QPointF pos, QPointF offSet, int &index);
    virtual SnapType GetSnapType();
    virtual bool Intersects(QRect rc);

    virtual QList<QPointF >GetGuidesPtList();
    virtual QList<QPointF>GetTargetPtList();
    virtual bool GetGuidesPt(int index, QPointF &pt);
    virtual int GetKeyPointCount();

    virtual void UpdataKeyPointNeedFillState(QPointF pt);
    virtual void UpdataKeyPointPressedOnState(QPointF pt);
    virtual void ClearKeyPointState();
    virtual void SetShiftFlag(bool bPressedShift);

    virtual void SetLinePathColor(QColor color);
    virtual void SetControlPathColor(QColor color);
    virtual void SetControlBoxFillColor(QColor color);
    virtual void SetRolationFillColor(QColor color);
    virtual void SetLineWidth(float w);
    virtual void SetLinePathStyle(Qt::PenStyle style);
    virtual bool NeedToDrawPath();

    bool   IsDestopVideo();
    double GetMouseScale();
    void   SetMouseScale(double scale);

    double GetMouseOpacity();
    void   SetMouseOpacity(double Opacity);

    QColor GetHighlightColor();
    void   SetHighlightColor(QColor color);

    QColor GetMouseRingColor();
    void   SetMouseRingColor(QColor color);

    int   GetMouseClickSound();
    void  SetMouseClickSound(int voluem);

    void SetHotKeySize(QString Size);
    void GetHotKeySize(QStringList &list, QString &CurSize);

    void SetHotKeyPosition(QString Position);
    void GetHotKeyPosition(QStringList &list, QString &CurPosition);

    void SetHotKeyStyle(QString style);
    void GetHotKeyStyle(QStringList &list, QString &CurStyles);

    QColor GetHotKeyColor();
    void SetHotKeyColor(QColor color);

    bool IsHotKeyEnable();
    void SetHotKeyEnable(bool bEable);

    void SetHotKeyOpacity(int Opacity);
    int GetHotKeyOpacity();


    void SetHotKeyFliterType(int type);
    int GetHotKeyFliterType();

    QString GetHotKeyText();
    void    SetHotKeyText(QString);

    INLECaptionTextObj *GetCaptionTextObj();
    INLECaptionBaseObj *GetCaptionBaseObj();

    void DeleteMouseClickSound();
    void DeleteMouseHighlight();
    void DeleteMouseRing();

    bool IsContainMouseSound(QString &filePath);
    bool IsContainMouseRings(INLETimelineBase **pFilter);
    bool IsContainMouseHighlight(INLETimelineBase **pFilter);

    void AddMouseSound(QString filePath);
    void AddMouseRings(INLETimelineBase *pFilter);
    void AddMouseHighLight(INLETimelineBase *pFilter);

    void SetDefaultCursorVolume();
    void SetDefaultCursorRingColor();
    void SetDefaultCursorHighlight();

    QString GetMaskPath();

    void SetMaskPath(const QString &path);

    void UpdataControlBoxInfo();
    virtual void InitDefaultPropertyValue();

    
    void InitHotKeyParmaMap(const QMap<QString, HotKeyParms> &parmMap);

private:
    void   ConnectSignals();
    void   InitScaleControlBoxInfo();
    void   InitMaskControlBoxInfo();

    void UpdateTimeLineParam(double angle, qreal scareX, qreal scareY, qreal transX, qreal transY, QRectF baseRc);

    void OnScaleTransFormChangedSlot();
    void OnCursorShapeChangedSlot(Qt::CursorShape shape);

    void OnMaskTransFormChangedSlot();
    void OnMaskPathChnagedSlot();

private:
    FSScaleBoxControl    *m_pScaleBoxControl;
    FSMaskBoxControl     *m_pMaskBoxControl;
    IMediaPlayerControl  *m_pPlayerControl;

    QString              m_StartChangeTransInfoStr;
    QString              m_StartChangeMaskPathStr;
    bool                 m_bMoved;
    QMap<QString, HotKeyParms>  m_MapHotkeyParms;
};



class FSAudioEditHelper :public IFSEditHelper{
    Q_OBJECT
public:
    FSAudioEditHelper(FSNLETimeline *timeLine, IMediaPlayerControl *pControl, QObject *parent);
    ~FSAudioEditHelper();


    virtual bool IsChanged(){ return false; };
    virtual bool DoMousePressEvent(QMouseEvent *e){ return false; };
    virtual bool IsScaleMode(){ return true; };


protected:
    virtual void InitDefaultPropertyValue();
};

#endif // FSDRAWRECTHELPER_H
