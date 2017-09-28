#ifndef UPDATEREGISTER_GLOBAL_H
#define UPDATEREGISTER_GLOBAL_H

#ifdef WIN32

#ifdef UPDATEREGISTER_LIB
#define UPDATEREGISTER_EXPORT __declspec(dllexport)
#else
#define UPDATEREGISTER_EXPORT __declspec(dllimport)
#endif

#else
#define UPDATEREGISTER_EXPORT  
#endif


class UPDATEREGISTER_EXPORT UpdateRegisterInit
{
public:
	//设置配置文件的所在目录。只需指定目录，不指定具体文件名
	static void SetConfigFileDir(QString dir);
	static QString GetConfigFileDir();
	static QString gGetStyleSheet(QString path);
};


#endif // UPDATEREGISTER_GLOBAL_H
