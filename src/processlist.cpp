#include <QTextStream>
#include <QStringRef>
#include <QVector>
#include <QString>
#include "processlist.h"
#include "processiterator.h"
#include "os.h"

TFile * TProcessList::addGetFile(bool p_realFile,const QString& p_fileName)
{
	TFile *l_file=files.value(p_fileName);
	if(nullptr==l_file){
		l_file=new TFile(p_fileName);
		files.insert(p_fileName,l_file);
		l_file->setFileType(getFileType(p_realFile,p_fileName));

	}
	return l_file;
}


void TProcessList::processFdInfoFlags(TOpenFile* p_openFile, QStringRef p_flags)
{
	bool l_ok;
	int l_flags=p_flags.toInt(&l_ok,8);	
	if(l_ok){
		p_openFile->setOpenMode(l_flags);
	}
}


void TProcessList::processFdInfo(TOpenFile* p_openFile)
{
	QString l_name=QString("%0/%1/fdinfo/%2").arg(procPath,QString::number(p_openFile->getProcess()->getId()),QString::number(p_openFile->getFd()));
	QFile l_file(l_name);
	if(!l_file.open(QIODevice::ReadOnly|QIODevice::Text)){
		return;
		
	}
	QTextStream l_stream(&l_file);
	QString l_line;
	while(true){
		l_line=l_stream.readLine();
		if(l_line.isNull()) break;		
		if(l_line.length()>0){
			QVector<QStringRef> l_split=l_line.splitRef(":");
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
TFileType TProcessList::getFileType(bool p_realFile,const QString &p_fileName){
	if(p_realFile){
		return getOSFileType(p_fileName);
	}
	if(p_fileName.startsWith("socket:")){
		return TFileType::DT_SOCKET;
	}
	if(p_fileName.startsWith("pipe:")){
		return TFileType::DT_PIPE;
	}
	return TFileType::DT_UNKOWN;
}

/**
 * Create new open file object
 * 
 * \param p_openFileIter    open file iterator. Current openfile is added
 * \param p_process         current process that is processed
 * 
 */
TOpenFile *TProcessList::processOpenFile(TOpenFileIterator &p_openFileIter,TProcess *p_process){
	TOpenFile *l_openFile;
	QString l_fileName;
	TFile *l_file;
	l_fileName=p_openFileIter.getFileName();
	l_file=addGetFile(p_openFileIter.getRealFile(),l_fileName);
	l_openFile=new TOpenFile(p_openFileIter.getRealFile(),p_process,p_openFileIter.getFd(),l_file);		
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


TProcessList::~TProcessList()
{
	qDeleteAll(files);
}
