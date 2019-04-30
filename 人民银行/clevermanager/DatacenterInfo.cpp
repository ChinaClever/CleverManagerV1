// DatacenterInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "DatacenterInfo.h"
#include "atlimage.h"
#include "DrawGraph.h"
#include "Meter.h"
#include "CanbinObjIcon.h"
#include "CanbinNPMDlg.h"
// CDatacenterInfo 对话框

IMPLEMENT_DYNAMIC(CDatacenterInfo, CDialog)

CDatacenterInfo::CDatacenterInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDatacenterInfo::IDD, pParent)
	, DCname(_T(""))
{

}

CDatacenterInfo::~CDatacenterInfo()
{
}

void CDatacenterInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatacenterInfo, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDatacenterInfo 消息处理程序

BOOL CDatacenterInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	rect.right -= 1;
	rect.bottom -= 1;
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
	//BkDc.SelectStockObject(NULL_PEN);
	//rect.bottom -= 1;
	//rect.right -= 1;
	BkDc.Rectangle(&rect);
	//::SetStretchBltMode(BkDc.GetSafeHdc(),COLORONCOLOR);
	CImage img; 
	HRESULT ret = img.Load(getFilePath()+"\\icon\\add.png"); // filename 是要加载的文件名（包含路径）
	drawFont(&BkDc,1,RGB(0,0,0));
	CSize fontsize= BkDc.GetTextExtent(DCname);
	BkDc.TextOut(rect.Width()/2-fontsize.cx/2,5,DCname);
	BkDc.MoveTo(0,25);
	BkDc.LineTo(rect.Width(),25);

	drawFont(&BkDc,0,RGB(96,192,96));
	BkDc.TextOut(28,8,"前一页 ");
	BkDc.TextOut(rect.right-60,8,"后一页");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\l.jpg",2,7,15,15);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\r.jpg",rect.right-22,7,15,15);

	int i,j;
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name == DCname && DCname.GetLength())
		{
			CDrawGraph p_draw(5,30,rect.Width()-5,rect.Height()*40/100);
#ifndef DEMO
			for(j=0;j<24;j++)
			{
				//for(int m=0;m<60;m++)
				//{
					//p_draw.value[j][m]=DCmember[i].kva[j][m];  //机房信息功率
					p_draw.value[j]=DCmember[i].kva[j];  //机房信息功率
				//}
			}
#endif
			p_draw.onDraw(&BkDc);

			drawbar(&BkDc,i);
			drawinline(&BkDc,i);
			drawTH(&BkDc,i);
		}
	}
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true; 


	//return CDialog::OnEraseBkgnd(pDC);
}

void CDatacenterInfo::drawbar(CDC *pDC,int index)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(5,rect.Height()*40/100+10,rect.Width()*20/100,rect.Height()-5);

	//CBrush brushtitle(RGB(230,235,242));
	//pDC->SelectObject(&brushtitle);
	//pDC->Rectangle(5,rect.Height()*40/100+10,rect.Width()*20/100,rect.Height()*40/100+35);
	retange(pDC,6,rect.Height()*40/100+11,rect.Width()*20/100-7,24);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(15,rect.Height()*40/100+15,"该机房内NPM工作状态");
	CBrush body(RGB(233,240,249));
	pDC->SelectObject(&body);
	pDC->Rectangle(5,rect.Height()*40/100+34,rect.Width()*20/100,rect.Height()-5);
	//////////////////////////////////////////
	pDC->MoveTo(20,rect.Height()-50);
	pDC->LineTo(rect.Width()*20/100-20,rect.Height()-50);
	int total = DCmember[index].normalnum+DCmember[index].alarmnum +DCmember[index].offnum;
	drawImagePNG(pDC,getFilePath()+"\\icon\\green.png",20+(rect.Width()*20/100-40)/7,rect.Height()-50,(rect.Width()*20/100-40)/7,
		total>0? -((rect.Height()*40/100)*DCmember[index].normalnum/total) :0);

	drawFont(pDC,0,RGB(64,64,64));
	CString num;
	num.Format("%d",DCmember[index].normalnum);
	pDC->TextOut(rect.Width()*5/100,rect.Height()-70-(total>0? (rect.Height()*40/100)*DCmember[index].normalnum/total :0),num);

	drawImagePNG(pDC,getFilePath()+"\\icon\\yellow.png",20+(rect.Width()*20/100-40)*3/7,rect.Height()-50,(rect.Width()*20/100-40)/7,
		total>0? -((rect.Height()*40/100)*DCmember[index].offnum/total) :0);

	num.Empty();
	num.Format("%d",DCmember[index].offnum);
	pDC->TextOut(rect.Width()*10/100,rect.Height()-70-(total>0? (rect.Height()*40/100)*DCmember[index].offnum/total :0),num);

	drawImagePNG(pDC,getFilePath()+"\\icon\\red.png",20+(rect.Width()*20/100-40)*5/7,rect.Height()-50,(rect.Width()*20/100-40)/7,
		total>0? -((rect.Height()*40/100)*DCmember[index].alarmnum/total) :0);

	num.Empty();
	num.Format("%d",DCmember[index].alarmnum);
	pDC->TextOut(rect.Width()*15/100,rect.Height()-70-(total>0? (rect.Height()*40/100)*DCmember[index].alarmnum/total :0),num);

	//pDC->TextOut(25,rect.Height()-40,"PDU status information");
	pDC->TextOut(rect.Width()*3/100,rect.Height()-45,"正常");
	pDC->TextOut(rect.Width()*9/100,rect.Height()-45,"离线");
	pDC->TextOut(rect.Width()*15/100,rect.Height()-45,"异常");
	//pDC->TextOut(25,rect.Height()-25,"PDU status information");
}

