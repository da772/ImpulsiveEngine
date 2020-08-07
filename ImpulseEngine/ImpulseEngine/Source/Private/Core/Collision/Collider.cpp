#include "gepch.h"

#include "Public/Core/Collision/Collider.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"

namespace GEngine {

	void Collider::CollideStart(Ref<Collider> collider)
	{
		OnCollision(collider);
		if (m_collisionFunctionStart)
			m_collisionFunctionStart(collider);
	}

	void Collider::CollideEnd(Ref<Collider> collider)
	{
		OnCollisionEnd(collider);
		if (m_collisionFunctionEnd)
			m_collisionFunctionEnd(collider);
	}

	GEngine::Weak<GEngine::Entity> Collider::GetEntity()
	{
		return entity;
	}

	GEngine::Weak<GEngine::Component> Collider::GetComponent()
	{
		return component;
	}

	void Collider::SetEntity(Weak<Entity> e)
	{
		entity = e;
	}

	void Collider::SetComponent(Weak<Component> c)
	{
		component = c;
	}

	void Collider::AddLastCollide(Weak<Collider> c)
	{
		Ref<Collider> __c = c.lock();
		for (Weak<Collider> _c : m_lastCollide) {
			if (_c.lock() == __c)
				return;
		}
		m_lastCollide.push_back(c);
	}

	void Collider::RemoveLastCollide(Weak<Collider> c)
	{
		std::vector<Weak<Collider>>::iterator it = m_lastCollide.begin();
		Ref<Collider> _c = c.lock();
		while (it != m_lastCollide.end()) {
			if (it->lock() == _c) {
				m_lastCollide.erase(it);
				return;
			}
			it++;
		}
		
	}

}