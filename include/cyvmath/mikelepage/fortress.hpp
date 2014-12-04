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

#ifndef _CYVMATH_MIKELEPAGE_FORTRESS_HPP_
#define _CYVMATH_MIKELEPAGE_FORTRESS_HPP_

#include <cyvmath/players_color.hpp>
#include "coordinate.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Fortress
		{
			protected:
				const PlayersColor m_color;
				Coordinate m_coord;

				bool m_ruined = false;

			public:
				const bool& isRuined = m_ruined;

				Fortress(PlayersColor color, Coordinate coord)
					: m_color{color}
					, m_coord{coord}
				{ }

				PlayersColor getColor()
				{ return m_color; }

				Coordinate getCoord()
				{ return m_coord; }

				virtual void setCoord(Coordinate coord)
				{ m_coord = coord; }

				virtual void ruined()
				{ m_ruined = true; }
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_FORTRESS_HPP_
