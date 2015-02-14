/* Copyright 2014 - 2015 Jonas Platte
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
#include <cyvmath/hexagon.hpp>
#include "bearing_table.hpp"
#include "piece.hpp"
#include "player.hpp"
#include "terrain.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match : public cyvmath::Match
		{
			public:
				using HexCoordinate = Hexagon<6>::Coordinate;

			protected:
				CoordPieceMap m_activePieces;
				TerrainMap m_terrain;

				BearingTable m_bearingTable;

			public:
				Match(const std::string& id = {}, bool random = false,
				      bool _public = false, playerArray players = playerArray())
					: cyvmath::Match(RuleSet::MIKELEPAGE, id, random, _public, std::move(players))
					, m_bearingTable(m_activePieces)
				{ }

				virtual ~Match() = default;

				auto getPlayer(PlayersColor color) const -> Player&
				{ return dynamic_cast<Player&>(*m_players.at(color)); }

				auto getActivePieces() -> CoordPieceMap&
				{ return m_activePieces; }

				auto getTerrain() -> TerrainMap&
				{ return m_terrain; }

				auto getBearingTable() -> BearingTable&
				{ return m_bearingTable; }

				auto getHorseMovementCenters() -> std::set<HexCoordinate>;

				auto getPieceAt(HexCoordinate) -> std::shared_ptr<Piece>;

				void forReachableCoords(HexCoordinate start, const MovementRange&, std::function<void(const HexCoordinate&, Piece*)>);

				virtual void addToBoard(PieceType, PlayersColor, const HexCoordinate&);
				virtual void removeFromBoard(std::shared_ptr<Piece>);
				virtual void endGame(PlayersColor /* winner */) { }
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_MATCH_HPP_
