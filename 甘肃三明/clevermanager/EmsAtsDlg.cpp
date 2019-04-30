// EmsAtsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "EmsAtsDlg.h"
#include "atlimage.h" 
#include "MakeSureDlg.h"
//#include "SnmpObj.h"

// CEmsAtsDlg 对话框

IMPLEMENT_DYNAMIC(CEmsAtsDlg, CDialog)

CEmsAtsDlg::CEmsAtsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmsAtsDlg::IDD, pParent)
	, normal(0)
	, offline(0)
	, alarm(0)
	, index(0)
{
	p_select=NULL;
	SelectIP=0;
}

CEmsAtsDlg::~CEmsAtsDlg()
{
}

void CEmsAtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CEmsAtsDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CEmsAtsDlg::OnTvnSelchangedTree1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CEmsAtsDlg::OnNMRclickTree1)
	ON_COMMAND(ID_EMSATS_RELEASEALARM, &CEmsAtsDlg::OnEmsatsReleasealarm)
	//ON_COMMAND(ID_EMSATS_ADDEMS, &CEmsAtsDlg::OnEmsatsAddems)
	ON_COMMAND(ID_EMSATS_ADDATS, &CEmsAtsDlg::OnEmsatsAddats)
	ON_COMMAND(ID_EMSATS_DELETE, &CEmsAtsDlg::OnEmsatsDelete)
	ON_COMMAND(ID_EMSATS_SET, &CEmsAtsDlg::OnEmsatsSet)
	ON_COMMAND(ID_EMSATS_ADDEMS_T, &CEmsAtsDlg::OnEmsatsAddemsT)
	//ON_COMMAND(ID_EMSATS_ADDATS_T, &CEmsAtsDlg::OnEmsatsAddatsT)
	ON_COMMAND(ID_EMSATS_DELETEDEV_T, &CEmsAtsDlg::OnEmsatsDeletedevT)
	ON_COMMAND(ID_EMSATS_DELALARM_T, &CEmsAtsDlg::OnEmsatsDelalarmT)
	ON_COMMAND(ID_ALARM_EMS, &CEmsAtsDlg::OnAlarmEms)
	ON_COMMAND(ID_EMS_ADD, &CEmsAtsDlg::OnEmsAdd)
	ON_COMMAND(ID_ATS_ADD, &CEmsAtsDlg::OnAtsAdd)
	ON_COMMAND(ID_EMS_DELETE, &CEmsAtsDlg::OnEmsDelete)
	ON_COMMAND(ID_EMS_SET, &CEmsAtsDlg::OnEmsSet)
	ON_COMMAND(ID_EMSATS_EDIT, &CEmsAtsDlg::OnEmsatsEdit)
	ON_COMMAND(ID_EMS_CHANGE, &CEmsAtsDlg::OnEmsChange)
	ON_COMMAND(ID_IP_PDU, &CEmsAtsDlg::OnIpPdu)
	ON_COMMAND(ID_IP_DEL, &CEmsAtsDlg::OnIpDel)
	ON_COMMAND(ID_IP_SET, &CEmsAtsDlg::OnIpSet)
	ON_COMMAND(ID_IP_EDIT, &CEmsAtsDlg::OnIpEdit)
	ON_COMMAND(ID_IP_ALARM, &CEmsAtsDlg::OnIpAlarm)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CEmsAtsDlg 消息处理程序

BOOL CEmsAtsDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	////////////////创建一个图片的绘图环境///////////////////////
    CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	CImage img; 
	HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径） 
	/////////////////////////////////
	SetBkMode(BkDc,TRANSPARENT);
    CBrush brush(RGB(255,255,255));
	BkDc.SelectObject(&brush);
	BkDc.SelectStockObject(NULL_PEN);
	rect.bottom += 1;
	rect.right += 1;
	BkDc.Rectangle(&rect);

	CPen  pen(PS_SOLID,1,RGB(0,0,0));
	BkDc.SelectObject(&pen);
	drawListFrm(&BkDc);
	drawDevStaus(&BkDc);
	
	if(p_select)
	{
		if(p_select->dtype==1)
		{
			drawEMS(&BkDc);
		}
		else if(p_select->dtype==2)
		{
			drawATS(&BkDc);
		}
		else if(p_select->dtype==3)
		{
			drawIPdata(&BkDc);
		}
	}
	drawFont(pDC,0,RGB(64,64,64));
	CString value;
	value.Format("%d",normal);
	BkDc.TextOut(rect.Width()*20/100,rect.Height()-105,value);
	value.Empty();
	value.Format("%d",offline);
	BkDc.TextOut(rect.Width()*20/100,rect.Height()-75,value);
	value.Empty();
	value.Format("%d",alarm);
	BkDc.TextOut(rect.Width()*20/100,rect.Height()-45,value);
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CEmsAtsDlg::drawListFrm(CDC *pDC)
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
	pDC->TextOut(15,6,"EMS&ATS设备列表");
	////////////////////
	pDC->Rectangle(rect.left,rect.Height()-155,rect.Width()*28/100,rect.Height()-130);
	CBrush brush1(RGB(255,255,255));
	pDC->SelectObject(&brush1);
	pDC->Rectangle(rect.left,rect.Height()-131,rect.Width()*28/100,rect.Height());

	retange(pDC,rect.left+1,rect.Height()-154,rect.Width()*28/100-2,24);
	pDC->TextOut(15,rect.Height()-149,"设备工作状态");
	drawImagePNG(pDC,getFilePath()+"\\icon\\lv.png",rect.Width()*2/100,rect.Height()-110,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\huang.png",rect.Width()*2/100,rect.Height()-80,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\hong.png",rect.Width()*2/100,rect.Height()-50,20,20);
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*10/100,rect.Height()-105,"正常");
	pDC->TextOut(rect.Width()*10/100,rect.Height()-75,"离线");
	pDC->TextOut(rect.Width()*10/100,rect.Height()-45,"异常");
}

void CEmsAtsDlg::drawDevStaus(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*28/100+6,rect.top,rect.Width(),rect.Height()*60/100);
	pDC->MoveTo(rect.Width()*28/100+6,25);
	pDC->LineTo(rect.Width(),25);

	retange(pDC,rect.Width()*28/100+7,rect.top+1,rect.Width()*72/100-7,25);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(rect.Width()*28/100+21,6,"工作状态信息");

	pDC->Rectangle(rect.Width()*28/100+6,rect.Height()*60/100+5,rect.Width(),rect.Height());
}

