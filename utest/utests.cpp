/*
 * IndividualTest.cpp
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */



#include "utests.h"
#include "Individual_test.h"
#include "Population_test.h"
#include "OperatorFactory_test.h"


using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(Individual_test);
CPPUNIT_TEST_SUITE_REGISTRATION(Population_test);
CPPUNIT_TEST_SUITE_REGISTRATION(OperatorFactory_test);


int tests_main(int argc, char* argv[]) {
	// configure a basic logger for the tests
	BasicConfigurator::configure();

	// informs test-listener about testresults
	CPPUNIT_NS::TestResult testresult;

	// register listener for collecting the test-results
	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener(&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener(&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest(
			CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	testrunner.run(testresult);

	// output results in compiler-format
	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults,
			std::cerr);
	compileroutputter.write();

	// Output XML for Jenkins CPPunit plugin
	ofstream xmlFileOut("cppTestsResults.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
