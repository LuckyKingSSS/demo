#include "stdafx.h"
#include "inc_FSCommonlib/MediaImportDlg.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_FSCommonlib/gathread.h"


MediaImportDlg::MediaImportDlg(const QStringList &list, QWidget *parent)
	: FSBaseDialog(tr("Filmora Scrn"),"Filmora Scrn",parent)
{
    setObjectName("MediaImportDlg");
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m_titlebar->setObjectName("MatterView_MediaImportDlg_TitleBar");
	m_titlebar->SetSystemBorderVisible(false);
	m_titlebar->SetResizeble(false);
	m_titlebar->SetDragMaximum(false);
	m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");
    m_titlebar->GetControl()->SetBorderColor(QColor(102,102,102));
    m_titlebar->GetControl()->SetBorderWidth(1);
	m_pLbName = new QLabel(this);
	m_pLbName->setObjectName("MatterView_MediaImportDlg_NameLabel");
	m_pLbProgress = new QLabel(this);
    m_pLbProgress->setAlignment(Qt::AlignRight);
	m_pLbProgress->setObjectName("MatterView_MediaImportDlg_ProgressLabel");
	m_pProgress = new QProgressBar(this);
	m_pProgress->setObjectName("MatterView_MediaImportDlg_ProgressBar");
	m_pProgress->setTextVisible(false);
	m_pProgress->setRange(0, 100);
	m_pProgress->setValue(0);
	m_pBtnCancel = new QPushButton(tr("Cancel"), this);
    m_pBtnCancel->setObjectName("MatterView_MediaImportDlg_CancelButton");
	m_pImportHelper = new ImportHelper(list);

	connect(m_pImportHelper, &ImportHelper::sigName, this, [&](QString name){
		m_pLbName->setText(name);
	},Qt::QueuedConnection);
	connect(m_pImportHelper, &ImportHelper::sigImportProgress, this, [&](int cur,int total){
		m_pLbProgress->setText(QString("%1/%2").arg(cur).arg(total));
		if (total != 0) m_pProgress->setValue(1.0 * cur / total * 100);
	}, Qt::QueuedConnection);
	connect(m_pImportHelper, &ImportHelper::sigImported, this, [&](QString srcfile, FSNLETimeline *timeline){
		emit sigImported(srcfile, timeline);
	}, Qt::QueuedConnection);
	connect(m_pImportHelper, &ImportHelper::sigFinish, this, [&]{
		done(0);
	}, Qt::QueuedConnection);
	connect(m_pBtnCancel, &QPushButton::clicked, this, [&]{
		m_pImportHelper->Cancel();
		done(1);
	}, Qt::QueuedConnection);
	setFixedSize(660, 242);
}

MediaImportDlg::~MediaImportDlg()
{
	m_pImportHelper->deleteLater();
}

int MediaImportDlg::DoImport()
{ 
	//启动导入的时候开始计时
	GAThread::getInstance()->startTime();
	m_pImportHelper->StartImport();
    int iRec = exec();
	int iCount = GAThread::getInstance()->getElapse();
	GAThread::getInstance()->insertEventItem(ImportTimer, "ImportFileTime", QString::number(iCount));
	return iRec;
}


void MediaImportDlg::SetRange(int minvalue, int maxvalue)
{
    m_pProgress->setRange(minvalue, maxvalue);
}

void MediaImportDlg::resizeEvent(QResizeEvent *event)
{
	QRect rcAll = rect();
	const int margins = 3;

	int widthWidth = width() - 2 * margins;

	m_titlebar->setGeometry(margins, margins, widthWidth, 34);
	int top = m_titlebar->geometry().bottom() + 61;
	const int btnWidth = 130;
	const int space = 20;
	int left = space;

    m_pLbName->setGeometry(left, top, width() - btnWidth - 2 * space - 10, 20);
	left = m_pLbName->geometry().right() +2;

    m_pLbProgress->setGeometry(left, top, btnWidth, 20);

    top = m_pLbProgress->geometry().bottom() +27;
	left = space;

	m_pProgress->setGeometry(space, top, width() - 2 * space, 4);
	m_pBtnCancel->setGeometry(width()-space-100, height()-66, 100, 40);
}


void MediaImportDlg::closeEvent(QCloseEvent *e)
{
	m_pImportHelper->Cancel();
	FSBaseDialog::closeEvent(e);
}

//////////////////////////////////////////////////////////////////////////
ImportHelper::ImportHelper(const QStringList &list, QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bCanceled = false;
	m_lstImportFiles = list;
	m_pThread = new QThread(this);
	moveToThread(m_pThread);
	m_pThread->start();
}

void ImportHelper::StartImport()
{
	QMetaObject::invokeMethod(this, "_Import", Qt::QueuedConnection);
}

void ImportHelper::Cancel()
{
	m_bCanceled = true;
}

void ImportHelper::_Import()
{
	int nCnt = m_lstImportFiles.size();
	emit sigImportProgress(0, nCnt);
	int i = 0;
	for (auto one : m_lstImportFiles)
	{
		if (m_bCanceled)
			break;
		++i;
		emit sigName(GetName(one));
		auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateTimelineForScreenRecording(one);

		if (!timeline) continue;
		FSNLETimeline *fstimeline = new FSNLETimeline(timeline);


		emit sigImported(one, fstimeline);
		emit sigImportProgress(i, nCnt);
	}
	emit sigFinish();
}

QString ImportHelper::GetName(QString path)
{
	QFileInfo file(path);
	return file.fileName();
}
