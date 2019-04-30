// DlgDataCenterMap.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "DlgDataCenterMap.h"
#include "atlimage.h"
#include "CanbinObjIcon.h"
#include "CanbinNPMDlg.h"
#include "MakeSureDlg.h"
#include "FileLogEdit.h"
#include "MainFrm.h"
#include "OtherDevice.h"
#include "CabinetDlg.h"
// CDlgDataCenterMap 对话框
static CString str_roomname;
IMPLEMENT_DYNAMIC(CDlgDataCenterMap, CDialog)

CDlgDataCenterMap::CDlgDataCenterMap(CWnd* pParent /*=NULL*/)
: CDialog(CDlgDataCenterMap::IDD, pParent)
, DCname(_T(""))
, mapEdit(false)
{
	MouseRPoint.x=0;
	MouseRPoint.y=0;
}

CDlgDataCenterMap::~CDlgDataCenterMap()
{
}

void CDlgDataCenterMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDataCenterMap, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MOVECANBIN_32771, &CDlgDataCenterMap::OnMovecanbin32771)
	ON_COMMAND(ID_MOVECANBIN_32772, &CDlgDataCenterMap::OnMovecanbin32772)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MOVECANBIN_32773, &CDlgDataCenterMap::OnMovecanbin32773)
	ON_COMMAND(ID_ADD_CABIN, &CDlgDataCenterMap::OnAddCabin)
	ON_COMMAND(ID_SAVE_MAP, &CDlgDataCenterMap::OnSaveMap)
	ON_WM_TIMER()
	ON_COMMAND(ID_MOVECANBIN_EDIT, &CDlgDataCenterMap::OnMovecanbinEdit)
END_MESSAGE_MAP()


// CDlgDataCenterMap 消息处理程序


BOOL CDlgDataCenterMap::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i;
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
	//::SetStretchBltMode(BkDc.GetSafeHdc(),COLORONCOLOR);
	CImage img; 
	HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径）
	//drawImagePNG(&BkDc,getFilePath()+"\\icon\\jifang.png",10,2,25,25);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\add.png",120,8,0,0);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\remove.png",150,8,0,0);

	drawImagePNG(&BkDc,getFilePath()+"\\icon\\canbin.png",300,2,35,25);
	if(mapEdit)
		drawImagePNG(&BkDc,getFilePath()+"\\icon\\open.png",475,8,15,15);
	else
		drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",475,8,13,13);
	//drawImagePNG(&BkDc,getFilePath()+"\\icon\\remove.png",390,8,0,0);



	drawFont(&BkDc,0,RGB(0,0,0));
	BkDc.TextOut(70,8,"机房");
	BkDc.TextOut(245,8,"机柜");
	BkDc.TextOut(400,8,"地图编辑");
	BkDc.SelectStockObject(BLACK_PEN);
	rect.bottom -= 1;
	rect.right -= 1;
	BkDc.Rectangle(0,35,rect.right,rect.bottom);
	BkDc.Rectangle(0,60,rect.right,rect.bottom);


	if(str_roomname.GetLength())
	{
		Drawname();
	}

	drawFont(&BkDc,0,RGB(96,192,96));
	BkDc.TextOut(28,42,"前一页");
	BkDc.TextOut(rect.right-60,42,"后一页");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\l.jpg",2,40,15,15);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\r.jpg",rect.right-22,40,15,15);

	drawFont(&BkDc,1,RGB(0,0,0));

	CSize fontsize= BkDc.GetTextExtent(DCname);
	BkDc.TextOut(rect.Width()/2-fontsize.cx/2,40,DCname);
	//////////////////////////////////////////////////
	CRect rectdraw;
	rectdraw = rect;
	rectdraw.top=60;
	CPen  pen(PS_DASHDOTDOT,1,RGB(210,210,210));
	BkDc.SelectObject(&pen);
	for(i=1;i<20;i++)
	{
		BkDc.MoveTo(0,rectdraw.Height()*i/20+60);
		BkDc.LineTo(rectdraw.Width(),rectdraw.Height()*i/20+60);

		BkDc.MoveTo(rectdraw.Width()*i/20,60);
		BkDc.LineTo(rectdraw.Width()*i/20,rectdraw.Height()+60);
	}
	/////////////////
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
				((CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos)))->onDraw (&BkDc,rect.Width(),rect.Height()-60,true);
		}
	}
	//////////////////
	Dcname();
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	//memdc.DeleteDC();
	return true; 

	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgDataCenterMap::OnInitDialog()
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

	CFileLogEdit p_edit;
	p_edit.mapCabinRead();

	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(15,16,1,"192.168.1.112","192.168.1.112",0,0));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(10,8,1,"","",0,0,1));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,14,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,12,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,10,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,8,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,6,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,4,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,2,1,"","",0,0,2));
	//DCmember[0].canbin.AddTail(new CCanbinObjIcon(16,15,1,"","",0,0,3));

	//p_edit.mapCabinWrite(0);

	SetTimer(MAP_TIMER,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataCenterMap::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int i;
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetHeadPosition ();
			while (pos != NULL)
				delete DCmember[i].canbin.GetNext (pos);
			DCmember[i].canbin.RemoveAll ();
		}
	}
}

