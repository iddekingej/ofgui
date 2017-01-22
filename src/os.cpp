#include "os.h"
#include <sys/types.h>
#include <pwd.h>
#include <QMap>
#include <QString>
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
