// CanbinNPMDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "clevermanager.h"
#include "CanbinNPMDlg.h"
#include "atlimage.h"
#include "SnmpObj.h"
#include "MakeSureDlg.h"
#include "DlgDataCenterMap.h"
#include "FileLogEdit.h"
// CCanbinNPMDlg 对话框

IMPLEMENT_DYNAMIC(CCanbinNPMDlg, CDialog)

CCanbinNPMDlg::CCanbinNPMDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCanbinNPMDlg::IDD, pParent)
, checkoutput(-1)
{
	mapEdit=false;
}

CCanbinNPMDlg::~CCanbinNPMDlg()
{
}

void CCanbinNPMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CCanbinNPMDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ADD_CABIN_DEVICE, &CCanbinNPMDlg::OnAddCabinDevice)
	ON_COMMAND(ID_DEL_CABIN_DEVICE, &CCanbinNPMDlg::OnDelCabinDevice)
	ON_WM_TIMER()
	//ON_EN_KILLFOCUS(IDC_EDIT1, &CCanbinNPMDlg::OnEnKillfocusEdit1)
	//ON_EN_SETFOCUS(IDC_EDIT1, &CCanbinNPMDlg::OnEnSetfocusEdit1)
	//ON_EN_KILLFOCUS(IDC_EDIT2, &CCanbinNPMDlg::OnEnKillfocusEdit2)
	//ON_EN_SETFOCUS(IDC_EDIT2, &CCanbinNPMDlg::OnEnSetfocusEdit2)
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_ADD_SLAVE_DEVICE, &CCanbinNPMDlg::OnAddSlaveDevice)
	ON_COMMAND(ID_DEL_SLAVE_DEVICE, &CCanbinNPMDlg::OnDelSlaveDevice)
	ON_COMMAND(ID_32876, &CCanbinNPMDlg::On32876)
	ON_COMMAND(ID_32877, &CCanbinNPMDlg::On32877)
	ON_COMMAND(ID_32878, &CCanbinNPMDlg::On32878)
	ON_COMMAND(ID_32879, &CCanbinNPMDlg::On32879)
END_MESSAGE_MAP()


// CCanbinNPMDlg 消息处理程序