void CDlgDataCenterMap::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i;
	CRect rect;
	GetClientRect(&rect);
	if(point.y<60)
	{
		if(point.x>120 && point.x<140 &&
			point.y>8 && point.y<28)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
			CDC* dc;
			dc = GetDC();
			//CBrush brush(RGB(255,255,255));
			//dc->SelectObject(&brush);
			//dc->SelectStockObject(NULL_PEN);
			//dc->Rectangle(118,6,170,30);
			drawImagePNG(dc,getFilePath()+"\\icon\\add.png",118,6,18,18);
			//drawImagePNG(dc,getFilePath()+"\\icon\\remove.png",150,8,0,0);
			ReleaseDC(dc);
		}
		else if(point.x>150 && point.x<170 &&
			point.y>8 && point.y<28)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
			CDC* dc;
			dc = GetDC();
			//CBrush brush(RGB(255,255,255));
			//dc->SelectObject(&brush);
			//dc->SelectStockObject(NULL_PEN);
			//dc->Rectangle(118,6,170,30);
			//drawImagePNG(dc,getFilePath()+"\\icon\\add.png",120,8,0,0);
			drawImagePNG(dc,getFilePath()+"\\icon\\remove.png",148,6,18,18);
			ReleaseDC(dc);
		}
		else if(point.x>475 && point.x<495 &&
			point.y>8 && point.y<28)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		}
		else if(point.x>2 && point.x<67 &&
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
		else
		{
			CDC* dc;
			dc = GetDC();

			CBitmap BkMap;
			BkMap.CreateCompatibleBitmap(dc,54,24);
			CDC BkDc;
			BkDc.CreateCompatibleDC(dc);
			BkDc.SelectObject(&BkMap);

			CBrush brush(RGB(255,255,255));
			BkDc.SelectObject(&brush);
			BkDc.SelectStockObject(NULL_PEN);
			BkDc.Rectangle(-2,-2,55,25);
			drawImagePNG(&BkDc,getFilePath()+"\\icon\\add.png",2,2,0,0);
			drawImagePNG(&BkDc,getFilePath()+"\\icon\\remove.png",32,2,0,0);

			dc->BitBlt(118,6,54,24,&BkDc,0,0,SRCCOPY);
			BkDc.DeleteDC();



			ReleaseDC(dc);
		}
	}
	else if(point.y>60)
	{
		for(i=0;i<10;i++)
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
					if(p_icon->move)
					{
						p_icon->x=point.x/(rect.Width()/20);
						p_icon->y=(point.y-60)/((rect.Height()-60)/20);
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

void CDlgDataCenterMap::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!mapEdit)
	{
		return;
	}
	MouseRPoint=point;
	int i;
	CRect rect;
	GetClientRect(&rect);
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				p_icon->select =false;
				p_icon->move=false;
			}
			pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(point.x>p_icon->x*rect.Width()/20 && point.x<((p_icon->x)+1)*rect.Width()/20 &&
					point.y>p_icon->y*(rect.Height()-60)/20+60 && point.y<(p_icon->y+1)*(rect.Height()-60)/20+60)
				{
					CMenu Popmenu;
					CMenu* pMenu;
					CPoint pt;
					::GetCursorPos(&pt);
					Popmenu.LoadMenu(IDR_MOVE_CANBIN);
					pMenu = Popmenu.GetSubMenu(0);
					pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
					p_icon->select = true;
					return;	
				}
			}
		}
	}

	CMenu Popmenu1;
	CMenu* pMenu1;
	CPoint pt;
	::GetCursorPos(&pt);
	Popmenu1.LoadMenu(IDR_ADD_CABIN);
	pMenu1 = Popmenu1.GetSubMenu(0);
	pMenu1->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);


	CDialog::OnRButtonDown(nFlags, point);
}

