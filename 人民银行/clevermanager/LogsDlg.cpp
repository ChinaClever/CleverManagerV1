// LogsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "clevermanager.h"
#include "LogsDlg.h"
#include "Energylogs.h"
#include "StatusLogs.h"
#include "OutputLog.h"
#include "AlarmLog.h"
// CLogsDlg 对话框

IMPLEMENT_DYNAMIC(CLogsDlg, CDialog)

CLogsDlg::CLogsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogsDlg::IDD, pParent)
	, refreshlisttimer(0)
{

}

CLogsDlg::~CLogsDlg()
{
}

void CLogsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CLogsDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CLogsDlg::OnTvnSelchangedTree1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CLogsDlg::OnNMRclickTree1)
END_MESSAGE_MAP()


// CLogsDlg 消息处理程序

BOOL CLogsDlg::OnEraseBkgnd(CDC* pDC)
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
	//BkDc.SelectStockObject(NULL_PEN);
	rect.bottom -= 1;
	rect.right -= 1;
	BkDc.Rectangle(&rect);

	CBrush brush1(RGB(255,255,255));
	BkDc.SelectObject(&brush1);
	BkDc.Rectangle(0,0,rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,24);

	drawFont(&BkDc,0,RGB(32,32,32));
	//CString title;
	//CTime et;
	//m_time.GetTime(et);
	//title.Format("%4d-%2d-%2d(y-m-d) Logs",et.GetYear(),et.GetMonth(),et.GetDay());
	BkDc.TextOut(20,6,"日志记录");

	BkDc.MoveTo(0,rect.Height()-30);
	BkDc.LineTo(rect.Width(),rect.Height()-30);
	BkDc.MoveTo(249,25);
	BkDc.LineTo(249,rect.Height()-30);

	//////////////
	drawFont(&BkDc,0,RGB(0,0,0));
	BkDc.TextOut(20,rect.Height()-20,"时间选择");

	CBrush brushBTN(RGB(240,240,240));
	BkDc.SelectObject(&brushBTN);
	BkDc.Rectangle(300,rect.Height()-26,420,rect.Height()-4);
	BkDc.TextOut(330,rect.Height()-21,"用户日志");
	BkDc.Rectangle(430,rect.Height()-26,550,rect.Height()-4);
	BkDc.TextOut(460,rect.Height()-21,"异常日志");

	BkDc.Rectangle(560,rect.Height()-26,680,rect.Height()-4);
	BkDc.TextOut(590,rect.Height()-21,"电能数据记录");

	drawImagePNG(&BkDc,getFilePath()+"\\icon\\pencil.png",310,rect.Height()-24,15,15);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",440,rect.Height()-24,15,15);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",570,rect.Height()-24,15,15);

	BkDc.Rectangle(10,rect.Height()-62,90,rect.Height()-40);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\fresh.png",15,rect.Height()-58,15,15);
	BkDc.TextOut(40,rect.Height()-58,"刷新");

	/////////////////////////////////////////////////
	BkDc.Rectangle(690,rect.Height()-26,810,rect.Height()-4);
	BkDc.TextOut(720,rect.Height()-21,"历史状态记录");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",700,rect.Height()-24,15,15);

	BkDc.Rectangle(820,rect.Height()-26,940,rect.Height()-4);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\pencil.png",825,rect.Height()-24,15,15);
	BkDc.TextOut(850,rect.Height()-21,"导出用户日志");

	BkDc.Rectangle(970,rect.Height()-26,1100,rect.Height()-4);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\pencil.png",975,rect.Height()-24,15,15);
	BkDc.TextOut(1000,rect.Height()-21,"导出异常日志");

	//drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",20,rect.Height()-24,15,15);
	///////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CLogsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list.ModifyStyle(LVS_LIST|LVS_ICON|LVS_SMALLICON ,   //LVS_ICON 
		LVS_REPORT |LVS_SHOWSELALWAYS|LVS_AUTOARRANGE); //LVS_REPORT|
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES );

	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 7, 1);
	m_cImageListNormal32.Attach(hList);
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON7));
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON5)); 
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON1)); 
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON5)); 
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON4)); 
	m_cImageListNormal32.Add(AfxGetApp()->LoadIcon(IDI_ICON6)); 
	m_list.SetImageList(&m_cImageListNormal32, LVSIL_SMALL);

	m_list.SetTextColor(RGB(64,64,64));

	m_list.InsertColumn(0,"图标",LVCFMT_LEFT,40,-1);
	m_list.InsertColumn(1,"时间",LVCFMT_LEFT,1,-1);
	m_list.InsertColumn(2,"对象",LVCFMT_LEFT,1,-1);
	m_list.InsertColumn(3,"内容",LVCFMT_LEFT,1,-1); 
	m_list.InsertColumn(4,"详细描述",LVCFMT_LEFT,1,-1); 

	CHeaderCtrl * m_head=m_list.GetHeaderCtrl();
    m_list.m_colorheader.SubclassWindow(m_head->GetSafeHwnd());


	HIMAGELIST hList1 = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 3, 1);
	m_cImageListTree.Attach(hList1);

	CBitmap cBmp1;
	cBmp1.LoadBitmap(IDB_BITMAP3);
	m_cImageListTree.Add(&cBmp1, RGB(255,255, 255));

	CBitmap cBmp2;
	cBmp2.LoadBitmap(IDB_BITMAP4);
	m_cImageListTree.Add(&cBmp2, RGB(255,255, 255));

	CBitmap cBmp3;
	cBmp3.LoadBitmap(IDB_BITMAP5);
	m_cImageListTree.Add(&cBmp3, RGB(255,255, 255));

	m_tree.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);
	m_tree.SetImageList ( &m_cImageListTree,TVSIL_NORMAL );
	treeinsert();
	SetTimer(IDS_LOGS_TIME,10*60*1000,NULL);//IDS_SHOWLOGS_TIME
	SetTimer(IDS_SHOWLOGS_TIME,5000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLogsDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	CDialog::OnSize(nType, cx, cy);
	if(IsWindow(m_time.m_hWnd) && IsWindow(m_list.m_hWnd))
	{
		m_tree.MoveWindow(5,32,243,rect.Height()-100);
		m_time.MoveWindow(100,rect.Height()-25,120,20);
		m_list.MoveWindow(250,26,rect.Width()-253,rect.Height()-57);
		m_list.SetColumnWidth(0,40);
		m_list.SetColumnWidth(1,(rect.Width()-250)*15/100);
		m_list.SetColumnWidth(2,(rect.Width()-250)*15/100);
		m_list.SetColumnWidth(3,(rect.Width()-250)*15/100);
		m_list.SetColumnWidth(4,(rect.Width()-250)*55/100-60);
	}
// TODO: 在此处添加消息处理程序代码
}

