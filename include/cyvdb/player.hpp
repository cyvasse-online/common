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

#ifndef _CYVDB_PLAYER_HPP_
#define _CYVDB_PLAYER_HPP_

#include <string>
#include <cyvmath/player.hpp>

namespace cyvdb
{
	using cyvmath::PlayersColor;

	class Player : public cyvmath::Player
	{
		public:
			const std::string id;
			const std::string matchID;


			Player(const std::string& argID = std::string(),
			       const std::string& argMatchID = std::string(),
			       PlayersColor argColor = PlayersColor::UNDEFINED)
				: cyvmath::Player{argColor}
				, id{argID}
				, matchID{argMatchID}
			{ }

			bool valid() const
			{
				return id.length() == 8 &&
					m_color != PlayersColor::UNDEFINED;
			}

			bool setupComplete() override
			{ return true; } // TODO
	};
}

#endif // _CYVDB_PLAYER_HPP_
