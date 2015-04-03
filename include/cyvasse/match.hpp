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

#ifndef _CYVASSE_MATCH_HPP_
#define _CYVASSE_MATCH_HPP_

#include <array>
#include <functional>
#include <memory>
#include <set>

#include <optional.hpp>

#include "bearing_table.hpp"
#include "hexcoordinate.hpp"
#include "piece.hpp"
#include "player.hpp"
#include "terrain.hpp"

namespace cyvasse
{
	class Match
	{
		public:
			typedef std::array<std::unique_ptr<Player>, 2> playerArray;

		protected:
			const std::string m_id;
			const bool m_random;
			const bool m_public;

			playerArray m_players;

			PlayersColor m_activePlayer = PlayersColor::WHITE;
			bool m_setup = true;

			CoordPieceMap m_activePieces;
			TerrainMap m_terrain;

			BearingTable m_bearingTable;

		public:
			Match(const std::string& id = {}, bool random = false, bool _public = false, playerArray players = playerArray())
				: m_id{id}
				, m_random{random}
				, m_public{_public}
				, m_players(std::move(players))
				, m_bearingTable(m_activePieces)
			{ }

			virtual ~Match() = default;

			auto getID() const -> const std::string&
			{ return m_id; }

			bool isRandom() const
			{ return m_random; }

			bool isPublic() const
			{ return m_public; }

			bool hasPlayer(PlayersColor color) const
			{ return (bool)m_players.at(color); }

			auto getPlayer(PlayersColor color) const -> Player&
			{ return *m_players.at(color); }

			void setPlayer(PlayersColor color, std::unique_ptr<Player> player)
			{ m_players[color] = std::move(player); }

			auto getActivePlayer() const -> PlayersColor
			{ return m_activePlayer; }

			bool inSetup() const
			{ return m_setup; }

			void setupDone()
			{ m_setup = false; }

			auto getActivePieces() -> CoordPieceMap&
			{ return m_activePieces; }

			auto getTerrain() -> TerrainMap&
			{ return m_terrain; }

			auto getBearingTable() -> BearingTable&
			{ return m_bearingTable; }

			auto getHorseMovementCenters() -> std::set<HexCoordinate<6>>;

			auto getPieceAt(HexCoordinate<6>) -> optional<std::reference_wrapper<Piece>>;

			void forReachableCoords(HexCoordinate<6> start, const MovementRange&, std::function<void(HexCoordinate<6>)>);

			virtual void addToBoard(PieceType, PlayersColor, HexCoordinate<6>);
			virtual void removeFromBoard(const Piece&);
			virtual void endGame(PlayersColor /* winner */) { }
	};
}

#endif // _CYVASSE_MATCH_HPP_
