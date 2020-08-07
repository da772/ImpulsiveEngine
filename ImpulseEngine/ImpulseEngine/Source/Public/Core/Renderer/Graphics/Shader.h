#pragma once

#include <glm/glm.hpp>

namespace GEngine {

	template<class T, class id>
	class ObjectPool;

	class FileData;

	class Shader {
	public:
	
		static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files = false);
		static Shader* Create(const std::string& filePath);
		static void Destroy(Shader* shader);
		virtual ~Shader() {};

		inline const std::string& GetName() { return m_Name; }

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void UploadUniformIntArray(const std::string& name, int* values, u32 count) = 0;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void UploadUniformFloat(const std::string& name, const float values) = 0;
		virtual void UploadUniformInt(const std::string& name, const int values) = 0;

		inline virtual uint32_t ShaderTypeFromString(const std::string& type) { return 0; }


		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		std::string ReadFile(const std::string& filePath);

	protected:
		Shader() {};
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {};
		
		std::string m_Name;
	private:
		static Scope<ObjectPool<Shader, std::string>> ShaderPool;


	};

	
	

}