void CDatacenterInfo::drawinline(CDC *pDC,int index)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*20/100+5,rect.Height()*40/100+10,rect.Width()*80/100,rect.Height()-5);

	//CBrush brushtitle(RGB(230,235,242));
	//pDC->SelectObject(&brushtitle);
	//pDC->Rectangle(rect.Width()*20/100+5,rect.Height()*40/100+10,rect.Width()*80/100,rect.Height()*40/100+35);
	retange(pDC,rect.Width()*20/100+6,rect.Height()*40/100+11,rect.Width()*60/100-7,25);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(rect.Width()*20/100+15,rect.Height()*40/100+15,"该机房电力输入信息");
	CBrush body(RGB(233,240,249));
	pDC->SelectObject(&body);
	pDC->Rectangle(rect.Width()*20/100+5,rect.Height()*40/100+34,rect.Width()*80/100,rect.Height()-5);

	/////////////////////////////
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(rect.Width()*20/100+35,rect.Height()*40/100+49,"线路输入:");
	pDC->TextOut(rect.Width()*20/100+35,rect.Height()*55/100,"电流:");
	pDC->TextOut(rect.Width()*20/100+35,rect.Height()*75/100,"电压:");
	pDC->TextOut(rect.Width()*20/100+35,rect.Height()*92/100,"功率:");
	pDC->TextOut(rect.Width()*20/100+35,rect.Height()*96/100,"电能:");
	pDC->TextOut(rect.Width()*36/100,rect.Height()*40/100+49,"L1");
	pDC->TextOut(rect.Width()*52/100,rect.Height()*40/100+49,"L2");
	pDC->TextOut(rect.Width()*68/100,rect.Height()*40/100+49,"L3");
}