void CDlgDataCenterMap::OnMovecanbin32771()
{
	// TODO: 在此添加命令处理程序代码
	int i;
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(p_icon->select)
				{
					p_icon->move=true;
					break;	
				}
			}
		}
	}
}

void CDlgDataCenterMap::OnMovecanbin32772()
{
	// TODO: 在此添加命令处理程序代码
	int i;
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				p_icon->select =false;
				p_icon->move=false;
				OnSaveMap();
			}
		}
	}
}
void CDlgDataCenterMap::OnMovecanbin32773()//删除
{
	// TODO: 在此添加命令处理程序代码
	int i;
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{	
			POSITION pos = DCmember[i].canbin.GetHeadPosition (),pos_old=NULL;
			CCanbinObjIcon* p_icon;
			do
			{
				pos_old = pos;
				p_icon = (CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
				if(p_icon->select)
				{
					int x,y;
					x = p_icon->x;
					y = p_icon->y;
					delete p_icon;
					DCmember[i].canbin.RemoveAt(pos_old);
					//////////////
					userlogs *logs;
					logs = new userlogs;
					logs->logtype = 1;
					logs->type = 3;
					memset(logs->name,0,sizeof(char)*32);
					memset(logs->item,0,sizeof(char)*32);
					strncpy_s(logs->item,sizeof(char)*32,"删除机柜",sizeof(char)*30);
					memset(logs->detail,0,sizeof(char)*128);
					CString del;
					CMakeSureDlg dlg;
					strncpy_s(logs->name,sizeof(char)*32,CMakeSureDlg::username,sizeof(char)*30);
					del.Format("机房(%s) 在位置(%d:%d)的机柜被删除。.",DCmember[i].name,x,y);
					strncpy_s(logs->detail,sizeof(char)*128,del,sizeof(char)*126);
					CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
					::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
				}
			}
			while(pos != NULL);
		}
	}
}


void CDlgDataCenterMap::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int i;
	CRect rect;
	GetClientRect(&rect);
	for(i=0;i<10;i++)
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
					if(p_icon->type==0)
					{
						
						CCanbinNPMDlg dlg;
						dlg.p_icon = p_icon;
						dlg.mapEdit = mapEdit;
						dlg.DoModal();
						
						//CCabinetDlg dlg;
						//dlg.DoModal();
						break;	
					}
					else if(p_icon->type==1)
					{
						COtherDevice dlg;
						dlg.type = 1;
						dlg.DoModal();
						break;	
					}
					else if(p_icon->type==2)
					{
						COtherDevice dlg;
						dlg.type = 2;
						dlg.DoModal();
						break;	
					}
					else if(p_icon->type==3)
					{
						ShellExecute(NULL,NULL, //"http://"+
							getFilePath()+"\\www\\cam.html", NULL,NULL,SW_NORMAL);
					}
				}
			}
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDlgDataCenterMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i,j;
	CRect rect;
	GetClientRect(&rect);
	if(point.y<60)
	{
		if(point.x>120 && point.x<140 &&
			point.y>8 && point.y<28)
		{
			CMakeSureDlg dlg;
			dlg.setOrder(1);
			dlg.DoModal();
		}
		else if(point.x>150 && point.x<170 &&
			point.y>8 && point.y<28)
		{
			CMakeSureDlg dlg;
			dlg.setOrder(2);
			dlg.DCname=DCname;
			dlg.DoModal();
			for(i=0;i<10;i++)
			{
				if(DCmember[i].name==DCname && DCmember[i].name.GetLength())
				{
					return;
				}
			}
			if(i>9)
			{
				for(i=0;i<10;i++)
				{
					if(DCmember[i].name.GetLength())
					{
						DCname=DCmember[i].name;
						str_roomname=DCmember[i].name;
						return;
					}
				}
			}
		}
		if(point.x>475 && point.x<495 &&
			point.y>8 && point.y<28)
		{
			bool flag = mapEdit;
			CMakeSureDlg dlg;
			if(flag==false)
			{
				dlg.setOrder(3);
				dlg.p_map =this;
				dlg.DoModal();
				CFileLogEdit p_edit;
				if( flag !=mapEdit  && !mapEdit)
				{
					for(i=0;i<10;i++)
					{
						if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
						{
							p_edit.mapCabinWrite(i);
						}
					}
				}
			}
			else
			{
				dlg.setOrder(23);
				dlg.p_map =this;
				dlg.DoModal();
				CFileLogEdit p_edit;
				if( flag !=mapEdit  && !mapEdit)
				{
					for(i=0;i<10;i++)
					{
						if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
						{
							p_edit.mapCabinWrite(i);
						}
					}
				}
			}
		}
		else if(point.x>2 && point.x<67 &&
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
							str_roomname=DCmember[i-j-1<0?i-j+9:i-j-1].name;
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
						str_roomname=DCmember[i].name;
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
							str_roomname=DCmember[i+j+1>9?i+j-9:i+j+1].name;
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
						str_roomname=DCmember[i].name;
						return;
					}
				}
			}
		}
	}
	else if(point.y>60)
	{

	}
	CDialog::OnLButtonDown(nFlags, point);
}



