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
#include "enum_str.hpp"
#include "players_color.hpp"

namespace cyvmath
{
	enum PieceType
	{
		PIECE_UNDEFINED,
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

	ENUM_STR(PieceType, ({
		{PIECE_UNDEFINED, "undefined"},
		{PIECE_MOUNTAIN, "mountain"},
		{PIECE_RABBLE, "rabble"},
		{PIECE_CROSSBOWS, "crossbows"},
		{PIECE_SPEARS, "spears"},
		{PIECE_LIGHT_HORSE, "light horse"},
		{PIECE_TREBUCHET, "trebuchet"},
		{PIECE_ELEPHANT, "elephant"},
		{PIECE_HEAVY_HORSE, "heavy horse"},
		{PIECE_DRAGON, "dragon"},
		{PIECE_KING, "king"}
	}))

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
		protected:
			const PlayersColor _color;
			const PieceType _type;

		public:
			Piece(PlayersColor color, PieceType type)
				: _color(color)
				, _type(type)
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

			virtual const MovementScope& getMovementScope() const = 0;
	};
}

#endif // _CYVMATH_PIECE_HPP_
