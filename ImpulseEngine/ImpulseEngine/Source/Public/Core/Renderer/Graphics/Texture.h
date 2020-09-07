#pragma once


namespace GEngine {

#define TEXTUREFLAGS_NONE 0
#define TEXTUREFLAGS_Wrap_ClampToEdge (uint32_t)BIT(0)
#define TEXTUREFLAGS_Wrap_Repeat (uint32_t)BIT(1)
#define TEXTUREFLAGS_Mag_Nearest (uint32_t)BIT(2)
#define TEXTUREFLAGS_Mag_Linear (uint32_t)BIT(3)
#define TEXTUREFLAGS_Mag_Linear_MipMap (uint32_t)BIT(4)
#define TEXTUREFLAGS_Mag_Nearest_MipMap (uint32_t)BIT(5)

#define TEXTUREFLAGS_Min_Nearest (uint32_t)BIT(6)
#define TEXTUREFLAGS_Min_Linear (uint32_t)BIT(7)
#define TEXTUREFLAGS_Min_Linear_MipMap (uint32_t)BIT(8)
#define TEXTUREFLAGS_Min_Nearest_MipMap (uint32_t)BIT(9)

#define TEXTUREFLAGS_DisableMipMap (uint32_t)BIT(10)

#define TEXTUREFLAGS_RED_CHANNEL   (uint32_t)BIT(11)
#define TEXTUREFLAGS_GREEN_CHANNEL (uint32_t)BIT(12)
#define TEXTUREFLAGS_BLUE_CHANNEL  (uint32_t)BIT(13)
#define TEXTUREFLAGS_RGB_CHANNEL   (uint32_t)BIT(14)
#define TEXTUREFLAGS_RGBA_CHANNEL   (uint32_t)BIT(15)
#define TEXTUREFLAGS_ALPHA_CHANNEL   (uint32_t)BIT(16)



	template<class T, class id>
	class ObjectPool;

	class Texture {
	public:
        virtual ~Texture() ;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetWidth() const = 0;

		virtual void SetData(void* data, uint32_t size, u32 flags = 0, int width = 0, int height = 0) = 0;
		virtual const u32 GetRendererID() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void UnBind() const = 0;
		virtual inline const std::string& GetName() { return name; }

		virtual void Unload() = 0;
		virtual void Reload() = 0;

	protected:
		static std::unordered_map<std::string, Weak<Texture>> s_TexturePool;
		std::string name;
		Weak<Texture> self;
		u32 m_flags;
	};

	class Texture2D : public Texture {
	public:
		inline Texture2D() {};
		virtual ~Texture2D();
		static void UnloadTextures();
		static void ReloadTextures();
		static Ref<Texture2D> Create(const std::string& path, const u32 flags = 0);
		static Ref<Texture2D> Create(std::string name,u32 width, uint32_t height);
		virtual void Resize(int width, int height) {};
		virtual uint32_t GetHeight() const override { return 0; };
		virtual uint32_t GetWidth() const override { return 0; }
		virtual void SetData(void* data, uint32_t size, u32 flags = 0, int width = 0, int height = 0)override {};
		virtual const u32 GetRendererID() const override { return 0; };
		virtual void Bind(uint32_t slot = 0) const override {};
		virtual void UnBind() const override {};
		static void Destroy(Weak<Texture2D> texture);
		std::mutex m_mutex;

	};


}
