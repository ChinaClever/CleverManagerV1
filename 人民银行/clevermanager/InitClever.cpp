#include "StdAfx.h"
#include "InitClever.h"
#include "clevermanager.h"
#include "SnmpObj.h"
#include "snmp_pp/snmp_pp.h"
#include "Alarm.h"
#include "MainFrm.h"
#include "CanbinObjIcon.h"
#include   <mmsystem.h>   
#pragma comment(lib,   "winmm.lib")   
#define UDSERDEF  ".1.3.6.1.4.1"
#define COP       ".30966"
#define NPM_OID  ".1.2"

////////test
#include "time.h"
////////test

//HANDLE snmpstop;
bool snmpstop;
bool threadrun;
HANDLE trapstop;
int EMSsnmpread(CSnmpObj *p_Object);
int IPPDUsnmpread(CSnmpObj *p_Object);
int ATSsnmpread(CSnmpObj *p_Object);
CInitClever::CInitClever(void)
{
}

CInitClever::~CInitClever(void)
{
}

void CInitClever::initData(void)
{
	//dataCenterWrite();
	gShowFlag = false;//自动刷新日志标志位
	int i;
	for(i=0;i<10;i++)
	{
		DCmember[i].cur1 =0;
		DCmember[i].cur2=0;
		DCmember[i].cur3=0;
		DCmember[i].vol1=0;
		DCmember[i].vol2=0;
		DCmember[i].vol3=0;
		DCmember[i].tp=0;
		DCmember[i].hm=0;
		DCmember[i].offnum=0;
		DCmember[i].alarmnum=0;
		DCmember[i].normalnum=0;
	}
	dataCenterRead();
	////////////
	//userWrite();
	memset(user,0,sizeof(userinfo)*20);
	userRead();
	///////////
	Snmpinit();
}

void CInitClever::userRead(void)
{
	CFile file;
	file.Open(getFilePath()+"\\Uinfo\\useinfo",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	file.Read((userinfo*)user,sizeof(userinfo)*20);
	file.Close();
}


void CInitClever::userWrite(void)
{
	CFile file;
	file.Open(getFilePath()+"\\Uinfo\\useinfo",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	file.Write((LPCTSTR)user,sizeof(userinfo)*20);
	file.Close();
}
void CInitClever::deviceListWrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\list",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==0)
		{
			strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
			strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

			file.Seek(132*i,CFile::begin);
			file.Write(ip,62);
			file.Seek(132*i+64,CFile::begin);
			file.Write(com,62);
			i++;
		}
	}
	file.Close();
}
void CInitClever::deviceListRead(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\list",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		i++;
		////////////
		if(strlen(ip) && strlen(com))
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,0));
		}
	}
	while(strlen(ip));

	file.Close();
}
void CInitClever::dataCenterRead()
{
	CFile file;
	file.Open(getFilePath()+"\\info\\init",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	int i;
	for(i=0;i<10;i++)
	{
		char *sp = new char[256];
		memset(sp,0,sizeof(char)*256);
		if(file.Read(sp,256)>0)
		{
			if(strlen(sp)>0)
			{
				DCmember[i].name = sp;
			}
		}
		delete[] sp;
		file.Seek(256*(i+1),CFile::begin);
	}
	file.Close();
}

void CInitClever::dataCenterWrite()
{
	CFile file;
	file.Open(getFilePath()+"\\info\\init",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	int i;
	for(i=0;i<10;i++)
	{
		//CString name;
		//name.Format("机房%d",i+1);
		int len = DCmember[i].name.GetLength();
		file.Write((LPCTSTR)DCmember[i].name,len);
		file.Seek(256*(i+1),CFile::begin);
	}
	file.Close();
}
///////////////////////////////////
void CInitClever::Snmpinit(void)
{
	//增加其他设备类型
	//snmpobj.AddTail(new CSnmpObj("192.168.1.165","public",类型//0为NPMIII));
	//snmpobj.AddTail(new CSnmpObj("192.168.1.171","public"));
	//snmpobj.AddTail(new CSnmpObj("192.168.1.173","public",0,1));//NPM-IV蜂鸣检查
	AddotherDevice();
	deviceListRead();
	NPMIVdeviceread();
	NPMVdeviceread();
	MSPDUdeviceread();
	snmpstart();
}
void CInitClever::Exitsnmp(void)
{
	if(trapstop != INVALID_HANDLE_VALUE)
	{
		SetEvent(trapstop);
	}
	while(trapstop != INVALID_HANDLE_VALUE)
	{
		Sleep(10);
	}
	//////////////////////////
	threadrun = false;
	while(snmpstop)
	{
		Sleep(10);
	}
	//////////////////////////
	POSITION pos = snmpobj.GetHeadPosition ();
	while (pos != NULL)
	{
		//CSnmpObj *p_Object = (CSnmpObj *)(snmpobj.GetNext (pos));
		//if(p_Object->ems_info)
		//{
		//	delete(p_Object->ems_info);
		//	p_Object->ems_info=NULL;
		//}
		//delete p_Object;
		delete snmpobj.GetNext (pos);
	}
	snmpobj.RemoveAll ();
}
void insertNPM(CString oid,CString value,CSnmpObj *p_Object)
{
	CString rec = oid;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[9].GetLength())
	{
		int deindex = (atoi(zd[9]))-1;
		if(deindex<0 || deindex>9)
		{
			return;
		}
		///////////////////////////////////////////////
		if(zd[10] == "2"&&zd[11] == "0")
		{
			p_Object->slavename[deindex] = value;
			p_Object->shousuo[deindex]=1;
		}
		else if(zd[10] == "3")
		{

			p_Object->devicetype[deindex] = changevalue(value);
			//value.Replace(" ","");
			//p_Object->devicetype[deindex].Left(6);
		}
		else if(zd[10] == "6")
		{
			p_Object->Tcur[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "7")
		{
			p_Object->Tcur[deindex][1] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "8")
		{
			p_Object->Tcur[deindex][2] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "9")
		{
			p_Object->Tvol[deindex][0] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "10")
		{
			p_Object->Tvol[deindex][1] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "11")
		{
			p_Object->Tvol[deindex][2] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "12")
		{
			p_Object->TEnergy[deindex][0] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "13")
		{
			p_Object->energy[deindex][0] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "14")
		{
			p_Object->energy[deindex][1] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "15")
		{
			p_Object->energy[deindex][2] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "16")
		{
			p_Object->energy[deindex][3] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "17")
		{
			p_Object->energy[deindex][4] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "18")
		{
			p_Object->energy[deindex][5] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "19")
		{
			p_Object->TH[deindex][0] = atoi(value=="Not found"?"0":value);
		}
		else if(zd[10] == "20")
		{
			p_Object->TH[deindex][3] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "21")
		{
			p_Object->TH[deindex][1] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "22")
		{
			p_Object->TH[deindex][4] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "23")
		{
			p_Object->TH[deindex][2] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "24")
		{
			p_Object->TH[deindex][5] = atoi(value =="Not found"?"0":value);
		}
		//////////////////////////////////////
		else if(zd[12] == "3"&&zd[13].GetLength())
		{
			p_Object->outputname[deindex][atoi(zd[13])-1] = value;
		}
		else if(zd[12] == "4"&&zd[13].GetLength())
		{
			p_Object->outputstatus[deindex][atoi(zd[13])-1] = value;
		}
		else if(zd[12] == "5"&&zd[13].GetLength())
		{
			p_Object->curout[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
		else if(zd[12] == "6"&&zd[13].GetLength())
		{
			p_Object->curmin[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
		else if(zd[12] == "7"&&zd[13].GetLength())
		{
			p_Object->curmax[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
	}
}
bool snmpRead(CSnmpObj *p_Object)
{
	bool rec =false;
	if(p_Object->dtype==0)
	{
		int requests = 0;        
		int objects  = 0;
		//int i,j;
		int i;
		for(i=0;i<10;i++)
		{
			p_Object->shousuo[i]=0;
		}
		if(p_Object->ipaddr.GetLength()>16)
		{
			return rec;
		}
		Snmp::socket_startup();
		char ipaddr[255];
		memset(ipaddr,0,sizeof(char)*255);
		strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
		UdpAddress address(ipaddr);     
		if ( !address.valid()) {          
			Snmp::socket_cleanup();
			return rec;
		}
		char oidnum[255];
		memset(oidnum,0,sizeof(char)*255);
		strcat_s(oidnum,sizeof(char)*254,UDSERDEF);
		strcat_s(oidnum,sizeof(char)*254-strlen(UDSERDEF),COP);
		///////NPM///////
		strcat_s(oidnum,sizeof(char)*254-strlen(UDSERDEF)-strlen(COP),NPM_OID);
		////////////
		Oid oid(oidnum); 
		if ( !oid.valid()) {            
			Snmp::socket_cleanup();
			return rec;
		}

		char com[255];
		memset(com,0,sizeof(char)*255);
		strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);

		snmp_version version=version2c;                  
		int retries=1;                                  
		int timeout=200;                                
		u_short port=161;                              
		OctetStr community(com);                  
		bool subtree = true;

		int status;
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if ( status != SNMP_CLASS_SUCCESS) {
			Snmp::socket_cleanup();
			return rec;
		}
		Pdu pdu;                               
		Vb vb;                                
		vb.set_oid( oid);                    
		pdu += vb;                            

		address.set_port(port);
		CTarget ctarget( address);             

		ctarget.set_version( version);         
		ctarget.set_retry( retries);           
		ctarget.set_timeout( timeout);         
		ctarget.set_readcommunity( community); 

		SnmpTarget *target;

		target = &ctarget;
		while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
		{
			Sleep(1);
			if(!threadrun)
			{
				Snmp::socket_cleanup();
				return 0;
			}
			requests++;
			for ( int z=0;z<pdu.get_vb_count(); z++) {
				pdu.get_vb( vb,z);

				Oid tmp;
				vb.get_oid(tmp);
				if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2)
						{
							p_Object->slaveOffline[i]=1;
						}
					}
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						}
					}
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
				objects++; 
				if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
				{
					CString id = vb.get_printable_oid();
					CString id_vlaue = vb.get_printable_value();
					insertNPM(id,id_vlaue,p_Object);
					rec=true;
				}
				else 
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2)
						{
							p_Object->slaveOffline[i]=1;
						}
					}
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						}
					}
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
			}
			pdu.set_vblist(&vb, 1);
		}
		////////////////////////
		Snmp::socket_cleanup();
		p_Object->status=0;
		for(i=0;i<10;i++)
		{
			p_Object->slaveOffline[i] =0;
		}
	}
	return rec;
}
void insertNPM_IV(CString oid,CString value,CSnmpObj *p_Object)
{
	CString rec = oid;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[9].GetLength())
	{
		int deindex = (atoi(zd[9]))-1;
		if(deindex<0 || deindex>9)
		{
			return;
		}
		///////////////////////////////////////////////
		if(zd[10] == "2"&&zd[11] == "0")
		{
			p_Object->slavename[deindex] = value;
			p_Object->shousuo[deindex]=1;
		}
		else if(zd[10] == "3")
		{

			p_Object->devicetype[deindex] = value;
			//value.Replace(" ","");
			//p_Object->devicetype[deindex].Left(6);
		}
		else if(zd[10] == "4")
		{
			if(p_Object->devicetype[deindex].GetLength())
				p_Object->devicetype[deindex] += value;
			//value.Replace(" ","");
			//p_Object->devicetype[deindex].Left(6);
		}
		else if(zd[10] == "7")
		{
			p_Object->Tcur[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "8")
		{
			p_Object->Tcur[deindex][1] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "9")
		{
			p_Object->Tcur[deindex][2] = (float)atof(value.GetLength()>4?"0":value);
		}
		else if(zd[10] == "10")
		{
			p_Object->Tvol[deindex][0] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "11")
		{
			p_Object->Tvol[deindex][1] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "12")
		{
			p_Object->Tvol[deindex][2] = atoi(value.GetLength()>5?"0":value);
		}
		else if(zd[10] == "16")
		{
			p_Object->TEnergy[deindex][0] = atoi(value.GetLength()>9?"0":value);
		}
		else if(zd[10] == "22")
		{
			p_Object->TH[deindex][0] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "23")
		{
			p_Object->TH[deindex][3] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "24")
		{
			p_Object->TH[deindex][1] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "25")
		{
			p_Object->TH[deindex][4] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "26")
		{
			p_Object->TH[deindex][2] = atoi(value =="Not found"?"0":value);
		}
		else if(zd[10] == "27")
		{
			p_Object->TH[deindex][5] = atoi(value =="Not found"?"0":value);
		}
		//////////////////////////////////////
		else if(zd[12] == "3"&&zd[13].GetLength())
		{
			p_Object->outputname[deindex][atoi(zd[13])-1] = value;
		}
		else if(zd[12] == "4"&&zd[13].GetLength())
		{
			p_Object->outputstatus[deindex][atoi(zd[13])-1] = value;
		}
		else if(zd[12] == "5"&&zd[13].GetLength())
		{
			p_Object->curout[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
		else if(zd[12] == "9"&&zd[13].GetLength())
		{
			p_Object->curmin[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
		else if(zd[12] == "10"&&zd[13].GetLength())
		{
			p_Object->curmax[deindex][atoi(zd[13])-1] = (float)atof(value);
		}
		else if(zd[12] == "7"&&zd[13].GetLength())
		{
			p_Object->energy[deindex][atoi(zd[13])-1] = atoi(value);
		}  
	}
}
int NPM_IVsnmpread(CSnmpObj *p_Object)
{
	bool rec =false;
	if(p_Object->dtype==0)
	{
		int requests = 0;        
		int objects  = 0;
		//int i,j;
		int i;
		for(i=0;i<10;i++)
		{
			p_Object->shousuo[i]=0;
		}
		if(p_Object->ipaddr.GetLength()>16)
		{
			return rec;
		}
		Snmp::socket_startup();
		char ipaddr[255];
		memset(ipaddr,0,sizeof(char)*255);
		strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
		UdpAddress address(ipaddr);     
		if ( !address.valid()) {          
			Snmp::socket_cleanup();
			return rec;
		}
		Oid oid("1.3.6.1.4.1.30966.4"); 
		if ( !oid.valid()) {            
			Snmp::socket_cleanup();
			return rec;
		}

		char com[255];
		memset(com,0,sizeof(char)*255);
		strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);

		snmp_version version=version2c;                  
		int retries=1;                                  
		int timeout=200;                                
		u_short port=161;                              
		OctetStr community(com);                  
		bool subtree = true;

		int status;
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if ( status != SNMP_CLASS_SUCCESS) {
			Snmp::socket_cleanup();
			return rec;
		}
		Pdu pdu;                               
		Vb vb;                                
		vb.set_oid( oid);                    
		pdu += vb;                            

		address.set_port(port);
		CTarget ctarget( address);             

		ctarget.set_version( version);         
		ctarget.set_retry( retries);           
		ctarget.set_timeout( timeout);         
		ctarget.set_readcommunity( community); 

		SnmpTarget *target;

		target = &ctarget;
		while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
		{
			Sleep(1);
			if(!threadrun)
			{
				Snmp::socket_cleanup();
				return 0;
			}
			requests++;
			for ( int z=0;z<pdu.get_vb_count(); z++) {
				pdu.get_vb( vb,z);

				Oid tmp;
				vb.get_oid(tmp);
				if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2)
						{
							p_Object->slaveOffline[i]=1;
						}
					}
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						}
					}
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
				objects++; 
				if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
				{
					CString id = vb.get_printable_oid();
					CString id_vlaue = vb.get_printable_value();
					insertNPM_IV(id,id_vlaue,p_Object);
					rec=true;
				}
				else 
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2)
						{
							p_Object->slaveOffline[i]=1;
						}
					}
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						}
					}
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
			}
			pdu.set_vblist(&vb, 1);
		}
		////////////////////////
		Snmp::socket_cleanup();
		p_Object->status=0;
		for(i=0;i<10;i++)
		{
			p_Object->slaveOffline[i] =0;
		}
	}
	return rec;
}

