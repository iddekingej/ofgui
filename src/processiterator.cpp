#include "processiterator.h"


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

TProcessIterator::TProcessIterator()
{
	dirIter=new QDirIterator("/proc");
}

TProcessIterator::~TProcessIterator()
{
	delete dirIter;
}
