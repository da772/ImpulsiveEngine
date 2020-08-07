#include "gepch.h"

#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Core/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"
#include "Public/Core/Application/Window.h"
#include "Public/Core/Platform/Graphics/Vulkan/Vulkan_Shader.h"
#include "Public/Core/Util/Utility.h"


#include <GLFW/glfw3.h>
#include <set>
#include <algorithm>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace GEngine {


	VkDevice Vulkan_GraphicsContext::device;
	Vulkan_GraphicsContext* Vulkan_GraphicsContext::m_Context;
	VkPresentModeKHR Vulkan_GraphicsContext::m_presentMode = VK_PRESENT_MODE_FIFO_KHR;

	Vulkan_GraphicsContext::Vulkan_GraphicsContext(const void* windowHandle)
		: m_WindowHandle((GLFWwindow*)windowHandle)
	{
		if (m_Context != nullptr) {
			GE_CORE_ASSERT(false, "Should be nullptr");
		}
		m_Context = this;
	}

	Vulkan_GraphicsContext::~Vulkan_GraphicsContext()
	{
		vkDeviceWaitIdle(device);
		cleanupSwapChain();
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		if (vertexStructureMap.size() > 0) {
			for (auto& k : vertexStructureMap) {
				auto& v = k.second;
				vkDestroyBuffer(device, v.indexBuffer, nullptr);
				vkFreeMemory(device, v.indexBufferMemory, nullptr);
				vkDestroyBuffer(device,v.vertexBuffer, nullptr);
				vkFreeMemory(device, v.vertexBufferMemory, nullptr);
			}
			
		}
		
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(device, commandPool, nullptr);

		vkDestroyDevice(device, nullptr);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);
		
		m_Context = nullptr;

	}

	void Vulkan_GraphicsContext::Init()
	{
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();

		//Get this out
		createDescriptorSetLayout();

		createFramebuffers();
		createCommandPool();
		createUniformBuffers();
		
		createCommandBuffers();
		createSyncObjects();
	}

	
	void Vulkan_GraphicsContext::SwapBuffers()
	{
		drawFrame();
	}

	void Vulkan_GraphicsContext::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			GE_CORE_ASSERT(false, "Vulkan Api: Validation layers requested, but not available!")
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Sandbox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "GEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;


		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create instance");
		}

		
		
	}

	void Vulkan_GraphicsContext::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to find GPUs with Vulkan support");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to find GPUs with Vulkan support");
		}

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevice, &props);
		
		GE_CORE_INFO("Vulkan Api: Api Version: {0}, Device: {1} - Driver Version: {2}", props.apiVersion, props.deviceName, props.driverVersion);

	}

	void Vulkan_GraphicsContext::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create logical device");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}

	void Vulkan_GraphicsContext::createSurface()
	{
	
		if (glfwCreateWindowSurface(instance, m_WindowHandle, nullptr, &surface) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create window surface!");
		}
	}

	void Vulkan_GraphicsContext::setupDebugMessenger()
	{
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to setup debug messenger!");
		}
	}

	void Vulkan_GraphicsContext::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create swap chain");
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
	}

	void Vulkan_GraphicsContext::createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to create image views");
			}
		}
	}

	std::vector<const char*> Vulkan_GraphicsContext::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool Vulkan_GraphicsContext::isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool Vulkan_GraphicsContext::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	GEngine::QueueFamilyIndices Vulkan_GraphicsContext::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	void Vulkan_GraphicsContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = Vulkan_GraphicsContext::debugCallback;
	}

	VkResult Vulkan_GraphicsContext::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan_GraphicsContext::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		GE_CORE_ERROR("Vulkan Api: {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	void Vulkan_GraphicsContext::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	bool Vulkan_GraphicsContext::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}


	void Vulkan_GraphicsContext::BindShader(uint32_t id)
	{
		Get()->boundShader = id;
	}

	void Vulkan_GraphicsContext::UploadShaderMat4(uint32_t id, const glm::mat4& m)
	{
		Get()->shaderMap[id].vproj = m;
	}

	void Vulkan_GraphicsContext::BindVertexStructure(uint32_t id)
	{
		if ((!Get()->vertexStructureMap[id].bound || Get()->vertexStructureMap[id].shader_id != Get()->boundShader))
			Get()->createVertexStructure(id);
	}

	void Vulkan_GraphicsContext::SetPipelineVertices(uint32_t id, const std::vector<float>& vertices)
	{

		Get()->vertexStructureMap[id].vertices = vertices;
		if (Get()->vertexStructureMap[id].bound) {
			vkDeviceWaitIdle(device);
			vkDestroyBuffer(device, Get()->vertexStructureMap[id].vertexBuffer, nullptr);
			vkFreeMemory(device, Get()->vertexStructureMap[id].vertexBufferMemory, nullptr);
			Get()->_createVertexBuffer(&Get()->vertexStructureMap[id]);
			Get()->createCommandBuffers();
		}
		
	}

	void Vulkan_GraphicsContext::SetPipelineIndices(uint32_t id, const std::vector<uint32_t>& indices)
	{
		Get()->vertexStructureMap[id].indices = indices;
	}

	void Vulkan_GraphicsContext::GenShaderId(uint32_t& id)
	{
		uint32_t i = 0;
		if (Get()->shaderMap_resuse.size() > 0) {
			i = Get()->shaderMap_resuse.back();
			Get()->shaderMap_resuse.pop_back();
		}
		else {
			i = Get()->shaderMap.size() > 0 ? (uint32_t)Get()->shaderMap.size()+1 : 1;
		}
		id = i;
		Get()->shaderMap[i] = ShaderInfo(id);
	
	}

	void Vulkan_GraphicsContext::SetShaderInfo(uint32_t id, std::vector<VkPipelineShaderStageCreateInfo> info, std::vector < std::vector<char> > sources)
	{
		Get()->shaderMap[id].info = info;
		Get()->shaderMap[id].sources = sources;
		
	}

	void Vulkan_GraphicsContext::GenVertexStructure(uint32_t& id)
	{
		uint32_t i = 0;
		if (Get()->vertexStructureMap_reuse.size() > 0) {
			i = Get()->vertexStructureMap_reuse.back();
			Get()->vertexStructureMap_reuse.pop_back();
		}
		else {
			i = Get()->vertexStructureMap.size() > 0 ? (uint32_t)Get()->vertexStructureMap.size()+1 : 1;
		}
		id = i;
		Get()->vertexStructureMap[i] = VertexStructure();
		Get()->vertexStructureMap[i].renderer_id = i;
	}


	void Vulkan_GraphicsContext::DestroyVertexStructure(uint32_t& id)
	{
		if (id > 0) {
			Get()->_DestroyVertexStructure(id);
			Get()->vertexStructureMap.erase(id);
			Get()->vertexStructureMap_reuse.push_back(id);
			id = 0;
			Get()->createCommandBuffers();
		}
		else {
			GE_CORE_ASSERT(false, "Vulkan Api: Vertex Structure has invalid id")
		}
	}

	void Vulkan_GraphicsContext::_DestroyVertexStructure(uint32_t id)
	{
		vkDeviceWaitIdle(device);

		auto& v = vertexStructureMap[id];
		if (!v.bound) return;
		vkDestroyBuffer(device, v.indexBuffer, nullptr);
		vkFreeMemory(device, v.indexBufferMemory, nullptr);
		vkDestroyBuffer(device, v.vertexBuffer, nullptr);
		vkFreeMemory(device, v.vertexBufferMemory, nullptr);

		for (int j = 0; j < v.uniformBuffers.size(); j++) {
			for (size_t i = 0; i < swapChainImages.size(); i++) {
				vkDestroyBuffer(device, v.uniformBuffers[j][i], nullptr);
				vkFreeMemory(device, v.uniformBuffersMemory[j][i], nullptr);
			}
		}
		vkDestroyDescriptorPool(device, v.descriptorPool, nullptr);
			
		
	}

	void Vulkan_GraphicsContext::DestroyShaderId(uint32_t& id)
	{	
		if (id == 0) return;
		Get()->_DestroyShader(id);
		Get()->shaderMap.erase(id);
		Get()->shaderMap_resuse.push_back(id);
		id = 0;
	
	}

	void Vulkan_GraphicsContext::_DestroyShader(uint32_t id)
	{

		if (Get()->shaderMap[id].bound) {
			vkDeviceWaitIdle(device);
			vkDestroyPipeline(device, Get()->shaderMap[id].graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(device, Get()->shaderMap[id].pipelineLayout, nullptr);
		}
	}

	void Vulkan_GraphicsContext::CreatePipeline(uint32_t id, VkVertexInputBindingDescription bindingDescription, std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions)
	{
		ShaderInfo* p = &Get()->shaderMap[id];
		GE_CORE_ASSERT(Get()->boundShader, "Vulkan Api: Shader not bound.");
		if (p->bound) return;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)Get()->swapChainExtent.width;
		viewport.height = (float)Get()->swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = Get()->swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &Get()->descriptorSetLayout;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &p->pipelineLayout) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create pipeline layout!");
		}

		Get()->createShader(id);
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = Get()->shaderMap[Get()->boundShader].info.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = p->pipelineLayout;
		pipelineInfo.renderPass = Get()->renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (pipelineInfo.pStages == NULL) {
			vkDestroyPipelineLayout(device, p->pipelineLayout, nullptr);
			return;
		}

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &p->graphicsPipeline) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create Graphics Pipeline!");
		}

		vkDestroyShaderModule(device, Get()->shaderMap[Get()->boundShader].info[0].module, nullptr);
		vkDestroyShaderModule(device, Get()->shaderMap[Get()->boundShader].info[1].module, nullptr);

		p->bound = true;
		
	}

	void Vulkan_GraphicsContext::createGraphicsPipeline()
	{
		for (auto& k : shaderMap) {
			if (k.first == 0) return;
			auto& v = k.second;
			
			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			auto bindingDescription = Vertex::getBindingDescription();
			auto attributeDescriptions = Vertex::getAttributeDescriptions();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)swapChainExtent.width;
			viewport.height = (float)swapChainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

			if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &v.pipelineLayout) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to create pipeline layout!");
			}
		
			Get()->createShader(v.shader_id);
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderMap[v.shader_id].info.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = v.pipelineLayout;
			pipelineInfo.renderPass = renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	
			if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &v.graphicsPipeline) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to create Graphics Pipeline!");
			}

			vkDestroyShaderModule(device, shaderMap[v.shader_id].info[0].module, nullptr);
			vkDestroyShaderModule(device, shaderMap[v.shader_id].info[1].module, nullptr);
		}

	}


	void Vulkan_GraphicsContext::createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create render pass!");
		}
	}

	void Vulkan_GraphicsContext::createFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			VkImageView attachments[] = {
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to create framebuffer!");
			}
		}
	}

	void Vulkan_GraphicsContext::createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create command pool!");
		}
	}

	void Vulkan_GraphicsContext::createCommandBuffers()
	{
		
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to begin recording command buffers!");
			}

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;

			VkClearValue clearColor = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			for (auto& k : vertexStructureMap) {
				auto& v = k.second;
				if (!v.bound) continue;
				if (!shaderMap[v.shader_id].bound) continue;
				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderMap[v.shader_id].graphicsPipeline);

				VkDeviceSize offsets[] = { 0 };
				if (vertexStructureMap.size() > 0) {
						vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &v.vertexBuffer, offsets);
						if (v.indices.size() > 0) {
							vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderMap[v.shader_id].pipelineLayout, 0, 1, &v.descriptorSets[i], 0, nullptr);
							vkCmdBindIndexBuffer(commandBuffers[i], v.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
							vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(v.indices.size()), 1, 0, 0, 0);
						}
						else {
							vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderMap[v.shader_id].pipelineLayout, 0, 1, &v.descriptorSets[i], 0, nullptr);
							vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(v.vertices.size()), 1, 0, 0);
						}	
				}
			}

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to record command buffer!");
			}
		}
	}


	void Vulkan_GraphicsContext::createVertexStructure(uint32_t id)
	{
		vkDeviceWaitIdle(device);
		VertexStructure* p = &Get()->vertexStructureMap[id];
		if (Get()->boundShader == 0) { GE_CORE_ERROR("Vulkan Api: No shader bound for VertexStructure id: {0}", id); return; }
		p->shader_id = Get()->boundShader;

		if (Get()->shaderMap[p->shader_id].bound == false) {
			CreatePipeline(p->shader_id, Vertex::getBindingDescription(), Vertex::getAttributeDescriptions());
		}

		Get()->_createVertexBuffer(p);
		Get()->_createIndexBuffer(p);
		Get()->_createUniformBuffers(p);
		Get()->_createDescriptorPool(p);
		Get()->_createDescriptorSets(p);
		p->bound = true;
		Get()->createCommandBuffers();
	}

	void Vulkan_GraphicsContext::_createVertexBuffer(VertexStructure* p)
	{
		VkDeviceSize bufferSize = sizeof(p->vertices[0]) * p->vertices.size();

		if (p->vertices.size() <= 0) { GE_CORE_WARN("Vulkan Api: Creating vertex buffer with size of 0 id: {0}", p->renderer_id); return; }

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, p->vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p->vertexBuffer, p->vertexBufferMemory);

		copyBuffer(stagingBuffer, p->vertexBuffer, bufferSize);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	void Vulkan_GraphicsContext::createSyncObjects()
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkane Api: Failed to create sychronization objects for a frame");
			}
		}
	}

	void Vulkan_GraphicsContext::drawFrame()
	{
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		if (dirtyPipeLine) {
			recreateSwapChain();
			dirtyPipeLine = false;
		}
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to acquire swap chain image");
		}

		updateUniformBuffer(imageIndex);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to presetn swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Vulkan_GraphicsContext::createIndexBuffer()
	{
		for (auto& k : vertexStructureMap) {
			auto& v = k.second;
			VkDeviceSize bufferSize = sizeof(v.indices[0]) * v.indices.size();

			if (v.indices.size() <= 0) {
				GE_CORE_WARN("Vulkan Api: Trying to create Index Buffer with a size of 0 id: {0}", k.first); return;
			}

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, v.indices.data(), (size_t)bufferSize);
			vkUnmapMemory(device, stagingBufferMemory);

			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v.indexBuffer, v.indexBufferMemory);

			copyBuffer(stagingBuffer, v.indexBuffer, bufferSize);

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingBufferMemory, nullptr);
		}
	}

	void Vulkan_GraphicsContext::_createIndexBuffer(VertexStructure* p)
	{
		VkDeviceSize bufferSize = sizeof(p->indices[0]) * p->indices.size();

		if (p->indices.size() <= 0) {
			GE_CORE_WARN("Vulkan Api: Trying to create Index Buffer with a size of 0 id: {0}", p->shader_id); return;
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, p->indices.data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p->indexBuffer, p->indexBufferMemory);

		copyBuffer(stagingBuffer, p->indexBuffer, bufferSize);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	void Vulkan_GraphicsContext::recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_WindowHandle, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(m_WindowHandle, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device);

		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSets();
		createCommandBuffers();
	}

	void Vulkan_GraphicsContext::createShader(uint32_t id)
	{
			if (id == 0) return;
			auto& v = shaderMap[id];
			if (v.sources.size() <= 0) { GE_CORE_WARN("Vulkan Api: Shader created with no sources id: {0}", id); return; };
			for (int i = 0; i < 2; i++) {
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = v.sources[i].size();
				createInfo.pCode = reinterpret_cast<const uint32_t*>(v.sources[i].data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(Vulkan_GraphicsContext::GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
					GE_CORE_ASSERT(false, "Vulkan Api: Failed to create shader module");
				}

				v.info[i].module = shaderModule;
			}	
	}

	void Vulkan_GraphicsContext::cleanupSwapChain()
	{
		for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
		}

		for (auto& k : vertexStructureMap) {
			auto& v = k.second;
			if (!v.bound) continue;
			for (int j = 0; j < v.uniformBuffers.size(); j++) {
				for (size_t i = 0; i < swapChainImages.size(); i++) {
					vkDestroyBuffer(device, v.uniformBuffers[j][i], nullptr);
					vkFreeMemory(device, v.uniformBuffersMemory[j][i], nullptr);
				}
			}
			vkDestroyDescriptorPool(device, v.descriptorPool, nullptr);
			
		}

		for (auto& k : shaderMap) {
			auto& v = k.second;
			if (!v.bound) continue;
			vkDestroyPipeline(device, v.graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(device, v.pipelineLayout, nullptr);
		}


		vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		
		vkDestroyRenderPass(device, renderPass, nullptr);

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			vkDestroyImageView(device, swapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(device, swapChain, nullptr);
		
	}

	void Vulkan_GraphicsContext::createDescriptorSets()
	{
		for (auto& k : vertexStructureMap) {
			auto& v = k.second;
			for (int j = 0; j < v.uniformBuffers.size(); j++) {
				std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = v.descriptorPool;
				allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
				allocInfo.pSetLayouts = layouts.data();

				v.descriptorSets.resize(swapChainImages.size());
				if (vkAllocateDescriptorSets(device, &allocInfo, v.descriptorSets.data()) != VK_SUCCESS) {
					GE_CORE_ASSERT(false, "Vulkan Api: Failed to allocate descriptor sets!");
				}

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = v.uniformBuffers[j][i];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UniformBufferObject);

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet = v.descriptorSets[i];
					descriptorWrite.dstBinding = 0;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.pBufferInfo = &bufferInfo;

					vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
				}
			}
		}
	}

	void Vulkan_GraphicsContext::_createDescriptorSets(VertexStructure* v)
	{
		for (int j = 0; j < v->uniformBuffers.size(); j++) {
			std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = v->descriptorPool;
			allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
			allocInfo.pSetLayouts = layouts.data();

			v->descriptorSets.resize(swapChainImages.size());
			if (vkAllocateDescriptorSets(device, &allocInfo, v->descriptorSets.data()) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to allocate descriptor sets!");
			}

			for (size_t i = 0; i < swapChainImages.size(); i++) {
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = v->uniformBuffers[j][i];
				bufferInfo.offset = 0;
				bufferInfo.range = sizeof(UniformBufferObject);

				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = v->descriptorSets[i];
				descriptorWrite.dstBinding = 0;
				descriptorWrite.dstArrayElement = 0;
				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pBufferInfo = &bufferInfo;

				vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
			}
		}
	}

	void Vulkan_GraphicsContext::createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create descriptor set layout!");
		}
	}

	void Vulkan_GraphicsContext::createDescriptorPool()
	{
		for (auto& k : vertexStructureMap) {
			auto& v = k.second;
			VkDescriptorPoolSize poolSize = {};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

			if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &v.descriptorPool) != VK_SUCCESS) {
				GE_CORE_ASSERT(false, "Vulkan Api: Failed to create desciptor pool");
			}
		}
	}

	void Vulkan_GraphicsContext::_createDescriptorPool(VertexStructure* p)
	{
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &p->descriptorPool) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create desciptor pool!")
		}
	}

	void Vulkan_GraphicsContext::updateUniformBuffer(uint32_t currentImage)
	{
		int c = 0;
		for (auto& k : vertexStructureMap) {
			auto& v = k.second;
			if (!v.bound) continue;
			for (int j = 0; j < v.uniformBuffers.size(); j++) {
				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				UniformBufferObject ubo = {};
				glm::mat4 m = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 _v = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 p = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
				p[1][1] *= -1;
				ubo.viewProjection = shaderMap[v.shader_id].vproj;
				//ubo.viewProjection = p*_v*m;
				ubo.transform = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)) * glm::scale(glm::mat4(1.0f), { 1.f, 1.f, 1.f });

				void* data;
				vkMapMemory(device, v.uniformBuffersMemory[j][currentImage], 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(device, v.uniformBuffersMemory[j][currentImage]);
			}
		}
	}

	void Vulkan_GraphicsContext::createUniformBuffers()
	{
		for (auto& k : vertexStructureMap) {
			auto& v = k.second;

			for (int j = 0; j < v.uniformBuffers.size(); j++) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				v.uniformBuffers[j].resize(swapChainImages.size());
				v.uniformBuffersMemory[j].resize(swapChainImages.size());

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					createBuffer(bufferSize,
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						v.uniformBuffers[j][i], v.uniformBuffersMemory[j][i]);
				}
			}
		}
	}

	void Vulkan_GraphicsContext::_createUniformBuffers(VertexStructure* p)
	{
		for (int j = 0; j < p->uniformBuffers.size(); j++) {
			VkDeviceSize bufferSize = sizeof(UniformBufferObject);

			p->uniformBuffers[j].resize(swapChainImages.size());
			p->uniformBuffersMemory[j].resize(swapChainImages.size());

			for (size_t i = 0; i < swapChainImages.size(); i++) {
				createBuffer(bufferSize,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					p->uniformBuffers[j][i], p->uniformBuffersMemory[j][i]);
			}
		}
	}

	void Vulkan_GraphicsContext::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create buffer");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to allocate buffer memory");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void Vulkan_GraphicsContext::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

	GEngine::SwapChainSupportDetails Vulkan_GraphicsContext::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR Vulkan_GraphicsContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR Vulkan_GraphicsContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == m_presentMode) {
				return availablePresentMode;
			} else if (m_presentMode == VK_PRESENT_MODE_MAILBOX_KHR && availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR 
				&& std::find(availablePresentModes.begin(), availablePresentModes.end(),VK_PRESENT_MODE_MAILBOX_KHR) == availablePresentModes.end()) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D Vulkan_GraphicsContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_WindowHandle);
			VkExtent2D actualExtent = { data.Width, data.Height};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	uint32_t Vulkan_GraphicsContext::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		GE_CORE_ASSERT(false, "Vulkan AouL Failed to find suitable memory type");
		return 0;
	}

}


#endif
