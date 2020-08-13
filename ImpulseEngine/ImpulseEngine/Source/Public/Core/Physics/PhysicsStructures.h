#pragma once

namespace GEngine {


	enum PhysicsInfoType {
		PHYSICS_Static = 0,
		PHYSICS_Kinematic,
		PHYSICS_Dynamic
	};

	enum PhysicsShapes {
		PHYSICS_Box = 0,
		PHYSICS_Cirlce
	};


	struct PhysicsInfo {

		PhysicsInfoType type = PHYSICS_Static;
		glm::vec2 position = glm::vec2(0,0);
		float rotation = 0;
		glm::vec2 linearVelocity = glm::vec2(0,0);
		float angularVelocity = 0;
		float linearDamping = .1f;
		float angularDamping = .1f;
		bool canSleep = true;
		bool awake = true;
		bool fixedRotation = false;
		bool bullet = false;
		bool enabled = true;
		void* userData = nullptr;
		float gravityScale = 1.f;


	};






}