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

namespace cyvws
{
	namespace json
	{
		template <class piece_t>
		Json::Value openingArray(const std::map<cyvmath::Coordinate, piece_t>& pieces)
		{
			Json::Value data;

			for(auto&& it : pieces)
			{
				auto pieceTypeStr = PieceTypeToStr(it.second->getType());
				auto coordStr = it.first.toString();

				data[pieceTypeStr].append(coordStr);
			}

			return data;
		}

		template <class piece_t>
		std::map<cyvmath::Coordinate, piece_t> openingArray(const Json::Value& val)
		{
			// TODO
			//for(const auto& : )

			return {};
		}

		template <class piece_t>
		Json::Value gameMsgSetOpeningArray(const std::map<cyvmath::Coordinate, piece_t>& pieces)
		{ return gameMsg(GameMsgAction::SET_OPENING_ARRAY, openingArray(pieces)); }
	}
}
