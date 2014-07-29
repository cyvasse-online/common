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

#include <array>
#include <utility>
#include <valarray>
#include <vector>
#include <cyvmath/mikelepage/match.hpp>
#include <cyvmath/mikelepage/fortress.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		const MovementVec Piece::stepsOrthogonal {
			{-1,  1}, // top left
			{ 0,  1}, // top right
			{ 1,  0}, // right
			{ 1, -1}, // bottom right
			{ 0, -1}, // bottom left
			{-1,  0}  // left
		};

		const MovementVec Piece::stepsDiagonal {
			{-1,  2}, // top
			{ 1,  1}, // top right
			{ 2, -1}, // bottom right
			{ 1, -2}, // bottom
			{-1, -1}, // bottom left
			{-2,  1}  // top left
		};

		const MovementVec Piece::stepsHexagonalLine {
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
			// for now the above is acceptable [TODO]
			/*auto scope = getMovementScope();

			int_least8_t distance = -1;
			switch(scope.first)
			{
				case Movement::ORTHOGONAL:
					distance = _coord->getDistanceOrthogonal(target);
					break;
				case Movement::DIAGONAL:
					distance = _coord->getDistanceDiagonal(target);
					break;
				case Movement::HEXAGONAL:
					// distance = _coord->getDistanceHexagonalLine(..., target);
					break;
				// disable compiler warning about unhandled values
				default: { }
			}

			if(distance == -1 || (scope.second && distance > scope.second))
				return false;

			return true;*/
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces(const MovementVec& steps, int_least8_t distance) const
		{
			// slightly altered version of getPossibleTargetTiles()
			// TODO: clean up both

			assert(_coord);

			PieceMap& activePieces = _match.getActivePieces();

			std::set<const Piece*> ret;

			for(const auto& step : steps)
			{
				assert(step.size() == 2);

				std::valarray<int_least8_t> tmpPos = _coord->toValarray();
				auto tmpCoord = make_unique(_coord);

				for(auto i = 0; i < distance; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = Coordinate::create(tmpPos);

					// if one step into this direction results in a
					// invalid coordinate, all further ones do too
					if(!tmpCoord)
						break;

					auto it = activePieces.find(*tmpCoord);
					if(it != activePieces.end())
					{
						if(it->second->getType() != PieceType::MOUNTAIN &&
						   it->second->getColor() != _color)
						{
							auto res = ret.insert(it->second.get());
							assert(res.second);
						}
						break;
					}
				}
			}

			return ret;
		}

		std::set<Coordinate> Piece::getPossibleTargetTiles(const MovementVec& steps, int_least8_t distance) const
		{
			assert(_coord);

			PieceMap& activePieces = _match.getActivePieces();

			std::set<Coordinate> set;

			for(const auto& step : steps)
			{
				assert(step.size() == 2);

				std::valarray<int_least8_t> tmpPos = _coord->toValarray();
				auto tmpCoord = make_unique(_coord);

				for(auto i = 0; i < distance; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = Coordinate::create(tmpPos);

					// if one step into this direction results in a
					// invalid coordinate, all further ones do too
					if(!tmpCoord)
						break;

					// the same is true for a piece blocking the way but when
					// it's an opponents piece and it can be taken, then add
					// the tile to the return vec before ending the loop
					auto it = activePieces.find(*tmpCoord);
					if(it != activePieces.end())
					{
						if(it->second->getType() != PieceType::MOUNTAIN &&
						   it->second->getColor() != _color &&
						   _match.getBearingTable().canTake(this, it->second.get()))
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

		Tier Piece::getBaseTier() const
		{
			static const std::map<PieceType, Tier> data {
				{PieceType::RABBLE,      Tier::_1},
				{PieceType::KING,        Tier::_1},
				{PieceType::CROSSBOWS,   Tier::_2},
				{PieceType::SPEARS,      Tier::_2},
				{PieceType::LIGHT_HORSE, Tier::_2},
				{PieceType::TREBUCHET,   Tier::_3},
				{PieceType::ELEPHANT,    Tier::_3},
				{PieceType::HEAVY_HORSE, Tier::_3},
				{PieceType::DRAGON,      Tier::_4}
			};

			auto it = data.find(_type);
			if(it == data.end())
				return Tier::UNDEFINED;

			return it->second;
		}

		Tier Piece::getEffectiveDefenseTier() const
		{
			static const std::map<Tier, Tier> nextTier {
				{Tier::_1, Tier::_2},
				{Tier::_2, Tier::_3},
				{Tier::_3, Tier::_4}
			};

			assert(_coord);

			Tier baseTier = getBaseTier();

			if(baseTier == Tier::UNDEFINED || baseTier == Tier::_4)
				return baseTier;

			auto fortress = _match.getPlayer(_color)->getFortress();

			if(fortress && fortress->getCoord() == *_coord)
				return nextTier.at(baseTier);

			auto terrainIt = _match.getTerrain().find(*_coord);
			if(terrainIt != _match.getTerrain().end() &&
			   terrainIt->second->getType() == getHomeTerrain())
			{
				return nextTier.at(baseTier);
			}

			return baseTier;
		}

		TerrainType Piece::getHomeTerrain() const
		{
			static const std::map<PieceType, TerrainType> data {
				{PieceType::CROSSBOWS,   TerrainType::HILL},
				{PieceType::SPEARS,      TerrainType::FOREST},
				{PieceType::LIGHT_HORSE, TerrainType::GRASSLAND},
				{PieceType::TREBUCHET,   TerrainType::HILL},
				{PieceType::ELEPHANT,    TerrainType::FOREST},
				{PieceType::HEAVY_HORSE, TerrainType::GRASSLAND}
			};

			auto it = data.find(_type);
			if(it == data.end())
				return TerrainType::UNDEFINED;

			return it->second;
		}

		TerrainType Piece::getSetupTerrain() const
		{
			static const std::map<PieceType, TerrainType> data {
				{PieceType::CROSSBOWS,   TerrainType::HILL},
				{PieceType::SPEARS,      TerrainType::FOREST},
				{PieceType::LIGHT_HORSE, TerrainType::GRASSLAND}
			};

			auto it = data.find(_type);
			if(it == data.end())
				return TerrainType::UNDEFINED;

			return it->second;
		}

		const MovementScope& Piece::getMovementScope() const
		{
			static const std::map<PieceType, MovementScope> data {
					{PieceType::MOUNTAIN,    MovementScope(MovementType::NONE,       0)},
					{PieceType::RABBLE,      MovementScope(MovementType::ORTHOGONAL, 1)},
					{PieceType::CROSSBOWS,   MovementScope(MovementType::ORTHOGONAL, 3)},
					{PieceType::SPEARS,      MovementScope(MovementType::DIAGONAL,   2)},
					{PieceType::LIGHT_HORSE, MovementScope(MovementType::HEXAGONAL,  3)},
					{PieceType::TREBUCHET,   MovementScope(MovementType::ORTHOGONAL, 0)},
					{PieceType::ELEPHANT,    MovementScope(MovementType::DIAGONAL,   0)},
					{PieceType::HEAVY_HORSE, MovementScope(MovementType::HEXAGONAL,  0)},
					{PieceType::DRAGON,      MovementScope(MovementType::RANGE,      4)},
					{PieceType::KING,        MovementScope(MovementType::ORTHOGONAL, 1)},
				};

			assert(_type != PieceType::UNDEFINED);

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

				if(!checkMoveValidity)
				{
					if(_type == PieceType::KING)
						player.getFortress()->setCoord(target);
					else
					{
						TerrainType tType = getSetupTerrain();

						if(tType != TerrainType::UNDEFINED)
						{
							auto it = _match.getTerrain().find(*_coord);
							assert(it != _match.getTerrain().end());

							it->second->setCoord(target);
						}
					}
				}
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
				_match.getBearingTable().add(this);
			}

			assert(selfSharedPtr.get() == this);

			_coord = make_unique<Coordinate>(target);

			auto it = activePieces.find(target);
			if(it != activePieces.end())
			{
				assert(it->second->getColor() == !_color);
				_match.removeFromBoard(it->second);
			}

			auto res = activePieces.emplace(target, selfSharedPtr);
			assert(res.second);

			if(checkMoveValidity)
				_match.getBearingTable().update(this);

			return true;
		}

		bool Piece::canReach(Coordinate coord) const
		{
			auto scope = getMovementScope();
			std::valarray<int_least8_t> step(2);

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					switch(_coord->getDirectionOrthogonal(coord))
					{
						case DirectionOrthogonal::TOP_LEFT:     step = stepsOrthogonal.at(0); break;
						case DirectionOrthogonal::TOP_RIGHT:    step = stepsOrthogonal.at(1); break;
						case DirectionOrthogonal::RIGHT:        step = stepsOrthogonal.at(2); break;
						case DirectionOrthogonal::BOTTOM_RIGHT: step = stepsOrthogonal.at(3); break;
						case DirectionOrthogonal::BOTTOM_LEFT:  step = stepsOrthogonal.at(4); break;
						case DirectionOrthogonal::LEFT:         step = stepsOrthogonal.at(5); break;
					}
					break;
				case MovementType::DIAGONAL:
					switch(_coord->getDirectionDiagonal(coord))
					{
						case DirectionDiagonal::TOP:          step = stepsDiagonal.at(0); break;
						case DirectionDiagonal::TOP_RIGHT:    step = stepsDiagonal.at(1); break;
						case DirectionDiagonal::BOTTOM_RIGHT: step = stepsDiagonal.at(2); break;
						case DirectionDiagonal::BOTTOM:       step = stepsDiagonal.at(3); break;
						case DirectionDiagonal::BOTTOM_LEFT:  step = stepsDiagonal.at(4); break;
						case DirectionDiagonal::TOP_LEFT:     step = stepsDiagonal.at(5); break;
					}
					break;
				default:
					for(const Piece* it : getReachableOpponentPieces())
					{
						assert(it->getCoord());
						if(*it->getCoord() == coord)
							return true;
					}
			}

			if(step.sum() != 0) // not the default values
			{
				const PieceMap& activePieces = _match.getActivePieces();

				auto distance = scope.second;
				if(!distance)
				{
					if(scope.first == MovementType::ORTHOGONAL)
						distance = (Hexagon::edgeLength - 1) * 2;
					else if(scope.first == MovementType::DIAGONAL)
						distance = Hexagon::edgeLength - 1;
				}

				std::valarray<int_least8_t> tmpPos = _coord->toValarray();
				auto tmpCoord = make_unique(_coord);

				for(auto i = 0; i < distance; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = Coordinate::create(tmpPos);

					// invalid coordinate
					if(!tmpCoord)
						break;

					if(coord == *tmpCoord)
						return true;

					// a piece that is blocking the way
					auto it = activePieces.find(*tmpCoord);
					if(it != activePieces.end())
						break;
				}
			}

			return false;
		}

		TileStateVec Piece::getHexagonalLineTiles() const
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			assert(scope.first == MovementType::HEXAGONAL);

			if(!distance)
				distance = (Hexagon::edgeLength - 1) * 6;

			const PieceMap& activePieces = _match.getActivePieces();

			TileStateVec ret;

			size_t i = 0;
			for(Coordinate centerCoord : _match.getHexagonMovementCenters())
			{
				TileStateVec tmpTileVec;

				int_least8_t centerDistance = centerCoord.getDistance(*_coord);

				// begin in top left of the hexagonal line
				std::valarray<int_least8_t> tmpPos {
					// explicit cast to avoid compiler warning
					int_least8_t(centerCoord.x() - centerDistance),
					int_least8_t(centerCoord.y() + centerDistance)
				};

				for(const auto& step : stepsHexagonalLine)
				{
					assert(step.size() == 2);

					for(auto i = 0; i < centerDistance; i++)
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
									else
										tileState = TileState::OP_OCCUPIED;
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

						if(tmpTileState == TileState::EMPTY || tmpTileState == TileState::OP_OCCUPIED)
						{
							auto coord = Coordinate::create(tileIt->first);
							assert(coord);

							ret.push_back(*tileIt);
						}

						if(tmpTileState != TileState::EMPTY)
							break;
					}
				}

				++i;
			}

			assert(i == 2);

			return ret;
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces() const
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					return getReachableOpponentPieces(stepsOrthogonal, distance);
					break;
				case MovementType::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					return getReachableOpponentPieces(stepsDiagonal, distance);
					break;
				case MovementType::HEXAGONAL:
				{
					std::set<const Piece*> ret;

					for(std::pair<std::valarray<int_least8_t>, TileState>& state : getHexagonalLineTiles())
					{
						if(state.second == TileState::OP_OCCUPIED)
						{
							auto coord = Coordinate::create(state.first);
							assert(coord);

							const PieceMap& activePieces = _match.getActivePieces();

							auto it = activePieces.find(*coord);
							assert(it != activePieces.end());
							assert(it->second->getColor() == !_color);
							assert(it->second->getType() != PieceType::MOUNTAIN);

							auto res = ret.insert(it->second.get());
							assert(res.second);
						}
					}

					return ret;
				}
				case MovementType::RANGE:
					// ...
					break;
			}

			return std::set<const Piece*>();
		}

		std::set<Coordinate> Piece::getPossibleTargetTiles() const
		{
			auto scope = getMovementScope();
			auto distance = scope.second;

			if(_type != PieceType::DRAGON)
				assert(_coord);

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					return getPossibleTargetTiles(stepsOrthogonal, distance);
				case MovementType::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					return getPossibleTargetTiles(stepsDiagonal, distance);
				case MovementType::HEXAGONAL:
				{
					std::set<Coordinate> ret;

					const PieceMap& activePieces = _match.getActivePieces();

					for(const auto& state : getHexagonalLineTiles())
					{
						if(state.second == TileState::EMPTY)
						{
							auto coord = Coordinate::create(state.first);
							assert(coord);

							ret.insert(*coord);
						}
						else
						{
							assert(state.second == TileState::OP_OCCUPIED);

							auto coord = Coordinate::create(state.first);
							assert(coord);

							auto opPieceIt = activePieces.find(*coord);
							assert(opPieceIt != activePieces.end());

							if(_match.getBearingTable().canTake(this, opPieceIt->second.get()))
								ret.insert(*coord);
						}
					}

					return ret;
				}
				case MovementType::RANGE:
				{
					std::set<Coordinate> ret;

					if(_coord)
					{
						// TODO
					}
					else
					{
						auto fortress = _match.getPlayer(_color)->getFortress();

						if(!fortress) // fortress ruined, dragon can't be brought out
							return std::set<Coordinate>();

						Coordinate fortressPos = fortress->getCoord();

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
					return std::set<Coordinate>();
			}
		}
	}
}
