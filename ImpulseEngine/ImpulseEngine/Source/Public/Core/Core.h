#pragma once

#ifndef GE_CORE_DEF
#define GE_CORE_DEF

#ifndef UFUNCTION
#define UFUNCTION()
#endif
#ifndef UCONSTRUCTOR
#define UCONSTRUCTOR()
#endif
#ifndef UPROPERTY
#define UPROPERTY()
#endif


#ifndef GE_PHYSICS_DEFINED
	#define GE_PHYSICS_DEFINED
	#define GE_PHYSICS_BOX2D
#endif



#ifdef GE_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#ifdef GE_DYNAMIC_LINK
	#ifdef GE_BUILD_DLL
	#define GE_API __declspec(dllexport)
	#else
	#define GE_API __declspec(dllimport)
	#endif
#else
#define GE_API
#endif
#elif defined(GE_PLATFORM_LINUX) || defined (GE_PLATFORM_MACOSX)
#ifdef GE_DYNAMIC_LINK
#define GE_API __attribute__((visibility("default")))
#else
#define GE_API
#endif
#else
#define GE_API
#endif

#define IMGUI_API GE_API

#ifndef GE_SERVER_APP
#if defined(GE_PLATFORM_WINDOWS) || defined(GE_PLATFORM_MACOSX) || defined(GE_PLATFORM_LINUX)
#define GE_GRAPHICS_API_OPENGL_3_3
#define GE_WINDOW_API_GLFW
#define GE_AUDIO_OPENAL
#define GE_CONSOLE_APP
#endif
#else
#define GE_GRAPHICS_API_NONE
#define GE_WINDOW_API_SERVER
#define IMGUI_DISABLE
#define GE_AUDIO_NONE
#endif

#if defined(GE_PLATFORM_IOS)
#include <TargetConditionals.h>
#endif

#if defined(GE_PLATFORM_IOS) || defined(GE_PLATFORM_ANDROID)
#define GE_MOBILE_APP
#define GE_WINDOW_API_MOBILE
#define GE_GRAPHICS_API_OPENGL_ES
#define GE_AUDIO_OPENAL
#if defined(GE_PLATFORM_IOS)
#if !TARGET_OS_SIMULATOR
#define GE_ADS_FIREBASE
#define GE_AD_PLATFORM 1
#endif
#else
#define GE_ADS_FIREBASE
#define GE_AD_PLATFORM 1

#endif
#endif

#if defined(GE_PLATFORM_WINDOWS)
//#define GE_GRAPHICS_API_D3D12
//#define GE_WINDOW_API_WIN32
#endif

#if defined(GE_DEBUG)
#define GE_ENABLE_ASSERTS
#endif

#ifdef GE_ENABLE_ASSERTS
#if defined(GE_PLATFORM_WINDOWS) && !defined(GE_LOADED_DLL)
	#define GE_ASSERT(x, ...) { if(!(x)) { GE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GE_CORE_ASSERT(x, ...) { if(!(x)) { GE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#elif defined(GE_PLATFORM_MACOSX) || defined(GE_PLATFORM_IOS)
#define GE_ASSERT(x, ...) { if(!(x)) { GE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
#define GE_CORE_ASSERT(x, ...) { if(!(x)) { GE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
#elif defined(GE_LOADED_DLL)
#define GE_ASSERT(x, ...)     
#define GE_CORE_ASSERT(x, ...)	
#else 
#define GE_ASSERT(x, ...)           if(!(x)) { GE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);}
#define GE_CORE_ASSERT(x, ...)	    if(!(x)) { GE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);}
#endif
#else
#define GE_ASSERT(x, ...)     
#define GE_CORE_ASSERT(x, ...)	
#endif

#define BIND_EVENT_FN(x,y) std::bind(&x::y, this, std::placeholders::_1)
#define BIT(x) (1 << x)

#ifndef GE_AD_PLATFORM
#define GE_AD_PLATFORM 0
#endif

#include "glm/glm.hpp"
#include "IEVector/IEVector.hpp"
#include "Public/Core/Profiling/Profiler.h"
#include <memory.h>

namespace GEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Weak = std::weak_ptr<T>;

	using u64 = uint64_t;
	using u32 = uint32_t;
	using u16 = uint16_t;
	using u8 = uint8_t;


	using s64 = int64_t;
	using s32 = int32_t;
	using s16 = int16_t;
	using s8 =  int8_t;

	template <typename T>
	using Vector2 = IEVector::Vector2<T>;
	template <typename T>
	using Vector3 = IEVector::Vector3<T>;
	template <typename T>
	using Vector4 = IEVector::Vector4<T>;


    using Vector2f = IEVector::Vector2f;
    using Vector3f = IEVector::Vector3f;
    using Vector4f = IEVector::Vector4f;

	using Vector2i = IEVector::Vector2<int>;
	using Vector3i = IEVector::Vector3<int>;
	using Vector4i = IEVector::Vector4<int>;

    using Mat4 = glm::mat4;

	using ShapeID = uint64_t;
	using ColliderID = uint64_t;


}



#endif
