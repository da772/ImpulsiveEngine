#pragma once


namespace GEngine {

	class AudioContext;
	class AudioSource;

	class AudioManager {

	public:
		static void Initalize();
		static void Shutdown();
		static void SetListenerPosition(const glm::vec3& pos);
		static Ref<AudioSource> PlayOgg(std::string filename, bool fromPak = true, bool relative = true);
		static void Update();
		static void RemoveSource(Ref<AudioSource> s);

	private:
		static Ref<AudioContext> s_audioManager;

	};

}