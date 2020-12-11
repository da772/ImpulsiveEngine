#pragma once


namespace GEngine {

	class AudioContext;
	class AudioSource;

	class AudioManager {

	public:
		static void Initalize();
		static void Shutdown();
		static void SetListenerPosition(const Vector3f& pos);
		static const Vector3f GetListenerPosition();
		static Ref<AudioSource> Load_OGG(std::string filename, bool fromPak = true, bool relative = true);
		static void SetListenerVolume(float f);
		static void SetListenerPitch(float f);
		static const float GetListenerPitch();
		static const float GetListenerVolume();
		static void Update();
		static void RemoveSource(Ref<AudioSource> s);
		static void ResetBuffers(Ref<AudioSource> s);
		static void Pause();
		static void Resume();

	private:
		static Ref<AudioContext> s_audioContext;

	};

}