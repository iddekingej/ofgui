#include "run_base.h"
#include "test01.h"

class TRunBaseRunner:public TRunner
{
protected:
/**
 *Run all base tests
 */		
	virtual void doRun()
	{
		runner(new TTest01());
	}
	
public:
	TRunBaseRunner(TTestConfig *p_testConfig):TRunner(p_testConfig)
	{
		
	}

};

/**
 * Start all base testes
 */
bool run_base(TTestConfig* p_config)
{

	TRunBaseRunner l_run(p_config);
	return l_run.run();
}

