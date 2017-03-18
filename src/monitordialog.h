#ifndef __MONITORDIALOG_H_
#define __MONITORDIALOG_H_
#include <QDialog>
#include <QMap>
#include <QTimer>
#include <QStandardItem>
#include "ui_monitor.h"
#include "linklist.h"
#include "filelogitem.h"
class TMonitorDialog:public QDialog{
	Q_OBJECT
private slots:
	void start();
	void refresh();
	void monitorClicked();
	
private:
	Ui::monitor ui;
	QTableView *processGrid;
	TLinkList<TFileLogItem> files;
	QMap<QString,TFileLogItem*> openFiles;
	QTimer refreshTimeout;
	
	uint monitorPid=0;
	void fillProcessSelector();
	void readOpenFiles(int p_id);	
	QStandardItem *createStandardItem(QString p_value,bool p_track,bool p_isNew);
public:
	TMonitorDialog();
	   
};

#endif
