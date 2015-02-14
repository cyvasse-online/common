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

#include <cyvws/common.hpp>
#include <cyvws/msg.hpp>
#include <cyvws/notification.hpp>

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

		Json::Value commErr(const std::string& errMsg)
		{
			Json::Value data;
			data[TYPE]    = NotificationType::COMM_ERROR;
			data[ERR_MSG] = errMsg;

			return notification(data);
		}

		Json::Value userJoined(const std::string& screenName, bool registered, const std::string& role)
		{
			Json::Value data;
			data[TYPE]        = NotificationType::USER_JOINED;
			data[SCREEN_NAME] = screenName;
			data[REGISTERED]  = registered;
			data[ROLE]        = role;

			return notification(data);
		}

		Json::Value userLeft(const std::string& screenName)
		{
			Json::Value data;
			data[TYPE]        = NotificationType::USER_LEFT;
			data[SCREEN_NAME] = screenName;

			return notification(data);
		}
	}
}
