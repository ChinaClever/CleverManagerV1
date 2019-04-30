#pragma once

typedef bool (*MAIL)(char *,char * ,char * ,char * ,
			 char * ,char * ,int ,int );

class CAlarm
{
public:
	CAlarm(void);
	~CAlarm(void);
	void sendmail(char* user,char*password,char* toaddr,char * title,char* body,char* smtp,int port,int mode);
};
