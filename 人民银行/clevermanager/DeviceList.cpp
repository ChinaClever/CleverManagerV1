// DeviceList.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "DeviceList.h"
#include "atlimage.h"
#include "Meter.h"
#include "MakeSureDlg.h"
#include "InitClever.h"
#include <windows.h>
#include "Alarm.h"
#include "MainFrm.h"
//#include   <mmsystem.h>   
//#pragma comment(lib,   "winmm.lib")   
// CDeviceList 对话框

IMPLEMENT_DYNAMIC(CDeviceList, CDialog)

CDeviceList::CDeviceList(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceList::IDD, pParent)
	, selectip(_T(""))
	, selectname(_T(""))
	, location(_T(""))
	, tp(_T(""))
	, index(0)
{
	p_select=NULL;
}

CDeviceList::~CDeviceList()
{
}

void CDeviceList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDeviceList, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CDeviceList::OnNMRclickTree1)
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDeviceList::OnTvnSelchangedTree1)
	
	ON_COMMAND(ID_DEV_DEL, &CDeviceList::OnDevDel)
	ON_COMMAND(ID_DEV_RELEASE, &CDeviceList::OnDevRelease)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	
	ON_COMMAND(ID_DEV_SETNPM, &CDeviceList::OnDevSetnpm)
	
	ON_COMMAND(ID_ADDNPM_ADD_III_TWO, &CDeviceList::OnAddnpmAddIiiTwo)
	ON_COMMAND(ID_ADDNPM2_ADDNPM, &CDeviceList::OnAddnpm2Addnpm)
	ON_COMMAND(ID_EDIT_DEVICE, &CDeviceList::OnEditDevice)
	ON_COMMAND(ID_NPM_V, &CDeviceList::OnNpmV)
	ON_COMMAND(ID_MS_PDU, &CDeviceList::OnMsPdu)
END_MESSAGE_MAP()


// CDeviceList 消息处理程序

