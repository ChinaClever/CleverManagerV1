// ServerSet.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "ServerSet.h"
#include "InitClever.h"
#include "MakeSureDlg.h"
#include "Alarm.h"
#include "web.h"
#include "webbody.h"


// CServerSet 对话框
int webtreadflags;
HINSTANCE hlib;
int webport;
int webactivetime;

IMPLEMENT_DYNAMIC(CServerSet, CDialog)

CServerSet::CServerSet(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSet::IDD, pParent)
{

}

CServerSet::~CServerSet()
{
}

void CServerSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_LIST, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_mailCom);
	DDX_Control(pDX, IDC_COMBO2, m_webCom);
	DDX_Control(pDX, IDC_COMBO3, m_userEdit);
}


BEGIN_MESSAGE_MAP(CServerSet, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, &CServerSet::OnNMRclickUserList)
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, &CServerSet::OnNMClickUserList)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_USER_DETAIL, &CServerSet::OnEnChangeUserDetail)
END_MESSAGE_MAP()


// CServerSet 消息处理程序

BOOL CServerSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	rect.right += 1;
	rect.bottom += 1;
	////////////////创建一个图片的绘图环境///////////////////////
    CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	/////////////////////////////////
	SetBkMode(BkDc,TRANSPARENT);
	BkDc.SelectStockObject(NULL_PEN);
    CBrush brush(RGB(255,255,255));
	BkDc.SelectObject(&brush);
	BkDc.Rectangle(&rect);

	BkDc.SelectStockObject(BLACK_PEN);
	drawuserform(&BkDc);

	///////////////////////////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CServerSet::drawuserform(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(230,235,242));
	pDC->SelectObject(&brush);
	pDC->Rectangle(rect.left,rect.top,rect.Width()*30/100,rect.Height()*50/100-5);
	
	//pDC->MoveTo(rect.left,25);
	//pDC->LineTo(rect.Width()*30/100,25);
	
	drawFont(pDC,0,RGB(64,64,64));
	pDC->TextOut(20,rect.Height()*4/100+28,"SMTP账号:");
	pDC->TextOut(20,rect.Height()*10/100+28,"密码:");
	pDC->TextOut(20,rect.Height()*16/100+28,"SMTP服务器:");
	pDC->TextOut(20,rect.Height()*22/100+28,"端口:");
	pDC->TextOut(20,rect.Height()*28/100+28,"认证方式:");
	drawBtn(pDC,rect.Width()*4/100,rect.Height()*40/100,80,20,0,"测试");
	drawBtn(pDC,rect.Width()*18/100,rect.Height()*40/100,80,20,1,"保存");
	//////////////////////////////////////////////////////////////////////
	pDC->Rectangle(rect.left,rect.Height()*50/100,rect.Width()*30/100,rect.Height());
	//pDC->MoveTo(rect.left,rect.Height()*50/100+25);
	//pDC->LineTo(rect.Width()*30/100,rect.Height()*50/100+25);
	
	pDC->TextOut(20,rect.Height()*54/100+28,"WEB端口:");
	pDC->TextOut(20,rect.Height()*60/100+28,"活动时间:");
	pDC->TextOut(20,rect.Height()*66/100+28,"动作:");
	drawBtn(pDC,rect.Width()*18/100,rect.Height()*80/100,80,20,1,"保存");
	//////////////////////////////////////////////////////////////////////
	pDC->Rectangle(rect.Width()*30/100+5,rect.top,rect.Width(),rect.Height()*50/100-5);
	//pDC->MoveTo(rect.Width()*30/100+5,25);
	//pDC->LineTo(rect.Width(),25);
	
	
	//////////////////////////////////////////////////////////////////////
	pDC->Rectangle(rect.Width()*30/100+5,rect.Height()*50/100,rect.Width(),rect.Height());
	//pDC->MoveTo(rect.Width()*30/100+5,rect.Height()*50/100+25);
	//pDC->LineTo(rect.Width(),rect.Height()*50/100+25);
	
	pDC->TextOut(rect.Width()*35/100,rect.Height()*54/100+28,"账号名称:");
	pDC->TextOut(rect.Width()*65/100,rect.Height()*54/100+28,"账号密码:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*60/100+28,"用户电话:");
	pDC->TextOut(rect.Width()*65/100,rect.Height()*60/100+28,"E-mail地址:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*66/100+28,"用户权限:");
	pDC->TextOut(rect.Width()*35/100,rect.Height()*72/100+28,"用户信息:");
	drawBtn(pDC,rect.Width()*80/100,rect.Height()*90/100,80,20,1,"保存");

	//////////
	CBrush brush1(RGB(255,255,255));
	pDC->SelectObject(&brush1);
	pDC->Rectangle(rect.left,rect.top,rect.Width()*30/100,25);
	retange(pDC,rect.left+1,rect.top+1,rect.Width()*30/100-2,24);
	pDC->Rectangle(rect.left,rect.Height()*50/100,rect.Width()*30/100,rect.Height()*50/100+25);
	retange(pDC,rect.left+1,rect.Height()*50/100+1,rect.Width()*30/100-2,24);
	pDC->Rectangle(rect.Width()*30/100+5,rect.top,rect.Width(),25);
	retange(pDC,rect.Width()*30/100+6,rect.top+1,rect.Width()*70/100-7,24);
	pDC->Rectangle(rect.Width()*30/100+5,rect.Height()*50/100,rect.Width(),rect.Height()*50/100+25);
	retange(pDC,rect.Width()*30/100+6,rect.Height()*50/100+1,rect.Width()*70/100-7,24);
	drawFont(pDC,1,RGB(0,0,0));
	pDC->TextOut(15,6,"SMTP服务设置");
	pDC->TextOut(15,rect.Height()*50/100+6,"WEB服务设置");
	pDC->TextOut(rect.Width()*30/100+15,6,"用户信息列表");
	pDC->TextOut(rect.Width()*30/100+15,rect.Height()*50/100+6,"编辑用户信息");
}

void CServerSet::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(IsWindow(m_list.m_hWnd))
	{	
		GetDlgItem(IDC_SMTP_USER)->MoveWindow(100,rect.Height()*4/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_SMTP_PSD)->MoveWindow(100,rect.Height()*10/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_SMTP_SERVER)->MoveWindow(100,rect.Height()*16/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_SMTP_PORT)->MoveWindow(100,rect.Height()*22/100+25,60,18);
		m_mailCom.MoveWindow(100,rect.Height()*28/100+25,100,18);
		///////////////////
		GetDlgItem(IDC_WEB_PORT)->MoveWindow(100,rect.Height()*54/100+25,100,18);
		GetDlgItem(IDC_WEB_TIME)->MoveWindow(100,rect.Height()*60/100+25,100,18);
		m_webCom.MoveWindow(100,rect.Height()*66/100+25,100,18);
		////////////////
		m_list.MoveWindow(rect.Width()*30/100+6,26,rect.Width()*70/100-6,rect.Height()*50/100-32);
		///////////////////
		GetDlgItem(IDC_USER_COUNT)->MoveWindow(rect.Width()*45/100,rect.Height()*54/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_USER_PSD)->MoveWindow(rect.Width()*75/100,rect.Height()*54/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_USER_TEL)->MoveWindow(rect.Width()*45/100,rect.Height()*60/100+25,rect.Width()*15/100,18);
		GetDlgItem(IDC_USER_MAIL)->MoveWindow(rect.Width()*75/100,rect.Height()*60/100+25,rect.Width()*15/100,18);
		m_userEdit.MoveWindow(rect.Width()*45/100,rect.Height()*66/100+25,rect.Width()*10/100,18);
		GetDlgItem(IDC_USER_DETAIL)->MoveWindow(rect.Width()*45/100,rect.Height()*72/100+25,rect.Width()*25/100,60);
		m_list.SetColumnWidth(1,(rect.Width()*70/100-30)/4);
		m_list.SetColumnWidth(2,(rect.Width()*70/100-30)/4);
		m_list.SetColumnWidth(3,(rect.Width()*70/100-30)/4);
		m_list.SetColumnWidth(4,(rect.Width()*70/100-30)/4);
	}
}

