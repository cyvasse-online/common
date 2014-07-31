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

#include <functional>
#include "bearing_table.hpp"
#include "coordinate.hpp"
#include "piece.hpp"
#include "player.hpp"
#include "terrain.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match : public cyvmath::Match
		{
			protected:
				PlayerArray _players;

				CoordPieceMap _activePieces;
				TerrainMap _terrain;

				BearingTable _bearingTable;

				// alternative centers for horse movement;
				// one replacement coord per ruined fortress
				std::set<Coordinate> _fortressReplaceCorners;

			public:
				Match()
					: cyvmath::Match(PlayersColor::WHITE)
					, _bearingTable(_activePieces)
				{ }

				virtual ~Match() = default;

				PlayerArray& getPlayers()
				{ return _players; }

				std::shared_ptr<Player> getPlayer(PlayersColor color) const
				{ return _players[color]; }

				CoordPieceMap& getActivePieces()
				{ return _activePieces; }

				TerrainMap& getTerrain()
				{ return _terrain; }

				BearingTable& getBearingTable()
				{ return _bearingTable; }

				std::set<Coordinate> getHexagonMovementCenters();

				std::shared_ptr<Piece> getPieceAt(Coordinate);

				void forReachableCoords(Coordinate start, const MovementRange&, std::function<void(Coordinate, Piece*)>);

				virtual void addToBoard(PieceType, PlayersColor, Coordinate);
				virtual void removeFromBoard(std::shared_ptr<Piece>);
				virtual void endGame(PlayersColor /* winner */)
				{ }
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_MATCH_HPP_