BOOL CDeviceList::OnInitDialog()
{
	CDialog::OnInitDialog();
	cur[0] = 0;
	cur[1] = 0;
	cur[2] = 0;
	vol[0] = 0;
	vol[1] = 0;
	vol[2] = 0;
	TH[0] = 0;
	TH[1] = 0;
	TH[2] = 0;
	TH[3] = 0;
	TH[4] = 0;
	TH[5] = 0;
	energy[0]=0;
	energy[1]=0;
	energy[2]=0;
	// TODO:  在此添加额外的初始化
	p_select=NULL;
	index=0;
	m_tree.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);
	//m_tree.SetBkColor(RGB(233,240,249));
	

	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 4, 1);
	m_cImageListNormal32.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP1);
	m_cImageListNormal32.Add(&cBmp, RGB(255,255, 255));

	m_list.ModifyStyle(LVS_LIST|LVS_ICON|LVS_SMALLICON ,   //LVS_ICON 
		LVS_REPORT |LVS_SHOWSELALWAYS|LVS_AUTOARRANGE); //LVS_REPORT|
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES );
	m_list.SetImageList(&m_cImageListNormal32, LVSIL_SMALL);
	m_list.SetTextColor(RGB(64,64,64));

	m_list.InsertColumn(0,"图标",LVCFMT_CENTER,40,-1);
	m_list.InsertColumn(1,"输出位名称",LVCFMT_CENTER,50,-1);
	m_list.InsertColumn(2,"开关状态",LVCFMT_CENTER,50,-1);
	m_list.InsertColumn(3,"电流(A)",LVCFMT_CENTER,50,-1); 
	
	m_list.InsertColumn(4,"功率(kW)",LVCFMT_CENTER,50,-1);
	m_list.InsertColumn(5,"电能(kWh)",LVCFMT_CENTER,50,-1);
	m_list.InsertColumn(6,"电流下限(A)",LVCFMT_CENTER,50,-1); 
	m_list.InsertColumn(7,"电流上限(A)",LVCFMT_CENTER,50,-1);

	CHeaderCtrl * m_head=m_list.GetHeaderCtrl();
    m_list.m_colorheader.SubclassWindow(m_head->GetSafeHwnd());

	m_tree.SetImageList ( &m_cImageListNormal32,TVSIL_NORMAL );
	

	//NPMIII = m_tree.InsertItem("NPMIII",0,0,NULL);
	//NPMIV = m_tree.InsertItem("NPMIV",0,0,NULL);
	NPMV = m_tree.InsertItem("NPMV",0,0,NULL);
	//MSPDU = m_tree.InsertItem("MS-PDU",0,0,NULL);
	//for(int i=0;i<100;i++)
	//{
	//m_tree.InsertItem("192.168.1.110",1,1,NULL);
	//}


	updateList();

	int i;
	for(i=0;i<24;i++)
	{
		m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,i,"",0,LVIS_SELECTED,4,0);
	}

    SetTimer(LIST_TIMER,10000,NULL);
	SetTimer(LIST_LOG_TIMER,120000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDeviceList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(IsWindow(m_tree.m_hWnd))
	{	
		//m_tree.MoveWindow(rect.left+1,rect.top+26,rect.Width()*28/100-2,rect.Height()-187);
		GetDlgItem(IDC_EDIT1)->MoveWindow(rect.left+10,rect.Height()-185,rect.Width()*12/100,22);
		m_tree.MoveWindow(rect.left+1,rect.top+30,rect.Width()*28/100-2,rect.Height()-220);
		m_list.MoveWindow(rect.Width()*28/100+7,rect.top+1,rect.Width()*72/100-8,rect.Height()*60/100-2);	
		m_list.SetColumnWidth(1,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(2,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(3,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(4,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(5,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(6,(rect.Width()*72/100-60)/7);
		m_list.SetColumnWidth(7,(rect.Width()*72/100-60)/7);
	}
}

BOOL CDeviceList::OnEraseBkgnd(CDC* pDC)
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
	//::SetStretchBltMode(BkDc.GetSafeHdc(),COLORONCOLOR);
	//CImage img; 
	//HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径）
	//drawFont(&BkDc,1,RGB(0,0,0));
	//BkDc.TextOut(15,8,"区域温度云分布图");

	//drawImagePNG(&BkDc,getFilePath()+"\\icon\\canbin.png",240,2,35,25);
	CPen  pen(PS_SOLID,1,RGB(0,0,0));
	BkDc.SelectObject(&pen);
	drawListFrm(&BkDc);
	drawDevStaus(&BkDc);
	drawbtn(&BkDc,rect.Width()*12/100+30,rect.Height()-185,80,20,0,"查找");
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CDeviceList::drawListFrm(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.left,rect.top,rect.Width()*28/100,rect.Height()-160);
	pDC->MoveTo(rect.left,25);
	pDC->LineTo(rect.Width()*28/100,25);

	retange(pDC,rect.left+1,rect.top+1,rect.Width()*28/100-2,25);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(15,6,"NPM设备列表");
	////////////////////
	pDC->Rectangle(rect.left,rect.Height()-155,rect.Width()*28/100,rect.Height()-130);
	CBrush brush1(RGB(233,240,249));
	pDC->SelectObject(&brush1);
	pDC->Rectangle(rect.left,rect.Height()-131,rect.Width()*28/100,rect.Height());

	retange(pDC,rect.left+1,rect.Height()-154,rect.Width()*28/100-2,24);
	pDC->TextOut(15,rect.Height()-149,"NPM设备工作状态");
	drawImagePNG(pDC,getFilePath()+"\\icon\\lv.png",rect.Width()*2/100,rect.Height()-110,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\huang.png",rect.Width()*2/100,rect.Height()-80,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\hong.png",rect.Width()*2/100,rect.Height()-50,20,20);
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*10/100,rect.Height()-105,"正常");
	pDC->TextOut(rect.Width()*10/100,rect.Height()-75,"离线");
	pDC->TextOut(rect.Width()*10/100,rect.Height()-45,"异常");
}

void CDeviceList::drawDevStaus(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*28/100+6,rect.top,rect.Width(),rect.Height()*60/100);
	//pDC->MoveTo(rect.Width()*28/100+6,25);
	//pDC->LineTo(rect.Width(),25);

	//retange(pDC,rect.Width()*28/100+7,rect.top+1,rect.Width()*72/100-7,25);
	//drawFont(pDC,1,RGB(0,0,0));
	//pDC->TextOut(rect.Width()*28/100+21,6,"输出位信息");

	pDC->Rectangle(rect.Width()*28/100+6,rect.Height()*60/100+5,rect.Width(),rect.Height());
	CBrush brush1(RGB(233,240,249));
	pDC->SelectObject(&brush1);
	pDC->Rectangle(rect.Width()*28/100+6,rect.Height()*60/100+30,rect.Width(),rect.Height());
	pDC->MoveTo(rect.Width()*28/100+6,rect.Height()*60/100+30);
	pDC->LineTo(rect.Width(),rect.Height()*60/100+30);

	retange(pDC,rect.Width()*28/100+7,rect.Height()*60/100+6,rect.Width()*72/100-8,24);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(rect.Width()*28/100+21,rect.Height()*60/100+11,"单台NPM电力信息");

	drawsenser(pDC);
}

void CDeviceList::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	HTREEITEM hItem=m_tree.GetSelectedItem();
	HTREEITEM hparent,hson;
	hparent=m_tree.GetParentItem(hItem);
	hson=m_tree.GetChildItem(hItem);
	str=m_tree.GetItemText(hItem);
	if(hItem && hparent&&hson)
	{
		CMenu Popmenu;
		CMenu* pMenu;
		CPoint pt;
		::GetCursorPos(&pt);
		Popmenu.LoadMenu(IDR_DEV_LIST);
		pMenu = Popmenu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
		*pResult = 0;
		return;
	}
	else if(hItem && hparent&&!hson)
	{
		if(m_tree.GetItemText(hparent)=="NPMIII"||m_tree.GetItemText(hparent)=="NPMIV"||m_tree.GetItemText(hparent)=="NPMV"||m_tree.GetItemText(hparent)=="MS-PDU")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_DEV_LIST);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
		else
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_RELEASE_ALARM);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
	}
	else if(hItem && !hparent&&hson)
	{
		if(m_tree.GetItemText(hItem)=="NPMIII")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_III);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="NPMIV")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_IV);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="NPMV")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_V);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="MS-PDU")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_MSPDU_ADD);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		return;
	}
	else if(hItem && !hparent&&!hson)
	{
		if(m_tree.GetItemText(hItem)=="NPMIII")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_III);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="NPMIV")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_IV);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="NPMV")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_NPM_ADD_V);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		else if(m_tree.GetItemText(hItem)=="MS-PDU")
		{
			CMenu Popmenu;
			CMenu* pMenu;
			CPoint pt;
			::GetCursorPos(&pt);
			Popmenu.LoadMenu(IDR_MSPDU_ADD);
			pMenu = Popmenu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
		}
		return;
	}
	
}

void CDeviceList::updateList(void)
{
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0&&p_Objectnow->npmtype!=1&&p_Objectnow->npmtype!=2&&p_Objectnow->npmtype!=3)
		{
			if(!checkList(NPMIII,p_Objectnow->ipaddr,p_Objectnow))
			{
				m_tree.InsertItem(p_Objectnow->ipaddr,1,1,NPMIII);
			}
		}
		if(p_Objectnow->ipaddr.GetLength() &&p_Objectnow->npmtype==1)
		{
			if(!checkList(NPMIV,p_Objectnow->ipaddr,p_Objectnow))
			{
				m_tree.InsertItem(p_Objectnow->ipaddr,0,1,NPMIV);//////////////
			}
		}
		if(p_Objectnow->ipaddr.GetLength() &&p_Objectnow->npmtype==2)
		{
			if(!checkList(NPMV,p_Objectnow->ipaddr,p_Objectnow))
			{
				m_tree.InsertItem(p_Objectnow->ipaddr,0,1,NPMV);//////////////
			}
		}
		if(p_Objectnow->ipaddr.GetLength() &&p_Objectnow->npmtype==3)
		{
			if(!checkList(MSPDU,p_Objectnow->ipaddr,p_Objectnow))
			{
				m_tree.InsertItem(p_Objectnow->ipaddr,0,1,MSPDU);//////////////
			}
		}
	}
}