void CServerSet::drawBtn(CDC* pDC,int x,int y,int w,int h,int icon,CString name)
{
	CBrush brush1(RGB(240,240,240)),*p_old;
	p_old = pDC->SelectObject(&brush1);
	pDC->Rectangle(x,y,x+w,y+h);
	pDC->SelectObject(p_old);
	if(icon==0)
	{
		drawImagePNG(pDC,getFilePath()+"\\icon\\open.png",x+8,y+4,12,12);
	}
	else if(icon==1)
	{
		drawImagePNG(pDC,getFilePath()+"\\icon\\save.png",x+8,y+4,12,12);
	}
	pDC->TextOut(x+30,y+4,name);
}

BOOL CServerSet::OnInitDialog()
{
	int i;
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list.ModifyStyle(LVS_LIST|LVS_ICON|LVS_SMALLICON ,   //LVS_ICON 
		LVS_REPORT |LVS_SHOWSELALWAYS|LVS_AUTOARRANGE); //LVS_REPORT|
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES );
	m_list.SetTextColor(RGB(64,64,64));

	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 1, 1);
	m_cImageListNormal32.Attach(hList);
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON1)); 
	m_list.SetImageList(&m_cImageListNormal32, LVSIL_SMALL);
	m_list.InsertColumn(0,"",LVCFMT_CENTER,20,-1);
	m_list.InsertColumn(0,"",LVCFMT_CENTER,20,-1);
	m_list.InsertColumn(0,"",LVCFMT_CENTER,20,-1);
	m_list.InsertColumn(0,"",LVCFMT_CENTER,20,-1);
	m_list.InsertColumn(0,"",LVCFMT_CENTER,20,-1);

	for(i=0;i<20;i++)
	{
		if(strlen(user[i].name))
		{
			m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,0,0);
			m_list.SetItemText(0,1,user[i].name);
			if(user[i].grade == 1)
			{
				m_list.SetItemText(0,2,"权限:可以编辑");
			}
			else
			{
				m_list.SetItemText(0,2,"权限:不可编辑");
			}
			CString listtel;
			listtel.Format("电话:%s",user[i].tel);
			m_list.SetItemText(0,3,listtel);
			m_list.SetItemText(0,4,user[i].mail);
		}
	}

	m_userEdit.ResetContent();
	m_userEdit.AddString("可以编辑");
	m_userEdit.AddString("不可编辑");
	m_mailCom.ResetContent();
	m_mailCom.AddString("NONE");
	m_mailCom.AddString("LOGIN");
	m_mailCom.AddString("SSL");
	m_webCom.ResetContent();
	m_webCom.AddString("启用");
	m_webCom.AddString("禁用");
/////////////////////////////////////////////
	CFile file;
	file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char value[128];
	memset(value,0,sizeof(char)*128);
	file.Read(value,128);
	GetDlgItem(IDC_SMTP_USER)->SetWindowTextA(value);
	file.Seek(128,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,128);
	GetDlgItem(IDC_SMTP_PSD)->SetWindowTextA(value);
	file.Seek(128*2,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,128);
	GetDlgItem(IDC_SMTP_SERVER)->SetWindowTextA(value);
	file.Seek(128*3,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,128);
	GetDlgItem(IDC_SMTP_PORT)->SetWindowTextA(value);
	file.Seek(128*4,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,128);
	m_mailCom.SetWindowTextA(value);
	file.Close();
    //////////////////////////
	file.Open(getFilePath()+"\\Uinfo\\web",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	memset(value,0,sizeof(char)*128);
	file.Read(value,30);
	webport = atoi(value);
	GetDlgItem(IDC_WEB_PORT)->SetWindowTextA(value);
	file.Seek(32,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,30);
	webactivetime = atoi(value);
	GetDlgItem(IDC_WEB_TIME)->SetWindowTextA(value);
	file.Seek(32*2,CFile::begin);
	memset(value,0,sizeof(char)*128);
	file.Read(value,30);
	m_webCom.SetWindowTextA(value);
	CString webstatus = value;
	file.Close();
	////////////
	webtreadflags = 0;
	if(webport>65535 || webport<0 || webactivetime<0)
	{
		return TRUE;
	}
	if(webstatus == "启用")
	{
		AfxBeginThread(webstart,NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CServerSet::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i;
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.Width()*80/100 && point.y > rect.Height()*90/100 
		&& point.x<rect.Width()*80/100+80 && point.y < rect.Height()*90/100+20)
	{
		CMakeSureDlg sure;
		sure.p_list = this;
		sure.setOrder(8);
		sure.DoModal();
	}
	else if(point.x>rect.Width()*4/100 && point.y > rect.Height()*40/100 
		&& point.x<rect.Width()*4/100+80 && point.y < rect.Height()*40/100+20)
	{
		CAlarm mail;
		char smtpuser[128];
		char psd[128];
		char smtpserver[128];
		char snedto[1024];
		int port = 0;
		int type = 0;
		memset(smtpuser,0,sizeof(char)*128);
		memset(psd,0,sizeof(char)*128);
		memset(smtpserver,0,sizeof(char)*128);
		memset(snedto,0,sizeof(char)*1024);
		CString value;
		GetDlgItem(IDC_SMTP_USER)->GetWindowTextA(value);
		strncpy_s(smtpuser,sizeof(char)*128,value,sizeof(char)*126);
		value.Empty();
		GetDlgItem(IDC_SMTP_PSD)->GetWindowTextA(value);
		strncpy_s(psd,sizeof(char)*128,value,sizeof(char)*126);
		value.Empty();
		GetDlgItem(IDC_SMTP_SERVER)->GetWindowTextA(value);
		strncpy_s(smtpserver,sizeof(char)*128,value,sizeof(char)*30);
		value.Empty();
		GetDlgItem(IDC_SMTP_PORT)->GetWindowTextA(value);
		port = atoi(value);
		value.Empty();
		m_mailCom.GetWindowTextA(value);
		if(value=="LOGIN")
			type = 1;
		else if(value=="SSL")
			type = 2;
		else
			type = 0;
		for(i=0;i<20;i++)
		{
			if(strlen(user[i].name)>0 && strlen(user[i].mail)>0)
			{
				strcat_s(snedto,sizeof(char)*1022,user[i].mail);
				strcat_s(snedto,sizeof(char)*1022,";");
			}
		}
		if(strlen(snedto))
		{
			mail.sendmail(smtpuser,psd,snedto,
				"CLEVER Manager test information","CLEVER Manager test information.",smtpserver,port,type);
			MessageBox("The test e-mail has been sent, please check。");
		}
		else 
		{
			MessageBox("Please add a user first.");
		}
	}
	else if(point.x>rect.Width()*18/100 && point.y > rect.Height()*40/100 
		&& point.x<rect.Width()*18/100+80 && point.y < rect.Height()*40/100+20)
	{
		CMakeSureDlg sure;
		sure.p_list = this;
		sure.setOrder(11);
		sure.DoModal();
	}
	else if(point.x>rect.Width()*18/100 && point.y > rect.Height()*80/100 
		&& point.x<rect.Width()*18/100+80 && point.y < rect.Height()*80/100+20)
	{
		CMakeSureDlg sure;
		sure.p_list = this;
		sure.setOrder(12);
		sure.DoModal();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CServerSet::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.Width()*80/100 && point.y > rect.Height()*90/100 
		&& point.x<rect.Width()*80/100+80 && point.y < rect.Height()*90/100+20)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>rect.Width()*4/100 && point.y > rect.Height()*40/100 
		&& point.x<rect.Width()*4/100+80 && point.y < rect.Height()*40/100+20)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>rect.Width()*18/100 && point.y > rect.Height()*40/100 
		&& point.x<rect.Width()*18/100+80 && point.y < rect.Height()*40/100+20)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>rect.Width()*18/100 && point.y > rect.Height()*80/100 
		&& point.x<rect.Width()*18/100+80 && point.y < rect.Height()*80/100+20)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CServerSet::OnNMRclickUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_list.GetSelectionMark();
	if(nIndex>=0)
	{
		CString listvalue = m_list.GetItemText(nIndex,1);
		CMakeSureDlg sure;
		sure.p_list = this;
		sure.setOrder(7);
		sure.DoModal();
		return;
	}
	///////////////////////////////////////////////////
	*pResult = 0;
}

