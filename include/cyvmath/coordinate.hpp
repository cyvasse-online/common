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

#ifndef _CYVMATH_COORDINATE_HPP_
#define _CYVMATH_COORDINATE_HPP_

#include <cstdint>

namespace cyvmath
{
	class Coordinate
	{
		protected:
			virtual bool isValid() const = 0;

			explicit operator bool() const
			{
				return isValid();
			}

		public:
			virtual int_least16_t dump() const = 0;

			virtual int_least8_t x() const = 0;
			virtual int_least8_t y() const = 0;

			bool operator==(const Coordinate& other) const
			{
				return dump() == other.dump();
			}

			bool operator!=(const Coordinate& other) const
			{
				return dump() != other.dump();
			}

			bool operator<(const Coordinate& other) const
			{
				return dump() < other.dump();
			}
	};
}

#endif // _CYVMATH_COORDINATE_HPP_
