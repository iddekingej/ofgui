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

typedef enum{
	DT_FILE
,	DT_DIR
,	DT_BLOCK_DEVICE
,	DT_CHARACTER_DEVICE
,	DT_FIFO
,	DT_SOCKET
,	DT_FAILED
,       DT_LINK
,	DT_UNKOWN
} TFileType;

TFileType getFileType(const QString p_fileName);
const char *fileTypeStr(TFileType p_type);

QString getUsernameById(uint p_uid);
void getAllUsers(QMap<uint,QString> &p_userList);
#endif
