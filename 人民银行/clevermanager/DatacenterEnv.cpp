// DatacenterEnv.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "DatacenterEnv.h"
#include "atlimage.h"
#include "CanbinObjIcon.h"
#include "SnmpObj.h"
// CDatacenterEnv 对话框

IMPLEMENT_DYNAMIC(CDatacenterEnv, CDialog)

CDatacenterEnv::CDatacenterEnv(CWnd* pParent /*=NULL*/)
	: CDialog(CDatacenterEnv::IDD, pParent)
	, DCname(_T(""))
{

}

CDatacenterEnv::~CDatacenterEnv()
{
}

void CDatacenterEnv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatacenterEnv, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDatacenterEnv 消息处理程序
BOOL CDatacenterEnv::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int i;
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
	BkDc.SelectStockObject(NULL_PEN);
	rect.bottom += 1;
	rect.right += 1;
	BkDc.Rectangle(&rect);
	//::SetStretchBltMode(BkDc.GetSafeHdc(),COLORONCOLOR);
	CImage img; 
	HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径）
	drawFont(&BkDc,1,RGB(0,0,0));
	BkDc.TextOut(15,8,"机房机柜温度云分布图");

	drawImagePNG(&BkDc,getFilePath()+"\\icon\\canbin.png",240,2,35,25);
	
	drawFont(&BkDc,0,RGB(0,0,0));
	BkDc.TextOut(285,8,"机柜");
	BkDc.SelectStockObject(BLACK_PEN);
	rect.bottom -= 1;
	rect.right -= 1;
	BkDc.Rectangle(0,35,rect.right,rect.bottom);
	BkDc.Rectangle(0,60,rect.right,rect.bottom);

	drawFont(&BkDc,0,RGB(96,192,96));
	BkDc.TextOut(28,42,"前一页 ");
	BkDc.TextOut(rect.right-60,42,"后一页");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\l.jpg",2,40,15,15);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\r.jpg",rect.right-22,40,15,15);

	drawFont(&BkDc,1,RGB(0,0,0));
	
	CSize fontsize= BkDc.GetTextExtent(DCname);
	BkDc.TextOut(rect.Width()/2-fontsize.cx/2,40,DCname);
	//////////////////////////////////////////////////
	int j;
	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			position[i][j]=0;
		}
	}
	////////////////////////////////

	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon *p_icon = ((CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos)));
				p_icon->onDraw (&BkDc,rect.Width(),rect.Height()-60,false);
				position[p_icon->x>19 && p_icon->x<0?0:p_icon->x][p_icon->y>19 && p_icon->y<0?0:p_icon->y]=p_icon->temp;
			}
		}
	}
	drawColor(&BkDc);
	drawflags(&BkDc);
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDatacenterEnv::drawColor(CDC *pDC)
{
	int i,j;
	CRect rect;
	GetClientRect(&rect);
	rect.top = 60;
	int x,y;
	x=rect.Width()/20;
	y=rect.Height()/20;
	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			if(position[i][j]==0)
			{
				if(position[i==0?0:i-1][j]>0 && position[i==19?19:i+1][j]>0)
				{
					position[i][j]=(position[i==0?0:i-1][j]+position[i==19?19:i+1][j])/2;
				}
				if(position[i][j==0?0:j-1]>0 && position[i][j==0?0:j+1]>0)
				{
					position[i][j]=(position[i][j==0?0:j-1]+position[i][j==19?19:j+1])/2;
				}
			}
		}
	}
	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			///////////////////////
			Point points[4];
			Color colors[4];

			colors[0] = setcolor(position[i][j]);
			points[0]=Point((INT)((i==0?0:i+0.5)*x),(INT)((j==0?0:j+0.5)*y+60));

			colors[1] = setcolor(position[i==19?19:i+1][j]);
			points[1]=Point((INT)((i+1.5)*x),(INT)((j==0?0:j+0.5)*y+60));

			colors[2] = setcolor(position[i==19?19:i+1][j==19?19:j+1]);
			points[2]=Point((INT)((i+1.5)*x),(INT)((j+1.5)*y+60));

			colors[3] = setcolor(position[i][j==19?19:j+1]);
			points[3]=Point((INT)((i==0?0:i+0.5)*x),(INT)((j+1.5)*y+60));
			
			///////////////////////////////
			int count=4;
			Graphics graphics(pDC->GetSafeHdc());
			GraphicsPath path;
			path.AddLines(points,count);
			PathGradientBrush pthGrBrush(&path);
			//设置中心点色彩

			pthGrBrush.SetCenterColor(setcolor(position[i][j]));

			//pthGrBrush.SetCenterColor(Color(128,0,0,255));
			pthGrBrush.SetCenterPoint(Point((INT)((i+0.5)*x),(INT)((j+0.5)*y+60)));

			//设置每个端点色彩
			//Color colors[]={
			//	Color(64,255,255,255),Color(64,255,255,255),Color(64,255,255,255),Color(64,255,255,255)
			//};
			pthGrBrush.SetSurroundColors(colors,&count);
			graphics.FillPath(&pthGrBrush,&path);
			graphics.ReleaseHDC(pDC->GetSafeHdc());
			
		}
	}
}


