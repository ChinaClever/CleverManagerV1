#include "stdafx.h"
#include "clevermanager.h"
#include "CanbinObjIcon.h"


// CCanbinObjIcon
CCanbinObjIcon::~CCanbinObjIcon()
{
}


// CCanbinObjIcon 成员函数

void CCanbinObjIcon::onDraw(CDC *pDC, int w, int h,bool maporth)
{
	if (type==0)
	{
		if(maporth)
		{
			if(status)
			{
				if(status==1)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==2)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==3)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==4)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==5)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));   ////黄色
					graphics.FillRectangle(&linGrBrush2,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));     //////绿色
					graphics.FillRectangle(&linGrBrush1,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==6)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));     /////红色
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==7)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==8)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==9)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==10)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==11)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==12)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==13)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==14)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==15)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20,(h*y/20)+60,w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+w/40,(h*y/20)+60,w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==16)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(0,0,0,0),Color(0,0,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
			}
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20,(h*y/20)+60,w/20,h/20);
			if(showtitle)
			{
				pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);
			}
		}
		else
		{	
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20,(h*y/20)+60,w/20,h/20);
			if(showtitle)
			{
				pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);
			}
		}
	}
	else if(type==1)
	{
		temp=0;
		//drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20,(h*y/20)+60,w*2/20,h*2/20);
		CPen  pen(PS_SOLID,1,RGB(64,64,64));
	    pDC->SelectObject(&pen);
		pDC->Rectangle(w*x/20,(h*y/20)+60,w*(x+2)/20,h*(y+2)/20+60);
		pDC->TextOutA(w*x/20+5,(h*y/20)+60+5,"Cooler");
		Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,196,196,255),Color(96,196,196,255));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w*2/20,h*2/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
	}
	else if(type==2)
	{
		temp=0;
		//drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20,(h*y/20)+60,w*2/20,h*2/20);
		CPen  pen(PS_SOLID,1,RGB(64,64,64));
	    pDC->SelectObject(&pen);
		pDC->Rectangle(w*x/20,(h*y/20)+60,w*(x+1)/20,h*(y+1)/20+60);
		pDC->TextOutA(w*x/20+5,(h*y/20)+60+5,"UPS");
		Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,196,196,255),Color(96,196,196,255));
					graphics.FillRectangle(&linGrBrush,w*x/20,(h*y/20)+60,w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
	}
	else if(type==3)
	{
		temp=0;
		drawImagePNG(pDC,getFilePath()+"\\icon\\cam.bmp",w*x/20+w/100,(h*y/20)+60+h/100,w/30,h/30);
	}
}
void CCanbinObjIcon::onDraw1(CDC *pDC, int w, int h,bool maporth)
{
	
	if (type==0)
	{
		if(maporth)
		{
			if(status)
			{
				if(status==1)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+21),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==2)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+21),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==3)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+21),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());

				}
				else if(status==4)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21,(h*y/20+21),w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21+w/40,(h*y/20+21),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==5)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21,(h*y/20+21),w/40,h/20);

					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21+w/40,(h*y/20+21),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
			}
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+21),w/20,h/20);
			if(showtitle)
			{
				
				//pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);//机柜名字
			}
		}
		else
		{	
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+21),w/20,h/20);
		}
	}
}

void CCanbinObjIcon::onDraw2(CDC *pDC, int w, int h,bool maporth)
{
	
	if (type==0)
	{
		if(maporth)
		{
			if(status)
			{
				if(status==1)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+h+61),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==2)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+h+61),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==3)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+h+61),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==4)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21,(h*y/20+h+61),w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21+w/40,(h*y/20+h+61),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==5)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21,(h*y/20+h+61),w/40,h/20);

					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21+w/40,(h*y/20+h+61),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
			}
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+h+61),w/20,h/20);
			if(showtitle)
			{
				
				//pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);//机柜名字
			}
		}
		else
		{	
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+h+61),w/20,h/20);
		}
	}
}

void CCanbinObjIcon::onDraw3(CDC *pDC, int w, int h,bool maporth)
{
	
	if (type==0)
	{
		if(maporth)
		{
			if(status)
			{
				if(status==1)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+2*h+101),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==2)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+2*h+101),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==3)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+2*h+101),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==4)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21,(h*y/20+2*h+101),w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21+w/40,(h*y/20+2*h+101),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==5)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21,(h*y/20+2*h+101),w/40,h/20);

					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21+w/40,(h*y/20+2*h+101),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
			}
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+2*h+101),w/20,h/20);
			if(showtitle)
			{
				
				//pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);//机柜名字
			}
		}
		else
		{	
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+2*h+100),w/20,h/20);
		}
	}
}

void CCanbinObjIcon::onDraw4(CDC *pDC, int w, int h,bool maporth)
{
	
	if (type==0)
	{
		if(maporth)
		{
			if(status)
			{
				if(status==1)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+3*h+141),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==2)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,0,0),Color(96,255,0,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+3*h+141),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==3)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush(Point(w*x/20,(h*y/20)),Point(w*(x+1)/20,h*(y+1)/20),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush,w*x/20+21,(h*y/20+3*h+141),w/20,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==4)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21,(h*y/20+3*h+141),w/40,h/20);

					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21+w/40,(h*y/20+3*h+141),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
				else if(status==5)
				{
					Gdiplus::Graphics graphics(pDC->GetSafeHdc());
					LinearGradientBrush linGrBrush2(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,255,255,0),Color(96,255,255,0));
					graphics.FillRectangle(&linGrBrush2,w*x/20+21,(h*y/20+3*h+141),w/40,h/20);

					LinearGradientBrush linGrBrush1(Point(w*x/20,(h*y/20)+60),Point(w*(x+1)/20,h*(y+1)/20+60),
						Color(96,0,255,0),Color(96,0,255,0));
					graphics.FillRectangle(&linGrBrush1,w*x/20+21+w/40,(h*y/20+3*h+141),w/40,h/20);
					graphics.ReleaseHDC(pDC->GetSafeHdc());
				}
			}
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+3*h+141),w/20,h/20);
			if(showtitle)
			{
				
				//pDC->TextOutA((INT)(w*(x+0.2)/20),(INT)(h*(y+0.2)/20+60),CCanbinObjIcon::cabientname);//机柜名字
			}
		}
		else
		{	
			drawImagePNG(pDC,getFilePath()+"\\icon\\canbin.png",w*x/20+21,(h*y/20+3*h+141),w/20,h/20);
		}
	}
}