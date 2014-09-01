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

#include <cyvdb/player_manager.hpp>

#include <tntdb/connect.h>
#include <tntdb/error.h>
#include <tntdb/statement.h>
#include <cyvdb/config.hpp>
#include <cyvdb/player.hpp>

using namespace cyvmath;

namespace cyvdb
{
	PlayerManager::PlayerManager(tntdb::Connection& conn)
		: m_conn(conn)
	{ }

	PlayerManager::PlayerManager()
		: m_conn(tntdb::connectCached(DBConfig::glob().getMatchDataUrl()))
	{ }

	Player PlayerManager::getPlayer(const std::string& playerID)
	{
		try
		{
			tntdb::Row row =
				m_conn.prepareCached(
					"SELECT match_id, color FROM players "
					"WHERE player_id = :id",
					"getPlayer" // cache key
				)
				.set("id", playerID)
				.selectRow();

			return Player(playerID, row.getString(0), StrToPlayersColor(row.getString(1)));
		}
		catch(tntdb::NotFound&)
		{
			return Player();
		}
	}

	std::vector<Player> PlayerManager::getPlayers(const std::string& matchID)
	{
		std::vector<Player> ret;

		for(const auto& row : m_conn.prepareCached(
				"SELECT player_id, color FROM players WHERE match_id = :matchID", "getPlayers")
			.set("matchID", matchID)
			)
		{
			ret.emplace_back(row.getString(0), matchID, StrToPlayersColor(row.getString(1)));
		}

		return ret;
	}

	void PlayerManager::addPlayer(const Player& player)
	{
		if(!player.valid())
			throw std::invalid_argument("The given Player object is invalid");

		m_conn.prepareCached(
			"INSERT INTO players (player_id, match_id, color) "
			"VALUES (:id, :matchID, :color)",
			"addPlayer" // statement cache key
			)
			.set("id", player.id)
			.set("matchID", player.matchID)
			.set("color", PlayersColorToStr(player.getColor()))
			.execute();
	}
}
