// ColorHeaderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "ColorHeaderCtrl.h"


// CColorHeaderCtrl

IMPLEMENT_DYNAMIC(CColorHeaderCtrl, CHeaderCtrl)

CColorHeaderCtrl::CColorHeaderCtrl()
{

}

CColorHeaderCtrl::~CColorHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorHeaderCtrl 消息处理程序



void CColorHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
 
 // TODO: Add your message handler code here
 
 // Do not call CHeaderCtrl::OnPaint() for painting messages
    CRect rect;
	GetClientRect(rect);
	SetBkMode(dc.GetSafeHdc(),TRANSPARENT);
	dc.FillSolidRect(rect,RGB(255,255,255));   //重绘标题栏颜色
	retange(&dc,rect.left,rect.top,rect.Width(),rect.Height());
	int nItems = GetItemCount();
	CRect rectItem;
	CPen m_pen(PS_SOLID,1,RGB(211,211,211));      //分隔线颜色
	CPen * pOldPen=dc.SelectObject(&m_pen);
	CFont m_font;
	m_font.CreatePointFont(90,"宋体");        //字体
	CFont * pOldFont=dc.SelectObject(&m_font);
	dc.SetTextColor(RGB(64,64,64));     //字体颜色
	for(int i = 0; i <nItems; i++)                    //对标题的每个列进行重绘
	{  
		GetItemRect(i, &rectItem);
		rectItem.top+=2;
		rectItem.bottom+=2; 
		dc.MoveTo(rectItem.right,rect.top);                //重绘分隔栏
		dc.LineTo(rectItem.right,rectItem.bottom);
		TCHAR buf[256];
		HD_ITEM hditem;

		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( i, &hditem );                                       //获取当然列的文字
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS ;
		dc.DrawText(buf, &rectItem, uFormat);           //重绘标题栏的文字
	}
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}
