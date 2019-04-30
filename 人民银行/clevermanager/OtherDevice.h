#pragma once


// COtherDevice 对话框

class COtherDevice : public CDialog
{
	DECLARE_DYNAMIC(COtherDevice)

public:
	COtherDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COtherDevice();

// 对话框数据
	enum { IDD = IDD_OTHER_DEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int type;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
