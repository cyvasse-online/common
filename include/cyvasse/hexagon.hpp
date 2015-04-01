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

#ifndef _CYVASSE_HEXAGON_HPP_
#define _CYVASSE_HEXAGON_HPP_

#include "hexcoordinate.hpp"

#include <set>

#include <cassert>
#include <cstdlib>

namespace cyvasse
{
	template <uint8_t l>
	class Hexagon
	{
		private:
			Hexagon() = delete;

		public:
			/// The edge length of the hexagon (template parameter)
			static constexpr uint8_t edgeLength = l;

			/// The amount of tiles that make up the hexagon
			static constexpr uint16_t tileCount = (l * (l - 1)) / 2 * 6 + 1;

			/// A container with all possible coordinates in this hexagon
			static const std::set<HexCoordinate<l>> allCoordinates;
	};

	template <uint8_t l>
	const std::set<HexCoordinate<l>> Hexagon<l>::allCoordinates = [] {
		std::set<HexCoordinate<l>> set;

		for (auto X = 0; X < (2 * l) - 1; X++)
		{
			auto yBegin = (X < (l - 1)) ? (l - 1 - X) : 0;
			auto yEnd   = (X < (l - 1)) ? (2 * l - 1) : (2 * l - 1) + (l - 1 - X);

			for (auto Y = yBegin; Y < yEnd; Y++)
			{
				auto res = set.emplace(X, Y);
				assert(res.second);
			}
		}

		return set;
	}();
}

#endif // _CYVASSE_HEXAGON_HPP_
