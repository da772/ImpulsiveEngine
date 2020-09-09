#include "Environment/PlatformEntity.hpp"

std::unordered_map<EPlatformType, glm::vec2> platformPositions = {
	{EPlatformType::TOP, {0,0}}, {EPlatformType::BOTTOM, {1,0}},{ EPlatformType::TOP_RIGHT, {2,0}}, {EPlatformType::TOP_LEFT, {3,0}}, {EPlatformType::MIDDLE, {4,0}},
	{EPlatformType::MIDDLE_RIGHT, {5,0}}, {EPlatformType::MIDDLE_LEFT, {6,0}}, {EPlatformType::BOTTOM_LEFT, {7,0}}, {EPlatformType::BOTTOM_RIGHT, {8,0}} };



void PlatformEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ scale.x,scale.y,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		// Bottom
		m_debugSprite->CreateQuad({ 0, scale.y*-.025f,5 }, rot, { 1,.95f,1 }, { 0,0,0,.45f });
		// Top
		m_debugSprite->CreateQuad({ 0, scale.y*.475f,6 }, rot, { 1,.05f,1.f }, { 0,1,0,.45f });


		// Test Texture
		
	}

	m_sprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_sprite);


	rows = scale.y / rowSize;
	columns = scale.x / columnSize;



	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			EPlatformType e = GetPlatformType(x, y);
			m_sprite->CreateSubTexturedQuad({ (x * columnSize+(columnSize)/2.f )-(scale.x/2.f), -y * rowSize + (scale.y / 2.f) - (rowSize) / 2.f, 1 }, 0, {columnSize / (float)scale.x,  rowSize / (float)scale.y , 1 }
				, { 1,1,1,1 },
				SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/bricks-spritesheet.png"), platformPositions[e], { 16,16 }, { 1,1 }));

		}
	}

	/*
	m_sprite->CreateSubTexturedQuad({ 0, 0,0 }, rot, { 1,1,1 }, { 1,1,1,1.f },
		SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/bluishBricks.png", TEXTUREFLAGS_Wrap_Repeat), { 0,0 }, { 64,64 }, { 1,1 }), { scale.x / 4.f, scale.y / 4.f });

	*/


	m_colliderWall = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, scale.y * -.025f), glm::vec2(1, .95f), rot, 0.f);
	m_colliderWall->SetTag("wall");
	AddComponent(m_colliderWall);
	m_colliderWall->SetBounce(.5f);
	

	
	m_colliderFloor = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, scale.y * .475f), glm::vec2(1, .05f), rot, 0.f);
	m_colliderFloor->SetTag("ground");
	AddComponent(m_colliderFloor);


}

void PlatformEntity::OnEnd()
{
	m_debugSprite = nullptr;
	m_colliderFloor = nullptr;
	m_colliderWall = nullptr;
	m_sprite = nullptr;
}

void PlatformEntity::OnUpdate(Timestep timestep)
{

}

const EPlatformType PlatformEntity::GetPlatformType(int x, int y)
{
	EPlatformType e;

	if (y == 0) {
		if (x == 0) {
			e = EPlatformType::TOP_LEFT;
		}
		else if (x == columns - 1) {
			e = EPlatformType::TOP_RIGHT;
		}
		else {
			e = EPlatformType::TOP;
		}
	}
	else if (y == rows - 1) {
		if (x == 0) {
			e = EPlatformType::BOTTOM_LEFT;
		}
		else if (x == columns - 1) {
			e = EPlatformType::BOTTOM_RIGHT;
		}
		else {
			e = EPlatformType::BOTTOM;
		}
	}
	else if (x == 0) {
		e = EPlatformType::MIDDLE_LEFT;
	}
	else if (x == columns - 1) {
		e = EPlatformType::MIDDLE_RIGHT;
	}
	else {
		// OR SWAP THIS WITH BLACK?
		e = EPlatformType::MIDDLE;
	}
	
	return e;

}
