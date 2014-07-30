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

#include <algorithm>
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

		std::set<Coordinate> Piece::getReachableTiles(const MovementRange& range) const
		{
			assert(_coord);

			std::set<Coordinate> ret;

			_match.forReachableCoords(*_coord, range, [&](Coordinate coord, Piece* piece) {
				if(!piece ||
					(
						piece->getColor() != _color &&
						piece->getType() != PieceType::MOUNTAIN
					)
				   )
				{
					auto res = ret.insert(coord);
					assert(res.second);
				}
			});

			return ret;
		}

		std::set<Coordinate> Piece::getPossibleTargetTiles(const MovementRange& range) const
		{
			assert(_coord);

			auto& bearingTable = _match.getBearingTable();

			std::set<Coordinate> set;

			_match.forReachableCoords(*_coord, range, [&](Coordinate coord, Piece* piece) {
				if(!piece ||
					(
						piece->getColor() != _color &&
						piece->getType() != PieceType::MOUNTAIN &&
						bearingTable.canTake(this, piece)
					)
				   )
				{
					auto res = set.insert(coord);
					assert(res.second);
				}
			});

			return set;
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces(const MovementRange& range) const
		{
			assert(_coord);

			std::set<const Piece*> ret;

			_match.forReachableCoords(*_coord, range, [&](Coordinate, Piece* piece) {
				if(piece && piece->getColor() != _color &&
				piece->getType() != PieceType::MOUNTAIN)
				{
					auto res = ret.insert(piece);
					assert(res.second);
				}
			});

			return ret;
		}

		bool Piece::moveToValid(Coordinate target) const
		{
			Piece* opPiece = _match.getPieceAt(target);

			return canReach(target) &&
				(!opPiece || _match.getBearingTable().canTake(this, opPiece));
		}

		uint_least8_t Piece::getBaseTier() const
		{
			static const std::map<PieceType, uint_least8_t> data {
				{PieceType::RABBLE,      1},
				{PieceType::KING,        1},
				{PieceType::CROSSBOWS,   2},
				{PieceType::SPEARS,      2},
				{PieceType::LIGHT_HORSE, 2},
				{PieceType::TREBUCHET,   3},
				{PieceType::ELEPHANT,    3},
				{PieceType::HEAVY_HORSE, 3},
				{PieceType::DRAGON,      4}
			};

			auto it = data.find(_type);
			if(it == data.end())
				return 0;

			return it->second;
		}

		uint_least8_t Piece::getEffectiveDefenseTier() const
		{
			assert(_coord);

			auto baseTier = getBaseTier();

			if(baseTier < 1 || baseTier >= 4)
				return baseTier;

			auto fortress = _match.getPlayer(_color)->getFortress();

			if(fortress && fortress->getCoord() == *_coord)
				return ++baseTier;

			auto terrainIt = _match.getTerrain().find(*_coord);
			if(terrainIt != _match.getTerrain().end() &&
			   terrainIt->second->getType() == getHomeTerrain())
			{
				return ++baseTier;
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

		bool Piece::canReach(Coordinate target) const
		{
			auto scope = getMovementScope();
			std::valarray<int_least8_t> step(2);

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					switch(_coord->getDirectionOrthogonal(target))
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
					switch(_coord->getDirectionDiagonal(target))
					{
						case DirectionDiagonal::TOP:          step = stepsDiagonal.at(0); break;
						case DirectionDiagonal::TOP_RIGHT:    step = stepsDiagonal.at(1); break;
						case DirectionDiagonal::BOTTOM_RIGHT: step = stepsDiagonal.at(2); break;
						case DirectionDiagonal::BOTTOM:       step = stepsDiagonal.at(3); break;
						case DirectionDiagonal::BOTTOM_LEFT:  step = stepsDiagonal.at(4); break;
						case DirectionDiagonal::TOP_LEFT:     step = stepsDiagonal.at(5); break;
					}
					break;
				// TODO: add extra case for RANGE and maybe be HEXAGONAL
				default:
					for(const Coordinate it : getReachableTiles())
						if(it == target)
							return true;

					return false;
					break;
			}

			assert(_coord);

			bool ret = false;

			if(step[0] != 0 || step[1] != 0) // not the default values
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

				_match.forReachableCoords(*_coord, {{step}, distance}, [&](Coordinate coord, Piece* piece) {
					if(coord == target)
					{
						assert(!ret);
						ret = true;
					}
				});
			}

			return ret;
		}

		TileStateMap Piece::getHexagonalLineTiles() const
		{
			typedef std::vector<std::pair<std::valarray<int_least8_t>, TileState>> TileStateVec;

			auto scope = getMovementScope();
			auto distance = scope.second;

			assert(scope.first == MovementType::HEXAGONAL);

			if(!distance)
				distance = (Hexagon::edgeLength - 1) * 6;

			const PieceMap& activePieces = _match.getActivePieces();

			TileStateMap ret;

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

							ret.emplace(*coord, tileIt->second);
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

		std::set<Coordinate> Piece::getReachableTiles() const
		{
			std::set<Coordinate> ret;

			auto scope = getMovementScope();
			auto distance = scope.second;

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					ret = getReachableTiles({stepsOrthogonal, distance});
					break;
				case MovementType::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					ret = getReachableTiles({stepsDiagonal, distance});
					break;
				case MovementType::HEXAGONAL:
					for(const auto& tile : getHexagonalLineTiles())
					{
						assert(tile.second != TileState::UNDEFINED);
						assert(tile.second != TileState::START);
						assert(tile.second != TileState::INACCESSIBLE);

						ret.insert(tile.first);
					}
					break;
				case MovementType::RANGE:
					if(_coord)
					{
						// completely unnecessary, and not perfect
						// ... but would probably work! :)
						if(!distance)
							distance = Hexagon::tileCount / 2;

						std::set<Coordinate> lastTiles {*_coord};
						std::set<Coordinate> tiles;

						// start with i = 1 because the first step is already done with
						for(int i = 0; i < distance; ++i)
						{
							for(auto tile : lastTiles)
							{
								// adjacent tiles of tile
								_match.forReachableCoords(tile, {stepsOrthogonal, 1}, [&](Coordinate coord, Piece* piece) {
									if(!piece || piece->getType() == PieceType::MOUNTAIN || piece->getColor() == !_color)
									{
										auto it = tiles.find(coord);
										if(it == tiles.end())
										{
											if(!piece || piece->getType() == PieceType::MOUNTAIN)
												tiles.insert(coord);

											if(piece->getType() != PieceType::MOUNTAIN)
												ret.insert(coord);
										}
									}
								});
							}

							lastTiles = tiles;
							tiles.clear();
						}
					}
					else
					{
						// this is dependent on the piece being a dragon,
						// we just assert that the MovementType RANGE and
						// the PieceType DRAGON imply each another

						auto fortress = _match.getPlayer(_color)->getFortress();

						if(!fortress) // fortress ruined, dragon can't be brought out
							return std::set<Coordinate>();

						Coordinate fortressPos = fortress->getCoord();

						// fortress is empty or has an opponents piece in it
						Piece* tmpPiece = _match.getPieceAt(fortressPos);
						if(!tmpPiece || tmpPiece->getColor() != _color)
							ret.insert(fortressPos);

						// check adjacent tiles of the fortress
						_match.forReachableCoords(fortressPos, {stepsOrthogonal, 1}, [&](Coordinate coord, Piece* piece) {
							if(!piece || piece->getColor() != _color)
								ret.insert(coord);
						});
					}

					break;
			}

			return ret;
		}

		std::set<Coordinate> Piece::getPossibleTargetTiles() const
		{
			if(_type != PieceType::DRAGON)
				assert(_coord);

			std::set<Coordinate> ret;

			auto scope = getMovementScope();
			auto distance = scope.second;

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					ret = getPossibleTargetTiles({stepsOrthogonal, distance});
					break;
				case MovementType::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					ret = getPossibleTargetTiles({stepsDiagonal, distance});
					break;
				case MovementType::HEXAGONAL:
					for(const auto& tile : getHexagonalLineTiles())
					{
						if(tile.second == TileState::EMPTY)
							ret.insert(tile.first);
						else
						{
							assert(tile.second == TileState::OP_OCCUPIED);

							Piece* opPiece = _match.getPieceAt(tile.first);
							assert(opPiece);

							if(_match.getBearingTable().canTake(this, opPiece))
								ret.insert(tile.first);
						}
					}

					break;
				case MovementType::RANGE:
					ret = getReachableTiles();
					break;
			}

			return ret;
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces() const
		{
			assert(_type != PieceType::DRAGON);
			assert(_type != PieceType::MOUNTAIN);

			std::set<const Piece*> ret;

			auto scope = getMovementScope();
			auto distance = scope.second;

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					if(!distance)
						distance = (Hexagon::edgeLength - 1) * 2;

					ret = getReachableOpponentPieces({stepsOrthogonal, distance});
					break;
				case MovementType::DIAGONAL:
					if(!distance)
						distance = Hexagon::edgeLength - 1;

					ret = getReachableOpponentPieces({stepsDiagonal, distance});
					break;
				case MovementType::HEXAGONAL:
					for(const auto& tile : getHexagonalLineTiles())
					{
						if(tile.second == TileState::OP_OCCUPIED)
						{
							const PieceMap& activePieces = _match.getActivePieces();

							auto it = activePieces.find(tile.first);
							assert(it != activePieces.end());
							assert(it->second->getColor() == !_color);
							assert(it->second->getType() != PieceType::MOUNTAIN);

							auto res = ret.insert(it->second.get());
							assert(res.second);
						}
					}
					break;
			}

			return ret;
		}

		bool Piece::moveTo(Coordinate target, bool setup)
		{
			if(!(setup || moveToValid(target)))
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

				if(setup)
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

				auto& inactivePieces = player.getInactivePieces();
				auto it = std::find_if(
					inactivePieces.begin(),
					inactivePieces.end(),
					[this](std::shared_ptr<Piece> ptr) { return ptr.get() == this; }
				);

				assert(it != inactivePieces.end());
				selfSharedPtr = *it;

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

			if(!setup)
				_match.getBearingTable().update(this);

			return true;
		}
	}
}
