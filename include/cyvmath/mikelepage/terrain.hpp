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

#ifndef _CYVMATH_MIKELEPAGE_TERRAIN_HPP_
#define _CYVMATH_MIKELEPAGE_TERRAIN_HPP_

#include <array>
#include <enum_str.hpp>
#include "common.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		enum class TerrainType
		{
			UNDEFINED,
			HILL,
			FOREST,
			GRASSLAND
		};

		ENUM_STR_PROT(TerrainType)

		class Terrain
		{
			private:
				TerrainType _type;
				Coordinate _coord;

			public:
				Terrain(TerrainType type, Coordinate coord)
					: _type(type)
					, _coord(coord)
				{ }

				TerrainType getType()
				{ return _type; }

				std::array<PieceType, 2> getAdvantagedPieceTypes();
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_TERRAIN_HPP_
