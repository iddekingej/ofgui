#ifndef __PROCESSITERATOR_H__
#define __PROCESSITERATOR_H__

#include <QString>
#include <QDirIterator>

/**
 * Iterates through all running processes by iterating through all entries  
 */

class TProcessIterator
{
/**
 * Directory iterator , used for scanning the /proc folder
 */
	QDirIterator *dirIter;
/**
 * Current pid  process
 */
	long    id;
/**
 * Current uid of processs
 */
	long    uid;
/**
 * Executable file of process
 */
	QString exe;	
public:
	inline const QString getFilePath(){ return dirIter->filePath();}
	inline long getId(){ return id;}
	inline long getUid(){ return uid;}
	inline const QString &getExe(){ return exe;}	
	bool next();
	TProcessIterator();
	~TProcessIterator();
};
#endif