BOOL CEmsAtsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_select=NULL;
	// TODO:  在此添加额外的初始化
	m_list.ModifyStyle(LVS_LIST|LVS_ICON|LVS_SMALLICON ,   //LVS_ICON 
		LVS_REPORT |LVS_SHOWSELALWAYS|LVS_AUTOARRANGE); //LVS_REPORT|
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES );
	m_list.SetTextColor(RGB(64,64,64));

	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 4, 1);
	m_cImageListNormal32.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP2);
	m_cImageListNormal32.Add(&cBmp, RGB(255,255, 255));

	m_list.SetImageList(&m_cImageListNormal32, LVSIL_SMALL);
	m_list.SetTextColor(RGB(64,64,64));

	m_list.InsertColumn(0,"编号",LVCFMT_LEFT,40,-1);
	m_list.InsertColumn(1,"项目",LVCFMT_LEFT,1,-1);
	m_list.InsertColumn(2,"内容",LVCFMT_LEFT,1,-1);
	m_list.InsertColumn(3,"详细描述",LVCFMT_LEFT,1,-1); 


	CHeaderCtrl * m_head=m_list.GetHeaderCtrl();
    m_list.m_colorheader.SubclassWindow(m_head->GetSafeHwnd());

	m_tree.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);
	

	m_tree.SetImageList ( &m_cImageListNormal32,TVSIL_NORMAL );

	ems = m_tree.InsertItem("EMS",3,3,NULL);
	ats = m_tree.InsertItem("ATS",3,3,NULL);
	ippdu = m_tree.InsertItem("IP-PDU",3,3,NULL);
	insertList();
	/////
	int i;
	for(i=0;i<12;i++)
	{
		m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,i,"",0,LVIS_SELECTED,4,0);
	}

	Slave=0;
	SelectIP=0;
	/////
	SetTimer(EMS_ATS_TIMER,5000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEmsAtsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(IsWindow(m_tree.m_hWnd))
	{	
		//GetDlgItem(IDC_EDIT1)->MoveWindow(rect.left+10,rect.Height()-185,rect.Width()*12/100,22);
		m_tree.MoveWindow(rect.left+1,rect.top+30,rect.Width()*28/100-2,rect.Height()-220);
		m_list.MoveWindow(rect.Width()*28/100+8,rect.Height()*60/100+6,rect.Width()*72/100-10,rect.Height()*40/100-10);	
		m_list.SetColumnWidth(1,(rect.Width()*72/100-70)/4);
		m_list.SetColumnWidth(2,(rect.Width()*72/100-70)/4);
		m_list.SetColumnWidth(3,(rect.Width()*72/100-70)/2);
	}
}

void CEmsAtsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int i,j;
	CString ip;
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int flag=0;
	if(m_tree.GetParentItem(hItem)==ats)
	{
		flag=2;
	}
	else if(m_tree.GetParentItem(m_tree.GetParentItem(hItem))==ems)
	{
		flag=1;
	}
	else if(m_tree.GetParentItem(m_tree.GetParentItem(hItem))==ippdu)
	{
		flag=3;
	}
	else if(m_tree.GetParentItem(hItem)==ems)
	{
		currentIP1=m_tree.GetItemText(hItem);
	}
	else if(m_tree.GetParentItem(hItem)==ippdu)
	{
		currentIP1=m_tree.GetItemText(hItem);
		flag=3;
	}
	if(!flag)
	{
		for(i=0;i<12;i++)
		{
			for(j=0;j<4;j++)
			{
				m_list.SetItemText(i,j,"");
			}
		}
		return;
	}
	if(flag == 1)
	{
		ip=m_tree.GetItemText(m_tree.GetParentItem(hItem));
	}
	else
	{
		ip=m_tree.GetItemText(hItem);
	}
	currentIP1 = ip;
	currentIP2.Empty();
	POSITION pos = snmpobj.GetTailPosition ();
	while (pos != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
		if(p_Object->ipaddr.GetLength()  && ip ==p_Object->ipaddr)
		{
			p_select = p_Object;
			if(p_Object->dtype==1)
			{
				for(i=0;i<12;i++)
				{
					if(p_Object->EMSname[i] == m_tree.GetItemText(hItem))
					{
						index =i;
						updateView();
						return;
					}
				}
			}
			updateView();
			return;
		}
	}
	*pResult = 0;
}

void CEmsAtsDlg::updateList(void)
{
	int nor=0,off=0,ala=0;
	POSITION pos = snmpobj.GetTailPosition ();
	while (pos != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
		if(p_Object->ipaddr.GetLength()  && p_Object->dtype==1)
		{
			HTREEITEM hson = checkList(ems,p_Object->ipaddr,3);
			if(hson)
			{
				int i;
				for(i=0;i<12;i++)
				{
					if(p_Object->EMSname[i].GetLength())
					{
						checkList(hson,p_Object->EMSname[i],p_Object->EMSstatus[i]);
						if(p_Object->EMSstatus[i]==1 && p_Object->EMSname[i].GetLength())
						{
							nor++;
						}
						else if(p_Object->EMSstatus[i]==2 && p_Object->EMSname[i].GetLength())
						{
							ala++;
						}
						else if(p_Object->EMSstatus[i]==0 && p_Object->EMSname[i].GetLength())
						{
							off++;
						}
					}
				}
			}
		}
		if(p_Object->ipaddr.GetLength()  && p_Object->dtype==2)
		{
			checkList(ats,p_Object->ipaddr,p_Object->ATS_status);
			if(p_Object->ATS_status==0)
			{
				off++;
			}
			else if(p_Object->ATS_status==1)
			{
				nor++;
			}
			else if(p_Object->ATS_status==2)
			{
				ala++;
			}
		}
		else if(p_Object->ipaddr.GetLength()  && p_Object->dtype==3)
		{
			checkList(ippdu,p_Object->ipaddr,p_Object->ipStatus);
			if(p_Object->ipStatus==0)
			{
				off++;
			}
			else if(p_Object->ipStatus==1)
			{
				nor++;
			}
			else if(p_Object->ipStatus==2)
			{
				ala++;
			}
		}
	}
	offline = off;
	normal = nor;
	alarm = ala;
}

void CEmsAtsDlg::insertList(void)
{
	POSITION pos = snmpobj.GetTailPosition ();
	while (pos != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
		if(p_Object->ipaddr.GetLength()  && p_Object->dtype==1)
		{
			m_tree.InsertItem(p_Object->ipaddr,3,3,ems);
		}
		else if(p_Object->ipaddr.GetLength()  && p_Object->dtype==2)
		{
			m_tree.InsertItem(p_Object->ipaddr,3,3,ats);
		}
		else if(p_Object->ipaddr.GetLength()  && p_Object->dtype==3)
		{
			m_tree.InsertItem(p_Object->ipaddr,3,3,ippdu);
		}
	}
}

void CEmsAtsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(EMS_ATS_TIMER==nIDEvent)
	{
		updateList();
		CDC *pDC;
		pDC = GetDC();
		if(p_select)
		{
			if(p_select->dtype==1)
			{
				drawEMSdata(pDC);
			}
			else if(p_select->dtype==2)
			{
				drawATSdata(pDC);
			}
			else if(p_select->dtype==3)
			{
				drawIPdata(pDC);
			}
		}
		ReleaseDC(pDC);
	}

	CDialog::OnTimer(nIDEvent);
}

HTREEITEM CEmsAtsDlg::checkList(HTREEITEM hitem,CString name,int flags)
{
	HTREEITEM hSon = m_tree.GetNextItem(hitem,TVGN_CHILD);
	while(NULL != hSon)
	{
	   CString textname = m_tree.GetItemText(hSon);
	   if( textname==name && name.GetLength())
	   {
		   if(flags==0)
		   {
			   m_tree.SetItemImage(hSon,1,1);
		   }
		   else if(flags==1)
		   {
			   m_tree.SetItemImage(hSon,0,0);
		   }
		   else if(flags==2)
		   {
			   m_tree.SetItemImage(hSon,2,2);
		   }
		   return hSon;
	   }
	   hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
	}
	m_tree.InsertItem(name,0,0,hitem);
	return NULL;
}

void CEmsAtsDlg::insert_m_list(void)
{
	//m_list.SetItemText(0,0,"3");
}

int CEmsAtsDlg::drawEMS(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	///////
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*28/100+6,25,rect.Width(),rect.Height()*60/100);
	///////
	drawImagePNG(pDC,getFilePath()+"\\icon\\mpdu.png",rect.Width()*32/100,40,rect.Width()*25/100,rect.Height()*22/100);
	drawImagePNG(pDC,getFilePath()+"\\icon\\mpdu.png",rect.Width()*32/100,rect.Height()*25/100+40,rect.Width()*25/100,rect.Height()*22/100);
	drawImagePNG(pDC,getFilePath()+"\\icon\\mpdu.png",rect.Width()*60/100,40,rect.Width()*25/100,rect.Height()*22/100);
	drawImagePNG(pDC,getFilePath()+"\\icon\\mpdu.png",rect.Width()*60/100,rect.Height()*25/100+40,rect.Width()*25/100,rect.Height()*22/100);
	drawFont(pDC,1,RGB(64,64,128));
	pDC->TextOut(rect.Width()*42/100,rect.Height()*4/100+40,"Metered PDU 1");
	pDC->TextOut(rect.Width()*70/100,rect.Height()*4/100+40,"Metered PDU 2");
	pDC->TextOut(rect.Width()*42/100,rect.Height()*29/100+40,"Metered PDU 3");
	pDC->TextOut(rect.Width()*70/100,rect.Height()*29/100+40,"Metered PDU 4");

	drawEMSdata(pDC);

	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*35/100,rect.Height()*10/100+40,"电流:");
	pDC->TextOut(rect.Width()*47/100,rect.Height()*10/100+40,"电压:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*16/100+40,"电能:");

	pDC->TextOut(rect.Width()*63/100,rect.Height()*10/100+40,"电流:");
	pDC->TextOut(rect.Width()*75/100,rect.Height()*10/100+40,"电压:");
	pDC->TextOut(rect.Width()*63/100,rect.Height()*16/100+40,"电能:");

	pDC->TextOut(rect.Width()*35/100,rect.Height()*35/100+40,"电流:");
	pDC->TextOut(rect.Width()*47/100,rect.Height()*35/100+40,"电压:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*41/100+40,"电能:");

	pDC->TextOut(rect.Width()*63/100,rect.Height()*35/100+40,"电流:");
	pDC->TextOut(rect.Width()*75/100,rect.Height()*35/100+40,"电压:");
	pDC->TextOut(rect.Width()*63/100,rect.Height()*41/100+40,"电能:");
	return 0;
}
void CEmsAtsDlg::drawATS(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	///////////////////
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*28/100+6,25,rect.Width(),rect.Height()*60/100);
	///////////////////

	drawImagePNG(pDC,getFilePath()+"\\icon\\IN.png",rect.Width()*32/100,40,rect.Width()*25/100,rect.Height()*22/100);
	drawImagePNG(pDC,getFilePath()+"\\icon\\OUT.png",rect.Width()*32/100,rect.Height()*25/100+40,rect.Width()*25/100,rect.Height()*22/100);
	drawImagePNG(pDC,getFilePath()+"\\icon\\IN.png",rect.Width()*60/100,40,rect.Width()*25/100,rect.Height()*22/100);
	drawFont(pDC,1,RGB(64,64,128));
	//pDC->TextOut(rect.Width()*42/100,rect.Height()*4/100+40,"Source A");
	//pDC->TextOut(rect.Width()*70/100,rect.Height()*4/100+40,"Source B");
	pDC->TextOut(rect.Width()*42/100,rect.Height()*29/100+40,"输出");

	drawATSdata(pDC);

	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*35/100,rect.Height()*10/100+40,"电流:");
	pDC->TextOut(rect.Width()*45/100,rect.Height()*10/100+40,"电压:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*16/100+40,"正常工作范围:");

	pDC->TextOut(rect.Width()*63/100,rect.Height()*10/100+40,"电流:");
	pDC->TextOut(rect.Width()*75/100,rect.Height()*10/100+40,"电压:");
	pDC->TextOut(rect.Width()*63/100,rect.Height()*16/100+40,"正常工作范围:");

	pDC->TextOut(rect.Width()*35/100,rect.Height()*35/100+40,"电流:");
	pDC->TextOut(rect.Width()*45/100,rect.Height()*35/100+40,"电压:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*41/100+40,"输出:");
}


void CEmsAtsDlg::updateView(void)
{
	CRect rect;
	GetClientRect(&rect);
	if(p_select)
	{
		CDC *pDC;
		pDC =  GetDC();
		if(p_select->dtype==1)
		{
			drawEMS(pDC);
		}
		else if(p_select->dtype==2)
		{
			drawATS(pDC);
		}
		else if(p_select->dtype==3)
		{
			drawIPdata(pDC);
		}
		drawFont(pDC,0,RGB(64,64,64));
		CString value;
		value.Format("%d",normal);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-105,"     ");
		pDC->TextOut(rect.Width()*20/100,rect.Height()-105,value);
		value.Empty();
		value.Format("%d",offline);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-75,"     ");
		pDC->TextOut(rect.Width()*20/100,rect.Height()-75,value);
		value.Empty();
		value.Format("%d",alarm);
		pDC->TextOut(rect.Width()*20/100,rect.Height()-45,"     ");
		pDC->TextOut(rect.Width()*20/100,rect.Height()-45,value);
		ReleaseDC(pDC);
	}
	//////////////////////
	insert_m_list();
}

