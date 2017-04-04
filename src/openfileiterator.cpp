#include "openfileiterator.h"

bool TOpenFileIterator::next()
{
	bool l_ok;
	while(fileIter->hasNext()){
		fileIter->next();
		fd=fileIter->fileName().toLong(&l_ok);
		if(l_ok){
			fileName=QFile(fileIter->filePath()).symLinkTarget();
			if(fileName.startsWith(path)){
				fileName=QFileInfo(fileName).fileName();
				realFile=false;
			} else {
				realFile=true;
			}
			return true;
		}
	}
	return false;
}

TOpenFileIterator::TOpenFileIterator(const QString& p_path)
{
	path=p_path;
	fileIter=new QDirIterator(p_path);
	
}

TOpenFileIterator::~TOpenFileIterator()
{
	delete fileIter;
}
