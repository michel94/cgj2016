#pragma once

template<class C>
class Singleton {
public:
	static C& instance() {
		static C c;	// Guaranteed to be destroyed.
					// Instantiated on first use.
		return c;
	}
};