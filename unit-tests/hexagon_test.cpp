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

#include "hexagon_test.hpp"

#include <map>
#include <stdexcept>
#include <utility>

using namespace std;

namespace CppUnit
{
	template<>
	struct assertion_traits<DirectionOrthogonal>
	{
		static bool equal(DirectionOrthogonal x, DirectionOrthogonal y)
		{ return x == y; }

		static string toString(const DirectionOrthogonal& val)
		{
			switch(val)
			{
				case DirectionOrthogonal::NONE:         return "none";
				case DirectionOrthogonal::TOP_LEFT:     return "top left";
				case DirectionOrthogonal::TOP_RIGHT:    return "top right";
				case DirectionOrthogonal::RIGHT:        return "right";
				case DirectionOrthogonal::BOTTOM_RIGHT: return "bottom right";
				case DirectionOrthogonal::BOTTOM_LEFT:  return "bottom left";
				case DirectionOrthogonal::LEFT:         return "left";
				default:
					CPPUNIT_FAIL("Invalid DirectionOrthogonal value!");
			}

			return {};
		}
	};

	template<>
	struct assertion_traits<DirectionDiagonal>
	{
		static bool equal(DirectionDiagonal x, DirectionDiagonal y)
		{ return x == y; }

		static string toString(const DirectionDiagonal& val)
		{
			switch(val)
			{
				case DirectionDiagonal::NONE:         return "none";
				case DirectionDiagonal::TOP_LEFT:     return "top left";
				case DirectionDiagonal::TOP:          return "top";
				case DirectionDiagonal::TOP_RIGHT:    return "top right";
				case DirectionDiagonal::BOTTOM_RIGHT: return "bottom right";
				case DirectionDiagonal::BOTTOM:       return "bottom";
				case DirectionDiagonal::BOTTOM_LEFT:  return "bottom left";
				default:
					CPPUNIT_FAIL("Invalid DirectionDiagonal value!");
			}

			return {};
		}
	};
}

HexagonTest::HexagonTest()
{
	// function try block would rethrow
	try
	{
		h6Coords       = { {3, 5}, {1, 5}, {3, 10}, {6, 2}, {1, 6}, {1, 9}, {9, 2}, {5, 7} };
		h6CoordStrings = { "D6",   "B6",   "D11",   "G3",   "B7",   "B10",  "J3",   "F8" };
	}
	catch(...)
	{
		CPPUNIT_FAIL("Hardcoded Coordinates not considered valid!");
	}
}

void HexagonTest::testCoordValidity()
{
	static const set<pair<int8_t, int8_t>> invalidCoords {
		{ 0, 11}, // outside – top
		{ 8,  9}, // outside – top right
		{11,  2}, // outside – bottom right
		{10, -2}, // outside – bottom
		{ 1,  3}, // outside – bottom left
		{-2,  7}  // outside – top left
	};

	CPPUNIT_ASSERT_EQUAL(size_t(8), h6Coords.size());

	for (const auto& invalidCoord : invalidCoords)
	{
		CPPUNIT_ASSERT(!HexCoordinate<6>::create(invalidCoord.first, invalidCoord.second));
		CPPUNIT_ASSERT_THROW(HexCoordinate<6>(invalidCoord.first, invalidCoord.second), invalid_argument);
	}
}

void HexagonTest::testCoordEquality()
{
	for (size_t i = 0; i < h6Coords.size(); i++)
		CPPUNIT_ASSERT_EQUAL(h6Coords[i], h6Coords[i]);

	for (size_t i = 1; i < h6Coords.size(); i++)
		CPPUNIT_ASSERT(h6Coords[0] != h6Coords[i]);
}

void HexagonTest::testCoordCompleteness()
{
	CPPUNIT_ASSERT_EQUAL(size_t(7),  Hexagon<2>::allCoordinates.size());
	CPPUNIT_ASSERT_EQUAL(size_t(19), Hexagon<3>::allCoordinates.size());
	CPPUNIT_ASSERT_EQUAL(size_t(37), Hexagon<4>::allCoordinates.size());

	CPPUNIT_ASSERT_NO_THROW(
		set<HexCoordinate<2>> allH2Coords({ {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1} });

		CPPUNIT_ASSERT_EQUAL(allH2Coords.size(), Hexagon<2>::allCoordinates.size());

		for (const auto& coord : Hexagon<2>::allCoordinates)
		{
			auto it = allH2Coords.find(coord);
			CPPUNIT_ASSERT(it != allH2Coords.end());
			allH2Coords.erase(it);
		}
	);
}

void HexagonTest::testCoordToString()
{
	CPPUNIT_ASSERT_EQUAL(h6Coords.size(), h6CoordStrings.size());

	for (size_t i = 0; i < h6Coords.size(); i++)
		CPPUNIT_ASSERT_EQUAL(h6Coords[i].toString(), h6CoordStrings[i]);
}

void HexagonTest::testCoordFromString()
{
	CPPUNIT_ASSERT_EQUAL(h6Coords.size(), h6CoordStrings.size());

	for (size_t i = 0; i < h6Coords.size(); i++)
		CPPUNIT_ASSERT_EQUAL(HexCoordinate<6>(h6CoordStrings[i]), h6Coords[i]);
}

