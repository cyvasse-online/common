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

#include <cyvasse/players_color.hpp>

namespace cyvasse
{
	const PlayersColor PlayersColor::WHITE = PlayersColor(0);
	const PlayersColor PlayersColor::BLACK = PlayersColor(1);

	PlayersColor PlayersColor::operator!() const
	{
		// int-to-bool, operator!(bool), bool-to-int
		return PlayersColor(!m_val);
	}

	const std::vector<PlayersColor> allPlayersColors {
		PlayersColor::WHITE,
		PlayersColor::BLACK
	};

	static const std::map<PlayersColor, std::string> prettyStrs {
		{PlayersColor::WHITE, "White player"},
		{PlayersColor::BLACK, "Black player"}
	};

	const std::string& PlayersColorToPrettyStr(PlayersColor c)
	{
		return prettyStrs.at(c);
	}
}
