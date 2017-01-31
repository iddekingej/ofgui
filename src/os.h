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

QString getUsernameById(uint p_uid);
void getAllUsers(QMap<uint,QString> &p_userList);
#endif
