#pragma once


// CUserTabCtrl

class CUserTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CUserTabCtrl)

public:
	CUserTabCtrl();
	virtual ~CUserTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


