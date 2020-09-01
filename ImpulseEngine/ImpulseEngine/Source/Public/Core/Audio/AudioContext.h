#pragma once


namespace GEngine {


	class FileData;
	class AudioSource;

	class AudioContext {
	public:
		AudioContext() {};
		~AudioContext() {};

		virtual Ref<AudioSource> LoadSource(const char* fileName, bool fromPak = true, bool relative = true) = 0;
		virtual void Update() = 0;
		virtual void Destroy(Ref<AudioSource> s) = 0;
		virtual void SetListenerPosition(const glm::vec3& pos) = 0;
		virtual void SetListenerPitch(float f) = 0;
		virtual void SetListenerVolume(float f) = 0;
		virtual const glm::vec3& GetListenerPosition() = 0;
		static Ref<AudioContext> Create();
		virtual void Pause();
		virtual void Resume();
		virtual void ResetBuffers(Ref<AudioSource> audioSource) = 0;
		virtual const float GetListenerVolume() = 0;
		virtual const float GetListenerPitch() = 0;



	protected:
		std::unordered_set<Ref<AudioSource>> m_sources;



	};





}