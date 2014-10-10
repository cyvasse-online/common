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

#ifndef _CYVMATH_MIKELEPAGE_PLAYER_HPP_
#define _CYVMATH_MIKELEPAGE_PLAYER_HPP_

#include <array>
#include <memory>
#include <cyvmath/player.hpp>

#include "fortress.hpp"
#include "piece.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match;

		class Player : public cyvmath::Player
		{
			protected:
				bool m_kingTaken;

				Match& m_match;

				TypePieceMap m_inactivePieces;

				std::unique_ptr<Fortress> m_fortress;

			public:
				Player(PlayersColor, Match&, std::unique_ptr<Fortress>);

				virtual ~Player() = default;

				bool kingTaken()
				{ return m_kingTaken; }

				void kingTaken(bool value)
				{ m_kingTaken = value; }

				TypePieceMap& getInactivePieces()
				{ return m_inactivePieces; }

				Fortress& getFortress()
				{ return *m_fortress; }

				virtual bool setupComplete() override;

				void onTurnEnd();
		};

		typedef std::array<std::shared_ptr<Player>, 2> PlayerArray;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PLAYER_HPP_
