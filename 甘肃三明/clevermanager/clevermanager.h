// clevermanager.h : clevermanager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CclevermanagerApp:
// 有关此类的实现，请参阅 clevermanager.cpp

//#define DEMO    1
#define VER    "版本:1.2.0-C"
struct datacenter
{
	CString name;
	CObList canbin;
	//float kva[24][60];
    float kva[24];
	float cur1;
	float cur2;
	float cur3;
	int vol1;
	int vol2;
	int vol3;
	int tp;
	int hm;
	int offnum;
	int alarmnum;
	int normalnum;
	CString str_draw;
	float energy1;
	float energy2;
	float energy3;
};
struct userinfo
{
	char name[32];
	char psd[32];
	char mail[64];
	char tel[20];
	char detail[128];
	int grade;
};
struct userlogs
{
	int logtype;
	int type;
	char name[32];
	char item[32];
	char detail[128];
};
class CclevermanagerApp : public CWinApp
{
public:
	CclevermanagerApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected: 
	GdiplusStartupInput m_gdiplusStartupInput; 
	ULONG_PTR m_gdiplusToken; 
public:
	virtual int ExitInstance();
};

extern CclevermanagerApp theApp;
extern CString changevalue(CString strHex);
extern CString getFilePath(void);
extern void drawImagePNG(CDC *pDC,CString src,int px,int py,int w,int h);
extern void drawFont(CDC *pDC,int type,COLORREF color);
extern datacenter DCmember[10];
extern CObList snmpobj;
extern CString currentIP1;
extern CString currentIP2;
extern void makesureDir(CString dir);
extern bool makesureFile(CString name);
void retange(CDC *pDC, int x, int y, int w, int h);
extern userinfo user[20];
extern CString changeToUtf8(CString body);
extern BOOL IsPathExist(char* lpszPath);
extern BOOL CreateDirectoryDir(char* lpszCreatePath);
