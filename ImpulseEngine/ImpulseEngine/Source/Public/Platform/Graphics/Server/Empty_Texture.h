#pragma once
#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	class GE_API Empty_Texture2D : public Texture2D {
	public:
		inline Empty_Texture2D(const std::string&  path, const u32 flags) {};
		inline Empty_Texture2D(uint32_t width, uint32_t height){};
		inline ~Empty_Texture2D(){};
		inline virtual void Unload() override{};
		inline virtual void Reload() override{};
		inline virtual void Bind(uint32_t slot = 0) const override{};
		inline virtual void UnBind() const override{};
		virtual inline uint32_t GetWidth() const override { return 0; }
		virtual inline uint32_t GetHeight() const override { return 0; }
		virtual inline const u32 GetRendererID() const override { return 0; }
		virtual void SetData(void* data, uint32_t size, u32 flags, int width = 0, int height = 0) override {};
		inline void Resize(int width, int height) override {};
	};





}