void CDlgDataCenterMap::OnAddCabin()
{
	CMakeSureDlg dlg;
	dlg.setOrder(4);
	int i;
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			dlg.datercenterNUM = i;
		}
	}
	dlg.DoModal();
}

void CDlgDataCenterMap::OnSaveMap()
{
	// TODO: 在此添加命令处理程序代码
	CFileLogEdit p_edit;
	int i;
	for(i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			p_edit.mapCabinWrite(i);
		}
	}
}

void CDlgDataCenterMap::Status()
{
	for(int i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon =NULL;
				p_icon= (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(p_icon->ipMaster.GetLength()||p_icon->ipSlave.GetLength())
				{
					int status1,status2;
					status1=FindStatus_M(p_icon);
					status2=FindStatus_S(p_icon);
					SetStatus(status1,status2,p_icon);
				}
				else if(p_icon->ipMaster.GetLength())
				{
					int status1;
					status1=FindStatus_M(p_icon);
					if(status1==1)
						p_icon->status=1;
					else if(status1==2)
						p_icon->status=2;
					else if(status1==3)
						p_icon->status=3;
				}
			}
		}
	}
}

void CDlgDataCenterMap::SetStatus(int status1,int status2,CCanbinObjIcon* p_icon)
{
	if(status1==1)
	{
		if(status2==1)
		{
			p_icon->status=1;
		}
		else if(status2==3)
		{
			p_icon->status=4;
		}
		else if(status2==2)
		{
			p_icon->status=8;
		}
		else if(status2==0)
		{
			p_icon->status=10;
		}
	}
	else if(status1==3)
	{
		if(status2==1)
		{
			p_icon->status=5;
		}
		else if(status2==3)
		{
			p_icon->status=3;
		}
		else if(status2==2)
		{
			p_icon->status=9;
		}
		else if(status2==0)
		{
			p_icon->status=11;
		}
	}
	else if(status1==2)
	{
		if(status2==1)
		{
			p_icon->status=6;
		}
		else if(status2==3)
		{
			p_icon->status=7;
		}
		else if(status2==2)
		{
			p_icon->status=2;
		}
		else if(status2==0)
		{
			p_icon->status=12;
		}
	}
	else if(status1==0)
	{
		if(status2==1)
		{
			p_icon->status=13;
		}
		else if(status2==3)
		{
			p_icon->status=14;
		}
		else if(status2==2)
		{
			p_icon->status=15;
		}
		else if(status2==0)
		{
			p_icon->status=16;
		}
	}
}


int CDlgDataCenterMap::FindStatus_M(CCanbinObjIcon * p_icon)
{
	POSITION posObj = snmpobj.GetTailPosition ();
	while (posObj != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posObj)));
		if(p_Object->ipaddr.GetLength() && p_Object->dtype==0)
		{
			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				if(p_Object->slaveOffline[p_icon->MasterNUM]==0)
					return 3;
				else if(p_Object->slaveOffline[p_icon->MasterNUM]==1)
					return 1;
				else if(p_Object->slaveOffline[p_icon->MasterNUM]==2)
					return 2;
			}
		}
	}
	return 0;
}

int CDlgDataCenterMap::FindStatus_S(CCanbinObjIcon * p_icon)
{
	POSITION posObj = snmpobj.GetTailPosition ();
	while (posObj != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posObj)));
		if(p_Object->ipaddr.GetLength() && p_Object->dtype==0)
		{
			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				if(p_Object->slaveOffline[p_icon->SlaveNUM]==0)
					return 3;
				else if(p_Object->slaveOffline[p_icon->SlaveNUM]==1)
					return 1;
				else if(p_Object->slaveOffline[p_icon->SlaveNUM]==2)
					return 2;
			}
		}
	}
	return 0;
}

