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

#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "hexagon.hpp"
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

	typedef std::map<Coordinate, TileState> TileStateMap;
	typedef std::vector<std::valarray<int8_t>> MovementVec;
	typedef std::pair<MovementVec, uint8_t> MovementRange;

	class Piece
	{
		public:
			using HexCoordinate = Hexagon<6>::Coordinate;

			static const MovementVec stepsOrthogonal;
			static const MovementVec stepsDiagonal;
			static const MovementVec stepsHexagonalLine;

		protected:
			const PlayersColor m_color;
			const PieceType m_type;

			optional<Coordinate> m_coord;

			Match& m_match;

			bool moveToValid(const HexCoordinate&) const;

			auto getReachableTiles(const MovementRange&) const -> std::set<HexCoordinate>;
			auto getPossibleTargetTiles(const MovementRange&) const -> std::set<HexCoordinate>;
			auto getReachableOpponentPieces(const MovementRange&) const -> std::set<const Piece*>;

		public:
			auto getColor() const -> PlayersColor
			{ return m_color; }

			auto getType() const -> PieceType
			{ return m_type; }

			auto getCoord() const -> optional<Coordinate>
			{ return m_coord; }

			Piece(PlayersColor color, PieceType type, const Coordinate& coord, Match& match)
				: m_color{color}
				, m_type{type}
				, m_coord{std::move(coord)}
				, m_match(match)
			{ }

			virtual ~Piece() = default;

			void setCoord(Coordinate coord)
			{ m_coord = coord; }

			auto getBaseTier() const -> uint8_t;
			auto getEffectiveDefenseTier() const -> uint8_t;
			auto getHomeTerrain() const -> optional<TerrainType>;
			auto getSetupTerrain() const -> optional<TerrainType>;
			auto getMovementScope() const -> const MovementScope&;

			bool canReach(const Coordinate&) const;

			auto getHexagonalLineTiles() const -> TileStateMap;
			auto getReachableTiles() const -> std::set<HexCoordinate>;
			auto getPossibleTargetTiles() const -> std::set<HexCoordinate>;
			auto getReachableOpponentPieces() const -> std::set<const Piece*>;

			virtual bool moveTo(const HexCoordinate&, bool setup);
			void promoteTo(PieceType);
	};

	// TODO
	typedef std::map<Coordinate, std::shared_ptr<Piece>> CoordPieceMap;
	typedef std::multimap<PieceType, std::shared_ptr<Piece>> TypePieceMap;

	// type from json::pieceMap
	void evalOpeningArray(const std::map<PieceType, std::set<Coordinate>>&);
}

#endif // _CYVASSE_PIECE_HPP_
