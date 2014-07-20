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

#include <utility>
#include <valarray>
#include <vector>
#include <cyvmath/mikelepage/common.hpp>
#include <cyvmath/mikelepage/match.hpp>

#include <iostream>

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

		const MovementVec Piece::stepsHexagonalLine = {
			{ 1,  0}, // right
			{ 1, -1}, // bottom right
			{ 0, -1}, // bottom left
			{-1,  0}, // left
			{-1,  1}, // top left
			{ 0,  1}  // top right
		};

		bool Piece::moveToValid(Coordinate target) const
		{
			auto validCoords = getPossibleTargetTiles();

			return validCoords.find(target) != validCoords.end();

			// something like this would be better, but
			// for now the above is acceptable
			/*auto scope = getMovementScope();

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

			return true;*/
		}

		CoordinateSet Piece::getPossibleTargetTiles(const MovementVec& steps, int_least8_t distance) const
		{
			assert(_coord);

			PieceMap& activePieces = _match.getActivePieces();

			CoordinateSet set;

			for(const auto& step : steps)
			{
				assert(step.size() == 2);

				std::valarray<int_least8_t> tmpPos = {_coord->x(), _coord->y()};
				auto tmpCoord = make_unique<Coordinate>(*_coord);

				for(auto i = 0; i < distance; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = Coordinate::create(tmpPos);

					// if one step into this direction results in a
					// invalid coordinate, all further ones do too
					if(!tmpCoord)
						break;

					// the same is true for a piece blocking the way
					// but when it's an opponents piece, then add the
					// tile to the return vec before ending the loop
					auto it = activePieces.find(*tmpCoord);
					if(it != activePieces.end())
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

		CoordinateSet Piece::getPossibleTargetTiles() const
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			if(_type != PIECE_DRAGON)
				assert(_coord);

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
						TILE_STATE_START,
						TILE_STATE_EMPTY,
						TILE_STATE_INACCESSIBLE,
						TILE_STATE_LAST_ACCESSIBLE
					} TileState;

					typedef std::vector<std::pair<std::valarray<int_least8_t>, TileState>> TileStateVec;

					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 6;

					PieceMap& activePieces = _match.getActivePieces();

					CoordinateSet res;

					for(auto it : _match.getFortressPositions())
					{
						TileStateVec tmpTileVec;
						Coordinate fortressPosition = it.second;
						int_least8_t fortressDistance = fortressPosition.getDistance(*_coord);

						// begin in top left of the hexagonal line
						std::valarray<int_least8_t> tmpPos = {
							// explicit cast to avoid compiler warning
							int_least8_t(fortressPosition.x() - fortressDistance),
							int_least8_t(fortressPosition.y() + fortressDistance)
						};

						for(const auto& step : stepsHexagonalLine)
						{
							assert(step.size() == 2);

							for(auto i = 0; i < fortressDistance; i++)
							{
								tmpPos += step;
								auto tmpCoord = Coordinate::create(tmpPos);

								auto tileState = TILE_STATE_EMPTY;

								if(!tmpCoord)
									tileState = TILE_STATE_INACCESSIBLE;
								else
								{
									if(*tmpCoord == *_coord)
										tileState = TILE_STATE_START;
									else
									{
										auto it = activePieces.find(*tmpCoord);
										if(it != activePieces.end())
										{
											if(it->second->getColor() == _color)
												tileState = TILE_STATE_INACCESSIBLE;
											else // TODO: Check whether the piece can be attacked
												tileState = TILE_STATE_LAST_ACCESSIBLE;
										}
									}
								}

								tmpTileVec.emplace_back(tmpPos, tileState);
							}
						}

						TileStateVec::iterator startTileIt;
						for(startTileIt = tmpTileVec.begin(); startTileIt != tmpTileVec.end(); ++startTileIt)
							if(startTileIt->second == TILE_STATE_START) break;

						assert(startTileIt != tmpTileVec.end());

						for(auto i = 0; i < 2; i++)
						{
							TileState tmpTileState;
							auto tileIt = startTileIt;

							for(auto d = 0; d < distance; d++)
							{
								if(i)
								{
									++tileIt;
									if(tileIt == tmpTileVec.end())
										tileIt = tmpTileVec.begin();
								}
								else
								{
									--tileIt;
									if(tileIt == (tmpTileVec.begin() - 1))
										tileIt = (tmpTileVec.end() - 1);
								}

								tmpTileState = tileIt->second;

								if(tmpTileState == TILE_STATE_EMPTY || tmpTileState == TILE_STATE_LAST_ACCESSIBLE)
								{
									auto coord = Coordinate::create(tileIt->first);
									assert(coord);

									res.insert(*coord);
								}

								if(tmpTileState != TILE_STATE_EMPTY)
									break;
							}
						}
					}

					return res;
				}
				default:
					return CoordinateSet();
			}
		}
	}
}
