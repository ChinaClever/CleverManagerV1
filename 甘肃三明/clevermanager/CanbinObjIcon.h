#pragma once

// CCanbinObjIcon ÃüÁîÄ¿±ê
struct canbindevice
{
	CString name;
	int start;
	int height;
	int maininputNum;
	int slaveinputNum;
};

class CCanbinObjIcon : public CObject
{
public:
	CCanbinObjIcon(int xzhou,int yzhou,int flags,CString ip1,CString ip2,int num1,int num2,int dtype):
	x(xzhou),y(yzhou),status(flags),ipMaster(ip1),ipSlave(ip2),MasterNUM(num1),SlaveNUM(num2),type(dtype)
	{
		showtitle=false;
		move = false;
		select = false;
#ifdef DEMO
		temp=rand()%(40 - 20) + 20;
#else
		temp=0;
#endif
		int i;
		for(i=0;i<24;i++)
		{
			device_master[i].name.Empty();
			device_master[i].start=0;
			device_master[i].height=0;
			
			device_master[i].slaveinputNum=0;
		}

		for(i=0;i<24;i++)
		{
			device_slave[i].name.Empty();
			device_slave[i].start=0;
			device_slave[i].height=0;
			device_slave[i].maininputNum=0;
			device_slave[i].slaveinputNum=0;
		}


		
	};
	virtual ~CCanbinObjIcon();
	void onDraw(CDC *pDC, int w, int h,bool maporth);
	void onDraw1(CDC *pDC, int w, int h,bool maporth);
	void onDraw2(CDC *pDC, int w, int h,bool maporth);
	void onDraw3(CDC *pDC, int w, int h,bool maporth);
	void onDraw4(CDC *pDC, int w, int h,bool maporth);
	int status;
	int x;
	int y;
	CString ipMaster;
	int MasterNUM;
	CString ipSlave;
	int SlaveNUM;
	bool showtitle;
	bool move;
	bool select;
	int temp;
	//canbindevice device[24];
	canbindevice device_slave[24];
	canbindevice device_master[24];
	int type;
	CString cabientname;
	CString DCname;
};


