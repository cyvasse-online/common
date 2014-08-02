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
			auto outsideOwnSide = (_color == PlayersColor::WHITE)
				? [](int8_t y) { return y >= (Hexagon::edgeLength - 1); }
				: [](int8_t y) { return y <= (Hexagon::edgeLength - 1); };

			for(auto& it : _match.getActivePieces())
			{
				if(it.second->getColor() == _color && outsideOwnSide(it.first.y()))
					return false;
			}

			return true;
		}

		void Player::onTurnEnd()
		{
			if(_fortress)
			{
				auto piece = _match.getPieceAt(_fortress->getCoord());
				if(piece)
				{
					if(piece->getColor() != _color)
						removeFortress();
					else if(_kingTaken && piece->getBaseTier() == 3)
					{
						piece->promoteTo(PieceType::KING);
						_kingTaken = false;
					}
				}
			}

			if(_kingTaken)
				_match.endGame(!_color);

			// workaround for bearing table bug
			_match.getBearingTable().clear();
			_match.getBearingTable().init();
		}

		void Player::setFortress(std::shared_ptr<Fortress> fortress)
		{
			assert(fortress);

			_fortress = fortress;
		}
	}
}
