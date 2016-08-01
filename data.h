#ifndef __DATA_H_
#define __DATA_H_

#include <QList>
#include "linklist.h"

class TOpenFile;

class TProgram{
private:
	QList<TOpenFile*> openFiles;
	long    id;
	QString programName;
public:
	inline bool hasOpenFile(){ return openFiles.length()>0;}
	inline QList<TOpenFile*> *getOpenFiles(){ return &openFiles;}
	inline int getId(){ return id;}
	inline const QString &getProgramName(){ return programName;}
	void addOpenFile(TOpenFile *p_openFile);
	TProgram(long p_id,const QString &p_programName);
};

class TOpenFile
{
private:
	TProgram *owner;
	long     fd;
	QString  fileName;
	bool     realFile;
public:
	inline TProgram *getOwner(){ return owner;}
	inline long getFd(){ return fd;}
	inline const QString &getFileName(){ return fileName;}
	inline bool getRealFile(){return realFile;}
	TOpenFile(bool p_realFile,TProgram *p_owner,long p_fd,const QString &p_fileName);
};


class TOpenFileList
{
private:
	TLinkList<TProgram>  programs;
	TLinkList<TOpenFile> openFiles;
	
	void processOpenFiles(const QString &p_path,TProgram *p_program);
	void processProc(const QString &p_fileName,long p_id);
public:
	inline TLinkListItem<TProgram> *getProgramsStart(){ return programs.getStart();}
	inline TLinkListItem<TOpenFile> *getOpenFilesStart(){ return openFiles.getStart();}
	
	void processInfo();
};

#endif