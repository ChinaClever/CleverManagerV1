// Show.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "Show.h"
#include "atlimage.h"
#include "CanbinObjIcon.h"
#include "DlgDataCenterMap.h"
// CShow 对话框

static int roomnum;
static int p_roomnum;
static int roomnum_len;
static CString DCname_room [10];
static int draw_rm;
static bool draw_scroll;
IMPLEMENT_DYNAMIC(CShow, CDialog)


CShow::CShow(CWnd* pParent /*=NULL*/)
: CDialog(CShow::IDD, pParent)
{

}

CShow::~CShow()
{
}

void CShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, SB_VERT_SHOW, m_bar);
}


BEGIN_MESSAGE_MAP(CShow, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()


// CShow 消息处理程序

BOOL CShow::OnEraseBkgnd(CDC* pDC)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(draw_rm==1)
	{
		CRect rect;
		GetClientRect(rect);

		CBitmap BkMap;
		BkMap.CreateCompatibleBitmap(pDC,rect.Width()-15,rect.Height());
		CDC BkDc;
		BkDc.CreateCompatibleDC(pDC);
		BkDc.SelectObject(&BkMap);
		SetBkMode(BkDc,TRANSPARENT);

		CBrush brushtitle(RGB(255,255,255));
		BkDc.SelectObject(&brushtitle);
		BkDc.Rectangle(0,-1,rect.Width(),rect.Height()-60);

		Drawroom(&BkDc,roomnum);
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();

	}
	else if(draw_rm==2)
	{
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
		BkDc.Rectangle(0,-1,rect.Width(),rect.Height()-60);

		Drawroom(&BkDc,roomnum);
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();

	}
	else if (draw_rm==3)
	{
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
		BkDc.Rectangle(0,-1,rect.Width(),rect.Height()-60);

		m_bar.ShowWindow(0);
		Drawroom(&BkDc,roomnum);
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
	}
	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CShow::Drawroom(CDC* pDC, int num)
{

	if(draw_rm==1)
	{
		CRect rect;
		GetClientRect(rect);

		CBitmap BkMap;
		BkMap.CreateCompatibleBitmap(pDC,rect.Width()-15,rect.Height()-60);
		CDC BkDc;
		BkDc.CreateCompatibleDC(pDC);
		BkDc.SelectObject(&BkMap);
		SetBkMode(BkDc,TRANSPARENT);
		CBrush brushtitle(RGB(255,255,255));
		BkDc.SelectObject(&brushtitle);
		BkDc.Rectangle(0,-1,rect.Width()-15,rect.Height()-60);

		CPen  pen(PS_DASHDOTDOT,1,RGB(210,210,210));
		BkDc.SelectObject(&pen);
		CImage img; 
		HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png");
		drawFont(&BkDc,0,RGB(0,0,0));
		CString str;

		int i,m;

		//即使上述步骤一不做，使用此条语句也可以显示滚动条 
		if(draw_scroll==true)
		{
			SCROLLINFO vinfo;  
			vinfo.cbSize =sizeof(vinfo); 
			vinfo.fMask =SIF_PAGE;  
			vinfo.nPage=4*600/p_roomnum;//滚动块自身的长短，通常有如下关系：其长度/滚动条长度（含两个箭头）=nPage/(nMax+2)，
			m_bar.SetScrollInfo(&vinfo,1);
			draw_scroll=false;
		}
		m_bar.MoveWindow(rect.Width()-15,0,15,rect.Height()-60);
		m_bar.ShowWindow(1);

		for(i=0;i<4;i++)
		{
			if(num<7)
			{
				if(DCname_room[num+i]==DCmember[num+i].name && DCmember[num+i].name.GetLength())
				{

					if(i==0)
					{
						BkDc.Rectangle(1,1,rect.Width()-16,(rect.Height()-60)/4);
						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//	drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,20,rect.Width()-5,rect.Height()/4-20);
						//}

						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20);
							BkDc.LineTo(rect.Width()-35,20+m*((rect.Height()-60)/4-40)/20);
						}

						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-55)/20,20);
							BkDc.LineTo(20+m*(rect.Width()-55)/20,(rect.Height()-60)/4-20);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw1(&BkDc,rect.Width()-55,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);

						BkDc.TextOut(10,0,str);
					}

					if(i==1)
					{
						BkDc.Rectangle(1,(1+(rect.Height()-60)/4),rect.Width()-16,(rect.Height()-60)/2);

						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,rect.Height()/4+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/4);
							BkDc.LineTo(rect.Width()-35,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/4);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-55)/20,20+(rect.Height()-60)/4);
							BkDc.LineTo(20+m*(rect.Width()-55)/20,(rect.Height()-60)/4-20+(rect.Height()-60)/4);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw2(&BkDc,rect.Width()-55,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,(rect.Height()-60)/4+1,str);

					}


					if(i==2)
					{
						BkDc.Rectangle(1,(1+(rect.Height()-60)/2),rect.Width()-16,3*(rect.Height()-60)/4);

						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,rect.Height()/2+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/2);
							BkDc.LineTo(rect.Width()-35,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/2);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-55)/20,20+(rect.Height()-60)/2);
							BkDc.LineTo(20+m*(rect.Width()-55)/20,(rect.Height()-60)/4-20+(rect.Height()-60)/2);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw3(&BkDc,rect.Width()-55,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,(rect.Height()-60)/2+1,str);

					}
					if(i==3)
					{

						BkDc.Rectangle(1,(1+3*(rect.Height()-60)/4),rect.Width()-16,rect.Height()-61);

						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,3*rect.Height()/4+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+3*(rect.Height()-60)/4);
							BkDc.LineTo(rect.Width()-35,20+m*((rect.Height()-60)/4-40)/20+3*(rect.Height()-60)/4);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-55)/20,20+3*(rect.Height()-60)/4);
							BkDc.LineTo(20+m*(rect.Width()-55)/20,(rect.Height()-60)/4-20+3*(rect.Height()-60)/4);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw4(&BkDc,rect.Width()-55,(rect.Height()-60)/4-40,true);
						}
						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,3*(rect.Height()-60)/4+1,str);
					}

				}
			}
		}
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
	}

	else if(draw_rm==2)
	{
		m_bar.ShowWindow(0);

		CRect rect;
		GetClientRect(rect);

		CBitmap BkMap;
		BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height()-60);
		CDC BkDc;
		BkDc.CreateCompatibleDC(pDC);
		BkDc.SelectObject(&BkMap);
		SetBkMode(BkDc,TRANSPARENT);
		CBrush brushtitle(RGB(255,255,255));
		BkDc.SelectObject(&brushtitle);
		BkDc.Rectangle(0,-1,rect.Width(),rect.Height()-60);


		CPen  pen(PS_DASHDOTDOT,1,RGB(210,210,210));
		BkDc.SelectObject(&pen);
		CImage img; 
		HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png");
		drawFont(&BkDc,0,RGB(0,0,0));
		CString str;

		int i,m;
		for(i=0;i<4;i++)
		{
			if(num<7)
			{
				if(DCname_room[num+i]==DCmember[num+i].name && DCmember[num+i].name.GetLength())
				{
					if(i==0)
					{
						BkDc.Rectangle(1,1,rect.Width()-1,(rect.Height()-60)/4);
						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//	drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,20,rect.Width()-5,rect.Height()/4-20);
						//}

						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20);
							BkDc.LineTo(rect.Width()-20,20+m*((rect.Height()-60)/4-40)/20);
						}

						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-40)/20,20);
							BkDc.LineTo(20+m*(rect.Width()-40)/20,(rect.Height()-60)/4-20);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw1(&BkDc,rect.Width()-40,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);

						BkDc.TextOut(10,0,str);
					}

					if(i==1)
					{
						BkDc.Rectangle(1,(1+(rect.Height()-60)/4),rect.Width()-1,(rect.Height()-60)/2);

						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,rect.Height()/4+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/4);
							BkDc.LineTo(rect.Width()-20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/4);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-40)/20,20+(rect.Height()-60)/4);
							BkDc.LineTo(20+m*(rect.Width()-40)/20,(rect.Height()-60)/4-20+(rect.Height()-60)/4);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw2(&BkDc,rect.Width()-40,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,(rect.Height()-60)/4+1,str);

					}


					if(i==2)
					{
						BkDc.Rectangle(1,(1+(rect.Height()-60)/2),rect.Width()-1,3*(rect.Height()-60)/4);

						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,rect.Height()/2+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/2);
							BkDc.LineTo(rect.Width()-20,20+m*((rect.Height()-60)/4-40)/20+(rect.Height()-60)/2);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-40)/20,20+(rect.Height()-60)/2);
							BkDc.LineTo(20+m*(rect.Width()-40)/20,(rect.Height()-60)/4-20+(rect.Height()-60)/2);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw3(&BkDc,rect.Width()-40,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,(rect.Height()-60)/2+1,str);

					}
					if(i==3)
					{

						BkDc.Rectangle(1,(1+3*(rect.Height()-60)/4),rect.Width()-1,rect.Height()-61);
						//if(DCmember[num+i].str_draw.GetLength())
						//{
						//drawImagePNG(&BkDc,DCmember[num+i].str_draw,2,3*rect.Height()/4+20,rect.Width()-5,rect.Height()/4-20);
						//}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20,20+m*((rect.Height()-60)/4-40)/20+3*(rect.Height()-60)/4);
							BkDc.LineTo(rect.Width()-20,20+m*((rect.Height()-60)/4-40)/20+3*(rect.Height()-60)/4);
						}
						for(m=0;m<=20;m++)
						{
							BkDc.MoveTo(20+m*(rect.Width()-40)/20,20+3*(rect.Height()-60)/4);
							BkDc.LineTo(20+m*(rect.Width()-40)/20,(rect.Height()-60)/4-20+3*(rect.Height()-60)/4);
						}
						POSITION pos = DCmember[num+i].canbin.GetTailPosition ();
						while (pos != NULL)
						{
							((CCanbinObjIcon*)(DCmember[num+i].canbin.GetPrev (pos)))->onDraw4(&BkDc,rect.Width()-40,(rect.Height()-60)/4-40,true);
						}

						str.Empty();
						str.Format("%d、%s",num+i+1,DCname_room[num+i]);
						BkDc.TextOut(10,3*(rect.Height()-60)/4+1,str);
					}

				}
			}
		}
		
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
	}
	else if(draw_rm==3)
	{
		m_bar.ShowWindow(0);

	}

}

