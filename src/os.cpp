/**
 *  data.h
 *  Part of ofgui.h
 * 
 *  (C) By Jeroen van Iddekinge (iddekingej@lycos.com)
 * 
 *  Some opensystem functions
 */

#include "os.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <QMap>
#include <QString>
#include "utils.h"
/** 
 * Get user name of the user by the user id
 * 
 * \param p_uid  Userid 
 * \return       Username belonging to userid in p_uid
 */

QString getUsernameById(uint p_uid)
{
	struct passwd *l_passwd=getpwuid(p_uid);
	if(l_passwd==nullptr){
		return "<unkown id="+QString::number(p_uid)+">";
	} else {
		return l_passwd->pw_name;
	}
}


/**  
 * Get list of users defined on the system.
 * 
 * 
 * \param p_userList  id/user name map of users defined in the system
 *                    The index is the user id and the value is the username.
 */

void getAllUsers(QMap<uint,QString> &p_userList)
{
	passwd *l_pwd;
	setpwent();
	while((l_pwd=getpwent())!= nullptr){
		p_userList.insert(l_pwd->pw_uid,l_pwd->pw_name);
	}
	endpwent();
}


TFileType getFileType(const QString p_fileName){
	struct stat l_info;
	int l_status=stat(qstr(p_fileName),&l_info);
	if(l_status != 0){
		return DT_FAILED;
	}
	int l_fmt=l_info.st_mode & S_IFMT;
	switch(l_fmt){
		case S_IFSOCK:return DT_SOCKET;
		case S_IFLNK :return DT_LINK;
		case S_IFREG :return DT_FILE;
		case S_IFBLK :return DT_BLOCK_DEVICE;
		case S_IFDIR :return DT_DIR;
		case S_IFCHR :return DT_CHARACTER_DEVICE;
		case S_IFIFO :return DT_FIFO;
		default:
			return DT_UNKOWN;
	}
}

const char *fileTypeStr(TFileType p_type)
{
	switch(p_type){
		case DT_SOCKET : return "Socket";
		case DT_LINK   : return "Link";
		case DT_FILE   : return "File";
		case DT_BLOCK_DEVICE : return "Block device";
		case DT_DIR          : return "Directory";
		case DT_CHARACTER_DEVICE : return "Character device";
		case DT_FIFO             : return "Fifo";
		case DT_FAILED           : return "Failed";
		default:
			return "Unkown";
	}
	
}
