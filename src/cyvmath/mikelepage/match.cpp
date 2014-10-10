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

#include <cyvmath/mikelepage/match.hpp>

#include <cyvmath/mikelepage/fortress.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		std::set<Coordinate> Match::getHorseMovementCenters()
		{
			return {
				m_players[PlayersColor::WHITE]->getFortress().getCoord(),
				m_players[PlayersColor::BLACK]->getFortress().getCoord()
			};
		}

		std::shared_ptr<Piece> Match::getPieceAt(Coordinate coord)
		{
			std::shared_ptr<Piece> ret;

			auto it = m_activePieces.find(coord);
			if(it != m_activePieces.end())
				ret = it->second;

			return ret;
		}

		void Match::forReachableCoords(Coordinate start, const MovementRange& range,
		                               std::function<void(Coordinate, Piece*)> func)
		{
			for(const auto& step : range.first)
			{
				assert(step.size() == 2);

				auto tmpPos = start.toValarray<int_least8_t>();
				auto tmpCoord = make_unique<Coordinate>(start);

				for(auto i = 0; i < range.second; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = Coordinate::create(tmpPos);

					// if one step into this direction results in a
					// invalid coordinate, all further ones do too
					if(!tmpCoord)
						break;

					std::shared_ptr<Piece> tmpPiece = getPieceAt(*tmpCoord);

					func(*tmpCoord, tmpPiece.get());

					// if there is a piece on the tile,
					// we can't reach any tiles beyond it
					if(tmpPiece)
						break;
				}
			}
		}

		void Match::addToBoard(PieceType type, PlayersColor color, Coordinate coord)
		{
			assert(type != PieceType::UNDEFINED);
			assert(color != PlayersColor::UNDEFINED);

			auto& inactivePieces = m_players.at(color)->getInactivePieces();

			auto it = inactivePieces.find(type);
			assert(it != inactivePieces.end());

			std::shared_ptr<Piece> piece = it->second;
			inactivePieces.erase(it);

			piece->setCoord(coord);
			m_activePieces.emplace(coord, piece);
		}

		void Match::removeFromBoard(std::shared_ptr<Piece> piece)
		{
			assert(piece->getCoord());

			auto it = m_activePieces.find(*piece->getCoord());
			assert(it != m_activePieces.end());
			m_activePieces.erase(it);

			auto& player = m_players.at(piece->getColor());

			player->getInactivePieces().emplace(piece->getType(), piece);

			if(piece->getType() == PieceType::KING)
				player->kingTaken(true);
		}
	}
}
