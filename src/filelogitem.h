#ifndef __FILELOGITEM_H_
#define __FILELOGITEM_H_

#include <QString>
#include <QDateTime>
/**
 * TFileLogItem is used for storing open file information when monitoring a  process.
 */
class TFileLogItem
{
private:
	int fd;
	QString fileName;
	QDateTime openAt;
	QDateTime closedAt;
	bool openCheck;
	bool realFile;
	bool isNew;
public:
	inline int getFd()const { return fd;} ;
	inline const QString &getFileName() const{ return fileName;};
	inline const QDateTime &getOpenAt()const{ return openAt;};
	inline const QDateTime &getClosedAt() const{ return closedAt;};
	inline void setOpenCheck(bool p_openCheck){ openCheck=p_openCheck;}
	inline bool getOpenCheck() const{ return openCheck;}
	inline bool getRealFile() const { return realFile;}
	inline bool getIsNew() const { return isNew;}
	inline void setIsNew(bool p_flag){ isNew=p_flag;}
	void fileIsClosed();
	TFileLogItem(int p_fd,bool p_realFile,const QString &p_fileName);
};

#endif
