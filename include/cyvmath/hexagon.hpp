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
#include <set>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <valarray>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <make_unique.hpp>

// not sure where to put this
template<uint8_t v>
using uint8 = std::integral_constant<uint8_t, v>;

namespace cyvmath
{
	enum class DirectionOrthogonal
	{
		UNDEFINED,
		TOP_LEFT,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
		LEFT
	};

	enum class DirectionDiagonal
	{
		UNDEFINED,
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		BOTTOM_LEFT
	};

	/* This should rather be a namespace, but namespaces cannot have template parameters.
	   hexagon<6>::Coordinate is way better understandable than hexagon::Coordinate<6>
	   so this is implemented as a template class with a deleted constructor.
	   The template parameter l stands for the edge length of the hexagon.
	 */
	template <uint8_t l>
	class Hexagon
	{
		/* The smallest possible hexagon has 7 tiles and an edge lenth of 2
		   The largest possible hexagon in which the highest X and Y can be represented
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
					Coordinate(short X, short Y)
						: cyvmath::Coordinate(X, Y)
					{ }

					static constexpr bool isValid(int8_t X, int8_t Y)
					{
						return X >= 0 && X < (l * 2 - 1) &&
							   Y >= 0 && Y < (l * 2 - 1) &&
							   (X + Y) >= (l - 1) &&
							   (X + Y) <= (l - 1) * 3;
					}

					bool isValid() const
					{ return isValid(m_x, m_y); }

				public:
					template<uint8_t X, uint8_t Y>
					constexpr Coordinate(uint8<X>, uint8<Y>)
						: cyvmath::Coordinate(X, Y)
					{
						static_assert(isValid(X, Y), "The given arguments are invalid.");
					}

					virtual ~Coordinate() = default;

					Coordinate(const Coordinate&) = default;
					Coordinate(Coordinate&&) = default;

					Coordinate& operator=(const Coordinate&) = default;
					Coordinate& operator=(Coordinate&&) = default;

					Coordinate(const cyvmath::Coordinate& other)
						: cyvmath::Coordinate(other.x(), other.y())
					{ }

					constexpr int8_t z() const
					{ return -(m_x + m_y); }

					/** Get the distance to another coordinate in form of the amount
						of single moves to adjacent tiles required to move there
					 */
					constexpr int8_t getDistance(Coordinate other) const
					{
						// Concept from http://keekerdc.com/2011/03/hexagon-grids-coordinate-systems-and-distance-calculations/
						// I have no idea why the maximum of x-, y- and z-difference
						// of the coordinates equals the distance between them...
						return std::max(abs(m_x - other.m_x), std::max(abs(m_y - other.m_y), abs(z() - other.z())));
					}

					/// Check if the given coordinate is reachable in one orthogonal move
					constexpr bool isOrthogonal(Coordinate other) const
					{ return m_x == other.m_x || m_y == other.m_y || z() == other.z(); }

					DirectionOrthogonal getDirectionOrthogonal(Coordinate other) const
					{
						if(*this == other) return DirectionOrthogonal::UNDEFINED;

						if(m_x == other.m_x)
							return (m_y > other.m_y) ? DirectionOrthogonal::BOTTOM_LEFT
							                       : DirectionOrthogonal::TOP_RIGHT;
						if(m_y == other.m_y)
							return (m_x > other.m_x) ? DirectionOrthogonal::LEFT
							                       : DirectionOrthogonal::RIGHT;
						if(z() == other.z())
							return (m_x > other.m_x) ? DirectionOrthogonal::TOP_LEFT
							                       : DirectionOrthogonal::BOTTOM_RIGHT;

						return DirectionOrthogonal::UNDEFINED;
					}

					/** Like getDistance(), but return -1 if the given coordinate
						is not reachable in one orthogonal move

						An orthogonal move is a straight move in one of the six
						directions of the neighboring hex-tiles.
					 */
					constexpr int8_t getDistanceOrthogonal(Coordinate other) const
					{ return isOrthogonal(other) ? getDistance(other) : -1; }

