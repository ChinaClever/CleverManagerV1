#pragma once
#include "CanbinObjIcon.h"
#include "SnmpObj.h"
#include "afxwin.h"

// CCanbinNPMDlg 对话框

class CCanbinNPMDlg : public CDialog
{
	DECLARE_DYNAMIC(CCanbinNPMDlg)

public:
	CCanbinNPMDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCanbinNPMDlg();

// 对话框数据
	enum { IDD = IDD_CANBIN_NPM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CCanbinObjIcon *p_icon;
	CSnmpObj *master;
	CSnmpObj *salve;
	int masterindex;
	int slaveindex;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAddCabinDevice();
	afx_msg void OnDelCabinDevice();
	int checkoutput;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool mapEdit;
	int SlaveNum(void);
	CEdit m_edit;
	CEdit m_edit1;
	//afx_msg void OnEnKillfocusEdit1();
	//afx_msg void OnEnSetfocusEdit1();
	//afx_msg void OnEnKillfocusEdit2();
	//afx_msg void OnEnSetfocusEdit2();
	int nCtrlID;
	struct canbindevice1
	{
		CString name;
		int start;
		int height;
		int maininputNum;
		int slaveinputNum;
	};
	
	canbindevice1 device1[24];
	canbindevice1 device2[24];
	
	CString str;
	CString str1;
	//CString DCname;
	void Dcname(CString DCname);
	afx_msg void OnSlave();
	afx_msg void OnDelSlave();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnAddSlaveDevice();
	afx_msg void OnDelSlaveDevice();
	afx_msg void On32876();
	afx_msg void On32877();
	afx_msg void On32878();
	afx_msg void On32879();
};
