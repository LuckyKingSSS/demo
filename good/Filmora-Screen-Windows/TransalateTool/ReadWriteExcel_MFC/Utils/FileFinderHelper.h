#pragma once
#include <string>
#include <vector>

using namespace std;
class CFileFinderHelper
{
public:
	CFileFinderHelper();
	~CFileFinderHelper();

public:
	//查找特定路径下，特定格式的文件的集合
	static void GetAllFormatFiles(string path, vector<string>& files, string format);
	//获取当前解决方案中所有的ts文件并拷贝到一个特定的文件夹下
	/*  wchar_t* lpPath                     --需要查找的路径
    //  std::vector<std::string> &fileList  --查询的结果
	//  wchar_t* strFileType                --过滤条件，查找文件类型（.exe .cpp .ts ...）
	*/
	static void find(const wchar_t* lpPath, std::vector<std::string> &fileList, wchar_t* strFileType);
    //根据文件明获取对应的文件类型（文件后缀）
	static string getFileType(wstring strFileName);

	//获取当前主程序所在的路径
	static wstring  GetAppPath();
	static bool IsFileExsit(string strFilePath);
};

