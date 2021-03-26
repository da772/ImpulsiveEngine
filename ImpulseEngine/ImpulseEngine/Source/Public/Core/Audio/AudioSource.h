#pragma once

namespace GEngine {
#define AUDIO_BUFFERS_NUM 4
#define AUDIO_BUFFER_SIZE 65536

	class FileData;

	struct GE_API AudioStreamingData {

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
		int fileSection = 0;
		uint32_t duration;
		bool fromPak = true;
		uint32_t bufferNum = AUDIO_BUFFERS_NUM;
		bool relative = true;
		bool queued = false;

	};

	class GE_API AudioSource {

	public:

		inline AudioSource() { };
		inline virtual ~AudioSource() {};

		virtual void Play() = 0;
		virtual void Pause()= 0;
		virtual void SetLoop(bool b) = 0;

		virtual void Destroy() = 0;

		virtual void SetVolume(float f) = 0;
		virtual void SetPitch(float f) = 0;
		virtual void Seek(float time) = 0;
        
        virtual void Unload() = 0;
        virtual void Reload() = 0;

		inline virtual float GetVolume() { return m_gain; }
		inline virtual float GetPitch() { return m_pitch; }
		inline virtual bool IsLooping() { return b_loop; }

		virtual void MaxDistance(const float f) = 0;

		inline virtual void SetPosition(const Vector3f& pos) { m_position = pos; }

		void SetSelf(Weak<AudioSource> s) { self = s; }

		virtual bool IsPlaying();

		inline virtual void __stopPlay() { b_isPlaying = false; };

		AudioStreamingData& GetData() { return m_audioData; }

		inline virtual bool GetStatic() { return b_isStatic; }

		virtual void SetStatic(bool b) = 0;

	protected:
		float m_gain = 1.f;
		float m_pitch = 1.f;
		bool b_loop = false;
		bool b_isPlaying = false;
		bool b_isStatic = false;
		AudioStreamingData m_audioData;
		Weak<AudioSource> self;
		Vector3f m_position;




	};


}
