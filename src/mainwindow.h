/**
 *  mainwindow.h
 *  Part of ofgui.h
 * 
 *  (C) By Jeroen van Iddekinge (iddekingej@lycos.com)
 * 
 *  mainwindow class. 
 *  The main windows displays the list with open files
 */

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_
#include <QMainWindow>
#include <QTreeView>
#include <QTimer>
#include "ui_main.h"
#include "data.h"
#include <QKeyEvent>
#include <QTableView>
#include "processlist.h"

class TMainWindow:public QMainWindow
{
	Q_OBJECT
private slots:
	void checkRefresh(int p_state);
	void detailsInfo();
	void doubleClickedGrid(const QModelIndex &p_index);
	void processChanged(int p_index);
	void onlyRealFilesChecked(int p_state);
	void timeOutChanged();
	void refresh();
	void openMonitor();
	void about();
private:
	Ui::MainWindow ui;
	/**
	 *  List of running processes
	 *  Each proces contains open files.
	 */
	TProcessList processList;         
	/**
	 * Widget inside the process combobox box containing running processes.	 
	 * This is the drop down selection list of the combobox
	 */
	QTableView   *processSelection;   
	/**
	 * Refresh timer.
	 * On each timeout the open files are reread and the list is refreshed.
	 */
	QTimer       refreshTimer;        
	
	void fillOpenFileGrid();
	void setProgramSelector();
public:
	TMainWindow(QWidget *p_parent=nullptr);
};
#endif
