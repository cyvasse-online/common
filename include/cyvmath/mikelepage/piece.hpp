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

		class Piece : public cyvmath::Piece
		{
			public:
				typedef std::map<Coordinate, std::shared_ptr<Piece>> PieceMap;

			protected:
				// can be none, so this is a pointer
				std::unique_ptr<Coordinate> _coord;
				PieceMap& _pieceMap;

				bool moveToValid(Coordinate);

			public:
				Piece(PlayersColor color, PieceType type, std::unique_ptr<Coordinate> coord, PieceMap& map)
					: cyvmath::Piece(color, type)
					, _coord(std::move(coord))
					, _pieceMap(map)
				{ }

				virtual ~Piece() = default;

				std::unique_ptr<Coordinate> getCoord()
				{ return make_unique(_coord); }

				CoordinateVec getPossibleTargetTiles();

				virtual const MovementScope& getMovementScope() const final override;
				virtual bool moveTo(Coordinate, bool checkMoveValidity);
		};

		typedef Piece::PieceMap PieceMap;
		typedef std::vector<std::shared_ptr<Piece>> PieceVec;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
