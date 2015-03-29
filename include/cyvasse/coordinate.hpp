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

#ifndef _CYVASSE_COORDINATE_HPP_
#define _CYVASSE_COORDINATE_HPP_

#include <ostream>
#include <stdexcept>
#include <string>
#include <valarray>
#include <cstdint>

namespace cyvasse
{
	class Coordinate
	{
		protected:
			int8_t m_x, m_y;

		public:
			constexpr Coordinate(int8_t X, int8_t Y)
				: m_x(X > 25 ? throw std::invalid_argument("Coordinate(X, Y): X (" + std::to_string(X) + ") out of range") : X)
				, m_y(Y > 25 ? throw std::invalid_argument("Coordinate(X, Y): Y (" + std::to_string(Y) + ") out of range") : Y)
			{ }

			Coordinate(std::string str);

			template<class T>
			Coordinate(typename std::valarray<T> a)
			{
				if(a.size() != 2)
					throw std::invalid_argument("Invalid std::valarray for Coordinate construction");

				if(a[0] > 25 || a[1] > 25)
					throw std::invalid_argument("Coordinate (" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ") is not valid");

				m_x = a[0];
				m_y = a[1];
			}

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

	std::ostream& operator <<(std::ostream& os, const Coordinate& coord);
}

#endif // _CYVASSE_COORDINATE_HPP_
