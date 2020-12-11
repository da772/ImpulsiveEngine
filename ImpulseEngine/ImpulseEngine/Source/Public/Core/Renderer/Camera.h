#pragma once
#include <glm/glm.hpp>

namespace GEngine {

	class Camera {
	public:

		Camera() {};

		virtual void SetPosition(const Vector3f& position) { m_Position = position; RecalculateViewMatrix(); }
		const Vector3f& GetPosition() const { return m_Position; }

		virtual void SetRotation(const Vector3f rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		
		virtual void SetRotation(float rotation) { m_flRotation = rotation;  RecalculateViewMatrix(); }
		const float GetRotation() const { return m_flRotation; }

		void SetProjection(float left, float right, float bottom, float top);
		void SetViewMatrix(const glm::mat4& view) { m_ViewMatrix = view; m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }
		void SetProjectionMatrix(const glm::mat4 proj) { m_ProjectionMatrix = proj; m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; };

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		virtual void RecalculateViewMatrix() {};

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1);
		glm::mat4 m_ViewMatrix = glm::mat4(1);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1);

		Vector3f m_Position = {0.f,0.f,0.f};
		Vector3f m_Rotation = { 0.f, 0.f, 0.f };

		float m_flRotation = 0.f;
		bool b_FlipY = false;
	};

	class Orthographic_Camera : public Camera {
	public:
		Orthographic_Camera(float left, float right, float bottom, float top, bool flipY = false);


	private:
		virtual void RecalculateViewMatrix() override;
	};


	class Perspective_Camera : public Camera {
	public:
		Perspective_Camera(float left, float right, float bottom, float top);
		virtual void SetPosition(const Vector3f& position) override { m_Position = Vector3f(-position.x, position.y, position.z); RecalculateViewMatrix(); } ;
	private:
		virtual void RecalculateViewMatrix() override;
	};


}
