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

#include <cyvmath/mikelepage/player.hpp>

#include <cassert>
#include <cyvmath/mikelepage/fortress.hpp>
#include <cyvmath/mikelepage/match.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		bool Player::setupComplete()
		{
			auto outsideOwnSide = (m_color == PlayersColor::WHITE)
				? [](int8_t y) { return y >= (Hexagon::edgeLength - 1); }
				: [](int8_t y) { return y <= (Hexagon::edgeLength - 1); };

			for(auto& it : m_match.getActivePieces())
			{
				if(it.second->getColor() == m_color && outsideOwnSide(it.first.y()))
					return false;
			}

			return true;
		}

		void Player::onTurnEnd()
		{
			if(m_fortress)
			{
				auto piece = m_match.getPieceAt(m_fortress->getCoord());
				if(piece)
				{
					if(piece->getColor() != m_color)
						removeFortress();
					else if(m_kingTaken && piece->getBaseTier() == 3)
					{
						piece->promoteTo(PieceType::KING);
						m_kingTaken = false;
					}
				}
			}

			if(m_kingTaken)
				m_match.endGame(!m_color);

			m_match.getBearingTable().update();
		}

		void Player::setFortress(std::shared_ptr<Fortress> fortress)
		{
			assert(fortress);

			m_fortress = fortress;
		}
	}
}
