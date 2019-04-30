#pragma once
#include "afxdtctl.h"
#include "afxcmn.h"
#include "MyListCtrl.h"

// CLogsDlg 对话框

class CLogsDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogsDlg)

public:
	CLogsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogsDlg();

// 对话框数据
	enum { IDD = IDD_LOGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDateTimeCtrl m_time;
	CMyListCtrl m_list;
	CImageList	m_cImageListNormal32;
	//void writeLogs(int logtype,int type,CString name, CString item, CString detail);
	void readToList(int logtype);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CTreeCtrl m_tree;
	CImageList	m_cImageListTree;
	void treeinsert(void);
	HTREEITEM findtree(HTREEITEM para,int title);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void treeloginsert(CString filename,int logtype);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int refreshlisttimer;
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
