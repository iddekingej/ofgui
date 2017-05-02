#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#include <QDialog>
#include <QString>
#include "ui_fileinfo.h"
class TFileInfo:public QDialog{
	Q_OBJECT

private:
	QString fileName;
	Ui::fileInfo ui;
public:
	TFileInfo(const QString p_fileName);
};
#endif