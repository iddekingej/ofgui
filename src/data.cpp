#include "data.h"
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include "os.h"
#include "processiterator.h"
#include "openfileiterator.h"
#include "utils.h"


 /**
 *  TProcess object contains information about a process.
 *  
 *  \param p_id Proces id 
 *  \param p_programName name of program belonging to this proces
 */

TProcess::TProcess(long int p_id, const QString& p_programName)
{
	id=p_id;
	programName=p_programName;
}


/**
 *  Add open file object to the openFiles  list 
 * 
 *  \param p_openFile  open file to add to the list
 */

void TProcess::addOpenFile(TOpenFile* p_openFile)
{
	openFiles.append(p_openFile);
}


/**
 *  Open file constructor
 *  This information is read from /proc/#pid#/fd
 *  This folder contains a link (name is fhe fd number) linking to the opened file.
 *  This information is read from /proc/#pid#/fd  see @see TProcessList::processOpenFiles()
 * 
 *  \param p_realFile  True if it is a real file or a socket etc...
 *  \param p_proces    Owning process
 *  \param p_fd        File descriptor id
 *  \param p_fileName  Name of file opened by 
 * 
 */

TOpenFile::TOpenFile(bool p_realFile,TProcess* p_process, long int p_fd, const QString &p_fileName)
{
	realFile=p_realFile;
	process=p_process;
	fd=p_fd;
	fileName=p_fileName;
}

 /**
 *  Read open files from one proces.
 *  The information is read from /proc/#pid#/fd
 *  This folder contain symlinks. The name  is the filediscriptor ID and links
 *  to the open file.
 *  
 *  \param p_path     Path to folder with information about pen files
 *  \param p_process  The openfiles belonging to this process
 */
void TProcessList::processOpenFiles(const QString &p_path,TProcess *p_process)
{
	TOpenFile *l_openFile;
	QString l_file;
	TOpenFileIterator l_iter(p_path);
	while(l_iter.next()){
		l_file=l_iter.getFileName();
		l_openFile=new TOpenFile(l_iter.getRealFile(),p_process,l_iter.getFd(),l_file);
		openFiles.append(l_openFile);
		p_process->addOpenFile(l_openFile);

	}
}

/**
 *  Read all process info and open files
 *  This routine scans /proc folder for numerical folders. 
 *  The name of the folder is the proces ID and the folder contains information about the proces.
 *  
 */
void TProcessList::processInfo()
{
	programs.clear();
	openFiles.clear();	

	getAllUsers(users);
	TProcessIterator l_iter;
	while(l_iter.next()){
		
		TProcess *l_program=new TProcess(l_iter.getId(),l_iter.getExe());
		programs.append(l_program);
		l_program->setOwnerId(l_iter.getUid());
		l_program->setOwner(users.value(l_iter.getUid()));
		processOpenFiles(l_iter.getFilePath()+QStringLiteral("/fd/"),l_program);
		
	}
}



