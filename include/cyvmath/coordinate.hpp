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

#include <string>
#include <valarray>
#include <cstdint>

namespace cyvmath
{
	class Coordinate
	{
		protected:
			int8_t m_x, m_y;

			Coordinate(int8_t X, int8_t Y)
				: m_x(X)
				, m_y(Y)
			{ }

		public:
			virtual ~Coordinate() = default;

			Coordinate(const Coordinate&) = default;
			Coordinate(Coordinate&&) = default;

			Coordinate& operator=(const Coordinate&) = default;
			Coordinate& operator=(Coordinate&&) = default;

			int8_t x() const
			{ return m_x; }

			int8_t y() const
			{ return m_y; }

			int16_t dump() const
			{ return (m_x << 8) | m_y; }

			virtual std::string toString() const;

			template<class T = uint8_t>
			std::valarray<T> toValarray() const
			{
				static_assert(std::is_integral<T>::value, "T has to be an integral type");
				return {static_cast<T>(m_x), static_cast<T>(m_y)};
			}

			bool operator==(const Coordinate& other) const
			{ return dump() == other.dump(); }

			bool operator!=(const Coordinate& other) const
			{ return dump() != other.dump(); }

			bool operator<(const Coordinate& other) const
			{ return dump() < other.dump(); }
	};
}

#endif // _CYVMATH_COORDINATE_HPP_
