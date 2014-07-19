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

#include <vector>
#include <cyvmath/mikelepage/common.hpp>
#include <cyvmath/mikelepage/match.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		const MovementVec Piece::stepsOrthogonal = {
				{-1,  1}, // top left
				{ 0,  1}, // top right
				{ 1,  0}, // right
				{ 1, -1}, // bottom right
				{ 0, -1}, // bottom left
				{-1,  0}  // left
			};

		const MovementVec Piece::stepsDiagonal = {
				{-1,  2}, // top
				{ 1,  1}, // top right
				{ 2, -1}, // bottom right
				{ 1, -2}, // bottom
				{-1, -1}, // bottom left
				{-2,  1}  // top left
			};

		bool Piece::moveToValid(Coordinate target) const
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

		CoordinateSet Piece::getPossibleTargetTiles(const MovementVec& steps, int_least8_t distance) const
		{
			CoordinateSet set;

			for(auto& step : steps)
			{
				auto tmpCoord = make_unique<Coordinate>(*_coord);
				for(auto i = 0; i < distance; i++)
				{
					assert(tmpCoord);
					tmpCoord = Coordinate::create((tmpCoord->x() + step.first), (tmpCoord->y() + step.second));

					// if one step into this direction results in a
					// invalid coordinate, all further ones do too
					if(!tmpCoord)
						break;

					// the same is true for a piece blocking the way
					// but when it's an opponents piece, then add the
					// tile to the return vec before ending the loop
					auto it = _match.getActivePieces().find(*tmpCoord);
					if(it != _match.getActivePieces().end())
					{
						if(it->second->getColor() != _color)
						{
							auto res = set.insert(*tmpCoord);
							assert(res.second);
						}
						break;
					}

					auto res = set.insert(*tmpCoord);
					assert(res.second);
				}
			}

			return set;
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

			PieceMap& activePieces = _match.getActivePieces();

			PieceMap::iterator it = activePieces.find(*_coord);
			if(it != activePieces.end())
			{
				// moving a piece that's already on the board
				// this is always the case until the dragon
				// and promotion stuff is added to the game.

				_coord = make_unique<Coordinate>(target);

				assert(it->second.get() == this);
				// add to new position in map before removing old
				// entry to ensure shared_ptr doesn't free the data
				auto res = activePieces.emplace(*_coord, it->second);

				if(!res.second) // there was already a piece on the target tile
					return false;
			}
			// else ... [TODO]

			if(it != activePieces.end())
				activePieces.erase(it);

			return true;
		}

		CoordinateSet Piece::getPossibleTargetTiles()
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			switch(scope.first)
			{
				case MOVEMENT_ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					return getPossibleTargetTiles(stepsOrthogonal, distance);
				case MOVEMENT_DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					return getPossibleTargetTiles(stepsDiagonal, distance);
				case MOVEMENT_HEXAGONAL:
				{
					typedef enum
					{
						TILE_STATE_EMPTY,
						TILE_STATE_NONEXISTENT,
						TILE_STATE_OCCUPIED_BY_SELF,
						TILE_STATE_OCCUPIED_BY_OPPONENT
					} TileState;

					CoordinateSet res;

					std::vector<TileState> tmpTileVec;

					// TODO

					return res;
				}
				default:
					return CoordinateSet();
			}
		}
	}
}
