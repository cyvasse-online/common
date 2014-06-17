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

#include <algorithm>
#include <array>
#include <functional>
#include <ostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cassert>
#include <cmath>

namespace cyvmath
{
	/* This should rather be a namespace, but namespaces cannot have template parameters.
	   hexagon<6>::Coordinate is way better understandable than hexagon::Coordinate<6>
	   so this is implemented as a template class with a deleted constructor.
	   The template parameter l stands for the edge length of the hexagon.
	 */
	template <int l>
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
				public:
					typedef std::vector<Coordinate> CoordinateVec;

				private:
					int8_t _x;
					int8_t _y;

					Coordinate(int8_t x, int8_t y)
						: _x(x)
						, _y(y)
					{ }

					DEEPCOPYABLE_DERIVED(Coordinate)

					static bool isValid(int8_t x, int8_t y)
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

					CoordinateVec getCoordinates(const std::array<std::pair<int8_t, int8_t>, 6>& steps, int8_t distance) const
					{
						CoordinateVec vec;

						for(int direction = 0; direction < 6; direction++)
						{
							std::pair<int, int> op = steps.at(direction);

							Coordinate tmpCoord(*this);
							for(int i = 0; i < distance; i++)
							{
								tmpCoord = {tmpCoord->_x + op.first, tmpCoord->_y + op.second};

								// if one step into this direction results in a
								// invalid coordinate, all further ones do too
								if(!tmpCoord)
									break;

								vec.push_back(tmpCoord);
							}
						}

						return vec;
					}

				public:
					virtual ~Coordinate() = default;

					int8_t x() const override
					{
						return _x;
					}

					int8_t y() const override
					{
						return _y;
					}

					int8_t z() const
					{
						return -(_x + _y);
					}

					int16_t dump() const
					{
						return (_x << 8) | _y;
					}

					/** Get the distance to another coordinate in form of the amount
						of single moves to adjacent tiles required to move there
					 */
					int8_t getDistance(Coordinate other) const
					{
						// Concept from http://keekerdc.com/2011/03/hexagon-grids-coordinate-systems-and-distance-calculations/
						// I have no idea why the maximum of x-, y- and z-difference
						// of the coordinates equals the distance between them...
						return std::max(abs(_x - other._x), std::max(abs(_y - other._y), abs(z() - other.z())));
					}

					/// Check if the given coordinate is reachable in one orthogonal move
					bool isOrthogonal(Coordinate other) const
					{
						return _x == other._x || _y == other._y || z() == other.z();
					}

					/** Like getDistance(), but return -1 if the given coordinate
						is not reachable in one orthogonal move

						An orthogonal move is a straight move in one of the six
						directions of the neighboring hex-tiles.
					 */
					int8_t getDistanceOrthogonal(Coordinate other) const
					{
						if(!isOrthogonal(other))
							return -1;

						return getDistance(other);
					}

					/** Get all coordinates reachable in one orthogonal move of
						maximally the provided distance of tiles

						The default value is the maximal distance possible on
						this hexagon (no distance limit).
					 */
					CoordinateVec getCoordinatesOrthogonal(int8_t distance = (l * 2 - 2)) const
					{
						static const std::array<std::pair<int8_t, int8_t>, 6> steps = {
								std::make_pair(-1,  1), // top left
								std::make_pair( 0,  1), // top right
								std::make_pair( 1,  0), // right
								std::make_pair( 1, -1), // bottom right
								std::make_pair( 0, -1), // bottom left
								std::make_pair(-1,  0)  // left
							};

						return getCoordinates(steps, distance);
					}

					/// Check if the given coordinate is reachable in one diagonal move
					bool isDiagonal(Coordinate other) const
					{
						int8_t dX = _x - other._x;
						int8_t dY = _y - other._y;
						int8_t dZ = z() - other.z();

						return dX == dY ||
							   dX == dZ ||
							   dY == dZ;
					}