Color CDatacenterEnv::setcolor(int value)
{
	Color points;
	if(value>35)
	{
		points = Color(128,255,0,0);
	}
	else if(value>30)
	{
		points = Color(128,255,255,0);
	}
	else if(value>25)
	{
		points = Color(128,0,255,0);
	}
	else if(value>0)
	{
		points = Color(128,0,0,255);
	}
	else
	{
		points = Color(128,255,255,255);
	}
	return points;
}

void CDatacenterEnv::drawflags(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);

	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	LinearGradientBrush linGrBrush(Point(rect.Width()-250,15),Point(rect.Width()-100,15),Color(128,255,0,0),Color(128,0,0,255));
	//LinearGradientBrush linGrBrush;
	Color colors[] = {
		Color(128, 255, 0, 0),   // red
		Color(128, 255, 255, 0),//yellow
		Color(128, 0, 255, 0),   // blue
		Color(128, 0, 0, 255)};  // green

		REAL positions[] = {
			0.0f,  
			0.2f,
			0.6f,
			1.0f}; 
			linGrBrush.SetInterpolationColors(colors, positions,4);
			Pen Mypen(&linGrBrush,15);
			graphics.DrawLine(&Mypen,Point(rect.Width()-249,15),Point(rect.Width()-100,15));
	graphics.ReleaseHDC(pDC->GetSafeHdc());

	drawFont(pDC,0,RGB(0,0,0));
	pDC->TextOut(rect.Width()-320,10,"图例:<--35");
	pDC->TextOut(rect.Width()-175,10,"28");
	pDC->TextOut(rect.Width()-98,10,"22--> (℃)");
}

BOOL CDatacenterEnv::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int i;
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			DCname = DCmember[i].name;
			break;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDatacenterEnv::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i,j;
	CRect rect;
	GetClientRect(&rect);
	if(point.y<60)
	{
		if(point.x>2 && point.x<67 &&
			point.y>30 && point.y<60)
		{
			for(i=0;i<10;i++)
			{
				if(DCname==DCmember[i].name && DCmember[i].name.GetLength())
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i-j-1<0?i-j+9:i-j-1].name.GetLength())
						{
							DCname = DCmember[i-j-1<0?i-j+9:i-j-1].name;
							CDC* dc;
							dc = GetDC();
							OnEraseBkgnd(dc);
							ReleaseDC(dc);
							return;
						}
					}
				}
			}
			if(i>9)
			{
				for(i=0;i<10;i++)
				{
					if(DCmember[i].name.GetLength())
					{
						DCname=DCmember[i].name;
						CDC* dc;
						dc = GetDC();
						OnEraseBkgnd(dc);
						ReleaseDC(dc);
						return;
					}
				}
			}
		}
		else if(point.x>rect.right-72 && point.x<rect.right-7 &&
			point.y>30 && point.y<60)
		{
			for(i=0;i<10;i++)
			{
				if(DCname==DCmember[i].name && DCmember[i].name.GetLength())
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i+j+1>9?i+j-9:i+j+1].name.GetLength())
						{
							DCname = DCmember[i+j+1>9?i+j-9:i+j+1].name;
							CDC* dc;
							dc = GetDC();
							OnEraseBkgnd(dc);
							ReleaseDC(dc);
							return;
						}
					}
				}
			}
			if(i>9)
			{
				for(i=0;i<10;i++)
				{
					if(DCmember[i].name.GetLength())
					{
						DCname=DCmember[i].name;
						CDC* dc;
						dc = GetDC();
						OnEraseBkgnd(dc);
						ReleaseDC(dc);
						return;
					}
				}
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDatacenterEnv::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnTimer(nIDEvent);
}

void CDatacenterEnv::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	if(point.y<60)
	{
		if(point.x>2 && point.x<67 &&
			point.y>30 && point.y<60)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
			CDC* dc;
			dc = GetDC();
			drawImagePNG(dc,getFilePath()+"\\icon\\l.jpg",2,40,20,20);
			ReleaseDC(dc);
		}
		else if(point.x>rect.right-72 && point.x<rect.right-7 &&
			point.y>30 && point.y<60)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
			CDC* dc;
			dc = GetDC();
			drawImagePNG(dc,getFilePath()+"\\icon\\r.jpg",rect.right-22,40,20,20);
			ReleaseDC(dc);
		}
	}
	else if(point.y>60)
	{
		for(int i=0;i<10;i++)
		{
			if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
			{
				POSITION pos = DCmember[i].canbin.GetTailPosition ();
				while (pos != NULL)
				{
					CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
					if(point.x>p_icon->x*rect.Width()/20 && point.x<((p_icon->x)+1)*rect.Width()/20 &&
						point.y>p_icon->y*(rect.Height()-60)/20+60 && point.y<(p_icon->y+1)*(rect.Height()-60)/20+60)
					{
						::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
						p_icon->showtitle =true;
					}
					else
					{
						p_icon->showtitle =false;
					}
				}
			}
		}
		CDC* dc;
		dc = GetDC();
		OnEraseBkgnd(dc);
		ReleaseDC(dc);

	}
	CDialog::OnMouseMove(nFlags, point);
}
