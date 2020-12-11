#pragma once
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

#include <optional>

#ifdef GE_GRAPHICS_API_VULKAN
#include "vulkan/vulkan.h"
#endif

struct GLFWwindow;

namespace GEngine {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

#ifdef GE_GRAPHICS_API_VULKAN
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
#endif

	class Vulkan_GraphicsContext : public GraphicsContext {

	public:
		Vulkan_GraphicsContext(const void* windowHandle);
		~Vulkan_GraphicsContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	public:
#ifdef GE_GRAPHICS_API_VULKAN
		inline static VkDevice GetDevice() { return Vulkan_GraphicsContext::device; };
		inline static void SetClearColor(const Vector4f& color) { Vulkan_GraphicsContext::Get()->m_ClearColor = color; }
		inline static void SetPresentMode(VkPresentModeKHR mode) { m_presentMode = mode; Get()->recreateSwapChain(); }
		//inline static void Clear() { Vulkan_GraphicsContext::Get()->recreateSwapChain(); }
		inline virtual void SetVSync(bool vsync) override { if (vsync) SetPresentMode(VK_PRESENT_MODE_FIFO_KHR); else SetPresentMode(VK_PRESENT_MODE_MAILBOX_KHR); };
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		static void BindShader(uint32_t id);
		static void UploadShaderMat4(uint32_t id, const glm::mat4&);
		static void BindVertexStructure(uint32_t id);
		static void SetPipelineVertices(uint32_t id, const std::vector<float>& vertices);
		static void SetPipelineIndices(uint32_t id, const std::vector<uint32_t>& indices);
		

		static void GenShaderId(uint32_t& id);
		static void SetShaderInfo(uint32_t id, std::vector<VkPipelineShaderStageCreateInfo> info, std::vector < std::vector<char> > sources);
		static void DestroyShaderId(uint32_t& id);
		static void _DestroyShader(uint32_t id);
		inline static void UnBindShader() { Get()->boundShader = 0; };
		

		struct ShaderInfo {

		public:
			ShaderInfo() {};
			ShaderInfo(uint32_t id) : shader_id(id) {};

			VkPipeline graphicsPipeline;
			VkPipelineLayout pipelineLayout;

			glm::mat4 vproj;

			std::vector<VkPipelineShaderStageCreateInfo> info;
			std::vector<std::vector<char>> sources;

			uint32_t shader_id = 0;

			bool bound = false;

		};

		struct VertexStructure {
			std::vector<float> vertices = {
			-0.5f, -0.5f,  0.f,   1.0f, 0.0f, 0.0f,
			 0.5f,  -0.5f,  0.f,   0.0f, 1.0f, 0.0f,
			 0.5f,   0.5f,  0.f,   0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.f,   1.0f, 1.0f, 1.0f,
			};
			std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0,
			};

			VkBuffer vertexBuffer;
			VkBuffer indexBuffer;
			
			VkDeviceMemory vertexBufferMemory;
			VkDeviceMemory indexBufferMemory;
			std::vector<VkDescriptorSet> descriptorSets;

			std::vector<std::vector<VkBuffer>> uniformBuffers = { std::vector<VkBuffer>() };
			std::vector<std::vector<VkDeviceMemory>> uniformBuffersMemory = { std::vector<VkDeviceMemory>() };

			VkDescriptorPool descriptorPool;

			bool operator==(VertexStructure l) {
				return l.renderer_id == renderer_id;
			}

			uint32_t renderer_id = 0;
			uint32_t shader_id = 0;
			bool bound = false;
		};

		static void CreatePipeline(uint32_t id, VkVertexInputBindingDescription bindingDescription, std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions);
		static void GenVertexStructure(uint32_t& id);
		static void DestroyVertexStructure(uint32_t& id);
		

#endif
		inline static Vulkan_GraphicsContext* Get() { return m_Context; }
		inline static void Resized() { Vulkan_GraphicsContext::Get()->framebufferResized = true; }
	private:
		void createInstance();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSurface();
		void setupDebugMessenger();
		void createSwapChain();
		void createImageViews();
		bool checkValidationLayerSupport();
		void createGraphicsPipeline();
		void createRenderPass();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();

		void createVertexStructure(uint32_t id);
		void _createVertexBuffer(VertexStructure* p);
		void createSyncObjects();
		void drawFrame();
		void createIndexBuffer();
		void _createIndexBuffer(VertexStructure* p);
		void recreateSwapChain();

		void createShader(uint32_t id);
		void cleanupSwapChain();
		void createDescriptorSets();
		void _createDescriptorSets(VertexStructure* v);
		void createDescriptorSetLayout();

		void createDescriptorPool();
		void _createDescriptorPool(VertexStructure* p);
		void updateUniformBuffer(uint32_t currentImage);
		void createUniformBuffers();
		void _createUniformBuffers(VertexStructure* p);

		std::vector<const char*> getRequiredExtensions();
#ifndef GE_DEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		void _DestroyVertexStructure(uint32_t id);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		struct Vertex {
			Vector3f pos;
			Vector3f color;

			static VkVertexInputBindingDescription getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, color);

				return attributeDescriptions;
			}
		};


		struct UniformBufferObject {
			glm::mat4 viewProjection;
			glm::mat4 transform;
		};


		std::unordered_map<uint32_t, VertexStructure> vertexStructureMap;
		std::vector<uint32_t> vertexStructureMap_reuse;
		uint32_t boundShader = 0;
		std::unordered_map<uint32_t, ShaderInfo> shaderMap;
		std::vector<uint32_t> shaderMap_resuse;;

#endif
		
	public:
		const int MAX_FRAMES_IN_FLIGHT = 2;
		static Vulkan_GraphicsContext* m_Context;
		bool framebufferResized = false;
		static VkPresentModeKHR m_presentMode;
#ifdef GE_GRAPHICS_API_VULKAN
		VkInstance instance;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = 0;
		static VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkDescriptorPool descriptorPool;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSwapchainKHR swapChain;
		VkCommandPool commandPool;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkDescriptorSetLayout descriptorSetLayout;
		VkRenderPass renderPass;
		std::vector<VkCommandBuffer> commandBuffers;
		uint32_t imageIndex;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
		Vector4f m_ClearColor = { 0,0,0, 1.f };
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		bool dirtyPipeLine = false;
#endif
		GLFWwindow* m_WindowHandle;
	};

}
