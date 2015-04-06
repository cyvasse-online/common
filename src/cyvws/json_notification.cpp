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

#include <cyvws/json_notification.hpp>

#include <json/value.h>
#include <cyvws/common.hpp>
#include <cyvws/msg.hpp>
#include <cyvws/notification.hpp>

using namespace std;

namespace cyvws
{
	namespace json
	{
		Json::Value notification(const Json::Value& notificationData)
		{
			Json::Value val;
			val[MSG_TYPE] = MsgType::NOTIFICATION;
			val[NOTIFICATION_DATA] = notificationData;

			return val;
		}

		Json::Value commErr(const string& errMsg)
		{
			Json::Value data;
			data[TYPE]    = NotificationType::COMM_ERROR;
			data[ERR_MSG] = errMsg;

			return notification(data);
		}

		Json::Value listUpdate(const string& listName, const GamesListMap& curList)
		{
			Json::Value data;
			data[TYPE]      = NotificationType::LIST_UPDATE;
			data[LIST_NAME] = listName;

			auto& listContent = data[LIST_CONTENT] = Json::Value(Json::arrayValue);
			for (auto&& game : curList)
			{
				Json::Value gameVal;
				gameVal[MATCH_ID] = game.first;
				gameVal[TITLE]    = game.second.title;
				//gameVal[RULE_SET]
				gameVal[PLAY_AS]  = PlayersColorToStr(game.second.playAs);
				//gameVal[EXTRA_RULES]

				listContent.append(gameVal);
			}

			return notification(data);
		}

		Json::Value userJoined(const string& username, bool registered, const string& role)
		{
			Json::Value data;
			data[TYPE]       = NotificationType::USER_JOINED;
			data[USERNAME]   = username;
			data[REGISTERED] = registered;
			data[ROLE]       = role;

			return notification(data);
		}

		Json::Value userLeft(const string& username)
		{
			Json::Value data;
			data[TYPE]     = NotificationType::USER_LEFT;
			data[USERNAME] = username;

			return notification(data);
		}
	}
}
