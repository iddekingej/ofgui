#include "data.h"
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QFile>


//Used for reading files in /sys/block
//Read file containing string (like /sys/block/sda/mode)
//p_path  - Path 
//p_name  - File name under path to read
//p_value - Returned value
//Return    true when successful  false file reading error

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
	p_value=l_stream.readLine();
	l_succes=(l_stream.status() ==  QTextStream::Ok);
	l_file.close();
	
	return l_succes;
}

//Used for reading files in /sys/block
//Read file containing ulong (like /sys/block/sda/size)
//p_path  - Path 
//p_name  - File name under path to read
//p_value - Returned value
//Return    true when successful  false file reading error

bool readLong(QString p_path,QString p_name,unsigned long &p_value)
{
	QString l_string;
	p_value=0;
	if(!readString(p_path,p_name,l_string)) return false;
	p_value=l_string.toULong();
	return true;
}

TProgram::TProgram(long int p_id, const QString& p_programName)
{
	id=p_id;
	programName=p_programName;
}

void TProgram::addOpenFile(TOpenFile* p_openFile)
{
	openFiles.append(p_openFile);
}

TOpenFile::TOpenFile(bool p_realFile,TProgram* p_owner, long int p_fd, const QString &p_fileName)
{
	realFile=p_realFile;
	owner=p_owner;
	fd=p_fd;
	fileName=p_fileName;
}

void TOpenFileList::processOpenFiles(const QString &p_path,TProgram *p_program)
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
			l_openFile=new TOpenFile(l_realFile,p_program,l_fd,l_file);
			openFiles.append(l_openFile);
			p_program->addOpenFile(l_openFile);
		}
	}
}

void TOpenFileList::processProc(const QString &p_path,long p_id)
{
	QDir l_dir(p_path);
	QFile l_file(l_dir.absoluteFilePath("exe"));
	
	TProgram *l_program=new TProgram(p_id,l_file.symLinkTarget().toUtf8().data());
	programs.append(l_program);
	processOpenFiles(l_dir.absoluteFilePath("fd"),l_program);
}

void TOpenFileList::processInfo()
{
	programs.clear();
	openFiles.clear();
	QDir l_procs("/proc");
	QDirIterator l_pi(l_procs);
	QString p_fileName;
	bool l_ok;
	long l_id;
	while(l_pi.hasNext()){
		l_pi.next();
		l_id=l_pi.fileName().toLong(&l_ok);
		if(l_ok){
			processProc(l_pi.filePath(),l_id);			
		}
	}
}



