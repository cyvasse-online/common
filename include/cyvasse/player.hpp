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

			bool m_kingTaken = false;

			Match& m_match;

			TypePieceMap m_inactivePieces;

			std::unique_ptr<Fortress> m_fortress;

		public:
			Player(Match&, PlayersColor, std::unique_ptr<Fortress>, const std::string& id = {});

			PlayersColor getColor() const
			{ return m_color; }

			const std::string& getID() const
			{ return m_id; }

			bool kingTaken() const
			{ return m_kingTaken; }

			void kingTaken(bool value)
			{ m_kingTaken = value; }

			TypePieceMap& getInactivePieces()
			{ return m_inactivePieces; }

			Fortress& getFortress()
			{ return *m_fortress; }

			virtual bool setupComplete() const;

			void onTurnEnd();

			void setFortress(std::unique_ptr<Fortress> fortress)
			{ m_fortress = std::move(fortress); }
	};
}

#endif // _CYVASSE_PLAYER_HPP_
