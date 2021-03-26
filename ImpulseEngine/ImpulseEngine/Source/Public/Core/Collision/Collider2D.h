#pragma once
#include "Public/Core/Collision/Collider.h"

namespace GEngine {



	class GE_API Collider2D : public Collider {
	public:
		
		Collider2D(const Vector2f& position, const Vector2f& scale, float rotation);
		~Collider2D() {};
		virtual bool CheckCollisionPoint(float x, float y) override;
		virtual bool CheckCollision(Collider* collider) override; 


	protected:
		Collider2D();
		bool AABBCheck(Collider2D* collider);
		bool AABBPointCheck(const float x,const float y);




	};

}