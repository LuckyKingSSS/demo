#pragma once
#include "afxcmn.h"

class ImageManager;
class CPictureEx;
class CMyProgressCtrl :
    public CProgressCtrl
{
public:
    CMyProgressCtrl();
    ~CMyProgressCtrl();
    int SetPos(int nPos);
    void SetRange(int nLower, int nUpper);
    void setText(CString strText){ m_strText = strText; };
    void initData();

private:
    CImage m_ProgressGif; //显示已经处理完成部分的gif图片
    COLORREF m_prgsColor; //进度条进度部分颜色
    COLORREF m_freeColor;//进度条后面空余部分颜色
    COLORREF m_prgsTextColor; //进度部分字体颜色
    COLORREF m_freeTextColor; //空白部分字体颜色
    int  m_iMin;    //进度条的最小值，通常是0
    int  m_iMax;    //进度条的最大值，通常是100
    int  m_iPos;    //当前的进度
    int  m_nBarWidth;  //进度条宽度
    CString m_strText;//当前进度值
	//ImageManager* gifManager;   //gif图片处理类
	CPictureEx*  m_gifHelper;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

