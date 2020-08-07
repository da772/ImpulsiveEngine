#include "gepch.h"
#ifdef GE_WINDOW_API_GLFW
#include "examples/imgui_impl_glfw.cpp"
#endif

#ifdef GE_GRAPHICS_API_OPENGL_3_3
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "examples/imgui_impl_vulkan.cpp"
#endif


