#pragma once

#ifndef GE_FACTORY_INCLUDE
#define GE_FACTORY_INCLUDE
#include "Public/Core/Util/Time.h"
#include "Public/Core/Util/Utility.h"
#if 1

#define GE_SIZE_OF_HASH 16

namespace GEngine {
	struct GE_API ObjectHash {

		inline ObjectHash() {}
		inline ObjectHash(const char* d) {
			if (d) {
				memcpy(hash, d, std::min((int)strlen(d), GE_SIZE_OF_HASH));
			}
		}

		inline ObjectHash(const ObjectHash& a) {
			memcpy(hash, a.hash, sizeof(hash));
		}

		const bool operator==(const ObjectHash& rhs) const noexcept {
			return std::strcmp(rhs.hash, hash) == 0;
		}

		ObjectHash& operator=(const ObjectHash& rhs) {
			if (this == &rhs)
				return *this;

			memcpy(hash, rhs.hash, sizeof(hash));
			return *this;
		}

		bool operator==(const int& other) {
			return other == std::strlen(hash);
		}

		ObjectHash& operator=(ObjectHash&& other) {
			if (&other == this) {
				return *this;
			}

			memcpy(hash, other.hash, sizeof(hash));
			return *this;
		}

		std::string ToString() const {
			return std::string(hash);
		}

		size_t operator()(const ObjectHash& obj) const noexcept {
			return std::hash<std::string_view>{}(std::string_view(obj.hash, GE_SIZE_OF_HASH));
		};

		char hash[GE_SIZE_OF_HASH+1] = { 0 };
	};
}


namespace std {
	template<> struct GE_API hash<GEngine::ObjectHash>
	{
		std::size_t operator()(const GEngine::ObjectHash& p) const noexcept
		{
			return p(p);
		}
	};
}

namespace GEngine {
	struct Factory {

		static std::unordered_set<ObjectHash> hashes;

		static inline ObjectHash GE_API NextHash() {
			ObjectHash h = ObjectHash();
			Utility::GenerateHash(h.hash, GE_SIZE_OF_HASH);
			while (Factory::hashes.find(h) != Factory::hashes.end()) {
				Utility::GenerateHash(h.hash, GE_SIZE_OF_HASH);
			}
			return h;
		}
		static inline std::string GE_API HashToString(const ObjectHash& h) {
			char ch[GE_SIZE_OF_HASH + 1] = { 0 };
			memcpy(ch, h.hash, sizeof(h.hash));
			return std::string(ch);
		}
		static inline GE_API void RemoveHash(ObjectHash hash) { if (hashes.size() > 0 && hashes.find(hash) != hashes.end()) hashes.erase(hash); };

		template<class E, typename ... Args>
		static inline std::shared_ptr<E> CreateGameObject(Args&& ... args) {
			std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
			e->self = e;
			GEngine::ObjectHash h;
			Utility::GenerateHash(h.hash, GE_SIZE_OF_HASH);

			while (Factory::hashes.find(h) != Factory::hashes.end()) {
				Utility::GenerateHash(h.hash, GE_SIZE_OF_HASH);
			}

			e->hash = h;
			return e;
		};

		template<class E, typename ... Args>
		static inline std::shared_ptr<E> CreateGameObject_ID(::GEngine::ObjectHash hash, Args&& ... args) {
			std::shared_ptr<E> e = std::make_shared<E>(std::forward<Args>(args)...);
			e->self = e;
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
	};
}

#endif
#endif