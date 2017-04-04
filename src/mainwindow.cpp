
#include "mainwindow.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QCheckBox>
#include <QTimer>
#include <QFileInfo>
#include <klocalizedstring.h>
#include <iostream>
#include <QAction>
#include "data.h"
#include "linklist.h"
#include "globals.h"
#include "os.h"
#include "monitordialog.h"
#include "about.h"
#include "utils.h"

/**
 *  Main window constructor
 *  Setup the window gui and the gui events
 */

TMainWindow::TMainWindow(QWidget* p_parent):QMainWindow(p_parent)
{
	ui.setupUi(this);

	processSelection=new QTableView();
	ui.processSelection->setView(processSelection);;	
	processSelection->setSelectionMode(QAbstractItemView::SingleSelection);
	processSelection->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(ui.processSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(processChanged(int)));	
	connect(ui.onlyRealFiles,SIGNAL(stateChanged(int)),this,SLOT(onlyRealFilesChecked(int)));
	connect(ui.checkRefresh,SIGNAL(stateChanged(int)),this,SLOT(checkRefresh(int)));	
	connect(ui.refreshTime,SIGNAL(editingFinished()),this,SLOT(timeOutChanged()));
	connect(ui.action_Exit,&QAction::triggered,g_app,QApplication::quit);
	connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(about()));
	connect(ui.actionMonitor,SIGNAL(triggered()),this,SLOT(openMonitor()));
	connect(ui.refreshButton,SIGNAL(pressed()),this,SLOT(refresh())); 
	connect(ui.searchButton,SIGNAL(pressed()),this,SLOT(refresh())); 
	connect(ui.searchText,SIGNAL(returnPressed()),this,SLOT(refresh()));
	connect(&refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()));
	refresh();
	checkRefresh(0);
	ui.refreshTime->setText("1");
}

void TMainWindow::about()
{
	TAbout l_dialog;
	l_dialog.exec();
}



/**
 * Display about dialog
 */

/**
 * Open monitor dialog
 */
void TMainWindow::openMonitor()
{
	TMonitorDialog l_dialog;
	l_dialog.exec();
}


/**
 *  On the main window there is a checkbox "only real files".
 *  When the checkbox is toggled this slot is called and the open file list is refresh
 *  reflecting the new selection.
 */

void TMainWindow::onlyRealFilesChecked(int p_state PAR_UNUSED)
{
	fillOpenFileGrid();
}

/**
 *  After the proces selection is clanged, this slot is called.
 *  The open file list is refreshed so only the open files of the selected proces are displayed
 */

void TMainWindow::processChanged(int p_index PAR_UNUSED)
{
	fillOpenFileGrid();	
}

/**
 *  When ui.refreshTime input is changed this routine is called.
 */
void TMainWindow::timeOutChanged()
{
	checkRefresh(0);
}

/**
 *  Reread all proces and open file information and refreshed the open file list
 *  and the process selector.
 */

void TMainWindow::refresh()
{
	processList.processInfo();
	setProgramSelector();
	fillOpenFileGrid();
	
}

/**
 * Event when "Refresh" checbox is checked of unchecked.
 * When checked an interval input is displayed  and the refresh timer is started
 * When unchecked  the interval input is hidden and the refresh timer is stopped.
 * This routine is also called when refreshTime is change \ref timeOutChanged 
 * 
 * \param p_state no used
 */

void TMainWindow::checkRefresh(int p_state PAR_UNUSED)
{
	
	if(ui.checkRefresh->checkState()==Qt::Checked){
		ui.refreshTime->setVisible(true);
		float l_value;
		bool l_ok;		
		l_value=ui.refreshTime->text().toFloat(&l_ok);
		if(l_value==0) {
			l_value=1;
		}
		if(l_ok){
			refreshTimer.start(int(l_value*1000));
		} else {
			refreshTimer.stop();
		}
	} else {
		ui.refreshTime->setVisible(false);
		refreshTimer.stop();
	}
}

