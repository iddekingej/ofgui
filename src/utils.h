/* 
 * Utility functions
*/

#ifndef __UTILS_H_
#define __UTILS_H_
#include <QString>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QItemSelectionModel>

#define PAR_UNUSED __attribute__((unused))

/** 
 *  function to easy convert QString to const QString
 *  \param p_string QString to convert
 */
#define qstr(p_string) ((p_string).toUtf8().data())


typedef unsigned long TDiskSize;

// Reading a single line from a file containing a unsinged long
bool readLong(QString p_path,QString p_name,unsigned long &p_size);


//Reading a single line from a file
bool readString(QString p_path,QString p_name,QString &p_return);

//convert size to readable format 1024=>1K
void getReadableSize(TDiskSize p_value,TDiskSize &p_size,char &p_ind);

//convert size to readable format 1024=>1K
QString getReadableSize(TDiskSize p_value);

//Set view model, delete old one and delete selection model
void setViewModel(QAbstractItemView *p_view,QStandardItemModel *p_model);



//Compare is paths are same, ignoring end slashes
QString normelizePath(QString p_path);

#define UNUSEDPAR __attribute__((unused))

#endif

