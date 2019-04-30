// clevermanagerView.h : CclevermanagerView 类的接口
//


#pragma once
#include "DlgDataCenterMap.h"
#include "DatacenterInfo.h"
#include "DatacenterEnv.h"
#include "DeviceList.h"
#include "UserTabCtrl.h"
#include "ServerSet.h"
#include "LogsDlg.h"
#include "EmsAtsDlg.h"
#include "Show.h"
#include "Show_title.h"
class CclevermanagerView : public CView
{
protected: // 仅从序列化创建
	CclevermanagerView();
	DECLARE_DYNCREATE(CclevermanagerView)

// 属性
public:
	CclevermanagerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CclevermanagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CUserTabCtrl m_tab;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDlgDataCenterMap tab1;
	CDatacenterInfo tab2;
	CDatacenterEnv tab3;
	CDeviceList tab4;
	CServerSet tab5;
	CLogsDlg tab6;
	CEmsAtsDlg tab7;
	CShow tab8;
	CShow_title tab9;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int menuwith;
	int page;
	void drawMenu(int index,CDC *pDC);
	
};

#ifndef _DEBUG  // clevermanagerView.cpp 中的调试版本
inline CclevermanagerDoc* CclevermanagerView::GetDocument() const
   { return reinterpret_cast<CclevermanagerDoc*>(m_pDocument); }
#endif

