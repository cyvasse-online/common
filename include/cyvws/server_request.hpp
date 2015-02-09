/* Copyright 2015 Jonas Platte
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

#ifndef _CYVWS_SERVER_REQUEST_HPP_
#define _CYVWS_SERVER_REQUEST_HPP_

#include <string>

namespace cyvws
{
	const std::string REQUEST_DATA = "requestData";

	namespace ServerRequestAction
	{
		const std::string
			CREATE_GAME                = "createGame",
			INIT_COMM                  = "initComm",
			JOIN_GAME                  = "joinGame",
			SUBSCR_GAME_LIST_UPDATES   = "subscrGameListUpdates",
			UNSUBSCR_GAME_LIST_UPDATES = "unsubscrGameListUpdates";
	}
}

#endif // _CYVWS_SERVER_REQUEST_HPP_
