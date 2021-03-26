#pragma once

#include "Public/Core/Audio/AudioSource.h"
#if defined (GE_AUDIO_OPENAL)
#include "vorbis/vorbisfile.h"
#else
struct OggVorbis_File {

};
#endif
namespace GEngine {

	

	class GE_API OpenAL_source : public AudioSource {
	public:
		OpenAL_source();
		virtual ~OpenAL_source();


		virtual void Play() override;
		virtual void Pause() override;
		virtual void SetStatic(bool b) override;
		virtual void SetLoop(bool b) override;
		virtual void Destroy() override;
		virtual void SetVolume(float f) override;
		virtual void SetPitch(float f) override;
		virtual void Seek(float time) override;
		virtual void SetPosition(const Vector3f& pos) override;
		virtual void MaxDistance(const float f) override;
        virtual void Unload() override;
        virtual void Reload() override;
		virtual bool IsPlaying() override;
		OggVorbis_File oggFile;

	private:
		bool m_unloaded = false;
		int m_loadPos = 0;

	};



}
