#pragma once


// CDatacenterInfo 对话框

class CDatacenterInfo : public CDialog
{
	DECLARE_DYNAMIC(CDatacenterInfo)

public:
	CDatacenterInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatacenterInfo();

// 对话框数据
	enum { IDD = IDD_DATACENTER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawbar(CDC *pDC,int index);
	void drawinline(CDC *pDC,int index);
	void drawTH(CDC *pDC,int index);
	CString DCname;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
