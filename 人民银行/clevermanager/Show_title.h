#pragma once


// CShow_title 对话框

class CShow_title : public CDialog
{
	DECLARE_DYNAMIC(CShow_title)

public:
	CShow_title(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShow_title();

// 对话框数据
	enum { IDD = IDD_SHOW_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
