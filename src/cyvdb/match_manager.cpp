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

#include <cyvdb/match_manager.hpp>

#include <stdexcept>
#include <thread>
#include <tntdb/connect.h>
#include <tntdb/error.h>
#include <tntdb/statement.h>
#include <cyvdb/config.hpp>
#include <cyvdb/player_manager.hpp>
#include <cyvmath/rule_set_create.hpp>

using namespace cyvmath;

namespace cyvdb
{
	bool MatchManager::matchValid(const Match& match)
	{
		return match.getRuleSet() != RuleSet::UNDEFINED
			&& match.getID().length() == 4;
	}

	int MatchManager::getRuleSetID(RuleSet ruleSet)
	{
		auto ruleSetStr = RuleSetToStr(ruleSet);

		try
		{
			return m_conn.prepareCached(
					"SELECT rule_set_id "
					"FROM rule_sets "
					"WHERE rule_set_str = :ruleSetStr",
					"getRuleSetID" // cache key
				)
				.set("ruleSetStr", ruleSetStr)
				.selectValue()
				.getInt();
		}
		catch(tntdb::NotFound&)
		{
			m_conn.prepareCached(
					"INSERT INTO rule_sets(rule_set_str) "
					"VALUES (:ruleSetStr)",
					"addRuleSet" // cache key
				)
				.set("ruleSetStr", ruleSetStr)
				.execute();

			return m_conn.lastInsertId();
		}
	}

	MatchManager::MatchManager(tntdb::Connection& conn)
		: m_conn(conn)
	{ }

	MatchManager::MatchManager()
		: m_conn(tntdb::connectCached(DBConfig::glob().getMatchDataUrl()))
	{ }

	MatchManager::matchList MatchManager::getOpenRandomMatches()
	{
		matchList ret;

		for(const auto& row : m_conn.prepareCached(
				"SELECT rule_set_str, match_id, public "
				"FROM matches "
				"INNER JOIN rule_sets ON matches.rule_set_id = rule_sets.rule_set_id "
				"WHERE random = TRUE "
				"GROUP BY match_id "
				"HAVING COUNT(*) = 1 "
				"ORDER BY created ASC",
				"getOpenRandomMatches" // cache key
			))
		{
			auto&& match = createMatch(
				cyvmath::StrToRuleSet(row.getString(0)),
				row.getString(1), true, row.getBool(2)
			);

			auto&& players = PlayerManager(m_conn).getPlayers(*match);
			assert(players[0] && !players[1]);

			match->setPlayers(std::move(players));

			ret.push_back(std::move(match));
		}

		return ret;
	}

	MatchManager::matchList MatchManager::getRunningPublicMatches()
	{
		matchList ret;

		for(const auto& row : m_conn.prepareCached(
				"SELECT rule_set_str, match_id, random "
				"FROM matches "
				"INNER JOIN rule_sets ON matches.rule_set_id = rule_sets.rule_set_id "
				"WHERE public = TRUE "
				"GROUP BY match_id "
				"HAVING COUNT(*) = 2 "
				"ORDER BY created ASC",
				"getRunningPublicMatches" // cache key
			))
			{
				auto&& match = createMatch(
					cyvmath::StrToRuleSet(row.getString(0)),
					row.getString(1), row.getBool(2), true
				);

				auto&& players = PlayerManager(m_conn).getPlayers(*match);
				assert(players[0] && players[1]);

				match->setPlayers(std::move(players));

				ret.push_back(std::move(match));
			}

			return ret;
	}

	void MatchManager::addMatch(std::unique_ptr<Match> match)
	{
		if(!matchValid(*match))
			throw std::invalid_argument("The given Match object is invalid");

		m_conn.prepareCached(
				"INSERT INTO matches (match_id, rule_set, random, public) "
				"VALUES (:id, :ruleSetID, :random, :public)",
				"addMatch" // cache key
			)
			.set("id", match->getID())
			.set("ruleSetID", getRuleSetID(match->getRuleSet()))
			.set("random", match->isRandom())
			.set("public", match->isPublic())
			.execute();
	}

	void MatchManager::removeMatch(const std::string& id)
	{
		m_conn.prepareCached("DELETE FROM matches WHERE match_id = :id", "removeMatch")
			.set("id", id)
			.execute();
	}
}
