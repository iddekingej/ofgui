#ifndef __OPENFILEITERATOR_H_
#define __OPENFILEITERATOR_H_
#include <QDirIterator>
#include <QString>

/**
 * This iterator scanes all open files belonging to a process
 * 
 * The folder /proc/#pid#/fd contains all  open files. This iterator scans
 * thisf older and returns all open files
 */

class TOpenFileIterator
{
private:
/**
 * Path where information about open files are stored
 */
	QString path;
/**
 * Directory Iterator  used for scanning open file information
 */
	QDirIterator *fileIter;
	
/**
 * Current open file
 */
	QString fileName;
/**
 * Fd of current item
 */
	int fd;
/**
 * If current file is a real file
 */
	bool realFile;
public:
	inline QString &getFileName(){ return fileName;};
	inline int &getFd(){ return fd;}
	inline bool getRealFile(){ return realFile;};
	bool next();
	TOpenFileIterator(const QString &p_path);
	~TOpenFileIterator();
};
#endif
