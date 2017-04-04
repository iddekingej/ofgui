#include "monitordialog.h"
#include <QStandardItemModel>
#include <QMapIterator>
#include <QStringBuilder>
#include <QVariant>
#include <QStandardItemModel>
#include <QStandardItem>
#include <klocalizedstring.h>
#include "processiterator.h"
#include "openfileiterator.h"
#include "os.h"
#include "ui_monitor.h"
#include "utils.h"
TMonitorDialog::TMonitorDialog():QDialog()
{
	ui.setupUi(this);
	connect(ui.closeButton,SIGNAL(pressed()),this,SLOT(close()));
	connect(ui.startButton,SIGNAL(pressed()),this,SLOT(start()));
	connect(ui.trackChanges,SIGNAL(clicked()),this,SLOT(monitorClicked()));
	connect(ui.refreshProcessList,SIGNAL(pressed()),this,SLOT(fillProcessSelector()));
	connect(ui.clearChanges,SIGNAL(pressed()),this,SLOT(clearChanges()));
	connect(&refreshTimeout,SIGNAL(timeout()),this,SLOT(refresh()));
	processGrid=new QTableView();
	ui.processSelector->setView(processGrid);
	ui.clearChanges->setVisible(false);
	fillProcessSelector();
	ui.fileList->sortByColumn(2,Qt::AscendingOrder);
}

/**
 * When the "track clanged" checkbox is clicked  the "isNew" (Indicates entry is new or changed) is reset for all items. 
 */
void TMonitorDialog::monitorClicked()
{
	ui.clearChanges->setVisible(ui.trackChanges->checkState()==Qt::Checked);
	clearChanges();
}

/**
 * Clear all change settings
 */
void TMonitorDialog::clearChanges()
{
	TLinkListIterator<TFileLogItem> l_iter(files);
	while(l_iter.hasNext()){
		l_iter.next()->setIsNew(false);
	}
}

/**
 * Fill the process selection list with currently running processes.
 * 
 */
void TMonitorDialog::fillProcessSelector()
{
	QMap<uint,QString> l_users;
	TProcessIterator l_iter;
	int l_cnt=0;
	QString l_userName;
	
	QStandardItemModel *l_model=new QStandardItemModel(3,0,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Pid")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("User")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Program")));
	
	getAllUsers(l_users);
	while(l_iter.next()){
		if(l_iter.getExe().length()>0){
			QStandardItem *l_programName=new QStandardItem(l_iter.getExe());
			l_programName->setData(QVariant((uint)(l_iter.getId())), Qt::UserRole + 1);

			l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_iter.getId())) );
			l_userName=l_users.value(l_iter.getUid());
			l_model->setItem(l_cnt,1,new QStandardItem(l_userName));
			l_model->setItem(l_cnt,2,l_programName);
			l_cnt++;
		}
	}
	
	ui.processSelector->setModel(l_model);
	ui.processSelector->setModelColumn(2);
	processGrid->resizeColumnsToContents();
	processGrid->resizeRowsToContents();
	processGrid->setSortingEnabled(true);
	processGrid->sortByColumn(2,Qt::AscendingOrder);
	
}

/**
 * When the start/stop button is pressed (is same button).
 * 
 * When start is pressed, the refresh timer is started and some widgets are disabled.
 * When stop is pressed, the refresh timer is stopped and the disabled widgets are enabled again.
 */

void TMonitorDialog::start()
{
	if(refreshTimeout.isActive()){
		refreshTimeout.stop();
		ui.startButton->setText(i18n("Start"));
		ui.processSelector->setDisabled(false);

	} else {
		files.clear();
		openFiles.clear();
		monitorPid=ui.processSelector->currentData(Qt::UserRole+1).toInt();
		refreshTimeout.start(100);
		ui.startButton->setText(i18n("Stop"));
		ui.processSelector->setDisabled(true);

	}
}


/**
 * Create QStandardItem.
 * 
 * When tracking is on and item is new (or is changed) than the back ground is set to a different color.
 * 
 * \param p_value  make item with this value
 * \param p_track  Tracking is enabled 
 * \param p_isnew  A new or changed entry
 */
