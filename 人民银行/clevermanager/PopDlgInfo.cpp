// PopDlgInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "PopDlgInfo.h"


// CPopDlgInfo 对话框

IMPLEMENT_DYNAMIC(CPopDlgInfo, CDialog)

CPopDlgInfo::CPopDlgInfo(CWnd* pParent /*=NULL*/)
: CDialog(CPopDlgInfo::IDD, pParent)
{

}

CPopDlgInfo::~CPopDlgInfo()
{
}

void CPopDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopDlgInfo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPopDlgInfo::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPopDlgInfo 消息处理程序

static CString device_ip;
static CString room_position;
static CString alarm_information;

void CPopDlgInfo::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PostNcDestroy();
	delete this;
}

void CPopDlgInfo::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();

	//CDialog::OnCancel();
}

void CPopDlgInfo::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPopDlgInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	CDC   *pdc; 
	pdc=GetDC();
	CDC   memDC;//定义一个兼容DC 
	memDC.CreateCompatibleDC(pdc);//创建DC 
	CBitmap   bmpDraw; 
	if   (m_bRed==false) 
	{
		bmpDraw.LoadBitmap(IDB_BITMAP6);//装入红灯
		m_bRed=true;
	}
	else
	{
		bmpDraw.LoadBitmap(IDB_BITMAP7);//装入绿灯 
		m_bRed=false;
	}
	//保存原有DDB，并选入新DDB入DC 
	CBitmap*   pbmpOld=memDC.SelectObject(&bmpDraw);//将源DC中(0,0,w,h)复制到目的DC(x,y,w,h); 
	pdc-> BitBlt(2,26,100,176,&memDC,0,0,SRCCOPY); 
	memDC.SelectObject(pbmpOld);//选入原DDB,以后如果需恢复时用
	*/
	CDialog::OnTimer(nIDEvent);
}

BOOL CPopDlgInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(ALARM_TIME,125,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



BOOL CPopDlgInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	CRect rect;
	GetClientRect(&rect);
	////////////////创建一个图片的绘图环境///////////////////////
	CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	/////////////////////////////////
	SetBkMode(BkDc,TRANSPARENT);
	CBrush brush(RGB(255,255,255));
	BkDc.SelectObject(&brush);
	BkDc.Rectangle(0,24,rect.Width(),rect.Height());

	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);
	drawFont(&BkDc,1,RGB(64,64,64));
	BkDc.TextOutA(20,5,"告警信息");
	retange(&BkDc,1,1,rect.Width()-2,23);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.Width()-25,6,12,12);

	BkDc.TextOutA(50,25,device_ip);
	BkDc.TextOutA(50,45,room_position);
	BkDc.TextOutA(50,65,alarm_information);



	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
	*/
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CPopDlgInfo::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	CRect rc;
	rc.bottom=23;
	rc.top=1;
	rc.left=1;
	rc.right=rect.right-25;
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
	return CDialog::OnNcHitTest(point);
}
void CPopDlgInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.x>270 && point.x<290 && point.y>5 && point.y<20)//BkDc.Rectangle(270,5,290,20);
	{
		OnOK();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CPopDlgInfo::Alarminformation(CString ip, CString position, CString information)
{
	device_ip=ip;//设备IP：192.168.1.163
	room_position=position;//机柜位置：clever2 （1:20）
	alarm_information=information;//output1.load low 0.0(1.0--10.0)
}
