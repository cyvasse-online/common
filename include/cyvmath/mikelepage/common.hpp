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
#include <cyvmath/players_color.hpp>
#include <cyvmath/piece.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		using cyvmath::PlayersColor;
		using cyvmath::PLAYER_UNDEFINED;
		using cyvmath::PLAYER_WHITE;
		using cyvmath::PLAYER_BLACK;

		using cyvmath::PieceType;
		using cyvmath::PIECE_MOUNTAIN;
		using cyvmath::PIECE_RABBLE;
		using cyvmath::PIECE_CROSSBOWS;
		using cyvmath::PIECE_SPEARS;
		using cyvmath::PIECE_LIGHT_HORSE;
		using cyvmath::PIECE_TREBUCHET;
		using cyvmath::PIECE_ELEPHANT;
		using cyvmath::PIECE_HEAVY_HORSE;
		using cyvmath::PIECE_DRAGON;
		using cyvmath::PIECE_KING;

		typedef Hexagon<6> Hexagon;
		typedef Hexagon::Coordinate Coordinate;
		typedef Hexagon::CoordinateVec CoordinateVec;
	}
}

#endif // _CYVMATH_MIKELEPAGE_COMMON_HPP_
