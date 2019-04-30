// MakeSureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "MakeSureDlg.h"
#include "InitClever.h"
#include "CanbinObjIcon.h"
#include "InitClever.h"
#include "MainFrm.h"
#include "Show.h"
// CMakeSureDlg 对话框

IMPLEMENT_DYNAMIC(CMakeSureDlg, CDialog)
CString CMakeSureDlg::username = "";
CMakeSureDlg::CMakeSureDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMakeSureDlg::IDD, pParent)
, order(0)
, DCname(_T(""))
, datercenterNUM(-1)
{
	p_icon = NULL;
	p_list=NULL;
	p_device=NULL;
	p_cabin = NULL;
	p_emsats=NULL;
}

CMakeSureDlg::~CMakeSureDlg()
{
	onok=false;
	onok1=false;
}

void CMakeSureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMakeSureDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMakeSureDlg 消息处理程序

BOOL CMakeSureDlg::OnEraseBkgnd(CDC* pDC)
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
	//BkDc.SelectStockObject(NULL_PEN);
	BkDc.Rectangle(0,24,rect.Width(),rect.Height());

	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);

	//BkDc.MoveTo(0,25);
	//BkDc.LineTo(rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,23);

	drawFont(&BkDc,1,RGB(64,64,64));
	BkDc.TextOutA(20,5,"信息确认");

	drawFont(&BkDc,0,RGB(64,64,64));
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.Width()-25,6,12,12);
	CBrush brush1(RGB(240,240,240));
	BkDc.SelectObject(&brush1);
	BkDc.Rectangle(275,170,335,190);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\open.png",280,173,12,12);
	BkDc.TextOutA(295,175,"确定");

	//////////////////////////////////
	if(order==1)
	{
		//GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(60,68,110,18);

		BkDc.TextOutA(20,40,"增加一个新的机房区域");
		BkDc.TextOutA(20,70,"名称:");
	}
	else if(order==2)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(50,70,"确定删除选中的机房区域吗?");
	}
	else if(order==3)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(40,60,"是否启用机房地图编辑?");
		BkDc.TextOutA(40,80,"请输入账号信息确认.");
	}
	else if(order==4)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(100,50,120,18);//房间名字
		GetDlgItem(IDC_EDIT5)->MoveWindow(100,85,120,18);//主NPM
		GetDlgItem(IDC_EDIT3)->MoveWindow(265,85,40,18);//备用NPM

		GetDlgItem(IDC_EDIT4)->MoveWindow(100,125,120,18);//副机1
		GetDlgItem(IDC_EDIT1)->MoveWindow(265,125,40,18);//副机2
		BkDc.TextOutA(20,30,"请填写新增机柜NPM的详细信息:");
		BkDc.TextOutA(40,55,"机柜名字:");///房间名字
		BkDc.TextOutA(20,90,"主要NPM　IP:");
		BkDc.TextOutA(230,90,"副机:");
		BkDc.TextOutA(20,130,"备用NPM　IP:");
		BkDc.TextOutA(230,130,"副机:");
		BkDc.TextOutA(20,175,"副机(0 代表:主机,1-9代表:1-9副机序号)");

		////////////////显示到设备///////////////////////
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
		return true;
	}
	else if(order==5)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//备用端口
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		BkDc.TextOutA(20,40,"增加机架设备名称:");
		BkDc.TextOutA(20,70,"主要NPM配电端口:");
		//BkDc.TextOutA(200,70,"备用NPM配电端口:");
		BkDc.TextOutA(20,100,"机架位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"占用高度:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==6)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"删除机架（主机）");
		BkDc.TextOutA(20,80,"主机位置:");
	}
	else if(order==7)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		BkDc.TextOutA(20,70,"确定删除选中的用户吗?");
	}
	else if(order==8)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		bool admin =false;
		for(i=0;i<20;i++)
		{
			if(user[i].grade && strlen(user[i].name))
			{
				admin=true;
			}
		}
		if(!admin)
		{
			GetDlgItem(IDC_EDIT1)->ShowWindow(0);
			GetDlgItem(IDC_EDIT2)->ShowWindow(0);
			BkDc.TextOutA(20,70,"新增用户.");
			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
			BkDc.DeleteDC();
			return true;
		}
		BkDc.TextOutA(20,70,"新增用户,请确认!");
	}
	else if(order==9 || order==22|| order==35|| order==36)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"设备IP地址:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==10)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"删除选中的设备，请输入用户信息确认!");
	}
	else if(order==11)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"修改SMTP服务信息，请输入用户信息确认!");
	}
	else if(order==12)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"修改WEB服务信息，请输入用户信息确认!");
	}
	else if(order==13)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("主要NPM端口:%s   状态:%.1fA (%.1f-%.1f)",
				p_cabin->master->outputname[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curout[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curmin[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curmax[p_cabin->masterindex][p_cabin->checkoutput]);
			BkDc.TextOutA(20,40,detail);
			BkDc.TextOutA(20,70,"命令(ON/OFF):");
			BkDc.TextOutA(20,100,"SET community:");
			GetDlgItem(IDC_EDIT3)->MoveWindow(150,68,90,18);
			GetDlgItem(IDC_EDIT4)->MoveWindow(150,98,90,18);
		}
	}
	else if(order==14)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("备用NPM端口:%s   状态:%.1fA (%.1f-%.1f)",
				p_cabin->salve->outputname[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curout[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curmin[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curmax[p_cabin->slaveindex][p_cabin->checkoutput]);
			BkDc.TextOutA(20,40,detail);
			BkDc.TextOutA(20,70,"命令(ON/OFF):");
			BkDc.TextOutA(20,100,"SET community:");
			GetDlgItem(IDC_EDIT3)->MoveWindow(150,68,90,18);
			GetDlgItem(IDC_EDIT4)->MoveWindow(150,98,90,18);
		}
	}
	else if(order==15)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("接收到异常信息:%s",p_cabin->master->trapinformation);
			BkDc.TextOutA(20,50,detail);
			BkDc.TextOutA(20,90,"是否清除SNMP异常信息，请输入用户信息确认!");
		}
	}
	else if(order==16)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("接收到异常信息:%s",p_cabin->salve->trapinformation);
			BkDc.TextOutA(20,50,detail);
			BkDc.TextOutA(20,90,"是否清除SNMP异常信息，请输入用户信息确认!");
		}
	}
	else if(order==17)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_device)
		{
			if(p_device->ipselect && p_device->slaveselect>=0 && p_device->slaveselect<10 &&
				p_device->ipselect->slaveOffline[p_device->slaveselect] ==2)
			{
				CString detail;
				detail.Format("接收到异常信息:%s",p_device->ipselect->trapinformation);
				BkDc.TextOutA(20,50,detail);
				BkDc.TextOutA(20,90,"是否清除SNMP异常信息，请输入用户信息确认!");
			}
		}
	}
	else if(order==18)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"解除选中设备异常信息，请输入用户信息确认!");
	}
	else if(order==19)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"EMS IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==20)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"ATS IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==21||order==41)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"删除选中设备，请输入用户信息确认!");
	}
	else if(order==23)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(40,60,"是否关闭机房地图编辑?");
		BkDc.TextOutA(40,80,"请输入账号信息确认.");
	}

	else if(order==24)
	{
		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_master[i].maininputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_master[i].name;
				mainnum=p_cabin->p_icon->device_master[i].maininputNum;
				slavenum=p_cabin->p_icon->device_master[i].slaveinputNum;
				height=p_cabin->p_icon->device_master[i].height;
				start=p_cabin->p_icon->device_master[i].start;
				mainputnum=p_cabin->p_icon->device_master[i].maininputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		this->SetDlgItemInt(IDC_EDIT4,height);
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		this->SetDlgItemInt(IDC_EDIT6,mainnum);
		GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//备用端口
		this->SetDlgItemInt(IDC_EDIT5,slavenum);

		BkDc.TextOutA(20,40,"增加机架:");
		BkDc.TextOutA(20,70,"主端口:");
		BkDc.TextOutA(200,70,"副端口:");
		BkDc.TextOutA(20,100,"位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"高度:");BkDc.TextOutA(305,100,"U");
	}

	else if(order==25)
	{

		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_master[i].maininputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_master[i].name;
				mainnum=p_cabin->p_icon->device_master[i].maininputNum;
				slavenum=p_cabin->p_icon->device_master[i].slaveinputNum;
				height=p_cabin->p_icon->device_master[i].height;
				start=p_cabin->p_icon->device_master[i].start;
				mainputnum=p_cabin->p_icon->device_master[i].maininputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		this->SetDlgItemInt(IDC_EDIT4,height);
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		this->SetDlgItemInt(IDC_EDIT6,mainnum);

		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		BkDc.TextOutA(20,40,"增加机架:");
		BkDc.TextOutA(20,70,"主端口:");
		BkDc.TextOutA(20,100,"位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"高度:");BkDc.TextOutA(305,100,"U");
	}

	else if(order==26)
	{

		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_slave[i].slaveinputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_slave[i].name;
				mainnum=p_cabin->p_icon->device_slave[i].maininputNum;
				slavenum=p_cabin->p_icon->device_slave[i].slaveinputNum;
				height=p_cabin->p_icon->device_slave[i].height;
				start=p_cabin->p_icon->device_slave[i].start;
				slaveputnum=p_cabin->p_icon->device_slave[i].slaveinputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		this->SetDlgItemInt(IDC_EDIT4,height);
		//GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		//this->SetDlgItemInt(IDC_EDIT6,mainnum);
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//备用端口
		this->SetDlgItemInt(IDC_EDIT5,slavenum);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);

		BkDc.TextOutA(20,40,"增加机架:");
		//BkDc.TextOutA(20,70,"Main PDU port:");
		BkDc.TextOutA(20,70,"副端口:");
		BkDc.TextOutA(20,100,"位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"高度:");BkDc.TextOutA(305,100,"U");

	}


	else if(order==27)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//备用端口

		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		BkDc.TextOutA(20,40,"增加机架:");
		BkDc.TextOutA(20,70,"副机端口:");
		BkDc.TextOutA(20,100,"位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"高度:");BkDc.TextOutA(305,100,"U");

	}

	else if(order==28)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		//GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//备用端口

		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		BkDc.TextOutA(20,40,"增加机架:");
		//BkDc.TextOutA(20,70,"Main PDU port:");
		BkDc.TextOutA(20,70,"副机端口:");
		BkDc.TextOutA(20,100,"位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"高度:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==29)
	{
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"删除机架（副机）");
		BkDc.TextOutA(20,80,"副机位置:");
	}


	else if(order==30)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==31)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==32)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==33||order==40)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==34)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(100,50,120,18);//房间名字
		this->SetDlgItemTextA(IDC_EDIT7,this->cabinet_name);
		GetDlgItem(IDC_EDIT5)->MoveWindow(100,85,120,18);//主NPM
		this->SetDlgItemTextA(IDC_EDIT5,this->ip_master);
		GetDlgItem(IDC_EDIT4)->MoveWindow(265,85,40,18);//备用NPM
		this->SetDlgItemTextA(IDC_EDIT3,this->ip_slave);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,125,120,18);//副机1
		this->SetDlgItemInt(IDC_EDIT4,this->slave_masternum);
		GetDlgItem(IDC_EDIT1)->MoveWindow(265,125,40,18);//副机2
		this->SetDlgItemInt(IDC_EDIT1,this->slave_slavenum);
		BkDc.TextOutA(20,30,"请填写修改增机柜NPM的详细信息:");
		BkDc.TextOutA(40,55,"机柜名字:");///房间名字
		BkDc.TextOutA(20,90,"主要NPM　IP:");
		BkDc.TextOutA(230,90,"副机:");
		BkDc.TextOutA(20,130,"备用NPM　IP:");
		BkDc.TextOutA(230,130,"副机:");
		BkDc.TextOutA(20,175,"副机(0 代表:主机,1-9代表:1-9副机序号)");

		////////////////显示到设备///////////////////////
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
		return true;
	}
	else if(order==37)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"IP-PDU IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}

	else if(order==38)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}
	else if(order==39)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"设备 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}
	else if(order==50)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//高度
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//位置
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//主要端口
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//备用端口
		GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);
		BkDc.TextOutA(20,40,"增加机架设备名称:");
		BkDc.TextOutA(20,70,"主要NPM配电端口:");
		BkDc.TextOutA(180,70,"备用NPM配电端口:");
		//BkDc.TextOutA(200,70,"备用NPM配电端口:");
		BkDc.TextOutA(20,100,"机架位置:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"占用高度:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==51)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"删除双输入机架设备");
		BkDc.TextOutA(20,80,"设备在机架位置:");
	}
	else if(order==52)
	{
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//名字
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,105,40,18);//高度
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,105,40,18);//位置
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,60,35,18);//主要端口
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//备用端口
		GetDlgItem(IDC_EDIT5)->MoveWindow(280,60,35,18);
		BkDc.TextOutA(20,40,"增加机架设备名称:");
		GetDlgItem(IDC_EDIT9)->MoveWindow(120,85,35,18);
		GetDlgItem(IDC_EDIT8)->MoveWindow(280,85,35,18);
		BkDc.TextOutA(20,65,"主要NPM配电端口1:");
		BkDc.TextOutA(20,85,"主要NPM配电端口2:");
		BkDc.TextOutA(180,65,"备用NPM配电端口1:");
		BkDc.TextOutA(180,90,"备用NPM配电端口2:");
		//BkDc.TextOutA(200,70,"备用NPM配电端口:");
		BkDc.TextOutA(20,110,"机架位置:"); BkDc.TextOutA(145,110,"U");
		BkDc.TextOutA(180,110,"占用高度:");BkDc.TextOutA(305,110,"U");

	}
	else if(order==53)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"删除四路输入机架设备");
		BkDc.TextOutA(20,80,"设备在机架位置:");
	}
	BkDc.TextOutA(20,130,"账号:");
	BkDc.TextOutA(175,130,"密码:");
	////////////////显示到设备///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CMakeSureDlg::setOrder(int value)
{
	order = value;
}

void CMakeSureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.x>275 && point.x<335 && point.y>170 && point.y<190)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	if(point.x>320 && point.x<355 && point.y>0 && point.y<25)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMakeSureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.x>275 && point.x<335 && point.y>170 && point.y<190)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));//点击确定的入口点
		OnOK();

	}
	if(point.x>320 && point.x<355 && point.y>0 && point.y<25)
	{
		onok1=true;
		OnOK();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMakeSureDlg::makesure(void)
{
	int i;
	if(order==1)//新增机房
	{
		CString dc;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(dc);
		if(dc.GetLength()!=NULL)
		{
			for(i=0;i<10;i++)
			{
				if(DCmember[i].name.GetLength()==0)
				{
					//CString username;
					if(cheakUser()==0)
					{
						AfxMessageBox("用户账号和密码不匹配!");
						return;
					}
					else if(cheakUser()==3)
					{
						AfxMessageBox("该用户没有权限!");
						return;
					}
					else if(cheakUser()==1)
					{
						GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
					}
					else if(cheakUser()==2)
					{
						username="管理员";
					}


					int j;
					for(j=0;j<10;j++)
					{
						if(DCmember[j].name == dc && dc.GetLength())
						{
							AfxMessageBox("该机房已经存在列表中!");
							return;
						}
					}
					DCmember[i].name = dc;
					CInitClever writedata;
					writedata.dataCenterWrite();
					writeUserLog(1,1,username,"新增机房","增加机房("+DCmember[i].name+")到机房拓扑中.");
					onok=true;
					CShow show;
					show.AddRoom();
					return;
				}

			}
			AfxMessageBox("新增机房拓扑太多!");
		}
		else
		{
			AfxMessageBox("请填写机房名字");
		}
	}
	else if(order==2)//删除机房
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name==DCname && DCmember[i].name.GetLength())
			{
				//CString username;
				CString jifang =  DCmember[i].name;
				if(cheakUser()==0)
				{
					AfxMessageBox("用户账号和密码不匹配!");
					return;
				}
				else if(cheakUser()==3)
				{
					AfxMessageBox("该用户没有权限!");
					return;
				}
				else if(cheakUser()==1)
				{
					GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
				}
				else if(cheakUser()==2)
				{
					username="管理员";
				}

				int num=0;
				while(DCmember[num].name.GetLength())
				{
					num++;
				}

				POSITION pos = DCmember[i].canbin.GetHeadPosition ();
				while (pos != NULL)
					delete DCmember[i].canbin.GetNext (pos);
				DCmember[i].canbin.RemoveAll ();
				DCmember[i].name="";
				DCmember[i].str_draw="";
				DCname="";

				for(;i<(num-1);i++)
				{
					DCmember[i].name=DCmember[i+1].name;
					DCmember[i].str_draw=DCmember[i+1].str_draw;
				}
				DCmember[num-1].name="";
				DCmember[num-1].str_draw="";
				DCname="";

				CInitClever writedata;
				writedata.dataCenterWrite();
				writeUserLog(1,1,username,"删除机房","从机房拓扑中删除机房("+jifang+")。");
				CShow show;
				show.AddRoom();
				onok=true;
				//OnOK();
				return;
			}
		}
	}
	else if(order==3)//开启机房
	{
		if(p_map)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}

			if(p_map->mapEdit)
			{
				p_map->mapEdit=false;
				writeUserLog(1,2,username,"编辑机房拓扑","关闭机房拓扑编辑功能.");
			}
			else
			{
				p_map->mapEdit=true;
				writeUserLog(1,2,username,"编辑机房拓扑","开启机房拓扑编辑功能.");
			}
			onok=true;
			//OnOK();
			return;
		}
	}
	else if(order==4)//增加机柜
	{
		CString ip1,ip2,num1,num2,cabname;
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(ip1);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(ip2);
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(num2);
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(cabname);
		bool device1=false;
		bool device2=false;
		///////////////////
		POSITION pos = snmpobj.GetTailPosition ();

		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr == ip1 && ip1.GetLength() && p_Object->dtype==0)
			{
				int deviceindex1 = atoi(num1);
				if(deviceindex1>=0 && deviceindex1<=9)
				{
					if(p_Object->slavename[deviceindex1].GetLength())
					{
						device1=true;
					}
				}
			}
			if(p_Object->ipaddr == ip2 && ip2.GetLength() && p_Object->dtype==0)
			{
				int deviceindex2 = atoi(num2);
				if(deviceindex2>=0 && deviceindex2<=9)
				{
					if(p_Object->slavename[deviceindex2].GetLength())
					{
						device2=true;
					}
				}
			}
		}
		if(datercenterNUM<0)
		{
			MessageBox("请选新增一个机房拓扑地图.");
			return;
		}
		if(!device1 && ip1.GetLength()&&num1.GetLength()&&atoi(num1)>=0 && atoi(num1)<=9&& ip2.GetLength())
		{
			if(MessageBox( "请确认新增的主要NPM已经在NPM设备列表中.", "", MB_OKCANCEL) != IDOK) 
				return;
		}
		if(!device2 && ip2.GetLength()&&num2.GetLength()&&atoi(num2)>=0 && atoi(num2)<=9)
		{
			if(MessageBox( "请确认新增的配用NPM已经在NPM设备列表中.", "", MB_OKCANCEL) != IDOK) 
				return;
		}
		///////////////////
		CCanbinObjIcon* p_icon=NULL;
		if(ip1.GetLength() && num1.GetLength())
		{
			if(atoi(num1)>=0 && atoi(num1)<=9&&!ip2.GetLength())
			{
				p_icon = new CCanbinObjIcon(1,1,0,ip1,ip2,atoi(num1),atoi(num2),0);
				p_icon->cabientname=cabname;
				//p_icon->DCname=DCmember[datercenterNUM].name;
				CString detail;
				detail.Format("在机房(%s)中，新增一条NPM,主要NPM IP:%s->%s,备用NPM IP:%s->%s",DCmember[datercenterNUM].name,ip1,num1,ip2,num2);
				writeUserLog(1,3,username,"增加机柜",detail);
				
				onok=true;
			}
			else if(atoi(num1)>=0 && atoi(num1)<=9&&ip2.GetLength())
			{
				if(atoi(num2)>=0 && atoi(num2)<=9)
				{
					p_icon = new CCanbinObjIcon(1,1,0,ip1,ip2,atoi(num1),atoi(num2),0);
					p_icon->cabientname=cabname;
					//p_icon->DCname=DCmember[datercenterNUM].name;
					CString detail;
					detail.Format("在机房(%s)中，新增一条NPM,主要NPM IP:%s->%s,备用NPM IP:%s->%s",DCmember[datercenterNUM].name,ip1,num1,ip2,num2);
					writeUserLog(1,3,username,"增加机柜",detail);
					onok=true;
				}
				else
				{
					AfxMessageBox("备用的NPM副机取值范围在0-9，0为主机，1-9为副机,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("主要的NPM副机取值范围在0-9，0为主机，1-9为副机,请正确填写");
			}
		}
		else if(ip1.GetLength() && !num1.GetLength())
		{
			AfxMessageBox("请填写副机，副机取值范围在0-9，0为主机，1-9为副机,请正确填写");
		}

		else
		{
			AfxMessageBox("请填写设备IP和副机，副机取值范围在0-9，0为主机，1-9为副机,请正确填写");
		}
		if(p_icon && datercenterNUM>=0 && datercenterNUM<10)
		{
			p_icon->select=true;
			p_icon->move=true;
			DCmember[datercenterNUM].canbin.AddTail(p_icon);
			
		}
		return;
	}
	else if(order==5)//新增主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		int mNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum=atoi(num3);

		int m=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}
		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( atoi(num3) > 0 && atoi(num3) < ipm)
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("主端口取值在1-%d，请正确填写",m);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}
					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==6)//删除主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("请填写正确的机架位置");
		}
		return;
	}
	else if(order==7)//删除用户
	{
		if(p_list)
		{
			//CString username;
			CString name = p_list->m_list.GetItemText(p_list->m_list.GetSelectionMark(),1);
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
				if( username != name)
				{
					AfxMessageBox("该用户没有权限!");
					return;
				}
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			if(name.GetLength())
			{
				for(i=0;i<20;i++)
				{
					if(name == user[i].name)
					{
						memset(user[i].name,0,sizeof(char)*32);
						memset(user[i].psd,0,sizeof(char)*32);
						memset(user[i].mail,0,sizeof(char)*64);
						memset(user[i].detail,0,sizeof(char)*128);
						memset(user[i].tel,0,sizeof(char)*20);
						user[i].grade = 0;
					}
				}
				CInitClever init;
				init.userWrite();
				p_list->m_list.DeleteItem(p_list->m_list.GetSelectionMark());
				writeUserLog(1,4,username,"删除用户 ",username+"删除了用户"+name+".");
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==8)//新增用户
	{
		if(p_list)
		{
			//CString username;
			CString name;
			CString psd;
			CString mail;
			CString tel;
			CString detail;
			CString grade;

			p_list->GetDlgItem(IDC_USER_PSD)->GetWindowTextA(psd);
			p_list->GetDlgItem(IDC_USER_TEL)->GetWindowTextA(tel);
			p_list->GetDlgItem(IDC_USER_MAIL)->GetWindowTextA(mail);
			p_list->m_userEdit.GetWindowTextA(grade);
			p_list->GetDlgItem(IDC_USER_DETAIL)->GetWindowTextA(detail);

			p_list->GetDlgItem(IDC_USER_COUNT)->GetWindowTextA(name);
			if(name.GetLength()<17&&name.GetLength()>0)
			{

				if(psd.GetLength()<17&&psd.GetLength()>0)
				{


					if(mail.GetLength()!=0)
					{
						bool admin =false;
						for(i=0;i<20;i++)
						{
							if(user[i].grade && strlen(user[i].name))
							{
								admin=true;
							}
						}
						if(admin)
						{
							if(cheakUser()==0)
							{
								AfxMessageBox("用户账号和密码不匹配!");
								return;
							}
							else if(cheakUser()==3)
							{
								GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
								if( name != username)
								{
									AfxMessageBox("该用户没有权限!");
									return;
								}
							}
							else if(cheakUser()==1)
							{
								GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
							}
							else if(cheakUser()==2)
							{
								username="管理员";
							}
						}



						int usernum = -1;
						bool exist = false;
						admin =false;
						for(i=0;i<20;i++)
						{
							if(name == user[i].name && name.GetLength())
							{
								exist =true;
								usernum = i;
							}
							if(user[i].grade && strlen(user[i].name))
							{
								admin=true;
							}
						}
						for(i=0;i<20;i++)
						{
							if(strlen(user[i].name) == 0)
							{
								break;
							}
						}
						if(i>=20)
						{
							//OnOK();
							onok=true;
							AfxMessageBox("新增用户数量太多.");
							return;
						}
						if(!exist && name.GetLength() && psd.GetLength())
						{
							strncpy_s(user[i].name,sizeof(char)*32,name,sizeof(char)*30);
							strncpy_s(user[i].psd,sizeof(char)*32,psd,sizeof(char)*30);
							strncpy_s(user[i].mail,sizeof(char)*64,mail,sizeof(char)*62);
							strncpy_s(user[i].tel,sizeof(char)*32,tel,sizeof(char)*30);
							strncpy_s(user[i].detail,sizeof(char)*128,detail,sizeof(char)*126);

							CInitClever init;
							if(!admin)
							{
								p_list->m_userEdit.SetWindowTextA("可以编辑");
								user[i].grade = 1;

								init.userWrite();

								p_list->m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,0,0);
								p_list->m_list.SetItemText(0,1,user[i].name);
								p_list->m_list.SetItemText(0,2,"权限 :可以编辑");
								CString listtel;
								listtel.Format("电话:%s",user[i].tel);
								p_list->m_list.SetItemText(0,3,listtel);
								p_list->m_list.SetItemText(0,4,user[i].mail);
								writeUserLog(1,4,username,"新增用户",username+"新增用户，用户名:"+user[i].name+".");
								//OnOK();
								onok=true;
								return;
							}
							else if(grade == "可以编辑")
							{
								user[i].grade = 1;
							}
							init.userWrite();
							p_list->m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,0,0);
							p_list->m_list.SetItemText(0,1,user[i].name);
							if(user[i].grade == 1)
							{
								p_list->m_list.SetItemText(0,2,"权限:可以编辑");
							}
							else
							{
								p_list->m_list.SetItemText(0,2,"权限:不可编辑");
							}
							CString listtel;
							listtel.Format("电话:%s",user[i].tel);
							p_list->m_list.SetItemText(0,3,listtel);
							p_list->m_list.SetItemText(0,4,user[i].mail);
							writeUserLog(1,4,username,"新增用户",username+"新增用户，用户名:"+user[i].name+".");
							onok=true;
						}
						else if(exist && username.GetLength())
						{
							memset(user[usernum].name,0,sizeof(char)*32);
							memset(user[usernum].psd,0,sizeof(char)*32);
							memset(user[usernum].mail,0,sizeof(char)*64);
							memset(user[usernum].tel,0,sizeof(char)*32);
							memset(user[usernum].detail,0,sizeof(char)*128);
							strncpy_s(user[usernum].name,sizeof(char)*32,name,sizeof(char)*30);
							strncpy_s(user[usernum].psd,sizeof(char)*32,psd,sizeof(char)*30);
							strncpy_s(user[usernum].mail,sizeof(char)*64,mail,sizeof(char)*62);
							strncpy_s(user[usernum].tel,sizeof(char)*32,tel,sizeof(char)*30);
							strncpy_s(user[usernum].detail,sizeof(char)*128,detail,sizeof(char)*126);
							CInitClever init;
							/*if(cheakUser()==1)
							{
							if(grade == "Edit")
							{
							user[usernum].grade = 1;
							}
							else
							{
							user[usernum].grade = 0;
							admin = false;
							for(i=0;i<20;i++)
							{
							if(user[i].grade && strlen(user[i].name))
							{
							admin=true;
							}
							}
							if(!admin)
							{
							user[usernum].grade = 1;
							}
							}
							}*/
							init.userWrite();
							int len=p_list->m_list.GetItemCount();
							for(i=0;i<len;i++)
							{
								if(p_list->m_list.GetItemText(i,1) == name)
								{
									/*if(cheakUser()==1)
									{
									if(user[usernum].grade == 1)
									{
									p_list->m_list.SetItemText(i,2,"权限:Edit");
									}
									else
									{
									p_list->m_list.SetItemText(i,2,"权限:Unable edit");
									}
									}*/
									CString listtel;
									listtel.Format("电话:%s",user[usernum].tel);
									p_list->m_list.SetItemText(i,3,listtel);
									p_list->m_list.SetItemText(i,4,user[usernum].mail);
								}
							}
							writeUserLog(1,4,username,"编辑用户",username+"，该用户的信息被修改.");
							onok=true;
						}
						else
						{
							AfxMessageBox("用户信息输入不正确.");
						}
					}
					else
					{
						AfxMessageBox("e-mail不能为空");
					}
				}
				else
				{
					AfxMessageBox("密码的长度在1-16");
				}

			}

			else
			{
				AfxMessageBox("账号的长度在1-16");
			}
		}
		//OnOK();
		return;
	}
	else if(order==9 || order==22||order==35||order==36)//npm
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if( order == 9)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIII);
							CInitClever sure;
							sure.deviceListWrite();
							writeUserLog(1,5,username,"增加设备",username+" 增加了一个新设备,该设备的IP:"+ip);
							onok=true;
						}
						else if(order == 22)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,1));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIV);
							CInitClever sure;
							sure.NPMIVdevicewrite();
							writeUserLog(1,5,username,"增加设备",username+" 增加了一个新设备,该设备的IP:"+ip);
							onok=true;
						}
						else if(order == 35)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,2));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMV);
							CInitClever sure;
							sure.NPMVdevicewrite();
							writeUserLog(1,5,username,"增加设备",username+" 增加了一个新设备,该设备的IP:"+ip);
							onok=true;
						}
						else if(order == 36)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,3));
							p_device->m_tree.InsertItem(ip,1,1,p_device->MSPDU);
							CInitClever sure;
							sure.MSPDUdevicewrite();
							writeUserLog(1,5,username,"增加设备",username+" 增加了一个新设备,该设备的IP:"+ip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get community 为public，请正确填写");
				}
			}
			else
			{
				AfxMessageBox("ip不能为空");
			}
		}
		return;
	}
	else if(order==10)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString name = p_device->m_tree.GetItemText(p_device->m_tree.GetSelectedItem());
			if(name.GetLength())
			{
				POSITION pos = snmpobj.GetTailPosition ();
				while (pos != NULL)
				{
					CSnmpObj *p_Object = NULL;
					p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
					if(p_Object->ipaddr == name)
					{
						p_Object->ipaddr.Empty();
						p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
						CInitClever sure;
						sure.deviceListWrite();
						sure.NPMIVdevicewrite();
						sure.NPMVdevicewrite();
						sure.MSPDUdevicewrite();
						writeUserLog(1,5,username,"删除设备",username+" 删除了一个设备,该设备 IP:"+name);
						onok=true;
					}
				}
			}
			else
			{
				AfxMessageBox("请选择一个设备.");
			}
		}
		//OnOK();
		return;
	}
	else if(order==11)//smtp
	{
		if(p_list)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			char smtpuser[128];
			char psd[128];
			char smtpserver[128];
			char port[128];
			char type[128];
			memset(smtpuser,0,sizeof(char)*128);
			memset(psd,0,sizeof(char)*128);
			memset(smtpserver,0,sizeof(char)*128);
			memset(port,0,sizeof(char)*128);
			memset(type,0,sizeof(char)*128);
			CString value;
			p_list->GetDlgItem(IDC_SMTP_USER)->GetWindowTextA(value);
			strncpy_s(smtpuser,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_PSD)->GetWindowTextA(value);
			strncpy_s(psd,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_SERVER)->GetWindowTextA(value);
			strncpy_s(smtpserver,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_PORT)->GetWindowTextA(value);
			strncpy_s(port,sizeof(char)*128,value,sizeof(char)*126);

			value.Empty();
			p_list->m_mailCom.GetWindowTextA(value);
			strncpy_s(type,sizeof(char)*128,value,sizeof(char)*126);

			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeReadWrite|CFile::modeCreate);
			file.SeekToBegin();

			file.Write(smtpuser,128);
			file.Seek(128,CFile::begin);
			file.Write(psd,128);
			file.Seek(128*2,CFile::begin);
			file.Write(smtpserver,128);
			file.Seek(128*3,CFile::begin);
			file.Write(port,128);
			file.Seek(128*4,CFile::begin);
			file.Write(type,128);

			file.Close();
			writeUserLog(1,6,username,"STMP服务编辑",username+"修改了SMTP服务设置.");

		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==12)//修改WEB设置
	{
		if(p_list)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			char port[32];
			char timer[32];
			char start[32];
			memset(port,0,sizeof(char)*32);
			memset(timer,0,sizeof(char)*32);
			memset(start,0,sizeof(char)*32);
			CString value;
			p_list->GetDlgItem(IDC_WEB_PORT)->GetWindowTextA(value);
			strncpy_s(port,sizeof(char)*32,value,sizeof(char)*30);

			value.Empty();
			p_list->GetDlgItem(IDC_WEB_TIME)->GetWindowTextA(value);
			strncpy_s(timer,sizeof(char)*32,value,sizeof(char)*30);

			value.Empty();
			p_list->m_webCom.GetWindowTextA(value);
			strncpy_s(start,sizeof(char)*32,value,sizeof(char)*30);

			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\web",CFile::modeReadWrite|CFile::modeCreate);
			file.SeekToBegin();
			file.Write(port,32);
			file.Seek(32,CFile::begin);
			file.Write(timer,32);
			file.Seek(32*2,CFile::begin);
			file.Write(start,32);
			file.Close();
			writeUserLog(1,6,username,"WEB服务编辑",username+"修改了WEB服务设置.");
			MessageBox("请重启软件，是修改的设定生效.");
			onok=true;
			

		}
		//OnOK();

		return;
	}
	else if(order==13)//设置输出位开关
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CInitClever init;
			CString order;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(order);
			CString setcom;
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(setcom);
			if(order=="ON"||order=="on"||order=="off"||order=="OFF")
			{
				if(setcom=="private")
				{
					if(order.GetLength())
					{
						init.snmpset(p_cabin->master->ipaddr,p_cabin->masterindex,p_cabin->checkoutput,order
							,p_cabin->master->community,setcom);
					}
					CString detail;
					detail.Format("%s发送了\"%s\" 命令，控制设备(%s->%d)的输出位%d.",username,order ,p_cabin->master->ipaddr
						,p_cabin->masterindex,p_cabin->checkoutput+1);
					writeUserLog(1,6,username,"开关控制",detail);
					//OnOK();
					onok=true;
				}
				else
				{
					AfxMessageBox("Set community 是private，请正确填写");
				}
			}
			else
			{
				AfxMessageBox("请正确填写开关控制");
			}
		}

		return;
	}
	else if(order==14)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CInitClever init;
			CString order;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(order);
			CString setcom;
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(setcom);
			if(order.GetLength())
			{
				init.snmpset(p_cabin->salve->ipaddr,p_cabin->slaveindex,p_cabin->checkoutput,order
					,p_cabin->salve->community,setcom);
			}
			CString detail;
			detail.Format("%s发送了\"%s\"命令，控制设备(%s->%d) 输出位%d.",username,order,p_cabin->salve->ipaddr
				,p_cabin->slaveindex,p_cabin->checkoutput+1);
			writeUserLog(1,6,username,"开关控制",detail);
		}
		onok=true;
		//OnOK();
		return;
	}
	else if(order==15)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3 )
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			p_cabin->master->slaveOffline[p_cabin->masterindex] = 1;
			CString detail;
			detail.Format("%s解除设备(%s->%d)报警状态.",username,p_cabin->master->ipaddr,p_cabin->masterindex);
			writeUserLog(1,7,username,"解除报警",detail);
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==16)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			p_cabin->salve->slaveOffline[p_cabin->slaveindex] = 1;
			CString detail;
			detail.Format("%s解除设备(%s->%d)报警状态.",username,p_cabin->salve->ipaddr,p_cabin->slaveindex);
			writeUserLog(1,7,username,"解除报警",detail);
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==17)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			if(p_device->ipselect && p_device->slaveselect>=0 && p_device->slaveselect<10 &&
				p_device->ipselect->slaveOffline[p_device->slaveselect] ==2)
			{
				p_device->ipselect->slaveOffline[p_device->slaveselect] = 1;
				p_device->ipselect->status=1;
				CString detail;
				detail.Format("%s解除设备(%s->%d)报警状态.",username,p_device->ipselect->ipaddr,p_device->slaveselect);
				writeUserLog(1,7,username,"解除报警",detail);
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==18)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			if(p_emsats->p_select)
			{
				if(p_emsats->p_select->dtype==1)
				{
					if(p_emsats->p_select->EMSstatus[p_emsats->index]==0)
						p_emsats->p_select->EMSstatus[p_emsats->index]=0;
					else
						p_emsats->p_select->EMSstatus[p_emsats->index]=1;
				}
				else if(p_emsats->p_select->dtype==3)
				{
					if(p_emsats->p_select->ipStatus==0)
						p_emsats->p_select->ipStatus=0;
					else
						p_emsats->p_select->ipStatus=1;
				}
				else
				{
					if(p_emsats->p_select->ATS_status==0)
						p_emsats->p_select->ATS_status=0;
					else
						p_emsats->p_select->ATS_status=1;
				}
				CString detail;
				detail.Format("%s解除设备(%s->%s)报警状态.",username,p_emsats->p_select->ipaddr
					,p_emsats->p_select->EMSname[p_emsats->index]);
				writeUserLog(1,7,username,"解除报警",detail);
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==19)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if(p_emsats->ems)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,1));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ems);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"增加设备",username+"增加了一个新的设备，该设备IP:"+emsip);
							onok=true;
						}
					}
					//OnOK();
				}
				else
				{
					AfxMessageBox("Get community为public，请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}

		return;
	}
	else if(order==20)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString atsip;
			CString atscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(atsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(atscom);
			if(atsip.GetLength()!=0)
			{
				if(atscom=="public")
				{
					if(atsip.GetLength()  &&  atscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==atsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if(p_emsats->ems)
						{
							snmpobj.AddTail(new CSnmpObj(atsip,atscom,2));
							p_emsats->m_tree.InsertItem(atsip,3,3,p_emsats->ats);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"增加设备",username+"增加了一个新的设备，该设备IP:"+atsip);
							onok=true;
						}
					}
					//OnOK();
				}
				else
				{
					AfxMessageBox("Get community 是public，请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}

		return;
	}
	else if(order==21||order==41)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			if(p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ems
				|| p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ats
				|| p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ippdu)
			{
				CString name = p_emsats->m_tree.GetItemText(p_emsats->m_tree.GetSelectedItem());
				if(name.GetLength())
				{
					POSITION pos = snmpobj.GetTailPosition ();
					while (pos != NULL)
					{
						CSnmpObj *p_Object = NULL;
						p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
						if(p_Object->ipaddr == name)
						{
							if(order==21)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.atsemsListWrite();
								writeUserLog(1,5,username,"删除设备",username+"删除了一个设备，该设备IP:"+name);
								onok=true;
							}
							else if(order==41)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.ippduListWrite();
								writeUserLog(1,5,username,"删除设备",username+"删除了一个设备，该设备IP:"+name);
								onok=true;
							}
						}
					}
				}
				else
				{
					AfxMessageBox("请选择一个设备.");
				}
			}
		}
		//OnOK();
		return;
	}
	if(order==23)
	{
		if(p_map)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}

			if(p_map->mapEdit)
			{
				p_map->mapEdit=false;
				writeUserLog(1,2,username,"编辑机房拓扑","关闭机房拓扑编辑功能.");
			}
			else
			{
				p_map->mapEdit=true;
				writeUserLog(1,2,username,"编辑机房拓扑","开启机房拓扑编辑功能.");
			}
			//OnOK();
			onok=true;
			return;
		}

	}

	if(order==24)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="管理员";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_master[i].maininputNum == mainputnum)
				{
					p_cabin->p_icon->device_master[i].name = "";
					p_cabin->p_icon->device_master[i].start = 0;
					p_cabin->p_icon->device_master[i].height = 0;
					p_cabin->p_icon->device_master[i].maininputNum = 0;
					p_cabin->p_icon->device_master[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(mainnum);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_master[i].name.GetLength())
				{
					p_cabin->p_icon->device_master[i].name = name;
					p_cabin->p_icon->device_master[i].start =atoi(start);
					p_cabin->p_icon->device_master[i].height = atoi(height);
					p_cabin->p_icon->device_master[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_master[i].slaveinputNum = atoi(slavenum);
				}
			}
		}
		CString detail;
		detail.Format("机柜名字 %s 替换设备IP%s",p_cabin->p_icon->cabientname,p_cabin->p_icon->ipMaster);
		writeUserLog(1,5,username,"替换设备",detail);

		onok=true;
	}

	else if(order==25)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="管理员";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_master[i].maininputNum == mainputnum)
				{
					p_cabin->p_icon->device_master[i].name = "";
					p_cabin->p_icon->device_master[i].start = 0;
					p_cabin->p_icon->device_master[i].height = 0;
					p_cabin->p_icon->device_master[i].maininputNum = 0;
					p_cabin->p_icon->device_master[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(mainnum);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_master[i].name.GetLength())
				{
					p_cabin->p_icon->device_master[i].name = name;
					p_cabin->p_icon->device_master[i].start =atoi(start);
					p_cabin->p_icon->device_master[i].height = atoi(height);
					p_cabin->p_icon->device_master[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_master[i].slaveinputNum = atoi(slavenum);
					break;
				}
			}
		}
		CString detail;
		detail.Format("机柜名字 %s 替换设备IP%s",p_cabin->p_icon->cabientname,p_cabin->p_icon->ipMaster );
		writeUserLog(1,5,username,"替换设备",detail);

		onok=true;
	}

	else if(order==26)
	{

		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="管理员";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_slave[i].slaveinputNum == slaveputnum)
				{
					p_cabin->p_icon->device_slave[i].name = "";
					p_cabin->p_icon->device_slave[i].start = 0;
					p_cabin->p_icon->device_slave[i].height = 0;
					p_cabin->p_icon->device_slave[i].maininputNum = 0;
					p_cabin->p_icon->device_slave[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_slave[i].name.GetLength())
				{
					p_cabin->p_icon->device_slave[i].name = name;
					p_cabin->p_icon->device_slave[i].start =atoi(start);
					p_cabin->p_icon->device_slave[i].height = atoi(height);
					p_cabin->p_icon->device_slave[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_slave[i].slaveinputNum = atoi(slavenum);
					break;
				}
			}
		}
		CString detail;
		detail.Format("机柜名字 %s 替换设备IP%s",p_cabin->p_icon->cabientname ,p_cabin->p_icon->ipSlave);
		writeUserLog(1,5,username,"替换设备",detail);
		onok=true;
	}

	else if(order==27)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="管理员";
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		int sNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3

		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num4);//1
		Start=atoi(num1);
		Height=atoi(num2);

		sNum=atoi(num4);
		int s=0;

		POSITION pos1 = snmpobj.GetTailPosition ();
		while (pos1 != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos1)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				for(int j=0;j<24;j++)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;

				}
			}
		}


		int ips=s+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if( atoi(num4) > 0 && atoi(num4) < ips)
						{
							p_icon->device_slave[i].slaveinputNum= atoi(num4);
						}
						else
						{
							CString str;
							str.Format("副机端口取值 1-%d,请正确输入",s);
							AfxMessageBox(str);
							return;
						}
					}



					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备高度为 1-50,请正确输入");
						AfxMessageBox(str);
						return;
					}
					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==29)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="管理员";
		}
		/////////////
		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("请输入正确的机架位置");
		}
		return;
	}
	else if(order==30)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.deviceListWrite();
								//sure.NPMIVdevicewrite();

								onok=true;
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIII);
						CInitClever sure;
						sure.deviceListWrite();
						writeUserLog(1,5,username,"修改设备",username+" 修改设备,设备的IP为:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  位 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}
		return;
	}

	else if(order==31)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,1));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIV);
						CInitClever sure;
						sure.NPMIVdevicewrite();
						writeUserLog(1,5,username,"修改设备",username+" 修改设备,设备的 IP为:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  为 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}
		return;
	}

	else if(order==32)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString atsip;
			CString atscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(atsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(atscom);
			if(atsip.GetLength()!=0)
			{
				if(atscom=="public")
				{
					if(atsip.GetLength()  &&  atscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==atsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.atsemsListWrite();

							}
						}
						snmpobj.AddTail(new CSnmpObj(atsip,atscom,2));
						p_emsats->m_tree.InsertItem(atsip,3,3,p_emsats->ats);
						CInitClever sure;
						sure.atsemsListWrite();
						writeUserLog(1,5,username,"修改设备",username+"修改设备,设备的 IP为:"+atsip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community 为 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("密码不能为空");
			}
		}
		return;
	}
	
	else if(order==33||order==40)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.atsemsListWrite();

							}
						}
						if(order==33)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,1));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ems);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"修改设备",username+"修改设备,设备的 IP为:"+emsip);
							onok=true;
						}
						else if(order==40)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,3));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ippdu);
							CInitClever sure;
							sure.ippduListWrite();
							writeUserLog(1,5,username,"修改设备",username+"修改设备,设备的 IP为:"+emsip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get community 为 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("密码不能为空");
			}
		}
	}
	else if(order==34)
	{
		//AfxMessageBox("密码不能为空");
		CString ip_m;
		CString ip_s;
		CString cabinet_n;
		CString ipmaster_n;
		CString ipslave_n;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(cabinet_n);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(ip_m);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip_s);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(ipmaster_n);
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(ipslave_n);
		if(ip_m.GetLength())
		{
			if(atoi(ipmaster_n)>=0&&atoi(ipmaster_n)<10)
			{
				if(ip_s.GetLength())
				{
					if(atoi(ipslave_n)>=0&&atoi(ipslave_n)<10)
					{

					}
					else 
						AfxMessageBox("副机的取值为0-9，请正确填写。");
				}
			}
			else
				AfxMessageBox("副机的取值为0-9，请正确填写。");
		}
		
		int i;
		/////////复制机架
		for(i=0;i<24;i++)
		{
			if(p_icon->device_master[i].name.GetLength())
			{
				device_master[i].name=p_icon->device_master[i].name;
				device_master[i].maininputNum=p_icon->device_master[i].maininputNum;
				device_master[i].height=p_icon->device_master[i].height;
				device_master[i].start=p_icon->device_master[i].start;
			}
			if(p_icon->device_slave[i].name.GetLength())
			{
				device_slave[i].name=p_icon->device_slave[i].name;
				device_slave[i].slaveinputNum=p_icon->device_slave[i].slaveinputNum;
				device_slave[i].height=p_icon->device_slave[i].height;
				device_slave[i].start=p_icon->device_slave[i].start;
			}
		}
		//////////
		int datercenterNUM_i;
		for(i=0;i<10;i++)
		{
			if(DCname==DCmember[i].name && DCmember[i].name.GetLength())
			{	
				POSITION pos = DCmember[i].canbin.GetHeadPosition (),pos_old=NULL;
				CCanbinObjIcon * p_icon;
				do
				{
					pos_old = pos;
					p_icon = (CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
					if(p_icon->select)
					{
						delete p_icon;
						DCmember[i].canbin.RemoveAt(pos_old);
						datercenterNUM_i=i;
					}
				}
				while(pos != NULL);
			}
		}

		//////////新增机架
		CCanbinObjIcon* p_icon_2=NULL;
		p_icon_2 = new CCanbinObjIcon(1,1,0,ip_m,ip_s,atoi(ipmaster_n),atoi(ipslave_n),0);
		p_icon_2->cabientname=cabinet_n;
		p_icon_2->select=true;
		p_icon_2->move=true;
		DCmember[datercenterNUM_i].canbin.AddTail(p_icon_2);

		////////复制机架
		for(i=0;i<24;i++)
		{
			if(device_master[i].name.GetLength())
			{
				p_icon_2->device_master[i].name =device_master[i].name;
				p_icon_2->device_master[i].maininputNum =device_master[i].maininputNum;
				p_icon_2->device_master[i].height =device_master[i].height;
				p_icon_2->device_master[i].start =device_master[i].start;
			}
			if(device_slave[i].name.GetLength())
			{
				p_icon_2->device_slave[i].name =device_slave[i].name;
				p_icon_2->device_slave[i].slaveinputNum =device_slave[i].slaveinputNum;
				p_icon_2->device_slave[i].height =device_slave[i].height;
				p_icon_2->device_slave[i].start =device_slave[i].start;
			}
		}
		CString detail;
		detail.Format("在机房(%s)中，修改一条NPM,主要NPM IP:%s->%s,备用NPM IP:%s->%s",DCmember[datercenterNUM_i].name,ip_m,ipmaster_n,ip_s,ipslave_n);
		writeUserLog(1,3,username,"增加机柜",detail);
		
		onok=true;
	}
	
	else if(order==37)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							onok=true;
							return;
						}
						if(p_emsats->ippdu)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,3));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ippdu);
							CInitClever sure;
							sure.ippduListWrite();
							writeUserLog(1,5,username,"增加设备",username+"增加了一个新的设备，该设备IP:"+emsip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get community为public，请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}
		return;
	}


	else if(order==38)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,3));
						p_device->m_tree.InsertItem(ip,1,1,p_device->MSPDU);
						CInitClever sure;
						sure.MSPDUdevicewrite();
						writeUserLog(1,5,username,"修改设备",username+" 修改设备,设备的 IP为:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  为 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}
		return;
	}
	else if(order==39)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("用户账号和密码不匹配!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("该用户没有权限!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="管理员";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("设备IP已经存在，请重新填写。");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,2));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMV);
						CInitClever sure;
						sure.NPMVdevicewrite();
						writeUserLog(1,5,username,"修改设备",username+" 修改设备,设备的 IP为:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  为 public,请正确填写");
				}
			}
			else
			{
				AfxMessageBox("设备IP不能为空");
			}
		}
		return;
	}
	else if(order==50)//新增主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		CString num5;
		int mNum,sNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num5);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum=atoi(num3);
		sNum=atoi(num5);

		int m=0,s=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}

		pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;
				}
			}
		}

		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( atoi(num3) > 0 && atoi(num3) < ipm)
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("主端口取值在1-%d，请正确填写",m);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);
					//OnOK();
					break;
					onok=true;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if( atoi(num5) > 0 && atoi(num5) < s+1)
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num5);
						}
						else
						{
							CString str;
							str.Format("备用端口取值在1-%d，请正确填写",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}


					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==51)//删除主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("请填写正确的机架位置");
		}
		return;
	}
	else if(order==52)//新增主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}
		/////////////
		CString devicename;
		CString num1,num2,num3,num4, num5,num6,num7;
		int mNum1,sNum1,mNum2,sNum2,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num5);//2

		GetDlgItem(IDC_EDIT9)->GetWindowTextA(num6);//2
		GetDlgItem(IDC_EDIT8)->GetWindowTextA(num7);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum1=atoi(num3);
		sNum1=atoi(num5);

		mNum2=atoi(num6);
		sNum2=atoi(num7);


		int m=0,s=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}

		pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;
				}
			}
		}

		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( (atoi(num3) > 0 && atoi(num3) < ipm)&&((atoi(num6) > 0 && atoi(num6) < ipm)))
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("主端口取值在1-%d，请正确填写",m);
							AfxMessageBox(str);
							return;
						}
					}
					
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					
					if(p_icon->ipSlave.GetLength())
					{
						if( (atoi(num5) > 0 && atoi(num5) < s+1)&&((atoi(num7) > 0 && atoi(num7) < s+1)))
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num5);
						}
						else
						{
							CString str;
							str.Format("备用端口取值在1-%d，请正确填写",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if(((atoi(num6) > 0 && atoi(num6) < ipm)))
						{
							p_icon->device_master[i].maininputNum = atoi(num6);
						}
						else
						{
							CString str;
							str.Format("主端口取值在1-%d，请正确填写",m);
							AfxMessageBox(str);
							return;
						}
					}
					
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if(((atoi(num7) > 0 && atoi(num7) < s+1)))
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num7);
						}
						else
						{
							CString str;
							str.Format("备用端口取值在1-%d，请正确填写",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("设备位置取值在1-50，请正确填写");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			return;
		}
		return;
	}
	else if(order==53)//删除主机架
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("用户账号和密码不匹配!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("该用户没有权限!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("请先增加一个用户!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("请填写正确的机架位置");
		}
		return;
	}
}


void CMakeSureDlg::writeUserLog(int logtype,int type,CString name, CString item, CString detail)
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

int CMakeSureDlg::cheakUser(void)
{
	//CString username;
	CString userpsd;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(userpsd);
	int i;
	for(i=0;i<20;i++)
	{
		if(strcmp(user[i].name,username)==0 && strcmp(user[i].psd,userpsd)==0 &&
			username.GetLength() && userpsd.GetLength() && user[i].grade==1)
		{
			return 1;
		}
		else if(strcmp(user[i].name,username)==0 && strcmp(user[i].psd,userpsd)==0 &&
			username.GetLength() && userpsd.GetLength() && user[i].grade==0)
		{
			return 3;
		}
	}
	for(i=0;i<20;i++)
	{
		if(strlen(user[i].name))
		{
			return 0;
		}
	}
	return 2;
}

void CMakeSureDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (onok1==true)
	{
		CDialog::OnOK();
		onok1=false;
	}
	else
	{
		makesure();
		if(onok==true)
		{
			CDialog::OnOK();
			onok=false;
		}
	}
}


LRESULT CMakeSureDlg::OnNcHitTest(CPoint point)
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