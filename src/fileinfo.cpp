#include "fileinfo.h"
#include "utils.h"
#include <QFileInfo>
#include <QDateTime>
TFileInfo::TFileInfo(const QString p_fileName):QDialog()
{
	ui.setupUi(this);
	fileName=p_fileName;
	ui.labelFileName->setText(p_fileName);
	QFileInfo l_info(p_fileName);
	ui.labelFileExists->setText(l_info.exists()?i18n("FIle exists"):i18n("File doesn't exists"));
	ui.labelSize->setText(QString::number(l_info.size()));
	ui.labelReadableSize->setText(getReadableSize(l_info.size()));
	ui.labelOwner->setText(l_info.owner());
	ui.labelGroup->setText(l_info.group());
	ui.labelWriteable->setText(l_info.isWritable()?i18n("Yes"):i18n("No"));
	ui.labelReadable->setText(l_info.isReadable()?i18n("Yes"):i18n("No"));
	ui.labelCreatedAt->setText(l_info.created().toString());
	ui.labeLastChanged->setText(l_info.lastModified().toString());	
	connect(ui.closeButton,SIGNAL(pressed()),this,SLOT(close()));
}
	