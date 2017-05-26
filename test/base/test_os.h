#ifndef __TEST_OS_H_
#define __TEST_OS_H_
#include "../lib/testclass.h"

class TTestOS:public TTestClass
{
protected:
	virtual void doRun() override;
private:
	void testFile();
	void testNull();
};
#endif
