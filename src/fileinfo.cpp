#include "fileinfo.h"
#include "utils.h"
#include <QFileInfo>
#include <QDateTime>
#include "data.h"
#include "linklist.h"

TFileInfo::TFileInfo(const QString p_fileName):QDialog()
{
	ui.setupUi(this);
	fileName=p_fileName;
	ui.labelFileName->setText(p_fileName);
	notify.addPath(p_fileName);
	connect(ui.closeButton,SIGNAL(pressed()),this,SLOT(close()));
	connect(ui.refreshButton,SIGNAL(pressed()),this,SLOT(fillData()));
	connect(&notify,SIGNAL(fileChanged(const QString &)),this,SLOT(fileChanged(const QString &)));
	fillData();
}
	
void TFileInfo::fileChanged(const QString& p_path)
{
	if(Qt::Checked==ui.autorefresh->checkState()){
		fillData();
	}
}

	
void TFileInfo::fillData()
{
	
	TProcessSignleFileList l_list;
	l_list.setFilterFileName(this->fileName);
	l_list.processInfo();
	TLinkList<TOpenFile> *l_op=l_list.getOpenFiles();
	TLinkListIterator<TOpenFile> l_iter(l_op);
	bool l_display=false;
	TOpenFile *l_first=nullptr;
	if(nullptr != l_op->getStart()){
		l_first=l_op->getStart()->getItem();
		l_display=(l_first->getFileCategory() != FC_OTHER);
	}
	ui.fileSummary->setVisible(l_display);
	if(l_display){
		QFileInfo l_info(this->fileName);
		QString l_type;
		l_first->getFileTypeStr(l_type);
		ui.labelType->setText(l_type);
		ui.labelFileExists->setText(l_info.exists()?i18n("FIle exists"):i18n("File doesn't exists"));
		ui.labelSize->setText(QString::number(l_info.size()));
		ui.labelReadableSize->setText(getReadableSize(l_info.size()));
		ui.labelOwner->setText(l_info.owner());
		ui.labelGroup->setText(l_info.group());
		ui.labelWriteable->setText(l_info.isWritable()?i18n("Yes"):i18n("No"));
		ui.labelReadable->setText(l_info.isReadable()?i18n("Yes"):i18n("No"));
		ui.labelCreatedAt->setText(l_info.created().toString());
		ui.labeLastChanged->setText(l_info.lastModified().toString());	
	}
	QStandardItemModel *l_model=new QStandardItemModel(0,3);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("fd")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Mode")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Pid")));
	l_model->setHorizontalHeaderItem(3,new QStandardItem(i18n("Program")));
	l_model->setHorizontalHeaderItem(4,new QStandardItem(i18n("Owner")));

	

	TProcess *l_process;
	TOpenFile *l_openFile;
	int l_cnt=0;
	while(l_iter.hasNext()){
		l_openFile=l_iter.next();
		l_process=l_openFile->getProcess();		
		l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_openFile->getFd())));
		l_model->setItem(l_cnt,1,new QStandardItem(l_openFile->getOpenModeDescription()));
		l_model->setItem(l_cnt,2,new QStandardItem(QString::number(l_process->getId())));
		l_model->setItem(l_cnt,3,new QStandardItem(l_process->getProgramName()));
		l_model->setItem(l_cnt,4,new QStandardItem(l_process->getOwner()));
		l_cnt++;
	}
	setViewModel(ui.processList,l_model);
	ui.processList->resizeColumnsToContents();;
	ui.processList->resizeRowsToContents();
	
}

	
