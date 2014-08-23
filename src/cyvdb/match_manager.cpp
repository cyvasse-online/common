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
#include <cyvdb/match.hpp>

using namespace cyvmath;

namespace cyvdb
{
	std::mutex MatchManager::randomMatchesMtx;

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
				m_conn.prepareCached("SELECT rule_set FROM matches WHERE match_id = :id", "getMatch")
				.set("id", matchID)
				.selectRow();

			return {matchID, StrToRuleSet(row.getString(0))};
		}
		catch(tntdb::NotFound&)
		{
			return Match();
		}
	}

	void MatchManager::addMatch(const Match& match)
	{
		if(!match.valid())
			throw std::invalid_argument("The given Match object is invalid");

		m_conn.prepareCached(
			"INSERT INTO matches (match_id, rule_set) "
			"VALUES (:id, :ruleSetID)",
			"addMatch" // cache key
			)
			.set("id", match.id)
			.set("ruleSetID", getRuleSetID(match.ruleSet))
			.execute();

		if(match.random)
		{
			m_conn.prepareCached("INSERT INTO random_matches (match_id) VALUES (:id)", "addRandomModeMatch")
				.set("id", match.id)
				.execute();
		}
	}

	Match MatchManager::getOldestRandomModeMatch(RuleSet ruleSet)
	{
		try
		{
			std::unique_lock<std::mutex> randomMatchesLock(randomMatchesMtx);

			auto matchID =
				m_conn.prepareCached(
					"SELECT match_id FROM random_matches WHERE created = "
					"(SELECT MIN(created) FROM random_matches "
					" INNER JOIN matches ON random_matches.match_id = matches.match_id "
					" WHERE rule_set = :ruleSetID)",
					"getOldestRandomModeMatch" // cache key
				)
				.set("ruleSetID", getRuleSetID(ruleSet))
				.selectValue()
				.getString();

			// TODO: change argument workaround to move capture when using C++14
			std::thread([this, matchID](std::unique_lock<std::mutex> lock) {
				m_conn.prepareCached("DELETE FROM random_matches WHERE match_id = :id", "removeRandomModeMatch")
					.set("id", matchID)
					.execute();

				lock.unlock();
			}, std::move(randomMatchesLock)).detach();

			return {matchID, ruleSet, true};
		}
		catch(tntdb::NotFound&)
		{
			return Match();
		}
	}

	void MatchManager::removeMatch(const std::string& id)
	{
		m_conn.prepareCached("DELETE FROM matches WHERE match_id = :id", "removeMatch")
			.set("id", id)
			.execute();
	}

	int MatchManager::getRuleSetID(RuleSet ruleSet)
	{
		std::string ruleSetStr = RuleSetToStr(ruleSet);

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
}