void CShow::AddRoom(void)
{
	int i;
	p_roomnum=0;
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			DCname_room[i] = DCmember[i].name;
			p_roomnum++;
		}
	}
	if(p_roomnum>4)
	{
		SCROLLINFO vinfo;  
		vinfo.cbSize =sizeof(vinfo); 
		vinfo.fMask =SIF_ALL;  
		vinfo.nPage=4*600/p_roomnum;//滚动块自身的长短，通常有如下关系：其长度/滚动条长度（含两个箭头）=nPage/(nMax+2)，
		//另外nPage取值-1时，滚动条会不见了。  
		vinfo.nMax =600;//滚动条所能滚动的最大值  
		vinfo.nMin=0;//滚动条所能滚动的最小值   
		vinfo.nPos=0;
		m_bar.SetScrollInfo(&vinfo,1);
		roomnum_len=600/p_roomnum;
		Draw_room(1);
		roomnum=0;
	}
	else if(p_roomnum==0)
	{
		Draw_room(3);
	}
	else 
	{
		SCROLLINFO vinfo;  
		vinfo.cbSize =sizeof(vinfo); 
		vinfo.fMask =SIF_ALL ;  
		vinfo.nPage =4*600/p_roomnum;//滚动块自身的长短，通常有如下关系：其长度/滚动条长度（含两个箭头）=nPage/(nMax+2)，
		//另外nPage取值-1时，滚动条会不见了。  
		vinfo.nMax =600;//滚动条所能滚动的最大值  
		vinfo.nMin=0;//滚动条所能滚动的最小值 
		vinfo.nPos=0;
		m_bar.SetScrollInfo(&vinfo,1);
		Draw_room(2);
		roomnum=0;
	}
}

