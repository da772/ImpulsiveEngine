#pragma once
#pragma once

#include "Public/Core/Application/Components/Scripts/NativeScript.h"

namespace GEngine {

	class GE_API AudioListenerComponent : public NativeScript {
	public:
		AudioListenerComponent(Entity* e);
		virtual ~AudioListenerComponent();
		
		void SetPitch(float f);
		void SetVolume(float f);
		
		const Vector3f GetPosition();

		const float GetPitch();
		const float GetVolume();

	public:
		void OnBegin() override;


		void OnEnd() override;

	private:
		float m_volume = 1.f, m_pitch = 1.f;

	};



}
