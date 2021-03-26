#pragma once

namespace GEngine {

	class Collider;

	class QuadColliderComponent;
	class Event;


	struct UIComparator final {
		bool operator() (Collider* left, Collider* right) const;
	};

	class GE_API CollisionDetection {

	public:
		static void CheckCollision();
		static std::vector<Collider*> CheckPoint(const float x, const float y);
		static std::vector<QuadColliderComponent*> CheckPointComponent(const float x, const float y);
		static bool CheckPointUI(const float x, const float y);
		static void AddCollider(Collider* collider);
		static void RemoveCollider(Collider* collider);
		static Collider* InteractionUI(const float x, const float y);
		static Collider* InteractionEndUI(const float x, const float y);
		static bool CheckLastUI(Collider* c);
		static void OnEvent(const Event& e);
		static void Reset();
		
	private:
		static std::unordered_set<Collider*> s_GameColliders;
		static std::unordered_set<Collider*> s_GameColliders_dynamic;
		static std::vector<Collider*> s_uiColliders;
		static Collider* s_lastUICollision;
		static bool s_lastCollider;
		static std::mutex s_uiMutex;

	};





}