void insertMS_PDU(CString oid,CString value,CSnmpObj *p_Object)
{
	CString rec = oid;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[9].GetLength())
	{
		int deindex = 0;
		if(deindex<0 || deindex>9)
		{
			return;
		}
		///////////////////////////////////////////////
		if(zd[9] == "1")
		{
			if(zd[10] == "1")
			{
				p_Object->slavename[deindex] = value;
				p_Object->shousuo[deindex]=1;
				if(	p_Object->status!=2)
				{
					p_Object->status=1;
					p_Object->slaveOffline[0]=1;
				}
				else if(p_Object->status==2)
				{
					p_Object->status=2;
					p_Object->slaveOffline[0]=2;
				}
			}
			else if(zd[10] == "2")
			{
				p_Object->devicetype[deindex].Format("MS-PDU-%s",value);
			}
			else if(zd[10] == "3")
			{
				p_Object->deviceOutputNum[deindex] = atoi(value);
			}
		}
		else if(zd[9] == "2")
		{
			if(zd[10] == "1")
			{
				p_Object->Tvol[deindex][0] = atoi(value.GetLength()>5?"0":value);
			}
			else if(zd[10] == "2")
			{
				p_Object->Tcur[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
			}
		}
		else if(zd[9] == "3")
		{
			p_Object->curout[deindex][atoi(zd[10])-1] = (float)atof(value);
		}
		else if(zd[9] == "4")
		{
			p_Object->outputstatus[deindex][atoi(zd[10])-1] = value;
			if(atoi(zd[10])<=p_Object->deviceOutputNum[deindex])
				p_Object->outputname[deindex][atoi(zd[10])-1].Format("Output%d",atoi(zd[10]));
		}
	}
}

int MS_PDUsnmpread(CSnmpObj *p_Object)
{
	bool rec =false;
	if(p_Object->dtype==0)
	{
		int requests = 0;        
		int objects  = 0;
		//int i,j;
		p_Object->shousuo[0]=0;

		if(p_Object->ipaddr.GetLength()>16)
		{
			return rec;
		}
		Snmp::socket_startup();
		char ipaddr[255];
		memset(ipaddr,0,sizeof(char)*255);
		strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
		UdpAddress address(ipaddr);     
		if ( !address.valid()) {          
			Snmp::socket_cleanup();
			return rec;
		}
		Oid oid("1.3.6.1.4.1.30966.10.2"); 
		if ( !oid.valid()) {            
			Snmp::socket_cleanup();
			return rec;
		}

		char com[255];
		memset(com,0,sizeof(char)*255);
		strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);

		snmp_version version=version1;                  
		int retries=1;                                  
		int timeout=200;                                
		u_short port=161;                              
		OctetStr community(com);                  
		bool subtree = true;

		int status;
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if ( status != SNMP_CLASS_SUCCESS) {
			Snmp::socket_cleanup();
			return rec;
		}
		Pdu pdu;                               
		Vb vb;                                
		vb.set_oid( oid);                    
		pdu += vb;                            

		address.set_port(port);
		CTarget ctarget( address);             

		ctarget.set_version( version);         
		ctarget.set_retry( retries);           
		ctarget.set_timeout( timeout);         
		ctarget.set_readcommunity( community); 

		SnmpTarget *target;

		target = &ctarget;
		while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
		{
			Sleep(1);
			if(!threadrun)
			{
				Snmp::socket_cleanup();
				return 0;
			}
			requests++;
			for ( int z=0;z<pdu.get_vb_count(); z++) {
				pdu.get_vb( vb,z);

				Oid tmp;
				vb.get_oid(tmp);
				if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					if(p_Object->slaveOffline[0] != 2)
					{
						p_Object->slaveOffline[0]=1;
					}

					//////////////////////////////////////
					bool statusflags = false;
					if(p_Object->slaveOffline[0] == 2)
					{
						statusflags = true;
					}

					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
				objects++; 
				if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
				{
					CString id = vb.get_printable_oid();
					CString id_vlaue = vb.get_printable_value();
					insertMS_PDU(id,id_vlaue,p_Object);
					rec=true;
				}
				else 
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					if(p_Object->slaveOffline[0] != 2)
					{
						p_Object->slaveOffline[0]=1;
					}

					//////////////////////////////////////
					bool statusflags = false;
					if(p_Object->slaveOffline[0] == 2)
					{
						statusflags = true;
					}

					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}
			}
			pdu.set_vblist(&vb, 1);
		}
		////////////////////////
		Snmp::socket_cleanup();
		//p_Object->status=0;
		//p_Object->slaveOffline[0] =0;

	}
	return rec;
}

