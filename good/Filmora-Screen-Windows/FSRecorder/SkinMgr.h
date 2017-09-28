#pragma once
class SkinMgr
{
public:
	enum SKIN_TYPE
	{
		SKIN_TYPE_LIGHT = 0,
		SKIN_TYPE_DARK,
	};

	static SkinMgr* GetInstance();
	virtual ~SkinMgr();

	void SetSkin(SKIN_TYPE type);
	void SetInitType(SKIN_TYPE type);
	SKIN_TYPE GetInitType();

private:
	SkinMgr();
	QMap<SKIN_TYPE, QString> m_ResMap;
	SKIN_TYPE m_type;
};

