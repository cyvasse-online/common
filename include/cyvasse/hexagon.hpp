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

#include "coordinate.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <set>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <optional.hpp>

namespace cyvasse
{
	enum class DirectionOrthogonal
	{
		NONE,
		TOP_LEFT,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
		LEFT
	};

	enum class DirectionDiagonal
	{
		NONE,
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
		// A hexagon of size 1 is a single square which definitely makes no sense
		static_assert(l >= 2,  "The minimum size of the hexagon edge length is 2.");

		// The biggest hexagon coordinate parts will always have the value l * 2 - 1
		// l = 13 thus is the biggest possible edge length when one part
		// of the coordinate is represented as a single character from A to Z
		static_assert(l <= 13, "The maximum size of the hexagon edge length is 13.");

		private:
			Hexagon() = delete;

		public:
			/// A coordinate on the hexboard (see mockup/hexboard-coordinates-internal.svg)
			class Coordinate : public cyvasse::Coordinate
			{
				private:
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
					constexpr Coordinate(int8_t X, int8_t Y)
						: cyvasse::Coordinate(
							!isValid(X, Y)
								? throw std::invalid_argument("Invalid HexCoordinate<" + std::to_string(l) + ">"
									"(" + std::to_string(X) + ", " + std::to_string(Y) + ")")
								: X, Y
						)
					{ }

					/// Create a Coordinate object from a coordinate in the public notation
					Coordinate(const std::string& str)
						: cyvasse::Coordinate(str)
					{
						if (!isValid())
						{
							throw std::invalid_argument("Invalid HexCoordinate<" + std::to_string(l) + ">"
								" : (" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")");
						}
					}

					template<class T>
					Coordinate(const typename std::valarray<T>& a)
						: cyvasse::Coordinate(a[0], a[1])
					{
						if(a.size() != 2)
							throw std::invalid_argument("Invalid std::valarray for HexCoordinate construction");

						if(!isValid(a[0], a[1]))
							throw std::invalid_argument("HexCoordinate (" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ") is not valid");
					}

					virtual ~Coordinate() = default;

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
						if (*this == other) return DirectionOrthogonal::NONE;

						if (m_x == other.m_x)
							return (m_y > other.m_y) ? DirectionOrthogonal::BOTTOM_LEFT
							                         : DirectionOrthogonal::TOP_RIGHT;
						if (m_y == other.m_y)
							return (m_x > other.m_x) ? DirectionOrthogonal::LEFT
							                         : DirectionOrthogonal::RIGHT;
						if (z() == other.z())
							return (m_x > other.m_x) ? DirectionOrthogonal::TOP_LEFT
							                         : DirectionOrthogonal::BOTTOM_RIGHT;

						return DirectionOrthogonal::NONE;
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
						auto dX = m_x - other.m_x;
						auto dY = m_y - other.m_y;
						auto dZ = z() - other.z();

						return dX == dY ||
						       dY == dZ ||
						       dZ == dX;
					}

					DirectionDiagonal getDirectionDiagonal(Coordinate other) const
					{
						if (*this == other)
							return DirectionDiagonal::NONE;

						int8_t dX = m_x  - other.m_x;
						int8_t dY = m_y  - other.m_y;
						int8_t dZ = z() - other.z();

						if (dX == dY)
							return (dX > 0) ? DirectionDiagonal::BOTTOM_LEFT
							                : DirectionDiagonal::TOP_RIGHT;
						if (dY == dZ)
							return (dY > 0) ? DirectionDiagonal::BOTTOM_RIGHT
							                : DirectionDiagonal::TOP_LEFT;
						if (dZ == dX)
							return (dZ > 0) ? DirectionDiagonal::TOP
							                : DirectionDiagonal::BOTTOM;

						return DirectionDiagonal::NONE;
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
						if (distance != other.getDistance(center))
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
					static constexpr optional<Coordinate> create(int8_t X, int8_t Y)
					{
						if (isValid(X, Y))
							return Coordinate(X, Y);

						return nullopt;
					}

					template<class T>
					static optional<Coordinate> create(typename std::valarray<T> a)
					{
						assert(a.size() == 2);
						return create(a[0], a[1]);
					}
					/// @}
			};

			/// The edge length of the hexagon (template parameter)
			static constexpr uint8_t edgeLength = l;

			/// The amount of tiles that make up the hexagon
			static constexpr uint16_t tileCount = (l * (l - 1)) / 2 * 6 + 1;

			/// A container with all possible coordinates in this hexagon
			static const std::set<Coordinate> allCoordinates;
	};

	template <uint8_t l>
	const std::set<typename Hexagon<l>::Coordinate> Hexagon<l>::allCoordinates = [] {
		std::set<Coordinate> set;

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

	template <uint8_t l>
	using HexCoordinate = typename Hexagon<l>::Coordinate;
}

#endif // _CYVASSE_HEXAGON_HPP_
