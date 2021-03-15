#include "Environment/FogEntity.h"

std::mutex FogEntity::m_mutex;
static unsigned char* m_lastNoiseData = nullptr;;
static uint64_t m_textureIds[3];
static float m_alpha = 1.f;
static uint16_t noiseDataSize = 128;
static SpriteComponent* m_spriteComponent = nullptr;
static uint64_t m_frame = 0;
static uint64_t m_frameProcessed = 0;
static uint8_t m_threadCount = 0;
static uint64_t m_frameProccesedTime = 0;
static uint8_t m_numberofThreads = 1;
static bool m_init = false;
static bool m_ending = false;
static float m_speed = 16; //MS
static FastNoiseLite noise;
static Ref<Texture2D> m_texture = nullptr;
static float entYscale = 1;

FogEntity::FogEntity(const uint32_t& id) : Entity(id)
{
	bUpdates = true;
	go_tag = "Fog";
	m_frame = 0; 
}

FogEntity::~FogEntity()
{
	m_ending = false;
	m_init = false;
}


void FogEntity::SetOctaves(int octaves)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_octaves = octaves;
	noise.SetFractalOctaves(m_octaves);
}

void FogEntity::SetFractalLacunarity(float lacuanuarity)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_lacunarity = lacuanuarity;
	noise.SetFractalLacunarity(m_lacunarity);
	
}

void FogEntity::SetFractalGain(float gain)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_gain = gain;
	noise.SetFractalGain(m_gain);
}

void FogEntity::SetFrequency(float freq)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_frequency = freq;
	noise.SetFrequency(m_frequency);
	
}

void FogEntity::SetSeed(int seed)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_seed = seed;
	noise.SetSeed(m_seed);
}

void FogEntity::SetTickSpeed(float speed)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_speed = speed;
}

void FogEntity::SetSpeed(float speed)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_moveSpeed = speed;
}

void FogEntity::OnUnloadGraphics() {
	if (m_spriteComponent) {
		m_spriteComponent->ClearQuads();
	}
	if (m_texture) {
		m_texture->SetData(nullptr, 0);
	}
    m_init = false;
}


void FogEntity::OnReloadGraphics() {
    RenderCommand::SetCulling(false);
}
void FogEntity::OnBegin()
{
	RenderCommand::SetCulling(false);
	/*
	if (Renderer::GetPipeline(m_pipelineId) == nullptr) {
		Ref<RenderPipeline> pipeline = std::shared_ptr<RenderPipeline>(new RenderPipeline_2d(Renderer::GetRenderScale(), "Renderpipeline_2d_Background"));
		Renderer::AddPipeline(m_pipelineId, pipeline,51);
		pipeline->SetSize(GEngine::Application::GetWidth(), GEngine::Application::GetHeight());
	}
	*/
	SetPosition({ 0,0,0 });
	//m_shader = Shader::Create("Content/shaders/FogShader.glsl");
	m_spriteComponent = AddComponent<SpriteComponent>(this);
	//m_audioComponent = CreateGameObject<AudioComponent>("Content/Audio/windLoop.ogg", true, true, true, .25f, .75f);
	//AddComponent(m_audioComponent);
	
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(m_octaves);
	noise.SetFractalLacunarity(m_lacunarity);
	noise.SetFractalGain(m_gain);
	noise.SetFrequency(m_frequency);
	noise.SetSeed(m_seed);
	//noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);


	
	m_texture = Texture2D::Create("fog_noise", noiseDataSize, noiseDataSize);
	

}

void FogEntity::OnEnd()
{
	RenderCommand::SetCulling(true);
	m_shader = nullptr;
	m_spriteComponent = nullptr;
	m_ending = true;
	m_init = false;
	m_texture = nullptr;
    if (m_lastNoiseData) {
        delete m_lastNoiseData;
        m_lastNoiseData = nullptr;
    }
}

