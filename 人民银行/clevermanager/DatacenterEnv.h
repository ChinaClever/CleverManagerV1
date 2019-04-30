#pragma once


// CDatacenterEnv 对话框

class CDatacenterEnv : public CDialog
{
	DECLARE_DYNAMIC(CDatacenterEnv)

public:
	CDatacenterEnv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatacenterEnv();

// 对话框数据
	enum { IDD = IDD_DATACENTER_ENV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int position[20][20];
	void drawColor(CDC *pDC);
	//void drawdelay(CDC *pDC);
	//bool delay;
	Color CDatacenterEnv::setcolor(int value);
	void drawflags(CDC *pDC);
	virtual BOOL OnInitDialog();
	CString DCname;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
