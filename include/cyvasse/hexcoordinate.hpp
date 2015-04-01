/* Copyright 2015 Jonas Platte
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

#ifndef _CYVASSE_HEXCOORDINATE_HPP_
#define _CYVASSE_HEXCOORDINATE_HPP_

#include <ostream>
#include <stdexcept>
#include <string>
#include <valarray>

#include <cassert>
#include <cstdint>

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

	/// A coordinate on the hexboard (see mockup/hexboard-coordinates-internal.svg)
	template <uint8_t l>
	class HexCoordinate
	{
		// A hexagon of size 1 is a single square which definitely makes no sense
		static_assert(l >= 2,  "The minimum size of the hexagon edge length is 2.");

		// The biggest hexagon coordinate parts will always have the value l * 2 - 1
		// l = 13 thus is the biggest possible edge length when one part
		// of the coordinate is represented as a single character from A to Z
		static_assert(l <= 13, "The maximum size of the hexagon edge length is 13.");

		private:
			static constexpr bool isValid(int8_t X, int8_t Y)
			{
				return X >= 0 && X < (l * 2 - 1) &&
					Y >= 0 && Y < (l * 2 - 1) &&
					(X + Y) >= (l - 1) &&
					(X + Y) <= (l - 1) * 3;
			}

			int8_t m_x, m_y;

			bool isValid() const
			{ return isValid(m_x, m_y); }

		public:
			constexpr HexCoordinate(int8_t X, int8_t Y)
				: m_x(!isValid(X, Y)
						? throw std::invalid_argument("Invalid HexCoordinate<" + std::to_string(l) + ">"
							"(" + std::to_string(X) + ", " + std::to_string(Y) + ")")
						: X
				)
				, m_y(Y)
			{ }

			/// Create a HexCoordinate object from a coordinate in the public notation
			HexCoordinate(const std::string& str)
				: HexCoordinate(str.at(0) - 'A', std::stoi(str.substr(1)) - 1)
			{ }

			template<class T>
			HexCoordinate(const typename std::valarray<T>& a)
				: HexCoordinate(a[0], a[1])
			{ }

			constexpr int8_t x() const
			{ return m_x; }

			constexpr int8_t y() const
			{ return m_y; }

			constexpr int8_t z() const
			{ return -(m_x + m_y); }

			constexpr int16_t dump() const
			{ return (m_x << 8) | m_y; }

			std::string toString() const
			{ return std::string(1, m_x + 'A') + std::to_string(m_y + 1); }

			template<class T = uint8_t>
			std::valarray<T> toValarray() const
			{
				static_assert(std::is_integral<T>::value, "T has to be an integral type");
				return {static_cast<T>(m_x), static_cast<T>(m_y)};
			}

			bool operator==(HexCoordinate other) const
			{ return dump() == other.dump(); }

			bool operator!=(HexCoordinate other) const
			{ return dump() != other.dump(); }

			bool operator<(HexCoordinate other) const
			{ return dump() < other.dump(); }

			/** Get the distance to another coordinate in form of the amount
				of single moves to adjacent tiles required to move there
			*/
			constexpr int8_t getDistance(HexCoordinate other) const
			{
				// Concept from http://keekerdc.com/2011/03/hexagon-grids-coordinate-systems-and-distance-calculations/
				// I have no idea why the maximum of x-, y- and z-difference
				// of the coordinates equals the distance between them...
				return std::max(abs(m_x - other.m_x), std::max(abs(m_y - other.m_y), abs(z() - other.z())));
			}

			/// Check if the given coordinate is reachable in one orthogonal move
			constexpr bool isOrthogonal(HexCoordinate other) const
			{ return m_x == other.m_x || m_y == other.m_y || z() == other.z(); }

			DirectionOrthogonal getDirectionOrthogonal(HexCoordinate other) const
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
			constexpr int8_t getDistanceOrthogonal(HexCoordinate other) const
			{ return isOrthogonal(other) ? getDistance(other) : -1; }

			/// Check if the given coordinate is reachable in one diagonal move
			constexpr bool isDiagonal(HexCoordinate other) const
			{
				auto dX = m_x - other.m_x;
				auto dY = m_y - other.m_y;
				auto dZ = z() - other.z();

				return dX == dY ||
				       dY == dZ ||
				       dZ == dX;
			}

			DirectionDiagonal getDirectionDiagonal(HexCoordinate other) const
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
			constexpr int8_t getDistanceDiagonal(HexCoordinate other) const
			{ return isDiagonal(other) ? getDistance(other) / 2 : -1; }

			/** Get the distance to another coordinate with the same distance to
				the center HexCoordinate along the line of these coordinates, or -1
				if the coordinate isn't reachable while only moving on that line.

				All possible movement targets here are along a hexagonal line.
				The direct distance from this coordinate to another one on that
				line may be different than the distance along the line.
			*/
			int8_t getDistanceHexagonalLine(HexCoordinate other, HexCoordinate center) const
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

				auto cornerCoords = getHexCoordinates(steps, 1);*/

				return 0;
			}

			/// @{
			/// Create a HexCoordinate object from an X and an Y
			/// If the coordinte is invalid, return nullptr
			static constexpr optional<HexCoordinate> create(int8_t X, int8_t Y)
			{
				if (isValid(X, Y))
					return HexCoordinate(X, Y);

				return nullopt;
			}

			template<class T>
			static optional<HexCoordinate> create(typename std::valarray<T> a)
			{
				assert(a.size() == 2);
				return create(a[0], a[1]);
			}
			/// @}
	};

	template <uint8_t l>
	std::ostream& operator<<(std::ostream& os, HexCoordinate<l> coord)
	{
		os << coord.toString();
		return os;
	}
}

#endif // _CYVASSE_HEXCOORDINATE_HPP_
