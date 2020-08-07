#pragma once
#ifndef GE_FACTORY_INCLUDE
#define GE_FACTORY_INCLUDE
#include "Public/Core/Util/Time.h"

namespace GEngine {

	namespace Factory {
		static unsigned long counter = 0;
		static uint64_t NextHash() {
			return ++Factory::counter;
		}
	}

	template<class E, typename ... Args>
	static inline std::shared_ptr<E> CreateGameObject(Args&& ... args) {
		std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
		e->self = e;
		std::hash<long long> hash;
		e->hash = hash(Time::GetEpochTimeNS());
		return e;
	};


	template<class E, typename ... Args>
	static inline std::shared_ptr<E> CreateGameObjectServer(Args&& ... args) {
		std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
		e->self = e;
		std::hash<std::shared_ptr<E>> hsh;
		e->hash = hsh(e);
		return e;
	};

	template<class E, typename ... Args>
	static inline std::shared_ptr<E> CreateGameObjectClient(uint64_t hash, Args&& ... args) {
		std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
		e->self = e;
        e->hash = hash;
		return e;
	};






}

#endif