void CServerSet::OnNMClickUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int nIndex = m_list.GetSelectionMark();
	if(nIndex>=0)
	{
		CString name = m_list.GetItemText(nIndex,1);
		for(i=0;i<20;i++)
		{
			if(name == user[i].name)
			{
				GetDlgItem(IDC_USER_COUNT)->SetWindowTextA(user[i].name);
				GetDlgItem(IDC_USER_PSD)->SetWindowTextA(user[i].psd);
				GetDlgItem(IDC_USER_TEL)->SetWindowTextA(user[i].tel);
				GetDlgItem(IDC_USER_MAIL)->SetWindowTextA(user[i].mail);
				if(user[i].grade == 1)
				{
					m_userEdit.SetWindowTextA("可以编辑");
				}
				else
				{
					m_userEdit.SetWindowTextA("不可编辑");
				}
				GetDlgItem(IDC_USER_DETAIL)->SetWindowTextA(user[i].detail);
				return;
			}
		}	
	}
	else
	{
		GetDlgItem(IDC_USER_COUNT)->SetWindowTextA("");
		GetDlgItem(IDC_USER_PSD)->SetWindowTextA("");
		GetDlgItem(IDC_USER_TEL)->SetWindowTextA("");
		GetDlgItem(IDC_USER_MAIL)->SetWindowTextA("");
		GetDlgItem(IDC_USER_DETAIL)->SetWindowTextA("");
	}
	*pResult = 0;
}
///////////////////////////////////////////web/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
CString loginsession(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
    for(i=0;i<20;i++)
	{
		if(zd[1]==user[i].name && zd[2] == user[i].psd && zd[1].GetLength() && zd[2].GetLength())
		{
			body = MAINBODY;
			return body;
		}
	}
    body = "Login false. Please confirm user and password.";
	return body;
}
bool Handlelogin(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/login",6)==0)
	{
		body = loginsession(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CString tuopu(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	int j = 0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	CString dc;
	int dcindex=-1;
	if(zd[1].GetLength() && zd[2].GetLength())
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name == zd[1])
			{
				if(zd[2] == "pre")
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i-j-1<0?i-j+9:i-j-1].name.GetLength())
						{
							dcindex = i-j-1<0?i-j+9:i-j-1;
							break;
						}
					}
				}
				else
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i+j+1>9?i+j-9:i+j+1].name.GetLength())
						{
							dcindex = i+j+1>9?i+j-9:i+j+1;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength())
			{
				dcindex=i;
				break;
			}
		}
	}
	if(dcindex>=0 && dcindex<10 && DCmember[dcindex].name.GetLength())
	{
		body += TUOPUHEAD;
		CString tabbody;
		tabbody.Format(TUOPUTITLE,DCmember[dcindex].name,DCmember[dcindex].name,DCmember[dcindex].name);
		body += tabbody;
		body += TUOPUBODY;

		POSITION pos = DCmember[dcindex].canbin.GetTailPosition ();
		while (pos != NULL)
		{
			CCanbinObjIcon * picon;
			picon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
			tabbody.Empty();
			if(picon->type==0)
			{
				if(picon->status ==2)
				{
					tabbody.Format(TUOPUCABIN,picon->x,picon->y,picon->x+1,picon->y+1,"ffaaaa",(picon->x)*45,(picon->y)*30);
				}
				else if(picon->status ==1)
				{
					tabbody.Format(TUOPUCABIN,picon->x,picon->y,picon->x+1,picon->y+1,"aaffaa",(picon->x)*45,(picon->y)*30);
				}
				else if(picon->status ==3)
				{
					tabbody.Format(TUOPUCABIN,picon->x,picon->y,picon->x+1,picon->y+1,"ffffaa",(picon->x)*45,(picon->y)*30);
				}
				else
				{
					tabbody.Format(TUOPUCABIN,picon->x,picon->y,picon->x+1,picon->y+1,"ffffff",(picon->x)*45,(picon->y)*30);
				}
			}
			body += tabbody;
		}

		body += TUOPUEND;
	}
	return body;
}
bool Handletuopu(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/tuopu.html",10)==0)
	{
		body = tuopu(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////
CString cabin(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[4];	
	int m = 0;
	int i = 0;
	int j=0;
	while(m<4)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength() && DCmember[i].name==zd[1])
		{
			POSITION pos = DCmember[i].canbin.GetTailPosition ();
			while (pos != NULL)
			{
				CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
				if(p_icon->x == atoi(zd[2]) && p_icon->y == atoi(zd[3]))
				{
					body = CABINHEAD;
					POSITION posnow = snmpobj.GetTailPosition ();
					CSnmpObj *p_master = NULL;
					CSnmpObj *p_slave = NULL;
					while (posnow != NULL)
					{
						CSnmpObj *p_Objectnow = NULL;
						p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
						if(p_Objectnow->ipaddr == p_icon->ipMaster && p_Objectnow->dtype==0)
						{
							p_master = p_Objectnow;
						}
						if(p_Objectnow->ipaddr == p_icon->ipSlave && p_Objectnow->dtype==0)
						{
							p_slave = p_Objectnow;
						}
					}
					CString tabbody;
					if(p_master)
					{
						tabbody.Empty();
						CString ip;
						ip.Format("%s->%d",p_icon->ipMaster,p_icon->MasterNUM);
						if(p_master->status==2)
							tabbody.Format(CABINBODY,"主要PDU",ip,"ff8080","报警");
						else if(p_master->status==1)
							tabbody.Format(CABINBODY,"主要PDU",ip,"00aa00","正常");
						else
							tabbody.Format(CABINBODY,"主要PDU",ip,"aaaa00","离线");
						body += tabbody;
						for(j=0;j<24;j++)
						{
							if(p_master->outputname[p_icon->MasterNUM][j].GetLength())
							{
								body += ",";
								tabbody.Empty();
								CString value;
								if("C"==p_master->devicetype[p_icon->MasterNUM].Right(1)||"A"==p_master->devicetype[p_icon->MasterNUM].Right(1))
								{
									value.Format("--    (最小值:--  -  最大值:--)"
										,p_master->curout[p_icon->MasterNUM][j]
									,p_master->curmin[p_icon->MasterNUM][j]
									,p_master->curmax[p_icon->MasterNUM][j]);
									tabbody.Format(CABINBODY,p_master->outputname[p_icon->MasterNUM][j],p_master->outputstatus[p_icon->MasterNUM][j]
									,(p_master->curout[p_icon->MasterNUM][j]>p_master->curmax[p_icon->MasterNUM][j] 
									||p_master->curout[p_icon->MasterNUM][j]<p_master->curmin[p_icon->MasterNUM][j])?"ff8080":"00aa00",value);
									body += tabbody;
								}
								else
								{
									value.Format("%.1fA    (最小值:%.1f-最大值:%.1f)"
										,p_master->curout[p_icon->MasterNUM][j]
									,p_master->curmin[p_icon->MasterNUM][j]
									,p_master->curmax[p_icon->MasterNUM][j]);
									tabbody.Format(CABINBODY,p_master->outputname[p_icon->MasterNUM][j],p_master->outputstatus[p_icon->MasterNUM][j]
									,(p_master->curout[p_icon->MasterNUM][j]>p_master->curmax[p_icon->MasterNUM][j] 
									||p_master->curout[p_icon->MasterNUM][j]<p_master->curmin[p_icon->MasterNUM][j])?"ff8080":"00aa00",value);
									body += tabbody;
								}
							}
						}
					}
					if(p_slave)
					{
						if(p_master)
						{
							body += ",";
						}
						tabbody.Empty();
						CString ip;
						ip.Format("%s->%d",p_icon->ipSlave,p_icon->SlaveNUM);
						if(p_slave->status==2)
							tabbody.Format(CABINBODY,"备用PDU",ip,"ff8080","Alarm");
						else if(p_slave->status==1)
							tabbody.Format(CABINBODY,"备用PDU",ip,"00aa00","Normal");
						else
							tabbody.Format(CABINBODY,"备用PDU",ip,"aaaa00","Offline");
						body += tabbody;
						for(j=0;j<24;j++)
						{
							if(p_slave->outputname[p_icon->SlaveNUM][j].GetLength())
							{
								body += ",";
								tabbody.Empty();
								CString value;
								
								if("C"==p_slave->devicetype[p_icon->SlaveNUM].Right(1)||"A"==p_slave->devicetype[p_icon->SlaveNUM].Right(1))
								{
									value.Format("--    (最小值:--  -  最大值:--)"
										,p_slave->curout[p_icon->SlaveNUM][j]
									,p_slave->curmin[p_icon->SlaveNUM][j]
									,p_slave->curmax[p_icon->SlaveNUM][j]);
									tabbody.Format(CABINBODY,p_slave->outputname[p_icon->SlaveNUM][j],p_slave->outputstatus[p_icon->SlaveNUM][j]
									,(p_slave->curout[p_icon->SlaveNUM][j]>p_slave->curmax[p_icon->SlaveNUM][j] 
									||p_slave->curout[p_icon->SlaveNUM][j]<p_slave->curmin[p_icon->SlaveNUM][j])?"ff8080":"00aa00",value);
									body += tabbody;
								}
								else
								{
									value.Format("%.1fA    (最小值:%.1f-最大值:%.1f)"
										,p_slave->curout[p_icon->SlaveNUM][j]
									,p_slave->curmin[p_icon->SlaveNUM][j]
									,p_slave->curmax[p_icon->SlaveNUM][j]);
									tabbody.Format(CABINBODY,p_slave->outputname[p_icon->SlaveNUM][j],p_slave->outputstatus[p_icon->SlaveNUM][j]
									,(p_slave->curout[p_icon->SlaveNUM][j]>p_slave->curmax[p_icon->SlaveNUM][j] 
									||p_slave->curout[p_icon->SlaveNUM][j]<p_slave->curmin[p_icon->SlaveNUM][j])?"ff8080":"00aa00",value);
									body += tabbody;
								}
							}
						}
					}
					body += CABINEND;
					return body;
				}
			}
		}
	}
	return body;
}
bool Handlecabin(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/cabin.html",10)==0)
	{
		body = cabin(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////

CString status(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	int j=0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}

	CString dc;
	int dcindex=-1;
	if(zd[1].GetLength() && zd[2].GetLength())
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name == zd[1])
			{
				if(zd[2] == "pre")
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i-j-1<0?i-j+9:i-j-1].name.GetLength())
						{
							dcindex = i-j-1<0?i-j+9:i-j-1;
							break;
						}
					}
				}
				else
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i+j+1>9?i+j-9:i+j+1].name.GetLength())
						{
							dcindex = i+j+1>9?i+j-9:i+j+1;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength())
			{
				dcindex=i;
				break;
			}
		}
	}
	body +="?";
	body +=DCmember[dcindex].name;
	SYSTEMTIME ct;
	GetLocalTime(&ct);
