#pragma once

#include <tuple>
#include <functional>

template <class T>
using Morphism = std::function<T>;

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
struct ReactiveVal {
	T value;

	ReactiveVal(T t) { value = t; }
	ReactiveVal(const ReactiveVal<T>& other) { value = other.value; }

	void operator=(const T& t) { value = t; }
	Get<T> getter() { return Get<T>(&value); }
	Set<T> setter() { return Set<T>(&value); }
	
};


template <class DependentT, class ...Ts>
struct Dependency {

	std::tuple<Get<Ts>...> values;

	Morphism<DependentT(Ts...)> _m;

	Set<DependentT> target;

public:

	Dependency( Set<DependentT> t
			  , Morphism<DependentT(Ts...)> f
			  , std::tuple<Get<Ts>...> vs
	) : target(t), _m(f), values(vs) {}

	void update() {
		target.update(apply_from_tuple(_m, values));
	}
};

// Borrowed from http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html

template<typename F, typename Tuple, size_t ...S >
decltype(auto) apply_tuple_impl(F&& fn, Tuple&& t, std::index_sequence<S...>)
{
	// NOTE: Has been modified from source to expect Get values and dereference them!
	return std::forward<F>(fn)((std::get<S>(std::forward<Tuple>(t)).value())...);
}

template<typename F, typename Tuple>
decltype(auto) apply_from_tuple(F&& fn, Tuple&& t)
{
	std::size_t constexpr tSize
		= std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
	return apply_tuple_impl(std::forward<F>(fn),
		std::forward<Tuple>(t),
		std::make_index_sequence<tSize>());
}

template <class ReturnT, class ...Ts>
struct Morph {

	std::tuple<Get<Ts>...> values;

	Morphism<ReturnT(Ts...)> _m;

public:

	Morph(Morphism<ReturnT(Ts...)> f, std::tuple<Get<Ts>...> vs)
		: _m(f), values(vs) {}

	ReturnT invoke() {
		return apply_from_tuple(_m,values);
	}

	// determine
	template <class T>
	Dependency<T, Ts...> determine(ReactiveVal<T>& target) {
		return Dependency<T, Ts...>(target.setter(), _m, values);
	}
};


template <class ...Ts>
struct Sources {

	std::tuple<Get<Ts>...> values;

public:

	Sources(Get<Ts> ...vs) {
		values = std::make_tuple(vs...);
	}

	template <class F>
	auto use(F f) -> Morph<decltype(apply_from_tuple(f, values)), Ts...> {

		// Determine return type by type checking the application of function
		using ReturnT = decltype(apply_from_tuple(f,values));

		// Function which takes in NORMAL values
		Morphism<ReturnT(Ts...)> m = f;

		// Forward the new Morph so auto will work
		return std::forward<Morph<ReturnT, Ts...>>(Morph<ReturnT, Ts...>(m, values));
	}
};

template <class ...Ts>
Sources<Ts...> from(ReactiveVal<Ts>&... sources) {
	return Sources<Ts...>((sources.getter())...);
}




