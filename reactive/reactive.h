#pragma once

#include <tuple>

template <class T>
class Get {
	T* source;
public:
	T value() {
		return *source;
	}
	Get() : source(NULL) {}
	Get(T* s) : source(s) {}
	Get(const Get& other) { this->source = other.source; }
};

template <class T>
class Set {
	T* source;
public:
	void update(T new_value) {
		*source = new_value;
	}
	Set() : source(NULL) {}
	Set(T* s) : source(s) {}
	Set(const Set& other) { this->source = other.source; }
};

template <class T>
struct Reactive {
	T v;

	Reactive() {}
	Reactive(T t) { v = t; }
	Reactive(const Reactive<T>& other) { v = other.v; }

	void operator=(const T& t) { v = t; }
	Get<T> getter() { return Get<T>(&v); }
	Set<T> setter() { return Set<T>(&v); }

};

// Borrowed from http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html

template<typename F, typename Tuple, size_t ...S >
decltype(auto) apply_sources_impl(F&& fn, Tuple&& t, std::index_sequence<S...>)
{
	// NOTE: Has been modified from source to expect Get values and dereference them!
	return std::forward<F>(fn)((std::get<S>(std::forward<Tuple>(t)).value())...);
}

template<typename F, typename Tuple>
decltype(auto) apply_from_sources(F&& fn, Tuple&& t)
{
	std::size_t constexpr tSize
		= std::tuple_size<typename std::remove_reference<Tuple>::type>::value;

	return apply_sources_impl(std::forward<F>(fn),
		std::forward<Tuple>(t),
		std::make_index_sequence<tSize>());
}