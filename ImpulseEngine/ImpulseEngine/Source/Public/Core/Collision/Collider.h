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
	class Event;

	class Collider {
	public:
		Collider() {};
		~Collider() {};
		virtual bool CheckCollisionPoint(const float x,const float y) { return false; }
		virtual bool CheckCollision(Collider* collider) { return false; };
		
		inline const EColliderShape GetColliderShape() const { return m_shape; }
		inline void SetColliderShape(EColliderShape shape) { m_shape = shape; }
		inline const EColliderLayer GetColliderLayer() const { return m_layer; }
		inline void SetColliderLayer(EColliderLayer shape) { m_layer = shape; }
		inline const EColliderType  GetColliderType() const { return m_type; }
		inline void SetColliderType(EColliderType shape) { m_type = shape; }

		inline void SetCollisionStartFunction(std::function<void(Collider*)> func) { m_collisionFunctionStart = func; }
		inline void SetCollisionEndFunction(std::function<void(Collider*)> func) { m_collisionFunctionEnd = func; }

		inline void SetUICollisionStartFunction(std::function<void(float, float)> func) { m_uiCollisionFunctionStart = func; }
		inline void SetUICollisionEndFunction(std::function<void(float, float)> func) { m_uiCollisionFunctionEnd = func; }
		inline void SetUIOnEvent(std::function<void(const Event&)> func) { m_uiEvent = func; }


		void CollideStart(Collider* collider);
		void CollideEnd(Collider* collider);

		void UIMouseCollideStart(const float x,const float y);
		void UIMouseCollideEnd(const float x,const float y);
		void UIOnEvent(const Event& e) { if (m_uiEvent) m_uiEvent(e); }

		const Vector3f GetPosition() const { return position; };
		const Vector3f GetScale() const { return scale;}
		const Vector3f GetRotation() const { return rotation; }

		void SetPosition(const Vector3f& pos) { position = pos; };
		void SetScale(const Vector3f& scale) { this->scale = scale; }
		void SetRotation(const Vector3f& rot) { rotation = rot; }

		Entity* GetEntity();
		Component* GetComponent();

		void SetEntity(Entity* e);
		void SetComponent(Component* c);

		inline std::vector<Collider*>& GetLastCollide() { return m_lastCollide; }
		
		void AddLastCollide(Collider* c);
		void RemoveLastCollide(Collider* c);

	protected:
		inline virtual void OnCollision(Collider* collider) {};
		inline virtual void OnCollisionEnd(Collider* collider) {};
		inline virtual void OnUIMouseCollision(const float x,const float y) {};
		inline virtual void OnUIMouseCollisionEnd(const float x,const float y) {};
		EColliderShape m_shape;
		EColliderType m_type;
		EColliderLayer m_layer;
		Vector3f position;
		Vector3f scale;
		
		Vector3f rotation;
		std::function<void(Collider*)> m_collisionFunctionStart;
		std::function<void(Collider*)> m_collisionFunctionEnd;
		

		std::function<void(float, float)> m_uiCollisionFunctionStart;
		std::function<void(float, float)> m_uiCollisionFunctionEnd;
		std::function<void(const Event& e)> m_uiEvent = nullptr;

		std::vector<Collider*> m_lastCollide;

		Entity* entity;
		Component* component;
		


	};
}