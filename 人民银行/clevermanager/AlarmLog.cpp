// OutputLog.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "AlarmLog.h"
#include <afxdb.h>

// COutputLog 对话框

IMPLEMENT_DYNAMIC(CAlarmLog, CDialog)

CAlarmLog::CAlarmLog(CWnd* pParent /*=NULL*/)
: CDialog(CAlarmLog::IDD, pParent)
{

}

CAlarmLog::~CAlarmLog()
{
}

void CAlarmLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_time1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_time2);
}


BEGIN_MESSAGE_MAP(CAlarmLog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// COutputLog 消息处理程序

BOOL CAlarmLog::OnEraseBkgnd(CDC* pDC)
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

	BkDc.TextOutA(20,30,"请确定导出日志的日期");
	drawFont(&BkDc,0,RGB(64,64,64));

	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.Width()-25,6,12,12);
	CBrush brush1(RGB(240,240,240));
	BkDc.SelectObject(&brush1);
	BkDc.Rectangle(225,170,285,190);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\open.png",230,173,12,12);
	BkDc.TextOutA(245,175,"导出");
	BkDc.TextOutA(40,55,"日志名字：");
	BkDc.TextOutA(40,90,"起始时间：");
	BkDc.TextOutA(40,125,"结束时间：");
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CAlarmLog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(point.x<285&&point.x>225&&point.y<190&&point.y>170)
	{
		//AfxMessageBox("aa");
		CString strName,strName1;
		GetDlgItemTextA(IDC_EDIT1,strName);
		CDatabase database;  
		CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动
		CString sExcelFile,sPath; 
		CString sSql;  

		//获取主程序所在路径,存在sPath中   
		GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);  
		sPath.ReleaseBuffer ();  
		int nPos;  
		nPos=sPath.ReverseFind ('\\');  
		sPath=sPath.Left (nPos);
		strName1.Format("\\%s.xls",strName);
		sPath += strName1;
		sExcelFile = sPath;             // 要建立的Excel文件   
		CFile file;
		try  
		{ 
			// 创建进行存取的字符串   
			sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile); 
			if(database.OpenEx(sSql,CDatabase::noOdbcDialog))  
			{ 
				sSql = "CREATE TABLE Exceldemo (日期 char,用户或IP char,内容 char,详细描述 char)";  
				database.ExecuteSQL(sSql);  

				CTime time,time1;
				m_time1.GetTime(time);
				CString day1;
				day1.Format("%4d%2d%2d",time.GetYear(),time.GetMonth(),time.GetDay());
				day1.Replace(" ","0");

				m_time2.GetTime(time1);
				CString day2;
				day2.Format("%4d%2d%2d",time1.GetYear(),time1.GetMonth(),time1.GetDay());
				day2.Replace(" ","0");
				/////////////////////////////////////////////////////////////////////查找日志
				CString str1,str2,str3,str4;
				CFileFind ff;
				BOOL res=ff.FindFile(getFilePath()+"\\logs\\*.*");
				while(res)
				{
					res=ff.FindNextFileA();
					if(ff.IsDirectory() && !ff.IsDots())
					{
						CString strTitle=ff.GetFileTitle();
						int aa=logtime(day1,day2,strTitle);
						if(aa!=0)
						{
							if(!makesureFile(getFilePath()+"\\logs\\"+strTitle))
							{
								return;
							}
							file.Open(getFilePath()+"\\logs\\"+strTitle+"\\alarm",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
							file.SeekToBegin();
							int i;
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
									break;
								}

								file.Seek(226*i+2,CFile::begin);

								memset(value,0,sizeof(char)*128);
								file.Read(value,32);
								str1.Format("%s",value);
								file.Seek(226*i+34,CFile::begin);

								memset(value,0,sizeof(char)*128);
								file.Read(value,32);
								str2.Format("%s",value);
								file.Seek(226*i+66,CFile::begin);

								memset(value,0,sizeof(char)*128);
								file.Read(value,32);
								str3.Format("%s",value);
								file.Seek(226*i+98,CFile::begin);

								memset(value,0,sizeof(char)*128);
								file.Read(value,126);
								str4.Format("%s",value);
								file.Seek(226*i+226,CFile::begin);

								sSql.Format("INSERT INTO Exceldemo (日期,用户或IP,内容,详细描述) VALUES ('%s','%s','%s','%s')",str1,str2,str3,str4); 
								database.ExecuteSQL(sSql);  
								i++;
							} 
						}
					}
					if(res==0)
					{
						AfxMessageBox("导出成功!");
						return;
					}
				}
				file.Close();
			}
		}
		catch(CUserException ex)  
		{  
			ex.ReportError();  
		}
	}
	if(point.x>270 && point.x<290 && point.y>5 && point.y<20)//BkDc.Rectangle(270,5,290,20);
	{
		OnOK();
	}
	CDialog::OnLButtonDown(nFlags, point);
}


int CAlarmLog::logtime(CString str1 , CString str2 , CString str3)
{
	int a1,a2,a3,b1,b2,b3,c1,c2,c3;
	a1=atoi(str1.Left(4));
	a2=atoi(str1.Mid(4,2));
	a3=atoi(str1.Right(2));

	b1=atoi(str2.Left(4));
	b2=atoi(str2.Mid(4,2));
	b3=atoi(str2.Right(2));

	c1=atoi(str3.Left(4));
	c2=atoi(str3.Mid(4,2));
	c3=atoi(str3.Right(2));

	if(a1>c1)
	{
		return 0;
	}
	if(b1<c1)
	{
		return 0;
	}
	if(a1==c1)
	{
		if(b1==c1)
		{
			if(a2<c2)
			{
				if(b2>c2)
				{
					return 1;
				}
				else if(b2==c2)
				{
					if(b3>c3||b3==c3)
					{
						return 1;
					}
					else
						return 0;
				}
				else
					return 0;
			}
			else if(a2==c2)
			{
				if(b2==c2)
				{
					if((a3<c3||a3==c3)&&(b3==c3||b3>c3))
					{
						return 1;
					}
					else
						return 0;
				}
				else if(b2>c2)
				{
					return 1;
				}
				else
					return 0;
			}
			else
				return 0;

		}
		else if(b1>c1)
		{
			return 1;
		}
	}
	else if(a1<c1)
	{
		if(b1==c1)
		{
			if(b2==c2)
			{
				if(b3==c3||b3>c3)
				{
					return 1;
				}
				else
					return 0;
			}
			else if(b2>c2)
			{
				return 1;
			}
			else
				return 0;
		}
		if(b1>c1)
		{
			return 1;
		}
	}


	return 0;
}
LRESULT CAlarmLog::OnNcHitTest(CPoint point)
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
	return CDialog::OnNcHitTest(point);
}