BOOL CShow::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	draw_rm=0;
	AddRoom();
	draw_scroll=false;
	roomnum=0;
	SetTimer(SHOW_TIME,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(SHOW_TIME == nIDEvent)
	{
		CDC *pDC;
		pDC = GetDC();
		if(roomnum==0)
		{
			Drawroom(pDC, 0);
			m_bar.SetScrollPos(0);
		}
		else
			Drawroom(pDC, roomnum);
		//OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}

	CDialog::OnTimer(nIDEvent);
}

void CShow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);
	SCROLLINFO scrollinfo; 
	m_bar.GetScrollInfo(&scrollinfo,SIF_ALL); 
	switch (nSBCode) 
	{ 
	case SB_LINEUP: //单击上箭头 
		if(roomnum==1)
		{
			scrollinfo.nPos=0;
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL); 
			ScrollWindow(0,10);  
			roomnum-=1;
			if(roomnum>=0)
			{
				Drawroom(pDC, roomnum);
			}
			break;
		}
		scrollinfo.nPos -= roomnum_len;  
		if (scrollinfo.nPos<scrollinfo.nMin)  
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		m_bar.SetScrollInfo(&scrollinfo,SIF_ALL); 
		ScrollWindow(0,10);  
		roomnum-=1;
		if(roomnum>=0)
		{
			Drawroom(pDC, roomnum);
		}
		break;  
	case SB_LINEDOWN: //单击下箭头 
		if((roomnum+1)==p_roomnum)
			break;
		scrollinfo.nPos += roomnum_len;  
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{ 
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		m_bar.SetScrollInfo(&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10); 
		roomnum+=1;
		Drawroom(pDC, roomnum);
		break;  
	case SB_PAGEUP: //单击滑块上方空白区域 
		if(roomnum==1)
		{
			scrollinfo.nPos=0;
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL); 
			ScrollWindow(0,10);  
			roomnum-=1;
			if(roomnum>=0)
			{
				Drawroom(pDC, roomnum);
			}
			break;
		}
		scrollinfo.nPos -= roomnum_len;  
		if (scrollinfo.nPos<scrollinfo.nMin)  
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break; 
		} 
		m_bar.SetScrollInfo(&scrollinfo,SIF_ALL);
		ScrollWindow(0,10); 
		roomnum-=1;
		Drawroom(pDC, roomnum);
		break;  
	case SB_PAGEDOWN: //单击滑块下方空白区域 
		if((scrollinfo.nMax-scrollinfo.nPos)<roomnum_len)
		{
			scrollinfo.nPos=scrollinfo.nMax;
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL); 
			ScrollWindow(0,-10); 
			roomnum+=1;
			if(roomnum>=0)
			{
				Drawroom(pDC, roomnum);
			}
			break;  
		}
		scrollinfo.nPos += roomnum_len; 
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax; 
			break;  
		}  
		m_bar.SetScrollInfo(&scrollinfo,SIF_ALL); 
		ScrollWindow(0,-10); 
		roomnum+=1;
		if(roomnum>=0)
		{
			Drawroom(pDC, roomnum);
		}
		break;  
	case SB_THUMBPOSITION:  // 滑动块

		int num1;
		num1=nPos/roomnum_len;
		if(num1==0)
		{
			roomnum=num1;
			Drawroom(pDC,roomnum);
			scrollinfo.nPos =0;   
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL);
		}
		if((nPos+600/p_roomnum-1)==scrollinfo.nMax)
		{
			roomnum=num1+1;
			Drawroom(pDC,roomnum);
			scrollinfo.nPos=scrollinfo.nMax;
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL);
		}
		else 
		{
			roomnum=num1;
			Drawroom(pDC,roomnum);
			scrollinfo.nPos = roomnum_len*num1;   
			m_bar.SetScrollInfo(&scrollinfo,SIF_ALL);
		}
		break;  
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}


