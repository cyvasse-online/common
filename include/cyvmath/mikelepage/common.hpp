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

#ifndef _CYVMATH_MIKELEPAGE_COMMON_HPP_
#define _CYVMATH_MIKELEPAGE_COMMON_HPP_

#include <cyvmath/hexagon.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		// cyvmath::PlayersColor -> cyvmath::mikelepage::PlayersColor
		typedef PlayersColor PlayersColor;
		using PlayersColor::PLAYER_UNDEFINED;
		using PlayersColor::PLAYER_WHITE;
		using PlayersColor::PLAYER_BLACK;

		// cyvmath::PieceType -> cyvmath::mikelepage::PieceType
		typedef PieceType PieceType;
		using PieceType::PIECE_MOUNTAIN;
		using PieceType::PIECE_RABBLE;
		using PieceType::PIECE_CROSSBOWS;
		using PieceType::PIECE_SPEARS;
		using PieceType::PIECE_LIGHT_HORSE;
		using PieceType::PIECE_TREBUCHET;
		using PieceType::PIECE_ELEPHANT;
		using PieceType::PIECE_HEAVY_HORSE;
		using PieceType::PIECE_DRAGON;
		using PieceType::PIECE_KING;

		typedef Hexagon<6> Hexagon;
		typedef Hexagon::Coordinate Coordinate;
	}
}

#endif // _CYVMATH_MIKELEPAGE_COMMON_HPP_
