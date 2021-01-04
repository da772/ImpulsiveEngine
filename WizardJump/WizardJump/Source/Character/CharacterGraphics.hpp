#pragma once

#include "GEngine.h"


using namespace GEngine;

enum class MovementAnim {

	None = 0,
	Idle,
	Walk,
	Crouch, 
	Jump,
	Falling,
	Landing

};

class CharacterGraphics : public SpriteComponent {

public:
	CharacterGraphics() {};
	~CharacterGraphics() {};

	std::vector<std::function<void()>> animQue;

	MovementAnim animState = MovementAnim::None;
	int dir = 1;
	bool bIsWalking = false;
    bool bAnimating = false;

	void Walk(std::function<void()> f = nullptr) {

		bAnimating = true;
		if (animState == MovementAnim::Walk) return;
		animState = MovementAnim::Walk;
		m_animationComp->SetFrameAnimation(8, 8, true, [this, f](int frame) {
			if (frame == 2) {
				FootDown(0);
			}
			else if (frame == 6) {
				FootDown(1);
			}
			else if (frame == 8) {
				if (f) {
					bAnimating = false;
					ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
						});
					return;
				}
			}
			bIsWalking = true;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame-1 : frame, 1), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			
		});
	}
    
    void Crouch(std::function<void()> f= nullptr ) {
        if (animState == MovementAnim::Crouch) return;
        bAnimating = true;
        animState = MovementAnim::Crouch;
        m_animationComp->SetFrameAnimation(8, 2, false, [this, f](int frame) {
            if (frame == 2) {
                bAnimating = false;
                if (f) {
					ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
                        });
                }
                return;
            }
            bIsWalking = false;
            m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame-1+1 : frame+1, 0), Vector2f(74, 74), Vector2f(dir, 1));
            SetSubTexture(quad, m_characterSpriteSheet);
            
            });
    }

	void JumpCrouch(std::function<void()> f = nullptr) {
		if (animState == MovementAnim::Crouch) return;
		bAnimating = true;
		animState = MovementAnim::Crouch;
		m_animationComp->SetFrameAnimation(8, 3, false, [this, f](int frame) {
			if (frame == 3) {
				bAnimating = false;
				if (f) {
					ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
						});
				}
				return;
			}
			bIsWalking = false;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame-1 : frame, 0), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			
			});
	}

	void LandWalk(std::function<void()> f = nullptr) {
		if (animState == MovementAnim::Landing) return;
		bAnimating = true;
		animState = MovementAnim::Landing;
		m_animationComp->SetFrameAnimation(8, 4, false, [this, f](int frame) {
			if (frame == 1) {
				FootDown(2);
			} else if(frame == 4) {
				if (f) {
					bAnimating = false;
					ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
						});
					return;
				}
				return;
			}
			bIsWalking = false;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame+5 - 1 : frame+5, 0), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			});
	}
    
    void LandIdle(std::function<void()> f = nullptr) {
        if (animState == MovementAnim::Landing) return;
        bAnimating = true;
        animState = MovementAnim::Landing;
        m_animationComp->SetFrameAnimation(8, 3, false, [this, f](int frame) {
			if (frame == 1) {
				FootDown(2);
			} else if (frame == 3) {
                if (f) {
                    bAnimating = false;
                    ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
                        });
                    return;
                }
                return;
            } 
            bIsWalking = false;
			
            m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame+5 - 1 : frame+5, 0), Vector2f(74, 74), Vector2f(dir, 1));
            SetSubTexture(quad, m_characterSpriteSheet);
            });
    }

	void JumpStart(std::function<void()> f = nullptr) {
		if (animState == MovementAnim::Jump) return;
		bAnimating = true;
		animState = MovementAnim::Jump;
		m_animationComp->SetFrameAnimation(8, 2, false, [this, f](int frame) {
			if (frame == 2) {
				bAnimating = false;
				if (f) {
					ThreadPool::AddMainThreadFunction([f]() {
						if (f)
							f();
						});
				}
				return;
			}
			bIsWalking = false;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? frame+2 - 1 : frame+2, 0), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			});
	}

	void Idle(std::function<void()> f = nullptr) {
		if (animState == MovementAnim::Idle) return;
		animState = MovementAnim::Idle;
		m_animationComp->SetFrameAnimation(255, 1, false, [this, f](int frame) {
			bIsWalking = false;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? 0 : 1, 0), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			bAnimating = false;
			if (f) {
				ThreadPool::AddMainThreadFunction([f]() {
					if (f)
						f();
					});
			}
			});
	}

	void Falling(std::function<void()> f = nullptr) {
		if (animState == MovementAnim::Falling) return;
		animState = MovementAnim::Falling;
		m_animationComp->SetFrameAnimation(255, 8, false, [this,f](int frame) {
			bIsWalking = false;
			m_characterSpriteSheet->SetCoords(Vector2f(dir >= 0 ? 4 : 5, 0), Vector2f(74, 74), Vector2f(dir, 1));
			SetSubTexture(quad, m_characterSpriteSheet);
			bAnimating = false;
			if (f) {
				ThreadPool::AddMainThreadFunction([f]() {
					if (f)
						f();
					});
			}
		});
	}

	void SetDirectionIndicator(float dir) {
		if (directionIndicatorColor.w > 0) {
			indicatorDirection = dir;
			SetRotation(directionIndicator, dir);
		}
	}

	void ShowDirectionIndicator(bool b) {
		if (b && directionIndicatorColor.w <= 0.f) {
			directionIndicatorColor.w = 1.f;
			SetQuadColor(directionIndicator, directionIndicatorColor);
		}
		if (!b && directionIndicator != -1 ) {
			directionIndicatorColor.w = 0.f;
			SetQuadColor(directionIndicator, directionIndicatorColor);
		}
	}

	void SetPowerBar(float amt) {

		int frame = amt >= .08f ? GEMath::clamp((int)GEMath::MapRange(amt, 0.f, 1.f, 0, 11), 0, 11) : 0;
		powerIndicatorTexture->SetCoords({ (float)frame,0.f }, { 32.f,32.f });
		SetSubTexture(powerIndicator, powerIndicatorTexture);
	}

	void ShowPowerBar(bool b) {
		if (b && powerIndicatorColor.w <= 0.f) {
			powerIndicatorColor.w = 1.f;
			SetQuadColor(powerIndicator, powerIndicatorColor);
		}
		if (!b && powerIndicator != -1) {
			powerIndicatorColor.w = 0.f;
			SetQuadColor(powerIndicator, powerIndicatorColor);
		}
	}

	// called when foot touches ground (0 - left, 1 - right, 2 - both)
	void FootDown(uint8_t foot) {
		if (footDownCallback != nullptr) {
			const std::function<void(const uint8_t)>& cb = footDownCallback;
			ThreadPool::AddMainThreadFunction([cb, foot]() {
				cb(foot);
			});
		}
			
	}

	std::function<void(const uint8_t)> footDownCallback = nullptr;

