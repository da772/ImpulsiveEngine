#pragma once
#ifndef GE_FACTORY_INCLUDE
#define GE_FACTORY_INCLUDE
#include "Public/Core/Util/Time.h"
#include "Public/Core/Util/Utility.h"
#if 0

namespace GEngine {

	namespace Factory {
		static unsigned long counter = 0;
		static uint64_t NextHash() {
			return ++Factory::counter;
		}
		static std::unordered_set<uint64_t> hashes;
		static void RemoveHash(uint64_t hash) { if (hashes.size() > 0 &&  hashes.find(hash) != hashes.end()) hashes.erase(hash); };
	}

	template<class E, typename ... Args>
	static inline std::shared_ptr<E> CreateGameObject(Args&& ... args) {
		std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
		e->self = e;
		char ch[8];
		Utility::GenerateHash(ch, 8);
		uint64_t h = 0;
		memcpy(&h, ch, sizeof(char) * 8);

		while (Factory::hashes.find(h) != Factory::hashes.end()) {
			Utility::GenerateHash(ch, 8);
			memcpy(&h, ch, sizeof(char) * 8);
		}

		e->hash = h;
		return e;
	};

	template<class E, typename ... Args>
	static inline std::shared_ptr<E> CreateGameObject_ID(uint64_t hash, Args&& ... args) {
		std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
		e->self = e;
		if (hash == 0) {
			hash = Time::GetEpochTimeNS();
		}
		e->hash = hash;
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
#endif