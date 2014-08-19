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

#ifndef _MATCH_HPP_
#define _MATCH_HPP_

#include <string>
#include <cyvmath/rule_sets.hpp>

namespace cyvdb
{
	using cyvmath::RuleSet;

	struct Match
	{
		const std::string id;

		const RuleSet ruleSet;
		bool searchingForPlayer;


		Match(const std::string& argID = std::string(),
		      RuleSet argRuleSet = RuleSet::UNDEFINED,
		      bool argSearchingForPlayer = true)
			: id{argID}
			, ruleSet{argRuleSet}
			, searchingForPlayer{argSearchingForPlayer}
		{ }

		bool valid() const
		{
			return id.length() == 4
				&& ruleSet != RuleSet::UNDEFINED;
		}
	};
}

#endif // _MATCH_HPP_