#ifndef DEMO
	for(i=0;i<24;i++)
	{
		CString value;
		value.Format("?%d?%.2f",(ct.wHour+i>23)?ct.wHour+i-24:ct.wHour+i,DCmember[dcindex].kva[(ct.wHour+i>23)?ct.wHour+i-24:ct.wHour+i]);
		body +=value;
	}
#else
	for(i=0;i<24;i++)
	{
		CString value;
		float a = (float)((i%5)+30)/10;
		value.Format("?%d?%.2f",(ct.wHour+i>23)?ct.wHour+i-24:ct.wHour+i, a);
		body +=value;
	}
#endif
	CString other;
	other.Format("?%d?%d?%d?%.1f?%.1f?%.1f?%d?%d?%d?%d?%d?",DCmember[dcindex].vol1
		,DCmember[dcindex].vol2
		,DCmember[dcindex].vol3
		,DCmember[dcindex].cur1
		,DCmember[dcindex].cur2
		,DCmember[dcindex].cur3
		,DCmember[dcindex].tp
		,DCmember[dcindex].hm
		,DCmember[dcindex].offnum
		,DCmember[dcindex].alarmnum
		,DCmember[dcindex].normalnum);
	body +=other;
	////////////////////////////
	/*CString   str; 
	str = body;
	int   nLen   =   MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   NULL,0); 
	WCHAR   *wszUTF   =   new   WCHAR[nLen+1]; 
	MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   wszUTF,   nLen); 
	nLen   =   WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   NULL,   0,   NULL,   NULL); 
	TCHAR   *szUTF   =   new   TCHAR[nLen+1]; 
	WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   szUTF,   nLen,   NULL,NULL); 
	
	CString  wecrec(szUTF);
	delete[]   szUTF; 
	delete[]   wszUTF;*/
	
	////////
	return changeToUtf8(body);
}
bool Handlestatus(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/status",7)==0)
	{
		body = status(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////
CString env(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	int j=0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}

	CString dc;
	int dcindex=-1;
	if(zd[1].GetLength() && zd[2].GetLength())
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name == zd[1])
			{
				if(zd[2] == "pre")
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i-j-1<0?i-j+9:i-j-1].name.GetLength())
						{
							dcindex = i-j-1<0?i-j+9:i-j-1;
							break;
						}
					}
				}
				else
				{
					for(j=0;j<10;j++)
					{
						if(DCmember[i+j+1>9?i+j-9:i+j+1].name.GetLength())
						{
							dcindex = i+j+1>9?i+j-9:i+j+1;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength())
			{
				dcindex=i;
				break;
			}
		}
	}
	if(dcindex>=0 && dcindex<10)
	{
		body += ENVHEAD;
		CString tabbody;
		tabbody.Format(ENVTITLE,DCmember[dcindex].name,DCmember[dcindex].name,DCmember[dcindex].name);
		body += tabbody;
		body += ENVBODY;

		POSITION pos = DCmember[dcindex].canbin.GetTailPosition ();
		while (pos != NULL)
		{
			CCanbinObjIcon* p_icon = (CCanbinObjIcon*)(DCmember[dcindex].canbin.GetPrev (pos));
			if(p_icon->type==0)
			{
				tabbody.Empty();
				if(p_icon->temp>35)
					tabbody.Format(ENVICON,p_icon->temp,"ffbbbb",p_icon->x*45,p_icon->y*30);
				else if(p_icon->temp>30)
					tabbody.Format(ENVICON,p_icon->temp,"ffffbb",p_icon->x*45,p_icon->y*30);
				else if(p_icon->temp>25)
					tabbody.Format(ENVICON,p_icon->temp,"bbffbb",p_icon->x*45,p_icon->y*30);
				else if(p_icon->temp==0)
					tabbody.Format(ENVICON,p_icon->temp,"ffffff",p_icon->x*45,p_icon->y*30);
				else
					tabbody.Format(ENVICON,p_icon->temp,"bbbbff",p_icon->x*45,p_icon->y*30);
				body += tabbody;
			}
		}
		body += ENVEND;

	}
	return body;
}
bool Handleenv(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/env.html",8)==0)
	{
		body = env(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////
CString list(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	int j=0;
	int k=0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[1]=="normal")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength() && p_Object->status==1 && p_Object->dtype==0)
			{
				if(j)
				{
					body +=",";
				}
				body +="{";

				CString tab;
				if(p_Object->npmtype==1)
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				else
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				tab.Format("\"text\":\"%s\",\"children\":[",p_Object->ipaddr);
				body +=tab;
				k=0;
				for(i=0;i<10;i++)
				{
					if(p_Object->slavename[i].GetLength())
					{
						if(k)
						{
							body +=",";
						}
						tab.Empty();
						tab.Format("{\"text\":\"%s\"}",p_Object->slavename[i]);
						body +=tab;
						k++;
					}
				}
				body +="]";
				body +="}";
				body +="]";
				body +="}";
				j++;
			}
		}
		body +="]";
	}
	if(zd[1]=="alarm")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength() && p_Object->status==2 && p_Object->dtype==0)
			{
				if(j)
				{
					body +=",";
				}
				body +="{";

				CString tab;
				if(p_Object->npmtype==1)
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				else
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				tab.Format("\"text\":\"%s\",\"children\":[",p_Object->ipaddr);
				body +=tab;
				k=0;
				for(i=0;i<10;i++)
				{
					if(p_Object->slavename[i].GetLength())
					{
						if(k)
						{
							body +=",";
						}
						tab.Empty();
						tab.Format("{\"text\":\"%s\"}",p_Object->slavename[i]);
						body +=tab;
						k++;
					}
				}
				body +="]";
				body +="}";
				body +="]";
				body +="}";
				j++;
			}
		}
		body +="]";
	}
	else if(zd[1]=="offline")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength() && p_Object->status==0  && p_Object->dtype==0)
			{
				if(j)
				{
					body +=",";
				}
				body +="{";
				CString tab;
				if(p_Object->npmtype==1)
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				else
				{
					tab.Format("\"text\":\"%s\",\"children\":[{","NPM-V");
					body +=tab;
				}
				tab.Format("\"text\":\"%s\"",p_Object->ipaddr);
				body +=tab;
				body +="}";
				body +="]";
				body +="}";
				j++;
			}
		}
		body +="]";
	}
	else if(zd[1]=="logslist")
	{
		class list
		{
		public:
			int title;
			int daytitle[32];
		};
		CList<list,list&> loglist;

		CFileFind ff;
		BOOL res = ff.FindFile(getFilePath()+"\\logs\\*.*");
		while(res)
		{
			res = ff.FindNextFile();
			if(ff.IsDirectory() && !ff.IsDots())
			{
				int year;
				int mon;
				int day;
				CString strTitle = ff.GetFileTitle();
				year = atoi(strTitle.Left(4));
				mon = atoi(strTitle.Mid(4,2));
				day = atoi(strTitle.Right(2));

				////////////////////////////////////
				int flags = false;
				POSITION pos=loglist.GetHeadPosition();
				while(pos!= NULL)
				{
					list *m_list;
					m_list=&(loglist.GetNext(pos));
					if(m_list->title == year*100+mon)
					{
						if(day>0 && day<32)
						{
							CString path = ff.GetFilePath();
							if(makesureFile(path+"\\users") && makesureFile(path+"\\alarm"))
								m_list->daytitle[day-1]=3;
							else if(makesureFile(path+"\\users"))
								m_list->daytitle[day-1]=1;
							else if(makesureFile(path+"\\alarm"))
								m_list->daytitle[day-1]=2;
						}
						flags = true;
					}
				}
				if(!flags)
				{
					list m_list;
					m_list.title = year*100+mon;
					for(i=0;i<32;i++)
					{
						m_list.daytitle[i]=0;
					}
					if(day>0 && day<31)
					{
						CString path = ff.GetFilePath();
						if(makesureFile(path+"\\users") && makesureFile(path+"\\alarm"))
							m_list.daytitle[day-1]=3;
						else if(makesureFile(path+"\\users"))
							m_list.daytitle[day-1]=1;
						else if(makesureFile(path+"\\alarm"))
							m_list.daytitle[day-1]=2;
					}
					loglist.AddTail(m_list);
				}
			}
		}
		ff.Close();
		//body = "[{\"id\":1,\"text\":\"Folder1\",\"children\":[{\"id\":1,\"text\":\"File1\"}] }]";
		body +="[";
		j=0;
		POSITION posnow = loglist.GetHeadPosition();
		while (posnow != NULL)
		{
			list m_list;
			m_list = loglist.GetNext(posnow);
			CString title1;
			title1.Format("Year:%d Month:%d",m_list.title/100,m_list.title%100);
			
			if(j)
			{
				body +=",";
			}
			body +="{";
			CString tab;
			tab.Format("\"text\":\"%s\"",title1);
			body +=tab;
			//////////////////////////
			body +=",\"children\":[";
			int k = 0;
			for(i=0;i<31;i++)
			{
				if(zd[2]=="1")
				{
					if(m_list.daytitle[i]==3 || m_list.daytitle[i]==1)
					{
						if(k)
						{
							body +=",{";
						}
						else
						{
							body +="{";
						}

						CString title2;
						title2.Format("%d/%d/%d",m_list.title%100,i+1,m_list.title/100);
						body +="\"text\":\"";
						body +=title2;

						body +="\"}";
						k++;
					}
				}
				else if(zd[2]=="2")
				{
					if(m_list.daytitle[i]==3 || m_list.daytitle[i]==2)
					{
						if(k)
						{
							body +=",{";
						}
						else
						{
							body +="{";
						}

						CString title2;
						title2.Format("%d/%d/%d",m_list.title%100,i+1,m_list.title/100);
						body +="\"text\":\"";
						body +=title2;

						body +="\"}";
						k++;
					}
				}
				
			}
			body +="]";
			///////////////////////////
			body +="}";
			j++;
			
		}
		body +="]";

	}
	else if(zd[1].GetLength() && zd[2].GetLength())
	{
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength() && p_Object->ipaddr==zd[1]  && p_Object->dtype==0)
			{
				for(i=0;i<10;i++)
				{
					if(p_Object->slavename[i]==zd[2] && zd[2].GetLength())
					{
						CString output;
						CString tab;
						k=0;
						for(j=0;j<24;j++)
						{
							if(p_Object->outputname[i][j].GetLength())
							{
								if(k)
								{
									output += ",";
								}
								if(p_Object->devicetype[i].Right(1)=="C"||p_Object->devicetype[i].Right(1)=="A")
								{
									tab.Empty();
									tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"<span style='color:%s'>%s</span>\",\
											   \"id4\":\"<span style='color:%s'>--</span>\",\"id5\":\"--\",\"id6\":\"--\",\"id7\":\"--\"}"
											   ,k+1,p_Object->outputname[i][j],
											   p_Object->outputstatus[i][j]=="ON"?"00aa00":"000000",p_Object->outputstatus[i][j],
											   (p_Object->curout[i][j]<p_Object->curmin[i][j] || p_Object->curout[i][j]>p_Object->curmax[i][j])?"aa0000":"00aa00",
											   p_Object->curout[i][j],
											   p_Object->curmin[i][j],p_Object->curmax[i][j],p_Object->curout[i][j]*p_Object->Tvol[i][0]);
									output += tab;
									k++;
								}
								else
								{
									tab.Empty();
									tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"<span style='color:%s'>%s</span>\",\
											   \"id4\":\"<span style='color:%s'>%.1f A</span>\",\"id5\":\"%.1f A\",\"id6\":\"%.1f A\",\"id7\":\"%.2f VA\"}"
											   ,k+1,p_Object->outputname[i][j],
											   p_Object->outputstatus[i][j]=="ON"?"00aa00":"000000",p_Object->outputstatus[i][j],
											   (p_Object->curout[i][j]<p_Object->curmin[i][j] || p_Object->curout[i][j]>p_Object->curmax[i][j])?"aa0000":"00aa00",
											   p_Object->curout[i][j],
											   p_Object->curmin[i][j],p_Object->curmax[i][j],p_Object->curout[i][j]*p_Object->Tvol[i][0]);
									output += tab;
									k++;
								}
							}
						}
						if(k)
							output += ",";
						tab.Empty();
						tab.Format("{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d V\",\"id4\":\"%s\",\"id5\":\"%.1f A\",\"id6\":\"\",\"id7\":\"\"}"
							,"L1","Line 1 votage",p_Object->Tvol[i][0],"Line 1 current",p_Object->Tcur[i][0]);
						output +=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d V\",\"id4\":\"%s\",\"id5\":\"%.1f A\",\"id6\":\"\",\"id7\":\"\"}"
							,"L2","Line 2 votage",p_Object->Tvol[i][1],"Line 2 current",p_Object->Tcur[i][1]);
						output +=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d V\",\"id4\":\"%s\",\"id5\":\"%.1f A\",\"id6\":\"\",\"id7\":\"\"}"
							,"L3","Line 2 votage",p_Object->Tvol[i][2],"Line 3 current",p_Object->Tcur[i][2]);
						output +=tab;

						tab.Empty();
						tab.Format(",{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d%s\",\"id4\":\"%s\",\"id5\":\"%d%s\",\"id6\":\"\",\"id7\":\"\"}"
							,"TH1","Temperature 1",p_Object->TH[i][0],"℃","Humidity 1",p_Object->TH[i][3],"%");
						output +=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d%s\",\"id4\":\"%s\",\"id5\":\"%d%s\",\"id6\":\"\",\"id7\":\"\"}"
							,"TH2","Temperature 2",p_Object->TH[i][1],"℃","Humidity 2",p_Object->TH[i][4],"%");
						output +=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%s\",\"id2\":\"%s\",\"id3\":\"%d%s\",\"id4\":\"%s\",\"id5\":\"%d%s\",\"id6\":\"\",\"id7\":\"\"}"
							,"TH3","Temperature 3",p_Object->TH[i][2],"℃","Humidity 3",p_Object->TH[i][5],"%");
						output +=tab;
						////////////////////
						tab.Empty();
						tab.Format("{\"total\":\"0\",\"rows\":[%s]}",output);
						body +=tab;
						return changeToUtf8(body);
					}
				}
			}
		}
	}
	return changeToUtf8(body);
}
bool Handlelist(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/datalist",8)==0)
	{
		body = list(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////
CString logs(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];
	CString tm[3];
	int m = 0;
	int i = 0;
	int j=0;
	int k=0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	m = 0;
	i=0;
	while(m<3)
	{
		CString a = zd[2].Tokenize("/",i);
		if(a == "")
			break;
		tm[m] = a;
		m++;
	}
	CString filename;
	filename.Format("%4s%2s%2s",tm[2],tm[0],tm[1]);
	filename.Replace(" ","0");

	/////////
	if(!makesureFile(getFilePath()+"\\logs\\"+filename))
	{
		body = "{\"total\":0,\"rows\":[]}";
		return changeToUtf8(body);
	}
	CFile file;
	if(zd[1]=="users")
	{
		if(!makesureFile(getFilePath()+"\\logs\\"+filename+"\\users"))
		{
			body = "{\"total\":0,\"rows\":[]}";
			return changeToUtf8(body);
		}
		CString tabbody;
		file.Open(getFilePath()+"\\logs\\"+filename+"\\users",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
		file.SeekToBegin();
		i=0;
		while(1)
		{
			int type =0;
			char value[128];
			memset(value,0,sizeof(char)*128);
			file.Read(value,1);
			type = value[0];
			if(type<=0)
			{
				file.Close();
				body.Empty();
				body.Format("{\"total\":0,\"rows\":[%s]}",tabbody);
				return changeToUtf8(body);
			}

			CString logvalue[4];
			file.Seek(226*i+2,CFile::begin);
			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[0] = value;
			file.Seek(226*i+34,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[1] = value;
			file.Seek(226*i+66,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[2] = value;
			file.Seek(226*i+98,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,126);
			logvalue[3] = value;
			file.Seek(226*i+226,CFile::begin); 

			if(i)
			{
				tabbody += ",";
			}
			CString tab;
			tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\"id4\":\"%s\",\"id5\":\"%s\"}",i+1,logvalue[0],
				logvalue[1],logvalue[2],logvalue[3]);
			tabbody += tab;
			////////////////
			i++;
		}
		file.Close();
	}
	else if(zd[1]=="alarm")
	{
		if(!makesureFile(getFilePath()+"\\logs\\"+filename+"\\alarm"))
		{
			body = "{\"total\":0,\"rows\":[]}";
			return changeToUtf8(body);
		}
		CString tabbody;
		file.Open(getFilePath()+"\\logs\\"+filename+"\\alarm",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
		file.SeekToBegin();
		i=0;
		while(1)
		{
			int type =0;
			char value[128];
			memset(value,0,sizeof(char)*128);
			file.Read(value,1);
			type = value[0];
			if(type<=0)
			{
				file.Close();
				body.Empty();
				body.Format("{\"total\":0,\"rows\":[%s]}",tabbody);
				return changeToUtf8(body);
			}

			CString logvalue[4];
			file.Seek(226*i+2,CFile::begin);
			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[0] = value;
			file.Seek(226*i+34,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[1] = value;
			file.Seek(226*i+66,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			logvalue[2] = value;
			file.Seek(226*i+98,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,126);
			logvalue[3] = value;
			file.Seek(226*i+226,CFile::begin); 

			if(i)
			{
				tabbody += ",";
			}
			CString tab;
			tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\"id4\":\"%s\",\"id5\":\"%s\"}",i+1,logvalue[0],
				logvalue[1],logvalue[2],logvalue[3]);
			tabbody += tab;
			////////////////
			i++;
		}
		file.Close();
	}
	body = "{\"total\":0,\"rows\":[]}";
	return changeToUtf8(body);
}
bool Handlelogs(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/dtlogs",6)==0)
	{
		body = logs(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////


CString emsats(char *url)
{
	/////////中文编码////////////////
	int len; 
	len = MultiByteToWideChar(CP_UTF8, 0, url, -1, NULL,0); 
	wchar_t* wszUniCode = new wchar_t[len*2+2]; 
	memset(wszUniCode, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, url, -1, wszUniCode, len); 
	CString  rec(wszUniCode); 
	delete[] wszUniCode; 
	////////
	CString body;
	CString zd[3];	
	int m = 0;
	int i = 0;
	int j=0;
	int k=0;
	while(m<3)
	{
		CString a = rec.Tokenize("=",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[1]=="normal")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if((p_Object->ipaddr.GetLength()&&p_Object->dtype!=0&&p_Object->EMSstatus[0]==1)||(p_Object->ipaddr.GetLength()&&p_Object->dtype!=0&&p_Object->ATS_status==1))
			{
				if(j)
				{
					body +=",";
				}
				

				CString tab;
				if(p_Object->dtype==1&&p_Object->EMSstatus[0]==1)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","EMS");
					body +=tab;
					tab.Format("\"text\":\"%s\",\"children\":[",p_Object->ipaddr);
					body +=tab;
					k=0;
					for(i=0;i<10;i++)
					{
						if(p_Object->EMSname[i].GetLength())
						{
							if(k)
							{
								body +=",";
							}
							tab.Empty();
							tab.Format("{\"text\":\"%s\"}",p_Object->EMSname[i]);
							body +=tab;
							k++;
						}
					}
					body +="]";
					body +="}";
					body +="]";
					body +="}";
					j++;
				}
				else if(p_Object->dtype==2&&p_Object->ATS_status==1)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","ATS");
					body +=tab;
					tab.Format("\"text\":\"%s\"}]",p_Object->ipaddr);
					body +=tab;
					body +="}";
					j++;
				}
				
				
			}
		}
		body +="]";
	}
	if(zd[1]=="alarm")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if((p_Object->ipaddr.GetLength()&&p_Object->dtype!=0&&p_Object->EMSstatus[0]==2)||(p_Object->ipaddr.GetLength()&&p_Object->dtype!=0&&p_Object->ATS_status==2))
			{
				if(j)
				{
					body +=",";
				}
				

				CString tab;
				if(p_Object->dtype==1&&p_Object->EMSstatus[0]==2)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","EMS");
					body +=tab;
					tab.Format("\"text\":\"%s\",\"children\":[",p_Object->ipaddr);
					body +=tab;
					k=0;
					for(i=0;i<10;i++)
					{
						if(p_Object->EMSname[i].GetLength())
						{
							if(k)
							{
								body +=",";
							}
							tab.Empty();
							tab.Format("{\"text\":\"%s\"}",p_Object->EMSname[i]);
							body +=tab;
							k++;
						}
					}
					body +="]";
					body +="}";
					body +="]";
					body +="}";
					j++;
				}
				else if(p_Object->dtype==2&&p_Object->ATS_status==2)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","ATS");
					body +=tab;
					tab.Format("\"text\":\"%s\"}]",p_Object->ipaddr);
					body +=tab;
					body +="}";
					j++;
				}
				
				
			}
		}
		body +="]";
	}
	if(zd[1]=="offline")
	{
		body +="[";
		j=0;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength()&&p_Object->dtype!=0)
			{
				if(j)
				{
					body +=",";
				}
				

				CString tab;
				if(p_Object->dtype==1&&p_Object->EMSstatus[0]==0)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","EMS");
					body +=tab;
					tab.Format("\"text\":\"%s\",\"children\":[",p_Object->ipaddr);
					body +=tab;
					k=0;
					for(i=0;i<10;i++)
					{
						if(p_Object->EMSname[i].GetLength())
						{
							if(k)
							{
								body +=",";
							}
							tab.Empty();
							tab.Format("{\"text\":\"%s\"}",p_Object->EMSname[i]);
							body +=tab;
							k++;
						}
					}
					body +="]";
					body +="}";
					body +="]";
					body +="}";
					j++;
				}
				else if(p_Object->dtype==2&&p_Object->ATS_status==0)
				{
					body +="{";
					tab.Format("\"text\":\"%s\",\"children\":[{","ATS");
					body +=tab;
					tab.Format("\"text\":\"%s\"}]",p_Object->ipaddr);
					body +=tab;
					body +="}";
					j++;
				}
				
				
			}
		}
		body +="]";
	}
	else if(zd[1]=="logslist")
	{
		class list
		{
		public:
			int title;
			int daytitle[32];
		};
		CList<list,list&> loglist;

		CFileFind ff;
		BOOL res = ff.FindFile(getFilePath()+"\\logs\\*.*");
		while(res)
		{
			res = ff.FindNextFile();
			if(ff.IsDirectory() && !ff.IsDots())
			{
				int year;
				int mon;
				int day;
				CString strTitle = ff.GetFileTitle();
				year = atoi(strTitle.Left(4));
				mon = atoi(strTitle.Mid(4,2));
				day = atoi(strTitle.Right(2));

				////////////////////////////////////
				int flags = false;
				POSITION pos=loglist.GetHeadPosition();
				while(pos!= NULL)
				{
					list *m_list;
					m_list=&(loglist.GetNext(pos));
					if(m_list->title == year*100+mon)
					{
						if(day>0 && day<32)
						{
							CString path = ff.GetFilePath();
							if(makesureFile(path+"\\users") && makesureFile(path+"\\alarm"))
								m_list->daytitle[day-1]=3;
							else if(makesureFile(path+"\\users"))
								m_list->daytitle[day-1]=1;
							else if(makesureFile(path+"\\alarm"))
								m_list->daytitle[day-1]=2;
						}
						flags = true;
					}
				}
				if(!flags)
				{
					list m_list;
					m_list.title = year*100+mon;
					for(i=0;i<32;i++)
					{
						m_list.daytitle[i]=0;
					}
					if(day>0 && day<31)
					{
						CString path = ff.GetFilePath();
						if(makesureFile(path+"\\users") && makesureFile(path+"\\alarm"))
							m_list.daytitle[day-1]=3;
						else if(makesureFile(path+"\\users"))
							m_list.daytitle[day-1]=1;
						else if(makesureFile(path+"\\alarm"))
							m_list.daytitle[day-1]=2;
					}
					loglist.AddTail(m_list);
				}
			}
		}
		ff.Close();
		//body = "[{\"id\":1,\"text\":\"Folder1\",\"children\":[{\"id\":1,\"text\":\"File1\"}] }]";
		body +="[";
		j=0;
		POSITION posnow = loglist.GetHeadPosition();
		while (posnow != NULL)
		{
			list m_list;
			m_list = loglist.GetNext(posnow);
			CString title1;
			title1.Format("Year:%d Month:%d",m_list.title/100,m_list.title%100);
			
			if(j)
			{
				body +=",";
			}
			body +="{";
			CString tab;
			tab.Format("\"text\":\"%s\"",title1);
			body +=tab;
			//////////////////////////
			body +=",\"children\":[";
			int k = 0;
			for(i=0;i<31;i++)
			{
				if(zd[2]=="1")
				{
					if(m_list.daytitle[i]==3 || m_list.daytitle[i]==1)
					{
						if(k)
						{
							body +=",{";
						}
						else
						{
							body +="{";
						}

						CString title2;
						title2.Format("%d/%d/%d",m_list.title%100,i+1,m_list.title/100);
						body +="\"text\":\"";
						body +=title2;

						body +="\"}";
						k++;
					}
				}
				else if(zd[2]=="2")
				{
					if(m_list.daytitle[i]==3 || m_list.daytitle[i]==2)
					{
						if(k)
						{
							body +=",{";
						}
						else
						{
							body +="{";
						}

						CString title2;
						title2.Format("%d/%d/%d",m_list.title%100,i+1,m_list.title/100);
						body +="\"text\":\"";
						body +=title2;

						body +="\"}";
						k++;
					}
				}
				
			}
			body +="]";
			///////////////////////////
			body +="}";
			j++;
			
		}
		body +="]";

	}
	else if(zd[1].GetLength() && zd[2].GetLength())
	{
		CString tab;
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Object = NULL;
			
			k=0;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Object->ipaddr.GetLength()   && p_Object->dtype==1)//EMS
			{
				for(i=0;i<10;i++)
				{
					if(p_Object->EMSname[i]==zd[2] && zd[2].GetLength()&&p_Object->EMSstatus[i]!=0)
					{
						if(k)
						{
							tab += ",";
						}
						tab.Empty();
						tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",1,"设备IP地址",p_Object->ipaddr,"正常");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",2,"设备名称",p_Object->EMSname[i],"副机设备名称");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",3,"门禁 1",p_Object->EMS_door_PDU[0][i],"第一个门禁传感器");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",4,"门禁 2",p_Object->EMS_door_PDU[1][i],"第二个门禁传感器");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",5,"水浸",p_Object->EMS_water_PDU[i],"水浸传感器");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"%s\"}",6,"烟雾",p_Object->EMS_smoke_PDU[i],"烟雾传感器");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"电流:%.1fA,电压:%dV,电能:%.1fKwh\"}",7,"PDU1","Metered PDU",p_Object->EMS_cur_PDU[0][i],p_Object->EMS_vol_PDU[0][i],p_Object->EMS_energy_PDU[0][i]);
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"电流:%.1fA,电压:%dV,电能:%.1fKwh\"}",8,"PDU2","Metered PDU",p_Object->EMS_cur_PDU[1][i],p_Object->EMS_vol_PDU[1][i],p_Object->EMS_energy_PDU[1][i]);
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"电流:%.1fA,电压:%dV,电能:%.1fKwh\"}",9,"PDU3","Metered PDU",p_Object->EMS_cur_PDU[2][i],p_Object->EMS_vol_PDU[2][i],p_Object->EMS_energy_PDU[2][i]);
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"电流:%.1fA,电压:%dV,电能:%.1fKwh\"}",10,"PDU4","Metered PDU",p_Object->EMS_cur_PDU[3][i],p_Object->EMS_vol_PDU[3][i],p_Object->EMS_energy_PDU[3][i]);
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"温度:%d ℃,湿度:%d%s\"}",11,"温度/湿度","The first sensor",p_Object->EMS_th_PDU[0][i],p_Object->EMS_th_PDU[1][i],"%");
						body+=tab;
						tab.Empty();
						tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
								   \"id4\":\"温度:%d ℃,湿度:%d%s\"}",12,"温度/湿度","The second sensor",p_Object->EMS_th_PDU[2][i],p_Object->EMS_th_PDU[3][i],"%");
						body+=tab;
						tab.Empty();
						tab.Format("{\"total\":\"0\",\"rows\":[%s]}",body);
						body =tab;
						k++;
						return changeToUtf8(body);
					}
					
				}
				
			}
			if(p_Object->ipaddr.GetLength()&&p_Object->ATS_status!=0&& p_Object->dtype==2)//ATS
			{
				if(p_Object->ipaddr==zd[2]&&zd[2].GetLength())
				{
					tab.Empty();
					tab.Format("{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",1,"设备IP地址",p_Object->ipaddr,"正常");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",2,"设备名称",p_Object->ATS_name,"");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",3,"Source A名称",p_Object->ATS_lin1name,"");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",4,"Source B名称",p_Object->ATS_lin2name,"");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",5,"当前输入",p_Object->ATS_currentinput,"");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"%s\"}",6,"主输入",p_Object->ATS_maininput,"");
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"电流:%.1fA,电压:%dV\"}",7,"Source A状态",p_Object->ATS_lin1name,p_Object->ATS_CA,p_Object->ATS_VA);
					body+=tab;
					tab.Empty();
					tab.Format(",{\"id1\":\"%d\",\"id2\":\"%s\",\"id3\":\"%s\",\
							   \"id4\":\"电流:%.1fA,电压:%dV\"}",8,"Source B状态",p_Object->ATS_lin2name,p_Object->ATS_CB,p_Object->ATS_VB);
					body+=tab;
					tab.Empty();
					tab.Format("{\"total\":\"0\",\"rows\":[%s]}",body);
					body =tab;
					return changeToUtf8(body);

				}
			}
		}
	}
	return changeToUtf8(body);
}