bool CDeviceList::checkList(HTREEITEM hitem,CString name,CSnmpObj *p_Objectnow)
{
	HTREEITEM hSon = m_tree.GetNextItem(hitem,TVGN_CHILD);
	while(NULL != hSon)
	{
	   CString textname = m_tree.GetItemText(hSon);
	   if( textname==name && name.GetLength())
	   {
		   if(p_Objectnow->status==1)
		   {
			   m_tree.SetItemImage(hSon,2,2);
		   }
		   else if(p_Objectnow->status==2)
		   {
			   m_tree.SetItemImage(hSon,3,3);
		   }
		   else
		   {
			   m_tree.SetItemImage(hSon,1,1);
		   }
		   removeson(hSon,p_Objectnow);
		   checkSonList(hSon,p_Objectnow);
		   return true;
	   }
	   hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
	}
	return false;
}
void CDeviceList::removeson(HTREEITEM hitem,CSnmpObj *p_Objectnow)
{
	int i;
	HTREEITEM hSon = m_tree.GetNextItem(hitem,TVGN_CHILD);
	while(NULL != hSon)
	{
		CString name  = m_tree.GetItemText(hSon);
		bool flag =false;
		for(i=0;i<10;i++)
		{
			CString devname = p_Objectnow->slavename[i];
			if(i) devname.Format("副机%d： %s",i, devname);
			else devname = "主机：" + devname;
			if(devname == name)
			{
				flag =true;
			}
		}
		if(!flag)
		{
			m_tree.DeleteItem(hSon);
		}
		hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
	}
}

struct sonList
{
	CString name;
	HTREEITEM hitem;
};
void CDeviceList::checkSonList(HTREEITEM hitem,CSnmpObj *p_Objectnow)
{
	int i,j;
	sonList list[10]={0};
	HTREEITEM hSon = m_tree.GetNextItem(hitem,TVGN_CHILD);
	i=0;
	while(NULL != hSon)
	{
		if(i>9)
		{
			return;
		}
		list[i].name = m_tree.GetItemText(hSon);
		list[i].hitem =hSon;
		hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
		i++;
	}

	/////////////////////////===================== 2019 4 1 

   for(i=0; i<10;i++)
   {
	   if(p_Objectnow->slaveOffline[i]) p_Objectnow->slaveOffline[i]=1;
	   for(j=0;j<24;j++)
	   {
		   if( j >= p_Objectnow->deviceOutputNum[i] ) break;

		    if((p_Objectnow->curmin[i][j]>p_Objectnow->curout[i][j] || p_Objectnow->curmax[i][j]<p_Objectnow->curout[i][j]) && p_Objectnow->curmax[i][j]>0)
		   {
			   //if(p_Objectnow->curalarm[i][j] == 0 || p_Objectnow->curalarm[i][j] == 2 || p_Objectnow->curalarm[i][j] == 3)
			   //{
				//   if(p_Objectnow->curalarm[i][j] == 0) {p_Objectnow->curalarm[i][j] = 2; continue;}
				//   else if(p_Objectnow->curalarm[i][j] == 2) {p_Objectnow->curalarm[i][j] = 3; continue;}
				//   else if(p_Objectnow->curalarm[i][j] == 3) {p_Objectnow->curalarm[i][j] = 4; }

				   if(p_Objectnow->curalarm[i][j] == 0) 
					   {
							p_Objectnow->curalarm[i][j] = 1;
							CString detail;
							CString content;
							detail.Format("PDU IP:%s->%d, %s-%s, %.1f(%.1f-%.1f)A",p_Objectnow->ipaddr,i,p_Objectnow->slavename[i],p_Objectnow->outputname[i][j],p_Objectnow->curout[i][j],p_Objectnow->curmin[i][j],p_Objectnow->curmax[i][j]);
							content.Format("报警机房:%s,机柜:%s ",p_Objectnow->DCname,p_Objectnow->cabientname[i]);
							writeUserLog(2,7,"PDU",content,detail);
				   }
					
					p_Objectnow->slaveOffline[i] = 2;
			 //  }
		   } else {
				p_Objectnow->curalarm[i][j] = 0;
			}
	   }

		for(j=0;j<3;j++)
	   {
		    if((p_Objectnow->Tcurmin[i][j]>p_Objectnow->Tcur[i][j] || p_Objectnow->Tcurmax[i][j]<p_Objectnow->Tcur[i][j]) && p_Objectnow->Tcurmax[i][j]>0)
		   {
			   // if(p_Objectnow->Tcuralarm[i][j] == 0 || p_Objectnow->Tcuralarm[i][j] == 2 || p_Objectnow->Tcuralarm[i][j] == 3)
			  // {
				//   if(p_Objectnow->Tcuralarm[i][j] == 0) {p_Objectnow->Tcuralarm[i][j] = 2; continue;}
				//   else if(p_Objectnow->Tcuralarm[i][j] == 2) {p_Objectnow->Tcuralarm[i][j] = 3; continue;}
				//   else if(p_Objectnow->Tcuralarm[i][j] == 3) {p_Objectnow->Tcuralarm[i][j] = 4; }

				   if(p_Objectnow->Tcuralarm[i][j] == 0) 
				   {
						p_Objectnow->Tcuralarm[i][j] = 1;
						CString detail;
						CString content;
						detail.Format("PDU IP:%s->%d, %s-Line %d, %.1f(%.1f-%.1f)A",p_Objectnow->ipaddr,i,p_Objectnow->slavename[i],j+1,p_Objectnow->Tcur[i][j],p_Objectnow->Tcurmin[i][j],p_Objectnow->Tcurmax[i][j]);
						content.Format("报警机房:%s,机柜:%s",p_Objectnow->DCname,p_Objectnow->cabientname[i]);
						writeUserLog(2,7,"PDU",content,detail);
				   }
				   
				   p_Objectnow->slaveOffline[i] = 2;
				//}
		   } else {
				p_Objectnow->Tcuralarm[i][j] = 0;
			}
	   }


		if(p_Objectnow->slaveOffline[i]==0) 
		{
			if(p_Objectnow->offlinerecord[i] <= 4)//Delay
			{//Delay
				p_Objectnow->offlinerecord[i]++;//Delay
				if(5 == p_Objectnow->offlinerecord[i])//Delay
			    //if(0 == p_Objectnow->offlinerecord[i])//Normal
				{
					//p_Objectnow->offlinerecord[i] = 1;//Normal
					CString str = "",name = "";
					CString content;
					content.Format("离线机房:%s,机柜:%s",p_Objectnow->DCname,p_Objectnow->cabientname[i]);
					if(i) name = "副机";
					else name = "主机";
					if(!p_Objectnow->slavename[i].IsEmpty())
						str.Format("设备离线%s%s->%s:%d",p_Objectnow->ipaddr,p_Objectnow->slavename[i],name,i);
					else
						str.Format("设备离线%s->%s:%d",p_Objectnow->ipaddr,name,i);
					writeUserLog(2,7,"IP->"+p_Objectnow->ipaddr,content,str);
					//toemail(p_Objectnow->ipaddr,index,p_Objectnow->slavename[i]);
				}
			}//Delay
		} else {
			if(p_Objectnow->slaveOffline[i])
			p_Objectnow->offlinerecord[i]=0;
		}
   }
	/////////////////////////===================== 2019 4 1 

	for(i=0;i<10;i++)
	{
		CString name = p_Objectnow->slavename[i];
		if(name.GetLength())
		{
			bool flags = false;
			if(i) name.Format("副机%d： %s",i, name);
			else name = "主机：" + name;

			for(j=0;j<10;j++)
			{				 
				if(name == list[j].name)
				{
					if(p_Objectnow->slaveOffline[i]==1)
					{
					   m_tree.SetItemImage(list[j].hitem,2,2);
					}
					else if(p_Objectnow->slaveOffline[i]==2)
					{
					   m_tree.SetItemImage(list[j].hitem,3,3);
					}
					else
					{
					   m_tree.SetItemImage(list[j].hitem,1,1);
					}
					flags =true;
				}
			}
			if(!flags)
			{
				m_tree.InsertItem(name,1,1,hitem);
			}
		}
	}

}

