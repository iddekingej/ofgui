#include  "src/utils.h"
#include <QApplication>

QApplication *g_app;

int main(UNUSEDPAR int argc,UNUSEDPAR char **argv) {
	g_app=new QApplication(argc,argv);
	return 0;
}
