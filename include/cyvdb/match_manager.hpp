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

#ifndef _CYVDB_MATCH_MANAGER_HPP_
#define _CYVDB_MATCH_MANAGER_HPP_

#include <mutex>
#include <tntdb/connection.h>
#include <cyvmath/rule_sets.hpp>

namespace cyvdb
{
	using cyvmath::RuleSet;

	class Match;

	class MatchManager
	{
		private:
			tntdb::Connection m_conn;

			static std::mutex randomMatchesMtx;

		public:
			explicit MatchManager(tntdb::Connection& conn);
			MatchManager();

			// queries
			Match getMatch(const std::string& matchID);
			std::vector<Match> getFreeRandomModeMatches();

			// modifications
			void addMatch(const Match&);

			//Match getOldestRandomModeMatch(RuleSet);

			void removeMatch(const std::string& id);
			void removeRandomModeMatch(const std::string& id);

			int getRuleSetID(RuleSet);
	};
}

#endif // _CYVDB_MATCH_MANAGER_HPP_
