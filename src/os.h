/**
 *  data.h
 *  Part of ofgui.h
 * 
 *  (C) By Jeroen van Iddekinge (iddekingej@lycos.com)
 * 
 *  Some opensystem functions
 */
#ifndef _OS_H_
#define _OS_H_

#include <QString>
#include <QMap>

enum class TFileType{
	DT_FILE
,	DT_DIR
,	DT_BLOCK_DEVICE
,	DT_CHARACTER_DEVICE
,	DT_FIFO
,	DT_SOCKET
,	DT_FAILED
,       DT_LINK
,	DT_PIPE
,       DT_OTHER
,	DT_UNKOWN
};

enum class TFileCategory{
	FC_FILE
,	FC_DEVICE
,	FC_OTHER
};

TFileType getOSFileType(const QString &p_fileName);
TFileCategory getFileCategoryFromType(TFileType p_type);
QString fileTypeStr(TFileType p_type);

QString getUsernameById(uint p_uid);
void getAllUsers(QMap<uint,QString> &p_userList);
#endif
