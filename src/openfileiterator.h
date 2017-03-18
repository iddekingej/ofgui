#ifndef __OPENFILEITERATOR_H_
#define __OPENFILEITERATOR_H_
#include <QDirIterator>
#include <QString>
class TOpenFileIterator
{
private:
	QString path;
	QDirIterator *fileIter;
	QString fileName;
	int fd;
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
