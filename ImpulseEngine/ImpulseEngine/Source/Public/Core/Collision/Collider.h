#pragma once

namespace GEngine {

	enum class EColliderShape {
		Quad = 0,
		Triangle,
		Cirlce,
		Convex

	};

	enum class EColliderLayer {
		UI = 0,
		Game
	};

	enum class EColliderType{
		Static = 0,
		Dynamic
	};

	class Entity;
	class Component;

	class Collider {
	public:
		Collider() {};
		~Collider() {};
		virtual bool CheckCollisionPoint(const float x,const float y) { return false; }
		virtual bool CheckCollision(Ref<Collider> collider) { return false; };
		
		inline const EColliderShape GetColliderShape() const { return m_shape; }
		inline void SetColliderShape(EColliderShape shape) { m_shape = shape; }
		inline const EColliderLayer GetColliderLayer() const { return m_layer; }
		inline void SetColliderLayer(EColliderLayer shape) { m_layer = shape; }
		inline const EColliderType  GetColliderType() const { return m_type; }
		inline void SetColliderType(EColliderType shape) { m_type = shape; }

		inline void SetCollisionStartFunction(std::function<void(Ref<Collider>)> func) { m_collisionFunctionStart = func; }
		inline void SetCollisionEndFunction(std::function<void(Ref<Collider>)> func) { m_collisionFunctionEnd = func; }

		inline void SetUICollisionStartFunction(std::function<void(float, float)> func) { m_uiCollisionFunctionStart = func; }
		inline void SetUICollisionEndFunction(std::function<void(float, float)> func) { m_uiCollisionFunctionEnd = func; }


		void CollideStart(Ref<Collider> collider);
		void CollideEnd(Ref<Collider> collider);

		void UIMouseCollideStart(const float x,const float y);
		void UIMouseCollideEnd(const float x,const float y);

		const glm::vec3 GetPosition() const { return position; };
		const glm::vec3 GetScale() const { return scale;}
		const glm::vec3 GetRotation() const { return rotation; }

		void SetPosition(const glm::vec3& pos) { position = pos; };
		void SetScale(const glm::vec3& scale) { this->scale = scale; }
		void SetRotation(const glm::vec3& rot) { rotation = rot; }

		Weak<Entity> GetEntity();
		Weak<Component> GetComponent();

		void SetEntity(Weak<Entity> e);
		void SetComponent(Weak<Component> c);

		inline std::vector<Weak<Collider>>& GetLastCollide() { return m_lastCollide; }
		
		void AddLastCollide(Weak<Collider> c);
		void RemoveLastCollide(Weak<Collider> c);

	protected:
		inline virtual void OnCollision(Ref<Collider> collider) {};
		inline virtual void OnCollisionEnd(Ref<Collider> collider) {};
		inline virtual void OnUIMouseCollision(const float x,const float y) {};
		inline virtual void OnUIMouseCollisionEnd(const float x,const float y) {};
		EColliderShape m_shape;
		EColliderType m_type;
		EColliderLayer m_layer;
		glm::vec3 position;
		glm::vec3 scale;
		
		glm::vec3 rotation;
		std::function<void(Ref<Collider>)> m_collisionFunctionStart;
		std::function<void(Ref<Collider>)> m_collisionFunctionEnd;

		std::function<void(float, float)> m_uiCollisionFunctionStart;
		std::function<void(float, float)> m_uiCollisionFunctionEnd;

		std::vector<Weak<Collider>> m_lastCollide;

		Weak<Entity> entity;
		Weak<Component> component;
		


	};
}