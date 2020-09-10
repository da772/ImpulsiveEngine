#include "Environment/BackgroundTiledEntity.hpp"

#include "Environment/PlatformEntity.hpp"



std::unordered_map<EPlatformType, glm::vec2> platformPositions1 = {
	{EPlatformType::TOP, {0,0}}, {EPlatformType::BOTTOM, {1,0}},{ EPlatformType::TOP_RIGHT, {2,0}}, {EPlatformType::MIDDLE_RIGHT, {3,0}}, {EPlatformType::MIDDLE, {4,0}},
	{EPlatformType::MIDDLE_LEFT, {5,0}}, {EPlatformType::BOTTOM_LEFT, {6,0}}, {EPlatformType::BOTTOM_RIGHT, {7,0}}, {EPlatformType::TOP_LEFT, {8,0}} };



void BackgroundTiledEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ scale.x,scale.y,1 });

	m_sprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_sprite);


	rows = scale.y / rowSize;
	columns = scale.x / columnSize;



	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			if (x == 12 || x == 14 || x == 16 || x == 18)
				continue;
			EPlatformType e = (EPlatformType)GetPlatformType(x, y);
			m_sprite->CreateSubTexturedQuad({ (x * columnSize + (columnSize) / 2.f) - (scale.x / 2.f), -y * rowSize + (scale.y / 2.f) - (rowSize) / 2.f, -1 }, 0, { columnSize / (float)scale.x,  rowSize / (float)scale.y , 1 }
				, { 1.f, 1.f, 1.f,1 },
				SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/bricks-spritesheet.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest), platformPositions1[e], { 16,16 }, { 1,1 }));

		}
	}

	/*
	m_sprite->CreateSubTexturedQuad({ 0, 0,0 }, rot, { 1,1,1 }, { 1,1,1,1.f },
		SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/bluishBricks.png", TEXTUREFLAGS_Wrap_Repeat), { 0,0 }, { 64,64 }, { 1,1 }), { scale.x / 4.f, scale.y / 4.f });

	*/


}

void BackgroundTiledEntity::OnEnd()
{
	m_debugSprite = nullptr;
	m_sprite = nullptr;
}

void BackgroundTiledEntity::OnUpdate(Timestep timestep)
{

}

const uint8_t BackgroundTiledEntity::GetPlatformType(int x, int y)
{
	EPlatformType e;

	return (uint8_t)EPlatformType::MIDDLE;

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

	return (uint8_t) e;

}