bool Handleemsats(TSession *r)
{
	CString body;
	if(strncmp(r->uri,"/emsatslist",8)==0)
	{
		body = emsats(r->uri);
		if(p_putanswer)
		{
			char* z =NULL;
			z = new char[body.GetLength()*2+2];
			ZeroMemory(z,body.GetLength()*2+2);
			strncpy_s(z,body.GetLength()*2+2,body,body.GetLength()*2);

			p_putanswer(r,200,z);
		
			delete[] z; 
			r->rec = TRUE;
			return TRUE;
		}
	}
	r->rec = FALSE;
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
UINT CServerSet::webstart(LPVOID p)
{
	webtreadflags = 1;
	hlib = ::LoadLibrary(getFilePath()+"\\web.dll");
	if(NULL == (p_webinit = (WEBINIT)GetProcAddress(hlib,"webinit"))) {
        goto Done;
    }
	if(NULL == (p_webend = (WEBEND)GetProcAddress(hlib,"webend"))) {
        goto Done;
    }
	if(NULL == (p_createweb = (WEBCREATE)GetProcAddress(hlib,"webcreate"))) {
        goto Done;
    }
	if(NULL == (p_webstart = (WEBSTART)GetProcAddress(hlib,"webstart"))) {
        goto Done;
    }
	if(NULL == (p_webadd = (WEBADD)GetProcAddress(hlib,"webadd"))) {
        goto Done;
    }
	if(NULL == (p_putanswer = (PUTANSWER)GetProcAddress(hlib,"putanswer"))) {
        goto Done;
    }
	
	///////////////////
	p_webinit();
	char path[255];
	memset(path,0,sizeof(char)*255);
	strcpy_s(path,sizeof(char)*254,getFilePath()+"\\www");
	p_createweb("clever",webport,webactivetime,path,NULL);
	p_webadd(Handlelogin);
	p_webadd(Handletuopu);
	p_webadd(Handlecabin);
	p_webadd(Handlestatus);
	p_webadd(Handleenv);
	p_webadd(Handlelist);
	p_webadd(Handleemsats);
	p_webadd(Handlelogs);
	p_webstart();

Done:
	if(NULL != hlib) {
		    ::FreeLibrary(hlib);
			hlib = NULL;
			p_webinit = NULL;
			p_webend = NULL;
			p_createweb = NULL;
			p_webstart = NULL;
			p_webadd = NULL;
			p_putanswer = NULL;
			
	}
	webtreadflags = 2;
	return 0;
}


void CServerSet::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	if(!webtreadflags)
	{
		return;
	}
	p_webend();
	WSADATA wsaDatta;
	WSAStartup(MAKEWORD(2,2),&wsaDatta);
	SOCKET connectsock;
	connectsock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(connectsock == INVALID_SOCKET)
	{
		WSACleanup();
		MessageBox(_T("stop the webservers is error!"));
		return;
	}
	sockaddr_in clientser;
	clientser.sin_family = AF_INET;
	clientser.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientser.sin_port = htons(webport);//注意
	if(connect(connectsock,(SOCKADDR*) &clientser,sizeof(clientser)) == SOCKET_ERROR)
	{
		WSACleanup();
		MessageBox(_T("stop the webservers is error!"));
		return;
	}
	send(connectsock,"clever",sizeof("clever"),0);
	WSACleanup();
	while(webtreadflags !=2)
	{
		Sleep(100);
	}
	if(NULL != hlib) {
		::FreeLibrary(hlib);
		hlib = NULL;
		p_webinit = NULL;
		p_webend = NULL;
		p_createweb = NULL;
		p_webstart = NULL;
		p_webadd = NULL;
		p_putanswer = NULL;
	}	
    webtreadflags =0;
}

void CServerSet::OnEnChangeUserDetail()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
