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

#include <cyvmath/coordinate.hpp>

using namespace std;

namespace cyvmath
{
	/* X: When using char's for calculations, their ASCII values are used,
	      and as the uppercase letters are consecutive in the ASCII table,
	      this will result in 0 for 'A', 1 for 'B' and so on.
	      We don't accept lowercase letters.
	   Y: Public coordinate notation starts with 1, we start with 0 => " - 1"
	*/
	Coordinate::Coordinate(string str)
		: Coordinate(str.at(0) - 'A', std::stoi(str.substr(1)) - 1)
	{ }

	string Coordinate::toString() const
	{
		return string(1, static_cast<char>(m_x + 'A')) + to_string(static_cast<int>(m_y + 1));
	}
}
