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
#include "piece_type.hpp"
#include "players_color.hpp"

namespace cyvmath
{
	enum class MovementType
	{
		NONE,
		ORTHOGONAL,
		DIAGONAL,
		HEXAGONAL,
		RANGE
	};

	typedef std::pair<MovementType, int8_t> MovementScope;

	class Piece
	{
		protected:
			const PlayersColor m_color;
			const PieceType m_type;

			std::unique_ptr<Coordinate> m_coord;

		public:
			Piece(PlayersColor color, PieceType type, std::unique_ptr<Coordinate> coord)
				: m_color{color}
				, m_type{type}
				, m_coord{std::move(coord)}
			{ }

			virtual ~Piece() = default;

			PlayersColor getColor() const
			{ return m_color; }

			PieceType getType() const
			{ return m_type; }

			std::unique_ptr<Coordinate> getCoord() const;

			virtual const MovementScope& getMovementScope() const = 0;
	};
}

#endif // _CYVMATH_PIECE_HPP_
