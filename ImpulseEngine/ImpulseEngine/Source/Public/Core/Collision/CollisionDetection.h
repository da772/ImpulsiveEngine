#pragma once

namespace GEngine {

	class Collider;

	class CollisionDetection {

	public:
		static void CheckCollision();

		static void AddCollider(Ref<Collider> collider);
		static void RemoveCollider(Ref<Collider> collider);

	private:
		
		static std::unordered_set<Ref<Collider>> s_GameColliders;
		static std::unordered_set<Ref<Collider>> s_GameColliders_dynamic;
		static std::unordered_set<Ref<Collider>> s_uiColliders;
		static std::unordered_set<Ref<Collider>> s_uiColliders_dynamic;

	};





}
