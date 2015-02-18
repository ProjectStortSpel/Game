#ifndef DYNAMICSCRIPTING_H
#define DYNAMICSCRIPTING_H

#include <vector>
#include <time.h>

/* Data struct for the pathfinder to store data */
struct Rule
{
	float		weight;
	//std::string	script;

	/*bool operator == (Rule r)
	{
		return weight == r.weight && script == r.script;
	}*/
};

class DynamicScripting
{
public:
	static DynamicScripting* Instance();
	static void Destroy();
	~DynamicScripting();

	void AddRule(Rule _rule);
	void Sum();
	void GenerateScript();
	void AdjustWeight(float _fitness);
	float FitnessFunction(float _fitness);
	void DistributeLeftOvers(float _leftOver);

	bool InsertToScript(Rule _rule);
	std::vector<Rule> GetScript();
	void SetScript(std::vector<Rule> _script);
	void SetRuleBook(std::vector<Rule> _rules);

	bool IsInScript(Rule _rule);

	float RandomFloat();

private:
	DynamicScripting();
	DynamicScripting(DynamicScripting const&) = delete;
	void operator=(DynamicScripting const&) = delete;
	static DynamicScripting* m_instance;


private:
	float				m_totalSum;
	unsigned int		m_noOfScriptsToUse;
	std::vector<Rule>	m_ruleBook;
	std::vector<Rule>	m_script;
};
#endif //DYNAMICSCRIPTING_H
