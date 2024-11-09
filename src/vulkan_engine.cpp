#include "vulkan_engine.h"

void VulkanEngine::Initialize() {
	// initialize SDL and create a window with it. 
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

	_window = SDL_CreateWindow(
		"vulkan_engine_window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_windowExtent.width,
		_windowExtent.height,
		window_flags
	);

	// create vulkan instance with bootstrap helper
	vkb::InstanceBuilder instancebuilder;
	auto instancebuilderReturn = instancebuilder
		.set_app_name("Vulkan Engine App")
		.use_default_debug_messenger()
		.require_api_version(1,3,0)
		.request_validation_layers()
		.build();
	if (!instancebuilderReturn) {
		printf("Instance builder failed\n");
		return;
	}
	_vkbInstance = instancebuilderReturn.value();
	
	// create sdl surface
	VkSurfaceKHR surface = nullptr;
	SDL_bool err = SDL_Vulkan_CreateSurface(_window, _vkbInstance.instance, &surface);
	if (!err) {
		printf("Create SDL Vulkan surface failed\n");
		return;
	}

	// define required vulkan 1.3 & 1.2 features
	VkPhysicalDeviceVulkan13Features requireFeatureVk13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	requireFeatureVk13.dynamicRendering = true;
	requireFeatureVk13.synchronization2 = true;
	VkPhysicalDeviceVulkan12Features requireFeatureVk12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	requireFeatureVk12.bufferDeviceAddress = true;
	requireFeatureVk12.descriptorIndexing = true;

	// select GPU
	vkb::PhysicalDeviceSelector deviceSelector(_vkbInstance);
	auto deviceSelectorReturn = deviceSelector
		.set_surface(surface)
		.set_required_features_13(requireFeatureVk13)
		.set_required_features_12(requireFeatureVk12)
		.select();
	if (!deviceSelectorReturn) {
		printf("Select GPU Physical Device failed\n");
		return;
	}
	_vkbPhysicalDevice = deviceSelectorReturn.value();
	vkb::DeviceBuilder deviceBuilder{ _vkbPhysicalDevice };
	auto builderReturn = deviceBuilder.build();
	if (!builderReturn) {
		printf("Device build failed\n");
		return;
	}
	_vkbDevice = builderReturn.value();

	// create swapchain
	vkb::SwapchainBuilder swapchainBuilder{ _vkbDevice };

	auto swapchainReturn = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(_windowExtent.width, _windowExtent.height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build();
	if (!swapchainReturn) {
		printf("Swapchain build failed\n");
		return;
	}
	_vkbSwapchain = swapchainReturn.value();
	_swapchain = _vkbSwapchain.swapchain;

	// initialize commands

	// initialize sync structures
	_windowInitialized = true;
}

void VulkanEngine::Destroy() {
	if (_vkbInstance != nullptr)
		vkb::destroy_instance(_vkbInstance);
	if (_vkbDevice != nullptr)
		vkb::destroy_device(_vkbDevice);
	if (_vkbSwapchain != nullptr)
		vkb::destroy_swapchain(_vkbSwapchain);
}
