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

#include <map>
#include <memory>
#include <utility>
#include "../common.hpp"
#include "../hexagon.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		typedef hexagon<6> Hexagon;
		typedef Hexagon::Coordinate Coordinate;

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

		class Piece
		{
			public:
				typedef std::map<Coordinate, Piece*> PieceMap;
				typedef std::pair<MovementType, int8_t> Movement;

			private:
				PlayersColor _color;
				PieceType _type;

			protected:
				// position as hexagon coordinate
				std::unique_ptr<Coordinate> _coord;

				// piece map with all pieces on the board
				PieceMap& _map;

			public:
				Piece(PlayersColor color, PieceType type, Coordinate* coord, PieceMap& map)
					: _color(color)
					, _type(type)
					, _coord(coord)
					, _map(map)
				{
				}

				virtual ~Piece()
				{
				}

				PlayersColor getPlayersColor() const
				{
					return _color;
				}

				PieceType getPieceType() const
				{
					return _type;
				}

				Coordinate getCoord() const
				{
					return *_coord; // creates a copy
				}

				const Movement& getMovementData() const
				{
					static const std::map<PieceType, Movement> data {
							{PIECE_MOUNTAIN,    Movement(MOVEMENT_NONE,       0)},
							{PIECE_RABBLE,      Movement(MOVEMENT_ORTHOGONAL, 1)},
							{PIECE_CROSSBOWS,   Movement(MOVEMENT_ORTHOGONAL, 3)},
							{PIECE_SPEARS,      Movement(MOVEMENT_DIAGONAL,   2)},
							{PIECE_LIGHT_HORSE, Movement(MOVEMENT_HEXAGONAL,  3)},
							{PIECE_TREBUCHET,   Movement(MOVEMENT_ORTHOGONAL, 0)},
							{PIECE_ELEPHANT,    Movement(MOVEMENT_DIAGONAL,   0)},
							{PIECE_HEAVY_HORSE, Movement(MOVEMENT_HEXAGONAL,  0)},
							{PIECE_DRAGON,      Movement(MOVEMENT_RANGE,      4)},
							{PIECE_KING,        Movement(MOVEMENT_ORTHOGONAL, 1)},
						};

					return data.at(_type);
				}
		};

		typedef std::vector<Piece*> PieceVec;
		typedef Piece::PieceMap PieceMap;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
