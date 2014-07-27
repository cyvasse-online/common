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

#ifndef _CYVMATH_MIKELEPAGE_MATCH_HPP_
#define _CYVMATH_MIKELEPAGE_MATCH_HPP_

#include <cyvmath/match.hpp>

#include <initializer_list>
#include <map>
#include <memory>
#include "player.hpp"
#include "terrain.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match : public cyvmath::Match
		{
			protected:
				PlayerMap _players;

				PieceMap _activePieces;
				TerrainMap _terrain;

			public:
				Match()
					: cyvmath::Match(PlayersColor::WHITE)
				{ }

				PlayerMap& getPlayers()
				{ return _players; }

				std::shared_ptr<Player> getPlayer(PlayersColor color) const
				{ return _players.at(color); }

				PieceMap& getActivePieces()
				{ return _activePieces; }

				TerrainMap& getTerrain()
				{ return _terrain; }

				virtual ~Match() = default;
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_MATCH_HPP_
