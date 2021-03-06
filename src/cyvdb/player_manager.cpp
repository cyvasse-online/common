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

using namespace std;
using namespace cyvasse;

namespace cyvdb
{
	bool PlayerManager::playerValid(const cyvasse::Player& player)
	{
		return player.getID().length() == 8;
	}

	int PlayerManager::getPlayersColorID(PlayersColor color)
	{
		auto colorStr = PlayersColorToStr(color);

		try
		{
			return m_conn.prepareCached(
				"SELECT players_color_id "
				"FROM players_colors "
				"WHERE players_color_str = :playersColorStr",
				"getPlayersColor" // cache key
			)
			.set("playersColorStr", colorStr)
			.selectValue()
			.getInt();
		}
		catch(tntdb::NotFound&)
		{
			m_conn.prepareCached(
				"INSERT INTO players_colors(players_color_str) "
				"VALUES (:playersColorStr)",
				"addPlayersColor" // cache key
			)
			.set("playersColorStr", colorStr)
			.execute();

			return m_conn.lastInsertId();
		}
	}

	PlayerManager::PlayerManager(tntdb::Connection& conn)
		: m_conn(conn)
	{ }

	PlayerManager::PlayerManager()
		: m_conn(tntdb::connectCached(DBConfig::glob().getMatchDataUrl()))
	{ }


	/*Player PlayerManager::getPlayer(const string& playerID)
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
	}*/

	/*PlayerManager::playerArray PlayerManager::getPlayers(Match& match)
	{
		playerArray ret;

		int i = 0;

		for(const auto& row : m_conn.prepareCached(
				"SELECT player_id, color FROM players WHERE match_id = :matchID",
				"getPlayers" // cache key
			)
			.set("matchID", match.getID()))
		{
			assert(i < 2);
			ret[i] = createPlayer(match, StrToPlayersColor(row.getString(1)), row.getString(0));
			i++;
		}

		return ret;
	}*/

	void PlayerManager::addPlayer(unique_ptr<cyvasse::Player> player, const string& matchID)
	{
		if(!playerValid(*player))
			throw invalid_argument("The given Player object is invalid");

		m_conn.prepareCached(
				"INSERT INTO players (player_id, match_id, color) "
				"VALUES (:id, :matchID, :color)",
				"addPlayer" // statement cache key
			)
			.set("id", player->getID())
			.set("matchID", matchID)
			.set("color", getPlayersColorID(player->getColor()))
			.execute();
	}
}