void CShow::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	CString str;
	CDC *pDC=GetDC();
	CDlgDataCenterMap  roomdraw;
	if(point.x>0&&point.x<rect.Width()-15&&point.y>0&&point.y<(rect.Height()-60)/4)
	{
		if(DCmember[roomnum].name.GetLength())
		{
			str.Empty();
			roomdraw.Drawroom(DCmember[roomnum].name);
		}
	}
	else if(point.x>0&&point.x<rect.Width()-15&&point.y>(rect.Height()-60)/4&&point.y<(rect.Height()-60)/2)
	{
		if(DCmember[roomnum+1].name.GetLength())
		{
			str.Empty();
			roomdraw.Drawroom(DCmember[roomnum+1].name);
		}
	}
	else if(point.x>0&&point.x<rect.Width()-15&&point.y>(rect.Height()-60)/2&&point.y<3*(rect.Height()-60)/4)
	{
		if(DCmember[roomnum+2].name.GetLength())
		{
			str.Empty();
			roomdraw.Drawroom(DCmember[roomnum+2].name);
		}
	}
	else if(point.x>0&&point.x<rect.Width()-15&&point.y>3*(rect.Height()-60)/4&&point.y<rect.Height()-60)
	{
		if(DCmember[roomnum+3].name.GetLength())
		{
			str.Empty();
			roomdraw.Drawroom(DCmember[roomnum+3].name);
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CShow::Draw_room(int change)
{
	draw_rm=change;
	draw_scroll=true;

}

BOOL CShow::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialog::PreCreateWindow(cs);
}
