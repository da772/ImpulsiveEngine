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



	class GE_API PhysicsCollision {
	public:
		PhysicsCollision(Ref<PhysicsBody> b, const std::string& t) : body(b), tag(t) {};
		virtual ~PhysicsCollision() {};
		Ref<PhysicsBody> body;
		std::string tag;
	};

	struct PhysicsParent {
		Weak<PhysicsBody> parent;
		std::function<void(Ref<PhysicsCollision>)> onStartCollide = nullptr;
		std::function<void(Ref<PhysicsCollision>)> onEndCollide = nullptr;
		std::string tag;
	};




	struct GE_API PhysicsInfo {

		PhysicsInfoType type = PhysicsInfoType::PHYSICS_Static;
		Vector2f position = Vector2f(0,0);
		float rotation = 0;
		Vector2f linearVelocity = Vector2f(0,0);
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