void CDeviceList::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
/*	if(nIDEvent==LIST_LOG_TIMER)
	{
		   POSITION posnow = snmpobj.GetTailPosition ();
		    CSnmpObj *p_Objectnow = NULL;
			p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
           if(p_Objectnow->status == 1 || p_Objectnow->status == 2)
			{
				CInitClever init;
				init.energyWriteLogs(p_Objectnow);

				SYSTEMTIME ct;
				GetLocalTime(&ct);
				makesureDir(getFilePath()+"\\status\\"+p_Objectnow->ipaddr);
				init.writeStatusLog(p_Objectnow);
				if(p_Objectnow->recordwMinute != ct.wMinute)
				{
					init.writeStatusLog(p_Objectnow);
					p_Objectnow->recordtimer = ct.wHour;
					p_Objectnow->recordwMinute = ct.wMinute;
				}
			}
	}*/

	if(nIDEvent==LIST_TIMER)
	{
		updateList();
		//////////////////////
		CDC *pDC = GetDC();
		SetBkMode(pDC->GetSafeHdc(),TRANSPARENT);
		drawFont(pDC,0,RGB(64,64,64));
		pDC->SelectStockObject(NULL_PEN);
		CBrush brush1(RGB(233,240,249));
		pDC->SelectObject(&brush1);
		pDC->Rectangle(rect.Width()*20/100-5,rect.Height()-120,rect.Width()*28/100-5,rect.Height()-10);

		int on =0,off=0,alarm =0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Objectnow = NULL;
			p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0)
			{
				int i;
				for(i=0;i<10;i++)
				{
					if(p_Objectnow->slaveOffline[i]==0 && p_Objectnow->slavename[i].GetLength())
					{
						off++;
					}
					else if(p_Objectnow->slaveOffline[i]==1 && p_Objectnow->slavename[i].GetLength())
					{
						on++;
					}
					else if(p_Objectnow->slaveOffline[i]==2 && p_Objectnow->slavename[i].GetLength())
					{
						alarm++;
					}
				}
			}
			///点能日志
			if(p_Objectnow->status == 1 || p_Objectnow->status == 2)
			{
				CInitClever init;
				init.energyWriteLogs(p_Objectnow);

				SYSTEMTIME ct;
				GetLocalTime(&ct);
				makesureDir(getFilePath()+"\\status\\"+p_Objectnow->ipaddr);
/*
				if(p_Objectnow->recordwMinute != ct.wMinute)
				{*/
					init.writeStatusLog(p_Objectnow);
/*					p_Objectnow->recordtimer = ct.wHour;
					p_Objectnow->recordwMinute = ct.wMinute;
				}*/
			}
		}
		CString value;
		value.Format("%d",on);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-105,value);
		value.Empty();
		value.Format("%d",off);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-75,value);
		value.Empty();
		value.Format("%d",alarm);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-45,value);
		ReleaseDC(pDC);

		insert_m_list();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDeviceList::insertList(CString ip,CString slavename)
{
	int i,j;
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->ipaddr==ip && p_Objectnow->dtype==0)
		{
			for(i=0;i<10;i++)
			{
				if(p_Objectnow->slavename[i] == slavename && slavename.GetLength())
				{
					m_list.DeleteAllItems();
					for(j=0;j<24;j++)
					{
						if(p_Objectnow->outputname[i][j].GetLength() && j<p_Objectnow->deviceOutputNum[i])
						{
							if(p_Objectnow->status==0)
							{
								m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,1,0);
							}
							else if((p_Objectnow->curmin[i][j]>p_Objectnow->curout[i][j] ||
								p_Objectnow->curmax[i][j]<p_Objectnow->curout[i][j]) && p_Objectnow->curmax[i][j]>0)
							{
								m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,3,0);
							}
							else if(p_Objectnow->outputstatus[i][j]=="--")
							{
								m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,0,0);
							}
							else if(p_Objectnow->outputstatus[i][j]=="OFF")
							{
								m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,0,0);
							}
							else
							{
								m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,2,0);
							}
							m_list.SetItemText(j,1,p_Objectnow->outputname[i][j]);
							m_list.SetItemText(j,2,"开关状态:"+p_Objectnow->outputstatus[i][j]);
							CString value;
							value.Format("电流:%.1f A",p_Objectnow->curout[i][j]);
							m_list.SetItemText(j,3,value);
							
							value.Empty();

							if(j<8)
							{
								value.Format("功率:%.3f kW",p_Objectnow->curout[i][j]*p_Objectnow->Tvol[i][0]/1000);
							}
							if(j<16 && j>7 && p_Objectnow->Tvol[i][1]>0)
							{
								value.Format("功率:%.3f kW",p_Objectnow->curout[i][j]*p_Objectnow->Tvol[i][1]/1000);
							}
							else
							{
								value.Format("功率:%.3f kW",p_Objectnow->curout[i][j]*p_Objectnow->Tvol[i][0]/1000);
							}
							if(j<24 && j>15 && p_Objectnow->Tvol[i][2]>0)
							{
								value.Format("功率:%.3f kW",p_Objectnow->curout[i][j]*p_Objectnow->Tvol[i][2]/1000);
							}
							else
							{
								value.Format("功率:%.3f kW",p_Objectnow->curout[i][j]*p_Objectnow->Tvol[i][0]/1000);
							}
							m_list.SetItemText(j,4,value);


							value.Empty();
							if(j<8)
							{
								value.Format("电能:%.3f ",p_Objectnow->energy[i][j]);
							}
							if(j<16 && j>7 && p_Objectnow->Tvol[i][1]>0)
							{
								value.Format("电能:%.3f",p_Objectnow->energy[i][j]);
							}
							else
							{
								value.Format("电能:%.3f",p_Objectnow->energy[i][j]);
							}
							if(j<24 && j>15 && p_Objectnow->Tvol[i][2]>0)
							{
								value.Format("电能:%.3f",p_Objectnow->energy[i][j]);
							}
							else
							{
								value.Format("电能:%.3f",p_Objectnow->energy[i][j]);
							}
							m_list.SetItemText(j,5,value);

							value.Empty();
							value.Format("下限:%.1f A",p_Objectnow->curmin[i][j]);
							m_list.SetItemText(j,6,value);
							value.Empty();
							value.Format("上限:%.1f A",p_Objectnow->curmax[i][j]);
							m_list.SetItemText(j,7,value);

						}
					}
					cur[0] = p_Objectnow->Tcur[i][0];
					cur[1] = p_Objectnow->Tcur[i][1];
					cur[2] = p_Objectnow->Tcur[i][2];
					vol[0] = p_Objectnow->Tvol[i][0];
					vol[1] = p_Objectnow->Tvol[i][1];
					vol[2] = p_Objectnow->Tvol[i][2];
					TH[0] = p_Objectnow->TH[i][0];
					TH[1] = p_Objectnow->TH[i][1];
					TH[2] = p_Objectnow->TH[i][2];
					TH[3] = p_Objectnow->TH[i][3];
					TH[4] = p_Objectnow->TH[i][4];
					TH[5] = p_Objectnow->TH[i][5];
					tp = p_Objectnow->devicetype[i];
					return;
				}
			}
		}
	}
}

