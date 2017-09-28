#ifndef FSMATTERVIEW_H
#define FSMATTERVIEW_H

#include "fsmatterview_global.h"
#include <QtWidgets/QtWidgets>
#include "inc_FSCommonlib/FSCommonWidget.h"

class FSMatterViewPrivate;
class IFSMatterViewManager;
class FSNLETimeline;
class FSMATTERVIEW_EXPORT FSMatterView : public FSBaseWidget
{
	Q_OBJECT
public:
	FSMatterView(QWidget *parent = 0);
	void AddRecordFils(QStringList files);
	IFSMatterViewManager *GetMatterViewManager();
protected:
	void resizeEvent(QResizeEvent *e) override;
private:
	FSMatterViewPrivate *m_p;
};

class FSMATTERVIEW_EXPORT IFSMatterViewManager : public QObject
{
	Q_OBJECT
public:
	virtual void ImportMediaFiles() = 0;
	virtual void ImportMediaFolder() = 0;
	virtual void UnselectAll() = 0;

	virtual bool CanDelete() = 0;
	virtual void Delete() = 0;

	virtual QString GetProjectString() = 0;
	virtual void LoadProjectString(QString strMatter) = 0;
	virtual void Clear() = 0;

	virtual QPixmap Get1stItemPixmap() = 0;
Q_SIGNALS :
	void sigLoad(FSNLETimeline *timeline);
	void sigAdd(FSNLETimeline *timeline);
	void sigUnselectTimelineView();
	void sigContentChanged(bool bChanged);
	void sigMediaSelectChanged();
	void sigFocusIn();
	void sigDisplayNameChanged(QString ancestralGuid, QString displayname);
    void sigShowMaximum(bool showMaximum);
	void sigMouseEffectDraging(bool bDragin);
};

#endif // FSMATTERVIEW_H
