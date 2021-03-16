#pragma once

#ifndef GE_GENGINE_DEF
#define GE_GENGINE_DEF


#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Log.h"
#include "Public/Core/Util/KeyCodes.h"
#include "Public/Core/Util/MouseButtonCodes.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Util/Time.h"
#include "Public/Core/Application/Layer.h"
#include "Public/Core/ImGui/ImGuiLayer.h"
#include "Public/Core/Application/Input.h"
#include "Public/Platform/Window/Mobile/Mobile_Input.h"
#include "Public/Platform/Window/Mobile/MobileWindow.h"
//#include "Public/Platform/Window/Mobile/Mobile_Interface.h"
#include "reflection/macros.h"
#include "Public/Core/Util/Random.h"

#include "Public/Core/Application/Components/UI/UIComponent.h"
#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Application/Components/SpriteAnimationComponent.h"
#include "Public/Core/Application/Components/QuadColliderComponent.h"
#include "Public/Core/Application/Components/UI/ButtonComponent.h"
#include "Public/Core/Application/Components/Audio/AudioComponent.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Components/Audio/AudioListenerComponent.h"

#include "Public/Core/Renderer/Graphics/Circle.h"
#include "Public/Core/Renderer/Graphics/Quad.h"

#include "Public/Core/Renderer/Graphics/Buffer.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline_ui.h"


#include "Public/Core/Events/ApplicationEvent.h"
#include "Public/Core/Events/KeyEvent.h"
#include "Public/Core/Events/MouseEvent.h"
#include "Public/Core/Events/TouchEvent.h"

#include "Public/Core/Networking/Networking.h"

#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/Font.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"

#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"

#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Factory.h"
#include "Public/Core/Util/Utility.h"
#include "Public/Core/Util/ThreadPool.h"

#include "Public/Platform/Scripting/NativeScriptComponent.h"
#include "Public/Platform/Scripting/ScriptApi.h"

#include "Public/Core/FileSystem/FileSystem.h"

#include "Public/Core/Controller/CameraController.h"

#include "Public/Core/Collision/CollisionDetection.h"

#include "Public/Core/Util/GEMath.h"

#include "Public/Core/Ads/AdManager.h"

#include "Public/Core/Audio/AudioManager.h"
#include "Public/Core/Audio/AudioSource.h"


#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Physics/PhysicsContext.h"
#include "Public/Core/Physics/PhysicsBody.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"


#include "Public/Core/Hardware/DeviceInfo.h"


#include "Public/Core/Core.h"

#endif

