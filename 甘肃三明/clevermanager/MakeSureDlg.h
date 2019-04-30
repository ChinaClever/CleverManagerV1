#pragma once
#include "DlgDataCenterMap.h"
#include "CanbinObjIcon.h"
#include "ServerSet.h"
#include "DeviceList.h"
#include "CanbinNPMDlg.h"
#include "EmsAtsDlg.h"

// CMakeSureDlg 对话框

class CMakeSureDlg : public CDialog
{
	DECLARE_DYNAMIC(CMakeSureDlg)

public:
	CMakeSureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMakeSureDlg();

// 对话框数据
	enum { IDD = IDD_SURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int order;
	void setOrder(int value);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void makesure(void);
	CString DCname;
	CDlgDataCenterMap * p_map;
	int datercenterNUM;
	CCanbinObjIcon *p_icon;
	CServerSet *p_list;
	CDeviceList *p_device;
	CCanbinNPMDlg *p_cabin;
	void writeUserLog(int logtype,int type,CString name, CString item, CString detail);
	int cheakUser(void);
	CEmsAtsDlg *p_emsats;
	bool onok;
	bool onok1;
	int mainputnum;
	int slaveputnum;
	CString pre_ip;
	CString cabinet_name;
	CString ip_master;
	CString ip_slave;
	static CString username;
	int slave_masternum;
	int slave_slavenum;
	canbindevice device_slave[24];
	canbindevice device_master[24];
protected:
	virtual void OnOK();
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);

};
