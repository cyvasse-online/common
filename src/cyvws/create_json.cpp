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

#include <cyvws/create_json.hpp>

using namespace cyvmath;

namespace cyvws
{
	Json::Value createJsonPiecePosition(PieceType pieceType, const Coordinate& pos)
	{
		if(pieceType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPiecePosition: pieceType is undefined");

		Json::Value data;
		data["pieceType"] = PieceTypeToStr(pieceType);
		data["pos"]	   = pos.toString();

		return data;
	}

	Json::Value createJsonPieceMovement(PieceType pieceType, const Coordinate& oldPos, const Coordinate& newPos)
	{
		if(pieceType == PieceType::UNDEFINED)
			throw std::invalid_argument("cyvws::createJsonPieceMovement: pieceType is undefined");

		Json::Value data;
		data["pieceType"] = PieceTypeToStr(pieceType);
		data["oldPos"]    = oldPos.toString();
		data["newPos"]    = newPos.toString();

		return data;
	}

	Json::Value createJsonPromotion(PieceType origType, PieceType newType)
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

	Json::Value createJsonPiecePosition(const Piece& piece)
	{
		if(!piece.getCoord())
			throw std::invalid_argument("cyvws::createJsonPiecePosition: piece has coordinate");

		return createJsonPiecePosition(piece.getType(), *piece.getCoord());
	}
}