void CDlgDataCenterMap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == MAP_TIMER)
	{
		Status();
		int i;
		int tmp = 0;
		int tmpcouter = 0;
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength())
			{
				POSITION pos = DCmember[i].canbin.GetTailPosition ();
				while (pos != NULL)
				{
					CCanbinObjIcon* p_icon =NULL;
					p_icon= (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
					POSITION posObj = snmpobj.GetTailPosition ();
					bool snmpobjFlags = false;
					CSnmpObj *p_pdu1 = NULL;
					CSnmpObj *p_pdu2 = NULL;
					while (posObj != NULL)
					{
						CSnmpObj *p_Object = NULL;
						p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posObj)));
						if(p_Object->ipaddr.GetLength() && p_Object->dtype==0)
						{
							
							if(p_icon->ipMaster == p_Object->ipaddr)
							{
								p_pdu1 = p_Object;
								snmpobjFlags = true;
								p_Object->DCname = DCmember[i].name;
								p_Object->cabientname[p_icon->MasterNUM] = p_icon->cabientname;

							}
						    if(p_icon->ipSlave == p_Object->ipaddr)
							{
								p_pdu2 = p_Object;
								snmpobjFlags = true;
								p_Object->DCname = DCmember[i].name;
								p_Object->cabientname[p_icon->SlaveNUM] = p_icon->cabientname;
							}
							
						}
					}
					////////////////////////////////////
					tmp = 0;
					tmpcouter = 0;
					if(p_pdu1)
					{
						if(p_pdu1->TH[p_icon->MasterNUM][0]>0)
						{
							tmp += p_pdu1->TH[p_icon->MasterNUM][0];
							tmpcouter++;
						}
						if(p_pdu1->TH[p_icon->MasterNUM][1]>0)
						{
							tmp += p_pdu1->TH[p_icon->MasterNUM][1];
							tmpcouter++;
						}
						if(p_pdu1->TH[p_icon->MasterNUM][2]>0)
						{
							tmp += p_pdu1->TH[p_icon->MasterNUM][2];
							tmpcouter++;
						}
					}
					if(p_pdu2)
					{
						if(p_pdu2->TH[p_icon->SlaveNUM][0]>0)
						{
							tmp += p_pdu2->TH[p_icon->SlaveNUM][0];
							tmpcouter++;
						}
						if(p_pdu2->TH[p_icon->SlaveNUM][1]>0)
						{
							tmp += p_pdu2->TH[p_icon->SlaveNUM][1];
							tmpcouter++;
						}
						if(p_pdu2->TH[p_icon->SlaveNUM][2]>0)
						{
							tmp += p_pdu2->TH[p_icon->SlaveNUM][2];
							tmpcouter++;
						}
					}
#ifndef DEMO
					if(tmpcouter>0)
					{
						p_icon->temp = tmp/tmpcouter;
					}
					else
					{
					    p_icon->temp = 0;
					}
#endif
					///////////////////////////////////////
					if(!snmpobjFlags)
					{
						p_icon->status = 0;
					}
				}
			}
		}

		CDC *pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC); 
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgDataCenterMap::Dcname()
{
	CCanbinNPMDlg aa;
	aa.Dcname(DCname);
}

void CDlgDataCenterMap::Drawroom(CString roomname)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			if(DCmember[i].name==roomname)
			{
				str_roomname=DCmember[i].name;
				
				return;
			}
			
		}
	}
}

void CDlgDataCenterMap::Drawname()
{
	DCname=str_roomname;
}
void CDlgDataCenterMap::OnMovecanbinEdit()
{
	// TODO: 在此添加命令处理程序代码
	CPoint point=MouseRPoint;
	CRect rect;
	GetClientRect(&rect);
	for(int i=0;i<10;i++)
	{
		if(DCname == DCmember[i].name && DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				p_icon->select =false;
				p_icon->move=false;
			}
			pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(point.x>p_icon->x*rect.Width()/20 && point.x<((p_icon->x)+1)*rect.Width()/20 &&
					point.y>p_icon->y*(rect.Height()-60)/20+60 && point.y<(p_icon->y+1)*(rect.Height()-60)/20+60)
				{
					CMakeSureDlg sure;
					sure.p_icon=p_icon;
					sure.cabinet_name=p_icon->cabientname;
					sure.ip_master=p_icon->ipMaster;
					sure.ip_slave=p_icon->ipSlave;
					sure.slave_masternum=p_icon->MasterNUM;
					sure.slave_slavenum=p_icon->SlaveNUM;
					p_icon->select=true;
					sure.DCname=DCname;
					sure.setOrder(34);
					sure.DoModal();
				}
			}
		}
	}
}
