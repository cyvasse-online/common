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

#include <cyvws/json_game_msg.hpp>

using namespace cyvmath;

namespace cyvws
{
namespace json
{
	Json::Value piecePosition(PieceType pieceType, const Coordinate& pos)
	{
		if(pieceType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPiecePosition: pieceType is undefined");

		Json::Value data;
		data["pieceType"] = PieceTypeToStr(pieceType);
		data["pos"]	   = pos.toString();

		return data;
	}

	Json::Value pieceMovement(PieceType pieceType, const Coordinate& oldPos, const Coordinate& newPos)
	{
		if(pieceType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPieceMovement: pieceType is undefined");

		Json::Value data;
		data["pieceType"] = PieceTypeToStr(pieceType);
		data["oldPos"]    = oldPos.toString();
		data["newPos"]    = newPos.toString();

		return data;
	}

	Json::Value promotion(PieceType origType, PieceType newType)
	{
		if(origType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPromotion: origType is undefined");
		if(newType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPromotion: newType is undefined");

		Json::Value data;
		data["origType"] = PieceTypeToStr(origType);
		data["newType"]  = PieceTypeToStr(newType);

		return data;
	}

	Json::Value piecePosition(const Piece& piece)
	{
		if(!piece.getCoord())
			throw std::invalid_argument("cyvws::createJsonPiecePosition: piece has coordinate");

		return piecePosition(piece.getType(), *piece.getCoord());
	}
}
}
