#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include <vector>
using std::vector;
// CEnergylogs 对话框

class CEnergylogs : public CDialog
{
	DECLARE_DYNAMIC(CEnergylogs)

public:
	CEnergylogs(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEnergylogs();

// 对话框数据
	enum { IDD = IDD_ENERGY_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CMyListCtrl m_list;
	virtual BOOL OnInitDialog();
	void insertList();
	CDateTimeCtrl tm1;
	CDateTimeCtrl tm2;
	void put_record(CString start_time,vector<int> start_a_energy,vector<int> start_b_energy,CString end_time,vector<int> end_a_energy,vector<int> end_b_energy,CString logname);
	void put_record_output(CString start_time,int start_a_energy,int start_b_energy,int start_c_energy,CString end_time,int end_a_energy,int end_b_energy,int end_c_energy,CString logname);
	int year_ret(CString str_year);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CComboBox m_device_ip;
	CComboBox m_device_num;
	CComboBox m_type;
	CComboBox m_data;
	afx_msg void OnCbnSelchangeCombo4();
	void SetCabinet(CString type);
	void put_record_energy(void);
	int get_room_energy(CString name,CString time,int change , int load);
	int get_cabinet_energy(CString name,CString time,int change ,int load);
	int get_ip_energy(CString ip,int num,CString time, int load);

	//void get_output_energy(CString ip,vector<int>& opstartenergy,vector<int>& opendenergy);
};
