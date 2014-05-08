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
	// This should rather be a namespace, but namespaces cannot have template parameters
	// but hexagon<6>::Coordinate is way better understandable than hexagon::Coordinate<6>
	// so this is implemented as a template class with a deleted constructor.
	// The template parameter l stands for the edge length of the hexagon.
	template <int l>
	class hexagon
	{
		// The smallest possible hexagon has 7 tiles and an edge lenth of 2
		// The largest possible hexagon in which the highest x and y can be represented
		// using a signed 8 bit integer has an edge length of 64 (=> width and height 127)
		static_assert(l >= 2,  "The minimum size of the hexagon edge length is 2.");
		static_assert(l <= 64, "The maximum size of the hexagon edge length is 64.");

		private:
			hexagon() = delete;

		public:
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
						// This function will currently fail on hexagons with l > 13 because there
						// would be multiple letters for the y coordinate in the public notation.
						// TODO: Decide on whether to set a maximum l of 13 or to alter this function

						if(str.length() < 2)
							throw std::invalid_argument("Coordinate not valid");

						// x: When using char's for calculations, their ASCII values are used,
						//    and as the uppercase letters are consecutive in the ASCII table,
						//    this will result in 0 for 'A', 1 for 'B' and so on.
						//    We don't accept lowercase letters.
						// y: Public coordinate notation starts with 1, we start with 0
						return {str.at(0) - 'A', std::stoi(str.substr(1)) - 1};
					}
			};
	};
}

#endif // _CYVMATH_HEXAGON_HPP_
