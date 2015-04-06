/* Copyright 2014 - 2015 Jonas Platte
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

#include <cyvasse/piece.hpp>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <utility>
#include <valarray>
#include <vector>
#include <cyvasse/hexagon.hpp>
#include <cyvasse/match.hpp>
#include <cyvasse/fortress.hpp>

namespace cyvasse
{
	using namespace std;

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

	bool Piece::moveToValid(HexCoordinate<6> target) const
	{
		auto opPiece = m_match.getPieceAt(target);

		return canReach(target) &&
			(!opPiece || m_match.getBearingTable().canTake(*this, *opPiece));
	}

	auto Piece::getReachableTiles(const MovementRange& range) const -> set<HexCoordinate<6>>
	{
		assert(m_coord);

		set<HexCoordinate<6>> ret;

		m_match.forReachableCoords(*m_coord, range, [&](HexCoordinate<6> coord) {
			auto piece = m_match.getPieceAt(coord);
			if (!piece || (
					piece->get().getColor() != m_color &&
					piece->get().getType() != PieceType::MOUNTAINS
				))
				ret.insert(coord);
		});

		return ret;
	}

	auto Piece::getPossibleTargetTiles(const MovementRange& range) const -> set<HexCoordinate<6>>
	{
		assert(m_coord);

		auto& bearingTable = m_match.getBearingTable();

		set<HexCoordinate<6>> ret;

		m_match.forReachableCoords(*m_coord, range, [&](HexCoordinate<6> coord) {
			auto piece = m_match.getPieceAt(coord);
			if (!piece || (
					piece->get().getColor() != m_color &&
					piece->get().getType() != PieceType::MOUNTAINS &&
					bearingTable.canTake(*this, *piece)
				))
				ret.insert(coord);
		});

		return ret;
	}

	auto Piece::getReachableOpponentPieces(const MovementRange& range) const -> vector<reference_wrapper<const Piece>>
	{
		assert(m_coord);

		vector<reference_wrapper<const Piece>> ret;

		m_match.forReachableCoords(*m_coord, range, [&](HexCoordinate<6> coord) {
			auto piece = m_match.getPieceAt(coord);
			if (piece && piece->get().getColor() != m_color && piece->get().getType() != PieceType::MOUNTAINS)
				ret.push_back(piece->get());
		});

		return ret;
	}

	auto Piece::getBaseTier() const -> uint8_t
	{
		static const map<PieceType, uint8_t> data {
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
		if (it == data.end())
			return 0;

		return it->second;
	}

	auto Piece::getEffectiveDefenseTier() const -> uint8_t
	{
		assert(m_coord);

		auto baseTier = getBaseTier();

		if (baseTier < 1 || baseTier >= 4)
			return baseTier;

		auto fortress = m_match.getPlayer(m_color).getFortress();

		if (!fortress.isRuined && fortress.getCoord() == *m_coord)
			return ++baseTier;

		auto terrainIt = m_match.getTerrain().find(*m_coord);
		if (terrainIt != m_match.getTerrain().end() &&
			terrainIt->second->getType() == getHomeTerrain())
		{
			return ++baseTier;
		}

		return baseTier;
	}

	auto Piece::getHomeTerrain() const -> optional<TerrainType>
	{
		static const map<PieceType, TerrainType> data {
			{PieceType::CROSSBOWS,   TerrainType::HILL},
			{PieceType::SPEARS,      TerrainType::FOREST},
			{PieceType::LIGHT_HORSE, TerrainType::GRASSLAND},
			{PieceType::TREBUCHET,   TerrainType::HILL},
			{PieceType::ELEPHANT,    TerrainType::FOREST},
			{PieceType::HEAVY_HORSE, TerrainType::GRASSLAND}
		};

		auto it = data.find(m_type);
		if (it == data.end())
			return nullopt;

		return it->second;
	}

	auto Piece::getSetupTerrain() const -> optional<TerrainType>
	{
		static const map<PieceType, TerrainType> data {
			{PieceType::CROSSBOWS,   TerrainType::HILL},
			{PieceType::SPEARS,      TerrainType::FOREST},
			{PieceType::LIGHT_HORSE, TerrainType::GRASSLAND}
		};

		auto it = data.find(m_type);
		if (it == data.end())
			return nullopt;

		return it->second;
	}

	auto Piece::getMovementScope() const -> const MovementScope&
	{
		static const map<PieceType, MovementScope> data {
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

		return data.at(m_type);
	}

	bool Piece::canReach(HexCoordinate<6> target) const
	{
		auto scope = getMovementScope();
		valarray<int8_t> step(2);

		{
			//auto coord = m_coord.value();
			assert(m_coord);
			auto coord = *m_coord;

			switch (scope.first)
			{
				case MovementType::ORTHOGONAL:
					switch(coord.getDirectionOrthogonal(target))
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
					switch(coord.getDirectionDiagonal(target))
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
				// TODO: add extra case for RANGE and maybe HEXAGONAL
				default:
					for (const auto& it : getReachableTiles())
						if (it == target)
							return true;

					return false;
					break;
			}
		}

		bool ret = false;

		if (step[0] != 0 || step[1] != 0) // not the default values
		{
			auto distance = scope.second;
			if (!distance)
			{
				if (scope.first == MovementType::ORTHOGONAL)
					distance = (Hexagon<6>::edgeLength - 1) * 2;
				else if (scope.first == MovementType::DIAGONAL)
					distance = Hexagon<6>::edgeLength - 1;
			}

			m_match.forReachableCoords(*m_coord, {{step}, distance}, [&](HexCoordinate<6> coord) {
				if (coord == target)
				{
					assert(!ret);
					ret = true;
				}
			});
		}

		return ret;
	}

	auto Piece::getHexagonalLineTiles() const -> TileStateMap
	{
		typedef vector<pair<valarray<int8_t>, TileState>> TileStateVec;

		auto scope = getMovementScope();
		auto distance = scope.second;

		assert(scope.first == MovementType::HEXAGONAL);

		if (!distance)
			distance = (Hexagon<6>::edgeLength - 1) * 6;

		const CoordPieceMap& activePieces = m_match.getActivePieces();

		TileStateMap ret;

		for (HexCoordinate<6> centerCoord : m_match.getHorseMovementCenters())
		{
			TileStateVec tmpTileVec;

			int8_t centerDistance = centerCoord.getDistance(*m_coord);

			if (!centerDistance) // standing on the movement center
				continue;

			// begin in top left of the hexagonal line
			valarray<int8_t> tmpPos {
				// explicit cast to avoid compiler warning
				int8_t(centerCoord.x() - centerDistance),
				int8_t(centerCoord.y() + centerDistance)
			};

			for (const auto& step : stepsHexagonalLine)
			{
				assert(step.size() == 2);

				for (auto i = 0; i < centerDistance; i++)
				{
					tmpPos += step;
					auto tmpCoord = HexCoordinate<6>::create(tmpPos);

					auto tileState = TileState::EMPTY;

					if (!tmpCoord)
						tileState = TileState::INACCESSIBLE;
					else
					{
						if (*tmpCoord == *m_coord)
							tileState = TileState::START;
						else
						{
							auto it = activePieces.find(*tmpCoord);
							if (it != activePieces.end())
							{
								if (it->second->getType() == PieceType::MOUNTAINS ||
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
			for (startTileIt = tmpTileVec.begin(); startTileIt != tmpTileVec.end(); ++startTileIt)
				if (startTileIt->second == TileState::START) break;

			assert(startTileIt != tmpTileVec.end());

			for (auto i = 0; i < 2; i++)
			{
				TileState tmpTileState;
				auto tileIt = startTileIt;

				for (auto d = 0; d < distance; d++)
				{
					if (i)
					{
						++tileIt;
						if (tileIt == tmpTileVec.end())
							tileIt = tmpTileVec.begin();
					}
					else
					{
						--tileIt;
						if (tileIt == (tmpTileVec.begin() - 1))
							tileIt = (tmpTileVec.end() - 1);
					}

					tmpTileState = tileIt->second;

					if (tmpTileState == TileState::EMPTY || tmpTileState == TileState::OP_OCCUPIED)
						ret.emplace(HexCoordinate<6>(tileIt->first), tileIt->second);

					if (tmpTileState != TileState::EMPTY)
						break;
				}
			}
		}

		return ret;
	}

	auto Piece::getReachableTiles() const -> set<HexCoordinate<6>>
	{
		set<HexCoordinate<6>> ret;

		auto scope = getMovementScope();
		auto distance = scope.second;

		switch(scope.first)
		{
			case MovementType::ORTHOGONAL:
				if (!distance)
					distance = (Hexagon<6>::edgeLength - 1) * 2;

				ret = getReachableTiles({stepsOrthogonal, distance});
				break;
			case MovementType::DIAGONAL:
				if (!distance)
					distance = Hexagon<6>::edgeLength - 1;

				ret = getReachableTiles({stepsDiagonal, distance});
				break;
			case MovementType::HEXAGONAL:
				for (const auto& tile : getHexagonalLineTiles())
				{
					assert(tile.second != TileState::START);
					assert(tile.second != TileState::INACCESSIBLE);

					ret.insert(tile.first);
				}
				break;
			case MovementType::RANGE:
			{
				// completely unnecessary, and not perfect
				// ... but would probably work! :)
				if (!distance)
					distance = Hexagon<6>::tileCount / 2;

				assert(m_coord); // value()
				set<HexCoordinate<6>> lastTiles {*m_coord};
				set<HexCoordinate<6>> tiles;

				// start with i = 1 because the first step is already done with
				for (int i = 0; i < distance; ++i)
				{
					for (const auto& tile : lastTiles)
					{
						// adjacent tiles of tile
						m_match.forReachableCoords(tile, {stepsOrthogonal, 1}, [&](HexCoordinate<6> coord) {
							auto it = tiles.find(coord);
							if (it == tiles.end()) // if the tile wasn't already checked
							{
								auto piece = m_match.getPieceAt(coord);

								if (!piece || piece->get().getType() == PieceType::MOUNTAINS)
									tiles.insert(coord);

								if (!piece || (piece->get().getColor() == !m_color && piece->get().getType() != PieceType::MOUNTAINS))
									ret.insert(coord);
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

	auto Piece::getPossibleTargetTiles() const -> set<HexCoordinate<6>>
	{
		set<HexCoordinate<6>> ret;

		auto scope = getMovementScope();
		auto distance = scope.second;

		switch(scope.first)
		{
			case MovementType::ORTHOGONAL:
				if (!distance)
					distance = (Hexagon<6>::edgeLength - 1) * 2;

				ret = getPossibleTargetTiles({stepsOrthogonal, distance});
				break;
			case MovementType::DIAGONAL:
				if (!distance)
					distance = Hexagon<6>::edgeLength - 1;

				ret = getPossibleTargetTiles({stepsDiagonal, distance});
				break;
			case MovementType::HEXAGONAL:
				for (const auto& tile : getHexagonalLineTiles())
				{
					if (tile.second == TileState::EMPTY)
						ret.insert(tile.first);
					else
					{
						assert(tile.second == TileState::OP_OCCUPIED);

						auto opPiece = m_match.getPieceAt(tile.first);
						assert(opPiece);

						if (m_match.getBearingTable().canTake(*this, *opPiece))
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

	auto Piece::getReachableOpponentPieces() const -> vector<reference_wrapper<const Piece>>
	{
		// this function is intended for flanking, but dragons
		// as well as mountains obviously don't have part in that
		assert(m_type != PieceType::DRAGON);
		assert(m_type != PieceType::MOUNTAINS);

		vector<reference_wrapper<const Piece>> ret;

		auto scope = getMovementScope();
		auto distance = scope.second;

		switch(scope.first)
		{
			case MovementType::ORTHOGONAL:
				if (!distance)
					distance = (Hexagon<6>::edgeLength - 1) * 2;

				ret = getReachableOpponentPieces({stepsOrthogonal, distance});
				break;
			case MovementType::DIAGONAL:
				if (!distance)
					distance = Hexagon<6>::edgeLength - 1;

				ret = getReachableOpponentPieces({stepsDiagonal, distance});
				break;
			case MovementType::HEXAGONAL:
				for (const auto& tile : getHexagonalLineTiles())
				{
					if (tile.second == TileState::OP_OCCUPIED)
					{
						const CoordPieceMap& activePieces = m_match.getActivePieces();

						auto it = activePieces.find(tile.first);
						assert(it != activePieces.end());
						assert(it->second->getColor() == !m_color);
						assert(it->second->getType() != PieceType::MOUNTAINS);

						ret.push_back(*it->second);
					}
				}
				break;
			default:
				assert(0);
		}

		return ret;
	}

	bool Piece::moveTo(HexCoordinate<6> target, bool setup)
	{
		if (!(setup || moveToValid(target)))
			return false;

		auto& activePieces = m_match.getActivePieces();
		auto& player = m_match.getPlayer(m_color);

		shared_ptr<Piece> selfSharedPtr;

		if (m_coord)
		{
			auto pieceIt = activePieces.find(*m_coord);
			assert(pieceIt != activePieces.end());

			selfSharedPtr = pieceIt->second;
			activePieces.erase(pieceIt);

			if (setup)
			{
				if (m_type == PieceType::KING)
					player.getFortress().setCoord(target);
				else
				{
					if (getSetupTerrain())
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
			auto pieceIt = find_if(
				inactivePieces.begin(),
				inactivePieces.end(),
				[this](pair<const PieceType, shared_ptr<Piece>>& it) { return it.second.get() == this; }
			);

			assert(pieceIt != inactivePieces.end());
			selfSharedPtr = pieceIt->second;

			player.getInactivePieces().erase(pieceIt);
		}

		assert(selfSharedPtr.get() == this);

		m_coord = target;

		auto it = activePieces.find(target);
		if (it != activePieces.end())
		{
			assert(it->second->getColor() == !m_color);
			m_match.removeFromBoard(*it->second);
		}

		auto res = activePieces.emplace(target, selfSharedPtr);
		assert(res.second);

		if (!setup)
		{
			auto& opFortress = m_match.getPlayer(!m_color).getFortress();
			if (!opFortress.isRuined && target == opFortress.getCoord())
				opFortress.ruined();
		}

		return true;
	}

	void Piece::promoteTo(PieceType type)
	{
		//auto coord = m_coord.value();
		assert(m_coord);
		auto coord = *m_coord;

		m_match.removeFromBoard(*m_match.getPieceAt(coord)); // TODO: value() instead of operator*
		m_match.addToBoard(type, m_color, coord);
		m_match.getBearingTable().update();

		if (type == PieceType::KING)
		{
			auto& player = m_match.getPlayer(m_color);

			assert(player.isKingTaken());
			player.kingTaken(false);
		}
	}

	static const map<PieceType, uint8_t> openingPieceCounts {
		{PieceType::MOUNTAINS, 6},
		{PieceType::RABBLE, 6},
		{PieceType::KING, 1},
		{PieceType::CROSSBOWS, 2},
		{PieceType::SPEARS, 2},
		{PieceType::LIGHT_HORSE, 2},
		{PieceType::TREBUCHET, 2},
		{PieceType::ELEPHANT, 2},
		{PieceType::HEAVY_HORSE, 2},
		{PieceType::DRAGON, 1}
	};

	void evalOpeningArray(const map<PieceType, set<HexCoordinate<6>>>& pieces)
	{
		if (pieces.size() != 10)
		{
			throw runtime_error("There have to be exactly 10 piece types in the opening array (got "
				+ to_string(pieces.size()) + ")");
		}

		for (const auto& it : pieces)
		{
			auto expectedPieceCount = openingPieceCounts.at(it.first);

			if (it.second.size() != expectedPieceCount)
			{
				throw runtime_error("There have to be exactly " + to_string(expectedPieceCount) + ' ' +
					PieceTypeToStr(it.first) + " pieces in the opening array (got " + to_string(it.second.size()) + ")");
			}

			// TODO: check whether all pieces are on correct side of the board
		}
	}
}
