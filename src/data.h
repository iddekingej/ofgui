#ifndef __DATA_H_
#define __DATA_H_

#include <QList>
#include <QMap>
#include "linklist.h"

class TOpenFile;

/**
 * Represents process that has open files
 */

class TProcess {
private:
	QList<TOpenFile*> openFiles;
	long    id;
	long    ownerId;
	QString owner;
	QString programName;
public:
	TProcess(long p_id,const QString &p_programName);
	inline bool hasOpenFile(){ return !openFiles.isEmpty();}
	inline QList<TOpenFile*> *getOpenFiles(){ return &openFiles;}
	inline int getId(){ return id;}
	inline const QString &getProgramName(){ return programName;}
	inline void setOwnerId(long p_ownerId){ ownerId=p_ownerId;}
	inline long getOwnerId(){ return ownerId;}
	inline void setOwner(const QString &p_owner){ owner=p_owner;}
	inline const QString &getOwner(){ return owner;}
	void addOpenFile(TOpenFile *p_openFile);
};

/**
 * An open file, A \ref TProcess objects has 0 or n TOpenFile objects
 */
class TOpenFile
{
private:
	TProcess *owner;
	long     fd;
	QString  fileName;
	bool     realFile;
public:
	inline TProcess *getOwner(){ return owner;}
	inline long getFd(){ return fd;}
	inline const QString &getFileName(){ return fileName;}
	inline bool getRealFile(){return realFile;}
	TOpenFile(bool p_realFile,TProcess *p_owner,long p_fd,const QString &p_fileName);
};


class TProcessList
{
private:
	TLinkList<TProcess>  programs;
	TLinkList<TOpenFile> openFiles;
	QMap<uint,QString>   users;
	
	void processOpenFiles(const QString &p_path,TProcess *p_program);
public:
	inline TLinkListItem<TProcess> *getProgramsStart(){ return programs.getStart();}
	inline TLinkListItem<TOpenFile> *getOpenFilesStart(){ return openFiles.getStart();}
	
	void processInfo();
};

#endif
