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

#include <memory>
#include <cassert>
#include <tntdb/connection.h>
#include <cyvmath/match.hpp>
#include <cyvmath/rule_sets.hpp>

namespace cyvdb
{
	class MatchManager
	{
		private:
			tntdb::Connection m_conn;

			static bool matchValid(const cyvmath::Match& match);

			int getRuleSetID(cyvmath::RuleSet);

		public:
			explicit MatchManager(tntdb::Connection& conn);
			MatchManager();

			// queries
			typedef std::vector<std::unique_ptr<cyvmath::Match>> matchList;

			matchList getOpenRandomMatches();
			matchList getRunningPublicMatches();

			// modifications
			void addMatch(std::unique_ptr<cyvmath::Match>);

			void removeMatch(const std::string& id);
	};
}

#endif // _CYVDB_MATCH_MANAGER_HPP_
