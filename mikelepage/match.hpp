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
		enum PlayersColor
		{
			PLAYER_WHITE,
			PLAYER_BLACK
		};

		class Match
		{
			protected:
				bool _setup;

				bool _setupComplete;

				const PlayersColor _playersColor;

				PieceMap _activePieces[2];
				PieceVec _inactivePieces[2];

				bool _dragonAlive[2];

			public:
				Match(PlayersColor color)
					: _setup(true)
					, _setupComplete(false)
					, _playersColor(color)
					, _dragonAlive{true, true}
				{
				}

				virtual ~Match()
				{
				}

				void checkSetupComplete()
				{
					for(auto it : _activePieces[_playersColor])
					{
						// hexagon<6> isn't typedef'ed in cyvmath. If that changes,
						// 5 should be replaced by (Hexagon::edgeLength - 1)
						if((_playersColor == PLAYER_WHITE && it.first.y() >= 5) ||
						   (_playersColor == PLAYER_BLACK && it.first.y() <= 5))
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
