#pragma once


namespace GEngine {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2,Int3,Int4, Bool
	};

	enum class ShaderDataName {
		None = 0, Position, TextureCoord, Scale, Color, TextureSlot, Rotation, Custom, TextureScale, AlphaChannel, Position2D
	};


	static ShaderDataType ShaderDataNameToType(ShaderDataName type) {
		switch (type) {
		case ShaderDataName::Position:
			return ShaderDataType::Float3;
		case ShaderDataName::TextureCoord:
			return ShaderDataType::Float2;
		case ShaderDataName::Scale:
			return ShaderDataType::Float3;
		case ShaderDataName::Color:
			return ShaderDataType::Float4;
		case ShaderDataName::TextureSlot:
			return ShaderDataType::Float;
		case ShaderDataName::Rotation:
			return ShaderDataType::Float3;
		case ShaderDataName::TextureScale:
			return ShaderDataType::Float2;
		case ShaderDataName::AlphaChannel:
			return ShaderDataType::Float;
        case ShaderDataName::Position2D:
            return ShaderDataType::Float2;
        case ShaderDataName::None:
		default:
			break;
		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return ShaderDataType::None;

	}

	static std::string ShaderDataNameToName(ShaderDataName name) {
		switch (name) {
		case ShaderDataName::Position:
			return "a_Position";
		case ShaderDataName::TextureCoord:
			return "a_TexCoord";
		case ShaderDataName::Scale:
			return "a_Scale";
		case ShaderDataName::Color:
			return "a_Color";
		case ShaderDataName::TextureSlot:
			return "a_TexSlot";
		case ShaderDataName::Rotation:
			return "a_Rotation";
		case ShaderDataName::TextureScale:
			return "a_TexScale";
		case ShaderDataName::AlphaChannel:
			return "a_AlphaChannel";
        case ShaderDataName::Position2D:
            return "a_Position2D";
        case ShaderDataName::None:
		default:
			break;

		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return "NULL";
	}

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
        case ShaderDataType::None:
        default:
        break;
		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;

	}


	struct BufferElement
	{
		BufferElement() {};
		std::string Name;
		ShaderDataName DataName;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;
		uint32_t elemOffset;

		BufferElement(ShaderDataName dataName, std::string name = "NULL", ShaderDataType type = ShaderDataType::None, bool normalized = false)
			: Name(name == "NULL" ? ShaderDataNameToName(dataName) : name), DataName(dataName), Type(type == ShaderDataType::None ? ShaderDataNameToType(dataName): type),
			Size(dataName != ShaderDataName::Custom ? ShaderDataTypeSize(ShaderDataNameToType(dataName)) : ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {

		}
		
		uint32_t GetComponentCount() const {
			switch (Type) {
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3: return 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Bool: return 1;
            case ShaderDataType::None:
            default:
            break;
			}
			GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};


	class BufferLayout {

	public:
		inline BufferLayout() {};
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) 
		{
			CalculateOffsetsAndStride();
		}
		inline virtual ~BufferLayout() {
		};
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		uint32_t GetStride() const {
			return m_Stride;
		}

		inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
		inline void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			uint32_t elemOffset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				element.elemOffset = elemOffset;
				offset += element.Size;
				elemOffset += element.GetComponentCount();
				m_Stride += element.Size;
			}
		}




	};


	class VertexBuffer {
	public:
		
		virtual ~VertexBuffer();

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		inline uint32_t GetVertexCount() const { return m_VertexCount; }

		virtual void SetLayout(Ref<BufferLayout> layout) = 0;
		virtual Ref<BufferLayout> GetLayout() const = 0;

		virtual void SetVertices(float* vertices, uint32_t size) = 0;
		virtual void SetData(const void* data, uint32_t size) {};

		static VertexBuffer* Create(float* vertices, uint32_t size);
		static VertexBuffer* Create(u32 size);
	protected:
		VertexBuffer(float* vertices, uint32_t size) : m_VertexCount(size) {};
		VertexBuffer(u32 size): m_VertexCount(size) {};

		uint32_t m_VertexCount = 0;


	};


	class IndexBuffer {
	public:
		virtual ~IndexBuffer();
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		inline uint32_t GetIndexCount() const { return m_IndexCount; };

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);


	protected:
		IndexBuffer(uint32_t* indices, uint32_t size) : m_IndexCount(size) {};

	private:
		uint32_t m_IndexCount = 0;

	};

	class Texture2D;

	class FrameBuffer {
	public:
		inline virtual ~FrameBuffer() {};
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void UpdateSize(int width, int height) = 0;
		inline Ref<Texture2D> GetTexture() const { return m_texture; }

		virtual void Unload() = 0;
		virtual void Reload() = 0;

		static Ref<FrameBuffer> Create(int width, int height, int format = 0, const char* texName = "", const float renderScale = 1.f);

	protected:
		inline FrameBuffer(int width, int height, uint32_t format, const char* texName, const float renderScale) :m_width(width), m_height(height), m_format(format), m_texName(texName), m_renderScale(renderScale) {};
		uint32_t m_format;
		int m_width, m_height;
		float m_renderScale = 1.f;
		Ref<Texture2D> m_texture;
		std::string m_texName;
	};




}
