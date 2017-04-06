#include "processiterator.h"

/**
 * Read the next entry.
 * Each entry is a folder in the /proc folder.
 * When this is a number it's a pid of a runing process
 * Under this folder there is a symlink with name exe that points to the executable file
 */
bool TProcessIterator::next()
{
	bool l_ok;
	
	while(dirIter->hasNext()){
		dirIter->next();
		id=dirIter->fileName().toLong(&l_ok);
		if(l_ok){
			uid=dirIter->fileInfo().ownerId();
			exe=QFile::symLinkTarget(dirIter->filePath()+QStringLiteral("/exe"));
			return true;
		}
	}
	return false;
}


/**
 * Initialize process iterator 
 * THe information is read from the /proc folder
 */
TProcessIterator::TProcessIterator()
{
	dirIter=new QDirIterator("/proc");
}

TProcessIterator::~TProcessIterator()
{
	delete dirIter;
}