QStandardItem * TMonitorDialog::createStandardItem(QString p_value, bool p_track, bool p_isNew)
{
	QStandardItem *l_item=new QStandardItem(p_value);
	if(p_track && p_isNew){
		l_item->setBackground(Qt::green);
	}
	return l_item;
}

/**
 * Rereads open files and refrehses the table view.
 */

void TMonitorDialog::refresh()
{
	int l_sortColumn=ui.fileList->verticalHeader()->sortIndicatorSection();
	Qt::SortOrder l_order=ui.fileList->verticalHeader()->sortIndicatorOrder();
	readOpenFiles(monitorPid);
	bool l_onlyRealFiles=ui.onlyRealFiles->checkState()==Qt::Checked;
	bool l_trackChanges=ui.trackChanges->checkState()==Qt::Checked;
	QStandardItemModel *l_model =new QStandardItemModel(0,4,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Fd.")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Name")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Open time")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("Close time")));
	TLinkListIterator<TFileLogItem> l_iter(files);
	int l_cnt=0;
	TFileLogItem *l_item;

	while(l_iter.hasNext()){

		l_item=l_iter.next();
		if(l_onlyRealFiles){
			if(!l_item->getRealFile()) continue;
		}
		
		l_model->setItem(l_cnt,0, createStandardItem(QString::number((uint)l_item->getFd()),l_trackChanges,l_item->getIsNew()));
		l_model->setItem(l_cnt,1, createStandardItem(l_item->getFileName(),l_trackChanges,l_item->getIsNew()));
		l_model->setItem(l_cnt,2, createStandardItem(l_item->getOpenAt().toString(Qt::ISODate),l_trackChanges,l_item->getIsNew()));
		l_model->setItem(l_cnt,3, createStandardItem(l_item->getClosedAt().toString(Qt::ISODate),l_trackChanges,l_item->getIsNew()));
		l_cnt++;		
	}

	setViewModel(ui.fileList,l_model);
	ui.fileList->resizeColumnsToContents();
	ui.fileList->resizeRowsToContents();
	ui.fileList->sortByColumn(l_sortColumn,l_order);
}


/**
 * Reread all open files
 * files contains all files (open/closed) and openFiles contain all open files.
 * the data is processed as follows.
 * -Set open check on all open files to false (this will be used later to filter files that aren't open anymore)
 * -Read all open files.
 * -When file not in open file list, make new entry
 * -When file is in open file list, set the "open check". This is used in the next step to filter out closed files
 * -Filter out closed files. All FileLogItems with "open check" is false is removed from the open file list.
 * 
 * \param p_id  read open files from this process
 */
void TMonitorDialog::readOpenFiles(int p_id)
{
	TFileLogItem *l_fileLog;
	QMapIterator<QString,TFileLogItem*> l_iter(openFiles);
	while(l_iter.hasNext()){
		l_iter.next();
		l_iter.value()->setOpenCheck(false);
	}
	QString l_ofPath=QStringLiteral("/proc/") % QString::number(p_id) % QStringLiteral("/fd/");
	TOpenFileIterator l_of(l_ofPath);
	while(l_of.next()){
		l_fileLog=openFiles.value(l_of.getFileName());
		if(l_fileLog != nullptr){
			l_fileLog->setOpenCheck(true);
		} else {
			l_fileLog=new TFileLogItem(l_of.getFd(),l_of.getRealFile(),l_of.getFileName());
			l_fileLog->setIsNew(true);
			files.append(l_fileLog);
			openFiles.insert(l_of.getFileName(),l_fileLog);
		}
	}
	QMapIterator<QString,TFileLogItem*> l_cleanIter(openFiles);
	while(l_cleanIter.hasNext()){
		l_cleanIter.next();
		l_fileLog=l_cleanIter.value();
		if(!l_fileLog->getOpenCheck()){			
			openFiles.remove(l_fileLog->getFileName());
			l_fileLog->fileIsClosed();
			l_fileLog->setIsNew(true);
		}
	}
}
