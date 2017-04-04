
#include "utils.h"
#include <QDir>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
/**
 * Some of the information is stored in a file containing a integer value (for example file size)
 * This routine reads a single line and converts the line into a unsigned long
 * 
 * \param p_path  - Path 
 * \param p_name  - File name. p_path+p_name 
 * \param p_value - Returned value
 * \Return    true when successful  and false when a reading error has occurred
 *            or when the value is not a valid integer
*/

bool readLong(QString p_path,QString p_name,unsigned long &p_value)
{
	QString l_string;
	bool l_ok;
	
	p_value=0;
	if(!readString(p_path,p_name,l_string)) return false;
	p_value=l_string.toULong(&l_ok);
	return l_ok;
}

/**
 * Some of the information is stored in a file containing one line with a string 
 * (for example /sys/block/sda/device/model contains the model name of de device)
 * 
 * \param p_path  - Path 
 * \param p_name  - File name under path to read
 * \param p_value - Returned value
 * \return        true when successful  false when a file reading error has occurred
*/

bool readString(QString p_path,QString p_name,QString &p_value)
{
	bool l_succes;	
	QFile l_file(p_path+"/"+p_name);
	if(!l_file.open(QIODevice::ReadOnly|QIODevice::Text)){
		p_value= QStringLiteral("");
		return false;
	}
	QTextStream l_stream(&l_file);
	p_value=l_stream.readLine();
	l_succes=(l_stream.status() ==  QTextStream::Ok);	
	l_file.close();
	
	return l_succes;
}

/**
* Return  size in an easy readable form(10240 =>10k)
* 
*\param p_value - value (ex 1024)
*\param p_size  - Return the numerical part of a readable size (10240 =>10 102400=>100 etc..)
*\param p_ind   - Return the size indicator (102400 =>p_size=100 p_ind='K')
*/
void getReadableSize(TDiskSize p_value,TDiskSize &p_size,char &p_ind)
{
	TDiskSize l_size=p_value;
	char l_ind=' ';
	if(l_size>=1024){
		l_size=l_size/1024;
		l_ind='K';
	}
	if(l_size>=1024){
		l_size=l_size/1024;
		l_ind='M';
	}
	if(l_size>=1024){
		l_size=l_size/1024;
		l_ind='G';
	}
	if(l_size>=1024){
		l_size=l_size/1024;
		l_ind='T';
	}
	p_ind=l_ind;
	p_size=l_size;
}

/**
 *  Converts size to a easily readable form (102400=>100K)
 *  @see getReadableSize(TDiskSize,TDiskSize,char)
 * 
 * \param  p_value Size in bytes
 * \return         Size in a readable form (10K, 100M etc..)
 */

QString getReadableSize(TDiskSize p_value)
{
    TDiskSize l_size;
    char l_ind;
    getReadableSize(p_value,l_size,l_ind);
    return QString::number(l_size)+l_ind;
}


/**
 * Simple normalization of a path
 * When there is no "/" at the end of the path, a "/" is added
 * 
 * \param  p_path Path to normalize
 * \return        Normalize string (/dummy => /dummy/  /dummy/=>/dummy/)
 */
QString normelizePath(QString p_path)
{
    
    return p_path+(!p_path.endsWith("/")?"/":"");
}

/**
 *  When setting the model to a new model, the old model and the selection model needs to be deleted (see qt docs)
 *  (Except in cases when the model or selection model are used in more than one view)
 * 
 * \param p_view  - View who's model needs to be set.
 * \param p_model - New model
 */

void setViewModel(QAbstractItemView *p_view,QStandardItemModel *p_model)
{
	QAbstractItemModel *l_oldModel=p_view->model();
	QItemSelectionModel *l_oldSelection=p_view->selectionModel();
	p_view->setModel(p_model);
	delete l_oldModel;
	delete l_oldSelection;
}
	

