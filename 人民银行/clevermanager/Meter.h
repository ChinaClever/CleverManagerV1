#pragma once

class CMeter
{
public:
	CMeter(int tp,int x1,int y1 ,int x2,int y2)
		:type(tp),left(x1),top(y1),right(x2),bottom(y2)
		, getvalue(0)
	{
		maxvalue =0 ;
	};
	int type;
	int left;
	int right;
	int top;
	int bottom;
	~CMeter(void);
	void onDraw(CDC *pDC);
	void SetValue(int value);
	int getvalue;
	int maxvalue;
};
