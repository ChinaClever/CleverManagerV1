#include "StdAfx.h"
#include "FileLogEdit.h"
#include "clevermanager.h"
#include "CanbinObjIcon.h"

CFileLogEdit::CFileLogEdit(void)
{
}

CFileLogEdit::~CFileLogEdit(void)
{
}

void CFileLogEdit::mapCabinRead(void)
{
	int i,j,m;
	
	for(i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength())
		{
			CString filename;
			CFile file;
			filename.Format("\\info\\map%d",i);
			file.Open(getFilePath()+filename,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate);
			file.SeekToBegin();
			j=0;
			while(j<400)
			{
				char value[32];
				memset(value,0,sizeof(char)*32);
				file.Seek(1024*j,CFile::begin);
				file.Read(value,1);
				int status = atoi(value);

				memset(value,0,sizeof(char)*32);
				file.Seek(1024*j+1,CFile::begin);
				file.Read(value,1);
				int type = atoi(value);

				memset(value,0,sizeof(char)*32);
				file.Seek(1024*j+2,CFile::begin);
				file.Read(value,2);
				int x = atoi(value);

				memset(value,0,sizeof(char)*32);
				file.Seek(1024*j+4,CFile::begin);
				file.Read(value,2);
				int y = atoi(value);

				if(status==0||status==1||status==2||status==3||status==4||status==5)
				{
					memset(value,0,sizeof(char)*32);
					file.Seek(1024*j+6,CFile::begin);
					file.Read(value,16);
					CString ip1;
					ip1	= value;	

					memset(value,0,sizeof(char)*32);
					file.Seek(1024*j+22,CFile::begin);
					file.Read(value,16);
					CString ip2;
					ip2	= value;

					memset(value,0,sizeof(char)*32);
					file.Seek(1024*j+38,CFile::begin);
					file.Read(value,2);
					int masterNum = atoi(value);

					memset(value,0,sizeof(char)*32);
					file.Seek(1024*j+40,CFile::begin);
					file.Read(value,2);
					int slaveNum = atoi(value);

					CCanbinObjIcon *p_obj=NULL;
					if(ip1.GetLength()||ip2.GetLength())
					{
						p_obj = new CCanbinObjIcon(x,y,status,ip1,ip2,masterNum,slaveNum,type);
						DCmember[i].canbin.AddTail(p_obj);

						for(m=0;m<24;m++)
						{
							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+42+m*58,CFile::begin);
							file.Read(value,16);
							p_obj->device_master[m].name = value;

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+62+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_master[m].start = atoi(value);

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+64+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_master[m].height = atoi(value);

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+66+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_master[m].maininputNum = atoi(value);

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+68+m*58,CFile::begin);
							file.Read(value,16);
							p_obj->device_slave[m].name = value;

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+84+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_slave[m].start = atoi(value);

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+86+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_slave[m].height = atoi(value);

							memset(value,0,sizeof(char)*32);
							file.Seek(1024*j+88+m*58,CFile::begin);
							file.Read(value,2);
							p_obj->device_slave[m].slaveinputNum = atoi(value);

							
						}
						memset(value,0,sizeof(char)*32);
						file.Seek(1024*j+m*58+90,CFile::begin);
						file.Read(value,10);
						p_obj->cabientname=value;
					}
				}
				else if(x>0 || y>0)
				{
					//DCmember[i].canbin.AddTail(new CCanbinObjIcon(x,y,0,"","",0,0,type));
				}
				j+=2;
			}

			file.Close();
		}

	}
	
}

void CFileLogEdit::mapCabinWrite(int mapnum)
{
	int j,m;
	CString filename;
	CFile file;
	filename.Format("\\info\\map%d",mapnum);
	file.Open(getFilePath()+filename,CFile::modeReadWrite |CFile::modeCreate);
	file.SeekToBegin();

	if(DCmember[mapnum].name.GetLength())
	{
		POSITION pos = DCmember[mapnum].canbin.GetHeadPosition ();
		j=0;
		while (pos != NULL)
		{
			CCanbinObjIcon *p_obj=NULL;
			p_obj= (CCanbinObjIcon*)(DCmember[mapnum].canbin.GetNext (pos));

			file.Seek(1024*j,CFile::begin);
			CString value;
			value.Format("%1d", p_obj->status);
			file.Write((LPCTSTR)value,1);
			file.Seek(1+1024*j,CFile::begin);

			value.Empty();
			value.Format("%1d", p_obj->type);
			file.Write((LPCTSTR)value,1);
			file.Seek(2+1024*j,CFile::begin);

			value.Empty();
			value.Format("%d", p_obj->x);
			file.Write((LPCTSTR)value,2);
			file.Seek(4+1024*j,CFile::begin);

			value.Empty();
			value.Format("%d", p_obj->y);
			file.Write((LPCTSTR)value,2);
			file.Seek(6+1024*j,CFile::begin);

			value.Empty();
			value = p_obj->ipMaster;
			file.Write((LPCTSTR)value,16);
			file.Seek(22+1024*j,CFile::begin);

			value.Empty();
			value = p_obj->ipSlave;
			file.Write((LPCTSTR)value,16);
			file.Seek(38+1024*j,CFile::begin);

			value.Empty();
			value.Format("%d", p_obj->MasterNUM);
			file.Write((LPCTSTR)value,2);
			file.Seek(40+1024*j,CFile::begin);

			value.Empty();
			value.Format("%d", p_obj->SlaveNUM);
			file.Write((LPCTSTR)value,2);
			

			for(m=0;m<24;m++)
			{
				file.Seek(42+1024*j+m*58,CFile::begin);
				value.Empty();
				value=p_obj->device_master[m].name;
				if(value.GetLength()>16)
				{
					value = value.Left(16);
				}
				file.Write((LPCTSTR)value,16);
				file.Seek(62+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_master[m].start);
				file.Write((LPCTSTR)value,2);
				file.Seek(64+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_master[m].height);
				file.Write((LPCTSTR)value,2);
				file.Seek(66+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_master[m].maininputNum);
				file.Write((LPCTSTR)value,2);
				file.Seek(68+1024*j+m*58,CFile::begin);

				value.Empty();
				value=p_obj->device_slave[m].name;
				if(value.GetLength()>16)
				{
					value = value.Left(16);
				}
				file.Write((LPCTSTR)value,16);
				file.Seek(84+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_slave[m].start);
				file.Write((LPCTSTR)value,2);
				file.Seek(86+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_slave[m].height);
				file.Write((LPCTSTR)value,2);
				file.Seek(88+1024*j+m*58,CFile::begin);

				value.Empty();
				value.Format("%d", p_obj->device_slave[m].slaveinputNum);
				file.Write((LPCTSTR)value,2);
				file.Seek(90+1024*j+m*58,CFile::begin);

			}
			value.Empty();
			value.Format("%s", p_obj->cabientname);
			file.Seek(90+1024*j+m*58,CFile::begin);
			file.Write((LPCTSTR)value,10);
			j+=2;
		}
	}
	file.Close();
}