void CDatacenterInfo::drawTH(CDC *pDC,int index)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.Width()*80/100+5,rect.Height()*40/100+10,rect.Width()-5,rect.Height()-5);

	//CBrush brushtitle(RGB(230,235,242));
	//pDC->SelectObject(&brushtitle);
	//pDC->Rectangle(rect.Width()*80/100+5,rect.Height()*40/100+10,rect.Width()-5,rect.Height()*40/100+35);
	retange(pDC,rect.Width()*80/100+6,rect.Height()*40/100+11,rect.Width()*20/100-12,25);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(rect.Width()*80/100+15,rect.Height()*40/100+15,"该机房内温湿度状态");
	CBrush body(RGB(233,240,249));
	pDC->SelectObject(&body);
	pDC->Rectangle(rect.Width()*80/100+5,rect.Height()*40/100+34,rect.Width()-5,rect.Height()-5);
	////////////////////////
	////////////////////////
	float maxvalue;
	maxvalue = (DCmember[index].cur1>DCmember[index].cur2? DCmember[index].cur1:DCmember[index].cur2)
					> DCmember[index].cur3? (DCmember[index].cur1>DCmember[index].cur2? DCmember[index].cur1:DCmember[index].cur2)
					:DCmember[index].cur3;
	CMeter meter1(0,rect.Width()*32/100,rect.Height()*45/100+49
		,rect.Width()*41/100,rect.Height()*65/100);
	meter1.SetValue((int)DCmember[index].cur1);
	meter1.maxvalue = (int)maxvalue;
	meter1.onDraw(pDC);

	CMeter meter2(0,rect.Width()*48/100,rect.Height()*45/100+49
		,rect.Width()*57/100,rect.Height()*65/100);
	meter2.SetValue((int)DCmember[index].cur2);
	meter1.maxvalue = (int)maxvalue;
	meter2.onDraw(pDC);

	CMeter meter3(0,rect.Width()*64/100,rect.Height()*45/100+49
		,rect.Width()*73/100,rect.Height()*65/100);
	meter3.SetValue((int)DCmember[index].cur3);
	meter1.maxvalue = (int)maxvalue;
	meter3.onDraw(pDC);

	CMeter meter4(1,rect.Width()*32/100,rect.Height()*66/100+49
		,rect.Width()*41/100,rect.Height()*86/100);
	meter4.SetValue(DCmember[index].vol1);
	meter4.onDraw(pDC);

	CMeter meter5(1,rect.Width()*48/100,rect.Height()*66/100+49
		,rect.Width()*57/100,rect.Height()*86/100);
	meter5.SetValue(DCmember[index].vol2);
	meter5.onDraw(pDC);

	CMeter meter6(1,rect.Width()*64/100,rect.Height()*66/100+49
		,rect.Width()*73/100,rect.Height()*86/100);
	meter6.SetValue(DCmember[index].vol3);
	meter6.onDraw(pDC);

	CMeter meter7(2,rect.Width()*82/100,rect.Height()*50/100
		,rect.Width()*88/100,rect.Height()*70/100);
	meter7.SetValue(DCmember[index].tp);
	meter7.onDraw(pDC);

	CMeter meter8(3,rect.Width()*82/100,rect.Height()*75/100
		,rect.Width()*88/100,rect.Height()*95/100);
	meter8.SetValue(DCmember[index].hm);
	meter8.onDraw(pDC);

	///////////////////////////////////
	drawFont(pDC,1,RGB(32,32,32));
	CString show;
	show.Format("%.1f A",DCmember[index].cur1);
	pDC->TextOut(rect.Width()*35/100,rect.Height()*65/100+10,show);
	show.Empty();
	show.Format("%.1f A",DCmember[index].cur2);
	pDC->TextOut(rect.Width()*51/100,rect.Height()*65/100+10,show);
	show.Empty();
	show.Format("%.1f A",DCmember[index].cur3);
	pDC->TextOut(rect.Width()*67/100,rect.Height()*65/100+10,show);

	show.Empty();
	show.Format("%d V",DCmember[index].vol1);
	pDC->TextOut(rect.Width()*35/100,rect.Height()*86/100+10,show);
	show.Empty();
	show.Format("%d V",DCmember[index].vol2);
	pDC->TextOut(rect.Width()*51/100,rect.Height()*86/100+10,show);
	show.Empty();
	show.Format("%d V",DCmember[index].vol3);
	pDC->TextOut(rect.Width()*67/100,rect.Height()*86/100+10,show);

	drawFont(pDC,2,RGB(32,128,32));
	show.Empty();
	show.Format("%.2f KVA",DCmember[index].vol1*DCmember[index].cur1/1000);
	pDC->TextOut(rect.Width()*34/100,rect.Height()*92/100,show);
	show.Empty();
	show.Format("%.2f KVA",DCmember[index].vol2*DCmember[index].cur2/1000);
	pDC->TextOut(rect.Width()*50/100,rect.Height()*92/100,show);
	show.Empty();
	show.Format("%.2f KVA",DCmember[index].vol3*DCmember[index].cur3/1000);
	pDC->TextOut(rect.Width()*66/100,rect.Height()*92/100,show);

	show.Empty();
	show.Format("%.1f KWH",DCmember[index].energy1/10);
	pDC->TextOut(rect.Width()*34/100,rect.Height()*96/100,show);
	show.Empty();
	show.Format("%.1f KWH",DCmember[index].energy2/10);
	pDC->TextOut(rect.Width()*50/100,rect.Height()*96/100,show);
	show.Empty();
	show.Format("%.1f KWH",DCmember[index].energy3/10);
	pDC->TextOut(rect.Width()*66/100,rect.Height()*96/100,show);

	//////////////////////////////////////////////////////

	drawFont(pDC,0,RGB(0,0,0));
	pDC->TextOut(rect.Width()*90/100,rect.Height()*50/100,"平均温度:");
	pDC->TextOut(rect.Width()*90/100,rect.Height()*75/100,"平均湿度:");

	drawFont(pDC,3,RGB(128,32,32));
	show.Empty();
	show.Format("%d ℃",DCmember[index].tp);
	pDC->TextOut(rect.Width()*91/100,rect.Height()*55/100,show);

	drawFont(pDC,3,RGB(32,32,128));
	show.Empty();
	show.Format("%d ",DCmember[index].hm);
	pDC->TextOut(rect.Width()*91/100,rect.Height()*80/100,show+"%");
}

