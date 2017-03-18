#include "filelogitem.h"

TFileLogItem::TFileLogItem(int p_fd,bool p_realFile, const QString& p_fileName)
{
	fd=p_fd;
	fileName=p_fileName;
	openAt=QDateTime::currentDateTime();
	openCheck=true;
	realFile=p_realFile;
	isNew=false;
}

void TFileLogItem::fileIsClosed()
{
	closedAt=QDateTime::currentDateTime();
}
