#pragma once

#include <iterator>
#include <set>

namespace util
{
	template <typename T>
	bool contains(const std::set<T>& container, const T& value)
	{
		auto it = std::find(container.begin(), container.end(), value);
		return it != container.end();
	}
};

// define some custom container where we do not have some kind of 'value_type'
template <typename... T> class CustomContainer {};

//template <std::iterator<T> I, std::iterator S, class T>
//bool contains(I first, S last, const T& value)
//{
//
//}

// specialization that deduces ContType and NumType from a single template argument
// This works for all continers where the first template argument is the 'value_type'
// also note the Ts... which is a placeholder for possible other template arguments that need to be passed on
template <template <typename...> class ContType, typename NumType, typename... Ts>
double avg_nums(ContType<NumType, Ts...>&)
{
	// so the work
	return NumType(0);
}

