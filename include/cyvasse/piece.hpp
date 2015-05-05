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

#ifndef _CYVASSE_PIECE_HPP_
#define _CYVASSE_PIECE_HPP_

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <valarray>
#include <vector>

#include <optional.hpp>
#include "hexcoordinate.hpp"
#include "piece_type.hpp"
#include "players_color.hpp"
#include "terrain_type.hpp"

namespace cyvasse
{
	class Match;

	enum class MovementType
	{
		NONE,
		ORTHOGONAL,
		DIAGONAL,
		HEXAGONAL,
		RANGE
	};

	typedef std::pair<MovementType, int8_t> MovementScope;

	enum class TileState
	{
		START,
		EMPTY,
		INACCESSIBLE,
		OP_OCCUPIED
	};

	typedef std::map<HexCoordinate<6>, TileState> TileStateMap;
	typedef std::vector<std::valarray<int8_t>> MovementVec;
	typedef std::pair<MovementVec, uint8_t> MovementRange;

	class Piece
	{
		public:
			static const MovementVec stepsOrthogonal;
			static const MovementVec stepsDiagonal;
			static const MovementVec stepsHexagonalLine;

		protected:
			const PlayersColor m_color;
			const PieceType m_type;

			optional<HexCoordinate<6>> m_coord;

			Match& m_match;

			bool moveToValid(HexCoordinate<6>) const;

			auto getReachableTiles(const MovementRange&) const -> std::set<HexCoordinate<6>>;
			auto getPossibleTargetTiles(const MovementRange&) const -> std::set<HexCoordinate<6>>;
			auto getReachableOpponentPieces(const MovementRange&) const -> std::vector<std::reference_wrapper<const Piece>>;

		public:
			Piece(PlayersColor color, PieceType type, optional<HexCoordinate<6>> coord, Match& match)
				: m_color{color}
				, m_type{type}
				, m_coord{coord}
				, m_match(match)
			{ }

			virtual ~Piece() = default;

			auto getColor() const -> PlayersColor
			{ return m_color; }

			auto getType() const -> PieceType
			{ return m_type; }

			auto getCoord() const -> optional<HexCoordinate<6>>
			{ return m_coord; }

			void setCoord(HexCoordinate<6> coord)
			{ m_coord = coord; }

			auto getBaseTier() const -> uint8_t;
			auto getEffectiveDefenseTier() const -> uint8_t;
			auto getHomeTerrain() const -> optional<TerrainType>;
			auto getSetupTerrain() const -> optional<TerrainType>;
			auto getMovementScope() const -> const MovementScope&;

			bool canReach(HexCoordinate<6>) const;

			auto getHexagonalLineTiles() const -> TileStateMap;
			auto getReachableTiles() const -> std::set<HexCoordinate<6>>;
			auto getPossibleTargetTiles() const -> std::set<HexCoordinate<6>>;
			auto getReachableOpponentPieces() const -> std::vector<std::reference_wrapper<const Piece>>;

			virtual bool moveTo(HexCoordinate<6>, bool setup);
			void promoteTo(PieceType);
	};

	// TODO
	typedef std::map<HexCoordinate<6>, std::shared_ptr<Piece>> CoordPieceMap;
	typedef std::multimap<PieceType, std::shared_ptr<Piece>> TypePieceMap;

	// type from json::pieceMap
	void evalOpeningArray(const std::map<PieceType, std::set<HexCoordinate<6>>>&);
}

#endif // _CYVASSE_PIECE_HPP_
