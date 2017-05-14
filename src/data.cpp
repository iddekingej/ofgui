#include "data.h"
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <fcntl.h>
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
 *  \param p_process    Owning process
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


QString TOpenFile::getOpenModeDescription()
{
	QString l_mode;
	switch(openMode & O_ACCMODE){
		case O_RDONLY:l_mode="Read ";break;
		case O_WRONLY: l_mode="Write ";break;
		case O_RDWR  : l_mode="Read/Write ";break;
	}
	return l_mode;
}


void TProcessList::processFdInfoFlags(TOpenFile* p_openFile, QStringRef p_flags)
{
	bool l_ok;
	int l_flags=p_flags.toInt(&l_ok,8);
	printf("%s=>%d \n ",qstr(p_flags),l_flags);
	if(l_ok){
		p_openFile->setOpenMode(l_flags);
	}
}


void TProcessList::processFdInfo(TOpenFile* p_openFile)
{
	QString l_name=QString("/proc/%0/fdinfo/%1").arg(QString::number(p_openFile->getProcess()->getId()),QString::number(p_openFile->getFd()));
	QFile l_file(l_name);
	if(!l_file.open(QIODevice::ReadOnly|QIODevice::Text)){
		printf("Failed:%s \n",qstr(l_name));
		return;
		
	}
	QTextStream l_stream(&l_file);
	QString l_line;
	while(true){
		l_line=l_stream.readLine();
		if(l_line.isNull()) break;		
		if(l_line.length()>0){
			QVector<QStringRef> l_split=l_line.splitRef(":");
			printf("%s %d \n",qstr(l_line),l_split.length());
			if(l_split.length()==2){
				if(l_split.first()=="flags"){
					processFdInfoFlags(p_openFile,l_split.last().trimmed());
				}
			}
		}
	}

}

/**
 * Get file type, see \ref TFileType
 * When \a p_realFile =true the file type is determined by stat fs call
 * Otherwise the file type is determined by its file name (\a p_fileName).
 * 
 * \param p_realFile If file name points to a file on the fs
 * \param p_fileName File name.
 */
TFileType TProcessList::getFileType(bool p_realFile,QString &p_fileName){
	if(p_realFile){
		return getOSFileType(p_fileName);
	}
	if(p_fileName.startsWith("socket:")){
		return DT_SOCKET;
	}
	if(p_fileName.startsWith("pipe:")){
		return DT_PIPE;
	}
	return DT_UNKOWN;
}

TOpenFile *TProcessList::processOpenFile(TOpenFileIterator &p_openFileIter,TProcess *p_process){
	TOpenFile *l_openFile;
	QString l_file;
	l_file=p_openFileIter.getFileName();
	l_openFile=new TOpenFile(p_openFileIter.getRealFile(),p_process,p_openFileIter.getFd(),l_file);		
	l_openFile->setFileType(getFileType(p_openFileIter.getRealFile(),l_file));
	processFdInfo(l_openFile);
	p_process->addOpenFile(l_openFile);
	openFiles.append(l_openFile);
	return l_openFile;
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
		if(filterFiles(l_iter)){
			l_openFile=processOpenFile(l_iter,p_process);
			p_process->addOpenFile(l_openFile);
		}

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

bool TProcessList::filterFiles(TOpenFileIterator& p_openFileIter)
{
	return true;
}

bool TProcessSignleFileList::filterFiles(TOpenFileIterator& p_openFileIter)
{
	return p_openFileIter.getFileName()==filterFileName;
}


