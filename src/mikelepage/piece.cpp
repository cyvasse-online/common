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
				case MovementT::ORTHOGONAL:
					distance = _coord->getDistanceOrthogonal(target);
					break;
				case MovementT::DIAGONAL:
					distance = _coord->getDistanceDiagonal(target);
					break;
				case MovementT::HEXAGONAL:
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

				std::valarray<int_least8_t> tmpPos = _coord->toValarray();
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
						if(it->second->getType() != PieceType::MOUNTAIN &&
						   it->second->getColor() != _color)
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
					{PieceType::MOUNTAIN,    MovementScope(MovementT::NONE,       0)},
					{PieceType::RABBLE,      MovementScope(MovementT::ORTHOGONAL, 1)},
					{PieceType::CROSSBOWS,   MovementScope(MovementT::ORTHOGONAL, 3)},
					{PieceType::SPEARS,      MovementScope(MovementT::DIAGONAL,   2)},
					{PieceType::LIGHT_HORSE, MovementScope(MovementT::HEXAGONAL,  3)},
					{PieceType::TREBUCHET,   MovementScope(MovementT::ORTHOGONAL, 0)},
					{PieceType::ELEPHANT,    MovementScope(MovementT::DIAGONAL,   0)},
					{PieceType::HEAVY_HORSE, MovementScope(MovementT::HEXAGONAL,  0)},
					{PieceType::DRAGON,      MovementScope(MovementT::RANGE,      4)},
					{PieceType::KING,        MovementScope(MovementT::ORTHOGONAL, 1)},
				};

			return data.at(_type);
		}

		bool Piece::moveTo(Coordinate target, bool checkMoveValidity)
		{
			if(checkMoveValidity && !moveToValid(target))
				return false;

			PieceMap& activePieces = _match.getActivePieces();
			Player& player = *_match.getPlayer(_color);

			std::shared_ptr<Piece> selfSharedPtr;

			if(_coord)
			{
				PieceMap::iterator it = activePieces.find(*_coord);
				assert(it != activePieces.end());

				selfSharedPtr = it->second;
				activePieces.erase(it);
			}
			else
			{
				// piece is not on the board
				// this means either we move the dragon
				// or there is a bug in the game
				assert(_type == PieceType::DRAGON);
				assert(player.dragonAliveInactive());

				PieceVec::iterator it;
				for(it = player.getInactivePieces().begin(); it != player.getInactivePieces().end(); ++it)
				{
					if(it->get() == this)
					{
						selfSharedPtr = *it;
						break;
					}
				}

				player.getInactivePieces().erase(it);
				player.dragonBroughtOut();
			}

			assert(selfSharedPtr.get() == this);

			_coord = make_unique<Coordinate>(target);

			auto res = activePieces.emplace(target, selfSharedPtr);
			assert(res.second);

			return true;
		}

		CoordinateSet Piece::getPossibleTargetTiles() const
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			if(_type != PieceType::DRAGON)
				assert(_coord);

			switch(scope.first)
			{
				case MovementT::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					return getPossibleTargetTiles(stepsOrthogonal, distance);
				case MovementT::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					return getPossibleTargetTiles(stepsDiagonal, distance);
				case MovementT::HEXAGONAL:
				{
					typedef enum class TileState
					{
						START,
						EMPTY,
						INACCESSIBLE,
						LAST_ACCESSIBLE
					} TileState;

					typedef std::vector<std::pair<std::valarray<int_least8_t>, TileState>> TileStateVec;

					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 6;

					PieceMap& activePieces = _match.getActivePieces();

					CoordinateSet ret;

					for(auto fPosIt : _match.getFortressPositions())
					{
						TileStateVec tmpTileVec;
						Coordinate fortressPosition = fPosIt.second;
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

								auto tileState = TileState::EMPTY;

								if(!tmpCoord)
									tileState = TileState::INACCESSIBLE;
								else
								{
									if(*tmpCoord == *_coord)
										tileState = TileState::START;
									else
									{
										auto it = activePieces.find(*tmpCoord);
										if(it != activePieces.end())
										{
											if(it->second->getType() == PieceType::MOUNTAIN ||
											   it->second->getColor() == _color)
												tileState = TileState::INACCESSIBLE;
											else // TODO: Check whether the piece can be attacked
												tileState = TileState::LAST_ACCESSIBLE;
										}
									}
								}

								tmpTileVec.emplace_back(tmpPos, tileState);
							}
						}

						TileStateVec::iterator startTileIt;
						for(startTileIt = tmpTileVec.begin(); startTileIt != tmpTileVec.end(); ++startTileIt)
							if(startTileIt->second == TileState::START) break;

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

								if(tmpTileState == TileState::EMPTY || tmpTileState == TileState::LAST_ACCESSIBLE)
								{
									auto coord = Coordinate::create(tileIt->first);
									assert(coord);

									ret.insert(*coord);
								}

								if(tmpTileState != TileState::EMPTY)
									break;
							}
						}
					}

					return ret;
				}
				case MovementT::RANGE:
				{
					CoordinateSet ret;

					if(_coord)
					{
						// TODO
					}
					else
					{
						// TODO: revise when terrain is added

						Coordinate fortressPos = _match.getFortressPositions().at(_color);

						auto it = _match.getActivePieces().find(fortressPos);
						// fortress is empty or has an opponents piece in it
						if(it == _match.getActivePieces().end() || it->second->getColor() != _color)
							ret.insert(fortressPos);

						// check adjacent tiles of the fortress
						for(const auto& step : stepsOrthogonal)
						{
							auto tmpCoord = Coordinate::create(fortressPos.toValarray() + step);

							if(tmpCoord)
							{
								auto it = _match.getActivePieces().find(*tmpCoord);
								if(it == _match.getActivePieces().end() || it->second->getColor() != _color)
									ret.insert(*tmpCoord);
							}
						}
					}

					return ret;
				}
				default:
					return CoordinateSet();
			}
		}
	}
}
