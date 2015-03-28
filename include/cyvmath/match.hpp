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

#ifndef _CYVMATH_MATCH_HPP_
#define _CYVMATH_MATCH_HPP_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "player.hpp"
#include "rule_sets.hpp"

namespace cyvmath
{
	class Match
	{
		public:
			typedef std::array<std::unique_ptr<Player>, 2> playerArray;

		protected:
			const RuleSet m_ruleSet;
			const std::string m_id;
			const bool m_random;
			const bool m_public;

			playerArray m_players;

			PlayersColor m_activePlayer;
			bool m_setup;

			Match(RuleSet ruleSet, const std::string& id, bool random, bool _public, playerArray players)
				: m_ruleSet{ruleSet}
				, m_id{id}
				, m_random{random}
				, m_public{_public}
				, m_players(std::move(players))
				, m_activePlayer{PlayersColor::WHITE}
				, m_setup{true}
			{ }

		public:
			virtual ~Match() = default;

			RuleSet getRuleSet() const
			{ return m_ruleSet; }

			const std::string& getID() const
			{ return m_id; }

			bool isRandom() const
			{ return m_random; }

			bool isPublic() const
			{ return m_public; }

			playerArray& getPlayers()
			{ return m_players; }

			void setPlayers(playerArray players)
			{ m_players = std::move(players); }
	};
}

#endif // _CYVMATH_MATCH_HPP_
