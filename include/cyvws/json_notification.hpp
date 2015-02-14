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

#ifndef _CYVWS_JSON_NOTIFICATION_HPP_
#define _CYVWS_JSON_NOTIFICATION_HPP_

#include <string>
#include <json/value.h>

namespace cyvws
{
	namespace json
	{
		Json::Value notification(const Json::Value& notificationData);

		Json::Value commErr(const std::string& errMsg);
		//Json::Value listUpdate(...);
		Json::Value userJoined(const std::string& screenName, bool registered, const std::string& role);
		Json::Value userLeft(const std::string& screenName);
	}
}

#endif // _CYVWS_JSON_NOTIFICATION_HPP_
