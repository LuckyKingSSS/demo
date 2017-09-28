#ifndef FMATTERVIEWMGR_H
#define FMATTERVIEWMGR_H

#include <QObject>
#include "FMatterType.h"

class FMediaData;

class FMatterViewMgr : public QObject
{
	Q_OBJECT

public:
	FMatterViewMgr(QObject *parent = 0);
	~FMatterViewMgr();

	void Load(FMediaData*);
	void SetMatterType(MatterType);
	// 返回分类名，以及分类下的资源个数
	QList<QPair<QString, int>>* GetCategory();

private:
	void InitMediaCategory();

private:
	MatterType m_MatterType;
	FMediaData *m_pMediaData;
	QList<QPair<QString, int>> m_MediaCategory;
	QList<QPair<QString, int>> m_MusicCategory;
	QList<QPair<QString, int>> m_TextCategory;
	QList<QPair<QString, int>> m_TransitionCategory;
	QList<QPair<QString, int>> m_FilterCategory;
	QList<QPair<QString, int>> m_OverlayCategory;
	QList<QPair<QString, int>> m_ElementCategory;
	QList<QPair<QString, int>> m_SplitScreenCategory;
};

#endif // FMATTERVIEWMGR_H
