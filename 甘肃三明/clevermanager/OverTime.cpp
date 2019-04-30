// OverTime.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "OverTime.h"
#include "MainFrm.h"

// COverTime 对话框

IMPLEMENT_DYNAMIC(COverTime, CDialog)

COverTime::COverTime(CWnd* pParent /*=NULL*/)
	: CDialog(COverTime::IDD, pParent)
{

}

COverTime::~COverTime()
{
}

void COverTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COverTime, CDialog)
	ON_BN_CLICKED(IDOK, &COverTime::OnBnClickedOk)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// COverTime 消息处理程序

void COverTime::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	
}

void COverTime::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID==SC_CLOSE)
	{
		OnBnClickedOk();
	}
	CDialog::OnSysCommand(nID, lParam);
}
