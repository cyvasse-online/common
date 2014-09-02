/* Copyright 2014 Jonas Platte
 *
 * This file is part of Cyvasse Online.
 *
 * Cyvasse Online is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Cyvasse Online is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CYVMATH_RULESETS_HPP_
#define _CYVMATH_RULESETS_HPP_

#include <map>
#include <vector>
#include <enum_str.hpp>

namespace cyvmath
{
	enum class RuleSet
	{
		UNDEFINED,
		MIKELEPAGE
	};

	ENUM_STR(RuleSet, ({
			{RuleSet::UNDEFINED,  "undefined"},
			{RuleSet::MIKELEPAGE, "mikelepage"}
		}),
		RuleSet::UNDEFINED
	)

	inline const std::vector<RuleSet>& allRuleSets()
	{
		static const std::vector<RuleSet> data {
			RuleSet::MIKELEPAGE
		};

		return data;
	}

	inline const std::string& RuleSetToPrettyStr(RuleSet r)
	{
		static const std::map<RuleSet, std::string> data {
			{RuleSet::UNDEFINED,  "[undefined]"},
			{RuleSet::MIKELEPAGE, "Mike Le Page's rule set"}
		};

		return data.at(r);
	}
}

#endif // _CYVMATH_RULESETS_HPP_
