#ifndef TEMPLATE_UTIL_H
#define TEMPLATE_UTIL_H

#include <tuple>


template<int ...>
struct Sequence { };

template<int N, int...S>
struct SequenceGen : SequenceGen<N - 1, N - 1, S...> { };

template<int ...S>
struct SequenceGen<0, S...> {
	typedef Sequence<S...> type;
};


template<int I>
using IntConst = std::integral_constant<int, I>;


/**
 * ContainerTuple is like a tuple except instead of holding a value for
 * each type it holds a vector for each type instead.
 * So for example:
 *   ContainerTuple<std::vector, int, float>::type
 *   is the same as
 *   std::tuple<std::vector<int>, std::vector<float>>
 */
template<template<typename...> class Container, typename... V>
struct ContainerTuple {
private:
	template<typename Value>
	static std::tuple<Container<Value>> genTuple(std::false_type) {
		return std::tuple<Container<Value>>();
	}
	template<typename Value, typename... Values>
	static auto genTuple(std::true_type = std::true_type()) {
		typename std::conditional<(sizeof...(Values) > 1), std::true_type, std::false_type>::type pah;
		return std::tuple_cat(std::tuple<Container<Value>>(), genTuple<Values...>(pah));
	}

public:
	typedef decltype(genTuple<V...>()) type;
};

/**
 * This is a function used to generate a ContainerTuple from another tuple.
 * If you had a std::tuple<int, bool> and wanted a std::tuple<std::vector<int>, std::vector<bool>>
 * you would use decltype(GenContainerTuple<std::vector>(tuple)).
 */
template<template<typename...> class Container, typename... V>
auto GenContainerTuple(std::tuple<V...> tuple) {
	typename ContainerTuple<Container, V...>::type pah;
	return pah;
}
template<template<typename...> class Container, class Tuple>
using ContainerTupleT = decltype(GenContainerTuple<Container>(Tuple()));


/**
 * PointerTuple puts it all in pointers.
 * Same as ContainerTuple but with pointers.
 */
template<typename...V>
struct PointerTuple {
private:
	template<typename Value>
	static std::tuple<Value*> genTuple(std::false_type) {
		return std::tuple<Value*>();
	}
	template<typename Value, typename... Values>
	static auto genTuple(std::true_type = std::true_type()) {
		typename std::conditional<(sizeof...(Values) > 1), std::true_type, std::false_type>::type pah;
		return std::tuple_cat(std::tuple<Value*>(), genTuple<Values...>(pah));
	}

public:
	typedef decltype(genTuple<V...>()) type;
};

template<typename... V>
auto GenPointerTuple(std::tuple<V...> tuple) {
	typename PointerTuple<V...>::type pah;
	return pah;
}
template<class Tuple>
using PointerTupleT = decltype(GenPointerTuple(Tuple()));

#endif // TEMPLATE_UTIL_H
