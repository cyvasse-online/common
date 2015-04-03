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

#ifndef _CYVASSE_BEARING_TABLE_HPP_
#define _CYVASSE_BEARING_TABLE_HPP_

#include <functional>
#include <map>
#include <vector>

#include "piece.hpp"

template <typename T>
struct addr_less
{
	constexpr bool operator()(const std::reference_wrapper<T> lhs, const std::reference_wrapper<T> rhs) const
	{
		return std::addressof(lhs.get()) < std::addressof(rhs.get());
	}
};

namespace cyvasse
{
	class BearingTable
	{
		private:
			typedef std::map<std::reference_wrapper<const Piece>, std::vector<std::reference_wrapper<const Piece>>, addr_less<const Piece>>
				BearingMap;

			CoordPieceMap& m_pieceMap;

			BearingMap m_canBeReachedBy;

			void addCanReach(std::reference_wrapper<const Piece>);
			void addCanBeReachedBy(std::reference_wrapper<const Piece>);

		public:
			BearingTable(CoordPieceMap& pieceMap)
				: m_pieceMap(pieceMap)
			{ }

			// non-copyable
			BearingTable(const BearingTable&) = delete;
			BearingTable& operator=(const BearingTable&) = delete;

			bool canTake(const Piece& attackingPiece, const Piece& defendingPiece) const;

			void init();
			void clear();
			void update();
	};
}

#endif // _CYVASSE_BEARING_TABLE_HPP_
