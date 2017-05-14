#include "openfileiterator.h"

/**
 * Read next entry
 * Each entry represnting a openfile is a symlink. The name is the fd number.
 * The symlink points to the open file. This route resolves the  file name to which the symlink points
 */
bool TOpenFileIterator::next()
{
	bool l_ok;
	while(fileIter->hasNext()){
		fileIter->next();
		fd=fileIter->fileName().toLong(&l_ok);
		if(l_ok){
			fileName=QFile(fileIter->filePath()).symLinkTarget();
			if(fileName.startsWith("/memfd:")){
				fileName=QFileInfo(fileName).fileName();
				realFile=false;	
			} else	if(fileName.startsWith(path)){ //Hack to distinguish between normal and 
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

/**
 * Setup openfile QDirIterator
 * \param p_path Path to folder with information about open files (thats /proc/#pid#/fd)
 */

TOpenFileIterator::TOpenFileIterator(const QString& p_path)
{
	path=p_path;
	fileIter=new QDirIterator(p_path);
	
}

/**
 * Release memory used by openfile iterator
 */

TOpenFileIterator::~TOpenFileIterator()
{
	delete fileIter;
}
