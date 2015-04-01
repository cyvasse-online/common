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

#ifndef _CYVASSE_TERRAIN_HPP_
#define _CYVASSE_TERRAIN_HPP_

#include <array>
#include <map>
#include <memory>
#include "hexagon.hpp"
#include "piece_type.hpp"
#include "terrain_type.hpp"

namespace cyvasse
{
	class Terrain
	{
		protected:
			const TerrainType m_type;
			HexCoordinate<6> m_coord;

		public:
			Terrain(TerrainType type, HexCoordinate<6> coord)
				: m_type(type)
				, m_coord(coord)
			{ }

			virtual ~Terrain() = default;

			TerrainType getType()
			{ return m_type; }

			virtual void setCoord(HexCoordinate<6> coord)
			{ m_coord = coord; }
	};

	typedef std::map<Coordinate, std::shared_ptr<Terrain>> TerrainMap;
}

#endif // _CYVASSE_TERRAIN_HPP_
