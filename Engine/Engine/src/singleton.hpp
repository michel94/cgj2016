#pragma once
#include <string>

using namespace std;

template<class C>
class Singleton {
public:
	static C& instance() {
		static C c;	// Guaranteed to be destroyed.
					// Instantiated on first use.
		return c;
	}
};

template<class C, typename Item>
class Manager {
public:
	static C& instance() {
		static C c;	// Guaranteed to be destroyed.
					// Instantiated on first use.
		return c;
	}

	virtual Item loadItem(string name) = 0;

	Item getItem(string name) {
		if (cache.find(name) == cache.end())
			cache[name] = loadItem(name);
		return cache[name];
	}

protected:
	map<string, Item> cache;
};
