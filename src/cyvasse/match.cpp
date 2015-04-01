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

#include <cyvasse/match.hpp>

#include <stdexcept>
#include <cyvasse/fortress.hpp>

using namespace std;

namespace cyvasse
{
	auto Match::getHorseMovementCenters() -> set<HexCoordinate<6>>
	{
		return {
			getPlayer(PlayersColor::WHITE).getFortress().getCoord(),
			getPlayer(PlayersColor::BLACK).getFortress().getCoord()
		};
	}

	auto Match::getPieceAt(HexCoordinate<6> coord) -> shared_ptr<Piece>
	{
		shared_ptr<Piece> ret;

		auto it = m_activePieces.find(coord);
		if(it != m_activePieces.end())
			ret = it->second;

		return ret;
	}

	void Match::forReachableCoords(HexCoordinate<6> start, const MovementRange& range, function<void(HexCoordinate<6>, Piece*)> func)
	{
		for(const auto& step : range.first)
		{
			assert(step.size() == 2);

			auto tmpPos = start.toValarray<int8_t>();
			optional<HexCoordinate<6>> tmpCoord = start;

			for(auto i = 0; i < range.second; i++)
			{
				assert(tmpCoord);

				tmpPos += step;
				tmpCoord = HexCoordinate<6>::create(tmpPos);

				// if one step into this direction results in a
				// invalid coordinate, all further ones do too
				if(!tmpCoord)
					break;

				shared_ptr<Piece> tmpPiece = getPieceAt(*tmpCoord);

				func(*tmpCoord, tmpPiece.get());

				// if there is a piece on the tile,
				// we can't reach any tiles beyond it
				if(tmpPiece)
					break;
			}
		}
	}

	void Match::addToBoard(PieceType type, PlayersColor color, HexCoordinate<6> coord)
	{
		auto& inactivePieces = getPlayer(color).getInactivePieces();

		auto it = inactivePieces.find(type);
		assert(it != inactivePieces.end());

		shared_ptr<Piece> piece = it->second;
		inactivePieces.erase(it);

		piece->setCoord(coord);
		m_activePieces.emplace(coord, piece);
	}

	void Match::removeFromBoard(shared_ptr<Piece> piece)
	{
		assert(piece->getCoord());

		auto it = m_activePieces.find(*piece->getCoord());
		assert(it != m_activePieces.end());
		m_activePieces.erase(it);

		auto& player = getPlayer(piece->getColor());

		player.getInactivePieces().emplace(piece->getType(), piece);

		if(piece->getType() == PieceType::KING)
			player.kingTaken(true);
	}
}
