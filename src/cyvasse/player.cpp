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

#include <cyvasse/player.hpp>

#include <cyvasse/hexagon.hpp>
#include <cyvasse/match.hpp>

using namespace std;

namespace cyvasse
{
	Player::Player(Match& match, PlayersColor color, unique_ptr<Fortress> fortress, const string& id)
		: m_color(color)
		, m_id(id)
		, m_match(match)
		, m_fortress(move(fortress))
	{ }

	bool Player::setupComplete() const
	{
		auto outsideOwnSide = (m_color == PlayersColor::WHITE)
			? [](int8_t y) { return y >= (Hexagon<6>::edgeLength - 1); }
			: [](int8_t y) { return y <= (Hexagon<6>::edgeLength - 1); };

		for(auto& it : m_match.getActivePieces())
		{
			if(it.second->getColor() == m_color && outsideOwnSide(it.first.y()))
				return false;
		}

		return true;
	}

	void Player::onTurnEnd()
	{
		if(m_kingTaken && !m_fortress->isRuined)
		{
			auto piece = m_match.getPieceAt(m_fortress->getCoord());
			if(piece && piece->getBaseTier() == 3)
				piece->promoteTo(PieceType::KING);
		}

		if(m_kingTaken)
			m_match.endGame(!m_color);

		auto& op = m_match.getPlayer(!m_color);
		if(op.kingTaken() && op.getFortress().isRuined)
			m_match.endGame(m_color);

		m_match.getBearingTable().update();
	}
}
