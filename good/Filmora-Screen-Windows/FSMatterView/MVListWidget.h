#ifndef MVLISTWIDGET_H
#define MVLISTWIDGET_H


#include "MVListItemData.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/ListWidgetBase.h"

class MediaItemWidget : public MVItemWidgetBase
{
	Q_OBJECT
public:
	MediaItemWidget(QWidget *parent);
	void SetThumbnail(QPixmap px);
	void Resize();
Q_SIGNALS:
	void sigSourceFileLost(MVItemWidgetBase* item,QString file);
	void sigDrag(MVListItemData *pdata);
protected:
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;

	void mousePressEvent(QMouseEvent *e) override;
    virtual void RetranslateUi();

	bool CheckSourceFile();
};

class AnnotationItemWidget : public MediaItemWidget
{
	Q_OBJECT
public:
    AnnotationItemWidget( QWidget *parent);
	void Drag() override;

    void ShowFavorite(bool show, bool hadFavorite);

Q_SIGNALS:
    void sigMenuPopup(QPoint point);
    void sigAddFavorite();
    void sigRemoveFavorite();

protected:
	void resizeEvent(QResizeEvent *e) override;
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    BaseStateButton *m_favorite;
};

class MouseEffectItemWidget : public MVItemWidgetBase
{
	Q_OBJECT
public:
	MouseEffectItemWidget(QWidget *parent);
	void Drag() override;
protected:
	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
};


class VideoItemWidget : public MediaItemWidget
{
	Q_OBJECT
public:
	VideoItemWidget(QWidget *parent);
	void Drag() override;
};

class AudioItemWidget : public MediaItemWidget
{
	Q_OBJECT
public:
	AudioItemWidget(QWidget *parent);
	void Drag() override;
};

class ImageItemWidget : public MediaItemWidget
{
	Q_OBJECT
public:
	ImageItemWidget(QWidget *parent);
	void Drag() override;
};

class RecordItemWidget : public MediaItemWidget
{
	Q_OBJECT
public:
	RecordItemWidget(QWidget *parent);
	void Drag() override;
};


#endif // MVLISTWIDGET_H
