#pragma once

#include "Public/Core/Audio/AudioContext.h"

namespace GEngine {

	class AudioSource;
	struct AudioStreamingData;

	class OpenAL_Context : public AudioContext {
	public:
		OpenAL_Context();
		~OpenAL_Context();
		virtual void Update() override;
		virtual void Destroy(Ref<AudioSource> s) override;
		virtual void SetListenerPosition(const glm::vec3& pos) override;
		Ref<AudioSource> LoadSource(const char* fileName, bool fromPak = true, bool relative = true) override;
		virtual void ResetBuffers(Ref<AudioSource> audioSource) override;;
		virtual const glm::vec3& GetListenerPosition() override;
		virtual void SetListenerPitch(float f)  override;
		virtual void SetListenerVolume(float f) override;
		virtual const float GetListenerPitch() override;
		virtual const float GetListenerVolume() override;

	private:
		void* device;
		void* ctx;

		void UpdateStream(Ref<AudioSource> audioSource);

		glm::vec3 m_listenerPos = glm::vec3(0,0,0);
		float m_listenerPitch = 1.f, m_listenerVolume = 1.f;
		




	};



}