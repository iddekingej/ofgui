#include "data.h"
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include "os.h"


/**
*Used for reading files in /sys/proc
*Read a single line from a file
* 
*\param p_path   Base path  to file
*\param p_name   File name under path to read
*\param p_value  Read one line  from p_path+"/"+p_file and returns it in p_value
*\return         true when successful  false file reading error
*/

bool readString(QString p_path,QString p_name,QString &p_value)
{
	bool l_succes;
	QDir l_dir=QDir(p_path);	
	QFile l_file(l_dir.absoluteFilePath(p_name));
	if(!l_file.open(QIODevice::ReadOnly|QIODevice::Text)){
		p_value= QStringLiteral("");
		return false;
	}
	QTextStream l_stream(&l_file);
	QString l_value=l_stream.readLine();
	p_value=l_value.trimmed();
	l_succes=(l_stream.status() ==  QTextStream::Ok);
	l_file.close();
	
	return l_succes;
}

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
 * Read one line from file in p_path+'/'+p_name , and converts it to a long
 * 
 *\param p_path       Path 
 *\param p_name       File name under path to read
 *\param p_value[out] Returned value
 *\Return             true when successful  false file reading error
*/

bool readLong(QString p_path,QString p_name,unsigned long &p_value)
{
	QString l_string;
	p_value=0;
	bool l_ok;
	if(!readString(p_path,p_name,l_string)) return false;
	p_value=l_string.toULong(&l_ok);
	return l_ok;
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
	QDir l_ofd(p_path);
	QDirIterator l_ofdi(l_ofd);
	QString l_file;
	long l_fd;
	bool l_ok;
	bool l_realFile;
	
	while(l_ofdi.hasNext()){
		l_ofdi.next();
		l_fd=l_ofdi.fileName().toLong(&l_ok);
		if(l_ok){
			QFile l_fi(l_ofdi.filePath());
			l_file=l_fi.symLinkTarget();
			if(l_file.startsWith(p_path)){ // cludge :filter out special files
				l_realFile=false;
				l_file=QFileInfo(l_file).fileName();
			}else {
				l_realFile=true;
			}
			l_openFile=new TOpenFile(l_realFile,p_process,l_fd,l_file);
			openFiles.append(l_openFile);
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
	QDirIterator l_pi("/proc");
	QString p_fileName;
	bool l_ok;
	long l_id;
	long l_userId;
	getAllUsers(users);
	while(l_pi.hasNext()){
		l_pi.next();
		l_id=l_pi.fileName().toLong(&l_ok);
		if(l_ok){
			TProcess *l_program=new TProcess(l_id,QFile::symLinkTarget(l_pi.filePath()+QStringLiteral("/exe")));
			programs.append(l_program);
			l_userId=l_pi.fileInfo().ownerId();
			l_program->setOwnerId(l_userId);
			l_program->setOwner(users.value(l_userId));
			processOpenFiles(l_pi.filePath()+"/fd/",l_program);
		}
	}
}