void CDeviceList::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int i;
	CRect rect;
	GetClientRect(&rect);
	HTREEITEM hItem=m_tree.GetSelectedItem();
	HTREEITEM hparent;
	hparent=m_tree.GetParentItem(hItem);
	if(hItem || !m_tree.GetParentItem(hItem))
	{
		if(!m_tree.GetParentItem(hItem))
		{
			currentIP1 = m_tree.GetItemText(hItem);
			return;
		}
		if ((m_tree.GetItemText(hparent)=="NPMIII")||(m_tree.GetItemText(hparent)=="NPMIV")||(m_tree.GetItemText(hparent)=="NPMV")||(m_tree.GetItemText(hparent)=="MS-PDU"))
		{
			return;
		}
		
	}
	currentIP1 = m_tree.GetItemText(m_tree.GetParentItem(hItem));
	currentIP2.Empty();
	tp.Empty();
	location.Empty();

	selectip = m_tree.GetItemText(m_tree.GetParentItem(hItem));
	selectname = m_tree.GetItemText(hItem);
	//insertList(m_tree.GetItemText(m_tree.GetParentItem(hItem)),m_tree.GetItemText(hItem));
	//////////////////////////////////////
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->ipaddr==selectip && p_Objectnow->dtype==0)
		{
			for(i=0;i<10;i++)
			{
				CString name = p_Objectnow->slavename[i];
				if(i) name.Format("副机%d： %s",i, name);
				else name = "主机：" + name;

				if(name == selectname && selectname.GetLength())
				{
					p_select = p_Objectnow;
					index = i;
				}
			}
		}
	}
	insert_m_list();
	////////////////////////////////////////
	CDC *pDC;
	pDC = GetDC();
	
	/////////////////////////////////////////////
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(p_icon->ipMaster==selectip || p_icon->ipSlave== selectip)
				{
					location.Format("%s(%d:%d)",DCmember[i].name,p_icon->x+1,p_icon->y+1);
					/////
					SetBkMode(pDC->GetSafeHdc(),TRANSPARENT);
					CBrush brush1(RGB(233,240,249));
					pDC->SelectObject(&brush1);
					pDC->Rectangle(rect.Width()*28/100+6,rect.Height()*60/100+30,rect.Width(),rect.Height());

					drawsenser(pDC);
					ReleaseDC(pDC);
					return;
				}
			}
		}
	}
	////////////////////////////////////////////////
	SetBkMode(pDC->GetSafeHdc(),TRANSPARENT);
	CBrush brush1(RGB(233,240,249));
	pDC->SelectObject(&brush1);
	pDC->Rectangle(rect.Width()*28/100+6,rect.Height()*60/100+30,rect.Width(),rect.Height());
	drawsenser(pDC);
	ReleaseDC(pDC);
	*pResult = 0;
}

