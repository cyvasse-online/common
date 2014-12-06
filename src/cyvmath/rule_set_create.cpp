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
#include <cyvmath/hexagon.hpp>
#include <cyvmath/mikelepage/fortress.hpp>
#include <cyvmath/mikelepage/match.hpp>
#include <cyvmath/mikelepage/player.hpp>

namespace cyvmath
{
	std::unique_ptr<Match> createMatch(RuleSet ruleSet, const std::string& id, bool random, bool _public)
	{
		switch(ruleSet)
		{
			case RuleSet::MIKELEPAGE:
				return make_unique<mikelepage::Match>(id, random, _public);
			default:
				throw std::invalid_argument("invalid rule set");
		}
	}

	std::unique_ptr<Player> createPlayer(Match& match, PlayersColor color, const std::string& id)
	{
		switch(match.getRuleSet())
		{
			case RuleSet::MIKELEPAGE:
				// fortress start coordinate doesn't matter on the server
				return make_unique<mikelepage::Player>(dynamic_cast<mikelepage::Match&>(match), color,
					make_unique<mikelepage::Fortress>(color, *Hexagon<6>::Coordinate::create(5,5)), id);
			default:
				throw std::runtime_error("Match object not recognized");
		}
	}
}
