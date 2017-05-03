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
	connect(ui.closeButton,SIGNAL(pressed()),this,SLOT(close()));
	connect(ui.refreshButton,SIGNAL(pressed()),this,SLOT(fillData()));
	fillData();
}
	
	
void TFileInfo::fillData()
{
	QFileInfo l_info(this->fileName);
	ui.labelFileExists->setText(l_info.exists()?i18n("FIle exists"):i18n("File doesn't exists"));
	ui.labelSize->setText(QString::number(l_info.size()));
	ui.labelReadableSize->setText(getReadableSize(l_info.size()));
	ui.labelOwner->setText(l_info.owner());
	ui.labelGroup->setText(l_info.group());
	ui.labelWriteable->setText(l_info.isWritable()?i18n("Yes"):i18n("No"));
	ui.labelReadable->setText(l_info.isReadable()?i18n("Yes"):i18n("No"));
	ui.labelCreatedAt->setText(l_info.created().toString());
	ui.labeLastChanged->setText(l_info.lastModified().toString());	
	QStandardItemModel *l_model=new QStandardItemModel(0,3);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Pid")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Program")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Owner")));
	TLinkList<TProcess> *l_op=processesByFile(fileName);
	TLinkListIterator<TProcess> l_iter(l_op);
	TProcess *l_process;
	int l_cnt=0;
	while(l_iter.hasNext()){
		l_process=l_iter.next();
		l_model->setItem(l_cnt,0,new QStandardItem(QString::number(l_process->getId())));
		l_model->setItem(l_cnt,1,new QStandardItem(l_process->getProgramName()));
		l_model->setItem(l_cnt,2,new QStandardItem(l_process->getOwner()));
		l_cnt++;
	}
	setViewModel(ui.processList,l_model);
	ui.processList->resizeColumnsToContents();;
	ui.processList->resizeRowsToContents();
	
	delete l_op;
}

	