#pragma once
#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	class OpenGL_Texture2D : public Texture2D {
	public:
		OpenGL_Texture2D(const std::string&  path, const u32 flags);
		OpenGL_Texture2D(uint32_t width, uint32_t height);
		~OpenGL_Texture2D();

		virtual void Unload() override;
		virtual void Reload() override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }
		virtual inline const u32 GetRendererID() const override { return m_RendererID; }
		virtual void SetData(void* data, uint32_t size, u32 flags) override;
		

	private:
		void UploadDataSTBI(unsigned char* _data, uint64_t _size);
		uint32_t m_Width, m_Height = 0;
		int  m_RendererID = 0;
		void* m_data = nullptr;
		uint64_t m_size = 0;
		std::string m_Path;
	};





}
