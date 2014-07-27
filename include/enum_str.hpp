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

#ifndef _CYVMATH_ENUM_STR_HPP_
#define _CYVMATH_ENUM_STR_HPP_

#include <initializer_list>
#include <map>
#include <string>
#include <cassert>

template <typename T>
using initMap = std::initializer_list<std::pair<const T, std::string>>;

template <typename EnumT>
std::map<std::string, const EnumT> swapFirstSecond(initMap<EnumT> orig)
{
	std::map<std::string, const EnumT> ret;
	for(const std::pair<const EnumT, std::string>& it : orig)
		ret.emplace(it.second, it.first);

	return ret;
}

#define ENUM_STR_PROT(type) \
	std::string type ## ToStr(type e); \
	type StrTo ## type(const std::string& s);

#define ENUM_STR(type, init, default_val) \
	inline std::string type ## ToStr(type e) \
	{ \
		static std::map< type , std::string> data(initMap< type > init); \
		\
		auto it = data.find(e); \
		if(it == data.end()) \
		{ \
			assert(e != default_val); \
			return type ## ToStr(default_val); \
		} \
		\
		return it->second; \
	}\
	\
	inline type StrTo ## type(const std::string& s) \
	{ \
		static std::map<std::string, const type > data(swapFirstSecond(initMap< type > init)); \
		\
		auto it = data.find(s); \
		if(it == data.end()) \
			return default_val; \
		\
		return it->second; \
	}

#endif // _CYVMATH_ENUM_STR_HPP_
