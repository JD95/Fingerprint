#pragma once

#include <tuple>
#include <functional>
#include <memory>
#include <iostream>
#include "nodes.h"

struct Updater {

	virtual void calculate() {}
	virtual void update() {}
	virtual ~Updater() {};
};

template <class DependentT, class ...Ts>
struct Dependency : Updater {

	std::shared_ptr<MorphNode<DependentT, Ts...>> m;

	Set<DependentT> target;
	DependentT new_value;

public:

	Dependency(Set<DependentT> t, std::shared_ptr<MorphNode<DependentT, Ts...>> f)
		: m(f), target(t) {}

	~Dependency() {}

	void calculate() {
		new_value = m.get()->apply();
		//std::cout << "Calculated: " << new_value << std::endl;
	}

	void update() {
		target.update(new_value);
	}
};

template <class ReturnT, class ...Ts>
struct Morph {

	std::shared_ptr<MorphNode<ReturnT, Ts...>> morph;

public:

	Morph(Morphism<ReturnT(Ts...)> f, std::shared_ptr<SourceNode<Ts...>> vs)
	{
		morph = std::make_shared<MorphNode<ReturnT, Ts...>>(MorphNode<ReturnT, Ts...>( f, vs ));
	}



	ReturnT invoke() {
		return morph.get()->apply();
	}

	// determine
	template <class T>
	std::unique_ptr<Updater> determine(Reactive<T>& target) {
		return std::move(std::unique_ptr<Updater>(new Dependency<T, Ts...>(target.setter(), morph)));
	}
};


template <class ...Ts>
struct Sources {

	std::shared_ptr<SourceNode<Ts...>> sources;

public:

	Sources(Get<Ts> ...vs) {
		sources = std::make_shared<SourceNode<Ts...>>(std::make_tuple(vs...));
	}

	// Inputs to function must match sources
	template <class F>
	auto use(F f) -> Morph<decltype(apply_from_sources(f, *sources)), Ts...> {

		// Determine return type by type checking the application of function
		using ReturnT = decltype(apply_from_sources(f, *sources));

		// Function which takes in NORMAL values
		Morphism<ReturnT(Ts...)> m = f;

		// Forward the new Morph so auto will work
		return Morph<ReturnT, Ts...>(m, sources);
	}
};

template <class ...Ts>
Sources<Ts...> from(Reactive<Ts>&... sources) {
	return Sources<Ts...>((sources.getter())...);
}




