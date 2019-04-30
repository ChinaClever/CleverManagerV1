#pragma once


// CColorHeaderCtrl

class CColorHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CColorHeaderCtrl)

public:
	CColorHeaderCtrl();
	virtual ~CColorHeaderCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


