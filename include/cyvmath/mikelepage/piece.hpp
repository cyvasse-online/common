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

#ifndef _CYVMATH_MIKELEPAGE_PIECE_HPP_
#define _CYVMATH_MIKELEPAGE_PIECE_HPP_

#include <cyvmath/piece.hpp>

#include <map>
#include <memory>
#include <set>
#include <valarray>
#include <vector>
#include <make_unique.hpp>
#include <cyvmath/coordinate.hpp>
#include "terrain_type.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		using cyvmath::MovementScope;

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
				static const MovementVec stepsOrthogonal;
				static const MovementVec stepsDiagonal;
				static const MovementVec stepsHexagonalLine;

			protected:
				Match& m_match;

				bool moveToValid(Coordinate) const;

				std::set<Coordinate> getReachableTiles(const MovementRange&) const;
				std::set<Coordinate> getPossibleTargetTiles(const MovementRange&) const;
				std::set<const Piece*> getReachableOpponentPieces(const MovementRange&) const;

			public:
				Piece(PlayersColor color, PieceType type, Coordinate coord, Match& match)
					: cyvmath::Piece(color, type, make_unique<Coordinate>(coord))
					, m_match(match)
				{ }

				virtual ~Piece() = default;

				std::unique_ptr<Coordinate> getCoord() const
				{ return make_unique<Coordinate>(dynamic_cast<Coordinate&>(*m_coord)); }

				// ugly hack for piece promotion
				void setCoord(Coordinate coord)
				{ m_coord = make_unique<Coordinate>(coord); }

				uint8_t getBaseTier() const;
				uint8_t getEffectiveDefenseTier() const;
				TerrainType getHomeTerrain() const;
				TerrainType getSetupTerrain() const;
				virtual const MovementScope& getMovementScope() const final override;

				bool canReach(Coordinate) const;

				TileStateMap getHexagonalLineTiles() const;
				std::set<Coordinate> getReachableTiles() const;
				std::set<Coordinate> getPossibleTargetTiles() const;
				std::set<const Piece*> getReachableOpponentPieces() const;

				virtual bool moveTo(Coordinate, bool setup);
				virtual void promoteTo(PieceType);
		};

		// TODO
		typedef std::map<Coordinate, std::shared_ptr<Piece>> CoordPieceMap;
		typedef std::multimap<PieceType, std::shared_ptr<Piece>> TypePieceMap;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
