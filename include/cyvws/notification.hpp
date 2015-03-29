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

#ifndef _CYVWS_NOTIFICATION_HPP_
#define _CYVWS_NOTIFICATION_HPP_

#include <string>
#include <cyvasse/players_color.hpp>

namespace cyvws
{
	const std::string
		NOTIFICATION_DATA = "notificationData",
		LIST_NAME         = "listName",
		LIST_CONTENT      = "listContent",
		TITLE             = "title",
		PLAY_AS           = "playAs";

	namespace NotificationType
	{
		const std::string
			COMM_ERROR  = "commError",
			LIST_UPDATE = "listUpdate",
			USER_JOINED = "userJoined",
			USER_LEFT   = "userLeft";
	}

	const std::string LISTS = "lists";

	namespace GamesList
	{
		const std::string
			OPEN_RANDOM_GAMES    = "openRandomGames",
			RUNNING_PUBLIC_GAMES = "runningPublicGames";
	}

	struct GamesListMappedType
	{
		std::string title;
		cyvasse::PlayersColor playAs;
	};

	typedef std::map<std::string, GamesListMappedType>
		GamesListMap;
}

#endif // _CYVWS_NOTIFICATION_HPP_