void CLogsDlg::readToList(int logtype)
{
	CTime et;
	m_time.GetTime(et);
	CString filename;
	filename.Format("%4d%2d%2d",et.GetYear(),et.GetMonth(),et.GetDay());
	filename.Replace(" ","0");
	treeloginsert(filename,logtype);
}


void CLogsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	if(point.x>300 && point.x<430 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>460 && point.x<550 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>580 && point.x<670 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>10 && point.x<90 && point.y>rect.Height()-62 && point.y<rect.Height()-40)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>690 && point.x<810 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>950 && point.x<1080 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));	
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CLogsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	if(point.x>300 && point.x<430 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		m_list.DeleteAllItems();
		readToList(1);
	}
	if(point.x>460 && point.x<550 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		m_list.DeleteAllItems();
		readToList(2);
	}
	if(point.x>580 && point.x<670 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		CEnergylogs dlg;
		dlg.DoModal();
	}
	if(point.x>10 && point.x<90 && point.y>rect.Height()-62 && point.y<rect.Height()-40)
	{
		treeinsert();
	}
    else if(point.x>690 && point.x<810 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		CStatusLogs dlg;
		dlg.DoModal();
	}
	if(point.x>820 && point.x<940 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		COutputLog logdlg;
		logdlg.DoModal();
	}
	if(point.x>950 && point.x<1080 && point.y>rect.Height()-26 && point.y<rect.Height()-4)
	{
		CAlarmLog logdlg;
		logdlg.DoModal();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CLogsDlg::treeinsert(void)
{
	/////////////////////
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
			CString title;
			title.Format("%d年%d月",year,mon);
			CString son_title;
			son_title.Format("%d-%d-%d",year,mon,day);
			
			HTREEITEM title1 =NULL;
			title1=findtree(NULL,year*100+mon);
			if(!title1)
			{
				HTREEITEM yearitem = m_tree.InsertItem(title,0,0,NULL);
				m_tree.SetItemData(yearitem,year*100+mon);
				HTREEITEM dayitem = m_tree.InsertItem(son_title,2,2,yearitem);
				m_tree.SetItemData(dayitem,day);
			}
			else
			{
				HTREEITEM title2 =NULL;
				title2=findtree(title1,day);
				if(!title2)
				{
					HTREEITEM dayitem = m_tree.InsertItem(son_title,2,2,title1);
					m_tree.SetItemData(dayitem,day);
				}
			}
		}
	}
	ff.Close();
}

