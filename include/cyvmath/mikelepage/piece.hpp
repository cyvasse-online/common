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
#include <valarray>
#include <vector>
#include <make_unique.hpp>
#include "common.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		using cyvmath::MovementScope;

		class Match;

		typedef std::vector<std::valarray<int_least8_t>> MovementVec;

		class Piece : public cyvmath::Piece
		{
			public:
				static const MovementVec stepsOrthogonal;
				static const MovementVec stepsDiagonal;
				static const MovementVec stepsHexagonalLine;

			protected:
				// can be none, so this is a pointer
				std::unique_ptr<Coordinate> _coord;
				Match& _match;

				bool moveToValid(Coordinate) const;
				CoordinateSet getPossibleTargetTiles(const MovementVec&, int_least8_t distance) const;

			public:
				Piece(PlayersColor color, PieceType type, std::unique_ptr<Coordinate> coord, Match& match)
					: cyvmath::Piece(color, type)
					, _coord(std::move(coord))
					, _match(match)
				{ }

				virtual ~Piece() = default;

				std::unique_ptr<Coordinate> getCoord() const
				{ return make_unique(_coord); }

				CoordinateSet getPossibleTargetTiles() const;

				virtual const MovementScope& getMovementScope() const final override;
				virtual bool moveTo(Coordinate, bool checkMoveValidity);
		};

		typedef std::map<Coordinate, std::shared_ptr<Piece>> PieceMap;
		typedef std::vector<std::shared_ptr<Piece>> PieceVec;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
