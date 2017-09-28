#ifndef FMATTERTHUMBNAILBASE_H
#define FMATTERTHUMBNAILBASE_H
#include "inc_FCommonLib/FBaseWidget.h"

class FLabel;
class FLineEdit;
class QProgressBar;

/*FMatterThumbnailBase*/
class FMatterThumbnailBase : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterThumbnailBase(QWidget *parent = nullptr);
	~FMatterThumbnailBase();

	void SetTitle(const QString&);
	void SetEditable(bool);
	void Click();
	void Unclick();
	void Rename();

Q_SIGNALS:
	void sigClicked();
	void sigDoubleClicked();
	void sigAdd();
	void sigTitleChanged(const QString&);

protected:
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mouseDoubleClickEvent(QMouseEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

protected:
	QString GetDisplayString(QString);
	void slotEditingFinished();

protected:
	bool m_bHover;                // 鼠标悬停
	bool m_bClicked;              // 鼠标点击
	bool m_bAddable;              // 可添加到时间线
	bool m_bEditable;             // 可编辑（修改标题，默认可编辑）
	bool m_bTryEdit;
	bool m_bBeginEdit;
	QDateTime m_TryEditTime;
	QPointF m_AddPt;              // 缩略图中心点（可添加到时间线的点）
	QString m_TitleStr;           // 标题
	QRect m_ThumbnailRect;        // 缩略图位置
	QRect m_BottomlineRect;       // 底部指示线位置
	FLabel *m_pTitleLbl;          // 标题Label 
	FLineEdit *m_pTitleLineEdit;  // 标题LineEdit
};

/*FThumbnailDownload*/
class FThumbnailDownload : public FMatterThumbnailBase
{
	Q_OBJECT

public:
	FThumbnailDownload(QWidget* parent = nullptr);
	~FThumbnailDownload();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	QProgressBar *m_pProgressBar;
};

/*FMediaThumbnail*/
class FMediaThumbnail : public FMatterThumbnailBase
{
	Q_OBJECT

public:
	FMediaThumbnail(QWidget* parent = nullptr);
	~FMediaThumbnail();

Q_SIGNALS:
	void sigEditInActionCam();

protected:
	void resizeEvent(QResizeEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

private:
	FLabel *m_pActionCamLbl;
	FLabel *m_pVideoLbl;
};
#endif // FMATTERTHUMBNAILBASE_H
