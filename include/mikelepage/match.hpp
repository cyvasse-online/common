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

#ifndef _CYVMATH_MIKELEPAGE_MATCH_HPP_
#define _CYVMATH_MIKELEPAGE_MATCH_HPP_

#include "piece.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Match
		{
			protected:
				const PlayersColor _playersColor;

				bool _setup;
				bool _setupComplete;

				bool _dragonAlive[2];

				PieceMap _activePieces;
				PieceVec _inactivePieces[2];

				PlayersColor _activePlayer;

			public:
				Match(PlayersColor color)
					: _setup(true)
					, _setupComplete(false)
					, _playersColor(color)
					, _dragonAlive{true, true}
					, _activePlayer(PLAYER_WHITE)
				{
				}

				virtual ~Match()
				{
				}

				void checkSetupComplete()
				{
					auto outsideOwnSide = _playersColor ? [](int8_t y) { return y <= Hexagon::edgeLength; }
					                                    : [](int8_t y) { return y >= Hexagon::edgeLength; };

					for(auto it : _activePieces)
					{
						if(_playersColor == it.second->getPlayersColor() && outsideOwnSide(it.first.y()))
						{
							_setupComplete = false;
							return;
						}
					}

					_setupComplete = true;
				}
		};
	}
}

#endif // _CYVMATH_MIKELEPAGE_MATCH_HPP_
