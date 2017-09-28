#include "inc_CommonLib/nlebasewidget.h"



static NLEBaseWidget::NLEBaseWidgetList g_NLEBaseWidgets;
NLEBaseWidget::NLEBaseWidget(QWidget *parent)
:QWidget(parent)
{
	setObjectName("NLEBaseWidget");
	g_NLEBaseWidgets.push_back(this);
}

void NLEBaseWidget::SetFocus(bool foucs)
{

}

void NLEBaseWidget::MouseMove(QPoint p)
{

}

NLEBaseWidget::NLEBaseWidgetList NLEBaseWidget::GetNLEBaseWidgets()
{
	return g_NLEBaseWidgets;
}

//////////////////////////////////////////////////////////////////////////

#include <QtWidgets/QShortcut>
#include <QtCore/QVariant>
#include "inc_CommonLib/externalapi.h"

static NLEGlobalShortcut* g_NLEGlobalShortcut = nullptr;
NLEGlobalShortcut* NLEGlobalShortcut::GetInstance()
{
	
	if (g_NLEGlobalShortcut == nullptr)
	{
		g_NLEGlobalShortcut = new NLEGlobalShortcut(gGetMainWindow());
	}
	return g_NLEGlobalShortcut;
}

//void NLEGlobalShortcut::Release()
//{
//	if (g_NLEGlobalShortcut)
//	{
//		delete g_NLEGlobalShortcut;
//		g_NLEGlobalShortcut = nullptr;
//	}
//}

NLEGlobalShortcut* NLEGlobalShortcut::CreateInstance(QWidget *parent)
{
	return new NLEGlobalShortcut(parent);
}




NLEGlobalShortcut::NLEGlobalShortcut(QWidget *parent)
:QObject(parent)
{

	const char *id = "id";
	// 回车
	QShortcut *sc =  new QShortcut(QKeySequence(Qt::Key_Space), parent);
	sc->setProperty(id, Key_Space);
	m_shortcuts << sc;

	sc = new QShortcut(QKeySequence(Qt::Key_Delete), parent);
	sc->setProperty(id, Key_Delete);
	m_shortcuts << sc;

	sc = new QShortcut(QKeySequence(Qt::Key_C + Qt::ControlModifier), parent);
	sc->setProperty(id, Key_Copy);
	m_shortcuts << sc;

	sc = new QShortcut(QKeySequence(Qt::Key_X + Qt::ControlModifier), parent);
	sc->setProperty(id, Key_Cut);
	m_shortcuts << sc;

	sc = new QShortcut(QKeySequence(Qt::Key_V + Qt::ControlModifier), parent);
	sc->setProperty(id, Key_Pasete);
	m_shortcuts << sc;

	for (auto shortcut : m_shortcuts)
	{
		shortcut->setContext(Qt::ApplicationShortcut);
	}

	for (auto shortcut : m_shortcuts)
	{
		connect(shortcut, &QShortcut::activated, this, [&](){
			this->actived(sender()->property("id").toInt());
		});
	}

}
void NLEGlobalShortcut::AddShortcut(QKeySequence key, int id)
{
	// id不能存在
	for (auto shortcut : m_shortcuts)
	{
		Q_ASSERT(shortcut->property("id").toInt() != id);
	}

	auto sc = new QShortcut(key, qobject_cast<QWidget*>(parent()));
	sc->setProperty("id", id);
	sc->setContext(Qt::ApplicationShortcut);
	m_shortcuts << sc;
	connect(sc, &QShortcut::activated, this, [&](){
		this->actived(sender()->property("id").toInt());
	});
}

void NLEGlobalShortcut::SetShortcutEnabled(int id, bool enable)
{
	for (auto shortcut : m_shortcuts)
	{
		if (shortcut->property("id").toInt() == id)
		{
			shortcut->setEnabled(enable);
			bool ddd = shortcut->isEnabled();
			break;
		}
	}
}

void NLEGlobalShortcut::EnableShortcuts(bool bEnable)
{
	for (auto shortcut : m_shortcuts)
	{
		shortcut->setEnabled(bEnable);
	}
}
