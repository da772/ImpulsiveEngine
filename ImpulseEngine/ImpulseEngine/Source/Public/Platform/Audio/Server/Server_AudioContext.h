#pragma once

#include "Public/Core/Audio/AudioContext.h"

namespace GEngine {

	class AudioSource;
	struct AudioStreamingData;

	class GE_API Server_AudioContext : public AudioContext {
	public:
		Server_AudioContext();
		inline ~Server_AudioContext() {};
		inline virtual void Update() override {};
		inline virtual void Destroy(Ref<AudioSource> s) override {};
		inline virtual void SetListenerPosition(const Vector3f& pos) override {};
		inline Ref<AudioSource> LoadSource(const char* fileName, bool fromPak = true, bool relative = true) override {return s_source;};
		inline virtual void ResetBuffers(Ref<AudioSource> audioSource, uint32_t buffer) override { };
		inline virtual const Vector3f& GetListenerPosition() override {return m_listenerPos;};
		inline virtual void SetListenerPitch(float f)  override {};
		inline virtual void SetListenerVolume(float f) override {};
		inline virtual const float GetListenerPitch() override {return .0f;};
		inline virtual const float GetListenerVolume() override {return 0.f;};

	private:
		Vector3f m_listenerPos = Vector3f(0,0,0);
		Ref<AudioSource> s_source;




	};



}
