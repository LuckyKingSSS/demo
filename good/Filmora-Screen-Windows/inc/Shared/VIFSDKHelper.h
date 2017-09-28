#pragma once
class CVIFSDKHelper
{
protected:
	CVIFSDKHelper(LPCWSTR pDir, LPCWSTR pLogDir);
	~CVIFSDKHelper();
public:
	static CVIFSDKHelper* GetVIFSDKHelper(LPCWSTR pDir);
	
	
private:
	static CVIFSDKHelper* m_pSDKHelperInstance;
};

