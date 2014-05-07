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

#include <string>
#include <stdexcept>

namespace cyvmath
{
	namespace hexagon
	{
		/// A coordinate on the hexboard (see mockup/hexboard-coordinates-internal.svg)
		class Coordinate
		{
			private:
				int8_t _x;
				int8_t _y;

			public:
				Coordinate(int8_t x, int8_t y)
					: _x(x)
					, _y(y)
				{
					if(!isValid())
						throw std::invalid_argument("Coordinate not valid");
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

				/** Get the distance to another coordinate in form of the amount
				    of single moves to adjacent tiles required to move there
				 */
				int8_t getDistance(Coordinate other)
				{
				}

				/** Like getDistance(), but return -1 if the given coordinate
				    is not reachable in one orthogonal move

				    An orthogonal move is a straight move in one of the six
				    directions of the neighboring hex-tiles.
				 */
				int8_t getDistanceOrthogonal(Coordinate other)
				{
				}

				/** Get the diagonal distance to a coordinate, or -1 if
				    the coordinate isn't reahcable in one diagonal move

				    A diagonal move is a straight move in one of the six
				    directions of the lines between the neighboring hex-tiles.
				 */
				int8_t getDistanceDiagonal(Coordinate other)
				{
				}

				/** Get the distance to another coordinate with the same distance
				    to the center Coordinate along the line of these coordinates.

				    All possible movement targets here are along a hexagonal line.
				    The direct distance from this coordinate to another one on that
				    line may be different than the distance along the line.
				 */
				// There certainly is no other ruleset than MikeL's that uses this
				// but there probably is no better place to put this code either.
				int8_t getDistanceHexagonalLine(Coordinate other, Coordinate center)
				{
				}

				void set(int8_t x, int8_t y)
				{
					_x = x;
					_y = y;

					if(!isValid())
						throw std::invalid_argument("Coordinate not valid");
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
						throw std::invalid_argument("Coordinate not valid");

					// x: When using char's for calculations, their ASCII values are used,
					//    and as the uppercase letters are consecutive in the ASCII table,
					//    this will result in 0 for 'A', 1 for 'B' and so on.
					//    We don't accept lowercase letters.
					// y: Public coordinate notation starts with 1, we start with 0
					return {str.at(0) - 'A', str.at(1) - 1};
				}
		};
	}
}

#endif // _CYVMATH_HEXAGON_HPP_
