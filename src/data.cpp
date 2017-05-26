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


TFile::TFile(QString p_fileName)
{
	fileName=p_fileName;
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

TOpenFile::TOpenFile(bool p_realFile,TProcess* p_process, long int p_fd, TFile *p_file)
{
	realFile=p_realFile;
	process=p_process;
	fd=p_fd;
	file=p_file;
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
