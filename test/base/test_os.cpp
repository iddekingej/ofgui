#include <QString>
#include "test_os.h"
#include "src/os.h"

void TTestOS::doRun()
{
	testFile();
	testNull();
}


void TTestOS::testFile()
{
	QString l_file=getConfig()->getFilePath("test");
	TFileType l_ft=getOSFileType(l_file);
	expect("File type",(long)l_ft,(long)TFileType::DT_FILE);
	
}

void TTestOS::testNull()
{
	TFileType l_ft=getOSFileType("/dev/null");
	expect("File type",(long)l_ft,(long)TFileType::DT_CHARACTER_DEVICE);
}

