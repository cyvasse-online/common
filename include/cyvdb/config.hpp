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

#ifndef _CYVDB_CONFIG_HPP_
#define _CYVDB_CONFIG_HPP_

#include <string>

namespace cyvdb
{
	class DBConfig
	{
		private:
			std::string m_matchDataUrl;

			DBConfig() = default;

			DBConfig(const DBConfig&) = delete;
			const DBConfig& operator= (const DBConfig&) = delete;

		public:
			static DBConfig& glob()
			{
				static DBConfig s_glob;
				return s_glob;
			}

			const std::string& getMatchDataUrl() const
			{ return m_matchDataUrl; }

			void setMatchDataUrl(const std::string& url)
			{ m_matchDataUrl = url; }
	};
}

#endif // _CYVDB_CONFIG_HPP_
