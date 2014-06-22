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

#ifndef _CYVMATH_PIECE_HPP_
#define _CYVMATH_PIECE_HPP_

#include <map>
#include <memory>
#include <utility>
#include "coordinate.hpp"
#include "players_color.hpp"

namespace cyvmath
{
	enum PieceType
	{
		PIECE_MOUNTAIN,
		PIECE_RABBLE,
		PIECE_CROSSBOWS,
		PIECE_SPEARS,
		PIECE_LIGHT_HORSE,
		PIECE_TREBUCHET,
		PIECE_ELEPHANT,
		PIECE_HEAVY_HORSE,
		PIECE_DRAGON,
		PIECE_KING
	};

	enum MovementType
	{
		MOVEMENT_NONE,
		MOVEMENT_ORTHOGONAL,
		MOVEMENT_DIAGONAL,
		MOVEMENT_HEXAGONAL,
		MOVEMENT_RANGE
	};

	typedef std::pair<MovementType, int8_t> MovementScope;

	class Piece
	{
		public:
			typedef std::map<CoordinateDcUqP, std::shared_ptr<Piece>, dc::managed_less<CoordinateDcUqP>> PieceMap;

		protected:
			const PlayersColor _color;
			const PieceType _type;

			// position on the board
			dc::unique_ptr<Coordinate> _coord;

			// piece map with all pieces on the board
			PieceMap& _map;

		public:
			Piece(PlayersColor color, PieceType type, dc::unique_ptr<Coordinate>&& coord, PieceMap& map)
				: _color(color)
				, _type(type)
				, _coord(std::move(coord))
				, _map(map)
			{ }

			virtual ~Piece() = default;

			PlayersColor getPlayersColor() const
			{
				return _color;
			}

			PieceType getPieceType() const
			{
				return _type;
			}

			CoordinateDcUqP getCoord() const
			{
				return _coord.clone();
			}

			virtual const MovementScope& getMovementScope() const = 0;

			/// @return true if the move was valid, false otherwise
			virtual bool moveTo(const CoordinateDcUqP&, bool checkMoveValidity) = 0;
	};

	typedef std::vector<std::shared_ptr<Piece>> PieceVec;
	typedef Piece::PieceMap PieceMap;
}

#endif // _CYVMATH_PIECE_HPP_
