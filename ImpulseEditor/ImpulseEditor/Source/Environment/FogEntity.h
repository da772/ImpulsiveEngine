#pragma once


#include <GEngine.h>

#include "Vendor/FastNoiseLite.h"

using namespace GEngine;


class FogEntity : public Entity {


public:
	FogEntity(const uint32_t& id);
	~FogEntity();

	//inline FastNoiseLite& GetNoise() { return { }; }

	void SetOctaves(int octaves);
	void SetFractalLacunarity(float lacuanuarity);
	void SetFractalGain(float gain);
	void SetFrequency(float freq);
	void SetSeed(int seed);
	void SetTickSpeed(float speed);
	void SetSpeed(float speed);

	float m_gain = .5f, m_frequency = .01f, m_lacunarity = 2.f;
	int m_octaves = 4, m_seed = 0;


	
	float m_moveSpeed = 2.f;
    
    static std::mutex m_mutex;

protected:
	
	inline void OnBegin();
	Ref<Shader> m_shader;
	float m_time = 0;
	
    
    virtual void OnUnloadGraphics() override;
    virtual void OnReloadGraphics() override;

	
  
    


	float m_noiseStrength = 4.f;
	float m_zNoiseStrength = .5f;

	
	uint64_t m_spriteId = 0;
	uint64_t m_pitchAdjust = 0;

	

	const char* m_pipelineId = "background";

	inline void OnEnd();
    


	void OnUpdate(Timestep timestep);

};
