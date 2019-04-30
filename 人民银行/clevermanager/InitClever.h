#pragma once
#include "SnmpObj.h"
class CInitClever
{
	
public:
	friend class GenAddress;
	CInitClever(void);
	~CInitClever(void);
	void initData(void);
	void dataCenterRead();
	void dataCenterWrite();
	void Snmpinit(void);
	void ExitClever(void);
	void Exitsnmp(void);
	void snmpstart(void);
	static UINT snmprun(LPVOID p);
	static UINT  snmpRecTrap(LPVOID p);
	void userRead(void);
	void userWrite(void);
	void deviceListWrite(void);
	void deviceListRead(void);
	void snmpset(CString ip, int salve, int outputnum,CString order,CString getstr,CString setstr);
	void energyWriteLogs(CSnmpObj *p_Objectnow);
	/////////////
	void AddotherDevice(void);
	void atsemsListRead(void);
	void atsemsListWrite(void);
	void writeStatusLog(CSnmpObj *p_Objectnow);
	void NPMIVdeviceread(void);
	void NPMVdeviceread(void);
	void NPMIVdevicewrite(void);
	void NPMVdevicewrite(void);
	void MSPDUdeviceread(void);
	void MSPDUdevicewrite(void);
	void ippduListWrite(void);
	void ippduListRead(void);
};