void CEmsAtsDlg::drawEMSdata(CDC *pDC)
{
	//m_list.DeleteAllItems();
	CRect rect;
	GetClientRect(&rect);

	drawFont(pDC,4,RGB(64,64,128));
	CString value;
	value.Format("%.1fA",p_select->EMS_cur_PDU[0][index]);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*10/100+38,value);
	value.Empty();
	value.Format("%dV",p_select->EMS_vol_PDU[0][index]);
	pDC->TextOut(rect.Width()*52/100,rect.Height()*10/100+38,"    ");
	pDC->TextOut(rect.Width()*52/100,rect.Height()*10/100+38,value);
	value.Empty();
	value.Format("%.1fKWh",p_select->EMS_energy_PDU[0][index]);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*16/100+38,"         ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*16/100+38,value);

	value.Empty();
	value.Format("%.1fA",p_select->EMS_cur_PDU[1][index]);
	pDC->TextOut(rect.Width()*68/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*10/100+38,value);
	value.Empty();
	value.Format("%dV",p_select->EMS_vol_PDU[1][index]);
	pDC->TextOut(rect.Width()*80/100,rect.Height()*10/100+38,"    ");
	pDC->TextOut(rect.Width()*80/100,rect.Height()*10/100+38,value);
	value.Empty();
	value.Format("%.1fKWh",p_select->EMS_energy_PDU[1][index]);
	pDC->TextOut(rect.Width()*68/100,rect.Height()*16/100+38,"         ");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*16/100+38,value);

	value.Empty();
	value.Format("%.1fA",p_select->EMS_cur_PDU[2][index]);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*35/100+38,"     ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*35/100+38,value);
	value.Empty();
	value.Format("%dV",p_select->EMS_vol_PDU[2][index]);
	pDC->TextOut(rect.Width()*52/100,rect.Height()*35/100+38,"    ");
	pDC->TextOut(rect.Width()*52/100,rect.Height()*35/100+38,value);
	value.Empty();
	value.Format("%.1fKWh",p_select->EMS_energy_PDU[2][index]);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*41/100+38,"         ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*41/100+38,value);

	value.Empty();
	value.Format("%.1fA",p_select->EMS_cur_PDU[3][index]);
	pDC->TextOut(rect.Width()*68/100,rect.Height()*35/100+38,"     ");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*35/100+38,value);
	value.Empty();
	value.Format("%dV",p_select->EMS_vol_PDU[3][index]);
	pDC->TextOut(rect.Width()*80/100,rect.Height()*35/100+38,"    ");
	pDC->TextOut(rect.Width()*80/100,rect.Height()*35/100+38,value);
	value.Empty();
	value.Format("%.1fKWh",p_select->EMS_energy_PDU[3][index]);
	pDC->TextOut(rect.Width()*68/100,rect.Height()*41/100+38,"         ");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*41/100+38,value);

	m_list.SetItemText(0,0,"1");
	m_list.SetItemText(0,1,"设备IP地址");
	m_list.SetItemText(0,2,p_select->ipaddr);
	if(p_select->EMSstatus[index]==0)
		m_list.SetItemText(0,3,"离线");
	else if(p_select->EMSstatus[index]==1)
		m_list.SetItemText(0,3,"正常");
	else if(p_select->EMSstatus[index]==2)
		m_list.SetItemText(0,3,"异常");
	m_list.SetItemText(1,0,"2");
	m_list.SetItemText(1,1,"设备名称");
	m_list.SetItemText(1,2,p_select->EMSname[index]);
	m_list.SetItemText(1,3,"副机设备名称");
	m_list.SetItemText(2,0,"3");
	m_list.SetItemText(2,1,"门禁 1");
	m_list.SetItemText(2,2,p_select->EMS_door_PDU[0][index]);
	m_list.SetItemText(2,3,"第一个门禁传感器");
	m_list.SetItemText(3,0,"4");
	m_list.SetItemText(3,1,"门禁 2");
	m_list.SetItemText(3,2,p_select->EMS_door_PDU[1][index]);
	m_list.SetItemText(3,3,"第二个门禁传感器");
	m_list.SetItemText(4,0,"5");
	m_list.SetItemText(4,1,"水浸");
	m_list.SetItemText(4,2,p_select->EMS_water_PDU[index]);
	m_list.SetItemText(4,3,"水浸传感器");
	m_list.SetItemText(5,0,"6");
	m_list.SetItemText(5,1,"烟雾");
	m_list.SetItemText(5,2,p_select->EMS_smoke_PDU[index]);
	m_list.SetItemText(5,3,"烟雾传感器");

	value.Empty();
	m_list.SetItemText(6,0,"7");
	m_list.SetItemText(6,1,"PDU1");
	m_list.SetItemText(6,2,"Metered PDU");
	value.Format("电流:%.1fA,电压:%dV,电能:%.1fKwh"
		,p_select->EMS_cur_PDU[0][index]
		,p_select->EMS_vol_PDU[0][index]
		,p_select->EMS_energy_PDU[0][index]);
	m_list.SetItemText(6,3,value);

	value.Empty();
	m_list.SetItemText(7,0,"8");
	m_list.SetItemText(7,1,"PDU2");
	m_list.SetItemText(7,2,"Metered PDU");
	value.Format("电流:%.1fA,电压:%dV,电能:%.1fKwh"
		,p_select->EMS_cur_PDU[1][index]
		,p_select->EMS_vol_PDU[1][index]
		,p_select->EMS_energy_PDU[1][index]);
	m_list.SetItemText(7,3,value);

	value.Empty();
	m_list.SetItemText(8,0,"9");
	m_list.SetItemText(8,1,"PDU3");
	m_list.SetItemText(8,2,"Metered PDU");
	value.Format("电流:%.1fA,电压:%dV,电能:%.1fKwh"
		,p_select->EMS_cur_PDU[2][index]
		,p_select->EMS_vol_PDU[2][index]
		,p_select->EMS_energy_PDU[2][index]);
	m_list.SetItemText(8,3,value);

	value.Empty();
	m_list.SetItemText(9,0,"10");
	m_list.SetItemText(9,1,"PDU4");
	m_list.SetItemText(9,2,"Metered PDU");
	value.Format("电流:%.1fA,电压:%dV,电能:%.1fKwh"
		,p_select->EMS_cur_PDU[3][index]
		,p_select->EMS_vol_PDU[3][index]
		,p_select->EMS_energy_PDU[3][index]);
	m_list.SetItemText(9,3,value);

	value.Empty();
	m_list.SetItemText(10,0,"11");
	m_list.SetItemText(10,1,"温度/湿度");
	m_list.SetItemText(10,2,"The first sensor");
	value.Format("温度:%d ℃,湿度:%d"
		,p_select->EMS_th_PDU[0][index],p_select->EMS_th_PDU[1][index]);
	value +="%";
	m_list.SetItemText(10,3,value);

	value.Empty();
	
	m_list.SetItemText(11,0,"12");
	m_list.SetItemText(11,1,"温度/湿度");
	m_list.SetItemText(11,2,"The second sensor");
	value.Format("温度:%d ℃,湿度:%d"
		,p_select->EMS_th_PDU[2][index],p_select->EMS_th_PDU[3][index]);
	value +="%";
	m_list.SetItemText(11,3,value);
}

