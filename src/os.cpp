
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
#include <KLocalizedString>
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


TFileType getOSFileType(const QString &p_fileName){
	struct stat l_info;
	int l_status=stat(qstr(p_fileName),&l_info);
	if(l_status != 0){
		return TFileType::DT_FAILED;
	}
	int l_fmt=l_info.st_mode & S_IFMT;
	switch(l_fmt){
		case S_IFSOCK:return TFileType::DT_SOCKET;
		case S_IFLNK :return TFileType::DT_LINK;
		case S_IFREG :return TFileType::DT_FILE;
		case S_IFBLK :return TFileType::DT_BLOCK_DEVICE;
		case S_IFDIR :return TFileType::DT_DIR;
		case S_IFCHR :return TFileType::DT_CHARACTER_DEVICE;
		case S_IFIFO :return TFileType::DT_FIFO;
		default:
			return TFileType::DT_UNKOWN;
	}
}

QString fileTypeStr(TFileType p_type)
{
	switch(p_type){
		case TFileType::DT_SOCKET           : return i18n("Socket");
		case TFileType::DT_LINK             : return i18n("Link");
		case TFileType::DT_FILE             : return i18n("File");
		case TFileType::DT_BLOCK_DEVICE     : return i18n("Block device");
		case TFileType::DT_DIR              : return i18n("Directory");
		case TFileType::DT_CHARACTER_DEVICE : return i18n("Character device");
		case TFileType::DT_FIFO             : return i18n("Fifo");
		case TFileType::DT_FAILED           : return i18n("Failed");
		case TFileType::DT_PIPE             : return i18n("Pipe");
		case TFileType::DT_UNKOWN           : return i18n("Unkown");
		default:
			return i18n("Unkown");
	}
	
}

TFileCategory getFileCategoryFromType(TFileType p_type){
		if(p_type==TFileType::DT_FILE || p_type== TFileType::DT_DIR){
			return TFileCategory::FC_FILE;
		}
		if(p_type==TFileType::DT_BLOCK_DEVICE || p_type==TFileType::DT_CHARACTER_DEVICE){
			return TFileCategory::FC_DEVICE;
		}
		return TFileCategory::FC_OTHER;
}
