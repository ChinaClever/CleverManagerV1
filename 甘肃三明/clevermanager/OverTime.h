#pragma once


// COverTime 对话框

class COverTime : public CDialog
{
	DECLARE_DYNAMIC(COverTime)

public:
	COverTime(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COverTime();

// 对话框数据
	enum { IDD = IDD_OVERTIME};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