void CEmsAtsDlg::drawATSdata(CDC *pDC)
{
	//m_list.DeleteAllItems();
	CRect rect;
	GetClientRect(&rect);

	drawFont(pDC,1,RGB(64,64,128));
	pDC->TextOut(rect.Width()*42/100,rect.Height()*4/100+40,p_select->ATS_lin1name);
	pDC->TextOut(rect.Width()*70/100,rect.Height()*4/100+40,p_select->ATS_lin2name);


	drawFont(pDC,4,RGB(64,64,128));
	CString value;
	value.Format("%.1fA",p_select->ATS_CA);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*10/100+38,value);

	value.Empty();
	value.Format("%dV",p_select->ATS_VA);
	pDC->TextOut(rect.Width()*50/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*50/100,rect.Height()*10/100+38,value);

	value.Empty();
	value.Format("%dV～%dV",p_select->ATS_VL,p_select->ATS_VH);
	pDC->TextOut(rect.Width()*40/100+35,rect.Height()*16/100+38,"          ");
	pDC->TextOut(rect.Width()*40/100+35,rect.Height()*16/100+38,value);

	value.Empty();
	value.Format("%.1fA",p_select->ATS_CB);
	pDC->TextOut(rect.Width()*68/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*10/100+38,value);

	value.Empty();
	value.Format("%dV",p_select->ATS_VB);
	pDC->TextOut(rect.Width()*80/100,rect.Height()*10/100+38,"     ");
	pDC->TextOut(rect.Width()*80/100,rect.Height()*10/100+38,value);

	value.Empty();
	value.Format("%dV～%dV",p_select->ATS_VL,p_select->ATS_VH);
	pDC->TextOut(rect.Width()*68/100+35,rect.Height()*16/100+38,"          ");
	pDC->TextOut(rect.Width()*68/100+35,rect.Height()*16/100+38,value);

	if(p_select->ATS_CO<p_select->ATS_CL || p_select->ATS_CO > p_select->ATS_CH)
	{
		drawFont(pDC,4,RGB(192,64,64));
		//p_select->ATS_status = 2;
	}
	else
	{
		drawFont(pDC,4,RGB(64,192,64));
	}
	value.Empty();
	value.Format("%.1fA",p_select->ATS_CO);
	pDC->TextOut(rect.Width()*40/100,rect.Height()*35/100+38,"     ");
	pDC->TextOut(rect.Width()*40/100,rect.Height()*35/100+38,value);
	
	drawFont(pDC,4,RGB(64,64,128));

	value.Empty();
	value.Format("%dV",p_select->ATS_VO);
	pDC->TextOut(rect.Width()*50/100,rect.Height()*35/100+38,"     ");
	pDC->TextOut(rect.Width()*50/100,rect.Height()*35/100+38,value);

	//value.Empty();
	//value.Format("%dV～%dV",p_select->ATS_VL,p_select->ATS_VH);
	pDC->TextOut(rect.Width()*40/100+35,rect.Height()*41/100+38,"          ");
	pDC->TextOut(rect.Width()*40/100+35,rect.Height()*41/100+38,p_select->ATS_currentinput);

	//pDC->TextOut(rect.Width()*40/100,rect.Height()*10/100+38,"3A");
	//pDC->TextOut(rect.Width()*50/100,rect.Height()*10/100+38,"220V");
	//pDC->TextOut(rect.Width()*40/100+35,rect.Height()*16/100+38,"180V～220V");

	m_list.SetItemText(0,0,"1");
	m_list.SetItemText(0,1,"设备IP地址");
	m_list.SetItemText(0,2,p_select->ipaddr);
	if(p_select->ATS_status==0)
		m_list.SetItemText(0,3,"离线");
	else if(p_select->ATS_status==1)
		m_list.SetItemText(0,3,"正常");
	else if(p_select->ATS_status==2)
		m_list.SetItemText(0,3,"异常");

	m_list.SetItemText(1,0,"2");
	m_list.SetItemText(1,1,"设备名称");
	m_list.SetItemText(1,2,p_select->ATS_name);

	m_list.SetItemText(2,0,"3");
	m_list.SetItemText(2,1,"Source A名称");
	m_list.SetItemText(2,2,p_select->ATS_lin1name);
	m_list.SetItemText(2,3,"");

	m_list.SetItemText(3,0,"4");
	m_list.SetItemText(3,1,"Source B名称");
	m_list.SetItemText(3,2,p_select->ATS_lin2name);
	m_list.SetItemText(3,3,"");

	m_list.SetItemText(4,0,"5");
	m_list.SetItemText(4,1,"当前输入");
	m_list.SetItemText(4,2,p_select->ATS_currentinput);
	m_list.SetItemText(4,3,"");

	m_list.SetItemText(5,0,"6");
	m_list.SetItemText(5,1,"主输入");
	m_list.SetItemText(5,2,p_select->ATS_maininput);
	m_list.SetItemText(5,3,"");

	value.Empty();
	m_list.SetItemText(6,0,"7");
	m_list.SetItemText(6,1,"Source A状态");
	m_list.SetItemText(6,2,p_select->ATS_lin1name);
	value.Format("电流:%.1fA,电压:%dV",p_select->ATS_CA,p_select->ATS_VA);
	m_list.SetItemText(6,3,value);

	value.Empty();
	m_list.SetItemText(7,0,"8");
	m_list.SetItemText(7,1,"Source B状态");
	m_list.SetItemText(7,2,p_select->ATS_lin2name);
	value.Format("电流:%.1fA,电压:%dV",p_select->ATS_CB,p_select->ATS_VB);
	m_list.SetItemText(7,3,value);

	m_list.SetItemText(8,0,"");
	m_list.SetItemText(8,1,"");
	m_list.SetItemText(8,2,"");
	m_list.SetItemText(8,3,"");

	m_list.SetItemText(9,0,"");
	m_list.SetItemText(9,1,"");
	m_list.SetItemText(9,2,"");
	m_list.SetItemText(9,3,"");
	
	m_list.SetItemText(10,0,"");
	m_list.SetItemText(10,1,"");
	m_list.SetItemText(10,2,"");
	m_list.SetItemText(10,3,"");

	m_list.SetItemText(11,0,"");
	m_list.SetItemText(11,1,"");
	m_list.SetItemText(11,2,"");
	m_list.SetItemText(11,3,"");
	
}