					/// Check if the given coordinate is reachable in one diagonal move
					constexpr bool isDiagonal(Coordinate other) const
					{
						// until C++14, this has to be implemented using
						// macros because C++11 doesn't allow constexpr
						// functions to include variable declarations
						#define dX m_x - other.m_x
						#define dY m_y - other.m_y
						#define dZ z() - other.z()

						return dX == dY ||
							   dY == dZ ||
							   dZ == dX;

						#undef dX
						#undef dY
						#undef dZ
					}

					DirectionDiagonal getDirectionDiagonal(Coordinate other) const
					{
						if(*this == other)
							return DirectionDiagonal::UNDEFINED;

						int8_t dX = m_x  - other.m_x;
						int8_t dY = m_y  - other.m_y;
						int8_t dZ = z() - other.z();

						if(dX == dY)
							return (dX > 0) ? DirectionDiagonal::BOTTOM_LEFT
							                : DirectionDiagonal::TOP_RIGHT;
						if(dY == dZ)
							return (dY > 0) ? DirectionDiagonal::BOTTOM_RIGHT
							                : DirectionDiagonal::TOP_LEFT;
						if(dZ == dX)
							return (dZ > 0) ? DirectionDiagonal::TOP
							                : DirectionDiagonal::BOTTOM;

						return DirectionDiagonal::UNDEFINED;
					}

					/** Get the diagonal distance to a coordinate, or -1 if
						the coordinate isn't reachable in one diagonal move

						A diagonal move is a straight move in one of the six
						directions of the lines between the neighboring hex-tiles.
					 */
					constexpr int8_t getDistanceDiagonal(Coordinate other) const
					{ return isDiagonal(other) ? getDistance(other) / 2 : -1; }

					/** Get the distance to another coordinate with the same distance to
						the center Coordinate along the line of these coordinates, or -1
						if the coordinate isn't reachable while only moving on that line.

						All possible movement targets here are along a hexagonal line.
						The direct distance from this coordinate to another one on that
						line may be different than the distance along the line.
					 */
					int8_t getDistanceHexagonalLine(Coordinate other, Coordinate center) const
					{
						auto distance = getDistance(center);

						// Check if the given coordinate has the same distance to center as this
						if(distance != other.getDistance(center))
							return -1;

						// TODO
						/*const MovementVec steps {
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

					/// @{
					/// Create a Coordinate object from an X and an Y
					/// If the coordinte is invalid, return nullptr
					static constexpr std::unique_ptr<Coordinate> create(int8_t X, int8_t Y)
					{
						// can't use make_unique because private constructor has to be called directly
						return isValid(X, Y)
							? std::unique_ptr<Coordinate>(new Coordinate(X, Y))
							: nullptr;
					}

					template<class T>
					static std::unique_ptr<Coordinate> create(typename std::valarray<T> a)
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

						/* X: When using char's for calculations, their ASCII values are used,
							  and as the uppercase letters are consecutive in the ASCII table,
							  this will result in 0 for 'A', 1 for 'B' and so on.
							  We don't accept lowercase letters.
						   Y: Public coordinate notation starts with 1, we start with 0
						 */
						return std::unique_ptr<Coordinate>(
							new Coordinate(str.at(0) - 'A', std::stoi(str.substr(1)) - 1)
						);
					}
			};

			/// The edge length of the hexagon (template parameter)
			static constexpr uint8_t edgeLength = l;

			/// The amount of tiles that make up the hexagon
			static constexpr uint16_t tileCount = (l * (l - 1)) / 2 * 6 + 1;

			/// Get a container with all possible coordinates in this hexagon
			static const std::set<Coordinate>& getAllCoordinates()
			{
				static std::set<Coordinate> set;

				if(set.empty())
				{
					for(auto X = 0; X < (2 * l) - 1; X++)
					{
						auto yBegin = (X < (l - 1)) ? (l - 1 - X) : 0;
						auto yEnd   = (X < (l - 1)) ? (2 * l - 1) : (2 * l - 1) + (l - 1 - X);

						for(auto Y = yBegin; Y < yEnd; Y++)
						{
							std::unique_ptr<Coordinate> c = Coordinate::create(X, Y);
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