HTREEITEM CLogsDlg::findtree(HTREEITEM para,int title)
{
	HTREEITEM hSon = m_tree.GetNextItem(para,TVGN_CHILD);
	while(NULL != hSon)
	{
		if(m_tree.GetItemData(hSon) == title)
		{
				return hSon;
		}
		hSon = m_tree.GetNextItem(hSon,TVGN_NEXT);
	}
	return NULL;
}

void CLogsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM selectitem = m_tree.GetSelectedItem();
	HTREEITEM paraitem = m_tree.GetParentItem(selectitem);
	if(paraitem)
	{
		CString name;
		name.Format("%d",m_tree.GetItemData(paraitem)*100+m_tree.GetItemData(selectitem));
		m_list.DeleteAllItems();
		treeloginsert(name,1);
		treeloginsert(name,2);
	}
	*pResult = 0;
}
void CLogsDlg::treeloginsert(CString filename,int logtype)
{
	if(!makesureFile(getFilePath()+"\\logs\\"+filename))
	{
		return;
	}
	CFile file;
	if(logtype==1)
	{
		if(!makesureFile(getFilePath()+"\\logs\\"+filename+"\\users"))
		{
			return;
		}
		file.Open(getFilePath()+"\\logs\\"+filename+"\\users",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
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
				return;
			}
			m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,type-1,0);
			file.Seek(226*i+2,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,1,value);
			file.Seek(226*i+34,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,2,value);
			file.Seek(226*i+66,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,3,value);
			file.Seek(226*i+98,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,126);
			m_list.SetItemText(0,4,value);
			file.Seek(226*i+226,CFile::begin);
			////////////////
			i++;
		}
		file.Close();
	}
	else if(logtype==2)
	{
		if(!makesureFile(getFilePath()+"\\logs\\"+filename+"\\alarm"))
		{
			return;
		}
		file.Open(getFilePath()+"\\logs\\"+filename+"\\alarm",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
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
				return;
			}
			m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,6,0);
			file.Seek(226*i+2,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,1,value);
			file.Seek(226*i+34,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,2,value);
			file.Seek(226*i+66,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,32);
			m_list.SetItemText(0,3,value);
			file.Seek(226*i+98,CFile::begin);

			memset(value,0,sizeof(char)*128);
			file.Read(value,126);
			m_list.SetItemText(0,4,value);
			file.Seek(226*i+226,CFile::begin);
			////////////////
			i++;
		}
		file.Close();
	}
}

void CLogsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDS_LOGS_TIME == nIDEvent)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		if(refreshlisttimer != st.wDay)
		{
			treeinsert();
			refreshlisttimer = st.wDay;
		}
	}
	if(IDS_SHOWLOGS_TIME== nIDEvent)
	{
		if(gShowFlag==true)
		{
			CTime et;
			m_time.GetTime(et);
			CString filename;
			filename.Format("%4d%2d%2d",et.GetYear(),et.GetMonth(),et.GetDay());
			filename.Replace(" ","0");

			m_list.DeleteAllItems();
			treeloginsert(filename,1);
			treeloginsert(filename,2);
			gShowFlag = false;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CLogsDlg::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//HTREEITEM hItem=m_tree.GetSelectedItem();
	//int data = (int)m_tree.GetItemData(hItem);
	//if(data<100)
	//{
	//	HTREEITEM para=m_tree.GetParentItem(hItem);
	//	data = ((int)m_tree.GetItemData(para))*100 + data;
	//	CString path;
	//	path.Format("%d",data);
	//	path = getFilePath()+"\\logs\\"+path;
	//	_rmdir(getFilePath()+"\\logs\\"+path+"\\");
	//}
	//else
	//{
	//	data = data*100;
	//}
	*pResult = 0;
}
