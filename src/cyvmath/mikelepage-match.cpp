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

#include <cyvmath/mikelepage/match.hpp>

#include <stdexcept>
#include <cyvmath/hexagon.hpp>
#include <cyvmath/mikelepage/fortress.hpp>

namespace cyvmath
{
	namespace mikelepage
	{
		auto Match::getHorseMovementCenters() -> std::set<HexCoordinate>
		{
			return {
				getPlayer(PlayersColor::WHITE).getFortress().getCoord(),
				getPlayer(PlayersColor::BLACK).getFortress().getCoord()
			};
		}

		auto Match::getPieceAt(HexCoordinate coord) -> std::shared_ptr<Piece>
		{
			std::shared_ptr<Piece> ret;

			auto it = m_activePieces.find(coord);
			if(it != m_activePieces.end())
				ret = it->second;

			return ret;
		}

		void Match::forReachableCoords(HexCoordinate start, const MovementRange& range,
		                               std::function<void(const HexCoordinate&, Piece*)> func)
		{
			for(const auto& step : range.first)
			{
				assert(step.size() == 2);

				auto tmpPos = start.toValarray<int8_t>();
				auto tmpCoord = make_unique<Coordinate>(start);

				for(auto i = 0; i < range.second; i++)
				{
					assert(tmpCoord);

					tmpPos += step;
					tmpCoord = HexCoordinate::create(tmpPos);

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

		void Match::addToBoard(PieceType type, PlayersColor color, const HexCoordinate& coord)
		{
			assert(type != PieceType::UNDEFINED);
			assert(color != PlayersColor::UNDEFINED);

			auto& inactivePieces = getPlayer(color).getInactivePieces();

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

			auto& player = getPlayer(piece->getColor());

			player.getInactivePieces().emplace(piece->getType(), piece);

			if(piece->getType() == PieceType::KING)
				player.kingTaken(true);
		}
	}
}
