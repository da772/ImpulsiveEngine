#pragma once
#include "Public/Core/Renderer/Graphics/Texture.h"
#ifdef GE_GRAPHICS_API_VULKAN
namespace GEngine {

	class GE_API Vulkan_Texture2D : public Texture2D {
	public:
		Vulkan_Texture2D(const std::string&  path, u32 flags = 0);
		Vulkan_Texture2D(uint32_t width, uint32_t height);
		~Vulkan_Texture2D();

		virtual void Bind(uint32_t slot = 0) const override;
		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }
		virtual inline const u32 GetRendererID() const override { return m_RendererID; }
		virtual void SetData(void* data, uint32_t size, u32 flags = 0, int width =0, int height = 0) override;
		virtual inline void Unload() {};
		virtual inline void Reload() {};


	private:
		uint32_t m_Width, m_Height, m_RendererID = 0;
		std::string m_Path;
	};





}
#endif