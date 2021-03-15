#include "gepch.h"

#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Collision/Collider.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Application/Components/QuadColliderComponent.h"
#include "Public/Core/Util/GEMath.h"
#include "Public/Core/Application/Application.h"

namespace GEngine {

	std::vector<Collider*> CollisionDetection::s_uiColliders;
	
	std::unordered_set<Collider*> CollisionDetection::s_GameColliders;
	std::unordered_set<Collider*> CollisionDetection::s_GameColliders_dynamic;
	Collider* CollisionDetection::s_lastUICollision = nullptr;
	bool CollisionDetection::s_lastCollider;
	std::mutex CollisionDetection::s_uiMutex;
	

	void CollisionDetection::CheckCollision()
	{
		unordered_set<Collider*> cList;

		for (Collider* c : s_GameColliders_dynamic) {
			{
				for (Collider* lastC : c->GetLastCollide()) {
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
			for (Collider* _c : s_GameColliders) {
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

	std::vector<Collider*> CollisionDetection::CheckPoint(const float x, const float y)
	{
		std::vector<Collider*> cList;
		for (Collider* c : s_GameColliders) {
			if (c->CheckCollisionPoint(x, y)) {
				cList.push_back(c);
			}
		}
		return cList;
	}

	std::vector<GEngine::QuadColliderComponent*> CollisionDetection::CheckPointComponent(const float x, const float y)
	{
		std::vector<QuadColliderComponent*> v;
		for (Collider* c : s_GameColliders) {
			if (c->CheckCollisionPoint(x, y)) {
				v.push_back(dynamic_cast<QuadColliderComponent*>(c->GetComponent()));
			}
		}		
		return v;
	}

	bool CollisionDetection::CheckPointUI(const float x, const float y)
	{
		bool b = false;
		float _x = GEMath::MapRange(x / Application::GetWidth(), 0, 1, -1, 1);
		float _y = -GEMath::MapRange(y / Application::GetHeight(), 0, 1, -1, 1);
		std::lock_guard<std::mutex> guard(s_uiMutex);
		for (Collider* c : s_uiColliders) {
			if (c->CheckCollisionPoint(_x, _y)) {
				return true;
			}
		}
		return b;
	}

	void CollisionDetection::AddCollider(Collider* collider)
	{
		if (collider->GetColliderLayer() == EColliderLayer::UI) {
			std::lock_guard<std::mutex> guard(s_uiMutex);
			s_uiColliders.push_back(collider);
			std::sort(s_uiColliders.begin(), s_uiColliders.end(), [](const Collider* l, const Collider* r) { return l->GetPosition().z < l->GetPosition().z; });
		}
		else if (collider->GetColliderLayer() == EColliderLayer::Game) {
			s_GameColliders.insert(collider);
			if (collider->GetColliderType() == EColliderType::Dynamic) {
				s_GameColliders_dynamic.insert(collider);
			}
		}
	}

	void CollisionDetection::RemoveCollider(Collider* collider)
	{
		if (collider->GetColliderLayer() == EColliderLayer::UI) {
			std::lock_guard<std::mutex> guard(s_uiMutex);
			std::vector<Collider*>::iterator it = std::find(s_uiColliders.begin(), s_uiColliders.end(), collider);
			s_uiColliders.erase(it);
			return;
		}
		std::unordered_set<Collider*>::iterator it = s_GameColliders.find(collider);
		
		if (it != s_GameColliders.end()) {
			s_GameColliders.erase(it);
			if (collider->GetColliderType() == EColliderType::Dynamic) {
				std::unordered_set<Collider*>::iterator it2 = s_GameColliders_dynamic.find(collider);
				s_GameColliders_dynamic.erase(it2);
			}
		}

	}

	GEngine::Collider* CollisionDetection::InteractionUI(const float x, const float y)
	{
		{

			std::lock_guard<std::mutex> guard(s_uiMutex);
			Collider* lastC = s_lastUICollision;
			for (Collider* c : s_uiColliders) {
				if (c->CheckCollisionPoint(x, y)) {
					
					if (!lastC)
						s_lastCollider = false;
					//GE_CORE_DEBUG("COLLIDE: Mouse Pos: ({0}, {1}) - Collider: ({2},{3}), ({4},{5})",
					//	x, y, c->GetPosition().x, c->GetPosition().y, c->GetScale().x, c->GetScale().y);
					if (!s_lastCollider || c != lastC) {
							c->UIMouseCollideStart(x,y);
							if (s_lastCollider && lastC && s_lastUICollision != c)
								lastC->UIMouseCollideEnd(x,y);
							s_lastUICollision = c;
							s_lastCollider = true;
						return c;
					}
					
					return c;
				}
			}
		}

		Collider* lastC = s_lastUICollision;
		if (s_lastCollider) {
			if (lastC) lastC->UIMouseCollideEnd(x,y);
			s_lastCollider = false;
			s_lastUICollision = nullptr;
		}
		return nullptr;
	}

	GEngine::Collider* CollisionDetection::InteractionEndUI(const float x, const float y)
	{

        std::lock_guard<std::mutex> guard(s_uiMutex);
        if (s_lastCollider) {
            if (s_lastUICollision)
                s_lastUICollision->UIMouseCollideEnd(x,y);
			s_lastUICollision = nullptr;
            s_lastCollider = false;
        }
		return nullptr;
	}

	bool CollisionDetection::CheckLastUI(Collider* c)
	{
		return s_lastUICollision == c;
	}

	void CollisionDetection::OnEvent(const Event& e)
	{
		if (s_lastCollider && s_lastUICollision)
			s_lastUICollision->UIOnEvent(e);
		else if (!s_lastCollider) {
			s_lastUICollision = nullptr;
			s_lastCollider = false;
		}
			
	}

	void CollisionDetection::Reset()
	{
		s_lastCollider = false;
		s_lastUICollision = nullptr;
	}

	bool UIComparator::operator()(Collider*left, Collider* right) const
	{
		return left->GetPosition().z < right->GetPosition().z;
	}

}