protected:

	ShapeID directionIndicator = -1;
	float indicatorDirection = 0;
	ShapeID powerIndicator = -1;
	Vector3f powerIndicatorPos = Vector3f(.45f, .45f, 12.f);
	Vector3f powerIndicatorScale = Vector3f(.5f, .5f, 1.f);
	Vector4f powerIndicatorColor = Vector4f(1, 1, 1, 0.f);

	Vector3f directionIndicatorPos = Vector3f(0, .45f, 12.f);
	Vector3f directionIndicatorScale = Vector3f(.75f, .75f, 1.f);
	Vector4f directionIndicatorColor = Vector4f(1, 1, 1, 0.f);

	Ref<SubTexture2D> powerIndicatorTexture = nullptr;
	
	void OnBegin() override
	{
		SpriteComponent::OnBegin();
		m_characterSpriteSheet = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/wizard.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest),
			{ 0,1 }, { 74,74 }, { 1,1 });
		quad = CreateSubTexturedQuad({ 0.f,0.f,10.f }, 0.f, { 2.f,2.f,1.f }, { 1.f,1.f,1.f,1.f }, m_characterSpriteSheet);
		powerIndicatorTexture = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/jumpProgressBar.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest), { 0,0 }, { 32,32 });
		directionIndicator = CreateQuad(directionIndicatorPos, 0.f, directionIndicatorScale, directionIndicatorColor, Texture2D::Create("Content/Textures/halfCircle.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest));
		powerIndicator = CreateSubTexturedQuad(powerIndicatorPos, 0.f, powerIndicatorScale, powerIndicatorColor,
			powerIndicatorTexture);
		m_animationComp = CreateGameObject<SpriteAnimationComponent>();
		GetEntity()->AddComponent(m_animationComp);
		Idle();
		
	}


	void OnEnd() override
	{
		SpriteComponent::OnEnd();
		m_characterSpriteSheet = nullptr;
		m_animationComp = nullptr;
		powerIndicatorTexture = nullptr;
	}

	void PlayNextAnim() {
		if (animQue.size() > 0) {
			std::function<void()> f = animQue[0];
			animQue.erase(animQue.begin());
			ThreadPool::AddMainThreadFunction([f]() {
				f();
				});
		}
	}

	

	Ref<SpriteAnimationComponent> m_animationComp;
	Ref<SubTexture2D> m_characterSpriteSheet;
	ShapeID quad = -1;
	
};
