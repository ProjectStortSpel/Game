#include "ECSL.h"
#include <string>
#include "ECSL/Framework/Parsing/Parser.h"

using namespace ECSL;
ECSL_Tester::ECSL_Tester()
{

}
ECSL_Tester::~ECSL_Tester()
{

}

void ECSL_Tester::TestParser()
{
	std::string lol = "Content/Components/TestComponent.cmp";
	Parser* test = new Parser();
	test->ParseFile(lol);
}