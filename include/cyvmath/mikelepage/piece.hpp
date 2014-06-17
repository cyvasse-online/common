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
#include "common.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Piece : public cyvmath::Piece
		{
			public:
				Piece(PlayersColor color, PieceType type, dc::unique_ptr<cyvmath::Coordinate>&& coord, PieceMap& map)
					: cyvmath::Piece(color, type, std::move(coord), map)
				{ }

				virtual ~Piece() = default;

				const Movement& getMovementData() const override
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

				virtual bool moveTo(const CoordinateDcUqP& coord, bool checkMoveValidity) override
				{
					if(checkMoveValidity)
					{
						// TODO: Check if the movement is legal, return if the check fails
					}

					PieceMap::iterator it = _map.find(_coord);
					assert(it != _map.end());

					_coord = coord.clone();

					assert(it->second.get() == this);
					// add to new position in map before removing old
					// entry to ensure shared_ptr doesn't free the data
					std::pair<PieceMap::iterator, bool> res = _map.emplace(coord.clone(), it->second);
					_map.erase(it);

					// assert there was no other piece already on coord.
					// the check for that is probably better to do outside this
					// functions, but this assertion may be removed in the future.
					assert(res.second);

					return true;
				}
		};

		// cyvmath -> cyvmath::mikelepage
		typedef PieceVec PieceVec;
		typedef PieceMap PieceMap;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PIECE_HPP_
