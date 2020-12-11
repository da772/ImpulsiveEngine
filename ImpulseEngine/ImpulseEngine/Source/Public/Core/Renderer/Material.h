#pragma once
#include "Public/Core/Core.h"

namespace GEngine {

	class Texture2D;
	class Shader;

	class GE_API Material {
	public:
		Material();
		Material(Vector4f color, Ref<Texture2D> texture = nullptr, Ref<Shader> shader = nullptr);
		~Material();

		Ref<Texture2D> GetTexture();
		Ref<Shader> GetShader();
		void SetTexture(Ref<Texture2D> texture);
		void SetShader(Ref<Shader> shader);

	private:
		Ref<Texture2D> texture;
		Vector4f color;
		Ref<Shader> shader;

	};


}