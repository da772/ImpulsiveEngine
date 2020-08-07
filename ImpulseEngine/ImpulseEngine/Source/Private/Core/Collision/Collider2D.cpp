#include "gepch.h"

#include "Public/Core/Collision/Collider2D.h"

namespace GEngine {

	Collider2D::Collider2D(glm::vec2 position, glm::vec2 scale, float rotation)
	{
		SetPosition(glm::vec3(position.x, position.y, 1));
		SetRotation(glm::vec3(0, 0, rotation));
		SetScale(glm::vec3(scale.x, scale.y, 1));
	}

	bool Collider2D::CheckCollision(Ref<Collider> collider)
	{
		EColliderShape shape = GetColliderShape();
		EColliderShape otherShape = collider->GetColliderShape();
		Ref<Collider2D> _collider = dynamic_pointer_cast<Collider2D>(collider);

		switch (otherShape) {
			case Quad: {
				if (shape == Quad) return AABBCheck(_collider);
				break;
			}
			case Triangle: {
				GE_CORE_ASSERT(false, "TRIANGLE NOT IMPLEMENTED");
				break;
			}
			
			case Cirlce: {

				GE_CORE_ASSERT(false, "CIRLCE NOT IMPLEMENTED");
				break;
			}
			
			case Convex: {
				break;
			}
			default: {
				GE_CORE_ASSERT(false, "INVALID COLLIDER TYPE");
				break;
			}

		}

		return false;

	}

	bool Collider2D::AABBCheck(Ref<Collider2D> collider)
	{
		glm::vec3 _position = collider->GetPosition();
		glm::vec3 _scale = collider->GetScale();
		bool b = position.x < _position.x + _scale.x &&
			position.x + scale.x > _position.x &&
			position.y < _position.y + _scale.y &&
			position.y + scale.y > _position.y;


		//GE_CORE_DEBUG("AABB POS CHECK {0} : ({1},{2},{3}) - ({4}, {5}, {6})", b, position.x, position.y, position.z, _position.x, _position.y, _position.z);
		//GE_CORE_DEBUG("AABB SCALE CHECK {0} : ({1},{2},{3}) - ({4}, {5}, {6})", b, scale.x, scale.y, scale.z, _scale.x, _scale.y, _scale.z);
		return b;
		
	}

}