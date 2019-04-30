#pragma once
#include "clevermanager.h"
class CDrawGraph
{
public:
	CDrawGraph::CDrawGraph(int x,int y,int w, int h)
	: top(y), left(x), right(w), buttom(h)
	{
		int i;
#ifndef DEMO
		for(i=0;i<24;i++)
		{
			//for(int j=0;j<60;j++)
			//{
//				value[i][j]= 0;
				value[i] = 0;
			//}
		}
#else
		value[0] = 3.2;
		value[1] = 3.4;
		value[2] = 3;
		value[3] = 3.3;
		value[4] = 3.2;
		value[5] = 3.1;
		value[6] = 3.2;
		value[7] = 3.5;
		value[8] = 3.3;
		value[9] = 3.2;
		value[10] = 3.4;
		value[11] = 3.3;
		value[12] = 3.4;
		value[13] = 3.1;
		value[14] = 3.3;
		value[15] = 3.3;
		value[16] = 3.3;
		value[17] = 3.2;
		value[18] = 3.4;
		value[19] = 3.2;
		value[20] = 3.4;
		value[21] = 3.3;
		value[22] = 3.3;
		value[23] = 3.1;
#endif
	}
	~CDrawGraph(void);
	void onDraw(CDC *pDC);
	int top;
	int left;
	int right;
	int buttom;
	//float value[24][60];
	float value[24];
	//float Maxvalue(int size, float a[24][60],bool flags);
	float Maxvalue(int size, float a[],bool flags);
	void drawTab(CDC *pDC);
};
