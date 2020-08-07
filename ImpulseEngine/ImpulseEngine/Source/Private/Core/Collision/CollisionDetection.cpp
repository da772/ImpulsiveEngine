#include "gepch.h"

#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Collision/Collider.h"

namespace GEngine {

	unordered_set<Ref<Collider>> CollisionDetection::s_uiColliders;
	unordered_set<Ref<Collider>> CollisionDetection::s_uiColliders_dynamic;
	unordered_set<Ref<Collider>> CollisionDetection::s_GameColliders;
	unordered_set<Ref<Collider>> CollisionDetection::s_GameColliders_dynamic;

	void CollisionDetection::CheckCollision()
	{

		for (Ref<Collider> c : s_uiColliders_dynamic) {
			{
				for (Weak<Collider> _lastC : c->GetLastCollide()) {
					Ref<Collider> lastC = _lastC.lock();
					if (lastC != nullptr) {
						if (c->CheckCollision(lastC)) {
							continue;
						}
						else {
							c->RemoveLastCollide(lastC);
							lastC->RemoveLastCollide(lastC);
							c->CollideEnd(lastC);
							lastC->CollideEnd(c);
						}
					}
				}
			}
			for (Ref<Collider> _c : s_uiColliders) {
				if (c != _c) {
					if (c->CheckCollision(_c)) {
						c->AddLastCollide(_c);
						_c->AddLastCollide(c);
						c->CollideStart(_c);
						_c->CollideStart(c);
					}
				}
			}
		}

		unordered_set<Ref<Collider>> cList;

		for (Ref<Collider> c : s_GameColliders_dynamic) {
			{
				for (Weak<Collider> _lastC : c->GetLastCollide()) {
					Ref<Collider> lastC = _lastC.lock();
					if (lastC != nullptr) {
						if (c->CheckCollision(lastC)) {
							cList.insert(lastC);
							continue;
						}
						else {
							c->RemoveLastCollide(lastC);
							lastC->RemoveLastCollide(lastC);
							c->CollideEnd(lastC);
							lastC->CollideEnd(c);
						}
					}
				}
			}
			for (Ref<Collider> _c : s_GameColliders) {
				if ( c != _c && cList.find(_c) == cList.end()) {
					if (c->CheckCollision(_c)) {
						c->AddLastCollide(_c);
						_c->AddLastCollide(c);
						c->CollideStart(_c);
						_c->CollideStart(c);
					}
				}
			}
		}

	}

	void CollisionDetection::AddCollider(Ref<Collider> collider)
	{
		if (collider->GetColliderLayer() == UI) {
			s_uiColliders.insert(collider);
			if (collider->GetColliderType() == Dynamic) {
				s_uiColliders_dynamic.insert(collider);
			}
		}
		else if (collider->GetColliderLayer() == Game) {
			s_GameColliders.insert(collider);
			if (collider->GetColliderType() == Dynamic) {
				s_GameColliders_dynamic.insert(collider);
			}
		}
	}

	void CollisionDetection::RemoveCollider(Ref<Collider> collider)
	{
		std::unordered_set<Ref<Collider>>::iterator it = collider->GetColliderLayer() == UI ? s_uiColliders.find(collider)  : s_GameColliders.find(collider);

		if (it != ( collider->GetColliderLayer() == UI ? s_uiColliders.end() : s_GameColliders.end())) {
			collider->GetColliderLayer() == UI ? s_uiColliders.erase(it) : s_GameColliders.erase(it);
			if (collider->GetColliderType() == Dynamic) {
				std::unordered_set<Ref<Collider>>::iterator it2 = collider->GetColliderLayer() == UI ? s_uiColliders_dynamic.find(collider) : s_GameColliders_dynamic.find(collider);
				collider->GetColliderLayer() == UI ? s_uiColliders_dynamic.erase(it2) : s_GameColliders_dynamic.erase(it2);
			}
		}

	}

}