#ifndef MVGLOBAL_H
#define MVGLOBAL_H

#include <QObject>

class MVGlobal : public QObject
{
	Q_OBJECT

public:
	static MVGlobal* GetInstance();

	void ImportMediaItems(QStringList files);

Q_SIGNALS:
	void sigImportMediaItems(QStringList files);
private:
	MVGlobal(QObject *parent = 0);

private:
	bool m_bConfigChanged;
	
};

#endif // MVGLOBAL_H
