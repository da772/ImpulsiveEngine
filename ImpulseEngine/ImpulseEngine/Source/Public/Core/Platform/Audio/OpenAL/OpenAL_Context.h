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

	private:
		void* device;
		void* ctx;

		void UpdateStream(Ref<AudioSource> audioSource);




	};



}
