#pragma once

// CSnmpObj 命令目标
struct EMS_info
{
	int EMSstatus[12];
	float EMS_cur_PDU[4][12];
	float EMS_energy_PDU[4][12];
	int EMS_vol_PDU[4][12];
	int EMS_th_PDU[4][12];
	char EMSname[32][12];
	//CString EMS_PDU_name[4][12];
	//CString EMS_door_PDU[2][12];
	//CString EMS_water_PDU[12];
	//CString EMS_smoke_PDU[12];
};
class CSnmpObj : public CObject
{
public:
	CSnmpObj(CString ip,CString com,int type,int npm_type = 0):
	ipaddr(ip),community(com),dtype(type),npmtype(npm_type)
	{
		int i,j;
		for(i=0;i<10;i++)
		{
			for(j=0;j<24;j++)
			{
				curout[i][j]=0;
				curmin[i][j]=0;
				curmax[i][j]=0;
				energy[i][j]=0;
				curalarm[i][j]=0;
			}
			for(j=0; j<3; j++)
			{
				Tcur[i][j]=0;
				Tvol[i][j]=0;
				Tcuralarm[i][j]=0;
			}
			for(j=0; j<8; j++)
			TH[i][j]=0;

			TEnergy[i][0]=0;
			TEnergy[i][1]=0;
			TEnergy[i][2]=0;
			slaveOffline[i]=0;
			offlinerecord[i]=5;//Delay
			//offlinerecord[i]=1;//Normal
			deviceOutputNum[i]=0; 
			Door1[i]="None.";
			Door2[i]="None.";
			Smoke[i]="None.";
			Water[i]="None.";
			Door1alarm[i]=0;
			Door2alarm[i]=0;
			Smokealarm[i]=0;
			Wateralarm[i]=0;
			for(int k = 0 ; k < 8 ; k ++)
			THalarm[i][k] = 0;

		}
		//Door1="";
		//Door2="";
		//Smoke="";
		//Water="";
		/////////////////////////
		for(i=0;i<12;i++)
		{
			EMSstatus[i]=0;
			for(j=0;j<4;j++)
			{
				EMS_cur_PDU[j][i] = 0;
				EMS_energy_PDU[j][i] = 0;
				EMS_vol_PDU[j][i] = 0;
				EMS_th_PDU[j][i] =0;
			}
		}
		ATS_CA=0;
		ATS_CB=0;
		ATS_VA=0;
		ATS_VB=0;
		ATS_CO=0;
		ATS_VO=0;

		ATS_CL=0;
		ATS_CH=0;
		ATS_VL=0;
		ATS_VH=0;
		ATS_status=0;
		recordtimer = 0;
		recordwMinute = 0;

	};
	virtual ~CSnmpObj();
	CString ipaddr;
	CString slavename[10];
	CString outputname[10][24];
	float curout[10][24];
	float curmin[10][24];
	float curmax[10][24];
	int curalarm[10][24];
	CString outputstatus[10][24];
	float Tcur[10][3];
	float Tcurmin[10][3];
	float Tcurmax[10][3];
	int Tcuralarm[10][3];
	float Tpower[10][3];
	float Tpf[10][3];
	int Tvol[10][3];
	int Tvolmin[10][3];
	int Tvolmax[10][3];
	int TH[10][8];//123T 456H 温度1-3 湿度1-3 温度4 湿度4
	int THmax[10][8];//123T 456H 温度1-3 湿度1-3 温度4 湿度4
	int THmin[10][8];//123T 456H 温度1-3 湿度1-3 温度4 湿度4
	int THalarm[10][8];//温度1-3 湿度1-3 温度4 湿度4
	CString Door1[10];
	CString Door2[10];
	CString Smoke[10];
	CString Water[10];
	int Door1alarm[10];
	int Door2alarm[10];
	int Smokealarm[10];
	int Wateralarm[10];
	//CString Door1;
	//CString Door2;
	//CString Smoke;
	//CString Water;
	int TEnergy[10][3];
	int status;//1正常 2报警 0离线
	CString community;
	int shousuo[10];//检索离线
	int slaveOffline[10];//离线//1正常 2报警 0离线
	int offlinerecord[10];
	CString trapinformation;
	int dtype;
	int npmtype;
	int deviceOutputNum[10];
	int energy[10][24];
	CString devicetype[10];
	////////////////////////////EMS////////////////
	int EMSstatus[12]; //1正常 2报警 0离线
	float EMS_cur_PDU[4][12];
	float EMS_energy_PDU[4][12];
	int EMS_vol_PDU[4][12];
	int EMS_th_PDU[4][12];
	CString EMSname[12];
	CString EMS_PDU_name[4][12];
	CString EMS_door_PDU[2][12];
	CString EMS_water_PDU[12];
	CString EMS_smoke_PDU[12];
	//struct EMS_info *ems_info;
	///////////////////////////ATS///////////////
	float  ATS_CA;
	float  ATS_CB;
	int  ATS_VA;
	int  ATS_VB;
	float  ATS_CO;
	int  ATS_VO;

	float  ATS_CL;
	float  ATS_CH;
	int  ATS_VL;
	int  ATS_VH;
	int  ATS_status;
	CString  ATS_name;
	CString  ATS_lin1name;
	CString  ATS_lin2name;
	CString  ATS_maininput;
	CString  ATS_currentinput;
	int recordtimer;
	int recordwMinute;
	CString ipname;
	CString cabientname[10];
	CString DCname;
	int ipTvol[3];
	float ipTcur[3];
	float ipTenergy[3];
	int ipStatus;
};