void insertNPM_V(CString oid,CString value,CSnmpObj *p_Object)
{
	CString rec = oid;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[8].GetLength())
	{
		int deindex = (atoi(zd[8]))-1;
		if(deindex<0 || deindex>9)
		{
			return;
		}
		///////////////////////////////////////////////
		if(zd[9] == "1"&&zd[10] == "1")
		{
			p_Object->slavename[deindex] = value;
			if(value == "--") return;
			p_Object->shousuo[deindex]=1;
		}
		else if(zd[10] == "2")
		{
			if(value=="A")
				p_Object->devicetype[deindex] = "NPM-V-A";
			else if(value=="B")
				p_Object->devicetype[deindex] = "NPM-V-B";
			else if(value=="C")
				p_Object->devicetype[deindex] = "NPM-V-C";
			else if(value=="D")
				p_Object->devicetype[deindex] = "NPM-V-D";
			else if(value=="E")
				p_Object->devicetype[deindex] = "NPM-V-E";
			else if(value=="F")
				p_Object->devicetype[deindex] = "NPM-V-A";
			else if(value=="G")
				p_Object->devicetype[deindex] = "NPM-V-B";
			else if(value=="J")
				p_Object->devicetype[deindex] = "NPM-V-C";
			else if(value=="K")
				p_Object->devicetype[deindex] = "NPM-V-D";
			else if(value=="I")
				p_Object->devicetype[deindex] = "NPM-V-E";
		}
        if(p_Object->devicetype[deindex] == "NPM-V-A"||p_Object->devicetype[deindex] == "NPM-V-B"||p_Object->devicetype[deindex] == "NPM-V-C"||p_Object->devicetype[deindex] == "NPM-V-D")
		{
				if(zd[10] == "3")
				{
					if(zd[11] == "1")//t_power
					{
						p_Object->Tpower[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "2")//pf
					{
						p_Object->Tpf[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "3")
					{
						/*if(value.Mid(1,1)==".")
						{
							p_Object->TEnergy[deindex][0] =atoi(value.Left(1))*10+atoi(value.Right(1));
						}
						else if(value.Mid(2,1)==".")
						{
							p_Object->TEnergy[deindex][0] =atoi(value.Left(2))*10+atoi(value.Right(1));
						}*/
						p_Object->TEnergy[deindex][0] =(int)(atof(value)*10);

						
					}
					else if(zd[11] == "4")
					{
						p_Object->Tcur[deindex][0] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "5")
					{
						p_Object->Tvol[deindex][0] = atoi(value.GetLength()>5?"0":value);
					}
					else if(zd[11] == "6")
					{
						p_Object->Tcurmin[deindex][0] = (float)atof(value);
					}
					else if(zd[11] == "7")
					{
						p_Object->Tcurmax[deindex][0] = (float)atof(value);
					}
					else if(zd[11] == "8")
					{
						p_Object->Tvolmin[deindex][0] = atoi(value);
					}
					else if(zd[11] == "9")
					{
						p_Object->Tvolmax[deindex][0] = atoi(value);
					}
				}
				else if(zd[10] == "4")
				{
					if(zd[11] == "1")//t_power
					{
						p_Object->Tpower[deindex][1] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "2")//pf
					{
						p_Object->Tpf[deindex][1] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "3")
					{
						p_Object->TEnergy[deindex][1] = (int)(atof(value)*10);
					}
					else if(zd[11] == "4")
					{
						p_Object->Tcur[deindex][1] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "5")
					{
						p_Object->Tvol[deindex][1] = atoi(value.GetLength()>5?"0":value);
					}
					else if(zd[11] == "6")
					{
						p_Object->Tcurmin[deindex][1] = (float)atof(value);
					}
					else if(zd[11] == "7")
					{
						p_Object->Tcurmax[deindex][1] = (float)atof(value);
					}
					else if(zd[11] == "8")
					{
						p_Object->Tvolmin[deindex][1] = atoi(value);
					}
					else if(zd[11] == "9")
					{
						p_Object->Tvolmax[deindex][1] = atoi(value);
					}
				}
				else if(zd[10] == "5")
				{
					if(zd[11] == "1")//t_power
					{
						p_Object->Tpower[deindex][2] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "2")//pf
					{
						p_Object->Tpf[deindex][2] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "3")
					{
						p_Object->TEnergy[deindex][2] = (int)(atof(value)*10);
					}
					else if(zd[11] == "4")
					{
						p_Object->Tcur[deindex][2] = (float)atof(value.GetLength()>4?"0":value);
					}
					else if(zd[11] == "5")
					{
						p_Object->Tvol[deindex][2] = atoi(value.GetLength()>5?"0":value);
					}
					else if(zd[11] == "6")
					{
						p_Object->Tcurmin[deindex][2] = (float)atof(value);
					}
					else if(zd[11] == "7")
					{
						p_Object->Tcurmax[deindex][2] = (float)atof(value);
					}
					else if(zd[11] == "8")
					{
						p_Object->Tvolmin[deindex][2] = atoi(value);
					}
					else if(zd[11] == "9")
					{
						p_Object->Tvolmax[deindex][2] = atoi(value);
					}
				}
				else if(zd[10] == "6")//tem hum
				{
					if(zd[11]=="1")
						p_Object->TH[deindex][0] = atoi(value =="Not found"?"0":value);
					else if(zd[11]=="2")
						p_Object->TH[deindex][1] = atoi(value =="Not found"?"0":value);
					else if(zd[11]=="3")
						p_Object->TH[deindex][2] = atoi(value =="Not found"?"0":value);
					//else if(zd[11]=="4")

					else if(zd[11]=="5")
						p_Object->TH[deindex][3] = atoi(value =="Not found"?"0":value);
					else if(zd[11]=="6")
						p_Object->TH[deindex][4] = atoi(value =="Not found"?"0":value);
					else if(zd[11]=="7")
						p_Object->TH[deindex][5] = atoi(value =="Not found"?"0":value);
					//else if(zd[11]=="8")

					else if(zd[11]=="9")
					{
						if(value&&value.GetLength())
							p_Object->Door1=value;
						else
							p_Object->Door1="Not found";
					}
					else if(zd[11]=="10")
					{
						if(value&&value.GetLength())
							p_Object->Door2=value;
						else
							p_Object->Door2 ="Not found";
					}
					else if(zd[11]=="11")
					{
						if(value&&value.GetLength())
							p_Object->Water =value;
						else
							p_Object->Smoke ="Not found"?"0":value;
					}
					else if(zd[11]=="12")
					{
						if(value&&value.GetLength())
							p_Object->Water =value;
						else
							p_Object->Water ="Not found";
					}
					
				}
				else if(zd[10]=="7")
				{
					//p_Object->deviceOutputNum[deindex]= value;
					int ret = atoi(value =="Not found"?"0":value);
					if(ret >= 16)
					{
						if(!deindex)
						p_Object->deviceOutputNum[deindex] = ret;
						else if(p_Object->deviceOutputNum[0])
						p_Object->deviceOutputNum[deindex] = p_Object->deviceOutputNum[0];
					}
					else
					{
						p_Object->deviceOutputNum[deindex] = ret;
					}
				}
				else if(zd[10] == "8")
				{
					p_Object->outputname[deindex][atoi(zd[11])-1] = value;
				}
				else if(zd[10]=="9")
				{
					p_Object->outputstatus[deindex][atoi(zd[11])-1] = value;
				}
				else if(zd[10]=="10")
				{
					float  tempValue = (float)atof(value);
					float  value = 0.0;
					if(tempValue >  0.1)
					{
						if(p_Object->curout[deindex][atoi(zd[11])-1] == 0)
						{
							value = 0.05;
						}
						else if( (p_Object->curout[deindex][atoi(zd[11])-1] - 0.05) <= 1e-9 )
						{
							value = 0.1;
						}
						else
						{
							value = tempValue;
						}
					}
					p_Object->curout[deindex][atoi(zd[11])-1] = value;
				}
				else if(zd[10]=="11")
				{
					p_Object->curmin[deindex][atoi(zd[11])-1] = (float)atof(value);
				}
				else if(zd[10]=="12")
				{
					p_Object->curmax[deindex][atoi(zd[11])-1] = (float)atof(value);
				}
				else if(zd[10]=="13")
				{
					/*if(value.Mid(1,1)==".")
					{
						p_Object->energy[deindex][atoi(zd[11])-1] =atoi(value.Left(1))*10+atoi(value.Right(1));
					}
					else if(value.Mid(2,1)==".")
					{
						p_Object->energy[deindex][atoi(zd[11])-1] =atoi(value.Left(2))*10+atoi(value.Right(1));
					}*/
					p_Object->energy[deindex][atoi(zd[11])-1] =(int)(atof(value)*10);
				}
		  }
     }
}

int NPM_Vsnmpread(CSnmpObj *p_Object)
{
	bool rec =false;
	if(p_Object->dtype==0)
	{
		int requests = 0;        
		int objects  = 0;
		//int i,j;
		int i;
		for(i=0;i<10;i++)
		{
			p_Object->shousuo[i]=0;
		}
		if(p_Object->ipaddr.GetLength()>16)
		{
			return rec;
		}
		Snmp::socket_startup();
		char ipaddr[255];
		memset(ipaddr,0,sizeof(char)*255);
		strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
		UdpAddress address(ipaddr);     
		if ( !address.valid()) {          
			Snmp::socket_cleanup();
			return rec;
		}
		Oid oid("1.3.6.1.4.1.30966.5"); 
		if ( !oid.valid()) {            
			Snmp::socket_cleanup();
			return rec;
		}

		char com[255];
		memset(com,0,sizeof(char)*255);
		strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);

		snmp_version version=version2c;                  
		int retries=1;                                  
		int timeout= 1200;                                
		u_short port=161;                              
		OctetStr community(com);                  
		bool subtree = true;

		int status;
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if ( status != SNMP_CLASS_SUCCESS) {
			Snmp::socket_cleanup();
			return rec;
		}
		Pdu pdu;                               
		Vb vb;                                
		vb.set_oid( oid);                    
		pdu += vb;                            

		address.set_port(port);
		CTarget ctarget( address);             

		ctarget.set_version( version);         
		ctarget.set_retry( retries);           
		ctarget.set_timeout( timeout);         
		ctarget.set_readcommunity( community); 

		SnmpTarget *target;

		target = &ctarget;
		while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
		{			
			Sleep(5);

			if(!threadrun)
			{
				Snmp::socket_cleanup();
				return 0;
			}
			requests++;

			//GenAddress addr;
			//pdu.get_v1_trap_address(addr);
			//if(strcmp(ipaddr, addr.address_buffer))  continue;

			for ( int z=0;z<pdu.get_vb_count(); z++) {
				pdu.get_vb( vb,z);

				Oid tmp;
				vb.get_oid(tmp);				
				if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2 && p_Object->shousuo[i]==1)
						{
							p_Object->slaveOffline[i] = 1;
						}
						else if( /* p_Object->slaveOffline[i] != 2 && */p_Object->shousuo[i]==0)
						{
							p_Object->slaveOffline[i]=0;
						}
					}
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						    //PlaySound("ALARM1.WAV",   NULL,  SND_FILENAME | SND_ASYNC);
						}
					}
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}//if subtree
				objects++; 
				if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
				{
					CString id = vb.get_printable_oid();
					CString id_vlaue = vb.get_printable_value();
					if(!id_vlaue.IsEmpty())
					insertNPM_V(id,id_vlaue,p_Object);
					rec=true;
				}
				else 
				{
					Snmp::socket_cleanup();
					///////////////////////////////////////
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] != 2&&p_Object->shousuo[i]==1)
						{
							p_Object->slaveOffline[i]=1;
						}
						else if(p_Object->slaveOffline[i] != 2&&p_Object->shousuo[i]==0)
						{
							p_Object->slaveOffline[i]=0;
						}
					}//for
					//////////////////////////////////////
					bool statusflags = false;
					for(i=0;i<10;i++)
					{
						if(p_Object->slaveOffline[i] == 2)
						{
							statusflags = true;
						}
					}//for
					if(statusflags)
					{
						p_Object->status=2;
					}
					else
					{
						p_Object->status=1;
					}
					return rec;
				}//else
			}//for pdu.get_vb_count()
			pdu.set_vblist(&vb, 1);
		}//while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
		////////////////////////
		Snmp::socket_cleanup();
		p_Object->status=0;
		for(i=0;i<10;i++)
		{
			p_Object->slaveOffline[i] =0;
		}
	}//if(p_Object->dtype==0)
	return rec;
}

