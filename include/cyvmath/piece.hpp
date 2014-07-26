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
#include <enum_str.hpp>
#include "coordinate.hpp"
#include "players_color.hpp"

namespace cyvmath
{
	enum class PieceType
	{
		UNDEFINED,
		MOUNTAIN,
		RABBLE,
		CROSSBOWS,
		SPEARS,
		LIGHT_HORSE,
		TREBUCHET,
		ELEPHANT,
		HEAVY_HORSE,
		DRAGON,
		KING
	};

	ENUM_STR(PieceType, ({
		{PieceType::UNDEFINED, "undefined"},
		{PieceType::MOUNTAIN, "mountain"},
		{PieceType::RABBLE, "rabble"},
		{PieceType::CROSSBOWS, "crossbows"},
		{PieceType::SPEARS, "spears"},
		{PieceType::LIGHT_HORSE, "light horse"},
		{PieceType::TREBUCHET, "trebuchet"},
		{PieceType::ELEPHANT, "elephant"},
		{PieceType::HEAVY_HORSE, "heavy horse"},
		{PieceType::DRAGON, "dragon"},
		{PieceType::KING, "king"}
	}))

	enum class MovementT
	{
		NONE,
		ORTHOGONAL,
		DIAGONAL,
		HEXAGONAL,
		RANGE
	};

	typedef std::pair<MovementT, int8_t> MovementScope;

	class Piece
	{
		protected:
			const PlayersColor _color;
			const PieceType _type;

		public:
			constexpr Piece(PlayersColor color, PieceType type)
				: _color(color)
				, _type(type)
			{ }

			virtual ~Piece() = default;

			PlayersColor getColor() const
			{ return _color; }

			PieceType getType() const
			{ return _type; }

			virtual const MovementScope& getMovementScope() const = 0;
	};
}

#endif // _CYVMATH_PIECE_HPP_
