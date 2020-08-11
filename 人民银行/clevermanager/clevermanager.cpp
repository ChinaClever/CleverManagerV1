// clevermanager.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "clevermanager.h"
#include "MainFrm.h"

#include "clevermanagerDoc.h"
#include "clevermanagerView.h"
#include "InitClever.h"
#include "OverTime.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
typedef int  (*DE)(char *,  char *, int );
typedef int  (*EN)(char * ,int ,char *);

// CclevermanagerApp

BEGIN_MESSAGE_MAP(CclevermanagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CclevermanagerApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CclevermanagerApp 构造

CclevermanagerApp::CclevermanagerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CclevermanagerApp 对象

CclevermanagerApp theApp;
datacenter DCmember[10];
CObList snmpobj;
CString currentIP1;//当前查看IP
CString currentIP2;//当前查看IP
userinfo user[20];
bool gShowFlag;//自动刷新日志标志位false//不刷新 true //刷新

CString changevalue(CString strHex)
{
	CString str;
	CString strSrc;
	CString strTmp;
	long Hex;
	int i =0;
	strHex.Trim();
	strHex.Replace(" ","");
	while(i < strHex.GetLength())
	{  
		strSrc = strHex.Mid(i,2); 
		i += 2;   
		Hex = _tcstoul(strSrc, NULL, 16);
		strTmp += (TCHAR)Hex;
	}
	return strTmp;
}
CString getFilePath(void)
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind (_T('\\'));
	sPath=sPath.Left(nPos);
	return sPath;
}
void drawImagePNG(CDC *pDC,CString src,int px,int py,int w,int h)
{
	Graphics graphics( pDC->GetSafeHdc());
	LPWSTR des =NULL;
	des = new WCHAR[src.GetLength()+2];
	memset(des,0,sizeof(WCHAR)*(src.GetLength()+2));
	
	MultiByteToWideChar(CP_ACP,0,src,src.GetLength(),des,src.GetLength()+2);

	
	Image image(des);
	SelectObject( pDC->m_hDC, &image);
	//if(w && h)
	//{
	//	graphics.DrawImage(&image, px,py,w,h);
	//}
	if(w || h)
	{
		graphics.DrawImage(&image, px,py,w,h);
	}
	else
	{
		graphics.DrawImage(&image, px,py);
	}
	delete[] des;
	graphics.ReleaseHDC(pDC->GetSafeHdc()); 
}
void drawFont(CDC *pDC,int type,COLORREF color)
{
	if(type==0)//正文
	{
		CFont font;
		font.CreateFont(
						  12,                        // nHeight
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
		pDC->SetTextColor(color);
	}
	else if(type==1)//标题
	{
		CFont font;
		font.CreateFont(
						  14,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
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
		pDC->SetTextColor(color);
	}
	else if(type==2)//标题
	{
		CFont font;
		font.CreateFont(
						  18,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
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
		pDC->SetTextColor(color);
	}
	else if(type==3)//标题
	{
		CFont font;
		font.CreateFont(
						  24,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
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
		pDC->SetTextColor(color);
	}
	else if(type==4)//标题
	{
		CFont font;
		font.CreateFont(
						  18,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_SEMIBOLD,                 // nWeight
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
		pDC->SetTextColor(color);
	}
	else if(type==5)//标题
	{
		CFont font;
		font.CreateFont(
						  10,                        // nHeight
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
		pDC->SetTextColor(color);
	}
}
// CclevermanagerApp 初始化
void makesureDir(CString dir)//文件夹是否存在
{
	CreateDirectoryDir((LPSTR)(LPCTSTR)dir);
	CFileFind   filedir; 
	if(filedir.FindFile(dir))
	{
		filedir.Close();
		return;
	}
	else
	{
		_mkdir(dir);
		filedir.Close();
	}
}

// 判断目录(不能用来查询根目录,否则总是失败.)是否存在的函数
BOOL IsPathExist(char* lpszPath)
{
	//char* szTemp = W2A(lpszPath);
	char szTemp[MAX_PATH]={0};
	strcpy(szTemp, lpszPath);

	BOOL bExist; // 最后结果，表示目录是否存在
	if(szTemp[strlen(lpszPath)-1]=='\\')
	{
		szTemp[strlen(lpszPath)-1]='\0';    // 先删除最后的“\”
	}
	WIN32_FIND_DATAA wfd;                                  // 查找
	HANDLE hFind = FindFirstFileA(szTemp, &wfd);                  
	if(hFind == INVALID_HANDLE_VALUE)
	{
		bExist = FALSE;            // 没有找到配备，目录肯定不存在
	}
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // 检查找到的结果是否目录
			bExist = TRUE;     // 是目录,目录存在
		else
			bExist = FALSE;       // 是文件,目录不存在
		FindClose(hFind);
	}
	return bExist;
}

//
// 函数: CreateDirectoryEx()
// 功能: 
//    创建给定路径, 即使是多级路径, 也可以创建. (注意: 给定的路径不能包含'\'结尾, 不能是根目录如: C:\ )
// 示例: CreateDirectoryEx( "D:\\Path1\\Path2\\..." )
// 参数: 
//   lpszCreatePath -- 要创建的目录
// 返回值:
//       如果成功创建所有目录返回TRUE, 否则返回FALSE
// 作者: SenkoBQB
// 创建日期: 2009-1-18        
BOOL CreateDirectoryDir(char* lpszCreatePath)
{
	char szCreatePathTemp[MAX_PATH]={0};
	//USES_CONVERSION;
	//char* szCreatePathTemp = W2A(lpszCreatePath);
	strcpy(szCreatePathTemp, lpszCreatePath);
	if(szCreatePathTemp[strlen(szCreatePathTemp)-1]=='\\')
	{
		szCreatePathTemp[strlen(szCreatePathTemp)-1]='\0';    // 先删除最后的“\”
	}
	char szTemp[MAX_PATH] = ""; // 临时缓存
	// 查找路径分隔符号'\'
	char *p = strchr(szCreatePathTemp, '\\'); // 第一次的出现位置(地址)
	if (p != NULL)
	{
		p = strchr(p+1, '\\');   // 第二次位置, 如果是NULL 则说明是一级目录, 如: c:\path 这样如果该目录不存在会直接创建该目录
	}    
	while (p != NULL)            // 之后的位置(要能进入此循环, 至少是二级目录, 如: c:\一级目录\二级目录\...)
	{
		strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp);     // 如: e:\1
		if (!IsPathExist(szTemp)) // 如果不存在, 则一路创建到倒数第二个目录
		{
			// 测试
			if (!CreateDirectoryA(szTemp, NULL)) 
			{
				printf("创建目录%s失败!错误代码: %d\n", szTemp, GetLastError());
				return FALSE;
			}
			printf("已成功创建目录:%s\n", szTemp); // 创建该层目录, 之后一路往下创建不用判断

			p = strchr(p+1, '\\');   // 下一级子目录
			while( p != NULL )
			{
				strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp); // 如: e:\1\2
				// 测试
				if (!CreateDirectoryA(szTemp, NULL)) 
				{
					printf("创建目录%s失败!错误代码: %d\n", szTemp, GetLastError());
					return FALSE;
				}
				p = strchr(p+1, '\\'); 
			}
		}
		if ( p != NULL ) // 防止上面内层的while
		{
			p = strchr(p+1, '\\');   // 下一级子目录
		}
	}
	if (!IsPathExist(lpszCreatePath)) // 如果目录不存在, 则创建目录.(也是创建最后一个目录)
	{
		// 测试
		if (!CreateDirectoryA(lpszCreatePath, NULL)) 
		{
			printf("创建目录%s失败!错误代码: %d\n", lpszCreatePath, GetLastError());
			return FALSE;
		}
		printf("已成功创建目录:%s\n", lpszCreatePath); 
	}


	return TRUE;
} 

bool makesureFile(CString name)//文件夹是否存在
{
	CFileFind   filedir; 
	if(filedir.FindFile(name))
	{
		filedir.Close();
		return true;
	}
	else
	{
		filedir.Close();
		return false;
	}
}

void retange(CDC *pDC, int x, int y, int w, int h)
{
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	LinearGradientBrush linGrBrush(Point(x,y),Point(x,y+h),Color(128,255,255,255),Color(128,255,255,255));
	//LinearGradientBrush linGrBrush;
	Color colors[] = {
		Color(128, 255,255,255),   // red
		Color(128, 225,225,240),//yellow
		Color(128, 164,164,225),   // blue
		Color(128, 128,128,164)};  // green

		REAL positions[] = {
			0.0f,  
			0.4f,
			0.8f,
			1.0f}; 
			linGrBrush.SetInterpolationColors(colors, positions,4);
			graphics.FillRectangle(&linGrBrush,x,y,w,h);
	graphics.ReleaseHDC(pDC->GetSafeHdc());
}
CString changeToUtf8(CString body)
{
	CString   str; 
	str = body;
	int   nLen   =   MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   NULL,0); 
	WCHAR   *wszUTF   =   new   WCHAR[nLen+1]; 
	MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   wszUTF,   nLen); 
	nLen   =   WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   NULL,   0,   NULL,   NULL); 
	TCHAR   *szUTF   =   new   TCHAR[nLen+1]; 
	WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   szUTF,   nLen,   NULL,NULL); 
	
	CString  wecrec(szUTF);
	delete[]   szUTF; 
	delete[]   wszUTF;
	return wecrec;
}

