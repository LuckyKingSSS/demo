#pragma once
#include <QObject>
#include "inc_MKCollecter/MKCollecter.h"
class MKCollecterPrivate : public MKCollecter
{
	Q_OBJECT
public:
	MKCollecterPrivate();
	~MKCollecterPrivate();

	bool SetMouseHook(bool bInstall);
	bool SetKeyboardHook(bool bInstall);
    bool GetMouseHook();
    bool GetKeyboardHook();

	Q_INVOKABLE bool _SetMouseHook(bool bInstall);
	Q_INVOKABLE bool _SetKeyboardHook(bool bInstall);


	QString GetHotKeyString() override;

	void EnalbedHotKeyDeliver(bool enabled) override;
	bool GetEnalbedHotKeyDeliver() override;

	QString GetKeyString(int key) override;
	QString GetControlString(int mk);

	void SetKey(int key, int mk);

	void AddHotKey(QString key) override;
	void ClearHotKey() override;
	void RemoveHotKey(QString key) override;
	QList<QString> GetHotKeys();
private:
	int m_key;
	int m_mk;

	QList<QString> m_hotkeys;

	bool m_enalbedHotKeyDeliver;
};