					/** Get the diagonal distance to a coordinate, or -1 if
						the coordinate isn't reachable in one diagonal move

						A diagonal move is a straight move in one of the six
						directions of the lines between the neighboring hex-tiles.
					 */
					int8_t getDistanceDiagonal(Coordinate other) const
					{
						if(!isDiagonal(other))
							return -1;

						return getDistance(other) / 2;
					}

					/** Get all coordinates reachable in one diagonal move of
						maximally the provided distance of tiles

						The default value is the maximal distance possible on
						this hexagon (no distance limit).
					 */
					CoordinateVec getCoordinatesDiagonal(int8_t distance = (l * 1)) const
					{
						static const std::array<std::pair<int8_t, int8_t>, 6> steps = {
								std::make_pair(-1,  2), // top
								std::make_pair( 1,  1), // top right
								std::make_pair( 2, -1), // bottom right
								std::make_pair( 1, -2), // bottom
								std::make_pair(-1, -1), // bottom left
								std::make_pair(-2,  1)  // top left
							};

						return getCoordinates(steps, distance);
					}

					/** Get the distance to another coordinate with the same distance to
						the center Coordinate along the line of these coordinates, or -1
						if the coordinate isn't reachable while only moving on that line.

						All possible movement targets here are along a hexagonal line.
						The direct distance from this coordinate to another one on that
						line may be different than the distance along the line.
					 */
					// There certainly is no other rule set than MikeL's that uses this
					// but there probably is no better place to put this code either.
					int8_t getDistanceHexagonalLine(Coordinate other, Coordinate center) const
					{
						// Check if the given coordinate has the same distance to center as this
						if(getDistance(center) != other.getDistance(center))
							return -1;

						// TODO
						return 0;
					}

					bool set(int8_t x, int8_t y)
					{
						if(isValid(x, y))
						{
							_x = x;
							_y = y;

							return true;
						}

						return false;
					}

					/// @{
					/// Create a Coordinate object from an x and an y
					/// If the coordinte is invalid, return nullptr
					static std::unique_ptr<Coordinate> create(int8_t x, int8_t y)
					{
						if(isValid(x, y))
							return std::unique_ptr<Coordinate>(new Coordinate(x, y));

						return nullptr;
					}

					static std::unique_ptr<Coordinate> create(std::pair<int8_t, int8_t> c)
					{
						if(isValid(c.first, c.second))
							return std::unique_ptr<Coordinate>(new Coordinate(c.first, c.second));

						return nullptr;
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
						return {str.at(0) - 'A', std::stoi(str.substr(1)) - 1};
					}

					// for debugging
					friend std::ostream& operator<<(std::ostream& os, Coordinate c)
					{
						os << "(" << (int) c._x << ", " << (int) c._y << ", " << (int) c.z() << ")";
						return os;
					}
			};

			typedef typename Coordinate::CoordinateVec CoordinateVec;

			/// The edge length of the hexagon (template parameter)
			static const int edgeLength = l;

			/// The amount of tiles that make up the hexagon
			static const int tileCount = (edgeLength * (edgeLength - 1)) / 2 * 6 + 1;

			/// Get a container with all possible coordinates in this hexagon
			static CoordinateVec getAllCoordinates()
			{
				static CoordinateVec vec;

				if(vec.empty())
				{
					vec.reserve(tileCount);

					for(int x = 0; x < (2 * l) - 1; x++)
					{
						int yBegin = (x < (l - 1)) ? (l - 1 - x) : 0;
						int yEnd   = (x < (l - 1)) ? (2 * l - 1) : (2 * l - 1) + (l - 1 - x);

						for(int y = yBegin; y < yEnd; y++)
						{
							std::unique_ptr<Coordinate> c = Coordinate::create(x, y);
							assert(c);

							vec.push_back(*c);
						}
					}
				}

				return vec;
			}
	};
}

#endif // _CYVMATH_HEXAGON_HPP_
