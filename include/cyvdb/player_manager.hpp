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

#ifndef _CYVDB_PLAYER_MANAGER_HPP_
#define _CYVDB_PLAYER_MANAGER_HPP_

#include <array>
#include <memory>
#include <cassert>
#include <tntdb/connection.h>
#include <cyvasse/match.hpp>
#include <cyvasse/player.hpp>

namespace cyvdb
{
	class PlayerManager
	{
		public:
			typedef std::array<std::unique_ptr<cyvasse::Player>, 2> playerArray;

		private:
			tntdb::Connection m_conn;

			static bool playerValid(const cyvasse::Player&);

			int getPlayersColorID(cyvasse::PlayersColor color);

		public:
			explicit PlayerManager(tntdb::Connection& conn);
			PlayerManager();

			// queries
			//cyvasse::Player getPlayer(const std::string& playerID);
			//playerArray getPlayers(cyvasse::Match&);

			// modifications
			void addPlayer(std::unique_ptr<cyvasse::Player>, const std::string& matchID);
	};
}

#endif // _CYVDB_PLAYER_MANAGER_HPP_
