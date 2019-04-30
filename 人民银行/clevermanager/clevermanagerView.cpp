// clevermanagerView.cpp : CclevermanagerView 类的实现
//

#include "stdafx.h"
#include "clevermanager.h"

#include "clevermanagerDoc.h"
#include "clevermanagerView.h"
#include "atlimage.h" 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CclevermanagerView

IMPLEMENT_DYNCREATE(CclevermanagerView, CView)

BEGIN_MESSAGE_MAP(CclevermanagerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	
END_MESSAGE_MAP()

// CclevermanagerView 构造/析构

CclevermanagerView::CclevermanagerView()
: menuwith(0)
, page(0)
	{
	// TODO: 在此处添加构造代码

}

CclevermanagerView::~CclevermanagerView()
{
}

BOOL CclevermanagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CclevermanagerView 绘制

void CclevermanagerView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	rect.top -= 1;
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	////////////////创建一个图片的绘图环境///////////////////////
	CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);

	////////////////把图片贴到图片环境///////////////////////
	SetBkMode(BkDc,TRANSPARENT);

	CBrush *p_oldbrush;
    ///////底色///////
	CBrush brush(RGB(255,255,255));
	p_oldbrush=BkDc.SelectObject(&brush);
	BkDc.Rectangle(&rect);
	BkDc.SelectObject(p_oldbrush);
	//////////////////
	CBrush menubrush(RGB(233,240,249));
	p_oldbrush=BkDc.SelectObject(&menubrush);
	CRect menurect;
	menurect = rect;
	menurect.bottom=85;
	menurect.top=59;
	BkDc.Rectangle(&menurect);
	BkDc.SelectObject(p_oldbrush);
	///////////////////
	CBrush titlebrush(RGB(233,240,249));
	p_oldbrush=BkDc.SelectObject(&titlebrush);
	CRect titlerect;
	titlerect = rect;
	titlerect.bottom=60;
	BkDc.Rectangle(&titlerect);
	BkDc.SelectObject(p_oldbrush);
	//////////////////////
	CBrush buttom(RGB(233,240,249));
	p_oldbrush=BkDc.SelectObject(&buttom);
	CRect buttomrect;
	buttomrect = rect;
	buttomrect.top=buttomrect.bottom-25;
	BkDc.Rectangle(&buttomrect);
	BkDc.SelectObject(p_oldbrush);
	//////////////////
	BkDc.SetTextColor(RGB(64, 64, 64));
	CFont font,*pOldFont;
	font.CreateFont(
					  22,                        // nHeight
					  0,                         // nWidth
					  0,                         // nEscapement
					  0,                         // nOrientation
					  FW_NORMAL,                 // nWeight
					  1,                     // bItalic
					  FALSE,                     // bUnderline
					  0,                         // cStrikeOut
					  ANSI_CHARSET,              // nCharSet
					  OUT_DEFAULT_PRECIS,        // nOutPrecision
					  CLIP_DEFAULT_PRECIS,       // nClipPrecision
					  DEFAULT_QUALITY,           // nQuality
					  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					  "Arial");                 // lpszFacename
	pOldFont=BkDc.SelectObject(&font);
	BkDc.TextOut(230,20,"CLEVER Manager");
	BkDc.SelectObject(pOldFont);  
	///////////////////////////////////
	CImage img; 
	HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径） 
	///////////////////////
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\logo.png",10,5,212,50);
	drawMenu(page,&BkDc);
	drawFont(&BkDc,0,RGB(0,0,0));
	BkDc.TextOut(rect.Width()-150,rect.bottom-20,"中国人民银行");
	BkDc.TextOut(rect.Width()-150,40,VER);
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	memdc.DeleteDC(); 
}


// CclevermanagerView 打印

BOOL CclevermanagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CclevermanagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CclevermanagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CclevermanagerView 诊断

#ifdef _DEBUG
void CclevermanagerView::AssertValid() const
{
	CView::AssertValid();
}

void CclevermanagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CclevermanagerDoc* CclevermanagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CclevermanagerDoc)));
	return (CclevermanagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CclevermanagerView 消息处理程序
void CclevermanagerView::drawMenu(int index,CDC *pDC)
{
		CBitmap BkMap;
		BkMap.CreateCompatibleBitmap(pDC,menuwith*7,26);
		CDC BkDc;
		BkDc.CreateCompatibleDC(pDC);
		BkDc.SelectObject(&BkMap);
		SetBkMode(BkDc.GetSafeHdc(),TRANSPARENT);
	CBrush brush(RGB(255,255,255));
	BkDc.SelectObject(&brush);

	
	BkDc.Rectangle(0,0,menuwith,26);
	retange(&BkDc,0,1,menuwith,26);

	BkDc.Rectangle(menuwith-1,0,menuwith*2,26);
	retange(&BkDc,menuwith-1,2,menuwith*2,26);

	BkDc.Rectangle(menuwith*2-1,0,menuwith*3,26);
	retange(&BkDc,menuwith*2-1,1,menuwith*3,26);

	BkDc.Rectangle(menuwith*3-1,0,menuwith*4,26);
	retange(&BkDc,menuwith*3-1,1,menuwith*4,26);

	BkDc.Rectangle(menuwith*4-1,0,menuwith*5,26);
	retange(&BkDc,menuwith*4-1,1,menuwith*5,26);

	BkDc.Rectangle(menuwith*5-1,0,menuwith*6,26);
	retange(&BkDc,menuwith*5-1,1,menuwith*6,26);

	BkDc.Rectangle(menuwith*6-1,0,menuwith*7,26);
	retange(&BkDc,menuwith*6-1,1,menuwith*7,26);

	

	drawFont(&BkDc,1,RGB(32,32,128));
	BkDc.TextOut(10,6,"机房拓扑");
	BkDc.TextOut(menuwith+10,6,"信息统计");
	BkDc.TextOut(menuwith*2+10,6,"环境状态");
	BkDc.TextOut(menuwith*3+10,6,"NPM设备列表");
	BkDc.TextOut(menuwith*4+10,6,"服务设置");
	BkDc.TextOut(menuwith*5+10,6,"日志查看");

	CBrush brush1(RGB(128,128,220));
	BkDc.SelectObject(&brush1);
	drawFont(&BkDc,1,RGB(255,255,255));
	if(index == 0)
	{
		BkDc.Rectangle(0,0,menuwith,26);
		BkDc.TextOut(10,6,"机房拓扑");
	}
	else if(index == 1)
	{
		BkDc.Rectangle(menuwith-1,0,menuwith*2,26);
		BkDc.TextOut(menuwith+10,6,"信息统计");
	}
	else if(index == 2)
	{
		BkDc.Rectangle(menuwith*2-1,0,menuwith*3,26);
		BkDc.TextOut(menuwith*2+10,6,"环境状态");
	}
	else if(index == 3)
	{
		BkDc.Rectangle(menuwith*3-1,0,menuwith*4,26);
		BkDc.TextOut(menuwith*3+10,6,"NPM设备列表");
	}
	else if(index == 4)
	{
		BkDc.Rectangle(menuwith*4-1,0,menuwith*5,26);
		BkDc.TextOut(menuwith*4+10,6,"服务设置");
	}
	else if(index == 5)
	{
		BkDc.Rectangle(menuwith*5-1,0,menuwith*6,26);
		BkDc.TextOut(menuwith*5+10,6,"日志查看");
	}
	/*else if(index == 6)
	{
		BkDc.Rectangle(menuwith*6-1,0,menuwith*7,26);
		BkDc.TextOut(menuwith*6+10,6,"");
	}*/


		pDC->BitBlt(10,59,menuwith*6,26,&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
}
void CclevermanagerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.x>10 && point.x<menuwith+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
		/*CDC* pDC;
		pDC = GetDC();
		CBitmap BkMap;
		BkMap.CreateCompatibleBitmap(pDC,menuwith*6,26);
		CDC BkDc;
		BkDc.CreateCompatibleDC(pDC);
		BkDc.SelectObject(&BkMap);
		SetBkMode(BkDc.GetSafeHdc(),TRANSPARENT);
		CBrush brush(RGB(230,235,242));
		CBrush brush1(RGB(128,128,220));
		BkDc.SelectObject(&brush1);
		BkDc.Rectangle(0,0,menuwith,26);
		drawFont(&BkDc,1,RGB(255,255,255));
		BkDc.TextOut(10,6,"Distribution");
		BkDc.SelectObject(&brush);
		BkDc.Rectangle(menuwith-1,0,menuwith*2,26);
		BkDc.Rectangle(menuwith*2-1,0,menuwith*3,26);
		BkDc.Rectangle(menuwith*3-1,0,menuwith*4,26);
		BkDc.Rectangle(menuwith*4-1,0,menuwith*5,26);
		BkDc.Rectangle(menuwith*5-1,0,menuwith*6,26);
		drawFont(&BkDc,1,RGB(128,128,220));
		BkDc.TextOut(menuwith+10,6,"Information");
		BkDc.TextOut(menuwith*2+10,6,"Environment");
		BkDc.TextOut(menuwith*3+10,6,"NPM list");
		BkDc.TextOut(menuwith*4+10,6,"Server");
		BkDc.TextOut(menuwith*5+10,6,"Logs View");
		pDC->BitBlt(10,59,menuwith*6,26,&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
		ReleaseDC(pDC);*/
		
	}
	else if(point.x>menuwith+10 && point.x<menuwith*2+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	else if(point.x>menuwith*2+10 && point.x<menuwith*3+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	else if(point.x>menuwith*3+10 && point.x<menuwith*4+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	else if(point.x>menuwith*4+10 && point.x<menuwith*5+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	else if(point.x>menuwith*5+10 && point.x<menuwith*6+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	else if(point.x>menuwith*6+10 && point.x<menuwith*7+10 && point.y>60 && point.y<85)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		
	}
	CView::OnMouseMove(nFlags, point);
}

void CclevermanagerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	page = 0;
	menuwith = 120;
	CRect rect;
	GetClientRect(&rect);
	m_tab.Create(TCS_BUTTONS,rect,this,IDC_MAIN_TAB);
	tab1.Create(IDD_DATACENTER_MAP,&m_tab);
	tab1.ShowWindow(5);
	tab8.Create(IDD_SHOW,&m_tab);
	tab8.ShowWindow(1);
	tab9.Create(IDD_SHOW_TITLE,&m_tab);
	tab9.ShowWindow(5);
	tab2.Create(IDD_DATACENTER_INFO,&m_tab);
	tab2.ShowWindow(0);
	tab3.Create(IDD_DATACENTER_ENV,&m_tab);
	tab3.ShowWindow(0);
	tab4.Create(IDD_DEVICE_LIST,&m_tab);
	tab4.ShowWindow(0);
	tab5.Create(IDD_SERVER_SET,&m_tab);
	tab5.ShowWindow(0);
	tab6.Create(IDD_LOGS,&m_tab);
	tab6.ShowWindow(0);
	//tab7.Create(IDD_EMSATS,&m_tab);
	//tab7.ShowWindow(0);
	m_tab.ShowWindow(1);
}

void CclevermanagerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	rect.top = 90;
	rect.left=5;
	rect.bottom = rect.bottom-30;
	rect.right = rect.right-5;
	if(IsWindow(m_tab.m_hWnd))
	{
		m_tab.MoveWindow(&rect);
		CRect rect1;
	    m_tab.GetClientRect(&rect1);
		tab1.MoveWindow(rect.Width()/5,0,4*rect.Width()/5,rect.Height());
		tab8.MoveWindow(rect.left-5,60,rect.Width()/5-5,rect.Height());
		tab9.MoveWindow(0,35,rect.Width()/5-5,25);
		tab2.MoveWindow(&rect1);
		tab3.MoveWindow(&rect1);
		tab4.MoveWindow(&rect1);
		tab5.MoveWindow(&rect1);
		tab6.MoveWindow(&rect1);
		//tab7.MoveWindow(&rect1);
	}
}

void CclevermanagerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.x>10 && point.x<menuwith+10 && point.y>60 && point.y<85)
	{
		page=0;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(0,pDC);
		ReleaseDC(pDC);

		tab1.ShowWindow(5);
		tab8.ShowWindow(5);
		tab9.ShowWindow(5);
		tab2.ShowWindow(0);
		tab3.ShowWindow(0);
		tab4.ShowWindow(0);
		tab5.ShowWindow(0);
		tab6.ShowWindow(0);
		//tab7.ShowWindow(0);
	}
	else if(point.x>menuwith+10 && point.x<menuwith*2+10 && point.y>60 && point.y<85)
	{
		page=1;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(1,pDC);
		ReleaseDC(pDC);

		tab1.ShowWindow(0);
		tab8.ShowWindow(0);
		tab9.ShowWindow(0);
		tab2.ShowWindow(1);
		tab3.ShowWindow(0);
		tab4.ShowWindow(0);
		tab5.ShowWindow(0);
		tab6.ShowWindow(0);
		//tab7.ShowWindow(0);
	}
	else if(point.x>menuwith*2+10 && point.x<menuwith*3+10 && point.y>60 && point.y<85)
	{
		page=2;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(2,pDC);
		ReleaseDC(pDC);
		tab1.ShowWindow(0);
		tab8.ShowWindow(0);
		tab9.ShowWindow(0);
		tab2.ShowWindow(0);
		tab3.ShowWindow(1);
		tab4.ShowWindow(0);
		tab5.ShowWindow(0);
		tab6.ShowWindow(0);
		//tab7.ShowWindow(0);
	}
	else if(point.x>menuwith*3+10 && point.x<menuwith*4+10 && point.y>60 && point.y<85)
	{
		page=3;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(3,pDC);
		ReleaseDC(pDC);
		tab1.ShowWindow(0);
		tab8.ShowWindow(0);
		tab9.ShowWindow(0);
		tab2.ShowWindow(0);
		tab3.ShowWindow(0);
		tab4.ShowWindow(1);
		tab5.ShowWindow(0);
		tab6.ShowWindow(0);
		//tab7.ShowWindow(0);
	}
	else if(point.x>menuwith*4+10 && point.x<menuwith*5+10 && point.y>60 && point.y<85)
	{
		page=4;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(4,pDC);
		ReleaseDC(pDC);
		tab1.ShowWindow(0);
		tab8.ShowWindow(0);
		tab9.ShowWindow(0);
		tab2.ShowWindow(0);
		tab3.ShowWindow(0);
		tab4.ShowWindow(0);
		tab5.ShowWindow(1);
		tab6.ShowWindow(0);
		//tab7.ShowWindow(1);
	}
	else if(point.x>menuwith*5+10 && point.x<menuwith*6+10 && point.y>60 && point.y<85)
	{
		page=5;
		CDC* pDC;
		pDC = GetDC();
		drawMenu(5,pDC);
		ReleaseDC(pDC);
		tab1.ShowWindow(0);
		tab8.ShowWindow(0);
		tab9.ShowWindow(0);
		tab2.ShowWindow(0);
		tab3.ShowWindow(0);
		tab4.ShowWindow(0);
		tab5.ShowWindow(0);
		tab6.ShowWindow(1);
		//tab7.ShowWindow(0);
	}
	//else if(point.x>menuwith*6+10 && point.x<menuwith*7+10 && point.y>60 && point.y<85)
	//{
	//	page=6;
	//	CDC* pDC;
	//	pDC = GetDC();
	//	drawMenu(6,pDC);
	//	ReleaseDC(pDC);
	//	tab1.ShowWindow(0);
	//	tab8.ShowWindow(0);
	//	tab9.ShowWindow(0);
	//	tab2.ShowWindow(0);
	//	tab3.ShowWindow(0);
	//	tab4.ShowWindow(0);
	//	tab5.ShowWindow(0);
	//	tab6.ShowWindow(1);
	//	//tab7.ShowWindow(0);
	//}
	CView::OnLButtonDown(nFlags, point);
}





