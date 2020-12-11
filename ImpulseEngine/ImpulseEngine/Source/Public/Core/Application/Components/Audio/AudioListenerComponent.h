#pragma once
#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class AudioListenerComponent : public Component {
	public:
		AudioListenerComponent();
		virtual ~AudioListenerComponent();

		
		void SetPitch(float f);
		void SetVolume(float f);
		
		const Vector3f GetPosition();

		const float GetPitch();
		const float GetVolume();

		void OnAttached(Ref<Entity> entity) override;
		void DeAttached(Ref<Entity> entity) override;

	protected:
		void OnBegin() override;


		void OnEnd() override;

	private:
		float m_volume = 1.f, m_pitch = 1.f;

	};



}
