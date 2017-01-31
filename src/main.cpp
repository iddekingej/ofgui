#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <KAboutData>
#include "mainwindow.h"

QApplication *g_app;

/**
 *  Main program of ofgui. 
 * Initialize QT and main window
 */

int main(int argc, char **argv) {
	
	g_app=new QApplication(argc,argv);
	KLocalizedString::setApplicationDomain("ofgui");
	KAboutData l_aboutData(
			"ofgui"
		,	i18n("Open file gui")
		,	"0.1"
		,	i18n("X")
		,	KAboutLicense::GPL
		,	i18n("(C) 2017")
		,	i18n("x")
		,	QStringLiteral("X")
		,	QStringLiteral("X")
		   );
	l_aboutData.addAuthor(i18n("Jeroen van Iddekinge"),i18n("X"),("iddekingej@lycos.com"));
	KAboutData::setApplicationData(l_aboutData);

	QCommandLineParser l_parser;
	l_parser.addHelpOption();
	l_parser.addVersionOption();
	l_aboutData.setupCommandLine(&l_parser);
	l_parser.process(*g_app);
	l_aboutData.processCommandLine(&l_parser);	
	//checkConfig();
	TMainWindow *l_main=new TMainWindow();
	l_main->show();
	int l_exit=g_app->exec();
	delete l_main;
	delete g_app;
	return l_exit;
}