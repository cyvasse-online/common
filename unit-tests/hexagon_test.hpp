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

#ifndef _HEXAGON_TEST_HPP_
#define _HEXAGON_TEST_HPP_

#include <cppunit/TestFixture.h>

#include <string>
#include <vector>
#include <cppunit/extensions/HelperMacros.h>
#include <cyvasse/hexagon.hpp>

using namespace cyvasse;

class HexagonTest : public CppUnit::TestFixture
{
	private:
		std::vector<HexCoordinate<6>> h6Coords;
		std::vector<std::string> h6CoordStrings;

	public:
		HexagonTest();

		void testCoordValidity();
		void testCoordEquality();
		void testCoordCompleteness();

		void testCoordToString();
		void testCoordFromString();

		void testDistanceOrthogonal();
		void testDistanceDiagonal();
		void testDistanceHexagonalLine();

		void testDirectionOrthogonal();
		void testDirectionDiagonal();

	CPPUNIT_TEST_SUITE(HexagonTest);
		CPPUNIT_TEST(testCoordValidity);
		CPPUNIT_TEST(testCoordEquality);
		CPPUNIT_TEST(testCoordCompleteness);

		CPPUNIT_TEST(testCoordToString);
		CPPUNIT_TEST(testCoordFromString);

		CPPUNIT_TEST(testDistanceOrthogonal);
		CPPUNIT_TEST(testDistanceDiagonal);
		CPPUNIT_TEST(testDistanceHexagonalLine);

		CPPUNIT_TEST(testDirectionOrthogonal);
		CPPUNIT_TEST(testDirectionDiagonal);
	CPPUNIT_TEST_SUITE_END();
};

#endif // _HEXAGON_TEST_HPP_
