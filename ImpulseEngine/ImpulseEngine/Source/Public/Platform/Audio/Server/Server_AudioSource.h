#pragma once

#include "Public/Core/Audio/AudioSource.h"

namespace GEngine {

	class GE_API Server_AudioSource : public AudioSource {
	public:
		inline Server_AudioSource() {};
		inline virtual ~Server_AudioSource() {};


		inline virtual void Play() override {};
		inline virtual void Pause() override {};
		inline virtual void SetStatic(bool b) override {};
		inline virtual void SetLoop(bool b) override {};
		inline virtual void Destroy() override {};
		inline virtual void SetVolume(float f) override {};
		inline virtual void SetPitch(float f) override {};
		inline virtual void Seek(float time) override {};
		inline virtual void SetPosition(const Vector3f& pos) override {};
		inline virtual void MaxDistance(const float f) override {};
        inline virtual void Unload() override {};
        inline virtual void Reload() override {};
		inline virtual bool IsPlaying() override {return false;};


	};



}