BOOL CDatacenterInfo::OnInitDialog()
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
    SetTimer(ENV_TIMER,5*1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDatacenterInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent ==ENV_TIMER)
	{
		SYSTEMTIME   ct; 
		GetLocalTime(&ct);

		float totalKva = 0;
		int i;
		for(i=0;i<10;i++)
		{
			//int totalNum = 0;
			int vol1 =0,vol1num=0;
			int vol2 =0,vol2num=0;
			int vol3 =0,vol3num=0;
			float cur1 =0;
			float cur2 =0;
			float cur3 =0;
			float energy1=0;
			float energy2=0;
			float energy3=0;
			int offline=0,narmal=0,alarm=0;

			int temp =0,tempNum=0;
			int hum =0,humNum=0;
			if(DCmember[i].name.GetLength())
			{
				POSITION pos = DCmember[i].canbin.GetTailPosition ();
				while (pos != NULL)
				{
					CCanbinObjIcon* p_icon =NULL;
					p_icon= (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
					POSITION posObj = snmpobj.GetTailPosition ();
					while (posObj != NULL)
					{
						CSnmpObj *p_Object = NULL;
						p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posObj)));
						if(p_icon->ipMaster == p_Object->ipaddr && p_Object->ipaddr.GetLength() && p_Object->dtype==0)
						{
							if(p_Object->slaveOffline[p_icon->MasterNUM]==2)
							{
								alarm++;
							}
							else if(p_Object->slaveOffline[p_icon->MasterNUM]==1)
							{
								narmal++;
							}
							else
							{
								offline++;
							}
							//totalKva += (p_Object->Tcur[p_icon->MasterNUM][0])*(p_Object->Tvol[p_icon->MasterNUM][0])/1000;
							if(p_Object->slaveOffline[p_icon->MasterNUM])
							{
								if(p_Object->Tvol[p_icon->MasterNUM][0])
								{
									vol1 += p_Object->Tvol[p_icon->MasterNUM][0];
									vol1num++;
								}
								if(p_Object->Tvol[p_icon->MasterNUM][1])
								{
									vol2 += p_Object->Tvol[p_icon->MasterNUM][1];
									vol2num++;
								}
								if(p_Object->Tvol[p_icon->MasterNUM][2])
								{
									vol3 += p_Object->Tvol[p_icon->MasterNUM][2];
									vol3num++;
								}
							
								cur1 += p_Object->Tcur[p_icon->MasterNUM][0];
									
								cur2 += p_Object->Tcur[p_icon->MasterNUM][1];
							
								cur3 += p_Object->Tcur[p_icon->MasterNUM][2];
								
								if(p_Object->TH[p_icon->MasterNUM][0])
								{
									temp += p_Object->TH[p_icon->MasterNUM][0];
									tempNum++;
								}
								if(p_Object->TH[p_icon->MasterNUM][1])
								{
									temp += p_Object->TH[p_icon->MasterNUM][1];
									tempNum++;
								}
								if(p_Object->TH[p_icon->MasterNUM][2])
								{
									temp += p_Object->TH[p_icon->MasterNUM][2];
									tempNum++;
								}
								if(p_Object->TH[p_icon->MasterNUM][3])
								{
									hum += p_Object->TH[p_icon->MasterNUM][3];
									humNum++;
								}
								if(p_Object->TH[p_icon->MasterNUM][4])
								{
									hum += p_Object->TH[p_icon->MasterNUM][4];
									humNum++;
								}
								if(p_Object->TH[p_icon->MasterNUM][5])
								{
									hum += p_Object->TH[p_icon->MasterNUM][5];
									humNum++;
								}
							}
							energy1+=p_Object->TEnergy[p_icon->MasterNUM][0];
							energy2+=p_Object->TEnergy[p_icon->MasterNUM][1];
							energy3+=p_Object->TEnergy[p_icon->MasterNUM][2];
						}
						if(p_icon->ipSlave == p_Object->ipaddr && p_Object->ipaddr.GetLength() && p_Object->dtype==0)
						{
							if(p_Object->slaveOffline[p_icon->SlaveNUM]==2)
							{
								alarm++;
							}
							else if(p_Object->slaveOffline[p_icon->SlaveNUM]==1)
							{
								narmal++;
							}
							else
							{
								offline++;
							}
							if(p_Object->slaveOffline[p_icon->SlaveNUM])
							{
								totalKva += (p_Object->Tcur[p_icon->SlaveNUM][0])*(p_Object->Tvol[p_icon->SlaveNUM][0])/1000;
								if(p_Object->Tvol[p_icon->SlaveNUM][0])
								{
									vol1 += p_Object->Tvol[p_icon->SlaveNUM][0];
									vol1num++;
								}
								if(p_Object->Tvol[p_icon->SlaveNUM][1])
								{
									vol2 += p_Object->Tvol[p_icon->SlaveNUM][1];
									vol2num++;
								}
								if(p_Object->Tvol[p_icon->SlaveNUM][2])
								{
									vol3 += p_Object->Tvol[p_icon->SlaveNUM][2];
									vol3num++;
								}
								cur1 += p_Object->Tcur[p_icon->SlaveNUM][0];
									
								cur2 += p_Object->Tcur[p_icon->SlaveNUM][1];
							
								cur3 += p_Object->Tcur[p_icon->SlaveNUM][2];

								if(p_Object->TH[p_icon->SlaveNUM][0])
								{
									temp += p_Object->TH[p_icon->SlaveNUM][0];
									tempNum++;
								}
								if(p_Object->TH[p_icon->SlaveNUM][1])
								{
									temp += p_Object->TH[p_icon->SlaveNUM][1];
									tempNum++;
								}
								if(p_Object->TH[p_icon->SlaveNUM][2])
								{
									temp += p_Object->TH[p_icon->SlaveNUM][2];
									tempNum++;
								}
								if(p_Object->TH[p_icon->SlaveNUM][3])
								{
									hum += p_Object->TH[p_icon->SlaveNUM][3];
									humNum++;
								}
								if(p_Object->TH[p_icon->SlaveNUM][4])
								{
									hum += p_Object->TH[p_icon->SlaveNUM][4];
									humNum++;
								}
								if(p_Object->TH[p_icon->SlaveNUM][5])
								{
									hum += p_Object->TH[p_icon->SlaveNUM][5];
									humNum++;
								}
							}
							energy1+=p_Object->TEnergy[p_icon->SlaveNUM][0];
							energy2+=p_Object->TEnergy[p_icon->SlaveNUM][1];
							energy3+=p_Object->TEnergy[p_icon->SlaveNUM][2];
						}
					}
				}
				/////////////////
				if(vol1num>0)
					DCmember[i].vol1 = vol1/vol1num;
				else
					DCmember[i].vol1 = 0;
				if(vol2num>0)
					DCmember[i].vol2 = vol2/vol2num;
				else
					DCmember[i].vol2 = 0;
#ifndef DEMO
				if(vol3num>0)
					DCmember[i].vol3 = vol3/vol3num;
				else
					DCmember[i].vol3 = 0;
#else
				DCmember[i].vol3 = 220;
#endif
				DCmember[i].cur1 = cur1;
				DCmember[i].cur2 = cur2;
#ifndef DEMO
				DCmember[i].cur3 = cur3;
#else
				DCmember[i].cur3 = 16;
#endif
				DCmember[i].normalnum = narmal;
				DCmember[i].offnum = offline;
				DCmember[i].alarmnum =alarm;
				if(tempNum>0)
					DCmember[i].tp = temp/tempNum;
				else
					DCmember[i].tp = 0;

				if(humNum>0)
					DCmember[i].hm = hum/humNum;
				else
					DCmember[i].hm = 0;

				DCmember[i].energy1=energy1;
				DCmember[i].energy2=energy2;
				DCmember[i].energy3=energy3;
				totalKva = (DCmember[i].vol1*DCmember[i].cur1+ DCmember[i].vol2*DCmember[i].cur2 + DCmember[i].vol3*DCmember[i].cur3)/1000;
			//	DCmember[i].kva[ct.wHour][ct.wMinute] = totalKva;
				DCmember[i].kva[ct.wHour] = totalKva;
			}

		}
		CDC *pDC;
		pDC = GetDC();
		OnEraseBkgnd(pDC);
		ReleaseDC(pDC);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDatacenterInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i,j;
	CRect rect;
	GetClientRect(&rect);
	if(point.x>2 && point.x<67 &&
			point.y>2 && point.y<30)
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
					return;
				}
			}
		}
	}
	else if(point.x>rect.right-72 && point.x<rect.right-7 &&
		point.y>2 && point.y<30)
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
					return;
				}
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}
