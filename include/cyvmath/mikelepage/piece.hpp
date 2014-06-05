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

#include <memory>
#include <unordered_map>
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

namespace cyvmath
{
	namespace mikelepage
	{
		class Piece
		{
			public:
				typedef std::unordered_map<Coordinate, Piece*> PieceMap;

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

				const std::pair<MovementType, int8_t>& getMovementData() const
				{
					static const std::unordered_map<PieceType, std::pair<MovementType, int8_t>> data = {
							{PIECE_MOUNTAIN,    std::make_pair(MOVEMENT_NONE,       0)},
							{PIECE_RABBLE,      std::make_pair(MOVEMENT_ORTHOGONAL, 1)},
							{PIECE_CROSSBOWS,   std::make_pair(MOVEMENT_ORTHOGONAL, 3)},
							{PIECE_SPEARS,      std::make_pair(MOVEMENT_DIAGONAL,   2)},
							{PIECE_LIGHT_HORSE, std::make_pair(MOVEMENT_HEXAGONAL,  3)},
							{PIECE_TREBUCHET,   std::make_pair(MOVEMENT_ORTHOGONAL, 0)},
							{PIECE_ELEPHANT,    std::make_pair(MOVEMENT_DIAGONAL,   0)},
							{PIECE_HEAVY_HORSE, std::make_pair(MOVEMENT_HEXAGONAL,  0)},
							{PIECE_DRAGON,      std::make_pair(MOVEMENT_RANGE,      4)},
							{PIECE_KING,        std::make_pair(MOVEMENT_ORTHOGONAL, 1)},
						};

					return data.at(_type);
				}
		};

		typedef std::vector<Piece*> PieceVec;
		typedef Piece::PieceMap PieceMap;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
