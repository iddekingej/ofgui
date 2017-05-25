#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#include <QDialog>
#include <QString>
#include <QFileSystemWatcher>

#include "ui_fileinfo.h"
class TFileInfo:public QDialog{
	Q_OBJECT
private slots:
	void fillData();
	void fileChanged(const QString &p_path);
private:
	QString fileName;
	QFileSystemWatcher notify;
	Ui::fileInfo ui;
public:
	TFileInfo(const QString p_fileName);
};
#endif
