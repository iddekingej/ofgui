
#include <QApplication>
#include <iostream>

#include "lib/testconfig.h"
#include "lib/testclass.h"
#include "base/run_base.h"
#include  "src/utils.h"

QApplication *g_app;

int main(UNUSEDPAR int argc,UNUSEDPAR char **argv) {
	g_app=new QApplication(argc,argv);
	TTestConfig *l_config=new TTestConfig();
	if(!l_config->initConfig()){
		std::cout << "init config failed" <<std::endl;
		return 255;
	}
	if(run_base(l_config)){
		std::cout <<"run_base failed" <<std::endl;
		return 255;
	}
	return 0;
}
