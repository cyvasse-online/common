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

#ifndef _CYVWS_NOTIFICATION_TYPE_HPP_
#define _CYVWS_NOTIFICATION_TYPE_HPP_

#include "enum_str.hpp"

namespace cyvws
{
	enum class NotificationType
	{
		UNDEFINED,
		LIST_UPDATE,
		USER_JOINED,
		USER_LEFT,
	};

	ENUM_STR(NotificationType, ({
			{NotificationType::UNDEFINED, "undefined"},
			{NotificationType::LIST_UPDATE, "listUpdate"},
			{NotificationType::USER_JOINED, "userJoined"},
			{NotificationType::USER_LEFT, "userLeft"},
		}),
		NotificationType::UNDEFINED
	)
}

#endif // _CYVWS_NOTIFICATION_TYPE_HPP_
