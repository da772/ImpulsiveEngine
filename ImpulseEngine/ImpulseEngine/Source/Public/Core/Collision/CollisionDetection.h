#pragma once

namespace GEngine {

	class Collider;

	class QuadColliderComponent;


	struct UIComparator final {
		bool operator() (Ref<Collider> left, Ref<Collider> right) const;
	};

	class CollisionDetection {

	public:
		static void CheckCollision();
		static std::vector<Ref<Collider>> CheckPoint(const float x, const float y);
		static std::vector<Ref<QuadColliderComponent>> CheckPointComponent(const float x, const float y);
		static bool CheckPointUI(const float x, const float y);
		static void AddCollider(Ref<Collider> collider);
		static void RemoveCollider(Ref<Collider> collider);
		static Ref<Collider> InteractionUI(const float x, const float y);
		static Ref<Collider> InteractionEndUI(const float x, const float y);
		static bool CheckLastUI(Ref<Collider> c);
		
	private:
		static std::unordered_set<Ref<Collider>> s_GameColliders;
		static std::unordered_set<Ref<Collider>> s_GameColliders_dynamic;
		static std::vector<Ref<Collider>> s_uiColliders;
		static Weak<Collider> s_lastUICollision;
		static bool s_lastCollider;
		static std::mutex s_uiMutex;

	};





}
