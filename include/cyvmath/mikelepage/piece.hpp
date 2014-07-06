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
#include <utility>
#include <make_unique.hpp>
#include "common.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		using cyvmath::MovementScope;

		class Piece;

		typedef std::map<Coordinate, std::shared_ptr<Piece>> PieceMap;
		typedef std::vector<std::shared_ptr<Piece>> PieceVec;
		typedef std::pair<int_least8_t, int_least8_t> Movement;
		typedef std::vector<Movement> MovementVec;

		class Piece : public cyvmath::Piece
		{
			public:
				static const MovementVec stepsOrthogonal;
				static const MovementVec stepsDiagonal;

			protected:
				// can be none, so this is a pointer
				std::unique_ptr<Coordinate> _coord;
				PieceMap& _activePieces;

				bool moveToValid(Coordinate) const;
				CoordinateVec getPossibleTargetTiles(const MovementVec&, int_least8_t distance) const;

			public:
				Piece(PlayersColor color, PieceType type, std::unique_ptr<Coordinate> coord, PieceMap& activePieces)
					: cyvmath::Piece(color, type)
					, _coord(std::move(coord))
					, _activePieces(activePieces)
				{ }

				virtual ~Piece() = default;

				std::unique_ptr<Coordinate> getCoord()
				{ return make_unique(_coord); }

				CoordinateVec getPossibleTargetTiles();

				virtual const MovementScope& getMovementScope() const final override;
				virtual bool moveTo(Coordinate, bool checkMoveValidity);
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
