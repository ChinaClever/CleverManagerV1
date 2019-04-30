#pragma once


// CDlgDataCenterMap 对话框
#include "CanbinObjIcon.h"
class CDlgDataCenterMap : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataCenterMap)

public:
	CDlgDataCenterMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataCenterMap();

// 对话框数据
	enum { IDD = IDD_DATACENTER_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMovecanbin32771();
	afx_msg void OnMovecanbin32772();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMovecanbin32773();
	afx_msg void OnAddCabin();
	afx_msg void OnSaveMap();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Dcname();
	void Drawroom(CString roomname);
	void Drawname();
	int FindStatus_M(CCanbinObjIcon* p_icon);
	int FindStatus_S(CCanbinObjIcon* p_icon);
	void Status();
	void SetStatus(int status1,int status2,CCanbinObjIcon* p_icon);
	afx_msg void OnMovecanbinEdit();
public:
	bool mapEdit;
	CString DCname;
	CPoint MouseRPoint;
};
