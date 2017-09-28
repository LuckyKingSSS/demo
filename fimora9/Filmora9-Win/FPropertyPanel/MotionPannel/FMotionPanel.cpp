#include "stdafx.h"
#include <QListWidget>
#include "..\include\inc_MediaPlayerView\MediaPlayerView.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include "ExternalAPI.h "
#include "FMotionPanel.h"
#include "FMotionPannelMgr.h"
#include "FMotionListItemWidget.h"
#include "PIPEditDialog/FPIPEditDialog.h"
#include "NLEWraper.h"
#include "inc_FProject/FNLETimelineFactory.h"

FMotionPanel::FMotionPanel(QWidget *parent)
	: FPropertyPanel(parent)
{
	
	Init();

}

FMotionPanel::~FMotionPanel()
{

}

void FMotionPanel::Init()
{
	InitUI();
	InitData();
	InitConnections();
}

void FMotionPanel::Show()
{

}

void FMotionPanel::Hide()
{

}

void FMotionPanel::SetEnabled(bool enabled)
{
	m_ListWidget->setEnabled(enabled);
	m_AdvanceBtn->setEnabled(enabled);
}

void FMotionPanel::Destory()
{

}

void FMotionPanel::Update()
{

}

void FMotionPanel::resizeEvent(QResizeEvent *event)
{
	//m_Player->resize(200, 200);
}

void FMotionPanel::onPlayMotion(QString strFilePath)
{
	NLEComPtr<INLETimelineBase> pTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline(strFilePath);
	Q_ASSERT(pTimeline);
	//FNLETimeline* timeline = new FNLETimeline(pTimeline);
    auto timeline = FNLETimelineFactory::GetInstance()->CreateTimeline(pTimeline);

	NLEPropComPtr pTimelineProp = timeline->GetProp();
	Q_ASSERT(pTimelineProp->Self() != nullptr);

	QString strValue = "{67F7E753-4CD4-4F7C-87CD-E88F72622BC0}@G:\\Code\\Fimora9\\Filmora9-Win\\bin\\x64\\Release\\MotionTemplate\\MotionTemplate.xml";
	std::wstring wstrValue = strValue.toStdWString();
	pTimelineProp->SetString(NLEKey::Motion::kMotion_URI, wstrValue.c_str());
}

void FMotionPanel::InitUI()
{
	m_AdvanceBtn = new FButton(nullptr);
	m_AdvanceBtn->setText(tr("Advance"));
	m_OkBtn = new FButton(nullptr);
	m_OkBtn->setText(tr("OK"));
	m_ListWidget = new QListWidget(nullptr);

	m_ListWidget->setViewMode(QListWidget::IconMode);
	m_ListWidget->setIconSize(QSize(105, 80));
	m_ListWidget->setResizeMode(QListWidget::Adjust);
	m_ListWidget->setMovement(QListView::Static);
	m_ListWidget->setLayoutMode(QListView::Batched);

	m_ListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ListWidget->setDragEnabled(false);
	m_ListWidget->setSpacing(10);
	m_ListWidget->setContentsMargins(15, 0, 20, 0);

	//m_ListWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	//m_ListWidget->setModelColumn(2);
	//m_ListWidget->setWrapping(TRUE);
	m_ListWidget->setLineWidth(0);
	m_ListWidget->setMouseTracking(true);


	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_ListWidget);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addStretch();
	hLayout->addWidget(m_AdvanceBtn);
	hLayout->addWidget(m_OkBtn);
	hLayout->setContentsMargins(0,0,0,0);
	hLayout->setSpacing(5);
	vLayout->addLayout(hLayout);
	vLayout->setContentsMargins(6,2,6,2);
	vLayout->setSpacing(2);
	setLayout(vLayout);
}

void FMotionPanel::InitData()
{
	m_PannelMgr = new FMotionPannelMgr(nullptr);
	QString strFilePath =  "G:\\Code\\Filmora-Screen-Windows\\bin\\x64\\Release\\MotionTemplate";
	QList<pMotionInfo> allInfo;
	allInfo =  m_PannelMgr->GetAllResource(strFilePath);

	for (int i = 0; i < allInfo.length();++i)
	{
		QListWidgetItem* item = new QListWidgetItem();
		//固定每一个项的大小
		item->setSizeHint(QSize(105, 80));
		m_ListWidget->addItem(item);
		FMotionListItemWidget* itemWidget = new FMotionListItemWidget(nullptr,"");
		itemWidget->SetLabelText(tr("LuckyKing"));
		m_ListWidget->setItemWidget(item, itemWidget);
		itemWidget->setMouseTracking(true);

	}
	m_Player = new MediaPlayerView(m_ListWidget);
	m_Player->SetBorderWidth(0);
	m_Player->SetBorderColor(QColor(255,0,0));
}

void FMotionPanel::InitConnections()
{
	connect(m_ListWidget, &QListWidget::itemEntered, this, [&](QListWidgetItem* item){
    //播放
		/*QRect rect = m_ListWidget->visualItemRect(item);
		m_Player->setGeometry(QRect(rect.left(), rect.top(), rect.right() - rect.left(), rect.bottom() - 15));*/
		const int iBorderWidth = 2;
		QWidget* curWidget = m_ListWidget->itemWidget(item);
		QRect rect = ((FMotionListItemWidget*)curWidget)->GetIconWidgetSize();
		QRect listItemRect = m_ListWidget->geometry();
		rect.setLeft(rect.left() + listItemRect.left() - iBorderWidth);
		rect.setTop(rect.top() + listItemRect.top() + iBorderWidth);
		m_Player->setGeometry(QRect(rect)); 
		//



	});

	connect(m_AdvanceBtn, &FButton::clicked, this, [&](){
	      //打开advance编辑界面
		FPIPEditDialog* pipDlg = new FPIPEditDialog(this);
		pipDlg->SetTimeline(GetTimeline());
		pipDlg->DoModal();
	});

	connect(m_OkBtn, &FButton::clicked, this, [&](){
		emit sigConfirmSetting();
	});
}
