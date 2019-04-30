#pragma once


// CCabinetDlg 对话框

class CCabinetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCabinetDlg)

public:
	CCabinetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCabinetDlg();

// 对话框数据
	enum { IDD = IDD_CABINETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
