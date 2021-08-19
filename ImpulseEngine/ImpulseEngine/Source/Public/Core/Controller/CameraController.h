#pragma once

#include <glm/glm.hpp>

namespace GEngine {

	class Camera;
	class Timestep;
	class WindowResizeEvent;
	class MouseScrolledEvent;
	class Event;

	using OnUpdateEventFn = std::function<void(Timestep, Vector3f&, Vector3f&, Vector2f&, uint64_t&, float&)>;
	using OnEventFn = std::function<void(Event&)>;

	class GE_API CameraController {
	public:
		CameraController() {};
        virtual ~CameraController(){};

		virtual void OnUpdate(Timestep timeStep) = 0;
		virtual void OnEvent(Event& e) = 0;

        inline void SetOnUpdateFn(OnUpdateEventFn e) { m_OnUpdateEventFn = e; };
        inline void SetOnEventFn(OnEventFn e) { m_OnEventFn = e; };

		virtual const float GetFOV() const = 0;

		inline const std::unique_ptr<Camera>& GetCamera() { return m_Camera; }

		virtual bool OnCameraZoom(const float x, const float y, const float amt) = 0;
		virtual bool OnWindowResized(WindowResizeEvent& e) = 0;
		virtual void SetCameraZoom(const float zoomLevel) {};

		inline void SetPosition(const Vector3f& position) { m_Position = position; }
		inline void SetRotation(const Vector3f& rotation) { m_Rotation = rotation; }
		virtual void SetAspectRatio(const float aspectRatio) {  };


		inline const Vector3f& GetPosition() { return m_Position; }
		inline const Vector3f& GetRotation() { return m_Rotation; }


	protected:
        OnUpdateEventFn m_OnUpdateEventFn;;
        OnEventFn m_OnEventFn;

	protected:
		Vector3f m_Position = { 0.f,0.f,0.f };
		Vector3f m_Rotation = { 0.f,0.f,0.f };
		float m_flRotation = 0.f;
		Vector2f m_LastTouchPos = {-1.f, -1.f};
		uint64_t m_LastTouchId = 0;
		float m_pinchDistance = 0;


	protected:
		Scope<Camera> m_Camera;

	};

	class GE_API Orthographic_CameraController : public CameraController {
	public:
		Orthographic_CameraController(const float aspectRatio);

		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnEvent(Event& e) override;


		virtual const float GetFOV() const override { return m_ZoomLevel; };

		virtual bool OnCameraZoom(const float x, const float y, const float amt) override;
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual void SetCameraZoom(const float zoomLevel) override;
		virtual void SetAspectRatio(const float aspectRatio) override;


	protected:
		
		
	private:
		float m_AspectRatio = 16.f / 9.f;
		float m_ZoomLevel = 1.f;
		


	};

	class GE_API Perspective_CameraController : public CameraController {
	public:
		Perspective_CameraController(const float aspectRatio);

		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnEvent(Event& e) override;
		virtual bool OnWindowResized(WindowResizeEvent& e) override;


		virtual bool OnCameraZoom(float x, float y, float amt) override;
		virtual const float GetFOV() const override { return m_ZoomLevel; };

		virtual void SetAspectRatio(const float aspectRatio) override;
	protected:


	private:
		float m_ZoomLevel = 1.f;
		float m_AspectRatio = 16.f / 9.f;


	};

}
