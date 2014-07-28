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

#include <cyvmath/mikelepage/bearing_table.hpp>

namespace cyvmath
{
    namespace mikelepage
    {
        bool BearingTable::canTake(Piece* attackingPiece, Piece* defendingPiece) const
        {

        }

        void BearingTable::init()
        {

        }

        void BearingTable::add(Piece* piece)
        {

        }

        void BearingTable::update(Piece* piece)
        {
            auto it1 = _canReach.find(piece);
            auto it2 = _canBeReachedBy.find(piece);

            assert(it1 != _canReach.end());
            assert(it2 != _canBeReachedBy.end());

            for(auto it1Piece : it1->second)
            {
                auto it = _canBeReachedBy.find(it1Piece);
                assert(it);

                auto pieceRefIt = it->second.find(piece);
                assert(pieceRefIt);
                it->second.erase(pieceRefIt);
            }

            for(auto it2Piece : it2->second)
            {
                auto it = _canReach.find(it2Piece);
                assert(it);

                auto pieceRefIt = it->second.find(piece);
                assert(pieceRefIt);
                it->second.erase(pieceRefIt);
            }

            _canReach.erase(it1);
            _canBeReachedBy.erase(it2);

            add(piece);
        }

        void BearingTable::clear()
        {
            _canReach.clear();
            _canBeReachedBy.clear();
        }
    }
}
