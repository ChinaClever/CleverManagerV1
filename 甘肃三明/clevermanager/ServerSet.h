#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CServerSet 对话框

class CServerSet : public CDialog
{
	DECLARE_DYNAMIC(CServerSet)

public:
	CServerSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerSet();

// 对话框数据
	enum { IDD = IDD_SERVER_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawuserform(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_list;
	CComboBox m_mailCom;
	void drawBtn(CDC* pDC,int x,int y,int w,int h,int icon,CString name);
	CComboBox m_webCom;
	virtual BOOL OnInitDialog();
	CImageList	m_cImageListNormal32;
	CComboBox m_userEdit;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNMRclickUserList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickUserList(NMHDR *pNMHDR, LRESULT *pResult);
	static UINT webstart(LPVOID p);
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeUserDetail();
};
