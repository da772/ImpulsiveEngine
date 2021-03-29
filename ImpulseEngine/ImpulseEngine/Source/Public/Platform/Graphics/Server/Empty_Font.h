#pragma once
#include "Public/Core/Renderer/Graphics/Font.h"

namespace GEngine {

	class GE_API Empty_Font : public Font {
	public:
		inline Empty_Font(std::string path, int size) {};
		inline virtual ~Empty_Font() {};
		inline virtual void LoadCharacters(const char*) override {};
		inline virtual Ref<SubTexture2D> GetCharacterCoords(const char c) override { return nullptr;};
		inline virtual Ref<SubTexture2D> GetCharacterCoords_u32(const uint32_t c) override { return nullptr; };
		inline virtual void LoadCharacter_u32(const uint32_t codepoint) override {};
		inline virtual void LoadCharacters_u32(const uint32_t* codepoint, const uint32_t size) override {};
		inline virtual Ref<StringInfo> DrawString(const std::string& s, float maxWidth, int viewWidth, int viewHeight) override {return nullptr;};
		inline virtual Ref<StringInfo> AppendString(Ref<StringInfo> info, const std::string& text, float maxWidth, int viewWidth, int viewHeight) override {return nullptr;};
		inline virtual Ref<StringInfo> DrawString_u32(uint32_t* s, int len, float maxWidth, int viewWidth, int viewHeight) override {return nullptr;};
		inline virtual Ref<StringInfo> AppendString_u32(Ref<StringInfo> info, uint32_t* text, int len, float maxWidth, int viewWidth, int viewHeight) override {return nullptr;};
		inline virtual void Unload() override {};
		inline virtual void Reload() override {};

	};



}