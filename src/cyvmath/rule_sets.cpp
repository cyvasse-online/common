#include <cyvmath/rule_sets.hpp>

namespace cyvmath
{
	const std::vector<RuleSet>& allRuleSets()
	{
		static const std::vector<RuleSet> data {
			RuleSet::MIKELEPAGE
		};

		return data;
	}

	const std::string& RuleSetToPrettyStr(RuleSet r)
	{
		static const std::map<RuleSet, std::string> data {
			{RuleSet::UNDEFINED,  "[undefined]"},
			{RuleSet::MIKELEPAGE, "Mike Le Page's rule set"}
		};

		return data.at(r);
	}
}
