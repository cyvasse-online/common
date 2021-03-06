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

#ifndef _CYVASSE_PLAYER_HPP_
#define _CYVASSE_PLAYER_HPP_

#include <memory>

#include "fortress.hpp"
#include "players_color.hpp"
#include "piece.hpp"

namespace cyvasse
{
	class Match;

	class Player
	{
		protected:
			const PlayersColor m_color;
			const std::string m_id;

			bool m_setupDone = false;
			bool m_kingTaken = false;

			Match& m_match;

			TypePieceMap m_inactivePieces;
			std::unique_ptr<Fortress> m_fortress;

		public:
			Player(Match&, PlayersColor, std::unique_ptr<Fortress>, const std::string& id = {});
			virtual ~Player() = default;

			auto getColor() const -> PlayersColor
			{ return m_color; }

			auto getID() const -> const std::string&
			{ return m_id; }

			bool isSetupDone() const
			{ return m_setupDone; }

			void setupDone()
			{ m_setupDone = true; }

			bool isKingTaken() const
			{ return m_kingTaken; }

			void kingTaken(bool value)
			{ m_kingTaken = value; }

			auto getInactivePieces() -> TypePieceMap&
			{ return m_inactivePieces; }

			auto getFortress() -> Fortress&
			{ return *m_fortress; }

			bool canEndSetup() const;

			void onTurnEnd();

			void setFortress(std::unique_ptr<Fortress> fortress)
			{ m_fortress = std::move(fortress); }
	};
}

#endif // _CYVASSE_PLAYER_HPP_
