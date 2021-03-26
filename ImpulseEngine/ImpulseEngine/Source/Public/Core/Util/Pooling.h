#pragma once

namespace GEngine {

	template<class T, class id>
	class GE_API ObjectPool {

	public:
		ObjectPool() {

		}
		
		inline void Add(T* obj, id name) {
			if (m_Objects.find(name) != m_Objects.end()) {
				m_Objects[name].count++;
			}
			else {
				m_Objects[name] = FObjectCount(obj);
			}
		}

		inline bool Remove(id name) {
			GE_CORE_ASSERT(m_Objects.find(name) != m_Objects.end(), "Object not found!");
			if (m_Objects[name].count <= 1) {
				T* obj = m_Objects[name].obj;
				m_Objects.erase(name);
				delete obj;
				return true;
			}
			else {
				m_Objects[name].count--;
				return false;
			}
		}

		inline bool Contains(id name) {
			return m_Objects.find(name) != m_Objects.end();
		}

		inline std::vector<T*> GetObjects() {
			std::vector<T*> v;
				for (auto a : m_Objects) {
					v.push_back(a.second.obj);
				}
				return v;
		}

		inline T* Get(id name) {
			GE_CORE_ASSERT(m_Objects.find(name) != m_Objects.end(), "Shader not found!");
			m_Objects[name].count++;
			return m_Objects[name].obj;
		}
		
		struct FObjectCount {
		public:
			FObjectCount() {};
			inline FObjectCount(T* t) : count(1), obj(t) {

			};

			T* obj;
			long count = 0;
		};


	private:
		std::unordered_map<id, FObjectCount> m_Objects;

	};







}

