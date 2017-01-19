#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_
#include <QMainWindow>
#include <QTreeView>
#include <QTimer>
#include "ui_main.h"
#include "data.h"
class TMainWindow:public QMainWindow
{
	Q_OBJECT
private slots:
	void checkRefresh(int p_state);
	void programChanged(int p_index);
	void onlyRealFilesChecked(int p_state);
	void timeOut();
	void timeOutChanged();
	void refreshButton();
	void searchPressed();
private:
	Ui::MainWindow ui;
	TOpenFileList openFileList;
	QTimer refreshTimer;
	bool updating;
	QString searchText;
	
	void refresh();
	void fillOpenFileGrid();
	void setProgramSelector();

public:
	TMainWindow(QWidget *p_parent=nullptr);
};
#endif