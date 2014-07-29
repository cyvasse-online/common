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
		std::set<Coordinate> Match::getHexagonMovementCenters()
		{
			std::set<Coordinate> ret;

			for(auto player : _players)
			{
				auto fortress = player->getFortress();

				if(fortress)
					ret.insert(fortress->getCoord());
			}

			std::set<Coordinate>& replacementCenters = _fortressReplaceCorners;
			assert(ret.size() + replacementCenters.size() == 2);
			ret.insert(replacementCenters.begin(), replacementCenters.end());

			return ret;
		}

		void Match::removeFromBoard(std::shared_ptr<Piece> piece)
		{
			assert(piece->getCoord());

			auto it = _activePieces.find(*piece->getCoord());
			assert(it != _activePieces.end());
			_activePieces.erase(it);

			_players.at(piece->getColor())->getInactivePieces().push_back(piece);
		}
	}
}