CString ReadItem(CSnmpObj * p_Object,Oid itemoid)
{
	CString ret;
	
	Snmp::socket_startup();
	char ipaddr[255];
	memset(ipaddr,0,sizeof(char)*255);
	strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
	UdpAddress address(ipaddr);     
	if ( !address.valid()) {          
		Snmp::socket_cleanup();
		return NULL;
	}
	
	if ( !itemoid.valid()) {            
		Snmp::socket_cleanup();
		return NULL;
	}

	char com[255];
	memset(com,0,sizeof(char)*255);
	strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);

	snmp_version version=version1;                  
	int retries=1;                                  
	int timeout=200;                                
	u_short port=161;                              
	OctetStr community(com);                  
	bool subtree = true;

	int status;
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if ( status != SNMP_CLASS_SUCCESS) {
		Snmp::socket_cleanup();
		return NULL;
	}
	
	Pdu pdu;                               
	Vb vb;                                
	vb.set_oid(itemoid);//"1.3.6.1.4.1.30966.10.2.3.1.0"                  
	pdu += vb;                            
	address.set_port(port);
	CTarget ctarget(address);             
	ctarget.set_version(version);         
	ctarget.set_retry(retries);           
	ctarget.set_timeout(timeout);         
	ctarget.set_readcommunity(community); 
	SnmpTarget *target;
	target = &ctarget;
	if(( status = snmp.get( pdu,*target))== SNMP_CLASS_SUCCESS) 
	{
		for ( int z=0;z<pdu.get_vb_count(); z++) 
		{
			pdu.get_vb( vb,z);
			Oid tmp;
			vb.get_oid(tmp);
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				ret = vb.get_printable_value();
				break;
			}
			else 
			{
				Snmp::socket_cleanup();
				return NULL;
			}
		}
	}
	////////////////////////
	Snmp::socket_cleanup();
	return ret;
}

UINT CInitClever::snmprun(LPVOID p)
{
	snmpstop = true;
	static int count = 0;
	while(threadrun)
	{
		Sleep(1);
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			////////test
			//CString teststr;
			//clock_t start_0,start_1,start_2,finish;
			//bool ret = false;
			//bool elseret = false;
			////////test

			CString ip1,ip2;
			ip1 = currentIP1;
			ip2 = currentIP2;

			if(ip1.GetLength() || ip2.GetLength())
			{
				POSITION posnow = snmpobj.GetTailPosition ();
				while (posnow != NULL)
				{
					Sleep(25);
					CSnmpObj *p_Objectnow = NULL;
					p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
					if(p_Objectnow->ipaddr == ip1 && ip1.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==2)
					{

						//start_0 = clock();////////test
						//ret = true;////////test

						if(count < 5) //Delay
						{
							count ++;//Delay
							break;//Delay
						}
						count = 0;
						
						if(!NPM_Vsnmpread(p_Objectnow))
						{
							p_Objectnow->status = 0;
							for(int s=0;s<10;s++)
							{
								p_Objectnow->slaveOffline[s] =0;
							}//for
						}//if
						if(!threadrun)
						{
							snmpstop = false;
							return 0;
						}//if
					}//if p_Objectnow
					else if(p_Objectnow->ipaddr == ip2 && ip2.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==2)
					{

						//start_1 = clock();////////test
						//elseret = true;//test

						if(count < 5) //Delay
						{
							count ++;//Delay
							break;//Delay
						}
						count = 0;
						
						if(!NPM_Vsnmpread(p_Objectnow))
						{
							p_Objectnow->status = 0;
							for(int s=0;s<10;s++)
							{
								p_Objectnow->slaveOffline[s] =0;
							}
						}
						if(!threadrun)
						{
							snmpstop = false;
							return 0;
						}
					}//else if
				}//while (posnow != NULL)
			} //if(ip1.GetLength() || ip2.GetLength())  
			////////////////////////////////
		//while (pos != NULL)
		//{
			Sleep(100);
			CSnmpObj *p_Object = NULL;
			POSITION oldpos = NULL;
			oldpos = pos;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			CString ip = p_Object->ipaddr;
			if(ip.GetLength()  && p_Object->dtype==0 && p_Object->npmtype==2)
			{
				//start_2 = clock();//test
				if(!NPM_Vsnmpread(p_Object))
				{
					p_Object->status = 0;
					for(int s=0;s<10;s++)
					{
						p_Object->slaveOffline[s] =0;
					}
				}

				////////test
				//finish = clock();
				//if(ret){
				//teststr.Format("currentIP1:%s time0:%f seconds; ",currentIP1,(double)(finish-start_0)/CLOCKS_PER_SEC);
				//teststr="";}
				//if(elseret)
				//{
				//	teststr.Format("currentIP2:%s time1:%f seconds; ",currentIP2,(double)(finish-start_1)/CLOCKS_PER_SEC);
				//	teststr="";
				//}
				//teststr.Format("IP:%s time2:%f seconds; ",ip,(double)(finish-start_2)/CLOCKS_PER_SEC);
				//teststr="";
				////////test

				if(!threadrun)
				{
					snmpstop = false;
					return 0;
				}
			}
			else
			{
				//delete p_Object;
				//snmpobj.RemoveAt(oldpos);
				/*struct snmpobjmem
				{
				CSnmpObj *p_obj;
				POSITION positon;
				};
				struct snmpobjmem *snmpobj_mem = NULL;
				snmpobj_mem = new (struct snmpobjmem);
				snmpobj_mem->p_obj = p_Object;
				snmpobj_mem->positon = oldpos;
				CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
				::SendMessage(p_maimframe->GetSafeHwnd(),IDS_DEL_OBJ_MEM,(WPARAM)snmpobj_mem,NULL); */
			}
		}//while (pos != NULL)
	}//while(threadrun)
	snmpstop = false;
	return 0;
}

