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
#include <tntdb/connect.h>
#include <tntdb/error.h>
#include <tntdb/statement.h>
#include <cyvdb/config.hpp>
#include <cyvdb/match.hpp>

using namespace cyvmath;

namespace cyvdb
{
	MatchManager::MatchManager(tntdb::Connection& conn)
		: m_conn(conn)
	{ }

	MatchManager::MatchManager()
		: m_conn(tntdb::connectCached(DBConfig::glob().getMatchDataUrl()))
	{ }

	Match MatchManager::getMatch(const std::string& matchID)
	{
		try
		{
			tntdb::Row row =
				m_conn.prepare(
					"SELECT rule_set, searching_for_player FROM matches "
					"WHERE match_id = :id"
				)
				.set("id", matchID)
				.selectRow();

			return {matchID, StrToRuleSet(row.getString(0)), row.getBool(1)};
		}
		catch(tntdb::NotFound&)
		{
			return Match();
		}
	}

	void MatchManager::addMatch(Match& match)
	{
		if(!match.valid())
			throw std::invalid_argument("The given Match object is invalid");

		int ruleSetID;

		try
		{
			ruleSetID = m_conn.prepareCached(
				"SELECT rule_set_id "
				"FROM rule_sets "
				"WHERE rule_set_str = :ruleSetStr",
				"getRuleSetID" // cache key
				)
				.set("ruleSetStr", RuleSetToStr(match.ruleSet))
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
				.set("ruleSetStr", RuleSetToStr(match.ruleSet))
				.execute();

			ruleSetID = m_conn.lastInsertId();
		}

		m_conn.prepareCached(
			"INSERT INTO matches (match_id, rule_set, searching_for_player) "
			"VALUES (:id, :ruleSetID, :searchingForPlayer)",
			"addMatch" // cache key
			)
			.set("id", match.id)
			.set("ruleSetID", ruleSetID)
			.set("searchingForPlayer", match.searchingForPlayer)
			.execute();
	}
}
