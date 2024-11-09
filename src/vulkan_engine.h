#pragma once
#include "src/VkBootstrap.h"
#include <SDL.h>
#include <SDL_vulkan.h>

class VulkanEngine {
public:
	void Initialize();
	void Destroy();

private:
	bool _windowInitialized = false;
	VkExtent2D _windowExtent{ 1700, 900 };
	SDL_Window* _window = nullptr;
	vkb::Instance _vkbInstance;
	vkb::PhysicalDevice _gpuDevice;
};