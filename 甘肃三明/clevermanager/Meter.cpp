#include "StdAfx.h"
#include "Meter.h"
#include "clevermanager.h"
#include<math.h>
#define PI    3.1415
CMeter::~CMeter(void)
{
}

void CMeter::onDraw(CDC *pDC)
{
	int h=bottom-top;
	int w=right-left;
	int max = 32;
	if(type==0)
	{
		int i;
		for(i=0;i<10;i++)
		{
			int v;
			int bit;
			bit= int(pow((float)10,(float)(9-i)));
			if( v = maxvalue/bit )
			{
				if(v<3)
				{
					max = 32*bit/10;
					break;
				}
				else if(v<6)
				{
					max = 64*bit/10;
					break;
				}
				else
				{
					max = bit*10;
					break;
				}
			}
		}

		drawImagePNG(pDC,getFilePath()+"\\icon\\A.png",left,top,w,h);
		CFont font,*p_oldfont;
		font.CreateFont(
			      (h>w?w:h)/12,                        // nHeight
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
						  "ËÎÌו");                 // lpszFacename
		p_oldfont = pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(128,128,128));
		CString ti;
		ti.Format("%d",max);
		pDC->TextOutA(right-w*25/100,top+h*10/100,ti);	
		pDC->SelectObject(p_oldfont);

		pDC->MoveTo(right-w*15/100,bottom-h*15/100);
		float jiao = (float)(PI*getvalue/(max*2));
		int len = w*65/100;
		//pDC->LineTo(right-w*15/100-(int)((cos(jiao))*(w*65/100))
		//	,bottom-h*15/100-(int)((sin(jiao))*(h*65/100))  );
		Graphics graphics(pDC->GetSafeHdc()); 
		graphics.SetSmoothingMode(SmoothingModeHighQuality);
		graphics.DrawLine(&Pen(Color::Red,2),right-w*15/100,bottom-h*15/100,
			right-w*15/100-(int)((cos(jiao))*(w*65/100)),bottom-h*15/100-(int)((sin(jiao))*(h*65/100)) );
		graphics.ReleaseHDC(pDC->GetSafeHdc());
	}
	else if(type==1)
	{
		drawImagePNG(pDC,getFilePath()+"\\icon\\V.png",left,top,w,h);
		pDC->MoveTo(right-w*15/100,bottom-h*15/100);
		float jiao = (float)(PI*(float)getvalue/(2*280));
		int len = w*65/100;
		//pDC->LineTo(right-w*15/100-(int)((cos(jiao))*(w*65/100))
		//	,bottom-h*15/100-(int)((sin(jiao))*(h*65/100))  );
		Graphics graphics(pDC->GetSafeHdc()); 
		graphics.SetSmoothingMode(SmoothingModeHighQuality);
		graphics.DrawLine(&Pen(Color::Red,2),right-w*15/100,bottom-h*15/100,
			right-w*15/100-(int)((cos(jiao))*(w*65/100)),bottom-h*15/100-(int)((sin(jiao))*(h*65/100)) );
		graphics.ReleaseHDC(pDC->GetSafeHdc());
	}
	else if(type==2)
	{
		
		CBrush brush(RGB(240,64,64));
		pDC->SelectStockObject(NULL_PEN);
		pDC->SelectObject(brush);
		pDC->Ellipse(left+w*38/100,top+h*79/100,left+w*65/100,top+h*93/100);
		pDC->Rectangle(left+w*46/100,(top+h*(15+50-(getvalue/2))/100),
			left+w*58/100,top+h*88/100);
		drawImagePNG(pDC,getFilePath()+"\\icon\\T.png",left,top,w,h);
		
	}
	else if(type==3)
	{
		CBrush brush(RGB(96,96,255));
		pDC->SelectStockObject(NULL_PEN);
		pDC->SelectObject(brush);
		pDC->Rectangle(left+w*44/100,top+h*(10+80-(80*getvalue/100))/100,
			left+w*60/100,top+h*90/100);
		drawImagePNG(pDC,getFilePath()+"\\icon\\H.png",left,top,w,h);
	}
}

void CMeter::SetValue(int value)
{
	getvalue = value;
}
