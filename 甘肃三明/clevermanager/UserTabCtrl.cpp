// UserTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "UserTabCtrl.h"


// CUserTabCtrl

IMPLEMENT_DYNAMIC(CUserTabCtrl, CTabCtrl)

CUserTabCtrl::CUserTabCtrl()
{

}

CUserTabCtrl::~CUserTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CUserTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CUserTabCtrl 消息处理程序



BOOL CUserTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	//CDC memdc;
	//memdc.CreateCompatibleDC(pDC);
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
	BkDc.SelectStockObject(NULL_PEN);
	rect.bottom += 1;
	rect.right += 1;
	BkDc.Rectangle(&rect);
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	//memdc.DeleteDC();
	return true;

	//return CTabCtrl::OnEraseBkgnd(pDC);
}
