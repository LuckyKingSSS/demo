#ifndef FSRECORDERMENUBAR_H
#define FSRECORDERMENUBAR_H

#include <QWidget>
#include "inc_FSCommonlib/FSCommonWidget.h"


class QMenuBar;
class FSRecorderMenuBar : public FSBaseWidget
{
	Q_OBJECT

public:
	FSRecorderMenuBar(QWidget *parent);
	~FSRecorderMenuBar();
	void RetranslateUi();

Q_SIGNALS:
	void sigStart();
	void sigStop();
	void sigClose();
	void sigEditor();

protected:
	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void Init();
	void ConnectSignals();
private:
	QMenuBar *m_pMenuBar;
	QMenu *m_pMenuCapture;
	QMenu *m_pMenuSettings;
	QMenu *m_pMenuHelp;
	QMenu *m_pMenuLang;

	QAction *m_pActStart;
	QAction *m_pActStop;
	QAction *m_pActClose;

	QAction *m_pActLang_Eng;
	QAction *m_pActOptions;

	QAction *m_pActSupportCenter;
	QAction *m_pActAbout;
	QAction *m_pActEditor;
};


class FSTitleBar : public FSBaseWidget
{
	Q_OBJECT

public:
	FSTitleBar(QWidget *parent);
	~FSTitleBar();

	FSRecorderMenuBar *GetMenuBar();
	void SetMoveWidget(QWidget *pWidget);
Q_SIGNALS:
	void sigClosed();
	void sigMinimize();

protected:
	

	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
private:
	FSRecorderMenuBar *m_pMenuBar;

	QPushButton *m_pBtnMin;
	QPushButton *m_pBtnExit;
	QWidget *m_pMoveWidget;

	QPoint m_ptPress;
	QPoint m_ptPosWhenPress;
};

#endif // FSRECORDERMENUBAR_H
