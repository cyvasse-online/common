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

#include "hexagon.hpp"

namespace cyvmath
{
	namespace mikelepage
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
	}
}

// leaving mikelepage and cyvmath namespaces here because we can't specialize
// std::hash else; this will hopefully be resolved with the next C++ standard
namespace std
{
	template <>
	struct hash<cyvmath::mikelepage::PieceType>
	{
		size_t operator()(const cyvmath::mikelepage::PieceType& x) const
		{
			return hash<int8_t>()(x);
		}
	};
}
		
#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
