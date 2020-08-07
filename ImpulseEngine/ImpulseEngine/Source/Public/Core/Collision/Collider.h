#pragma once

namespace GEngine {

	enum EColliderShape {
		Quad = 0,
		Triangle,
		Cirlce,
		Convex

	};

	enum EColliderLayer {
		UI = 0,
		Game
	};

	enum EColliderType {
		Static = 0,
		Dynamic
	};

	class Entity;
	class Component;

	class Collider {
	public:
		Collider() {};
		~Collider() {};
		virtual bool CheckCollision(Ref<Collider> collider) { return false; };
		
		inline const EColliderShape GetColliderShape() const { return m_shape; }
		inline void SetColliderShape(EColliderShape shape) { m_shape = shape; }
		inline const EColliderLayer GetColliderLayer() const { return m_layer; }
		inline void SetColliderLayer(EColliderLayer shape) { m_layer = shape; }
		inline EColliderType  GetColliderType() const { return m_type; }
		inline void SetColliderType(EColliderType shape) { m_type = shape; }

		inline void SetCollisionStartFunction(std::function<void(Ref<Collider>)> func) { m_collisionFunctionStart = func; }
		inline void SetCollisionEndFunction(std::function<void(Ref<Collider>)> func) { m_collisionFunctionEnd = func; }


		void CollideStart(Ref<Collider> collider);
		void CollideEnd(Ref<Collider> collider);

		const glm::vec3 GetPosition() const { return position; };
		const glm::vec3 GetScale() const { return scale;}
		const glm::vec3 GetRotation() const { return rotation; }

		void SetPosition(glm::vec3 pos) { position = pos; };
		void SetScale(glm::vec3 scale) { this->scale = scale; }
		void SetRotation(glm::vec3 rot) { rotation = rot; }

		Weak<Entity> GetEntity();
		Weak<Component> GetComponent();

		void SetEntity(Weak<Entity> e);
		void SetComponent(Weak<Component> c);

		inline std::vector<Weak<Collider>>& GetLastCollide() { return m_lastCollide; }
		void AddLastCollide(Weak<Collider> c);
		void RemoveLastCollide(Weak<Collider> c);
		
	protected:
		virtual void OnCollision(Ref<Collider> collider) {};
		virtual void OnCollisionEnd(Ref<Collider> collider) {};
		EColliderShape m_shape;
		EColliderType m_type;
		EColliderLayer m_layer;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		std::function<void(Ref<Collider>)> m_collisionFunctionStart;
		std::function<void(Ref<Collider>)> m_collisionFunctionEnd;

		std::vector<Weak<Collider>> m_lastCollide;

		Weak<Entity> entity;
		Weak<Component> component;
		


	};
}