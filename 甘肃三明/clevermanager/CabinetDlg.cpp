// CabinetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "CabinetDlg.h"


// CCabinetDlg 对话框

IMPLEMENT_DYNAMIC(CCabinetDlg, CDialog)

CCabinetDlg::CCabinetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCabinetDlg::IDD, pParent)
{

}

CCabinetDlg::~CCabinetDlg()
{
}

void CCabinetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCabinetDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CCabinetDlg 消息处理程序

BOOL CCabinetDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CCabinetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCabinetDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CMenu Popmenu1;
	CMenu* pMenu1;
	CPoint pt;
	::GetCursorPos(&pt);
	Popmenu1.LoadMenu(IDR_CABINET_MENU);
	pMenu1 = Popmenu1.GetSubMenu(0);
	pMenu1->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
	CDialog::OnRButtonDown(nFlags, point);
}
