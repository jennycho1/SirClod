#include <iostream>
#define VOLK_IMPLEMENTATION
#include "volk.h"
#include "src/VkBootstrap.h"

#include <stdio.h>
#include "vulkan_engine.h"
#include <SDL.h>

int main(int argc, char** args)
{
    VulkanEngine engine;
    engine.Initialize();
    std::cout<<"Heluuuu"<<std::endl;
    while (true) {}
    engine.Destroy();
    return 0;
}
