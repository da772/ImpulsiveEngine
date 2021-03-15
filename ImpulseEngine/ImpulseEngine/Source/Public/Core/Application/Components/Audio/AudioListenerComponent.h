#pragma once
#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class AudioListenerComponent : public Component {
	public:
		AudioListenerComponent(Entity* e);
		virtual ~AudioListenerComponent();
		
		void SetPitch(float f);
		void SetVolume(float f);
		
		const Vector3f GetPosition();

		const float GetPitch();
		const float GetVolume();

	protected:
		void OnBegin() override;


		void OnEnd() override;

	private:
		float m_volume = 1.f, m_pitch = 1.f;

	};



}
