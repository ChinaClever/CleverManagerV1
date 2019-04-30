#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include <afxdb.h>

// CStatusLogs dialog

class CStatusLogs : public CDialog
{
	DECLARE_DYNAMIC(CStatusLogs)

public:
	CStatusLogs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatusLogs();

// Dialog Data
	enum { IDD = IDD_STATUS_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void drawgraph(CDC *pDC,int y_zhou_show);
	void drawgraph_60(CDC *pDC);
	CString title;
	int x_zhou[25];
	int x_zhou_60[24][60];
	int data_x;
	int data_y;
	CDateTimeCtrl tm1;
	CComboBox m_com;
	CString sSql;
	void insertline(void);

	void fillcurrent_60(float data_cur[24][60]);


	void fillcurrent(float cur[24][60]);
	void fillvoltage(int cur[24][60]);
	void fillth(int cur[24][60]);
	CDateTimeCtrl tm2;
	void exportexl(void);
	int year_ret(CString str_year);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void putdata_int(CString text,int data[60]);
	void putdata_float(CString text,float data[60]);
	CComboBox m_ip;
};

