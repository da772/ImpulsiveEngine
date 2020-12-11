#pragma once


#include <GEngine.h>

#include "Vendor/FastNoiseLite.h"

using namespace GEngine;


class FogEntity : public Entity {


public:
	FogEntity();
	~FogEntity();

	inline FastNoiseLite& GetNoise() { return noise; }

	void SetOctaves(int octaves);
	void SetFractalLacunarity(float lacuanuarity);
	void SetFractalGain(float gain);
	void SetFrequency(float freq);
	void SetSeed(int seed);

	float m_gain = .5f, m_frequency = .01f, m_lacunarity = 2.f;
	int m_octaves = 4, m_seed = 0;

protected:
	FastNoiseLite noise;
	uint16_t noiseDataSize = 128;
	inline void OnBegin();
	Ref<Shader> m_shader;
	float m_time = 0;
	Ref<Texture2D> m_texture = nullptr;
	Ref<SpriteComponent> m_spriteComponent;
	Ref<AudioComponent> m_audioComponent;
    
    virtual void OnUnloadGraphics() override;
    virtual void OnReloadGraphics() override;

	float m_alpha = 1.f;
    
    uint64_t m_textureIds[3];

	float m_speed = 16; //MS

	float m_noiseStrength = 4.f;
	float m_zNoiseStrength = .5f;

	std::mutex m_mutex;
	uint64_t m_spriteId = 0;

	uint64_t m_pitchAdjust = 0;

	bool m_init = false;

	uint64_t m_frame = 0;
	uint64_t m_frameProcessed = 0;
	uint8_t m_threadCount = 0;
	uint64_t m_frameProccesedTime = 0;
	uint8_t m_numberofThreads = 1;
	bool m_ending = false;


	const char* m_pipelineId = "background";

	inline void OnEnd();
    
    unsigned char* m_lastNoiseData = nullptr;;


	void OnUpdate(Timestep timestep);

};
