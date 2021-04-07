#pragma once


#ifndef GE_FACTORY_INCLUDE
#define GE_FACTORY_INCLUDE
#include "Public/Core/Util/Time.h"
#include "Public/Core/Util/Utility.h"
#if 1

namespace GEngine {

	namespace Factory {
		inline unsigned long counter = 0;
		inline std::unordered_set<uint64_t> hashes;
		inline uint64_t GE_API NextHash() {
			char ch[8];
			Utility::GenerateHash(ch, 8);
			uint64_t h = 0;
			memcpy(&h, ch, sizeof(char) * 8);
			while (Factory::hashes.find(h) != Factory::hashes.end()) {
				Utility::GenerateHash(ch, 8);
				memcpy(&h, ch, sizeof(char) * 8);
			}
			return h;
		}
		inline std::string GE_API HashToString(const uint64_t& h) {
			char ch[9];
			memcpy(ch, &h, sizeof(uint64_t));
			ch[8] = 0;
			return std::string(ch);
		}
		inline void GE_API RemoveHash(uint64_t hash) { if (hashes.size() > 0 &&  hashes.find(hash) != hashes.end()) hashes.erase(hash); };
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