/**
 *  On the main Gui there is a selection list with currently running processes.
 *  When a proces is selected, only files used by this proces are displayed
 *  Only files which has open files are added to the selection list.
 * 
 *  This routine fills this list
 */

void TMainWindow::setProgramSelector()
{
	ui.processSelection->blockSignals(true);	
	TLinkListIterator<TProcess> l_procIter(processList.getPrograms());
	QStandardItemModel *l_model=new QStandardItemModel(0,4,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Program")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Proces ID")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Path")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("User")));
	QStandardItem *l_item;	
	QString l_path;
	int l_cnt=1;
	l_item=new QStandardItem(i18n("All processes"));
	l_item->setData(QVariant(0),Qt::UserRole + 1);
	l_model->setItem(0,0,l_item);
	l_model->setItem(0,1,new QStandardItem(""));
	l_model->setItem(0,2,new QStandardItem(""));
	long l_selectedId=ui.processSelection->currentData(Qt::UserRole+1).toInt();
	long l_selectedIdx=0;
	TProcess *l_process;
	while(l_procIter.hasNext()){
		l_process=l_procIter.next();
		if(l_process->hasOpenFile()){
			QFileInfo l_info(l_process->getProgramName());
			l_item=new QStandardItem(l_info.fileName());
			l_item->setData(QVariant(l_process->getId()), Qt::UserRole + 1);			
			if(l_process->getId()==l_selectedId){
				l_selectedIdx=l_cnt;
			}
			l_model->setItem(l_cnt,1,new QStandardItem(QString::number(l_process->getId())));
			l_model->setItem(l_cnt,0,l_item);
			l_model->setItem(l_cnt,2,new QStandardItem(l_info.filePath()));
			l_model->setItem(l_cnt,3,new QStandardItem(l_process->getOwner()));
			l_cnt++;
		}
		
	}
	ui.processSelection->setModel(l_model);	
	ui.processSelection->setModelColumn(0);
	ui.processSelection->setCurrentIndex(l_selectedIdx);
	processSelection->resizeColumnsToContents();
	processSelection->resizeRowsToContents();
	ui.processSelection->blockSignals(false);
}


/**
 *  This method fills open file grid with open files.
 */

void TMainWindow::fillOpenFileGrid()
{
	TLinkListIterator<TOpenFile> l_ofIter(processList.getOpenFiles());
	QStandardItemModel *l_model=new QStandardItemModel(0,5,this);
	QVariant l_selected=ui.processSelection->currentData(Qt::UserRole + 1);
	long l_programId=l_selected.toUInt();
	int l_cnt=0;
	bool l_onlyRealFiles=(ui.onlyRealFiles->checkState()==Qt::Checked);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("File descr.")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("File name")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Prog. id.")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("Prog. name")));
	l_model->setHorizontalHeaderItem(4,new QStandardItem(i18n("Proc. owner")));
	QString l_searchText=ui.searchText->text();
	TOpenFile *l_openFile;
	while (l_ofIter.hasNext()){
		l_openFile=l_ofIter.next();
		if((l_programId ==0 || (l_openFile->getProcess()->getId()==l_programId))
		&& (!l_onlyRealFiles|| l_openFile->getRealFile())
		&& (l_searchText.length()==0 || l_openFile->getFileName().toLower().contains(l_searchText))		
		) {
			l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_openFile->getFd())));
			l_model->setItem(l_cnt,1,new QStandardItem(l_openFile->getFileName()));
			l_model->setItem(l_cnt,2,new QStandardItem(QString::number(l_openFile->getProcess()->getId())));
			l_model->setItem(l_cnt,3,new QStandardItem(l_openFile->getProcess()->getProgramName()));
			l_model->setItem(l_cnt,4,new QStandardItem(l_openFile->getProcess()->getOwner()));
			l_cnt++;

		}		
	}
	ui.openFileList->setModel(l_model);
	ui.openFileList->resizeColumnsToContents();	
	ui.openFileList->resizeRowsToContents();
}