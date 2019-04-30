#include "StdAfx.h"
#include "DrawGraph.h"
#include "clevermanager.h"
CDrawGraph::~CDrawGraph(void)
{
}

void CDrawGraph::onDraw(CDC *pDC)
{
	drawTab(pDC);
}

//float CDrawGraph::Maxvalue(int size, float a[24][60],bool flags)
float CDrawGraph::Maxvalue(int size, float a[],bool flags)
{
	float max = 0;
	if(flags)
	{
		for (int i=0;i<size;i++)
		{
			//for(int j=0;j<60;j++)
			//{
				//if(max<a[i][j])
				//	max=a[i][j];
				if(max<a[i])
					max=a[i];
			//}
		}
	}
	else
	{
		for (int i=0;i<size;i++)
		{
			//for(int j=0;j<60;j++)
			//{
				//if(a[i][j]>0)
				//{
				//	if(max>a[i][j])
				//	{
				//		max=a[i][j];
				//	}
				//	else if(max==0)
				//	{
				//		max=a[i][j];
				//	}
				//}
				if(a[i]>0)
				{
					if(max>a[i])
					{
						max=a[i];
					}
					else if(max==0)
					{
						max=a[i];
					}
				}
			//}
		}
	}
    return max;
}

void CDrawGraph::drawTab(CDC *pDC)
{
	CRect rect(left,top,right,buttom);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(left,top,right,buttom);

	Graphics graphics(pDC->GetSafeHdc()); 
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	LinearGradientBrush lgb(Point(left+1, top+1), 
		Point(left+1,buttom-1), Color::Color(230,235,242), Color::Color(240,240,240));
	graphics.FillRectangle(&lgb, left+1,top+1,right-left-2,buttom-top-2);	

	drawFont(pDC,1,RGB(0,0,0));
	CSize fontsize= pDC->GetTextExtent("该机房总功率曲线(过去24小时)");
	pDC->TextOut(rect.Width()/2-fontsize.cx/2+left,10+top,"该机房总功率曲线(过去24小时)");
////////////
	pDC->SelectObject(&brush);
	pDC->Rectangle(left+50,top+30,right-20,buttom-40);
		LinearGradientBrush lgb1(Point(left+50, top+30), 
		Point(left+50,buttom-40), Color::Color(255,255,255), Color::Color(240,240,192));
	graphics.FillRectangle(&lgb1, left+50,top+31,right-left-70,buttom-top-70);	
/////////////
	pDC->TextOut(left+20,top+10,"kVA");
	drawFont(pDC,1,RGB(0,0,0));
	CPen  pen(PS_DOT,1,RGB(128,128,128));
	pDC->SelectObject(&pen);
	/////////////////
	int i;
	for(i=1;i<5;i++)
	{
		pDC->MoveTo(left+51,(buttom-top-70)*i/5+top+30);
		pDC->LineTo(right-left-16,(buttom-top-70)*i/5+top+30);
	}
	///////////////////
	float maxVA = Maxvalue(24,value,true);
	float unit = maxVA/4;
	for(i=0;i<5;i++)
	{
		CString xzhou;
		xzhou.Format("%.2f",unit*(5-i));
		pDC->TextOut(left+10,(buttom-top-70)*i/5+top+30,xzhou);
	}
	//////////////////
	SYSTEMTIME   ct; 
	GetLocalTime(&ct);
	for(i=0;i<24;i++)
	{
		if(!(i%4))
		{
			CString tm;
			if(ct.wHour+1+i<24)
			{
				tm.Format("%d:00",ct.wHour+1+i);
				pDC->TextOut((right-left-70)*i/24+50,buttom-30,tm);
			}
			else
			{
				tm.Format("%d:00",(ct.wHour+1+i)-24);
				pDC->TextOut((right-left-70)*i/24+50,buttom-30,tm);
			}
			if(i>0)
			{
				pDC->MoveTo((right-left-70)*i/24+55,top+30);
				pDC->LineTo((right-left-70)*i/24+55,buttom-40);
			}
		}
	}
///////////////////////////////////////////////

	//CPen  pendian(PS_SOLID,1,RGB(0,0,255));
	//pDC->SelectObject(&pendian);

	//int show_width,show_height;
	//show_width=show_height=0;
	//show_width=right-120;
	//show_height=buttom-100;

	//int index_data=0;
	//float data[1440];
	//for(i=0;i<24;i++)
	//{
	//	for(int j=0;j<60;j++)
	//	{
	//		data[index_data]=value[i][j];
	//		index_data++;
	//	}
	//}
	//for(i=1;i<1440;i++)
	//{
	//	pDC->MoveTo(left+50+(i-1)*show_width/1400,buttom-40-show_height*data[i-1]/maxVA);
	//	pDC->LineTo(left+50+i*show_width/1400,buttom-40-show_height*data[i]/maxVA);
	//}
	//int index_x=0;
	CPen  pendian(PS_SOLID,1,RGB(128,192,128));
	pDC->SelectObject(&pendian);
	for(i=0;i<24;i++)
	{
		//for(int j=0;j<59;j++)
		//{
			float h = unit*5;
//			int y1 = int(buttom-40-value[ct.wHour+i+1<24 ? ct.wHour+i+1 : (ct.wHour+i+1)-24][ct.wMinute+j+1<60 ? ct.wMinute+j+1 : (ct.wMinute+j+1)-60]/h *(buttom-top-70));
//			int y2 = int(buttom-40-value[ct.wHour+i+1<24 ? ct.wHour+i+1 : (ct.wHour+i+1)-24][ct.wMinute+j+1<60 ? ct.wMinute+j+1 : (ct.wMinute+j+1)-60]/h *(buttom-top-70)-5);
//			int ynext = int(buttom-40-value[ct.wHour+i+1<24 ? ct.wHour+i+1 : (ct.wHour+i+1)-24][ct.wMinute+j+2<60 ? ct.wMinute+j+2 : (ct.wMinute+j+2)-60]/h *(buttom-top-70));
			int y1 = int(buttom-40-value[ct.wHour+i+1<24 ? ct.wHour+i+1 : (ct.wHour+i+1)-24]/h *(buttom-top-70));
			int y2 = int(buttom-40-value[ct.wHour+i+1<24 ? ct.wHour+i+1 : (ct.wHour+i+1)-24]/h *(buttom-top-70)-5);
			int ynext = int(buttom-40-value[ct.wHour+i+2<24 ? ct.wHour+i+2 : (ct.wHour+i+2)-24]/h *(buttom-top-70));
			pDC->Ellipse((right-left-70)*i/24+55, y1 ,(right-left-70)*i/24+55+5,y2 );
//			graphics.DrawLine(&Pen(Color::Green,2),left+50+(right-left-70)*index_x/1416,y1-3,left+50+(right-left-70)*(index_x+1)/1416,ynext-3);
		    graphics.DrawLine(&Pen(Color::Green,2),(right-left-70)*i/24+58,y1-3,(right-left-70)*(i+1)/24+58,ynext-3);
			//index_x++;
		//}
	}
	graphics.ReleaseHDC(pDC->GetSafeHdc());
}
