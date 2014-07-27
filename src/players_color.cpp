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

#include <cyvmath/players_color.hpp>

#include <cassert>

namespace cyvmath
{
	const PlayersColor PlayersColor::UNDEFINED = PlayersColor(-1);
	const PlayersColor PlayersColor::WHITE     = PlayersColor(0);
	const PlayersColor PlayersColor::BLACK     = PlayersColor(1);

	PlayersColor PlayersColor::operator!() const
	{
		assert(_val != -1);

		// int-to-bool, operator!(bool), bool-to-int
		return PlayersColor(!_val);
	}

	PlayersColor::operator unsigned() const
	{
		assert(_val != -1);

		return _val;
	}
}
