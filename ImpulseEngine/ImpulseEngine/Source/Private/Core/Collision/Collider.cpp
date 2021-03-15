#include "gepch.h"

#include "Public/Core/Collision/Collider.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"

namespace GEngine {

	void Collider::CollideStart(Collider* collider)
	{
		OnCollision(collider);
		if (m_collisionFunctionStart)
			m_collisionFunctionStart(collider);
	}

	void Collider::CollideEnd(Collider* collider)
	{
		OnCollisionEnd(collider);
		if (m_collisionFunctionEnd)
			m_collisionFunctionEnd(collider);
	}

	void Collider::UIMouseCollideStart(float x, float y)
	{
		OnUIMouseCollision(x, y);
		if (m_uiCollisionFunctionStart)
			m_uiCollisionFunctionStart(x,y);
	}

	void Collider::UIMouseCollideEnd(float x, float y)
	{
		OnUIMouseCollisionEnd(x, y);
		if (m_uiCollisionFunctionEnd)
			m_uiCollisionFunctionEnd(x, y);
	}

	GEngine::Entity* Collider::GetEntity()
	{
		return entity;
	}

	GEngine::Component* Collider::GetComponent()
	{
		return component;
	}

	void Collider::SetEntity(Entity* e)
	{
		entity = e;
	}

	void Collider::SetComponent(Component* c)
	{
		component = c;
	}

	void Collider::AddLastCollide(Collider* c)
	{
		for (Collider* _c : m_lastCollide) {
			if (_c== this)
				return;
		}
		m_lastCollide.push_back(c);
	}

	void Collider::RemoveLastCollide(Collider* c)
	{
		std::vector<Collider*>::iterator it = m_lastCollide.begin();
		while (it != m_lastCollide.end()) {
			if (*it == this) {
				m_lastCollide.erase(it);
				return;
			}
			it++;
		}
		
	}

}