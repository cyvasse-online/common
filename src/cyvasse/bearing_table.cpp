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

#include <cyvasse/bearing_table.hpp>

#include <cassert>

namespace cyvasse
{
	bool BearingTable::canTake(const Piece& atkPiece, const Piece& defPiece) const
	{
		assert(defPiece.getType() != PieceType::MOUNTAINS);

		uint8_t defenseTier = defPiece.getEffectiveDefenseTier();

		if (atkPiece.getBaseTier() >= defenseTier)
			return true;

		bool haveKing = (atkPiece.getType() == PieceType::KING);

		uint8_t maxAllowedTier = haveKing ? 3 : atkPiece.getBaseTier();
		uint8_t maxTier = 1;

		auto defPieceIt = m_canBeReachedBy.find(&defPiece);
		if (defPieceIt == m_canBeReachedBy.end())
			return false;

		std::map<uint8_t, uint8_t> flankingTiers {
			{1, 0},
			{2, 0},
			{3, 0}
		};

		for (const Piece* piece : defPieceIt->second)
		{
			auto baseTier = piece->getBaseTier();

			if (baseTier > maxAllowedTier)
				continue;

			if (piece->getType() == PieceType::KING)
			{
				// king will count as maxTier after the loop
				haveKing = true;
				continue;
			}

			if (baseTier > maxTier)
				maxTier = baseTier;

			++flankingTiers[baseTier];
		}

		if (haveKing)
			++flankingTiers[maxTier];

		for (uint8_t i = 1; i < maxTier; ++i)
			flankingTiers[i+1] += (flankingTiers[i] > 0 ? flankingTiers[i] - 1 : 0);

		uint8_t attackTier = maxTier + flankingTiers[maxTier] - 1;

		return attackTier >= defenseTier;
	}

	void BearingTable::init()
	{
		for (auto it : m_pieceMap)
		{
			auto piece = it.second.get();

			if (piece->getType() == PieceType::MOUNTAINS || piece->getType() == PieceType::DRAGON)
				continue;

			auto reachableOpPieces = piece->getReachableOpponentPieces();

			if (reachableOpPieces.empty())
				continue;

			for (const Piece* opPiece : reachableOpPieces)
			{
				auto opPieceIt = m_canBeReachedBy.find(opPiece);
				if (opPieceIt == m_canBeReachedBy.end())
				{
					auto res = m_canBeReachedBy.emplace(opPiece, std::set<const Piece*>{piece});
					assert(res.second);
				}
				else
				{
					auto res = opPieceIt->second.insert(piece);
					assert(res.second);
				}
			}
		}
	}

	void BearingTable::clear()
	{
		m_canBeReachedBy.clear();
	}

	void BearingTable::update()
	{
		clear();
		init();
	}
}