void ReadString(HKEY key,CString valuename,CString & value)
{
	DWORD type;
	DWORD len =_MAX_PATH;
	BYTE s[_MAX_PATH];
	memset(&s,0,sizeof(BYTE)*_MAX_PATH);
	if (RegQueryValueEx(key,valuename,0,&type,s,&len)==ERROR_SUCCESS)
	{
		value = (TCHAR *) s;
	}
	return;
}

BOOL CclevermanagerApp::InitInstance()
{
	CInitClever cleverinit;
	cleverinit.initData();
	/////////////////////////////////////////////////////////////////
	/*
	HINSTANCE hdelib =NULL;
	hdelib = NULL;
	hdelib = ::LoadLibrary(getFilePath()+"\\dedll.dll");
	DE p_de = NULL;
	EN  p_en = NULL;
	if(NULL == (p_de = (DE)GetProcAddress(hdelib,"?de@@YAXPAD0H@Z"))) {
        ::FreeLibrary(hdelib);
		hdelib = NULL;
		return FALSE;
    }

	if(NULL == (p_en = (EN)GetProcAddress(hdelib,"?en@@YA_NPADH0@Z"))) {
        ::FreeLibrary(hdelib);
		hdelib = NULL;
		return FALSE;
    }

	HKEY key;
	RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\cl", &key);

	CString decode;
	CString encode;
    ReadString(key,"cl1",decode);
	ReadString(key,"cl2",encode);
	if(!decode.GetLength() && !encode.GetLength())
	{
		AfxMessageBox(_T("Software has not been register!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	char encodechar[255];
	memset(encodechar,0,sizeof(char)*255);
	char decodechar[255];
	memset(decodechar,0,sizeof(char)*255);
	strcpy_s(decodechar,sizeof(char)*254,decode);
	p_en(decodechar,decode.GetLength(),encodechar);
	CString regcode;
	regcode = encodechar;
	regcode.Replace("=","");
	if(regcode != encode)
	{
		::FreeLibrary(hdelib);
		hdelib = NULL;
		AfxMessageBox(_T("Software has not been register!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}

    ::FreeLibrary(hdelib);
		hdelib = NULL;
	//////////////////////////////////////////////////////////////////////////////////
	HANDLE cleverMutex;
	cleverMutex=INVALID_HANDLE_VALUE;
	cleverMutex=CreateMutex(NULL,true,"clevermutex");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		 AfxMessageBox(_T("applications may be repeat or applications is ending,please wait!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		 return FALSE;
	}

*/
	///////////////////////////////////////////////////////////////////
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CclevermanagerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CclevermanagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	//修改试用时间
/*	
	COverTime aa;
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	CString str,str1;
	str.Format("%4d%2d%2d",ct.wYear,ct.wMonth,ct.wDay);
	str.Replace(" ","0");
	if(atoi(str.Left(4))==2015)
	{
		str1=str.Mid(4,2);
		if(atoi(str1)>7)
		{
			aa.DoModal();
		}
		else if(atoi(str1)==7)
		{
			str1=str.Right(2);
			if(atoi(str1)>30)
			{
				aa.DoModal();
			}
		}
	}
	else if(atoi(str.Left(4))>2015)
		aa.DoModal();*/	
	
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CclevermanagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CclevermanagerApp 消息处理程序


int CclevermanagerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	GdiplusShutdown(m_gdiplusToken); 
	CInitClever cleverinit;
	cleverinit.ExitClever();

	return CWinApp::ExitInstance();
}
