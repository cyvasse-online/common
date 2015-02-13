/* Copyright 2014 - 2015 Jonas Platte
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

#ifndef _CYVWS_JSON_GAME_MSG_HPP_
#define _CYVWS_JSON_GAME_MSG_HPP_

#include <map>
#include <set>
#include <json/value.h>
#include <cyvmath/coordinate.hpp>
#include <cyvmath/piece_type.hpp>
#include <cyvmath/piece.hpp>

namespace cyvws
{
	struct PiecePosition
	{
		cyvmath::PieceType pieceType;
		cyvmath::Coordinate pos;
	};

	struct PieceMovement
	{
		cyvmath::PieceType pieceType;
		cyvmath::Coordinate oldPos;
		cyvmath::Coordinate newPos;
	};

	struct Promotion
	{
		cyvmath::PieceType origType;
		cyvmath::PieceType newType;
	};

	typedef std::map<cyvmath::PieceType, std::set<cyvmath::Coordinate>> OpeningArray;

	namespace json
	{
		Json::Value piecePosition(cyvmath::PieceType pieceType, cyvmath::Coordinate pos);
		PiecePosition piecePosition(const Json::Value&);
		Json::Value piecePosition(const cyvmath::Piece&); // convenience overload

		Json::Value pieceMovement(cyvmath::PieceType pieceType, cyvmath::Coordinate oldPos, cyvmath::Coordinate newPos);
		PieceMovement pieceMovement(const Json::Value&);

		Json::Value openingArray(const OpeningArray&);
		template <class piece_t> // convenience overload
		Json::Value openingArray(const std::map<cyvmath::Coordinate, piece_t>&);
		OpeningArray openingArray(const Json::Value&);

		Json::Value promotion(cyvmath::PieceType origType, cyvmath::PieceType newType);
		Promotion promotion(const Json::Value&);

		// complete message assembling

		Json::Value gameMsg(const std::string& gameMsgAction, const Json::Value& param);

		Json::Value gameMsgSetOpeningArray(const OpeningArray&);
		template <class piece_t> // convenience overload
		Json::Value gameMsgSetOpeningArray(const std::map<cyvmath::Coordinate, piece_t>& pieces);

		Json::Value gameMsgSetIsReady(bool val);
		Json::Value gameMsgMove(cyvmath::PieceType pieceType, cyvmath::Coordinate oldPos, cyvmath::Coordinate newPos);
		Json::Value gameMsgPromote(cyvmath::PieceType origType, cyvmath::PieceType newType);
	}
}

#include "json_game_msg.ipp"

#endif // _CYVWS_JSON_GAME_MSG_HPP_
