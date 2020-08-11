// MainFrm.h : CMainFrame 类的接口
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:
	afx_msg LONG msgWriteLogs(WPARAM wParam,LPARAM lParam);
	//afx_msg LONG msgDelMem(WPARAM wParam,LPARAM lParam);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	//NOTIFYICONDATA tnd;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTray(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowClevermanager();
	afx_msg void OnCloseClevermanager();
	afx_msg void OnDestroy();
	void Systnd();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};


