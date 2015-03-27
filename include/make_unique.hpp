/* Copyright 2014 Jonas Platte
 *
 * This file is NO part of Cyvasse Online or cyvasse-common.
 *
 * Use it however you want to.
 */

#ifndef _MAKE_UNIQUE_HPP_
#define _MAKE_UNIQUE_HPP_

#include <memory>

// copy a std::unique_ptr's managed object (must be copy-constructible, no deep-copy)
template<class T>
inline std::unique_ptr<T> make_unique(const std::unique_ptr<T>& orig)
{
	return orig ? make_unique<T>(*orig) : nullptr;
}

// copy a std::shared_ptr's managed object (must be copy-constructible, no deep-copy)
template<class T>
inline std::unique_ptr<T> make_unique(const std::shared_ptr<T>& orig)
{
	return orig ? make_unique<T>(*orig) : nullptr;
}

#endif // _MAKE_UNIQUE_HPP_
