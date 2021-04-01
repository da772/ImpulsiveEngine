#pragma once
#include <GEngine.h>

using namespace GEngine;


class ParticleSystem2D_Renderable;

class ParticleSystem2D : public Component {


public:
	ParticleSystem2D(Entity* e, const uint32_t& count, const Vector3f& startPos, const Vector2f& vel, const Vector2f& startScale, const Vector2f& endScale, Ref<Texture2D> m_texture, Ref<Shader> m_shader, const Vector4f& startColor, const Vector4f& endColor, const float& lifespan, bool loop = true);
	~ParticleSystem2D();

	inline Vector4f GetStartColor() const { return m_startColor; }
	inline Vector4f GetEndColor() const { return m_endColor; }
	inline Vector3f GetStartPosition() const { return m_startPosition; }
	inline Vector2f GetVelocity() const { return m_velocity; }
	inline Vector2f GetStartScale() const { return m_startScale; }
	inline Vector2f GetEndScale()const { return m_endScale; }
	inline uint32_t GetParticleCount() const { return m_particleCount; }
	inline Ref<Shader> GetShader() const { return m_shader; }
	inline Ref<Texture2D> GetTexture() const { return m_texture; }
	inline bool DoesLoop() const { return m_loop; }
	inline float GetLifeSpan() const { return m_lifeSpan; }


	void UnloadGraphics() override;
	void ReloadGraphics() override;

private:
	Vector3f m_startPosition;
	Vector4f m_startColor;
	Vector4f m_endColor;
	float m_lifeSpan = 0;
	Vector2f m_velocity;
	Vector2f m_startScale;
	Vector2f m_endScale;
	bool m_loop;
	uint32_t m_particleCount;
	Ref<ParticleSystem2D_Renderable> m_renderable;
	Ref<RenderPipeline> m_pipeline;
	Ref<Texture2D> m_texture;
	Ref<Shader> m_shader;
	float m_lastRenderableTime = 0;
		
protected:
	void OnBegin() override;
	void OnEnd() override;

};

class ParticleSystem2D_Renderable : public Renderable {

public:
	ParticleSystem2D_Renderable(ParticleSystem2D* system, float time = 0);
	~ParticleSystem2D_Renderable();
	void Render() override;
	inline uint64_t GetLastRenderTime() const { return m_lastTime; }
private:
	float m_lastTime = 0;
	float m_totalTime = 0;
	ParticleSystem2D* m_system;
	Ref<VertexArray> m_vertexArray;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;

};