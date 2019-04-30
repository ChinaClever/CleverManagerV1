// clevermanagerDoc.cpp : CclevermanagerDoc 类的实现
//

#include "stdafx.h"
#include "clevermanager.h"

#include "clevermanagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclevermanagerDoc

IMPLEMENT_DYNCREATE(CclevermanagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CclevermanagerDoc, CDocument)
END_MESSAGE_MAP()


// CclevermanagerDoc 构造/析构

CclevermanagerDoc::CclevermanagerDoc()
{
	// TODO: 在此添加一次性构造代码

}

CclevermanagerDoc::~CclevermanagerDoc()
{
}

BOOL CclevermanagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CclevermanagerDoc 序列化

void CclevermanagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CclevermanagerDoc 诊断

#ifdef _DEBUG
void CclevermanagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CclevermanagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CclevermanagerDoc 命令
