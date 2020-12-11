#pragma once

#include <glm/glm.hpp>

namespace GEngine {
	class Camera;
	class Texture2D;

	class Renderer2D {
	public:
		
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera* camera);
		static void EndScene();

		static void DrawQuad(const Vector2f& position, const Vector2f& size, const Vector4f& color);
		static void DrawQuad(const Vector3f& position, const Vector2f& size, const Vector4f& color);
		static void DrawQuad(const Vector3f& position, const Vector2f& size, const Vector4f& color, Texture2D* texture, float alphaTolerance = 0.f, const Vector2f& uvScale = { 1.f, 1.f });
		static void DrawQuad(const Vector2f& position, const Vector2f& size, const Vector4f& color, Texture2D* texture, float alphaTolerance = 0.f, const Vector2f& uvScale = { 1.f, 1.f });

	};

}