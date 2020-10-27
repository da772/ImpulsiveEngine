#pragma once

#include "WindowEntity.hpp"
#include "Lighting/LightComponent.h"

static float xSlope = -.25f;
static float ySlope = -.5f;

static glm::vec2 leftPos(-.15f, -0.0f), centerPos(0, -0.0f), rightPos(.5f, -0.0f);

static float xScale = 2.0f;
static float yScale = 2.0f;

static float rightSizeFalloffSize = .25f;

static std::vector<float> lightVert = {
			(leftPos.x + xSlope * 0.f) * xScale, (leftPos.y + ySlope * 0.f) * yScale,0.0f,0.f, // 0
			(centerPos.x + xSlope * 0.f) * xScale, (centerPos.y + ySlope * 0.f) * yScale,0.0f,.250f, // 1 CENTER
			.25f, (rightPos.y + ySlope * 0.f) * yScale,0.0f,.250f, // 2

			(leftPos.x + xSlope * 1.f) * xScale, (leftPos.y + ySlope * 1.f) * yScale,0.0f,1.0f, // 3
			(centerPos.x + xSlope * 1.f) * xScale, (centerPos.y + ySlope * 1.f) * yScale,0.0f,1.f, // 4 CENTER
			.25f, (rightPos.y + ySlope * 1.f) * yScale,0.0f,1.f, // 5

			(leftPos.x + xSlope * 2.f) * xScale, (leftPos.y + ySlope * 2.f) * yScale,0.0f,1.0f, // 6
			(centerPos.x + xSlope * 2.f) * xScale, (centerPos.y + ySlope * 2.f) * yScale,0.0f,1.0f, // 7CENTER
			.25f, (rightPos.y + ySlope * 2.f) * yScale,0.0f,1.f, // 8

			(leftPos.x + xSlope * 3.f) * xScale, (leftPos.y + ySlope * 3.f) * yScale,0.0f,0.50f, // 9
			(centerPos.x + xSlope * 3.f) * xScale, (centerPos.y + ySlope * 3.f) * yScale,0.0f,.50f, // 10 CENTER
			.15f, (rightPos.y + ySlope * 3.f) * yScale,0.0f,.5f, // 11

			(leftPos.x + xSlope * 4.f) * xScale, (leftPos.y + ySlope * 4.f) * yScale,0.0f,0.15f, // 12
			(centerPos.x + xSlope * 4.f) * xScale, (centerPos.y + ySlope * 4.f) * yScale,0.0f,0.15f, // 13 CENTER
			0.f, (rightPos.y + ySlope * 4.f) * yScale,0.0f,0.25f, // 14

			(leftPos.x + xSlope * 5.f) * xScale, (leftPos.y + ySlope * 5.f) * yScale,0.0f,0.0f, //15
			(centerPos.x + xSlope * 5.f) * xScale, (centerPos.y + ySlope * 5.f) * yScale,0.0f,0.0f, // 16 CENTER
			-.15f, (rightPos.y + ySlope * 5.f) * yScale,0.0f,0.00f, //17
			//NEW
			(leftPos.x * 2.f + xSlope * 0.f) * xScale, (leftPos.y + ySlope * 0.f) * yScale,0.0f,0.50f, // 18
			.25f + rightSizeFalloffSize, (rightPos.y + ySlope * 0.f) * yScale,0.0f,0.10f, // 19

			(leftPos.x * 2.f + xSlope * 1.f) * xScale, (leftPos.y + ySlope * 1.f) * yScale,0.0f,0.0f, // 20
			.25f + rightSizeFalloffSize, (rightPos.y + ySlope * 1.f) * yScale,0.0f,0.0f, // 21

			(leftPos.x * 2.f + xSlope * 2.f) * xScale, (leftPos.y + ySlope * 2.f) * yScale,0.0f,0.0f, // 22
			.25f + rightSizeFalloffSize, (rightPos.y + ySlope * 2.f) * yScale,0.0f,0.0f, // 23

			(leftPos.x * 2.f + xSlope * 3.f) * xScale, (leftPos.y + ySlope * 3.f) * yScale,0.0f,0.00f, // 24
			.15f + rightSizeFalloffSize, (rightPos.y + ySlope * 3.f) * yScale,0.0f,0.0f, // 25

			(leftPos.x * 2.f + xSlope * 4.f) * xScale, (leftPos.y + ySlope * 4.f) * yScale,0.0f,0.00f, // 26
			0.f + rightSizeFalloffSize, (rightPos.y + ySlope * 4.f) * yScale,0.0f,0.0f, // 27

			(leftPos.x * 2.f + xSlope * 5.f) * xScale, (leftPos.y + ySlope * 5.f) * yScale,0.0f,0.00f, //28
			-.15f + rightSizeFalloffSize, (rightPos.y + ySlope * 5.f) * yScale,0.0f,0.00f, // 29
};

static std::vector<uint32_t> lightIndices = {

	0, 3, 1, 3, 4, 1, //
		1, 4, 2, 4, 5, 2,

		3, 6, 4, 6, 7, 4,
		4, 7, 5, 7, 8, 5,

		6, 9, 7, 9, 10, 7,
		7, 10, 8, 10, 11, 8,

		9, 12, 10, 12, 13, 10,
		10, 13, 11, 13, 14, 11,

		12, 15, 13, 15, 16, 13,
		13, 16, 14, 16, 17, 14,

		18, 20, 0, 20, 3, 0,
		2, 5, 19, 5, 21, 19,
		20, 22, 3, 22, 6, 3,
		5, 8, 21, 8, 23, 21,
		22, 24, 6, 24, 9, 6,
		8, 11, 23, 11, 25, 23,
		24, 26, 9, 26, 12, 9,
		11, 14, 25, 14, 27, 25,
		26, 28, 12, 28, 15, 12,
		14, 17, 27, 17, 29, 27




};
static Ref<BufferLayout> lightLayout = Ref<BufferLayout>(new BufferLayout({
{GEngine::ShaderDataName::Position },
{GEngine::ShaderDataName::AlphaChannel} }));

WindowEntity::WindowEntity(const glm::vec3 position, const glm::vec2& scale, const glm::vec4& lightColor)
	: m_position(position), m_scale(scale), m_lightColor(lightColor)
{
	
}

void WindowEntity::OnBegin()
{

	SetEntityPosition(m_position);
	SetEntityScale({ m_scale.x,m_scale.y, 1 });
	m_lightComponent = CreateGameObject<LightComponent>();
	m_spriteComponent = CreateGameObject<SpriteComponent>();

	AddComponent(m_lightComponent);
	AddComponent(m_spriteComponent);


	//m_spriteComponent->CreateQuad({ 0,0,0 }, 0, { 1,1,1 }, { 1,1,1,1 }, Texture2D::Create("Content/Textures/bigWindow.png"));
	m_lightId = m_lightComponent->AddQuadLight({ 0.f,0.f }, 0, { 4.4f,4.5625f }, {1,1,1,1}, Texture2D::Create("Content/Textures/windowLight01.png"));


}

void WindowEntity::OnEnd()
{
	
}

void WindowEntity::OnUpdate(Timestep timestep)
{

}
