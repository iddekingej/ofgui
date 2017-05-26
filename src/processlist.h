#ifndef __PROCESSLIST_H_
#define __PROCESSLIST_H_
#include <QMap>
#include <QString>
#include "linklist.h"
#include "data.h"

/**
 * Container contains all running processes and open files
 */

class TProcessList
{
private:
	/**
	 * Path to proc infomation. Can be changed for testing
	 */
	QString procPath="/proc";
	/**
	 * List of running processes
	 */
	TLinkList<TProcess>  programs;
	
	/**
	 * List of open program
	 */
	TLinkList<TOpenFile> openFiles;
	
	/**
	 * Mapping uid to user name
	 */
	QMap<uint,QString>   users;
	
	QMap<QString, TFile*> files;
	
	TFile *addGetFile(bool p_realFile,const QString &p_fileName);
	
	void processFdInfoFlags(TOpenFile *p_openFile,QStringRef p_flags);	
	void processFdInfo(TOpenFile *p_openFile);
	TFileType getFileType(bool p_realFile,const QString &p_fileName);
	TOpenFile *processOpenFile(TOpenFileIterator &p_openFileIter,TProcess *p_process);
	void processOpenFiles(const QString &p_path,TProcess *p_program);
protected:
	virtual bool filterFiles(TOpenFileIterator &p_openFileIter);
public:
	inline void setProcPath(const QString &p_path){ procPath=p_path;}
	inline const QString &getProcPath(){ return procPath;}
	inline TLinkList<TProcess> *getPrograms(){ return &programs;}
	inline TLinkList<TOpenFile> *getOpenFiles(){ return &openFiles;}
	inline QMap<QString,TFile *>  *getFiles(){ return &files;}
	void processInfo();
	virtual ~TProcessList();
};

class TProcessSignleFileList:public TProcessList
{
private:
	QString filterFileName;
protected:
	virtual bool filterFiles(TOpenFileIterator &p_openFileIter) override;
public:
	inline void setFilterFileName(QString &p_fileName){
		filterFileName=p_fileName;
	}
	
	inline QString &getFilterFileName()
	{
		return filterFileName; 
	}
};

#endif