void CDeviceList::drawsenser(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*30/100,rect.Height()*66/100,"输入总电流:");
	pDC->TextOut(rect.Width()*30/100,rect.Height()*74/100,"输入总电压:");
	pDC->TextOut(rect.Width()*30/100,rect.Height()*82/100,"输入总功率:");
	pDC->TextOut(rect.Width()*30/100,rect.Height()*88/100,"总电能:");
	pDC->TextOut(rect.Width()*30/100,rect.Height()*95/100,"环境状态信息:");
	drawFont(pDC,1,RGB(64,64,192));
	CString value;
	if(vol[1]>50)
		value.Format("L1: %.1fA    L2: %.1fA    L3: %.1fA",cur[0],cur[1],cur[2]);
	else
		value.Format("L1: %.1fA",cur[0]);

	pDC->TextOut(rect.Width()*44/100,rect.Height()*68/100,value);
	value.Empty();
	if(vol[1]>50)
		value.Format("L1: %dV    L2: %dV    L3: %dV",vol[0],vol[1],vol[2]);
	else
		value.Format("L1: %dV",vol[0]);

	pDC->TextOut(rect.Width()*44/100,rect.Height()*77/100,value);

	value.Empty();
	if(vol[1]>50)
		value.Format("L1: %.2f W    L2: %.2f W    L3: %.2f W",cur[0]*vol[0],cur[1]*vol[1],cur[2]*vol[2]);
	else
		value.Format("L1: %.1f W",cur[0]*vol[0]);

	pDC->TextOut(rect.Width()*44/100,rect.Height()*82/100,value);

	value.Empty();
	if(vol[1]>50)
		value.Format("L1: %d.%d kWh    L2: %d.%d kWh    L3: %d.%d kWh",energy[0]/10,energy[0]%10,energy[1]/10,energy[1]%10,energy[2]/10,energy[2]%10);
	else
		value.Format("L1: %d.%d kWh",energy[0]/10,energy[0]%10);

	pDC->TextOut(rect.Width()*44/100,rect.Height()*88/100,value);

	CMeter meter1(0,rect.Width()*37/100,rect.Height()*66/100,rect.Width()*41/100,rect.Height()*72/100);
	meter1.SetValue((int)cur[0]);
	meter1.onDraw(pDC);

	CMeter meter2(1,rect.Width()*37/100,rect.Height()*75/100,rect.Width()*41/100,rect.Height()*81/100);
	meter2.SetValue(vol[0]);
	meter2.onDraw(pDC);

	CMeter meter3(2,rect.Width()*37/100,rect.Height()*90/100,rect.Width()*40/100,rect.Height()*98/100);
	meter3.SetValue(20);
	meter3.onDraw(pDC);

	CMeter meter4(3,rect.Width()*72/100,rect.Height()*90/100,rect.Width()*75/100,rect.Height()*98/100);
	meter4.SetValue(50);
	meter4.onDraw(pDC);

	drawFont(pDC,1,RGB(64,64,192));
	value.Empty();
	value.Format("T1: %d℃   T2: %d℃   T3: %d℃",TH[0],TH[1],TH[2]);
	pDC->TextOut(rect.Width()*44/100,rect.Height()*94/100,value);
	value.Empty();
	value.Format("H1: %d%s   H2: %d%s   H3: %d%s",TH[3],"%",TH[4],"%",TH[5],"%");
	pDC->TextOut(rect.Width()*78/100,rect.Height()*94/100,value);
	/////////////////////////

	pDC->TextOut(rect.Width()*78/100+70,rect.Height()*70/100,selectip);
	pDC->TextOut(rect.Width()*78/100+75,rect.Height()*74/100,selectname);
	char devicetp[32];
	memset(devicetp,0,sizeof(char)*32);
	strncpy_s(devicetp,sizeof(char)*32,tp,sizeof(char)*30);
	pDC->TextOut(rect.Width()*78/100+75,rect.Height()*78/100,devicetp);
	pDC->TextOut(rect.Width()*78/100+75,rect.Height()*82/100,location);
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*78/100,rect.Height()*70/100,"设备IP地址:");
	pDC->TextOut(rect.Width()*78/100,rect.Height()*74/100,"设备名称:");
	pDC->TextOut(rect.Width()*78/100,rect.Height()*78/100,"设备类型:");
	pDC->TextOut(rect.Width()*78/100,rect.Height()*82/100,"设备位置:");
}

void CDeviceList::OnDevDel()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(10);
	sure.DoModal();
}

void CDeviceList::OnDevRelease()
{
	// TODO: 在此添加命令处理程序代码
	int i;
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(17);
	ipselect = NULL;
	slaveselect = -1;
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		HTREEITEM hItem=m_tree.GetSelectedItem();
	    CString ip = m_tree.GetItemText(m_tree.GetParentItem(hItem));
		CString slave = m_tree.GetItemText(hItem);
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->ipaddr==ip && p_Objectnow->dtype==0)
		{
			ipselect = p_Objectnow;
			for(i=0;i<10;i++)
			{
				if(p_Objectnow->slavename[i] == slave && slave.GetLength())
				{
					slaveselect = i;
					break;
				}
			}
		}
	}
	sure.DoModal();
}