void FogEntity::OnUpdate(Timestep timestep)
{
	m_time += timestep;
	

	while (m_threadCount < 1) {
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			entYscale = GetScale().y;
			m_threadCount++;
		}
		
		int movespeed = m_moveSpeed;
		float noiseStrength = m_noiseStrength;
		float znoiseStrength = m_zNoiseStrength;

		ThreadPool::AddJob([movespeed, noiseStrength, znoiseStrength]() {


			ThreadPool::GetPauseMutex().lock();
			ThreadPool::GetPauseMutex().unlock();
           
			unsigned char* noiseData = new unsigned char[noiseDataSize * noiseDataSize * 4];
            
            if (m_lastNoiseData) {
				memcpy(noiseData, m_lastNoiseData, (noiseDataSize*noiseDataSize*4)*sizeof(unsigned char));
            }
			int frame = 0;
			uint64_t cTime = Time::GetEpochTimeMS();
			while (cTime - m_frameProccesedTime < m_speed) {
				cTime = Time::GetEpochTimeMS();
				continue;
			}
			m_frameProccesedTime = cTime;

			{
				if (m_ending) {
					delete[] noiseData;
					noiseData = nullptr;
					m_threadCount--;
					return;
				}
				std::lock_guard<std::mutex> guard(m_mutex);
				frame = ++m_frame;
				
			}
			int index = 0;
			
            if (noiseData) {
                for (int y = 0; y < noiseDataSize; y++)
                {
                    for (int x = 0; x < noiseDataSize; x++)
                    {
                        unsigned char c;
                        {
                            std::lock_guard<std::mutex> guard(m_mutex);
                            c = GEMath::clamp((int)(255.f * noise.GetNoise((float)(x+(float)frame/ movespeed *noiseStrength), (float)y* noiseStrength, (float)frame/ movespeed *noiseStrength * znoiseStrength)), 0, 255);
                        }
						float _fade = .2f;
                        noiseData[index++] = 255;
                        noiseData[index++] = 255;
                        noiseData[index++] = 255;
						noiseData[index++] = c;
                    }
                }
            } else {
                int nrCols = 32;
                index = (noiseDataSize-nrCols)*noiseDataSize*4;
                for (int y = 0; y < noiseDataSize; y++) {
                    for (int x = noiseDataSize-nrCols; x < noiseDataSize; x++) {
                        unsigned char c;
                        {
                            std::lock_guard<std::mutex> guard(m_mutex);
                            c = GEMath::clamp((int)(255.f * noise.GetNoise((float)(x+(float)frame* noiseStrength), (float)y* noiseStrength, (float)frame* noiseStrength * znoiseStrength)), 0, 255);
                        }
						float _fade = .1f;
                        noiseData[index++] = 255;
                        noiseData[index++] = 255;
                        noiseData[index++] = 255;
						if (x < (noiseDataSize * _fade)) {
							c *= (x/(noiseDataSize*_fade)) ;
							GE_LOG_DEBUG("NOISEDATA: {0}, {1}, {2}", x, noiseDataSize * _fade, x);
						}
						else if (x > (noiseDataSize * (1.f - _fade))) {
							c *=  (x / (noiseDataSize * (1.f-_fade)));
							GE_LOG_DEBUG("NOISEDATA: {0}, {1}, {2}", x, noiseDataSize * (1.f - _fade), x);
						}
						noiseData[index++] = c;
                    }
                }
            }
            
            
			int cFrame = 0;
			{
				if (m_ending) { m_threadCount--; return; };
				std::lock_guard<std::mutex> guard(m_mutex);
				cFrame = m_frameProcessed;
			}

			uint64_t ct = Time::GetEpochTimeMS();


			ThreadPool::GetPauseMutex().lock();
			ThreadPool::GetPauseMutex().unlock();
			
			ThreadPool::AddMainThreadFunction([frame, noiseData]() {
				if (Application::IsGamePaused() || !m_texture) {
					if (m_lastNoiseData) {
						delete m_lastNoiseData;
						m_lastNoiseData = nullptr;
					}
					delete noiseData;
					m_threadCount--;
					return;
				}
				std::lock_guard<std::mutex> guard(m_mutex);
				m_texture->SetData(noiseData, noiseDataSize * noiseDataSize * 4, TEXTUREFLAGS_Mag_Linear_MipMap | TEXTUREFLAGS_Min_Linear_MipMap |  TEXTUREFLAGS_Wrap_ClampToEdge);
				if (!m_init) {
					//m_spriteId = m_spriteComponent->CreateQuad({ 0,0,15 }, 0, { 10,20, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					m_textureIds[0] = m_spriteComponent->CreateQuad({ -5.f,-.7f,15 }, 0, { 12.5f,2.f, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,.160f });
					m_textureIds[1] = m_spriteComponent->CreateQuad({ 30.650f,-22.f,-10.f }, 0, { 12.5f,12.5f, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
                    //m_textureIds[1] = m_spriteComponent->CreateQuad({ 2.f,-3.5f,15 }, 0, { -1.f,7.5f, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, 0 }, m_texture, { 1.f,1.f });
					
                  //  m_textureIds[2] = m_spriteComponent->CreateQuad({ -4.75f,-17.25f* entYscale,15 }, 0, { 13.f,-20.f, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					//m_spriteId = m_spriteComponent->CreateQuad({ 0,0,15 }, 0, { 20,7.5, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					//m_spriteId = m_spriteComponent->CreateQuad({ 0,-3.75,15 }, 0, { 20,-7.5, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					//m_spriteId = m_spriteComponent->CreateQuad({ 0,3.75,15 }, 0, { 20,7.5, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					//m_spriteId = m_spriteComponent->CreateQuad({ 0,-11.25f,15 }, 0, { 20,7.5, 1 }, { 250.f / 255.f, 235.f / 255.f, 202.f / 255.f, m_alpha }, m_texture, { 1.f,1.f });
					
					m_init = true;
				}
				//GE_CORE_DEBUG("Fog Frame: {0}, Delay: {1}", frame, Time::GetEpochTimeMS() - m_frameProccesedTime);
				m_frameProccesedTime = Time::GetEpochTimeMS();
				m_frameProcessed = frame;
				m_threadCount--;
                if (m_lastNoiseData) {
                    delete m_lastNoiseData;
                    m_lastNoiseData = nullptr;
                }
                m_lastNoiseData = noiseData;
				//delete[] noiseData;
				
				});

			});

	}

	
	//m_spriteComponent->SetTextureScale(m_spriteId, { m_time*.5f, .3f * sin(.5f * (m_time))});


}
