#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "SnmpObj.h"
#include "afxwin.h"
// CEmsAtsDlg 对话框

class CEmsAtsDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmsAtsDlg)

public:
	CEmsAtsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmsAtsDlg();

// 对话框数据
	enum { IDD = IDD_EMSATS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CMyListCtrl m_list;
	CImageList	m_cImageListNormal32;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawListFrm(CDC *pDC);
	void drawDevStaus(CDC *pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int drawEMS(CDC *pDC);
	void drawATS(CDC *pDC);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void updateList(void);
	HTREEITEM ems;
	HTREEITEM ats;
	HTREEITEM ippdu;
	void insertList(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	HTREEITEM checkList(HTREEITEM hitem,CString name,int flags);
	int normal;
	int offline;
	int alarm;
	void insert_m_list(void);
	void updateView(void);
	CSnmpObj *p_select;
	int index;
	int Slave;
	int SelectIP;
	void drawEMSdata(CDC *pDC);
	void drawATSdata(CDC *pDC);
	void drawIPdata(CDC *pDC);
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEmsatsReleasealarm();
	//afx_msg void OnEmsatsAddems();
	afx_msg void OnEmsatsAddats();
	afx_msg void OnEmsatsDelete();
	afx_msg void OnEmsatsSet();
	afx_msg void OnEmsatsAddemsT();
	//afx_msg void OnEmsatsAddatsT();
	afx_msg void OnEmsatsDeletedevT();
	afx_msg void OnEmsatsDelalarmT();
	afx_msg void OnAlarmEms();
	afx_msg void OnEmsAdd();
	afx_msg void OnAtsAdd();
	afx_msg void OnEmsDelete();
	afx_msg void OnEmsSet();
	afx_msg void OnEmsatsEdit();
	afx_msg void OnEmsChange();
	afx_msg void OnIpPdu();
	afx_msg void OnIpDel();
	afx_msg void OnIpSet();
	afx_msg void OnIpEdit();
	afx_msg void OnIpAlarm();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
