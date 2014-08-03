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
			assert(m_coord);

			std::set<Coordinate> ret;

			m_match.forReachableCoords(*m_coord, range, [&](Coordinate coord, Piece* piece) {
				if(!piece ||
					(
						piece->getColor() != m_color &&
						piece->getType() != PieceType::MOUNTAINS
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
			assert(m_coord);

			auto& bearingTable = m_match.getBearingTable();

			std::set<Coordinate> ret;

			m_match.forReachableCoords(*m_coord, range, [&](Coordinate coord, Piece* piece) {
				if(!piece ||
					(
						piece->getColor() != m_color &&
						piece->getType() != PieceType::MOUNTAINS &&
						bearingTable.canTake(this, piece)
					)
				   )
				{
					auto res = ret.insert(coord);
					assert(res.second);
				}
			});

			return ret;
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces(const MovementRange& range) const
		{
			assert(m_coord);

			std::set<const Piece*> ret;

			m_match.forReachableCoords(*m_coord, range, [&](Coordinate, Piece* piece) {
				if(piece && piece->getColor() != m_color &&
				piece->getType() != PieceType::MOUNTAINS)
				{
					auto res = ret.insert(piece);
					assert(res.second);
				}
			});

			return ret;
		}

		bool Piece::moveToValid(Coordinate target) const
		{
			std::shared_ptr<Piece> opPiece = m_match.getPieceAt(target);

			return canReach(target) &&
				(!opPiece || m_match.getBearingTable().canTake(this, opPiece.get()));
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

			auto it = data.find(m_type);
			if(it == data.end())
				return 0;

			return it->second;
		}

		uint_least8_t Piece::getEffectiveDefenseTier() const
		{
			assert(m_coord);

			auto baseTier = getBaseTier();

			if(baseTier < 1 || baseTier >= 4)
				return baseTier;

			auto fortress = m_match.getPlayer(m_color)->getFortress();

			if(fortress && fortress->getCoord() == *m_coord)
				return ++baseTier;

			auto terrainIt = m_match.getTerrain().find(*m_coord);
			if(terrainIt != m_match.getTerrain().end() &&
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

			auto it = data.find(m_type);
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

			auto it = data.find(m_type);
			if(it == data.end())
				return TerrainType::UNDEFINED;

			return it->second;
		}

		const MovementScope& Piece::getMovementScope() const
		{
			static const std::map<PieceType, MovementScope> data {
					{PieceType::MOUNTAINS,    MovementScope(MovementType::NONE,       0)},
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

			assert(m_type != PieceType::UNDEFINED);

			return data.at(m_type);
		}

		bool Piece::canReach(Coordinate target) const
		{
			auto scope = getMovementScope();
			std::valarray<int_least8_t> step(2);

			switch(scope.first)
			{
				case MovementType::ORTHOGONAL:
					switch(m_coord->getDirectionOrthogonal(target))
					{
						case DirectionOrthogonal::TOP_LEFT:     step = stepsOrthogonal.at(0); break;
						case DirectionOrthogonal::TOP_RIGHT:    step = stepsOrthogonal.at(1); break;
						case DirectionOrthogonal::RIGHT:        step = stepsOrthogonal.at(2); break;
						case DirectionOrthogonal::BOTTOM_RIGHT: step = stepsOrthogonal.at(3); break;
						case DirectionOrthogonal::BOTTOM_LEFT:  step = stepsOrthogonal.at(4); break;
						case DirectionOrthogonal::LEFT:         step = stepsOrthogonal.at(5); break;
						default: break; // disable compiler warning about unhandled enum value
					}
					break;
				case MovementType::DIAGONAL:
					switch(m_coord->getDirectionDiagonal(target))
					{
						case DirectionDiagonal::TOP:          step = stepsDiagonal.at(0); break;
						case DirectionDiagonal::TOP_RIGHT:    step = stepsDiagonal.at(1); break;
						case DirectionDiagonal::BOTTOM_RIGHT: step = stepsDiagonal.at(2); break;
						case DirectionDiagonal::BOTTOM:       step = stepsDiagonal.at(3); break;
						case DirectionDiagonal::BOTTOM_LEFT:  step = stepsDiagonal.at(4); break;
						case DirectionDiagonal::TOP_LEFT:     step = stepsDiagonal.at(5); break;
						default: break; // disable compiler warning about unhandled enum value
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

			assert(m_coord);

			bool ret = false;

			if(step[0] != 0 || step[1] != 0) // not the default values
			{
				auto distance = scope.second;
				if(!distance)
				{
					if(scope.first == MovementType::ORTHOGONAL)
						distance = (Hexagon::edgeLength - 1) * 2;
					else if(scope.first == MovementType::DIAGONAL)
						distance = Hexagon::edgeLength - 1;
				}

				m_match.forReachableCoords(*m_coord, {{step}, distance}, [&](Coordinate coord, Piece*) {
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

			const CoordPieceMap& activePieces = m_match.getActivePieces();

			TileStateMap ret;

			for(Coordinate centerCoord : m_match.getHexagonMovementCenters())
			{
				TileStateVec tmpTileVec;

				int_least8_t centerDistance = centerCoord.getDistance(*m_coord);

				if(!centerDistance) // standing on the movement center
					continue;

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
							if(*tmpCoord == *m_coord)
								tileState = TileState::START;
							else
							{
								auto it = activePieces.find(*tmpCoord);
								if(it != activePieces.end())
								{
									if(it->second->getType() == PieceType::MOUNTAINS ||
									   it->second->getColor() == m_color)
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
			}

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
				{
					std::unique_ptr<Coordinate> startCoord;

					if(m_coord)
						startCoord = make_unique(m_coord);
					else
					{
						// this is dependent on the piece being a dragon,
						// we just assert that the MovementType RANGE and
						// the PieceType DRAGON imply each another

						auto fortress = m_match.getPlayer(m_color)->getFortress();

						if(!fortress) // fortress ruined, dragon can't be brought out
							return std::set<Coordinate>();

						startCoord = make_unique<Coordinate>(fortress->getCoord());
					}

					assert(startCoord);

					// completely unnecessary, and not perfect
					// ... but would probably work! :)
					if(!distance)
						distance = Hexagon::tileCount / 2;

					std::set<Coordinate> lastTiles {*startCoord};
					std::set<Coordinate> tiles;

					// start with i = 1 because the first step is already done with
					for(int i = 0; i < distance; ++i)
					{
						for(auto tile : lastTiles)
						{
							// adjacent tiles of tile
							m_match.forReachableCoords(tile, {stepsOrthogonal, 1}, [&](Coordinate coord, Piece* piece) {
								if(!piece || piece->getType() == PieceType::MOUNTAINS || piece->getColor() == !m_color)
								{
									auto it = tiles.find(coord);
									if(it == tiles.end())
									{
										if(!piece || piece->getType() == PieceType::MOUNTAINS)
											tiles.insert(coord);

										if(piece->getType() != PieceType::MOUNTAINS)
											ret.insert(coord);
									}
								}
							});
						}

						lastTiles = tiles;
						tiles.clear();
					}

					break;
				}
				default:
					assert(0);
			}

			return ret;
		}

		std::set<Coordinate> Piece::getPossibleTargetTiles() const
		{
			if(m_type != PieceType::DRAGON)
				assert(m_coord);

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

							std::shared_ptr<Piece> opPiece = m_match.getPieceAt(tile.first);
							assert(opPiece);

							if(m_match.getBearingTable().canTake(this, opPiece.get()))
								ret.insert(tile.first);
						}
					}

					break;
				case MovementType::RANGE:
					ret = getReachableTiles();
					break;
				default:
					assert(0);
			}

			return ret;
		}

		std::set<const Piece*> Piece::getReachableOpponentPieces() const
		{
			assert(m_type != PieceType::DRAGON);
			assert(m_type != PieceType::MOUNTAINS);

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
							const CoordPieceMap& activePieces = m_match.getActivePieces();

							auto it = activePieces.find(tile.first);
							assert(it != activePieces.end());
							assert(it->second->getColor() == !m_color);
							assert(it->second->getType() != PieceType::MOUNTAINS);

							auto res = ret.insert(it->second.get());
							assert(res.second);
						}
					}
					break;
				default:
					assert(0);
			}

			return ret;
		}

		bool Piece::moveTo(Coordinate target, bool setup)
		{
			if(!(setup || moveToValid(target)))
				return false;

			CoordPieceMap& activePieces = m_match.getActivePieces();
			Player& player = *m_match.getPlayer(m_color);

			std::shared_ptr<Piece> selfSharedPtr;

			if(m_coord)
			{
				auto pieceIt = activePieces.find(*m_coord);
				assert(pieceIt != activePieces.end());

				selfSharedPtr = pieceIt->second;
				activePieces.erase(pieceIt);

				if(setup)
				{
					if(m_type == PieceType::KING)
						player.getFortress()->setCoord(target);
					else
					{
						TerrainType tType = getSetupTerrain();

						if(tType != TerrainType::UNDEFINED)
						{
							auto terrainIt = m_match.getTerrain().find(*m_coord);
							assert(terrainIt != m_match.getTerrain().end());

							terrainIt->second->setCoord(target);
						}
					}
				}
			}
			else
			{
				// piece is not on the board

				auto& inactivePieces = player.getInactivePieces();
				auto pieceIt = std::find_if(
					inactivePieces.begin(),
					inactivePieces.end(),
					[this](std::pair<const PieceType, std::shared_ptr<Piece>>& it) { return it.second.get() == this; }
				);

				assert(pieceIt != inactivePieces.end());
				selfSharedPtr = pieceIt->second;

				player.getInactivePieces().erase(pieceIt);

				if(m_type == PieceType::DRAGON)
					player.dragonBroughtOut();
			}

			assert(selfSharedPtr.get() == this);

			m_coord = make_unique<Coordinate>(target);

			auto it = activePieces.find(target);
			if(it != activePieces.end())
			{
				assert(it->second->getColor() == !m_color);
				m_match.removeFromBoard(it->second);
			}

			auto res = activePieces.emplace(target, selfSharedPtr);
			assert(res.second);

			return true;
		}

		void Piece::promoteTo(PieceType type)
		{
			assert(m_coord);

			std::shared_ptr<Piece> selfSharedPtr = m_match.getPieceAt(*m_coord);

			m_match.removeFromBoard(selfSharedPtr);
			m_match.addToBoard(type, m_color, *m_coord);
		}
	}
}
