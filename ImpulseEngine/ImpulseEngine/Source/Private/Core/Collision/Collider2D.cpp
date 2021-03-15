#include "gepch.h"

#include "Public/Core/Collision/Collider2D.h"

namespace GEngine {


	Collider2D::Collider2D(const Vector2f& position, const Vector2f& scale, const float rotation)
	{
		SetPosition(Vector3f(position.x, position.y, 1));
		SetRotation(Vector3f(0, 0, rotation));
		SetScale(Vector3f(scale.x, scale.y, 1));
	}

	bool Collider2D::CheckCollisionPoint(const float x, const float y)
	{
		EColliderShape shape = GetColliderShape();
		return AABBPointCheck(x, y);
	}

	bool Collider2D::CheckCollision(Collider* collider)
	{
		EColliderShape shape = GetColliderShape();
		EColliderShape otherShape = collider->GetColliderShape();
		Collider2D* _collider = dynamic_cast<Collider2D*>(collider);

		switch (otherShape) {
			case EColliderShape::Quad: {
				if (shape == EColliderShape::Quad) return AABBCheck(_collider);
				break;
			}
			case EColliderShape::Triangle: {
				GE_CORE_ASSERT(false, "TRIANGLE NOT IMPLEMENTED");
				break;
			}
			
			case EColliderShape::Cirlce: {

				GE_CORE_ASSERT(false, "CIRLCE NOT IMPLEMENTED");
				break;
			}
			
			case EColliderShape::Convex: {
				break;
			}
			default: {
				GE_CORE_ASSERT(false, "INVALID COLLIDER TYPE");
				break;
			}

		}

		return false;

	}

	bool Collider2D::AABBCheck(Collider2D* collider)
	{
		Vector3f _position = collider->GetPosition();
		Vector3f _scale = collider->GetScale();
		float L1 = position.x - scale.x/2.f;
		float R1 = position.x + scale.x/2.f;
		float T1 = position.y + scale.y/2.f;
		float B1 = position.y - scale.y/2.f;

		float L2 = _position.x - _scale.x/2.f;
		float R2 = _position.x + _scale.x/2.f;
		float T2 = _position.y + _scale.y/2.f;
		float B2 = _position.y - _scale.y/2.f;


		bool b = L1 < R2 && R1 > L2 && T1 > B2 && B1 < T2;


		//GE_CORE_DEBUG("AABB POS CHECK {0} : ({1},{2},{3}) - ({4}, {5}, {6})", b, position.x, position.y, position.z, _position.x, _position.y, _position.z);
		//GE_CORE_DEBUG("AABB SCALE CHECK {0} : ({1},{2},{3}) - ({4}, {5}, {6})", b, scale.x, scale.y, scale.z, _scale.x, _scale.y, _scale.z);
		return b;
		
	}

	bool Collider2D::AABBPointCheck(const float x, const float y)
	{
		bool b = x <= position.x + scale.x / 2.f && x >= position.x - scale.x / 2.f
			&& y <= position.y + scale.y/2.f && y >= position.y - scale.y/2.f;
		return b;
	}

}