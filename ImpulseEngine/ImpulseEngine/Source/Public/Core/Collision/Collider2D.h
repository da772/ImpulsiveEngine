#pragma once
#include "Public/Core/Collision/Collider.h"

namespace GEngine {



	class Collider2D : public Collider {
	public:
		Collider2D(glm::vec2 position, glm::vec2 scale, float rotation);
		~Collider2D() {};
		virtual bool CheckCollision(Ref<Collider> collider) override; 


	protected:
		bool AABBCheck(Ref<Collider2D> collider);




	};

}