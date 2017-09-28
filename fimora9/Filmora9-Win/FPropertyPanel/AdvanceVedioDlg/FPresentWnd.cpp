#include "stdafx.h"
#include "FPresentWnd.h"
#include <QStringList>
#include "FPresentWndMgr.h"
#include <QBoxLayout>

FPresentWnd::FPresentWnd(QWidget *parent)
	: FBaseWidget(parent)
{
	initUI();
	initData();
}

FPresentWnd::~FPresentWnd()
{

}

void FPresentWnd::slotFrameChanged(QPixmap curPicture)
{

}

void FPresentWnd::initUI()
{
	m_ColorTypeCombo = new FComboBox(this);
	m_ColorList = new QListWidget(this);
	
	m_ColorList->setViewMode(QListWidget::IconMode);
	m_ColorList->setIconSize(QSize(100, 56));
	m_ColorList->setResizeMode(QListWidget::Adjust);
	m_ColorList->setMovement(QListView::Static);
	m_ColorList->setLayoutMode(QListView::Batched);

	m_ColorList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ColorList->setDragEnabled(false);
	//m_ColorList->setSpacing(20);
	m_ColorList->setSpacing(10);
	m_ColorList->setContentsMargins(15, 0, 20, 0);
	//m_ColorList->setContentsMargins(0, 0, 0, 0);

	m_ColorList->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	m_ColorList->setModelColumn(2);
	m_ColorList->setWrapping(TRUE);
	m_ColorList->setFrameShape(QFrame::NoFrame);

	m_ColorTypeCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_ColorTypeCombo->setFixedSize(185, 20);
	QVBoxLayout* vLayout = new QVBoxLayout();
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setContentsMargins(0, 10, 0, 0);
	hLayout->addWidget(m_ColorTypeCombo);
	vLayout->addLayout(hLayout);
	vLayout->addWidget(m_ColorList);
	vLayout->setContentsMargins(0 ,0, 0, 0);
	setLayout(vLayout);
}

void FPresentWnd::initData()
{
	m_DataMgr = new FPresentWndMgr(nullptr);
	QStringList strList = m_DataMgr->GetTotalColors();
	for (int i = 0; i < strList.length(); ++i)
	{
		insertItem(strList.at(i));
	}
	QStringList strTypelist;// = { "All Presets", "Custom", "Favourite", "Common", "Film", "Vignette", "3DLUT" };
	strTypelist << "All Presets" << "Custom" << "Favourite" << "Common" << "Film" << "Vignette" << "3DLUT";
	m_ColorTypeCombo->addItems(strTypelist);
}

void FPresentWnd::insertItem(QString strColorType)
{
	QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap(":/FPropertyPanel/Resources/screen.png")), strColorType);
	//固定每一个项的大小
	item->setSizeHint(QSize(105, 80));
	m_ColorList->addItem(item);
}