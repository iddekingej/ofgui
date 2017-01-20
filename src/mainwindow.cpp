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

TMainWindow::TMainWindow(QWidget* p_parent):QMainWindow(p_parent)
{
	ui.setupUi(this);

	processSelection=new QTableView();
	ui.processSelection->setView(processSelection);;	
	processSelection->setSelectionMode(QAbstractItemView::SingleSelection);
	processSelection->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(ui.processSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(programChanged(int)));	
	connect(ui.onlyRealFiles,SIGNAL(stateChanged(int)),this,SLOT(onlyRealFilesChecked(int)));
	connect(ui.checkRefresh,SIGNAL(stateChanged(int)),this,SLOT(checkRefresh(int)));	
	connect(ui.refreshTime,SIGNAL(editingFinished()),this,SLOT(timeOutChanged()));
	connect(ui.action_Exit,&QAction::triggered,g_app,QApplication::quit);
	connect(ui.refreshButton,SIGNAL(pressed()),this,SLOT(refresh())); 
	connect(ui.searchButton,SIGNAL(pressed()),this,SLOT(refresh())); 
	connect(ui.searchText,SIGNAL(returnPressed()),this,SLOT(refresh()));
	connect(&refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()));
	refresh();
	checkRefresh(0);
	ui.refreshTime->setText("1");
}


void TMainWindow::onlyRealFilesChecked(int p_state)
{
	fillOpenFileGrid();
}

void TMainWindow::programChanged(int p_index)
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


void TMainWindow::refresh()
{
	openFileList.processInfo();
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

void TMainWindow::checkRefresh(int p_state)
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
			std::cout  <<"Refresh changed" <<std::endl;
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
 * 
 *  This routine fills this list
 */

void TMainWindow::setProgramSelector()
{
	ui.processSelection->blockSignals(true);
	TLinkListItem<TProcess> *l_current=openFileList.getProgramsStart();
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
	while(l_current != nullptr){
		if(l_current->getItem()->hasOpenFile()){
			QFileInfo l_info(l_current->getItem()->getProgramName());
			l_item=new QStandardItem(l_info.fileName());
			l_item->setData(QVariant(l_current->getItem()->getId()), Qt::UserRole + 1);			
			if(l_current->getItem()->getId()==l_selectedId){
				l_selectedIdx=l_cnt;
			}
			l_model->setItem(l_cnt,1,new QStandardItem(QString::number(l_current->getItem()->getId())));
			l_model->setItem(l_cnt,0,l_item);
			l_model->setItem(l_cnt,2,new QStandardItem(l_info.filePath()));
			l_model->setItem(l_cnt,3,new QStandardItem(l_current->getItem()->getOwner()));
			l_cnt++;
		}
		l_current=l_current->getNext();
		
	}
	ui.processSelection->setModel(l_model);	
	//programSelectList->setSortingEnabled(true);
	ui.processSelection->setModelColumn(0);
	ui.processSelection->setCurrentIndex(l_selectedIdx);
	processSelection->resizeColumnsToContents();
	processSelection->resizeRowsToContents();
	ui.processSelection->blockSignals(false);
}


/**
 *  Fill open file grid with open files.
 */

void TMainWindow::fillOpenFileGrid()
{
	TLinkListItem<TOpenFile> *l_current=openFileList.getOpenFilesStart();
	QStandardItemModel *l_model=new QStandardItemModel(0,5,this);
	QVariant l_selected=ui.processSelection->currentData(Qt::UserRole + 1);
	long     l_programId=l_selected.toUInt();
	int l_cnt=0;
	bool l_onlyRealFiles=(ui.onlyRealFiles->checkState()==Qt::Checked);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("File descr.")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("File name")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Prog. id.")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("Prog. name")));
	l_model->setHorizontalHeaderItem(4,new QStandardItem(i18n("Proc. owner")));
	QString l_searchText=ui.searchText->text();
	while (l_current != nullptr){
		if((l_programId ==0 || (l_current->getItem()->getOwner()->getId()==l_programId))
		&& (!l_onlyRealFiles|| l_current->getItem()->getRealFile())
		&& (l_searchText.length()==0 || l_current->getItem()->getFileName().toLower().contains(l_searchText))		
		) {
			l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_current->getItem()->getFd())));
			l_model->setItem(l_cnt,1,new QStandardItem(l_current->getItem()->getFileName()));
			l_model->setItem(l_cnt,2,new QStandardItem(QString::number(l_current->getItem()->getOwner()->getId())));
			l_model->setItem(l_cnt,3,new QStandardItem(l_current->getItem()->getOwner()->getProgramName()));
			l_model->setItem(l_cnt,4,new QStandardItem(l_current->getItem()->getOwner()->getOwner()));
			l_cnt++;

		}
		l_current=l_current->getNext();
	}
	ui.openFileList->setModel(l_model);
	ui.openFileList->resizeColumnsToContents();	
	ui.openFileList->resizeRowsToContents();
}