void callback( int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd)
{
	Vb nextVb;
	int pdu_error =0;
	GenAddress addr;

	target.get_address(addr);
	UdpAddress from(addr);
	CString ip,ipaddr;
	ip = from.get_printable();
	ipaddr=ip.Left(ip.Find("/"));
	pdu_error = pdu.get_error_status();
	if (pdu_error){
		return;
	}
	Oid id;
	pdu.get_notify_id(id);
	CString zd = id.get_printable();

	int rectype = pdu.get_type();
	bool alarm =false;
	int i,j;
	int slaveindex=0;
	int slaveinde=0;
	CString salvename;
	char salvname[8]={0};
	CString body;
	CSnmpObj *p_record = NULL;
	///////////////////////////////////////////////////////

	POSITION posObj = snmpobj.GetTailPosition ();
	while (posObj != NULL)
	{
		CSnmpObj *p_Object = NULL;
		p_Object = ((CSnmpObj*)(snmpobj.GetPrev (posObj)));
		if(p_Object->ipaddr == ipaddr && ipaddr.GetLength())
		{
			p_record = p_Object;
		}
	}
	//////////////////////////////
	if(!p_record)
	{
		return;
	}
	if(p_record->dtype==0)
	{
		for (i=0; i<pdu.get_vb_count(); i++)
		{
			pdu.get_vb(nextVb, i);
			if(i==1 && ipaddr.GetLength())
			{
				salvename = nextVb.get_printable_value();
				for (j=0; j<10; j++)
				{
					if(p_record->slavename[j] == salvename && p_record->slavename[j].GetLength())
					{
						slaveindex = j;
						if(p_record->slaveOffline[j] == 2)
						{
							alarm =true;
						}
						p_record->slaveOffline[j]=2;
					}
				}
			}
			else if(i==0)
			{
				body.Empty();
				body += nextVb.get_printable_value();
			}
			else if(i==2)
			{
				body += ",";
				body += nextVb.get_printable_value();
			}
			else if(i==3)
			{
				body += ",";
				body += nextVb.get_printable_value();
			}
			else if(i==4)
			{
				body += "(";
				body += nextVb.get_printable_value();
			}
			else if(i==5)
			{
				body += " -- ";
				body += nextVb.get_printable_value();
				body += ").";
			}
		}
		//////////////////////////////////////////////////////////
		if(p_record->npmtype==3)
		{
			int i=0;
			int m = 0;
			CString ReadOidStr;
			CString item[40];
			while(m<40)
			{
				CString a = zd.Tokenize(".",i);
				if(a == "")
					break;
				item[m] = a;
				m++;
			}
			if(atoi(item[9])==2)
			{
				body.Format("%s","Total Current alarm");
				ReadOidStr.Format("1.3.6.1.4.1.30966.10.2.2.1.0");
			}
			else if(atoi(item[9])==3)
			{
				body.Format("Output%s Current A alarm",item[10]);
				ReadOidStr.Format("1.3.6.1.4.1.30966.10.2.3.%d.0",atoi(item[10]));
			}
			
			char ReadOidCh[255];
			memset(ReadOidCh,0,sizeof(ReadOidCh));
			strncpy_s(ReadOidCh,sizeof(char)*255,ReadOidStr,sizeof(char)*250);
			Oid ReadOid(ReadOidCh);
			CString CurItem=ReadItem(p_record,ReadOid);
			p_record->status=2;
			alarm=true;
		}
		if(!alarm && body.GetLength())
		{
			CAlarm mail;
			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
		//	PlaySound("ALARM1.WAV",   NULL,  SND_FILENAME | SND_ASYNC);
			file.SeekToBegin();
			char smtpuser[128];
			memset(smtpuser,0,sizeof(char)*128);
			file.Read(smtpuser,128);
			file.Seek(128,CFile::begin);

			char psd[128];
			memset(psd,0,sizeof(char)*128);
			file.Read(psd,128);
			file.Seek(128*2,CFile::begin);

			char smtpserver[128];
			memset(smtpserver,0,sizeof(char)*128);
			file.Read(smtpserver,128);
			file.Seek(128*3,CFile::begin);

			char portnum[128];
			memset(portnum,0,sizeof(char)*128);
			file.Read(portnum,128);
			int port = atoi(portnum);
			file.Seek(128*4,CFile::begin);

			char sendtype[128];
			memset(sendtype,0,sizeof(char)*128);
			file.Read(sendtype,128);
			CString tp=sendtype;
			int type;
			if(tp == "SSL")
				type=2;
			else if(tp == "LOGIN")
				type=1;
			else
				type =0;
			file.Close();
			//////////////////////////
			char snedto[1024];
			memset(snedto,0,sizeof(char)*1024);
			char sndbody[256];
			memset(sndbody,0,sizeof(char)*256);
			strncpy(salvname,body,4);
//			slaveinde=body[4];
            CString substr = body.Mid(3,1);
            slaveinde=atoi(substr);
  //          memcpy(slaveinde, body, 1);  
			CString mailbody;
			mailbody.Format("device ip:%s, salve name:%s, detail:%s",ipaddr,salvname,body);
			strncpy_s(sndbody,sizeof(char)*255,mailbody,sizeof(char)*250);
			for(i=0;i<20;i++)
			{
				if(strlen(user[i].name)>0 && strlen(user[i].mail)>0)
				{
					strcat_s(snedto,sizeof(char)*1022,user[i].mail);
					strcat_s(snedto,sizeof(char)*1022,";");
				}
			}
			//////////////////////
			if(strlen(snedto)>0)
			{
				mail.sendmail(smtpuser,psd,snedto,
					"CLEVER Manager information",sndbody,smtpserver,port,type);
			}
			//////////////
			if(p_record)
			{
				p_record->trapinformation = body;
			}
			//////////////logs///////////
			int x_point=0,y_point=0,jifangnum=-1;
			for(i=0;i<10;i++)
			{
				if(DCmember[i].name.GetLength())
				{
					POSITION pos = DCmember[i].canbin.GetTailPosition ();
					while (pos != NULL)
					{
						CCanbinObjIcon* p_icon =NULL;
						p_icon= (CCanbinObjIcon*)(DCmember[i].canbin.GetPrev (pos));
						if(p_icon->ipMaster == ipaddr && p_icon->MasterNUM == slaveindex)
						{
							x_point = p_icon->x+1;
							y_point = p_icon->y+1;
							jifangnum = i;
							break;
						}
						if(p_icon->ipSlave == ipaddr && p_icon->SlaveNUM == slaveindex)
						{
							x_point = p_icon->x+1;
							y_point = p_icon->y+1;
							jifangnum = i;
							break;
						}
					}
				}
			}

			if(p_record->npmtype==2)
			{
				p_record->slaveOffline[slaveinde-1]=2;
				alarm=true;
			}
			userlogs *logs;
			logs = new userlogs;
			logs->logtype = 2;
			logs->type = 1;
			memset(logs->name,0,sizeof(char)*32);
			CString alarmdetail;
			alarmdetail.Format("设备:%s->%d",ipaddr,slaveinde-1);
			strncpy_s(logs->name,sizeof(char)*32,alarmdetail,sizeof(char)*30);
			memset(logs->item,0,sizeof(char)*32);
			if(jifangnum>=0 && jifangnum<10 && x_point>0 && y_point>0)
			{
				alarmdetail.Empty();
				alarmdetail.Format("机柜位置:%s(%d:%d)",DCmember[jifangnum].name,x_point,y_point);
				strncpy_s(logs->item,sizeof(char)*32,alarmdetail,sizeof(char)*30);
			}
			memset(logs->detail,0,sizeof(char)*128);
			strncpy_s(logs->detail,sizeof(char)*128,body,sizeof(char)*126);
			CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
		}
	}
	else if(p_record->dtype==1)
	{
		CString ems_name;
		CString ems_slave;
		CString ems_item;
		CString ems_body;
		int emsindex;
		for (i=0; i<pdu.get_vb_count(); i++)
		{
			pdu.get_vb(nextVb, i);
			if(i==1)
			{
				CString zd = nextVb.get_printable_value();
				emsindex = atoi(zd)-1;
				if(emsindex<0 || emsindex>9)
				{
					return;
				}
			}
			else if(i==0)
			{
				ems_item +=nextVb.get_printable_value();
			}
			else if(i==2)
			{
				ems_name =nextVb.get_printable_value();
			}
			else if(i==3)
			{
				ems_item +="(";
				ems_item +=nextVb.get_printable_value();
				ems_item +=") ";
			}
			else if(i==4)
			{
				ems_body+=nextVb.get_printable_value();
				ems_body+=",the value is ";
			}
			else if(i==5)
			{
				ems_body+=nextVb.get_printable_value();
				ems_body+="(";
			}
			else if(i==6)
			{
				ems_body+=nextVb.get_printable_value();
				ems_body+="-";
			}
			else if(i==7)
			{
				ems_body+=nextVb.get_printable_value();
				ems_body+=").";
			}

		}
		if(emsindex<0 || emsindex>9)
		{
			return;
		}
		if(!p_record->EMSname[emsindex].GetLength())
		{
			return;
		}
		ems_slave.Format("%s(%s->%s)", ipaddr,ems_name, p_record->EMSname[emsindex]);
		if(p_record->EMSstatus[emsindex] !=2)
		{
			CAlarm mail;
			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
			file.SeekToBegin();
			char smtpuser[128];
			memset(smtpuser,0,sizeof(char)*128);
			file.Read(smtpuser,128);
			file.Seek(128,CFile::begin);

			char psd[128];
			memset(psd,0,sizeof(char)*128);
			file.Read(psd,128);
			file.Seek(128*2,CFile::begin);

			char smtpserver[128];
			memset(smtpserver,0,sizeof(char)*128);
			file.Read(smtpserver,128);
			file.Seek(128*3,CFile::begin);

			char portnum[128];
			memset(portnum,0,sizeof(char)*128);
			file.Read(portnum,128);
			int port = atoi(portnum);
			file.Seek(128*4,CFile::begin);

			char sendtype[128];
			memset(sendtype,0,sizeof(char)*128);
			file.Read(sendtype,128);
			CString tp=sendtype;
			int type;
			if(tp == "SSL")
				type=2;
			else if(tp == "LOGIN")
				type=1;
			else
				type =0;
			file.Close();
			//////////////////////////
			char snedto[1024];
			memset(snedto,0,sizeof(char)*1024);
			char sndbody[256];
			memset(sndbody,0,sizeof(char)*256);
			CString mailbody;
			mailbody.Format("device ip:%s, salve :%s,item:%s detail:%s",ipaddr,ems_slave,ems_item,ems_body);
			strncpy_s(sndbody,sizeof(char)*255,mailbody,sizeof(char)*250);
			for(i=0;i<20;i++)
			{
				if(strlen(user[i].name)>0 && strlen(user[i].mail)>0)
				{
					strcat_s(snedto,sizeof(char)*1022,user[i].mail);
					strcat_s(snedto,sizeof(char)*1022,";");
				}
			}
			//////////////////////
			if(strlen(snedto)>0)
			{
				mail.sendmail(smtpuser,psd,snedto,
					"CLEVER Manager information",sndbody,smtpserver,port,type);
			}
			//////////////
			if(p_record)
			{
				p_record->trapinformation = ems_item+ems_body;
			}
			userlogs *logs;
			logs = new userlogs;
			logs->logtype = 2;
			logs->type = 1;
			memset(logs->name,0,sizeof(char)*32);
			strncpy_s(logs->name,sizeof(char)*32,ipaddr,sizeof(char)*30);
			memset(logs->item,0,sizeof(char)*32);
			strncpy_s(logs->item,sizeof(char)*32,p_record->EMSname[emsindex],sizeof(char)*30);
			memset(logs->detail,0,sizeof(char)*128);
			strncpy_s(logs->detail,sizeof(char)*128,p_record->trapinformation,sizeof(char)*126);
			CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
		}
		p_record->EMSstatus[emsindex] =2;
	}
	else if(p_record->dtype==2)
	{
		CString ats_name;
		CString ats_body;
		for (i=0; i<pdu.get_vb_count(); i++)
		{
			pdu.get_vb(nextVb, i);

			if(i==0)
			{
				ats_name =nextVb.get_printable_value();
			}
			else if(i==1)
			{
				ats_body +=nextVb.get_printable_value();
			}
			else if(i==2)
			{
				ats_body +=",";
				p_record->trapinformation = nextVb.get_printable_value();
				ats_body +=p_record->trapinformation;
			}
		}
		if(p_record->ATS_status !=2 && p_record->ATS_name.GetLength())
		{
			CAlarm mail;
			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
			file.SeekToBegin();
			char smtpuser[128];
			memset(smtpuser,0,sizeof(char)*128);
			file.Read(smtpuser,128);
			file.Seek(128,CFile::begin);

			char psd[128];
			memset(psd,0,sizeof(char)*128);
			file.Read(psd,128);
			file.Seek(128*2,CFile::begin);

			char smtpserver[128];
			memset(smtpserver,0,sizeof(char)*128);
			file.Read(smtpserver,128);
			file.Seek(128*3,CFile::begin);

			char portnum[128];
			memset(portnum,0,sizeof(char)*128);
			file.Read(portnum,128);
			int port = atoi(portnum);
			file.Seek(128*4,CFile::begin);

			char sendtype[128];
			memset(sendtype,0,sizeof(char)*128);
			file.Read(sendtype,128);
			CString tp=sendtype;
			int type;
			if(tp == "SSL")
				type=2;
			else if(tp == "LOGIN")
				type=1;
			else
				type =0;
			file.Close();
			//////////////////////////
			char snedto[1024];
			memset(snedto,0,sizeof(char)*1024);
			char sndbody[256];
			memset(sndbody,0,sizeof(char)*256);
			CString mailbody;
			mailbody.Format("device ip:%s, name :%s, detail:%s",ipaddr,ats_name,ats_body);
			strncpy_s(sndbody,sizeof(char)*255,mailbody,sizeof(char)*250);
			for(i=0;i<20;i++)
			{
				if(strlen(user[i].name)>0 && strlen(user[i].mail)>0)
				{
					strcat_s(snedto,sizeof(char)*1022,user[i].mail);
					strcat_s(snedto,sizeof(char)*1022,";");
				}
			}
			//////////////////////
			if(strlen(snedto)>0)
			{
				mail.sendmail(smtpuser,psd,snedto,
					"CLEVER Manager information",sndbody,smtpserver,port,type);
			}
			//////////////
			userlogs *logs;
			logs = new userlogs;
			logs->logtype = 2;
			logs->type = 1;
			memset(logs->name,0,sizeof(char)*32);
			strncpy_s(logs->name,sizeof(char)*32,ipaddr,sizeof(char)*30);
			memset(logs->item,0,sizeof(char)*32);
			strncpy_s(logs->item,sizeof(char)*32,p_record->ATS_name,sizeof(char)*30);
			memset(logs->detail,0,sizeof(char)*128);
			strncpy_s(logs->detail,sizeof(char)*128,ats_body,sizeof(char)*126);
			CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
		}
		p_record->ATS_status =2;
	}
	else if(p_record->dtype==3)
	{
		CString alarmInfor;
		int i=0;
		int m = 0;
		CString item[40];
		while(m<40)
		{
			CString a = zd.Tokenize(".",i);
			if(a == "")
				break;
			item[m] = a;
			m++;
		}
		if(atoi(item[10])==4)
		{
			alarmInfor.Format("%s","Total Current A alarm");
		}
		else if(atoi(item[10])==5)
		{
			alarmInfor.Format("%s","Total Current B alarm");
		}
		else if(atoi(item[10])==6)
		{
			alarmInfor.Format("%s","Total Current C alarm");
		}

		CString CurItem=ReadItem(p_record,id);

		if(p_record->ipStatus !=2 && p_record->ipname.GetLength())
		{
			CAlarm mail;
			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
			file.SeekToBegin();
			char smtpuser[128];
			memset(smtpuser,0,sizeof(char)*128);
			file.Read(smtpuser,128);
			file.Seek(128,CFile::begin);

			char psd[128];
			memset(psd,0,sizeof(char)*128);
			file.Read(psd,128);
			file.Seek(128*2,CFile::begin);

			char smtpserver[128];
			memset(smtpserver,0,sizeof(char)*128);
			file.Read(smtpserver,128);
			file.Seek(128*3,CFile::begin);

			char portnum[128];
			memset(portnum,0,sizeof(char)*128);
			file.Read(portnum,128);
			int port = atoi(portnum);
			file.Seek(128*4,CFile::begin);

			char sendtype[128];
			memset(sendtype,0,sizeof(char)*128);
			file.Read(sendtype,128);
			CString tp=sendtype;
			int type;
			if(tp == "SSL")
				type=2;
			else if(tp == "LOGIN")
				type=1;
			else
				type =0;
			file.Close();
			//////////////////////////
			char snedto[1024];
			memset(snedto,0,sizeof(char)*1024);
			char sndbody[256];
			memset(sndbody,0,sizeof(char)*256);
			CString mailbody;
			mailbody.Format("device ip:%s, name :%s, detail:%s %sA",ipaddr,p_record->ipname,alarmInfor,CurItem);
			strncpy_s(sndbody,sizeof(char)*255,mailbody,sizeof(char)*250);
			for(i=0;i<20;i++)
			{
				if(strlen(user[i].name)>0 && strlen(user[i].mail)>0)
				{
					strcat_s(snedto,sizeof(char)*1022,user[i].mail);
					strcat_s(snedto,sizeof(char)*1022,";");
				}
			}
			//////////////////////
			if(strlen(snedto)>0)
			{
				mail.sendmail(smtpuser,psd,snedto,
					"CLEVER Manager information",sndbody,smtpserver,port,type);
			}
			//////////////
			userlogs *logs;
			logs = new userlogs;
			logs->logtype = 2;
			logs->type = 1;
			memset(logs->name,0,sizeof(char)*32);
			strncpy_s(logs->name,sizeof(char)*32,ipaddr,sizeof(char)*30);
			memset(logs->item,0,sizeof(char)*32);
			strncpy_s(logs->item,sizeof(char)*32,p_record->ipname,sizeof(char)*30);
			memset(logs->detail,0,sizeof(char)*128);
			strncpy_s(logs->detail,sizeof(char)*128,alarmInfor+CurItem,sizeof(char)*126);
			CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
		}
		p_record->ipStatus = 2;
	}
}
UINT CInitClever::snmpRecTrap(LPVOID p)
{
	int trap_port;
	trap_port = 162; 
	int status; 
	Snmp::socket_startup();  
	Snmp snmp(status);                
	if ( status != SNMP_CLASS_SUCCESS)
	{
		Snmp::socket_cleanup();
		CloseHandle(trapstop);
		trapstop = INVALID_HANDLE_VALUE;
		return 0;
	}
	OidCollection oidc;
	TargetCollection targetc;
	snmp.notify_set_listen_port(trap_port);
	status = snmp.notify_register(oidc, targetc, callback, NULL);
	if (status != SNMP_CLASS_SUCCESS)
	{
		Snmp::socket_cleanup();
		CloseHandle(trapstop);
		trapstop = INVALID_HANDLE_VALUE;
		return 0;
	}
	else
		snmp.start_poll_thread(1000);
	while(1)
	{
		if(WaitForSingleObject(trapstop,INFINITE) == WAIT_TIMEOUT)
		{
		}
		else
		{
			snmp.stop_poll_thread();
			Snmp::socket_cleanup();  
			CloseHandle(trapstop);
			trapstop = INVALID_HANDLE_VALUE;
			return 0;
		}
	}
	return 0;
}
void CInitClever::snmpstart(void)
{
	threadrun = true;
	snmpstop = false;
	trapstop = INVALID_HANDLE_VALUE;
	currentIP1="";
	currentIP2="";

	AfxBeginThread(snmprun,this);

	//trapstop = CreateEvent(NULL,true,false,"trap");
	//AfxBeginThread(snmpRecTrap,NULL);
}