void CEmsAtsDlg::drawIPdata(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	///////////////////
	CPen linepen;
	linepen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	CPen * Oldlinepen;
	Oldlinepen=NULL;
	Oldlinepen=pDC->SelectObject(&linepen);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*28/100+7,26,rect.Width()-1,rect.Height()*60/100-1);
	pDC->SelectObject(Oldlinepen);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOutA(rect.Width()*30/100,55,"Device information");
	drawFont(pDC,0,RGB(0,0,0));
	CString value;
	CPen pen(PS_SOLID,3,RGB(85,141,196));
	CPen *oldpen;
	oldpen=pDC->SelectObject(&pen);
	pDC->Rectangle(rect.Width()*47/100,40,rect.Width()*82/100,rect.Height()*57/100);
	pDC->Rectangle(rect.Width()*47/100,rect.Height()*52/100,rect.Width()*47/100+(rect.Width()*35/100)/3,rect.Height()*57/100);
	pDC->Rectangle(rect.Width()*47/100+(rect.Width()*35/100)/3,rect.Height()*52/100,rect.Width()*47/100+2*(rect.Width()*35/100)/3,rect.Height()*57/100);
	pDC->Rectangle(rect.Width()*47/100+2*(rect.Width()*35/100)/3,rect.Height()*52/100,rect.Width()*82/100,rect.Height()*57/100);
	pDC->Rectangle(rect.Width()*85/100,80,rect.Width()*95/100,rect.Height()*40/100);
	pDC->Rectangle(rect.Width()*85/100,80,rect.Width()*95/100,80+(rect.Height()*40/100-80)/5);
	pDC->Rectangle(rect.Width()*85/100,80+(rect.Height()*40/100-80)/5,rect.Width()*95/100,80+2*(rect.Height()*40/100-80)/5);
	pDC->Rectangle(rect.Width()*85/100,80+2*(rect.Height()*40/100-80)/5,rect.Width()*95/100,80+3*(rect.Height()*40/100-80)/5);
	pDC->Rectangle(rect.Width()*85/100,80+3*(rect.Height()*40/100-80)/5,rect.Width()*95/100,80+4*(rect.Height()*40/100-80)/5);
	pDC->Rectangle(rect.Width()*85/100,80+4*(rect.Height()*40/100-80)/5,rect.Width()*95/100,rect.Height()*40/100);
	pDC->SelectObject(oldpen);
	drawFont(pDC,2,RGB(0,0,0));
	CSize size;
	size=pDC->GetTextExtent("Line1");
	pDC->TextOutA(rect.Width()*47/100+((rect.Width()*35/100)/3-size.cx)/2,rect.Height()*52/100+((rect.Height()*5/100)-size.cy)/2,"Line1");
	size=pDC->GetTextExtent("Line2");
	pDC->TextOutA(rect.Width()*47/100+(rect.Width()*35/100)/3+((rect.Width()*35/100)/3-size.cx)/2,rect.Height()*52/100+((rect.Height()*5/100)-size.cy)/2,"Line2");
	size=pDC->GetTextExtent("Line3");
	pDC->TextOutA(rect.Width()*47/100+2*(rect.Width()*35/100)/3+((rect.Width()*35/100)/3-size.cx)/2,rect.Height()*52/100+((rect.Height()*5/100)-size.cy)/2,"Line3");
	size=pDC->GetTextExtent("Master");
	pDC->TextOutA(rect.Width()*85/100+(rect.Width()*10/100-size.cx)/2,80+((rect.Height()*40/100-80)/5-size.cy)/2,"Master");
	size=pDC->GetTextExtent("Slave1");
	pDC->TextOutA(rect.Width()*85/100+(rect.Width()*10/100-size.cx)/2,80+(rect.Height()*40/100-80)/5+((rect.Height()*40/100-80)/5-size.cy)/2,"Slave1");
	size=pDC->GetTextExtent("Slave2");
	pDC->TextOutA(rect.Width()*85/100+(rect.Width()*10/100-size.cx)/2,80+2*(rect.Height()*40/100-80)/5+((rect.Height()*40/100-80)/5-size.cy)/2,"Slave2");
	size=pDC->GetTextExtent("Slave3");
	pDC->TextOutA(rect.Width()*85/100+(rect.Width()*10/100-size.cx)/2,80+3*(rect.Height()*40/100-80)/5+((rect.Height()*40/100-80)/5-size.cy)/2,"Slave3");
	size=pDC->GetTextExtent("Slave4");
	pDC->TextOutA(rect.Width()*85/100+(rect.Width()*10/100-size.cx)/2,80+4*(rect.Height()*40/100-80)/5+((rect.Height()*40/100-80)/5-size.cy)/2,"Slave4");
	drawFont(pDC,0,RGB(0,0,0));
	if(p_select->ipStatus==1||p_select->ipStatus==2)
	{
		CFont font;
		font.CreateFont(
			rect.Height()/10,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			0,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"宋体");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,0));

		if(SelectIP==0)
			pDC->TextOutA(rect.Width()*50/100,40+rect.Height()*5/100,"L1");
		else if(SelectIP==1)
			pDC->TextOutA(rect.Width()*50/100,40+rect.Height()*5/100,"L2");
		else if(SelectIP==2)
			pDC->TextOutA(rect.Width()*50/100,40+rect.Height()*5/100,"L3");
		value.Format("%.1f",p_select->ipTcur[SelectIP]);
		if(p_select->ipStatus==2)
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOutA(rect.Width()*70/100,40+rect.Height()*5/100,value);
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOutA(rect.Width()*70/100,40+rect.Height()*5/100,value);
		}
		pDC->SetTextColor(RGB(0,0,0));
		value.Format("%d",p_select->ipTvol[SelectIP]);
		pDC->TextOutA(rect.Width()*50/100,40+rect.Height()*20/100,value);
		value.Format("%.1f",(p_select->ipTvol[SelectIP]*p_select->ipTcur[SelectIP])/1000);
		pDC->TextOutA(rect.Width()*70/100,40+rect.Height()*20/100,value);
		value.Format("%.1f",(p_select->ipTenergy[SelectIP])/10);
		CSize size=pDC->GetTextExtent(value);
		pDC->TextOutA(rect.Width()*78/100-size.cx,40+rect.Height()*35/100,value);

		CFont font1;
		font1.CreateFont(
			rect.Height()/20,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			0,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"宋体");                 // lpszFacename
		pDC->SelectObject(&font1);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOutA(rect.Width()*79/100,40+rect.Height()*9/100,"A");
		pDC->TextOutA(rect.Width()*57/100,40+rect.Height()*24/100,"V");
		pDC->TextOutA(rect.Width()*79/100,40+rect.Height()*24/100,"kW");
		pDC->TextOutA(rect.Width()*78/100,40+rect.Height()*39/100,"kWh");

		drawFont(pDC,0,RGB(0,0,0));


		value.Format("%.1f",p_select->ipTcur[SelectIP]);
		m_list.SetItemText(0,0,"1");
		m_list.SetItemText(0,1,"Total Current");
		m_list.SetItemText(0,2,value);
		m_list.SetItemText(0,3,"A");

		value.Format("%d",p_select->ipTvol[SelectIP]);
		m_list.SetItemText(1,0,"2");
		m_list.SetItemText(1,1,"Total Voltage");
		m_list.SetItemText(1,2,value);
		m_list.SetItemText(1,3,"V");

		value.Format("%.1f",p_select->ipTcur[SelectIP]*p_select->ipTvol[SelectIP]);
		m_list.SetItemText(2,0,"3");
		m_list.SetItemText(2,1,"Power");
		m_list.SetItemText(2,2,value);
		m_list.SetItemText(2,3,"kW");

		value.Format("%.1f",p_select->ipTenergy[SelectIP]/10);
		m_list.SetItemText(3,0,"4");
		m_list.SetItemText(3,1,"Energy");
		m_list.SetItemText(3,2,value);
		m_list.SetItemText(3,3,"kWh");

		m_list.SetItemText(4,0,"5");
		m_list.SetItemText(4,1,"DeviceName");
		m_list.SetItemText(4,2,p_select->ipname);
		m_list.SetItemText(4,3,"");

		m_list.SetItemText(5,0,"6");
		m_list.SetItemText(5,1,"DeviceIP");
		m_list.SetItemText(5,2,p_select->ipaddr);
		m_list.SetItemText(5,3,"");

		m_list.SetItemText(6,0,"");
		m_list.SetItemText(6,1,"");
		m_list.SetItemText(6,2,"");
		m_list.SetItemText(6,3,"");

		m_list.SetItemText(7,0,"");
		m_list.SetItemText(7,1,"");
		m_list.SetItemText(7,2,"");
		m_list.SetItemText(7,3,"");

		m_list.SetItemText(8,0,"");
		m_list.SetItemText(8,1,"");
		m_list.SetItemText(8,2,"");
		m_list.SetItemText(8,3,"");

		m_list.SetItemText(9,0,"");
		m_list.SetItemText(9,1,"");
		m_list.SetItemText(9,2,"");
		m_list.SetItemText(9,3,"");

		m_list.SetItemText(10,0,"");
		m_list.SetItemText(10,1,"");
		m_list.SetItemText(10,2,"");
		m_list.SetItemText(10,3,"");

		m_list.SetItemText(11,0,"");
		m_list.SetItemText(11,1,"");
		m_list.SetItemText(11,2,"");
		m_list.SetItemText(11,3,"");
	}
	
}

