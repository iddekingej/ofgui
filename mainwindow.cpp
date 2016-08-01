#include "mainwindow.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QCheckBox>
#include <QTimer>
#include <klocalizedstring.h>
#include "data.h"
#include "linklist.h"
#include "globals.h"

void TMainWindow::onlyRealFilesChecked(int p_state)
{
	fillOpenFileGrid();
}

void TMainWindow::programChanged(int p_index)
{
	if(!updating){
		fillOpenFileGrid();
	}
}

void TMainWindow::timeOut()
{
	refresh();
}

void TMainWindow::timeOutChanged()
{
	checkRefresh(0);
}

void TMainWindow::refresh()
{
	openFileList.processInfo();
	updating=true;
	setProgramSelector();
	updating=false;
	fillOpenFileGrid();
	
}


void TMainWindow::checkRefresh(int p_state)
{
	
	if(ui.checkRefresh->checkState()==Qt::Checked){
		ui.refreshTime->setVisible(true);
		float l_value;
		bool l_ok;
		l_value=ui.refreshTime->text().toFloat(&l_ok);
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

void TMainWindow::setProgramSelector()
{
	TLinkListItem<TProgram> *l_current=openFileList.getProgramsStart();
	QStandardItemModel *l_model=new QStandardItemModel(0,1,this);
	QStandardItem *l_item;
	QString l_name;
	int l_cnt=1;
	l_item=new QStandardItem("");
	l_item->setData(QVariant(0),Qt::UserRole + 1);
	l_model->setItem(0,1,new QStandardItem(""));
	long l_selectedId=ui.programSelection->currentData(Qt::UserRole+1).toInt();
	long l_selectedIdx=0;
	while(l_current != nullptr){
		if(l_current->getItem()->hasOpenFile()){
			if(l_current->getItem()->getProgramName().length()==0){
				l_name="ID="+QString::number(l_current->getItem()->getId());
			} else {
				l_name=l_current->getItem()->getProgramName();
			}		
			l_item=new QStandardItem(l_name);
			l_item->setData(QVariant(l_current->getItem()->getId()), Qt::UserRole + 1);			
			if(l_current->getItem()->getId()==l_selectedId){
				l_selectedIdx=l_cnt;
			}
			l_model->setItem(l_cnt,l_item);
			l_cnt++;
		}
		l_current=l_current->getNext();
		
	}
	ui.programSelection->setModel(l_model);
	ui.programSelection->setModelColumn(0);
	ui.programSelection->setCurrentIndex(l_selectedIdx);
}

void TMainWindow::fillOpenFileGrid()
{
	TLinkListItem<TOpenFile> *l_current=openFileList.getOpenFilesStart();
	QStandardItemModel *l_model=new QStandardItemModel(0,4,this);
	QVariant l_selected=ui.programSelection->currentData(Qt::UserRole + 1);
	long     l_programId=l_selected.toUInt();
	int l_cnt=0;
	bool l_onlyRealFiles=(ui.onlyRealFiles->checkState()==Qt::Checked);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("File descr.")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("File name")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Prog. id.")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("Prog. name")));
	while (l_current != nullptr){
		if((l_programId ==0 || (l_current->getItem()->getOwner()->getId()==l_programId))
		&& (!l_onlyRealFiles|| l_current->getItem()->getRealFile())
			
		) {
			l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_current->getItem()->getFd())));
			l_model->setItem(l_cnt,1,new QStandardItem(l_current->getItem()->getFileName()));
			l_model->setItem(l_cnt,2,new QStandardItem(QString::number(l_current->getItem()->getOwner()->getId())));
			l_model->setItem(l_cnt,3,new QStandardItem(l_current->getItem()->getOwner()->getProgramName()));
			l_cnt++;

		}
		l_current=l_current->getNext();
	}
	ui.openFileList->setModel(l_model);
	ui.openFileList->resizeRowsToContents();
	ui.openFileList->resizeColumnsToContents();	
}

TMainWindow::TMainWindow(QWidget* p_parent):QMainWindow(p_parent)
{
	ui.setupUi(this);
	updating=false;
	refresh();
	checkRefresh(0);
	connect(ui.programSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(programChanged(int)));	
	connect(ui.onlyRealFiles,SIGNAL(stateChanged(int)),this,SLOT(onlyRealFilesChecked(int)));
	connect(ui.checkRefresh,SIGNAL(stateChanged(int)),this,SLOT(checkRefresh(int)));	
	connect(ui.refreshTime,SIGNAL(editingFinished()),this,SLOT(timeOutChanged()));
	connect(ui.action_Exit,&QAction::triggered,g_app,QApplication::quit);
	connect(&refreshTimer,SIGNAL(timeout()),this,SLOT(timeOut()));


}