void CDeviceList::drawbtn(CDC* pDC,int x,int y,int w,int h,int icon,CString name)
{
	CBrush brush1(RGB(240,240,240)),*p_old;
	p_old = pDC->SelectObject(&brush1);
	CPen  pen(PS_SOLID,1,RGB(0,0,0));
	pDC->SelectObject(&pen);
	pDC->Rectangle(x,y,x+w,y+h);
	pDC->SelectObject(p_old);
	drawImagePNG(pDC,getFilePath()+"\\icon\\search.png",x+8,y+4,12,12);
	pDC->TextOut(x+30,y+4,name);
}

void CDeviceList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	if(point.x< rect.Width()*12/100+110&& point.x> rect.Width()*12/100+30 
		&& point.y<rect.Height()-165  && point.y> rect.Height()-185)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDeviceList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	if(point.x< rect.Width()*12/100+110&& point.x> rect.Width()*12/100+30 
		&& point.y<rect.Height()-165  && point.y> rect.Height()-185)
	{
		CString ip;
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(ip);
		if(ip.GetLength())
		{
			HTREEITEM hSon = m_tree.GetNextItem(NULL,TVGN_CHILD);
			while(NULL != hSon)
			{
				CString textname = m_tree.GetItemText(hSon);
				if(textname == ip)
				{
					//MessageBox(ip);
					m_tree.SelectSetFirstVisible(hSon);
					m_tree.SelectItem(hSon);
					return;
				}
				hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDeviceList::insert_m_list(void)
{
	    int i;
		if(!p_select)
		{
			return;
		}
		for(i=0;i<24;i++)
		{
			LV_ITEM   lvItem={0}; 
			lvItem.mask=LVIF_IMAGE;	    
			lvItem.iItem = i; 
			if(p_select->outputname[index][i].GetLength() && i<p_select->deviceOutputNum[index])
			{
				if(p_select->slaveOffline[index] == 0)
				{

					lvItem.iImage=1;   //输出位颜色
					m_list.SetItem(&lvItem);
					//m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,"",0,LVIS_SELECTED,1,0);
				}
				else if((p_select->curmin[index][i]>p_select->curout[index][i] ||
									p_select->curmax[index][i]<p_select->curout[index][i]) && p_select->curmax[index][i]>0)
				{
					lvItem.iImage=3;
					m_list.SetItem(&lvItem);
				}
				else if(p_select->outputstatus[index][i] == "--")
				{
					lvItem.iImage=2;
					m_list.SetItem(&lvItem);
				}
				else if(p_select->outputstatus[index][i] == "OFF")
				{
					lvItem.iImage=0;
					m_list.SetItem(&lvItem);
				}
				else
				{
					lvItem.iImage=2;
					m_list.SetItem(&lvItem);
				}


			m_list.SetItemText(i,1,p_select->outputname[index][i]);
			m_list.SetItemText(i,2,""+p_select->outputstatus[index][i]);
			CString value;
			//value.Format("Current:%.1f A",p_select->curout[index][i]);
			//m_list.SetItemText(i,3,value);

			CString dtype= p_select->devicetype[index];
			dtype = dtype.Right(1);
			if(dtype=="B")
			{
				p_select->outputstatus[index][i]="--";
			}
			if(dtype=="C" || dtype=="A")
			{

				value="--";
				m_list.SetItemText(i,3,value);

				value.Empty();
				value.Format("-- ",p_select->curmin[index][i]);
				m_list.SetItemText(i,6,value);
				value.Empty();
				value.Format("--",p_select->curmax[index][i]);
				m_list.SetItemText(i,7,value);
				value.Empty();
				if(i<8)
				{
					value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
				}
				if(i<16 && i>7 && p_select->Tvol[index][1]>0)
				{
					value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][1]/1000);
				}
				else
				{
					value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
				}
				if(i<24 && i>15 && p_select->Tvol[index][2]>0)
				{
					value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][2]/1000);
				}
				else
				{
					value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
				}
				m_list.SetItemText(i,4,value);

				if(i<8)
				{
					value.Format("-- ",p_select->energy[index][i]);
				}
				if(i<16 && i>7 && p_select->Tvol[index][1]>0)
				{
					value.Format("-- ",p_select->energy[index][i]);
				}
				else
				{
					value.Format("-- ",p_select->energy[index][i]);
				}
				if(i<24 && i>15 && p_select->Tvol[index][2]>0)
				{
					value.Format("-- ",p_select->energy[index][i]);
				}
				else
				{
					value.Format("-- ",p_select->energy[index][i]);
				}
				m_list.SetItemText(i,5,value);
			}
			else
			{
				if(p_select->dtype==0&&p_select->npmtype==3)
				{
					value.Format("%.1f",p_select->curout[index][i]);
					m_list.SetItemText(i,3,value);

					value.Empty();
					value.Format("-- ",p_select->curmin[index][i]);
					m_list.SetItemText(i,4,value);
					value.Empty();
					value.Format("-- ",p_select->curmax[index][i]);
					m_list.SetItemText(i,5,value);
					value.Empty();
					if(i<8)
					{
						value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					if(i<16 && i>7 && p_select->Tvol[index][1]>0)
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][1]/1000);
					}
					else
					{
						value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					if(i<24 && i>15 && p_select->Tvol[index][2]>0)
					{
						value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][2]/1000);
					}
					else
					{
						value.Format("-- ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					m_list.SetItemText(i,6,value);
				}
				else
				{
					value.Format("%.1f ",p_select->curout[index][i]);
					m_list.SetItemText(i,3,value);

					value.Empty();
					value.Format("%.1f ",p_select->curmin[index][i]);
					m_list.SetItemText(i,6,value);
					value.Empty();
					value.Format("%.1f ",p_select->curmax[index][i]);
					m_list.SetItemText(i,7,value);
					value.Empty();
					if(i<8)
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					if(i<16 && i>7 && p_select->Tvol[index][1]>0)
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][1]/1000);
					}
					else
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					if(i<24 && i>15 && p_select->Tvol[index][2]>0)
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][2]/1000);
					}
					else
					{
						value.Format("%.3f ",p_select->curout[index][i]*p_select->Tvol[index][0]/1000);
					}
					m_list.SetItemText(i,4,value);

					if(i<8)
					{
						value.Format("%d.%d",p_select->energy[index][i]/10,p_select->energy[index][i]%10);
					}
					if(i<16 && i>7 && p_select->Tvol[index][1]>0)
					{
						value.Format("%d.%d",p_select->energy[index][i]/10,p_select->energy[index][i]%10);
					}
					else
					{
						value.Format("%d.%d",p_select->energy[index][i]/10,p_select->energy[index][i]%10);
					}
					if(i<24 && i>15 && p_select->Tvol[index][2]>0)
					{
						value.Format("%d.%d",p_select->energy[index][i]/10,p_select->energy[index][i]%10);
					}
					else
					{
						value.Format("%d.%d",p_select->energy[index][i]/10,p_select->energy[index][i]%10);
					}
					m_list.SetItemText(i,5,value);

				}
			}

		}
		else
		{
			lvItem.iImage=4;
			m_list.SetItem(&lvItem);
			m_list.SetItemText(i,1,"");
			m_list.SetItemText(i,2,"");
			m_list.SetItemText(i,3,"");
			m_list.SetItemText(i,4,"");
			m_list.SetItemText(i,5,"");
			m_list.SetItemText(i,6,"");
			m_list.SetItemText(i,7,"");
		}
		cur[0] = p_select->Tcur[index][0];
		cur[1] = p_select->Tcur[index][1];
		cur[2] = p_select->Tcur[index][2];
		vol[0] = p_select->Tvol[index][0];
		vol[1] = p_select->Tvol[index][1];
		vol[2] = p_select->Tvol[index][2];
		TH[0] = p_select->TH[index][0];
		TH[1] = p_select->TH[index][1];
		TH[2] = p_select->TH[index][2];
		TH[3] = p_select->TH[index][3];
		TH[4] = p_select->TH[index][4];
		TH[5] = p_select->TH[index][5];
		energy[0]=p_select->TEnergy[index][0];
		energy[1]=p_select->TEnergy[index][1];
		energy[2]=p_select->TEnergy[index][2];
		tp = p_select->devicetype[index];
	}
}