void CEmsAtsDlg::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu Popmenu;
	CMenu* pMenu;
	CPoint pt;
	::GetCursorPos(&pt);
	//ATS
	Popmenu.LoadMenu(IDR_ATS_ADD);
	pMenu = Popmenu.GetSubMenu(0);
	//EMS
	CMenu Popmenu1;
	CMenu* pMenu1;
	Popmenu1.LoadMenu(IDR_EMS_ADD);
	pMenu1= Popmenu1.GetSubMenu(0);
	//ATS IP
	CMenu Popmenu2;
	CMenu* pMenu2;
	Popmenu2.LoadMenu(IDR_ATSEMS);
	pMenu2= Popmenu2.GetSubMenu(0);
	//EMS1
	CMenu Popmenu3;
	CMenu* pMenu3;
	Popmenu3.LoadMenu(IDR_ALARM_EMS);
	pMenu3= Popmenu3.GetSubMenu(0);
	//
	CMenu Popmenu4;
	CMenu* pMenu4;
	Popmenu4.LoadMenu(IDR_EMS);
	pMenu4= Popmenu4.GetSubMenu(0);

	CMenu Popmenu5;
	CMenu* pMenu5;
	Popmenu5.LoadMenu(IDR_IPPDU);
	pMenu5= Popmenu5.GetSubMenu(0);

	CMenu Popmenu6;
	CMenu* pMenu6;
	Popmenu6.LoadMenu(IDR_IP_MENU);
	pMenu6= Popmenu6.GetSubMenu(0);

	CString str1,str2,str3;
	str1="ATS";
	str2="EMS";
	str3="IP-PDU";
	HTREEITEM hSon,hParent,hParent_e;
	hSon=m_tree.GetSelectedItem();
	hParent=m_tree.GetParentItem(hSon);
	hParent_e=m_tree.GetParentItem(hParent);
	if(m_tree.GetItemText(hSon)=="ATS")
	{
		//ATS菜单
		pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
		*pResult = 0;
		return;
	}
	else if(m_tree.GetItemText(hSon)=="EMS")
	{
		//EMS菜单
		pMenu1->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
		*pResult = 0;
		return;
	}
	else if(m_tree.GetItemText(hSon)=="IP-PDU")
	{
		pMenu5->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
		*pResult = 0;
		return;
	}
	else 
	{
		if(m_tree.GetItemText(hParent)=="ATS")
		{
			//ATS的设备IP菜单
			pMenu2->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
		if(m_tree.GetItemText(hParent)=="IP-PDU")
		{
			//IP的设备IP菜单
			pMenu6->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
		else if(m_tree.GetItemText(hParent)=="EMS")
		{
			//EMS的设备IP菜单
			pMenu4->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
		else if(m_tree.GetItemText(hParent_e)=="EMS")
		{
			//EMS1的菜单
			pMenu3->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
			*pResult = 0;
			return;
		}
	}
	*pResult = 0;
}

void CEmsAtsDlg::OnEmsatsReleasealarm()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int flag=0;
	if(m_tree.GetParentItem(hItem)==ats)
	{
		flag=2;
	}
	else if(m_tree.GetParentItem(m_tree.GetParentItem(hItem))==ems)
	{
		flag=1;
	}
	if(!flag)
	{
		return;
	}
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(18);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsatsAddats()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(20);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsatsDelete()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(21);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsatsSet()//EmsAts设备连接
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	HTREEITEM tree;
	tree=m_tree.GetSelectedItem();
	if(m_tree.GetParentItem(tree))
	{
		str=m_tree.GetItemText(tree);
		ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+str,NULL,NULL, SW_NORMAL);
		
	}
}