//////////////////////////////////////
void CInitClever::ExitClever(void)
{
	Exitsnmp();
}
int determine_vb( SmiUINT32 val, Vb &vb,char *buffer)
{
	//char buffer[255];
	switch (val) {
		//case sNMP_SYNTAX_NOSUCHINSTANCE:
		//{
		//vb.set_value(buffer);
		//return TRUE;
		//}
		case sNMP_SYNTAX_OCTETS:
			{
				OctetStr octetstr( buffer);
				if ( octetstr.valid()) {
					vb.set_value( octetstr);
					return TRUE;
				}
				else {
					return FALSE;
				}
			}
			// Gauge32
		case sNMP_SYNTAX_GAUGE32:
			{
				unsigned long i;
				i = atol( buffer);
				Gauge32 gauge32(i); 
				if ( gauge32.valid()) {
					vb.set_value( gauge32);
					return TRUE;
				}
				else {
					return FALSE;
				}
			}
		case sNMP_SYNTAX_CNTR32:
			{
				unsigned long i;
				i = atol( buffer);
				Counter32 counter32(i);
				if ( counter32.valid()) {
					vb.set_value( counter32);
					return TRUE;
				}
				else {
					return FALSE;
				}
			}
		case sNMP_SYNTAX_CNTR64:
			{
				unsigned long i;
				i = atol( buffer);
				Counter64 counter64;
				counter64.set_low(i);
				i = atol( buffer);
				counter64.set_high(i);
				if ( counter64.valid()) {
					vb.set_value( counter64);
					return TRUE;
				}
				else {
					return FALSE;
				}
			}
		case sNMP_SYNTAX_INT:
			{
				unsigned long i;
				i = atol( buffer);
				long l ;
				l = ( long) i;
				vb.set_value( l);
				return TRUE;
			}

		case sNMP_SYNTAX_NOSUCHOBJECT:
			{
				return FALSE;
			}
		default:
			return FALSE;
	}
}
void CInitClever::snmpset(CString ip, int salve, int outputnum,CString order,CString getstr,CString setstr)
{
	CString id;
	POSITION posnow = snmpobj.GetTailPosition ();
	int npm_dtype = false;
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(ip == p_Objectnow->ipaddr && ip.GetLength() && p_Objectnow->dtype==0)
		{
			npm_dtype=p_Objectnow->npmtype;
			break;
		}
	}
	if(npm_dtype==1)
	{
		id.Format(".1.3.6.1.4.1.30966.4.2.%d.1.1.4.%d.0",salve+1,outputnum+1);
	}
	else if(npm_dtype==2)
	{
		id.Format(".1.3.6.1.4.1.30966.5.%d.1.9.%d.0",salve+1,outputnum+1);
	}
	else if(npm_dtype==3)
	{
		id.Format(".1.3.6.1.4.1.30966.10.2.4.%d.0",outputnum+1);
	}
	else
	{
		id.Format(".1.3.6.1.4.1.30966.1.2.%d.1.1.4.%d",salve+1,outputnum+1);
	}
	char setoid[255];
	char ipaddr[255];
	char getcom[255];
	char setcom[255];
	memset(setoid,0,sizeof(char)*255);
	memset(ipaddr,0,sizeof(char)*255);
	memset(getcom,0,sizeof(char)*255);
	memset(setcom,0,sizeof(char)*255);

	strncpy_s(setoid,sizeof(char)*255,id,sizeof(char)*250);
	strncpy_s(ipaddr,sizeof(char)*255,ip,sizeof(char)*250);
	strncpy_s(getcom,sizeof(char)*255,getstr,sizeof(char)*250);
	strncpy_s(setcom,sizeof(char)*255,setstr,sizeof(char)*250);
	Snmp::socket_startup();
	UdpAddress address(ipaddr);
	if ( !address.valid()) {
		return;
	}
	Oid oid(setoid);
	if ( !oid.valid()) {
		return;
	}

	snmp_version version;
	if(npm_dtype==2)
		version=version2c;
	else
		version=version1;
	int retries=1;
	int timeout=200;
	u_short port=161;
	OctetStr community(setcom);
	OctetStr get_community(getcom);
	int status;
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));
	if ( status != SNMP_CLASS_SUCCESS) {
		return;
	}

	Pdu pdu;
	Vb vb;
	vb.set_oid( oid);
	pdu += vb;
	address.set_port(port);
	CTarget ctarget( address);
	ctarget.set_version( version);
	ctarget.set_retry( retries);
	ctarget.set_timeout( timeout);
	ctarget.set_readcommunity( get_community);
	ctarget.set_writecommunity( community);
	SnmpTarget *target;
	target = &ctarget;
	if (( status = snmp.get( pdu,*target))== SNMP_CLASS_SUCCESS) {
		pdu.get_vb( vb,0);

		CString id_vlaue = vb.get_printable_value();
		char ord[255];
		memset(ord,0,sizeof(char)*255);
		strncpy_s(ord,sizeof(char)*255,order,sizeof(char)*250);
		if ( determine_vb(vb.get_syntax(), vb,ord)) {
			Pdu setpdu;
			vb.set_oid( oid);
			setpdu += vb; 
			status = snmp.set( setpdu, *target);
		}
	}
	Snmp::socket_cleanup();

}

