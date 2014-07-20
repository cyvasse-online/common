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

#include "coordinate.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <ostream>
#include <set>
#include <string>
#include <stdexcept>
#include <valarray>
#include <cassert>
#include <cmath>
#include <make_unique.hpp>

namespace cyvmath
{
	/* This should rather be a namespace, but namespaces cannot have template parameters.
	   hexagon<6>::Coordinate is way better understandable than hexagon::Coordinate<6>
	   so this is implemented as a template class with a deleted constructor.
	   The template parameter l stands for the edge length of the hexagon.
	 */
	template <uint_least8_t l>
	class Hexagon
	{
		/* The smallest possible hexagon has 7 tiles and an edge lenth of 2
		   The largest possible hexagon in which the highest x and y can be represented
		   using a signed 8 bit integer has an edge length of 64 (=> width and height 127)
		 */
		static_assert(l >= 2,  "The minimum size of the hexagon edge length is 2.");
		static_assert(l <= 64, "The maximum size of the hexagon edge length is 64.");

		private:
			Hexagon() = delete;

		public:
			/// A coordinate on the hexboard (see mockup/hexboard-coordinates-internal.svg)
			class Coordinate : public cyvmath::Coordinate
			{
				private:
					int_least8_t _x;
					int_least8_t _y;

					constexpr Coordinate(int_least8_t x, int_least8_t y)
						: _x(x)
						, _y(y)
					{ }

					static constexpr bool isValid(int_least8_t x, int_least8_t y)
					{
						return x >= 0 && x < (l * 2 - 1) &&
							   y >= 0 && y < (l * 2 - 1) &&
							   (x + y) >= (l - 1) &&
							   (x + y) <= (l - 1) * 3;
					}

					bool isValid() const
					{
						return isValid(_x, _y);
					}

				public:
					virtual ~Coordinate() = default;

					int_least8_t x() const final override
					{
						return _x;
					}

					int_least8_t y() const final override
					{
						return _y;
					}

					constexpr int_least8_t z() const
					{
						return -(_x + _y);
					}

					int_least16_t dump() const final override
					{
						return (_x << 8) | _y;
					}

					/** Get the distance to another coordinate in form of the amount
						of single moves to adjacent tiles required to move there
					 */
					constexpr int_least8_t getDistance(Coordinate other) const
					{
						// Concept from http://keekerdc.com/2011/03/hexagon-grids-coordinate-systems-and-distance-calculations/
						// I have no idea why the maximum of x-, y- and z-difference
						// of the coordinates equals the distance between them...
						return std::max(abs(_x - other._x), std::max(abs(_y - other._y), abs(z() - other.z())));
					}

					/// Check if the given coordinate is reachable in one orthogonal move
					constexpr bool isOrthogonal(Coordinate other) const
					{
						return _x == other._x || _y == other._y || z() == other.z();
					}

					/** Like getDistance(), but return -1 if the given coordinate
						is not reachable in one orthogonal move

						An orthogonal move is a straight move in one of the six
						directions of the neighboring hex-tiles.
					 */
					constexpr int_least8_t getDistanceOrthogonal(Coordinate other) const
					{
						return isOrthogonal(other) ? getDistance(other) : -1;
					}

					/// Check if the given coordinate is reachable in one diagonal move
					constexpr bool isDiagonal(Coordinate other) const
					{
						// until C++14, this has to be implemented using
						// macros because C++11 doesn't allow constexpr
						// functions to include variable declarations
						#define dX _x - other._x
						#define dY _y - other._y
						#define dZ z() - other.z()

						return dX == dY ||
							   dX == dZ ||
							   dY == dZ;

						#undef dX
						#undef dY
						#undef dZ
					}

					/** Get the diagonal distance to a coordinate, or -1 if
						the coordinate isn't reachable in one diagonal move

						A diagonal move is a straight move in one of the six
						directions of the lines between the neighboring hex-tiles.
					 */
					constexpr int_least8_t getDistanceDiagonal(Coordinate other) const
					{
						return isDiagonal(other) ? getDistance(other) / 2 : -1;
					}

