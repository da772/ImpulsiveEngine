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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Texture2D* texture, float alphaTolerance = 0.f, const glm::vec2& uvScale = { 1.f, 1.f });
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Texture2D* texture, float alphaTolerance = 0.f, const glm::vec2& uvScale = { 1.f, 1.f });

	};

}