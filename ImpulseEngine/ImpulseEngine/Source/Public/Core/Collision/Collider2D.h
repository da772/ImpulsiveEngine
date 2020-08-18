#pragma once
#include "Public/Core/Collision/Collider.h"

namespace GEngine {



	class Collider2D : public Collider {
	public:
		Ref<Collider2D> Create();
		~Collider2D() {};
		virtual bool CheckCollisionPoint(float x, float y) override;
		virtual bool CheckCollision(Ref<Collider> collider) override; 


	protected:
		Collider2D();
		bool AABBCheck(Ref<Collider2D> collider);
		bool AABBPointCheck(float x, float y);




	};

}