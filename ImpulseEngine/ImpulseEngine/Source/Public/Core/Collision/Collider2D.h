#pragma once
#include "Public/Core/Collision/Collider.h"

namespace GEngine {



	class Collider2D : public Collider {
	public:
		
		Collider2D(const glm::vec2& position, const glm::vec2& scale, float rotation);
		~Collider2D() {};
		virtual bool CheckCollisionPoint(float x, float y) override;
		virtual bool CheckCollision(Ref<Collider> collider) override; 


	protected:
		Collider2D();
		bool AABBCheck(Ref<Collider2D> collider);
		bool AABBPointCheck(const float x,const float y);




	};

}