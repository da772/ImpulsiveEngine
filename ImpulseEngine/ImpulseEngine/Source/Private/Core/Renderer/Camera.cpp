#include "gepch.h"
#include "Core/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GEngine {

	Orthographic_Camera::Orthographic_Camera(float left, float right, float bottom, float top, bool flipY)
	{
		m_ProjectionMatrix = glm::ortho(left, right, -1.f, 1.f, 1e9f, -1e9f);
		b_FlipY = flipY;
		if (b_FlipY)
			m_ProjectionMatrix[1][1] *= -1;
		m_ViewMatrix = glm::mat4(1.f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Orthographic_Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position) * glm::rotate(glm::mat4(1.f), glm::radians(m_flRotation), Vector3f(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, 1e9f, -1e9f);
		if (b_FlipY)
			m_ProjectionMatrix[1][1] *= -1;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Perspective_Camera::Perspective_Camera(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians( 45.f), 16 / 9.f, .1f, 1000.0f);
		m_ProjectionMatrix = glm::scale(m_ProjectionMatrix, Vector3f(-1.0f, 1.0f, 1.0f));
		RecalculateViewMatrix();
	}

	void Perspective_Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position) * glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.x), Vector3f(1, 0, 0)) * 
			glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.y), Vector3f(0, 1, 0)) *
			glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.z), Vector3f(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}