#ifndef MVLISTITEMDATA_H
#define MVLISTITEMDATA_H

#include <QObject>
#include "inc_FSCommonlib/ListItemDataBase.h"

#include "MVListItemData.h"

enum MVItemDataType
{
	MVItemDataType_AddNewItem = Global_AddNewItemType,
	MVItemDataType_Record = 1,
	MVItemDataType_Video,
	MVItemDataType_Audio,
	MVItemDataType_Imgage,
	MVItemDataType_Annotation,
	MVItemDataType_CursorHightline,
	MVItemDataType_ClickRing,
	MVItemDataType_ClickSound,
};


class MediaData : public ItemDataBase
{
	Q_OBJECT
public:
	MediaData(MVItemDataType type, QObject *parent = 0);
	void SetSourceFile(const QString &file);
	QString GetSourceFile();
protected:
	QString m_strSourceFile;
};

class VideoData : public MediaData
{
	Q_OBJECT
public:
	VideoData(QObject *parent = 0);
};

class AudioData : public MediaData
{
	Q_OBJECT
public:
	AudioData(QObject *parent = 0);
};

class ImageData : public MediaData
{
	Q_OBJECT
public:
	ImageData(QObject *parent = 0);
};

class RecordData : public MediaData
{
	Q_OBJECT
public:
	RecordData(QObject *parent = 0);
};

class AnnotationData;
class AnnotationDataHelper;
class AnnotationItemData : public MediaData
{
	Q_OBJECT
public:
    AnnotationItemData(AnnotationData *data, AnnotationDataHelper *helper, QObject *parent = 0);

    QString GetCatalog();
    QString GetAlias();

    FSNLETimeline* GetTimeline() override;

    AnnotationDataHelper *GetHelper();
    AnnotationData* GetData();
private:
    AnnotationData *m_data;
    AnnotationDataHelper *m_helper;
};
typedef QList<AnnotationItemData*> AnnotationItems;

class CursorHightlineData : public ItemDataBase
{
	Q_OBJECT
public:
	CursorHightlineData(QObject *parent = 0);

};

class ClickRingData : public ItemDataBase
{
	Q_OBJECT
public:
	ClickRingData(QObject *parent = 0);

};

class ClickSoundData : public ItemDataBase
{
	Q_OBJECT
public:
	ClickSoundData(QObject *parent = 0);
	QString GetSoundFile();
	void SetSoundFile(const QString &file);
protected:
	QString m_strSoundFile;
};

class AnnotationData
{
public:
	QString guid;
	int type;
	QString catalog;
    QString alias;
    QString resfile;
    QString thumbnailfile;
    QString displayName;
	
};
typedef QList<AnnotationData*> AnnotationDatas;

class CategoryData
{
public:
	int type;
	QString text;
	std::string untrtext;
};
class AnnotationDataHelper : public QObject
{
    Q_OBJECT
public:
    AnnotationDataHelper(QObject *parent);

    QList<std::string> GetCatagoryStrings();
    AnnotationItems GetItems();
    AnnotationItems GetItems(QString catalog);
    bool BelongCatalog(AnnotationItemData *item, QString catalog);
    bool IsFavorite(AnnotationItemData *item);
    QString GetFavoriteString();

    void AddItem2Favorite(AnnotationItemData *item);
    void RemoveItemFromFavorite(AnnotationItemData *item);


    FSNLETimeline* CreateFSNLETimeline(AnnotationItemData *item);

    bool Load();
    bool Save();

	void RetranslateText();
private:
	bool LoadCategorys(QString path);
    bool LoadAnnotation(QString path);
    bool LoadOverlay(QString path);
	bool LoadElement(QString path);

    bool LoadItemData(QString catalog, AnnotationData *data);

	void AddCatagory(CategoryData catalog);
	QString GetCatagoryString(int type);
    void LoadFavorite();

private:
    //AnnotationDatas m_datas;
    AnnotationDatas m_datas;
    AnnotationItems m_items;
	QList<CategoryData> m_catalogs;
    AnnotationItems m_favorites;
    QString m_favoriteString;

};

#endif // MVLISTITEMDATA_H
