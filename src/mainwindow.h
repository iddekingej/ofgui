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
	void processChanged(int p_index);
	void onlyRealFilesChecked(int p_state);
	void timeOutChanged();
	void refresh();

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
