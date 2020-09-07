#pragma once

namespace GEngine {

	class PhysicsBody;

	enum class PhysicsInfoType {
		PHYSICS_Static = 0,
		PHYSICS_Kinematic,
		PHYSICS_Dynamic
	};

	enum class PhysicsShapes {
		PHYSICS_Box = 0,
		PHYSICS_Cirlce
	};


	struct PhysicsParent {
		Weak<PhysicsBody> parent;
	};


	struct PhysicsInfo {

		PhysicsInfoType type = PhysicsInfoType::PHYSICS_Static;
		glm::vec2 position = glm::vec2(0,0);
		float rotation = 0;
		glm::vec2 linearVelocity = glm::vec2(0,0);
		float angularVelocity = 0;
		float linearDamping = 0.f;
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