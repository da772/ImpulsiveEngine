#pragma once
#include <glm/glm.hpp>


#ifdef GE_PLATFORM_WINDOWS
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#ifdef GE_PLATFORM_MACOSX
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#ifdef GE_PLATFORM_IOS
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#ifdef GE_PLATFORM_ANDROID
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif

namespace GEngine {


	namespace GraphicsApi { enum FGraphicsApi : uint8_t { NONE = 0, OPENGL, DIRECTX12, DIRECTX11, VULKAN }; }

	using namespace GraphicsApi;


	class GraphicsContext {

	public:
		GraphicsContext();
		virtual ~GraphicsContext();

		virtual void Init() {};
		virtual void SwapBuffers() = 0;

		virtual void SetVSync(bool vsync) = 0;

		static GraphicsContext* Create(const void* window);

		static inline FGraphicsApi GetGraphicsApi() { return s_GraphicsApi; };
		static inline void SetGraphicsApi(FGraphicsApi api) { s_GraphicsApi = api; };


	private:
		template<typename T>
		static GraphicsContext* _CreateContext(const void* window);

		static FGraphicsApi s_GraphicsApi;

	};


}
