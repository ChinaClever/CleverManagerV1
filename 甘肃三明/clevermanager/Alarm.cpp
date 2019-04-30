#include "StdAfx.h"
#include "Alarm.h"

CAlarm::CAlarm(void)
{
}

CAlarm::~CAlarm(void)
{
}

void CAlarm::sendmail(char* user,char*password,char* toaddr,char * title,char* body,char* smtp,int port,int mode)
{
	if(*user!=NULL&&*toaddr!=NULL)
	{
		MAIL  p_mail=NULL;
		HINSTANCE hlib =NULL;
		hlib = ::LoadLibrary(_T("CSmtp_release.dll"));
		
		if(NULL == (p_mail = (MAIL)GetProcAddress(hlib,"?mailsend@@YAHPAD00000HH@Z"))) {
			ASSERT(FALSE);
			goto Done;
		}
		p_mail(user,password,toaddr,title,body,smtp,port,mode);

		//p_mail("zhangdan@china-clever.com","clever123","zhangdan@china-clever.com","clever","clever\rclever\r\n",
		//	"mail.china-clever.com",25,1);
		Done:
		if(NULL != hlib) {
				::FreeLibrary(hlib);
		}
	}
}
