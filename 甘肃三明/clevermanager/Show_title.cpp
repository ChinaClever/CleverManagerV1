// Show_title.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "Show_title.h"


// CShow_title 对话框

IMPLEMENT_DYNAMIC(CShow_title, CDialog)

CShow_title::CShow_title(CWnd* pParent /*=NULL*/)
: CDialog(CShow_title::IDD, pParent)
{

}

CShow_title::~CShow_title()
{
}

void CShow_title::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShow_title, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CShow_title 消息处理程序

BOOL CShow_title::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);

	CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	SetBkMode(BkDc,TRANSPARENT);
	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),rect.Height());
	drawFont(&BkDc,1,RGB(0,0,0));
	BkDc.TextOutA(20,5,"机房列表");
	//retange(&BkDc,1,1,rect.Width()-1,rect.Height());
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
}
