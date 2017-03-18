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
TProcess::TProcess(long int p_id, const QString& p_programName)
{
	id=p_id;
	programName=p_programName;
}



/**
 * Add information about open file to process
 * 
 * \param p_openFile  Openfile to add to the process
 */
void TProcess::addOpenFile(TOpenFile* p_openFile)
{
	openFiles.append(p_openFile);
}


TOpenFile::TOpenFile(bool p_realFile,TProcess* p_owner, long int p_fd, const QString &p_fileName)
{
	realFile=p_realFile;
	owner=p_owner;
	fd=p_fd;
	fileName=p_fileName;
}

/**
 * Information about open files from a proces is stored in /proc/#pid#/fd (where #pid#=process id)
 * This method reads this information and creates TOpenFile object for each open file
 * 
 * \param p_path  Path to open file information /proc/#pid#/fd
 * \param p_program Object representing the process Information about open file is added to this object
 */

void TProcessList::processOpenFiles(const QString &p_path,TProcess *p_program)
{
	TOpenFile *l_openFile;
	QString l_file;
	TOpenFileIterator l_iter(p_path);
	while(l_iter.next()){
		l_file=l_iter.getFileName();
		l_openFile=new TOpenFile(l_iter.getRealFile(),p_program,l_iter.getFd(),l_file);
		openFiles.append(l_openFile);
		p_program->addOpenFile(l_openFile);
	}
}


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



