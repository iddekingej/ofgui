#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_
#include <QMainWindow>
#include <QTreeView>
#include <QTimer>
#include "ui_main.h"
#include "data.h"
#include <QKeyEvent>
#include <QTableView>
class TMainWindow:public QMainWindow
{
	Q_OBJECT
private slots:
	void checkRefresh(int p_state);
	void programChanged(int p_index);
	void onlyRealFilesChecked(int p_state);
	void timeOutChanged();
	void refresh();

private:
	Ui::MainWindow ui;
	TProcessList openFileList;  
	QTableView   *processSelection;   /**< Widget used for displaying process list  in process selector*/
	QTimer       refreshTimer;        /**< Timer used for refreshing the open file grid  */
	/*QString      searchText;*/
	
	void fillOpenFileGrid();
	void setProgramSelector();
public:
	TMainWindow(QWidget *p_parent=nullptr);
};
#endif
