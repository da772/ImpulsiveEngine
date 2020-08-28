#pragma once

#include "Public/Core/Audio/AudioSource.h"

namespace GEngine {

	class OpenAL_source : public AudioSource {
	public:
		OpenAL_source(AudioStreamingData* data);
		virtual ~OpenAL_source();


		virtual void Play() override;
		virtual void Pause() override;
		virtual void SetLoop(bool b) override;
		virtual void Destroy() override;
		virtual void SetVolume(float f) override;
		virtual void SetPitch(float f) override;
		virtual void Seek(float time) override;


	};



}