					/** Get the distance to another coordinate with the same distance to
						the center Coordinate along the line of these coordinates, or -1
						if the coordinate isn't reachable while only moving on that line.

						All possible movement targets here are along a hexagonal line.
						The direct distance from this coordinate to another one on that
						line may be different than the distance along the line.
					 */
					int_least8_t getDistanceHexagonalLine(Coordinate other, Coordinate center) const
					{
						auto distance = getDistance(center);

						// Check if the given coordinate has the same distance to center as this
						if(distance != other.getDistance(center))
							return -1;

						// TODO
						/*const MovementVec steps = {
								{-distance,  distance}, // top left
								{        0,  distance}, // top right
								{ distance,         0}, // right
								{ distance, -distance}, // bottom right
								{        0, -distance}, // bottom left
								{-distance,         0}  // left
							};

						auto cornerCoords = getCoordinates(steps, 1);*/

						return 0;
					}

					bool set(int_least8_t x, int_least8_t y)
					{
						if(isValid(x, y))
						{
							_x = x;
							_y = y;

							return true;
						}

						return false;
					}

					std::string toString()
					{
						std::string ret;
						ret.append(1, char(x()) + 'A');
						ret.append(std::to_string(int(y())));

						return ret;
					}

					/// @{
					/// Create a Coordinate object from an x and an y
					/// If the coordinte is invalid, return nullptr
					static constexpr std::unique_ptr<Coordinate> create(int_least8_t x, int_least8_t y)
					{
						// can't use make_unique because private constructor has to be called directly
						return isValid(x, y)
							? std::unique_ptr<Coordinate>(new Coordinate(x, y))
							: nullptr;
					}

					static std::unique_ptr<Coordinate> create(std::valarray<int_least8_t> a)
					{
						assert(a.size() == 2);
						return create(a[0], a[1]);
					}
					/// @}

					/// Create a Coordinate object from a coordinate in the public notation
					/// (see mockup/hexboard-coordinates-public.svg)
					static std::unique_ptr<Coordinate> createFromStr(const std::string& str)
					{
						// This function will currently fail on hexagons with l > 13 because there
						// would be multiple letters for the y coordinate in the public notation.
						// TODO: Decide on whether to set a maximum l of 13 or to alter this function

						if(str.length() < 2)
							return nullptr;

						/* x: When using char's for calculations, their ASCII values are used,
							  and as the uppercase letters are consecutive in the ASCII table,
							  this will result in 0 for 'A', 1 for 'B' and so on.
							  We don't accept lowercase letters.
						   y: Public coordinate notation starts with 1, we start with 0
						 */
						return std::unique_ptr<Coordinate>(
							new Coordinate(str.at(0) - 'A', std::stoi(str.substr(1)) - 1)
						);
					}

					// for debugging
					friend std::ostream& operator<<(std::ostream& os, Coordinate c)
					{
						os << "(" << (int) c._x << ", " << (int) c._y << ", " << (int) c.z() << ")";
						return os;
					}
			};

			typedef std::set<Coordinate> CoordinateSet;

			/// The edge length of the hexagon (template parameter)
			static constexpr uint_least8_t edgeLength = l;

			/// The amount of tiles that make up the hexagon
			static constexpr uint_least16_t tileCount = (l * (l - 1)) / 2 * 6 + 1;

			/// Get a container with all possible coordinates in this hexagon
			static const CoordinateSet& getAllCoordinates()
			{
				static CoordinateSet set;

				if(set.empty())
				{
					for(auto x = 0; x < (2 * l) - 1; x++)
					{
						auto yBegin = (x < (l - 1)) ? (l - 1 - x) : 0;
						auto yEnd   = (x < (l - 1)) ? (2 * l - 1) : (2 * l - 1) + (l - 1 - x);

						for(auto y = yBegin; y < yEnd; y++)
						{
							std::unique_ptr<Coordinate> c = Coordinate::create(x, y);
							assert(c);

							auto res = set.insert(*c);
							assert(res.second);
						}
					}
				}

				return set;
			}
	};
}

#endif // _CYVMATH_HEXAGON_HPP_
