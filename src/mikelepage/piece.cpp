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

#include <cyvmath/mikelepage/piece.hpp>

#include <cyvmath/mikelepage/common.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		bool Piece::moveToValid(Coordinate target)
		{
			auto scope = getMovementScope();

			int_least8_t distance = -1;
			switch(scope.first)
			{
				case MOVEMENT_ORTHOGONAL:
					distance = _coord->getDistanceOrthogonal(target);
					break;
				case MOVEMENT_DIAGONAL:
					distance = _coord->getDistanceDiagonal(target);
					break;
				case MOVEMENT_HEXAGONAL:
					// distance = _coord->getDistanceHexagonalLine(..., target);
					break;
				// disable compiler warning about unhandled values
				default: { }
			}

			if(distance == -1 || (scope.second && distance > scope.second))
				return false;

			return true;
		}

		const MovementScope& Piece::getMovementScope() const
		{
			static const std::map<PieceType, MovementScope> data {
					{PIECE_MOUNTAIN,    MovementScope(MOVEMENT_NONE,       0)},
					{PIECE_RABBLE,      MovementScope(MOVEMENT_ORTHOGONAL, 1)},
					{PIECE_CROSSBOWS,   MovementScope(MOVEMENT_ORTHOGONAL, 3)},
					{PIECE_SPEARS,      MovementScope(MOVEMENT_DIAGONAL,   2)},
					{PIECE_LIGHT_HORSE, MovementScope(MOVEMENT_HEXAGONAL,  3)},
					{PIECE_TREBUCHET,   MovementScope(MOVEMENT_ORTHOGONAL, 0)},
					{PIECE_ELEPHANT,    MovementScope(MOVEMENT_DIAGONAL,   0)},
					{PIECE_HEAVY_HORSE, MovementScope(MOVEMENT_HEXAGONAL,  0)},
					{PIECE_DRAGON,      MovementScope(MOVEMENT_RANGE,      4)},
					{PIECE_KING,        MovementScope(MOVEMENT_ORTHOGONAL, 1)},
				};

			return data.at(_type);
		}

		bool Piece::moveTo(Coordinate target, bool checkMoveValidity)
		{
			if(checkMoveValidity && !moveToValid(target))
				return false;

			PieceMap::iterator it = _pieceMap.find(*_coord);
			if(it != _pieceMap.end())
			{
				// moving a piece that's already on the board
				// this is always the case until the dragon
				// and promotion stuff is added to the game.

				_coord = make_unique<Coordinate>(target);

				assert(it->second.get() == this);
				// add to new position in map before removing old
				// entry to ensure shared_ptr doesn't free the data
				std::pair<PieceMap::iterator, bool> res = _pieceMap.emplace(*_coord, it->second);

				if(!res.second) // there was already a piece on the target tile
					return false;
			}
			// else ... [TODO]

			if(it != _pieceMap.end())
				_pieceMap.erase(it);

			return true;
		}

		CoordinateVec Piece::getPossibleTargetTiles()
		{
			auto scope = getMovementScope();

			switch(scope.first)
			{
				case MOVEMENT_ORTHOGONAL:
					return scope.second ? _coord->getCoordinatesOrthogonal(scope.second)
						: _coord->getCoordinatesOrthogonal();
				case MOVEMENT_DIAGONAL:
					return scope.second ? _coord->getCoordinatesDiagonal(scope.second)
						: _coord->getCoordinatesDiagonal();
				case MOVEMENT_HEXAGONAL:
				//	return scope.second ? _coord->getCoordinatesHexagonalLine(..., scope.second)
				//		: _coord->getCoordinatesHexagonalLine(...);
				default:
					return CoordinateVec();
			}
		}
	}
}
