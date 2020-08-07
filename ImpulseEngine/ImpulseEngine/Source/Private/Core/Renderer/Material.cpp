#include "gepch.h"
#include "Public/Core/Renderer/Material.h"

namespace GEngine {

	Material::Material()
	{

	}

	Material::Material(glm::vec4 color, Ref<Texture2D> texture /*= nullptr*/, Ref<Shader> shader /*= nullptr*/)
		: color(color), texture(texture), shader(shader)
	{

	}

	Material::~Material()
	{

	}

	GEngine::Ref<GEngine::Texture2D> Material::GetTexture()
	{
		return texture;
	}

	GEngine::Ref<GEngine::Shader> Material::GetShader()
	{
		return shader;
	}

	void Material::SetTexture(Ref<Texture2D> texture)
	{
		this->texture= texture;
	}

	void Material::SetShader(Ref<Shader> shader)
	{
		this->shader = shader;
	}

}