void CInitClever::energyWriteLogs(CSnmpObj *p_Objectnow)
{
	SYSTEMTIME ct;
	GetLocalTime(&ct);

	makesureDir(getFilePath()+"\\energy\\"+p_Objectnow->ipaddr);

	CString filename;
	filename.Format("%4d%2d%2d",ct.wYear,ct.wMonth,ct.wDay);
	filename.Replace(" ","0");

	//if(makesureFile(getFilePath()+"\\energy\\"+p_Objectnow->ipaddr+"\\"+filename))
	//{
	//	return;
	//}
	CFile file;
	struct energydata
	{
		int TEnergy[10][3];
		int energy[10][24];
	};
	if(file.Open(getFilePath()+"\\energy\\"+p_Objectnow->ipaddr+"\\"+filename,CFile::modeReadWrite|CFile::modeCreate))
	{
		energydata *s_energy;
		s_energy = new energydata;
		memset(s_energy,0,sizeof(energydata));
		int i,j;
		for(i=0;i<10;i++)
		{
			for(j=0;j<24;j++)
			{
				if(p_Objectnow->energy[i][j])
				{
					s_energy->energy[i][j] = p_Objectnow->energy[i][j];
				}
			}
			s_energy->TEnergy[i][0] = p_Objectnow->TEnergy[i][0];
			s_energy->TEnergy[i][1] = p_Objectnow->TEnergy[i][1];
			s_energy->TEnergy[i][2] = p_Objectnow->TEnergy[i][2];
		}
		file.SeekToBegin();
		file.Write((LPCTSTR)s_energy,sizeof(energydata));
		file.Close();
		delete s_energy;
	}
}
void CInitClever::AddotherDevice(void)
{
	//snmpobj.AddTail(new CSnmpObj("192.168.1.185","public",1));//ems
	//snmpobj.AddTail(new CSnmpObj("192.168.1.175","public",2));//ats
	atsemsListRead();
	ippduListRead();
}
void insertIPPDU(CSnmpObj *p_Object,CString id,CString id_value)
{
	CString rec = id;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[9] == "1")
	{
		p_Object->ipname=id_value;
		if(p_Object->ipStatus!=2)
			p_Object->ipStatus=1;
	}
	if(zd[9] == "2")
	{
		if(zd[10] == "1")
		{
			p_Object->ipTvol[0]=atoi(id_value);
		}
		if(zd[10] == "2")
		{
			p_Object->ipTvol[1]=atoi(id_value);
		}
		if(zd[10] == "3")
		{	
			p_Object->ipTvol[2]=atoi(id_value);
		}
		if(zd[10] == "4")
		{
			p_Object->ipTcur[0]=(float)atof(id_value)/10;
		}
		if(zd[10] == "5")
		{
			p_Object->ipTcur[1]=(float)atof(id_value)/10;
		}
		if(zd[10] == "6")
		{
			p_Object->ipTcur[2]=(float)atof(id_value)/10;
		}
		if(zd[10] == "7")
		{
			p_Object->ipTenergy[0]=(float)atof(id_value);
		}
		if(zd[10] == "8")
		{
			p_Object->ipTenergy[1]=(float)atof(id_value);
		}
		if(zd[10] == "9")
		{
			p_Object->ipTenergy[2]=(float)atof(id_value);
		}
	}
}

int IPPDUsnmpread(CSnmpObj *p_Object)
{
	int rec = 0;
	int requests = 0;        
	int objects  = 0;
	char ipaddr[255];
	memset(ipaddr,0,sizeof(char)*255);
	strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
	Snmp::socket_startup();
	UdpAddress address(ipaddr);   
	Oid oid(".1.3.6.1.4.1.30966.10.3"); 
	if ( !oid.valid()) {            
		Snmp::socket_cleanup();
		return rec;
	}

	snmp_version version=version1;                  
	int retries=1;                                  
	int timeout=200;                                
	u_short port=161; 
	char com[255];
	memset(com,0,sizeof(char)*255);
	strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);
	OctetStr community(com);                  
	bool subtree = true;

	int status;
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if ( status != SNMP_CLASS_SUCCESS) {
		Snmp::socket_cleanup();
		return rec;
	}
	Pdu pdu;                               
	Vb vb;                                
	vb.set_oid( oid);                    
	pdu += vb;                            

	address.set_port(port);
	CTarget ctarget(address);             

	ctarget.set_version( version);         
	ctarget.set_retry( retries);           
	ctarget.set_timeout( timeout);         
	ctarget.set_readcommunity( community); 

	SnmpTarget *target;

	target = &ctarget;
	while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
	{
		Sleep(1);
		if(!threadrun)
		{
			Snmp::socket_cleanup();
			return rec;
		}
		requests++;
		for ( int z=0;z<pdu.get_vb_count(); z++) {
			pdu.get_vb( vb,z);

			Oid tmp;
			vb.get_oid(tmp);
			if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
			{
				Snmp::socket_cleanup();
				return rec;
			}
			objects++; 
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				CString id = vb.get_printable_oid();
				CString id_vlaue = vb.get_printable_value();
				insertIPPDU(p_Object,id,id_vlaue);
				rec = 1;
			}
			else 
			{
				Snmp::socket_cleanup();
				return rec;
			}
		}
		pdu.set_vblist(&vb, 1);
	}
	////////////////////////
	Snmp::socket_cleanup();
	return rec;
}