BOOL CCanbinNPMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	master=NULL;
	salve =NULL;
	SetTimer(CABIN_TIMER,5000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

static CString DCname;

BOOL CCanbinNPMDlg::OnEraseBkgnd(CDC* pDC)
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
	CBrush brush(RGB(233,240,249));
	BkDc.SelectObject(&brush);
	BkDc.Rectangle(&rect);
	//BkDc.MoveTo(0,25);
	//BkDc.LineTo(rect.Width(),25);
	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,23);
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_icon->ipMaster.GetLength())
		{
			if(p_Objectnow->ipaddr == p_icon->ipMaster && p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0)
			{
				master = p_Objectnow;
				currentIP1 = master->ipaddr;
			}
		}
		if(p_icon->ipSlave.GetLength())
		{
			if(p_Objectnow->ipaddr == p_icon->ipSlave && p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0)
			{
				salve = p_Objectnow;
				currentIP2 = salve->ipaddr;
			}
		}
	}
	masterindex=p_icon->MasterNUM;
	slaveindex=p_icon->SlaveNUM;

	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",580,6,10,10);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\jijia.jpg",190,45,210,620);
	drawFont(&BkDc,0,RGB(0,0,0));
	///////////////////////////////////////////////////////
	if(master)
	{
		CString dtype= master->devicetype[masterindex];
		dtype = dtype.Right(1);
		for(i=0;i<24;i++)
		{
			if(master->outputname[masterindex][i].GetLength() && i<master->deviceOutputNum[0])
			{
				drawFont(&BkDc,0,RGB(0,0,0));
				CString num,kw;

				if(dtype=="C" || dtype=="A")
				{
					num.Format("%d: %s",i+1,"--");
					kw.Format("%s","--");
				}
				else
				{
					num.Format("%d: %.1fA",i+1,master->curout[masterindex][i]);
					kw.Format("%.1fW",master->curout[masterindex][i]*master->Tvol[masterindex][0]);
				}
				BkDc.TextOutA(10,65+24*i,num);
				BkDc.TextOutA(10,75+24*i,kw);
				if(master->status == 0 || master->slaveOffline[masterindex] == 0)
				{
					drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-y.png",60,65+24*i,30,24);
				}
				else if(master->curmax[masterindex][i]>0)
				{
					if(master->curmin[masterindex][i]>master->curout[masterindex][i] ||
						master->curmax[masterindex][i]<master->curout[masterindex][i]
					)
					{
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-r.png",60,65+24*i,30,24);
					}
					else if(master->outputstatus[masterindex][i]=="OFF")
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-n.png",60,65+24*i,30,24);
					else
					{
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-g.png",60,65+24*i,30,24);
					}
				}
				else
				{
					if(master->outputstatus[masterindex][i]=="OFF")
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-n.png",60,65+24*i,30,24);
					else
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-g.png",60,65+24*i,30,24);
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////
	if(salve)
	{
		CString dtype= salve->devicetype[slaveindex];
		dtype = dtype.Right(1);
		for(i=0;i<24 && i<salve->deviceOutputNum[slaveindex];i++)
		{
			if(salve->outputname[slaveindex][i].GetLength())
			{
				drawFont(&BkDc,0,RGB(0,0,0));
				CString num,kw;
				
				if(dtype=="C" || dtype=="A")
				{
					num.Format("%d: %s",i+1,"--");
					kw.Format("%s","--");
				}
				else
				{
					num.Format("%d: %.1fA",i+1,salve->curout[slaveindex][i]);
					kw.Format("%.1fW",salve->curout[slaveindex][i]*salve->Tvol[slaveindex][0]);
				}
				BkDc.TextOutA(550,65+24*i,num);
				BkDc.TextOutA(550,75+24*i,kw);
				if(salve->status == 0 || salve->slaveOffline[slaveindex] == 0)
				{
					drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-y.png",510,65+24*i,30,24);
				}
				else if(salve->curmax[slaveindex][i]>0)
				{
					if(salve->curmin[slaveindex][i]>salve->curout[slaveindex][i] ||
						salve->curmax[slaveindex][i]<salve->curout[slaveindex][i]
					)
					{
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-r.png",510,65+24*i,30,24);
					}
					else if(salve->outputstatus[slaveindex][i]=="OFF")
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-n.png",510,65+24*i,30,24);
					else
					{
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-g.png",510,65+24*i,30,24);
					}
				}
				else
				{
					if(salve->outputstatus[slaveindex][i]=="OFF")
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-n.png",510,65+24*i,30,24);
					else
						drawImagePNG(&BkDc,getFilePath()+"\\icon\\C13-g.png",510,65+24*i,30,24);
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////
	drawFont(&BkDc,1,RGB(64,64,64));
	CString name;
	name.Format("机柜位置: %d:%d   %s",p_icon->x+1,p_icon->y+1,p_icon->cabientname);
	BkDc.TextOut(20,5,name);
	//drawFont(&BkDc,0,RGB(0,0,0));

	if(master)
	{
		if(master->slaveOffline[masterindex] == 2)
		{
			drawFont(&BkDc,0,RGB(192,32,32));
		}
		else if(master->slaveOffline[masterindex] == 1)
		{
			drawFont(&BkDc,0,RGB(32,192,32));
		}
		else
		{
			drawFont(&BkDc,0,RGB(192,192,32));
		}
		name.Empty();
		name.Format("主要PDU:%s ->%d:",master->ipaddr,masterindex);
		BkDc.TextOut(20,32,name);
		drawFont(&BkDc,0,RGB(64,64,128));//显示设备IP地址，连接masterIP
		name.Format("设备设置:%s->%d",master->ipaddr,masterindex);
		BkDc.TextOut(10,640,name);
	}
	if(salve)
	{
		if(salve->slaveOffline[slaveindex] == 2)
		{
			drawFont(&BkDc,0,RGB(192,32,32));
		}
		else if(salve->slaveOffline[slaveindex] == 1)
		{
			drawFont(&BkDc,0,RGB(32,192,32));
		}
		else
		{
			drawFont(&BkDc,0,RGB(192,192,32));
		}
		name.Empty();
		name.Format("备用PDU:%s ->%d:",salve->ipaddr,slaveindex);
		BkDc.TextOut(420,32,name);
		drawFont(&BkDc,0,RGB(64,64,128));//显示设备IP地址，连接slaveIP
		name.Format("设备设置:%s->%d",salve->ipaddr,slaveindex);
		BkDc.TextOut(420,640,name);
	}
	//////////////////////////////////
	drawFont(&BkDc,0,RGB(0,0,0));
	if(master)
	{
		if(master->Tvol[masterindex][2]>50)
		{
			name.Empty();
			name.Format("L3电压:%dV",master->Tvol[masterindex][2]);
			BkDc.TextOut(180,720,name);
			name.Empty();
			name.Format("L3电流:%.1fA",master->Tcur[masterindex][2]);
			BkDc.TextOut(180,740,name);
		}
		if(master->Tvol[masterindex][1]>50)
		{
			name.Empty();
			name.Format("L2电压:%dV",master->Tvol[masterindex][1]);
			BkDc.TextOut(180,680,name);
			name.Empty();
			name.Format("L2电流:%.1fA",master->Tcur[masterindex][1]);
			BkDc.TextOut(180,700,name);
		}
		
		name.Empty();
		name.Format("L1电压:%dV",master->Tvol[masterindex][0]);
		BkDc.TextOut(10,660,name);
		name.Empty();
		name.Format("L1电流:%.1fA",master->Tcur[masterindex][0]);
		BkDc.TextOut(10,680,name);
		name.Empty();
		name.Format("水禁:%s",master->Water[masterindex]=="None."?"没有":master->Water[masterindex]=="Alarm."?"报警":"正常");
		BkDc.TextOut(10,700,name);

		name.Empty();
		name.Format("温度1:%d°C",master->TH[masterindex][0]);
		BkDc.TextOut(10,720,name);

		name.Empty();
		name.Format("湿度1:%d%%",master->TH[masterindex][3]);
		BkDc.TextOut(10,740,name);

		name.Empty();
		name.Format("门禁1:%s",master->Door1[masterindex]=="None."?"没有":master->Door1[masterindex]=="Opened."?"打开":"正常");
		BkDc.TextOut(100,660,name);

		name.Empty();
		name.Format("门禁2:%s",master->Door2[masterindex]=="None."?"没有":master->Door2[masterindex]=="Opened."?"打开":"正常");
		BkDc.TextOut(100,680,name);

		name.Empty();
		name.Format("烟雾:%s",master->Smoke[masterindex]=="None."?"没有":master->Smoke[masterindex]=="Alarm."?"报警":"正常");
		BkDc.TextOut(100,700,name);


		name.Empty();
		name.Format("温度2:%d°C",master->TH[masterindex][1]);
		BkDc.TextOut(100,720,name);

		name.Empty();
		name.Format("湿度2:%d%%",master->TH[masterindex][4]);
		BkDc.TextOut(100,740,name);
	}

	if(salve)
	{
		name.Empty();
		name.Format("L1电压:%dV",salve->Tvol[slaveindex][0]);
		BkDc.TextOut(420,660,name);
		name.Empty();
		name.Format("L1电流:%.1fA",salve->Tcur[slaveindex][0]);
		BkDc.TextOut(420,680,name);
		name.Empty();
		name.Format("水禁:%s",salve->Water[slaveindex]=="None."?"没有":salve->Water[slaveindex]=="Alarm."?"报警":"正常");		
		BkDc.TextOut(420,700,name);

		name.Empty();
		name.Format("温度1:%d°C",salve->TH[slaveindex][0]);
		BkDc.TextOut(420,720,name);

		name.Empty();
		name.Format("湿度1:%d%%",salve->TH[slaveindex][3]);
		BkDc.TextOut(420,740,name);

		name.Empty();
		name.Format("门禁1:%s",salve->Door1[slaveindex]=="None."?"没有":salve->Door1[slaveindex]=="Opened."?"打开":"正常");		
		BkDc.TextOut(520,660,name);

		name.Empty();
		name.Format("门禁2:%s",salve->Door2[slaveindex]=="None."?"没有":salve->Door2[slaveindex]=="Opened."?"打开":"正常");
		BkDc.TextOut(520,680,name);

		name.Empty();
		name.Format("烟雾:%s",salve->Smoke[slaveindex]=="None."?"没有":salve->Smoke[slaveindex]=="Alarm."?"报警":"正常");
		BkDc.TextOut(520,700,name);

		name.Empty();
		name.Format("温度2:%d°C",salve->TH[slaveindex][1]);
		BkDc.TextOut(520,720,name);

		name.Empty();
		name.Format("湿度2:%d%%",salve->TH[slaveindex][4]);
		BkDc.TextOut(520,740,name);

		if(salve->Tvol[slaveindex][1]>50)
		{
			name.Empty();
			name.Format("L2电压:%dV",salve->Tvol[slaveindex][1]);
			BkDc.TextOut(340,680,name);
			name.Empty();
			name.Format("L2电流:%.1fA",salve->Tcur[slaveindex][1]);
			BkDc.TextOut(340,700,name);
			name.Empty();
		}

		if(salve->Tvol[slaveindex][2]>50)
		{
			name.Format("L3电压:%dV",salve->Tvol[slaveindex][2]);
			BkDc.TextOut(340,720,name);
			name.Empty();
			name.Format("L3电流:%.1fA",salve->Tcur[slaveindex][2]);
			BkDc.TextOut(340,740,name);
		}
	}

	////////////////////////////////////
	CBrush Green(RGB(96,96,96));
	BkDc.SelectObject(&Green);

	int m_n,s_n;
	m_n=0;
	s_n=0;
	for(i=0;i<24;i++)
	{
		if(master&&master->outputname[masterindex][i].GetLength()&&m_n<master->deviceOutputNum[masterindex])
		{
			m_n++;
		}
		if(salve&&salve->outputname[slaveindex][i].GetLength()&&s_n<salve->deviceOutputNum[slaveindex])
		{
			s_n++;
		}
	}

	for(i=0;i<24;i++)
	{
		if(p_icon->device_master[i].name.GetLength() && p_icon->device_master[i].start>0 && i< master->deviceOutputNum[p_icon->MasterNUM])
		{
			BkDc.Rectangle(210,78+(48-p_icon->device_master[i].start-p_icon->device_master[i].height)*12,
				380,79+(48-p_icon->device_master[i].start)*12);

			drawFont(&BkDc,0,RGB(64,196,64));
			name.Empty();
			name.Format("%d: %s  ",p_icon->device_master[i].start,p_icon->device_master[i].name);
			BkDc.TextOutA(220,78+(48-p_icon->device_master[i].start-p_icon->device_master[i].height)*12,
				name);

			CPen mypen(PS_DOT, 1, RGB(64,192,64));
			CPen *old = BkDc.SelectObject(&mypen);
			if(master)
			{
				if(master->outputname[masterindex][i].GetLength()&&p_icon->device_master[i].maininputNum<=m_n)
				{
					BkDc.MoveTo(90,54+24*p_icon->device_master[i].maininputNum);
					BkDc.LineTo(210,85+(48-p_icon->device_master[i].start-p_icon->device_master[i].height)*12);//机柜主机连线
				}
			}
			
			BkDc.SelectObject(old);
		}
		if(p_icon->device_slave[i].name.GetLength() && p_icon->device_slave[i].start>0 && i < salve->deviceOutputNum[p_icon->SlaveNUM])
		{
			BkDc.Rectangle(210,78+(48-p_icon->device_slave[i].start-p_icon->device_slave[i].height)*12,
				380,79+(48-p_icon->device_slave[i].start)*12);

			drawFont(&BkDc,0,RGB(64,196,64));
			name.Empty();
			name.Format("%d: %s  ",p_icon->device_slave[i].start,p_icon->device_slave[i].name);
			BkDc.TextOutA(220,78+(48-p_icon->device_slave[i].start-p_icon->device_slave[i].height)*12,
				name);

			CPen mypen(PS_DOT, 1, RGB(64,192,64));
			CPen *old = BkDc.SelectObject(&mypen);
			
			if(salve)
			{
				if(salve->outputname[slaveindex][i].GetLength()&&p_icon->device_slave[i].slaveinputNum<=s_n)
				{
					BkDc.MoveTo(510,54+24*p_icon->device_slave[i].slaveinputNum);
					BkDc.LineTo(380,85+(48-p_icon->device_slave[i].start-p_icon->device_slave[i].height)*12);//机柜主机连线
				}
			}
			BkDc.SelectObject(old);
		}
	}
	
	//////////////////////////////////////////第一个编辑控件
	/*
	CBrush Brush1(RGB(255,255,255));
	BkDc.SelectObject(&Brush1);
	BkDc.Rectangle(259,669,361,687);
	if(nCtrlID ==IDC_EDIT1) 
	{ 
		//   在第一个编辑框上 
		CString str1;
		GetDlgItemTextA(IDC_EDIT2,str1);
		SetDlgItemTextA(IDC_EDIT2,str1);
		CString str;
		GetDlgItemTextA(IDC_EDIT1,str);
		SetDlgItemTextA(IDC_EDIT1,str);
		m_edit.SetSel(str.GetLength(),str.GetLength());

	}
	drawFont(&BkDc,1,RGB(64,64,128));
	BkDc.TextOut(200,669,"修改IP：");
	BkDc.Rectangle(365,669,400,687);
	BkDc.TextOut(367,670,"左");
	BkDc.Rectangle(365,689,400,707);
	BkDc.TextOut(367,690,"右");
	////////////////////////////////////////////第二个编辑控件
	if(nCtrlID==IDC_EDIT2) 
	{ 
		//在第二个编辑框上 
		CString str;
		GetDlgItemTextA(IDC_EDIT1,str);
		SetDlgItemTextA(IDC_EDIT1,str);
		CString str1;
		GetDlgItemTextA(IDC_EDIT2,str1);
		SetDlgItemTextA(IDC_EDIT2,str1);
		m_edit1.SetSel(str1.GetLength(),str1.GetLength());

	}
	GetDlgItem(IDC_EDIT1)->MoveWindow(260,670,100,16);
	GetDlgItem(IDC_EDIT2)->MoveWindow(260,690,100,16);
	BkDc.Rectangle(259,689,361,707);
	BkDc.TextOut(200,689,"替换IP：");
	////////////////显示到设备///////////////////////
	*/
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CCanbinNPMDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.x>580 && point.x<595 
		&& point.y>4 && point.y<19)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	int outputmasternum=0;
	while(1)
	{
		if(master)
		{
			if(!master->outputname[p_icon->MasterNUM][outputmasternum].GetLength()||outputmasternum>=master->deviceOutputNum[p_icon->MasterNUM])
			{
				break;
			}
			else if(outputmasternum < master->deviceOutputNum[p_icon->MasterNUM] )
				outputmasternum++;
		}
		else
			break;
	}
	int outputslavenum=0;
	while(1)
	{
		if(salve)
		{
			if(!salve->outputname[p_icon->SlaveNUM][outputslavenum].GetLength()||outputslavenum >= salve->deviceOutputNum[p_icon->SlaveNUM])
			{
				break;
			}
			else if(outputslavenum < salve->deviceOutputNum[p_icon->SlaveNUM] )
				outputslavenum++;
		}
		else
			break;
	}
	int i;
	for(i=0;i<outputslavenum;i++)
	{
		if(point.x>60 && point.x<90 
			&& point.y>70+24*i && point.y<88+24*i)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		}
	}

	for(i=0;i<outputmasternum;i++)
	{
		if(point.x>510 && point.x<540 
			&& point.y>70+24*i && point.y<88+24*i)
		{
			::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
		}
	}
	if(point.x>20 && point.x<200 
		&& point.y>32 && point.y<45)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>420 && point.x<600 
		&& point.y>32 && point.y<45)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CCanbinNPMDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(point.x>580 && point.x<595 
		&& point.y>4 && point.y<19)
	{
		OnOK();
	}
	int outputmasternum=0;
	while(1)
	{
		if(master)//&&)
		{
			if(!master->outputname[p_icon->MasterNUM][outputmasternum].GetLength()||outputmasternum>=master->deviceOutputNum[p_icon->MasterNUM])
			{
				break;
			}
			else if(outputmasternum < master->deviceOutputNum[p_icon->MasterNUM])
				outputmasternum++;
		}
		else
			break;
	}
	
	CString dtype;
	int i;
	for(i=0;i<outputmasternum;i++)
	{
		if(point.x>60 && point.x<90 && point.y>70+24*i && point.y<88+24*i)
		{
			if(master)
			{
				if(master->status>0)
				{
					dtype=master->devicetype[masterindex].Right(1);
					//AfxMessageBox(dtype);
					if(dtype!="B" && dtype!="A")
					{
						checkoutput = i;
						CMakeSureDlg sure;
						sure.p_cabin = this;
						sure.setOrder(13);
						sure.DoModal();
					}
				}
			}
		}
	}
	int outputslavenum=0;
	while(1)
	{
		if(salve)
		{
			if(!salve->outputname[p_icon->SlaveNUM][outputslavenum].GetLength()||outputslavenum >= salve->deviceOutputNum[p_icon->SlaveNUM])
			{
				break;
			}
			else if(outputslavenum < salve->deviceOutputNum[p_icon->SlaveNUM])
				outputslavenum++;
		}
		else
			break;
	}
	for(i=0;i<outputslavenum;i++)
	{
		if(point.x>510 && point.x<540 && point.y>70+24*i && point.y<88+24*i)
		{
			if(salve)
			{
				if(salve->status>0)
				{
					dtype=salve->devicetype[slaveindex].Right(1);
					
					if(dtype!="B" && dtype!="A")
					{
						checkoutput = i;
						CMakeSureDlg sure;
						sure.p_cabin = this;
						sure.setOrder(14);
						sure.DoModal();
					}
				}
			}
		}
	}

	if(point.x>20 && point.x<200 && point.y>32 && point.y<45)
	{
		if(master)
		{
			if(master->slaveOffline[masterindex]==2)
			{
				CMakeSureDlg sure;
				sure.p_cabin = this;
				sure.setOrder(15);
				sure.DoModal();
			}
		}
	}
	else if(point.x>420 && point.x<600 && point.y>32 && point.y<45)
	{
		if(salve)
		{
			if(salve->slaveOffline[slaveindex]==2)
			{
				CMakeSureDlg sure;
				sure.p_cabin = this;
				sure.setOrder(16);
				sure.DoModal();
			}
		}
	}
	CClientDC dc(this);//连接master设备IP
	if(master!=NULL)
	{
		CPoint pt1;
		CString name1;
		name1.Format("设备设置:%s->%d",master->ipaddr,masterindex);
		CSize sz1=dc.GetTextExtent(name1);
		pt1.x=10+sz1.cx;
		pt1.y=640+sz1.cy;
		if(point.x>10&&point.x<pt1.x&&point.y>640&&point.y<pt1.y)
		{
			ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+master->ipaddr,NULL,NULL, SW_NORMAL);
		}
	}
	if(salve!=NULL)//连接slave设备IP
	{
		CString name2;
		CPoint pt2;
		name2.Format("设备设置:%s->%d",salve->ipaddr,slaveindex);
		CSize sz2=dc.GetTextExtent(name2);
		pt2.x=420+sz2.cx;
		pt2.y=640+sz2.cy;
		if(point.x>420&&point.x<pt2.x&&point.y>640&&point.y<pt2.y)
		{
			ShellExecute((HWND)::AfxGetMainWnd(),"open", "http://"+salve->ipaddr,NULL,NULL, SW_NORMAL);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CCanbinNPMDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!mapEdit)
	{
		return;
	}
	if(point.x>190&&point.x<400&&point.y>50&&point.y<660)
	{
		CMenu Popmenu;
		CMenu* pMenu;
		CPoint pt;
		::GetCursorPos(&pt);
		Popmenu.LoadMenu(IDR_CABIN_DEVICE);
		pMenu = Popmenu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
	}
	int outputmasternum=0;
	while(1)
	{
		if(master)
		{
			if(!master->outputname[p_icon->MasterNUM][outputmasternum].GetLength()||outputmasternum>=master->deviceOutputNum[p_icon->MasterNUM])
			{
				break;
			}
			else if(outputmasternum < master->deviceOutputNum[p_icon->MasterNUM])
				outputmasternum++;
		}
		else
			break;
	}
	int i;
	for(i=0;i<outputmasternum;i++)
	{
		if(point.x>60 && point.x<90 
			&& point.y>70+24*i && point.y<88+24*i)
		{
			if(master)
			{
				if(master->status>0)
				{
					for(int m=0;m<outputmasternum;m++)
					{
						if(p_icon->device_master[m].name.GetLength()&&p_icon->device_master[m].maininputNum==(i+1)&&p_icon->device_master[m].start>0)
						{
							checkoutput = i;
							CMakeSureDlg sure;
							sure.p_cabin = this;
							sure.setOrder(25);
							sure.DoModal();
							break;
						}
					}
				}
			}
		}
	}
	int outputslavenum=0;
	while(1)
	{
		if(salve)
		{
			if(!salve->outputname[p_icon->SlaveNUM][outputslavenum].GetLength()||outputslavenum >= salve->deviceOutputNum[p_icon->SlaveNUM])
			{
				break;
			}
			else if(outputslavenum < salve->deviceOutputNum[p_icon->SlaveNUM]) 
				outputslavenum++;
		}
		else
			break;
	}
	for(i=0;i<outputslavenum;i++)
	{
		if(point.x>510 && point.x<540 
			&& point.y>70+24*i && point.y<88+24*i)
		{
			if(salve)
			{
				if(salve->status>0)
				{
					for(int m=0;m<outputslavenum;m++)
					{
						if(p_icon->device_slave[m].name.GetLength()&&p_icon->device_slave[m].slaveinputNum==(i+1)&&p_icon->device_slave[m].start>0)
						{
							checkoutput = i;
							CMakeSureDlg sure;
							sure.p_cabin = this;
							sure.setOrder(26);
							sure.DoModal();
							break;
						}
					}
				}
			}
		}
	}

	CDialog::OnRButtonDown(nFlags, point);
}

void CCanbinNPMDlg::OnAddCabinDevice()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(5);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::OnDelCabinDevice()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(6);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(CABIN_TIMER == nIDEvent)
	{
		CDC *pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}

	CDialog::OnTimer(nIDEvent);
}
/*
void CCanbinNPMDlg::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	nCtrlID=0;
}

void CCanbinNPMDlg::OnEnSetfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	nCtrlID=1003;
}

void CCanbinNPMDlg::OnEnKillfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	nCtrlID=0;
}

void CCanbinNPMDlg::OnEnSetfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	nCtrlID=1004;
}

*/
void CCanbinNPMDlg::Dcname(CString DCnameCenterMap)
{
	DCname=DCnameCenterMap;
}
LRESULT CCanbinNPMDlg::OnNcHitTest(CPoint point)
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
	
}
void CCanbinNPMDlg::OnAddSlaveDevice()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(27);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::OnDelSlaveDevice()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(29);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::On32876()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(50);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::On32877()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(51);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::On32878()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(52);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}

void CCanbinNPMDlg::On32879()
{
	// TODO: 在此添加命令处理程序代码
	if(p_icon)
	{
		CMakeSureDlg dlg;
		dlg.p_icon = p_icon;
		dlg.setOrder(53);
		dlg.DoModal();
		CDC * pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}
}
