#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class AudioSource;

	class AudioComponent : public Component {
	public:

		AudioComponent(const std::string& fileName, bool playing = false, bool looping = false, bool bStatic = false, float volume = 1.f, float pitch = 1.f, bool fromPak = true, bool relative = true);
		virtual ~AudioComponent();

		bool IsPlaying();
		bool IsLooping();

		void SetPlaying(bool b);
		void SetLooping(bool b);
		void Restart();
		void SetPitch(float f);
		void SetVolume(float f);
		void SetStatic(bool b);
		bool GetStatic();

		inline float GetPitch() { return m_pitch; }
		inline float GetVolume() { return m_volume; }

		void OnAttached(Ref<Entity> entity) override;
		void DeAttached(Ref<Entity> entity) override;

	protected:
		void OnBegin() override;


		void OnEnd() override;

	private:
		Ref<AudioSource> m_audioSource = nullptr;
		std::string m_fileName;
		bool m_fromPak, m_relative;
		float m_volume = 1.f, m_pitch = 1.f;
		bool m_playing = false, m_looping = false, m_static = false;

	};



}
