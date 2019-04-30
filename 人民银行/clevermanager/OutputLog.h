#pragma once
#include "afxdtctl.h"


// COutputLog 对话框

class COutputLog : public CDialog
{
	DECLARE_DYNAMIC(COutputLog)

public:
	COutputLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutputLog();

// 对话框数据
	enum { IDD = IDD_OUTPUTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int zhang(CString str1 , CString str2 , CString str3);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_time2;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
