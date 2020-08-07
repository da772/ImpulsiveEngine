#pragma once
#include "Public/Core/Application/Layer.h"


class ImDrawData;
namespace GEngine {

	

	class GE_API ImGuiLayer : public Layer{
	public:
		ImGuiLayer();
		~ImGuiLayer(); 
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;



		void Begin();
		void End();
		
	private:
		void Api_OnDetach();
		void OpenGL_OnDetach();
		void Vulkan_OnDetach();

		void Api_OnAttach();
		void OpenGL_OnAttach();
		void Vulkan_OnAttach();

		void Api_Begin();
		void OpenGL_Begin();
		void Vulkan_Begin();

		void Api_End();
		void OpenGL_End();
		void Vulkan_End();



	private:
		float m_Time = 0;

	};


}