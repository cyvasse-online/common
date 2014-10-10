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

#include <cyvmath/rule_set_create.hpp>

#include <stdexcept>
#include <make_unique.hpp>
#include <cyvmath/mikelepage/fortress.hpp>
#include <cyvmath/mikelepage/match.hpp>
#include <cyvmath/mikelepage/player.hpp>

namespace cyvmath
{
	std::unique_ptr<Match> createMatch(RuleSet ruleSet)
	{
		switch(ruleSet)
		{
			case RuleSet::MIKELEPAGE:
				return make_unique<mikelepage::Match>();
			default:
				throw std::invalid_argument("invalid rule set");
		}
	}

	std::unique_ptr<Player> createPlayer(PlayersColor color, Match& match)
	{
		if(dynamic_cast<mikelepage::Match*>(&match) != nullptr)
		{
			auto& m = dynamic_cast<mikelepage::Match&>(match);

			// fortress start coordinate doesn't matter on the server
			return make_unique<mikelepage::Player>(color, m,
				make_unique<mikelepage::Fortress>(color, *mikelepage::Coordinate::create(5,5)));
		}
		else throw std::runtime_error("Match object not recognized");
	}
}
