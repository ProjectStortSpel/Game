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
	std::string lol = "Content/Components/Component.cmp";
	Parser* test = new Parser();
	test->ParseFile(lol);
}