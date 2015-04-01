#ifndef _ENUM_STR_HPP_
#define _ENUM_STR_HPP_

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>

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

#define ENUM_STR(type, init) \
	inline std::string type ## ToStr(type e) \
	{ \
		static std::map< type , std::string> data(initMap< type > init); \
		\
		auto it = data.find(e); \
		if(it == data.end()) \
			throw std::invalid_argument("Invalid " #type ": " + std::to_string(static_cast<int>(e))); \
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
			throw std::invalid_argument("Invalid " #type ": " + s); \
		\
		return it->second; \
	}

#endif // _ENUM_STR_HPP_
