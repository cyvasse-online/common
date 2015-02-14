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

#ifndef _CYVMATH_MIKELEPAGE_PIECE_HPP_
#define _CYVMATH_MIKELEPAGE_PIECE_HPP_

#include <cyvmath/piece.hpp>

#include <map>
#include <memory>
#include <set>
#include <valarray>
#include <vector>
#include <cyvmath/hexagon.hpp>
#include "terrain_type.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match;

		enum class TileState
		{
			UNDEFINED,
			START,
			EMPTY,
			INACCESSIBLE,
			OP_OCCUPIED
		};

		typedef std::map<Coordinate, TileState> TileStateMap;
		typedef std::vector<std::valarray<int8_t>> MovementVec;
		typedef std::pair<MovementVec, uint8_t> MovementRange;

		class Piece : public cyvmath::Piece
		{
			public:
				using HexCoordinate = Hexagon<6>::Coordinate;

				static const MovementVec stepsOrthogonal;
				static const MovementVec stepsDiagonal;
				static const MovementVec stepsHexagonalLine;

			protected:
				Match& m_match;

				bool moveToValid(const HexCoordinate&) const;

				auto getReachableTiles(const MovementRange&) const -> std::set<HexCoordinate>;
				auto getPossibleTargetTiles(const MovementRange&) const -> std::set<HexCoordinate>;
				auto getReachableOpponentPieces(const MovementRange&) const -> std::set<const Piece*>;

			public:
				Piece(PlayersColor color, PieceType type, const Coordinate& coord, Match& match)
					: cyvmath::Piece(color, type, make_unique<Coordinate>(coord))
					, m_match(match)
				{ }

				virtual ~Piece() = default;

				void setCoord(Coordinate);

				uint8_t getBaseTier() const;
				uint8_t getEffectiveDefenseTier() const;
				TerrainType getHomeTerrain() const;
				TerrainType getSetupTerrain() const;
				virtual const MovementScope& getMovementScope() const final override;

				bool canReach(const Coordinate&) const;

				TileStateMap getHexagonalLineTiles() const;
				auto getReachableTiles() const -> std::set<HexCoordinate>;
				auto getPossibleTargetTiles() const -> std::set<HexCoordinate>;
				auto getReachableOpponentPieces() const -> std::set<const Piece*>;

				virtual bool moveTo(const HexCoordinate&, bool setup);
				virtual void promoteTo(PieceType);
		};

		// TODO
		typedef std::map<Coordinate, std::shared_ptr<Piece>> CoordPieceMap;
		typedef std::multimap<PieceType, std::shared_ptr<Piece>> TypePieceMap;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
