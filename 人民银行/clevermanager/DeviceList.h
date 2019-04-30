#pragma once
#include "afxcmn.h"
#include "SnmpObj.h"
#include "MyListCtrl.h"
// CDeviceList 对话框

class CDeviceList : public CDialog
{
	DECLARE_DYNAMIC(CDeviceList)

public:
	CDeviceList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceList();

// 对话框数据
	enum { IDD = IDD_DEVICE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CMyListCtrl m_list;
	CImageList	m_cImageListNormal32;
	float cur[3];
	int vol[3];
	int TH[6];
	int energy[3];
	CSnmpObj *ipselect;
	int slaveselect;
	CString selectip;
	CString selectname;
	CString location;
	CString tp;
	CSnmpObj *p_select;
	int index;
	HTREEITEM NPMIII;
	HTREEITEM NPMIV;
	HTREEITEM NPMV;
	HTREEITEM MSPDU;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawListFrm(CDC *pDC);
	void drawDevStaus(CDC *pDC);
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void updateList(void);
	bool checkList(HTREEITEM hitem,CString name,CSnmpObj *p_Objectnow);
	void checkSonList(HTREEITEM hitem,CSnmpObj *p_Objectnow);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void insertList(CString ip,CString slavename);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void drawsenser(CDC *pDC);
	afx_msg void OnDevDel();
	afx_msg void OnDevRelease();
	void drawbtn(CDC* pDC,int x,int y,int w,int h,int icon,CString name);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void insert_m_list(void);
	void removeson(HTREEITEM hSon,CSnmpObj *p_Objectnow);
	afx_msg void OnDevSetnpm();
	afx_msg void OnAddnpmAddIiiTwo();
	afx_msg void OnAddnpm2Addnpm();
	afx_msg void OnEditDevice();
	afx_msg void OnNpmV();
	afx_msg void OnMsPdu();
	void writeUserLog(int logtype,int type,CString name, CString item, CString detail);
};
