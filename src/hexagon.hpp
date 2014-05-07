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

#ifndef _CYVMATH_HEXAGON_HPP_
#define _CYVMATH_HEXAGON_HPP_

namespace cyvmath
{
	namespace hexagon
	{
		/// A coordinate on the hexboard (see mockup/hexboard-coordinates-internal.svg)
		class Coordinate
		{
			private:
				uint8_t _x;
				uint8_t _y;

			public:
				Coordinate(uint8_t x, uint8_t y)
					: _x(x)
					, _y(y)
				{
					if(!isValid())
						throw;
				}

				bool isValid()
				{
				}

				operator bool()
				{
					return isValid();
				}

				/// Check if the given coordinate is a direct neighbor
				bool isAdjacent()
				{
				}

				/// Get the distance to another coordinate in form
				/// of the amount of steps required to move there
				uint8_t getDistance(Coordinate other)
				{
				}

				void set(uint8_t x, uint8_t y)
				{
					_x = x;
					_y = y;

					if(!isValid())
						throw;
				}

				void set(Coordinate other)
				{
					set(other._x, other._y);
				}

				Coordinate& operator=(Coordinate other)
				{
					set(other._x, other._y);
					return *this;
				}

				/// Create a Coordinate object from a coordinate in the public notation
				/// (see mockup/hexboard-coordinates-public.svg)
				static Coordinate fromStr(const std::string& str)
				{
					if(str.length() != 2)
						throw;

					// When using char's for calculations, their ASCII values are used,
					// and as the uppercase letters are consecutive in the ASCII table,
					// this will result in 0 for 'A', 1 for 'B' and so on.
					// We don't accept lowercase letters.
					_x = str.at(0) - 'A';

					// Public coordinate notation starts with 1, we start with 0
					_y = str.at(1) - 1;
				}
		};
	}
}

#endif // _CYVMATH_HEXAGON_HPP_
