/**
 *  data.h
 *  Part of ofgui.h
 * 
 *  (C) By Jeroen van Iddekinge (iddekingej@lycos.com)
 * 
 *  Data classes. 
 *  
 * Proces information is read from the /proc folder 
 * 
 * First the /proc folder is scanned for numerical files (containing proces information)
 * for each proces a proces information object is created.
 * For each proces the list of open files are read from /proc/#pid#/fd (e.g. /proc/2501/fd
 * Open file information is added to the process information object 
 *  
 */

#ifndef __DATA_H_
#define __DATA_H_

#include <QList>
#include <QMap>
#include "linklist.h"
#include "os.h"
class TOpenFile;

/**
 * Information about  processes running on the system 
 */

class TProcess {
private:
	
/**
 * List of open files
 */
	QList<TOpenFile*> openFiles;
/**
 *pid of process
 */	
	long    id;
/**
 * user id of process owner
 */	
	long    ownerId;
/**
 *Username of process owner
 */
	QString owner;
/**
 *Program name of process
 */	
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
/**
 *   Proces that opened the file
 */
	TProcess *process;
/*
 *  File handle ID
 */
	long     fd;
	
/**
 *  Name of the file that is opened
 */

	QString  fileName;
	
/**
 * True if it is a realfile or otherwise a socket,event fd etc...
 * 
 */
	bool     realFile;
/**
 * File type;
 */
	TFileType fileType;
public:
	inline TProcess *getProcess(){ return process;}
	inline long getFd(){ return fd;}
	inline const QString &getFileName(){ return fileName;}
	inline bool getRealFile(){return realFile;}
	TOpenFile(bool p_realFile,TProcess *p_process,long p_fd,const QString &p_fileName);
	inline   TFileType getFileType(){ return fileType;}
	inline void setFileType(TFileType p_fileType){ fileType=p_fileType;}
	inline void getFileTypeStr(QString &p_fileTypeStr){
		p_fileTypeStr=fileTypeStr(fileType);
	}
};


class TProcessList
{
private:
	TLinkList<TProcess>  programs;
	TLinkList<TOpenFile> openFiles;
	QMap<uint,QString>   users;
	
	void processOpenFiles(const QString &p_path,TProcess *p_program);
public:
	inline TLinkList<TProcess> *getPrograms(){ return &programs;}
	inline TLinkList<TOpenFile> *getOpenFiles(){ return &openFiles;}
	
	void processInfo();
};

TLinkList<TProcess> *processesByFile(const QString& p_fileName);
#endif
