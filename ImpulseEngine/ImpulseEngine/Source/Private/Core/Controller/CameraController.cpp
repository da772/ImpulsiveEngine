#include "gepch.h"
#include "Public/Core/Controller/CameraController.h"

#include "Public/Core/Events/MouseEvent.h"
#include "Public/Core/Events/ApplicationEvent.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

namespace GEngine {

	Orthographic_CameraController::Orthographic_CameraController(float aspectRatio)
		: m_ZoomLevel(1.f), m_AspectRatio(aspectRatio)
	{
		m_Camera = std::unique_ptr<Orthographic_Camera>(new Orthographic_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel,
			GraphicsContext::GetGraphicsApi() == GraphicsApi::VULKAN));
	}

	void Orthographic_CameraController::OnUpdate(Timestep timeStep)
	{
		if (m_OnUpdateEventFn) m_OnUpdateEventFn(timeStep, m_Position, m_Rotation, m_LastTouchPos, m_LastTouchId, m_pinchDistance);

		m_Camera->SetPosition(m_Position);
		m_Camera->SetRotation(m_Rotation.x);
	}

	void Orthographic_CameraController::OnEvent(Event& e)
	{
		if (m_OnEventFn) m_OnEventFn(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Orthographic_CameraController, OnWindowResized));
	}

	bool Orthographic_CameraController::OnCameraZoom(float x, float y, float amt)
	{
		m_ZoomLevel = glm::clamp(m_ZoomLevel - (y*amt), 0.1f, 100.f);
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}


	bool Orthographic_CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	void Orthographic_CameraController::SetCameraZoom(float zoomLevel)
	{
		m_ZoomLevel = zoomLevel;
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	Perspective_CameraController::Perspective_CameraController(float aspectRatio)
		: m_ZoomLevel(1.f), m_AspectRatio(aspectRatio)
	{
		m_Camera = std::unique_ptr<Perspective_Camera>(new Perspective_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel));
	}

	void Perspective_CameraController::OnUpdate(Timestep timeStep)
	{
		if (m_OnUpdateEventFn) m_OnUpdateEventFn(timeStep, m_Position, m_Rotation, m_LastTouchPos, m_LastTouchId, m_pinchDistance);

		m_Camera->SetPosition(m_Position);
		m_Camera->SetRotation(m_Rotation);
	}

	void Perspective_CameraController::OnEvent(Event& e)
	{
		if (m_OnEventFn) m_OnEventFn(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Perspective_CameraController, OnWindowResized));
	}

	bool Perspective_CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

	bool Perspective_CameraController::OnCameraZoom(float x, float y, float amt)
	{
		return false;
	}

}