void CEmsAtsDlg::OnEmsatsAddemsT()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(19);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsatsDeletedevT()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(21);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsatsDelalarmT()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int flag=0;
	if(m_tree.GetParentItem(hItem)==ats)
	{
		flag=2;
	}
	else if(m_tree.GetParentItem(m_tree.GetParentItem(hItem))==ems)
	{
		flag=1;
	}
	if(!flag)
	{
		return;
	}
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(18);
	sure.DoModal();
}



void CEmsAtsDlg::OnAlarmEms()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int flag=0;
	if(m_tree.GetParentItem(hItem)==ats)
	{
		flag=2;
	}
	else if(m_tree.GetParentItem(m_tree.GetParentItem(hItem))==ems)
	{
		flag=1;
	}
	if(!flag)
	{
		return;
	}
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(18);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsAdd()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(19);
	sure.DoModal();
}

void CEmsAtsDlg::OnAtsAdd()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(20);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsDelete()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(21);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsSet()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	HTREEITEM tree;
	tree=m_tree.GetSelectedItem();
	if(m_tree.GetParentItem(tree))
	{
		str=m_tree.GetItemText(tree);
		ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+str,NULL,NULL, SW_NORMAL);
	}
}

void CEmsAtsDlg::OnEmsatsEdit()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM  tree_se;
	tree_se=m_tree.GetSelectedItem();
	CString pre_ip=m_tree.GetItemText(tree_se);
	CMakeSureDlg sure;
	sure.pre_ip=pre_ip;
	sure.p_emsats = this;
	sure.setOrder(32);
	sure.DoModal();
}

void CEmsAtsDlg::OnEmsChange()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM  tree_se;
	tree_se=m_tree.GetSelectedItem();
	CString pre_ip=m_tree.GetItemText(tree_se);
	CMakeSureDlg sure;
	sure.pre_ip=pre_ip;
	sure.p_emsats = this;
	sure.setOrder(33);
	sure.DoModal();
}

void CEmsAtsDlg::OnIpPdu()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(37);
	sure.DoModal();
}



void CEmsAtsDlg::OnIpDel()
{
	// TODO: 在此添加命令处理程序代码
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(41);
	sure.DoModal();
}

void CEmsAtsDlg::OnIpSet()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	HTREEITEM tree;
	tree=m_tree.GetSelectedItem();
	if(m_tree.GetParentItem(tree))
	{
		str=m_tree.GetItemText(tree);
		ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+str,NULL,NULL, SW_NORMAL);
	}
}

void CEmsAtsDlg::OnIpEdit()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM  tree_se;
	tree_se=m_tree.GetSelectedItem();
	CString pre_ip=m_tree.GetItemText(tree_se);
	CMakeSureDlg sure;
	sure.pre_ip=pre_ip;
	sure.p_emsats = this;
	sure.setOrder(40);
	sure.DoModal();
}

void CEmsAtsDlg::OnIpAlarm()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int flag=0;
	if(m_tree.GetParentItem(hItem)==ippdu)
	{
		flag=1;
	}
	if(!flag)
	{
		return;
	}
	CMakeSureDlg sure;
	sure.p_emsats = this;
	sure.setOrder(18);
	sure.DoModal();
}

void CEmsAtsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	CRect UpdataRect;
	UpdataRect.left=rect.Width()*28/100+7;
	UpdataRect.right=rect.Width()-1;
	UpdataRect.top=26;
	UpdataRect.bottom=rect.Height()*60/100-1;
	if(point.x>rect.Width()*47/100&&point.x<rect.Width()*47/100+(rect.Width()*35/100)/3&&point.y>rect.Height()*52/100&&point.y<rect.Height()*57/100)
	{
		SelectIP=0;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*47/100+(rect.Width()*35/100)/3&&point.x<rect.Width()*47/100+2*(rect.Width()*35/100)/3&&point.y>rect.Height()*52/100&&point.y<rect.Height()*57/100)
	{
		SelectIP=1;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*47/100+2*(rect.Width()*35/100)/3&&point.x<rect.Width()*82/100&&point.y>rect.Height()*52/100&&point.y<rect.Height()*57/100)
	{
		SelectIP=2;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*85/100&&point.x<rect.Width()*95/100&&point.y>80&&point.y<80+(rect.Height()*40/100-80)/5)
	{
		Slave=0;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*85/100&&point.x<rect.Width()*95/100&&point.y>80+(rect.Height()*40/100-80)/5&&point.y<80+2*(rect.Height()*40/100-80)/5)
	{
		Slave=1;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*85/100&&point.x<rect.Width()*95/100&&point.y>80+2*(rect.Height()*40/100-80)/5&&point.y<80+3*(rect.Height()*40/100-80)/5)
	{
		Slave=2;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*85/100&&point.x<rect.Width()*95/100&&point.y>80+3*(rect.Height()*40/100-80)/5&&point.y<80+4*(rect.Height()*40/100-80)/5)
	{
		Slave=3;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	else if(point.x>rect.Width()*85/100&&point.x<rect.Width()*95/100&&point.y>80+4*(rect.Height()*40/100-80)/5&&point.y<rect.Height()*40/100)
	{
		Slave=4;
		this->InvalidateRect(UpdataRect);
		//drawIPdata(pDC);
	}
	CDialog::OnLButtonDown(nFlags, point);
}
