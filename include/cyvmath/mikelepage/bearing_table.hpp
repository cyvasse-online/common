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

#ifndef _CYVMATH_MIKELEPAGE_BEARING_TABLE_HPP_
#define _CYVMATH_MIKELEPAGE_BEARING_TABLE_HPP_

#include <map>
#include <set>
#include "piece.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class BearingTable
		{
			private:
				typedef std::map<const Piece*, std::set<const Piece*>> BearingMap;

				CoordPieceMap& m_pieceMap;

				BearingMap m_canBeReachedBy;

				void addCanReach(const Piece*);
				void addCanBeReachedBy(const Piece*);

			public:
				BearingTable(CoordPieceMap& pieceMap)
					: m_pieceMap(pieceMap)
				{ }

				// non-copyable
				BearingTable(const BearingTable&) = delete;
				BearingTable& operator=(const BearingTable&) = delete;

				bool canTake(const Piece* attackingPiece, const Piece* defendingPiece) const;

				void init();
				void clear()
				{ m_canBeReachedBy.clear(); }

				void update()
				{
					clear();
					init();
				}
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_BEARING_TABLE_HPP_