void insertEMS(CSnmpObj *p_Object,CString id,CString id_value)
{
	CString rec = id;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(zd[12].GetLength())
	{
		int index = atoi(zd[12]) -1;
		if(index<0 || index>12)
		{
			return;
		}
		if(zd[8] == "2"&&zd[11] == "3")
		{
			if(p_Object->EMSstatus[index]!=2)
			{
				p_Object->EMSstatus[index]=1;
			}
			p_Object->EMSname[index].Empty();
			p_Object->EMSname[index] = id_value;
		}
		else if(zd[8] == "2"&&zd[11] == "4")
		{
			p_Object->EMS_th_PDU[0][index] = atoi(id_value);
		}
		else if(zd[8] == "2"&&zd[11] == "5")
		{
			p_Object->EMS_th_PDU[1][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "6")
		{
			p_Object->EMS_th_PDU[2][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "7")
		{
			p_Object->EMS_th_PDU[3][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "8")
		{
			p_Object->EMS_door_PDU[0][index].Empty();
			p_Object->EMS_door_PDU[0][index] = id_value;
		}
		if(zd[8] == "2"&&zd[11] == "9")
		{
			p_Object->EMS_door_PDU[1][index].Empty();
			p_Object->EMS_door_PDU[1][index] = id_value;
		}
		if(zd[8] == "2"&&zd[11] == "10")
		{
			p_Object->EMS_water_PDU[index].Empty();
			p_Object->EMS_water_PDU[index] = id_value;
		}
		if(zd[8] == "2"&&zd[11] == "11")
		{
			p_Object->EMS_smoke_PDU[index].Empty();
			p_Object->EMS_smoke_PDU[index] = id_value;
		}
		if(zd[8] == "2"&&zd[11] == "12")
		{
			p_Object->EMS_vol_PDU[0][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "13")
		{
			p_Object->EMS_cur_PDU[0][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "14")
		{
			p_Object->EMS_energy_PDU[0][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "15")
		{
			p_Object->EMS_vol_PDU[1][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "16")
		{
			p_Object->EMS_cur_PDU[1][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "17")
		{
			p_Object->EMS_energy_PDU[1][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "18")
		{
			p_Object->EMS_vol_PDU[2][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "19")
		{
			p_Object->EMS_cur_PDU[2][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "20")
		{
			p_Object->EMS_energy_PDU[2][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "21")
		{
			p_Object->EMS_vol_PDU[3][index] = atoi(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "22")
		{
			p_Object->EMS_cur_PDU[3][index] = (float)atof(id_value);
		}
		if(zd[8] == "2"&&zd[11] == "23")
		{
			p_Object->EMS_energy_PDU[3][index] = (float)atof(id_value);
		}
	}
}
int EMSsnmpread(CSnmpObj *p_Object)
{
	int rec = 0;
	int requests = 0;        
	int objects  = 0;
	char ipaddr[255];
	memset(ipaddr,0,sizeof(char)*255);
	strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
	Snmp::socket_startup();
	UdpAddress address(ipaddr);   
	Oid oid(".1.3.6.1.4.1.30966.2"); 
	if ( !oid.valid()) {            
		Snmp::socket_cleanup();
		return rec;
	}

	snmp_version version=version2c;                  
	int retries=1;                                  
	int timeout=200;                                
	u_short port=161; 
	char com[255];
	memset(com,0,sizeof(char)*255);
	strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);
	OctetStr community(com);                  
	bool subtree = true;

	int status;
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if ( status != SNMP_CLASS_SUCCESS) {
		Snmp::socket_cleanup();
		return rec;
	}
	Pdu pdu;                               
	Vb vb;                                
	vb.set_oid( oid);                    
	pdu += vb;                            

	address.set_port(port);
	CTarget ctarget(address);             

	ctarget.set_version( version);         
	ctarget.set_retry( retries);           
	ctarget.set_timeout( timeout);         
	ctarget.set_readcommunity( community); 

	SnmpTarget *target;

	target = &ctarget;
	while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
	{
		Sleep(1);
		if(!threadrun)
		{
			Snmp::socket_cleanup();
			return rec;
		}
		requests++;
		for ( int z=0;z<pdu.get_vb_count(); z++) {
			pdu.get_vb( vb,z);

			Oid tmp;
			vb.get_oid(tmp);
			if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
			{
				Snmp::socket_cleanup();
				return rec;
			}
			objects++; 
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				CString id = vb.get_printable_oid();
				CString id_vlaue = vb.get_printable_value();
				insertEMS(p_Object,id,id_vlaue);
				rec = 1;
			}
			else 
			{
				Snmp::socket_cleanup();
				return rec;
			}
		}
		pdu.set_vblist(&vb, 1);
	}
	////////////////////////
	Snmp::socket_cleanup();
	return rec;
}
///////////////////////////////////////////ATS///////////////////////////////////////////////////////
void insertATS(CSnmpObj *p_Object,CString id,CString id_value)
{
	CString rec = id;
	int i = 0;
	int m = 0;
	CString zd[40];
	while(m<40)
	{
		CString a = rec.Tokenize(".",i);
		if(a == "")
			break;
		zd[m] = a;
		m++;
	}
	if(p_Object->ATS_status != 2)
	{
		p_Object->ATS_status = 1;
	}
	if(zd[10] == "1")
	{
		p_Object->ATS_name.Empty();
		p_Object->ATS_name = "ATS";

		p_Object->ATS_lin1name.Empty();
		p_Object->ATS_lin1name = id_value.Mid(52,6);
	}
	else if(zd[10] == "2")
	{
		p_Object->ATS_lin2name.Empty();
		p_Object->ATS_lin2name = id_value.Mid(52,6);
	}
	else if(zd[10] == "4")
	{
		p_Object->ATS_maininput.Empty();
		p_Object->ATS_maininput = id_value.Mid(52,6);

		p_Object->ATS_currentinput.Empty();
		p_Object->ATS_currentinput = id_value.Mid(52,6);
	}
	else if(zd[10] == "6")
	{
		p_Object->ATS_VA = atoi(id_value);
	}
	else if(zd[10] == "7")
	{
		p_Object->ATS_VB = atoi(id_value);	
	}
	else if(zd[10] == "8")
	{
		p_Object->ATS_VO = atoi(id_value);
	}
	else if(zd[10] == "9")
	{
		p_Object->ATS_CA = (float)atof(id_value)/10;
	}
	else if(zd[10] == "10")
	{
		p_Object->ATS_CB = (float)atof(id_value)/10;
	}
	else if(zd[10] == "11")
	{
		
		p_Object->ATS_CO = (float)atof(id_value)/10;

	}
	else if(zd[10] == "12")
	{
		p_Object->ATS_VL = atoi(id_value);
	}
	else if(zd[10] == "13")
	{
		p_Object->ATS_VH = atoi(id_value);
	}
	else if(zd[10] == "14")
	{
		p_Object->ATS_CL = (float)atof(id_value)/10;
	}
	else if(zd[10] == "15")
	{
		p_Object->ATS_CH = (float)atof(id_value)/10;
	}

}
int ATSsnmpread(CSnmpObj *p_Object)
{
	int rec = 0;
	int requests = 0;        
	int objects  = 0;
	char ipaddr[255];
	memset(ipaddr,0,sizeof(char)*255);
	strncpy_s(ipaddr,sizeof(char)*255,p_Object->ipaddr,sizeof(char)*250);
	Snmp::socket_startup();
	UdpAddress address(ipaddr);   
	Oid oid(".1.3.6.1.4.1.30966.10.1.1"); 
	if ( !oid.valid()) {            
		Snmp::socket_cleanup();
		return rec;
	}

	snmp_version version=version1;                  
	int retries=1;                                  
	int timeout=200;                                
	u_short port=161; 
	char com[255];
	memset(com,0,sizeof(char)*255);
	strncpy_s(com,sizeof(char)*255,p_Object->community,sizeof(char)*250);
	OctetStr community(com);                  
	bool subtree = true;

	int status;
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if ( status != SNMP_CLASS_SUCCESS) {
		Snmp::socket_cleanup();
		return rec;
	}
	Pdu pdu;                               
	Vb vb;                                
	vb.set_oid( oid);                    
	pdu += vb;                            

	address.set_port(port);
	CTarget ctarget(address);             

	ctarget.set_version( version);         
	ctarget.set_retry( retries);           
	ctarget.set_timeout( timeout);         
	ctarget.set_readcommunity( community); 

	SnmpTarget *target;

	target = &ctarget;
	while (( status = snmp.get_next( pdu,*target))== SNMP_CLASS_SUCCESS) 
	{
		Sleep(1);
		if(!threadrun)
		{
			Snmp::socket_cleanup();
			return rec;
		}
		requests++;
		for ( int z=0;z<pdu.get_vb_count(); z++) {
			pdu.get_vb( vb,z);

			Oid tmp;
			vb.get_oid(tmp);
			if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
			{
				Snmp::socket_cleanup();
				return rec;
			}
			objects++; 
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				CString id = vb.get_printable_oid();
				CString id_vlaue = vb.get_printable_value();
				insertATS(p_Object,id,id_vlaue);
				rec = 1;
			}
			else 
			{
				Snmp::socket_cleanup();
				return rec;
			}
		}
		pdu.set_vblist(&vb, 1);
	}
	////////////////////////
	Snmp::socket_cleanup();
	return rec;
}
void CInitClever::atsemsListRead(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\atsemslist",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	int type;
	char dtype;
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		dtype=0;
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		file.Seek(132*i+128,CFile::begin);
		file.Read(&dtype,1);
		i++;
		////////////
		type = (int)dtype;
		if(strlen(ip) && strlen(com) && dtype)
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,type));
		}
	}
	while(strlen(ip));

	file.Close();
}

void CInitClever::atsemsListWrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\atsemslist",CFile::modeReadWrite | CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		char type;
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength())
		{
			if( p_Objectnow->dtype==1 || p_Objectnow->dtype==2)
			{
				type = p_Objectnow->dtype;
				strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
				strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

				file.Seek(132*i,CFile::begin);
				file.Write(ip,62);
				file.Seek(132*i+64,CFile::begin);
				file.Write(com,62);
				file.Seek(132*i+128,CFile::begin);
				file.Write(&type,1);
				i++;
			}
		}
	}
	file.Close();
}

void CInitClever::ippduListRead(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\ippdu",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	int type;
	char dtype;
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		dtype=0;
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		file.Seek(132*i+128,CFile::begin);
		file.Read(&dtype,1);
		i++;
		////////////
		type = (int)dtype;
		if(strlen(ip) && strlen(com) && dtype)
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,type));
		}
	}
	while(strlen(ip));

	file.Close();
}

void CInitClever::ippduListWrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\ippdu",CFile::modeReadWrite | CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		char type;
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength())
		{
			if( p_Objectnow->dtype==3)
			{
				type = p_Objectnow->dtype;
				strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
				strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

				file.Seek(132*i,CFile::begin);
				file.Write(ip,62);
				file.Seek(132*i+64,CFile::begin);
				file.Write(com,62);
				file.Seek(132*i+128,CFile::begin);
				file.Write(&type,1);
				i++;
			}
		}
	}
	file.Close();
}

void CInitClever::writeStatusLog(CSnmpObj *p_Objectnow)
{
	int i,j,k;
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	//if(recordtimer == ct.wHour)
	//	return;
	makesureDir(getFilePath()+"\\status\\"+p_Objectnow->ipaddr);


	CString filename;
	filename.Format("%4d%2d%2d",ct.wYear,ct.wMonth,ct.wDay);
	filename.Replace(" ","0");

	CFile file;
	struct recorddata
	{
		float cur[24][60][10][24];
		float Tcur[24][60][10][3];
		int Tvol[24][60][10][3];
		int th[24][60][10][6];
	};
	recorddata *p_record;
	p_record = new recorddata;
	if(file.Open(getFilePath()+"\\status\\"+p_Objectnow->ipaddr+"\\"+filename,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
	{
		memset(p_record,0,sizeof(recorddata));
		file.Read(p_record,sizeof(recorddata));
		file.Close();
	}
	for(i=0;i<10;i++)
	{
		if(p_Objectnow->slaveOffline[i]==1)
		{
			k=i;
			for(j=0;j<24;j++)
			{
				p_record->cur[ct.wHour][ct.wMinute][k][j] = p_Objectnow->curout[k][j];
			}
			for(j=0;j<3;j++)
			{
				p_record->Tcur[ct.wHour][ct.wMinute][k][j] = p_Objectnow->Tcur[k][j];
				p_record->Tvol[ct.wHour][ct.wMinute][k][j] = p_Objectnow->Tvol[k][j];
			}
			for(j=0;j<6;j++)
			{
				p_record->th[ct.wHour][ct.wMinute][k][j] = p_Objectnow->TH[k][j];
			}
		}
	}
	if(file.Open(getFilePath()+"\\status\\"+p_Objectnow->ipaddr+"\\"+filename,CFile::modeReadWrite|CFile::modeCreate))
	{
		file.Write(p_record,sizeof(recorddata));
		file.Close();
	}
	delete p_record;
}

void CInitClever::NPMIVdeviceread(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\npm_iv",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		i++;
		////////////
		if(strlen(ip) && strlen(com))
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,0,1));
		}
	}
	while(strlen(ip));

	file.Close();
}

void CInitClever::NPMVdeviceread(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\npm_v",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		i++;
		////////////
		if(strlen(ip) && strlen(com))
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,0,2));
		}
	}
	while(strlen(ip));

	file.Close();
}

void CInitClever::NPMIVdevicewrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\npm_iv",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==1)
		{
			strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
			strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

			file.Seek(132*i,CFile::begin);
			file.Write(ip,62);
			file.Seek(132*i+64,CFile::begin);
			file.Write(com,62);
			i++;
		}
	}
	file.Close();
}

void CInitClever::NPMVdevicewrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\npm_v",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==2)
		{
			strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
			strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

			file.Seek(132*i,CFile::begin);
			file.Write(ip,62);
			file.Seek(132*i+64,CFile::begin);
			file.Write(com,62);
			i++;
		}
	}
	file.Close();
}


void CInitClever::MSPDUdevicewrite(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\ms_pdu",CFile::modeReadWrite|CFile::modeCreate);
	file.SeekToBegin();
	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		char ip[64];
		char com[64];
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->community.GetLength() && p_Objectnow->dtype==0 && p_Objectnow->npmtype==3)
		{
			strncpy_s(ip,sizeof(char)*64,p_Objectnow->ipaddr,sizeof(char)*62);
			strncpy_s(com,sizeof(char)*64,p_Objectnow->community,sizeof(char)*62);

			file.Seek(132*i,CFile::begin);
			file.Write(ip,62);
			file.Seek(132*i+64,CFile::begin);
			file.Write(com,62);
			i++;
		}
	}
	file.Close();
}

void CInitClever::MSPDUdeviceread(void)
{
	int i=0;
	CFile file;
	file.Open(getFilePath()+"\\info\\list\\ms_pdu",CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
	file.SeekToBegin();
	char ip[64];
	char com[64];
	do
	{
		memset(ip,0,sizeof(char)*64);
		memset(com,0,sizeof(char)*64);
		file.Seek(132*i,CFile::begin);
		file.Read(ip,62);
		file.Seek(132*i+64,CFile::begin);
		file.Read(com,62);
		i++;
		////////////
		if(strlen(ip) && strlen(com))
		{
			snmpobj.AddTail(new CSnmpObj(ip,com,0,3));
		}
	}
	while(strlen(ip));

	file.Close();
}