void HexagonTest::testDistanceOrthogonal()
{
	static const set<pair<pair<int, int>, int>> distances {
		{{0, 1}, 2},
		{{0, 2}, 5},
		{{0, 3}, 3},

		{{1, 2}, -1},
		{{0, 4}, -1},

		{{0, 0}, 0}
	};

	for (const auto& distDataSet : distances)
	{
		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.first].getDistanceOrthogonal(h6Coords[distDataSet.first.second]))
		);

		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.second].getDistanceOrthogonal(h6Coords[distDataSet.first.first]))
		);
	}
}

void HexagonTest::testDistanceDiagonal()
{
	static const set<pair<pair<int, int>, int>> distances {
		{{0, 4}, 1},
		{{0, 5}, 2},
		{{0, 6}, 3},

		{{0, 1}, -1},
		{{1, 2}, -1},

		{{0, 0}, 0}
	};

	for (const auto& distDataSet : distances)
	{
		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.first].getDistanceDiagonal(h6Coords[distDataSet.first.second]))
		);

		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.second].getDistanceDiagonal(h6Coords[distDataSet.first.first]))
		);
	}
}

void HexagonTest::testDistanceHexagonalLine()
{
	static const set<pair<pair<int, std::pair<int, int>>, int>> distances {
		{{0, {3, 5}}, -1},
		{{3, {0, 5}}, -1},

		{{0, {0, 0}}, 0},
	};

	for (const auto& distDataSet : distances)
	{
		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.first].getDistanceHexagonalLine(
				h6Coords[distDataSet.first.second.first], h6Coords[distDataSet.first.second.second]
			))
		);

		CPPUNIT_ASSERT_EQUAL(
			distDataSet.second,
			int(h6Coords[distDataSet.first.first].getDistanceHexagonalLine(
				h6Coords[distDataSet.first.second.second], h6Coords[distDataSet.first.second.first]
			))
		);
	}
}

void HexagonTest::testDirectionOrthogonal()
{
	static const set<pair<pair<int, int>, DirectionOrthogonal>> directions {
		{{0, 2}, DirectionOrthogonal::TOP_RIGHT},
		{{0, 1}, DirectionOrthogonal::LEFT},
		{{0, 3}, DirectionOrthogonal::BOTTOM_RIGHT},

		{{0, 0}, DirectionOrthogonal::NONE},

		{{0, 4}, DirectionOrthogonal::NONE},
		{{1, 2}, DirectionOrthogonal::NONE},
		{{5, 6}, DirectionOrthogonal::NONE},
	};

	auto opposite = [](DirectionOrthogonal val) {
		switch(val)
		{
			case DirectionOrthogonal::TOP_RIGHT:    return DirectionOrthogonal::BOTTOM_LEFT;
			case DirectionOrthogonal::RIGHT:        return DirectionOrthogonal::LEFT;
			case DirectionOrthogonal::BOTTOM_RIGHT: return DirectionOrthogonal::TOP_LEFT;
			case DirectionOrthogonal::BOTTOM_LEFT:  return DirectionOrthogonal::TOP_RIGHT;
			case DirectionOrthogonal::LEFT:         return DirectionOrthogonal::RIGHT;
			case DirectionOrthogonal::TOP_LEFT:     return DirectionOrthogonal::BOTTOM_RIGHT;
			default:                                return DirectionOrthogonal::NONE;
		}
	};

	for (const auto& dirDataSet : directions)
	{
		CPPUNIT_ASSERT_EQUAL(
			dirDataSet.second,
			h6Coords[dirDataSet.first.first].getDirectionOrthogonal(h6Coords[dirDataSet.first.second])
		);

		CPPUNIT_ASSERT_EQUAL(
			opposite(dirDataSet.second),
			h6Coords[dirDataSet.first.second].getDirectionOrthogonal(h6Coords[dirDataSet.first.first])
		);
	}
}

void HexagonTest::testDirectionDiagonal()
{
	static const set<pair<pair<int, int>, DirectionDiagonal>> directions {
		{{0, 5}, DirectionDiagonal::TOP},
		{{0, 6}, DirectionDiagonal::BOTTOM_RIGHT},
		{{0, 7}, DirectionDiagonal::TOP_RIGHT},

		{{0, 0}, DirectionDiagonal::NONE},

		{{0, 1}, DirectionDiagonal::NONE},
		{{1, 2}, DirectionDiagonal::NONE},
		{{4, 5}, DirectionDiagonal::NONE},
	};

	auto opposite = [](DirectionDiagonal val) {
		switch(val)
		{
			case DirectionDiagonal::TOP:          return DirectionDiagonal::BOTTOM;
			case DirectionDiagonal::BOTTOM_RIGHT: return DirectionDiagonal::TOP_LEFT;
			case DirectionDiagonal::BOTTOM_LEFT:  return DirectionDiagonal::TOP_RIGHT;
			case DirectionDiagonal::BOTTOM:       return DirectionDiagonal::TOP;
			case DirectionDiagonal::TOP_LEFT:     return DirectionDiagonal::BOTTOM_RIGHT;
			case DirectionDiagonal::TOP_RIGHT:    return DirectionDiagonal::BOTTOM_LEFT;
			default:                              return DirectionDiagonal::NONE;
		}
	};

	for (const auto& dirDataSet : directions)
	{
		CPPUNIT_ASSERT_EQUAL(
			dirDataSet.second,
			h6Coords[dirDataSet.first.first].getDirectionDiagonal(h6Coords[dirDataSet.first.second])
		);

		CPPUNIT_ASSERT_EQUAL(
			opposite(dirDataSet.second),
			h6Coords[dirDataSet.first.second].getDirectionDiagonal(h6Coords[dirDataSet.first.first])
		);
	}
}
