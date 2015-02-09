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

#ifndef _CYVMATH_PLAYERS_COLOR_HPP_
#define _CYVMATH_PLAYERS_COLOR_HPP_

#include <vector>
#include <enum_str.hpp>

namespace cyvmath
{
	// supposed to act like an enum, but
	// enums can't have conversion operators

	class PlayersColor
	{
		private:
			int m_val;

			explicit PlayersColor(int val)
				: m_val(val)
			{ }

		public:
			PlayersColor(const PlayersColor& other)
				: m_val(other.m_val)
			{ }

			static const PlayersColor UNDEFINED;
			static const PlayersColor WHITE;
			static const PlayersColor BLACK;

			PlayersColor operator!() const;

			bool operator==(PlayersColor other) const
			{ return m_val == other.m_val; }

			bool operator!=(PlayersColor other) const
			{ return m_val != other.m_val; }

			bool operator<(PlayersColor other) const
			{ return m_val < other.m_val; }

			bool operator>(PlayersColor other) const
			{ return m_val > other.m_val; }

			operator unsigned() const;

			// don't allow comparing with bool,
			// int or unsigned, which would
			// otherwise be possible because of
			// the conversion operator to unsigned
			bool operator==(bool) const     = delete;
			bool operator==(int) const      = delete;
			bool operator==(unsigned) const = delete;
			bool operator!=(bool) const     = delete;
			bool operator!=(int) const      = delete;
			bool operator!=(unsigned) const = delete;
			bool operator<(int) const       = delete;
			bool operator<(unsigned) const  = delete;
			bool operator>(int) const       = delete;
			bool operator>(unsigned) const  = delete;
	};

	// moving the definition to the cpp leads
	// to undefined references for some reason
	ENUM_STR(PlayersColor, ({
		{PlayersColor::UNDEFINED, "undefined"},
		{PlayersColor::WHITE, "white"},
		{PlayersColor::BLACK, "black"}
	}))

	const std::vector<PlayersColor> allPlayersColors();
	const std::string& PlayersColorToPrettyStr(PlayersColor);
}

#endif // _CYVMATH_PLAYERS_COLOR_HPP_
