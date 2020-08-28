#pragma once

namespace GEngine {
#define AUDIO_BUFFERS_NUM 4
#define AUDIO_BUFFER_SIZE 8192//65536

	class FileData;

	struct AudioStreamingData {

		uint32_t buffers[AUDIO_BUFFERS_NUM];
		std::string fileName;
		Ref<FileData> file = nullptr;
		uint8_t channels;
		uint32_t sampleRate;
		uint8_t bitsPerSample;
		uint32_t size;
		uint32_t source;
		uint32_t sizeConsumed = 0;
		int format;
		void* nativeFile;
		int_fast32_t fileSection = 0;
		uint32_t duration;
		bool fromPak = true;
		bool relative = true;

	};

	class AudioSource {

	public:

		inline AudioSource(AudioStreamingData* data) { m_audioData = data; };
		inline virtual ~AudioSource() {};

		virtual void Play() = 0;
		virtual void Pause()= 0;
		virtual void SetLoop(bool b) = 0;

		virtual void Destroy() = 0;

		virtual void SetVolume(float f) = 0;
		virtual void SetPitch(float f) = 0;
		virtual void Seek(float time) = 0;

		inline virtual float GetVolume() { return m_gain; }
		inline virtual float GetPitch() { return m_pitch; }
		inline virtual bool IsLooping() { return b_loop; }

		void SetSelf(Weak<AudioSource> s) { self = s; }

		inline bool IsPlaying() { return b_isPlaying; }

		inline virtual void __stopPlay() { b_isPlaying = false; };

		AudioStreamingData* GetData() { return m_audioData; }

	protected:
		float m_gain = 1.f;
		float m_pitch = 1.f;
		bool b_loop = false;
		bool b_isPlaying = false;
		AudioStreamingData* m_audioData;
		Weak<AudioSource> self;




	};


}