void CDeviceList::OnDevSetnpm()//npm设置连接
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM tree,parent,son;
	CString str;
	tree=m_tree.GetSelectedItem();
	parent=m_tree.GetParentItem(tree);
	son=m_tree.GetChildItem(tree);
	if(son&&parent&&tree)
	{
		str=m_tree.GetItemText(tree);

		ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+str,NULL,NULL, SW_NORMAL);
	}
	else if(parent&&tree&&!son)
	{
		str=m_tree.GetItemText(tree);
		ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+str,NULL,NULL, SW_NORMAL);
	}
}

void CDeviceList::OnAddnpmAddIiiTwo()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(9);
	sure.DoModal();
}

void CDeviceList::OnAddnpm2Addnpm()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(22);
	sure.DoModal();
}

void CDeviceList::OnEditDevice()
{
	HTREEITEM  tree_se,parent;
	tree_se=m_tree.GetSelectedItem();
	CString pre_ip=m_tree.GetItemText(tree_se);
	parent=m_tree.GetParentItem(tree_se);
	if(m_tree.GetItemText(parent)=="NPMIII")
	{
		CMakeSureDlg sure;
		sure.pre_ip=pre_ip;
		sure.p_device = this;
		sure.setOrder(30);
		sure.DoModal();
	}
	else if(m_tree.GetItemText(parent)=="NPMIV")
	{
		CMakeSureDlg sure;
		sure.pre_ip=pre_ip;
		sure.p_device = this;
		sure.setOrder(31);
		sure.DoModal();
	}
	else if(m_tree.GetItemText(parent)=="NPMV")
	{
		CMakeSureDlg sure;
		sure.pre_ip=pre_ip;
		sure.p_device = this;
		sure.setOrder(39);
		sure.DoModal();
	}
	else if(m_tree.GetItemText(parent)=="MS-PDU")
	{
		CMakeSureDlg sure;
		sure.pre_ip=pre_ip;
		sure.p_device = this;
		sure.setOrder(38);
		sure.DoModal();
	}
}

void CDeviceList::OnNpmV()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(35);
	sure.DoModal();
}

void CDeviceList::OnMsPdu()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_device = this;
	sure.setOrder(36);
	sure.DoModal();
}

void CDeviceList::writeUserLog(int logtype,int type,CString name, CString item, CString detail)///////////////////写日志
{
	////////////////////////////////////
	name.Replace("\"","");
	item.Replace("\"","");
	detail.Replace("\"","");
	name.Replace("'","");
	item.Replace("'","");
	detail.Replace("'","");
	name.Replace("{","");
	item.Replace("{","");
	detail.Replace("{","");
	name.Replace("}","");
	item.Replace("}","");
	detail.Replace("}","");
	name.Replace("[","");
	item.Replace("[","");
	detail.Replace("[","");
	name.Replace("]","");
	item.Replace("]","");
	detail.Replace("]","");
	userlogs *logs;
	logs = new userlogs;
	logs->logtype = logtype;
	logs->type = type;
	memset(logs->name,0,sizeof(char)*32);
	strncpy_s(logs->name,sizeof(char)*32,name,sizeof(char)*30);
	memset(logs->item,0,sizeof(char)*32);
	strncpy_s(logs->item,sizeof(char)*32,item,sizeof(char)*30);
	memset(logs->detail,0,sizeof(char)*128);
	strncpy_s(logs->detail,sizeof(char)*128,detail,sizeof(char)*126);
	CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
}