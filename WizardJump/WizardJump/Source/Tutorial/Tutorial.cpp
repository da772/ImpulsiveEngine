#include "Tutorial/Tutorial.hpp"
#include "UI/DialogFrame.hpp"
#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"

static const Vector4f color = { 0.078f,.039f,0.031f,1 };

void Tutorial::CreateMainTutorial(Weak<Scene> s, Weak<CharacterEntity> characterEntity)
{
	//AddEntity(CreateGameObject<CloudGenerator>(Vector3f(20, 0, 14), Vector3f(-20, 0, 14), Vector2f(-.5f, .5f), 1.f, 10));

	//tutorialCover = FPSuiComponent->CreateQuad({ -.5f,0,0 }, 0, { 0.f,0,1 }, { .25f,.25f,.25f,0 });
	
	characterEntity.lock()->m_characterComponent->bEnableInput = false;
	characterEntity.lock()->m_characterComponent->bEnableJump = false;
	Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", color, "This is a big mountain. I wonder how I got up here. Maybe I should search that tower for clues so I can get out of here! \n(Tap here to continue...)", true, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));


	dialog->SetOnDialogComplete([s, characterEntity]() {

		Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", color, "Press and hold on the left side of the screen to move to the left!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
		dialog->SetStickyFrame(true);
		s.lock()->AddEntity(dialog);
		Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
		dialog->AddComponent(spriteAnim);
		spriteAnim->SetFrameAnimation(30, 15, false, [s](int frame) {
			//GE_LOG_DEBUG("FRAME ALPHA: {0}", (float)frame * (.9f / 15.f));
			//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame * (.9f / 15.f) });
			//FPSuiComponent->SetScale(tutorialCover, { (float)frame * (1.f / 15.f),(float)frame * (2.f / 15.f),.0f });
			});
		spriteAnim->Start();

		characterEntity.lock()->m_characterComponent->bEnableInput = true;

		characterEntity.lock()->m_characterComponent->SetInputFilterFunction([s, characterEntity, dialog](const GEngine::FTouchInfo& touch) {
			// ensure we are moving right!
			if ((touch.state == 0 || touch.state == 1) && touch.x >= (float)Application::GetWidth() / 2.f) {
				return false;
			}
			else {
				if (characterEntity.lock()->m_characterComponent->bodyComp->GetVelocity().x < -2.f || characterEntity.lock()->GetEntityPosition().x < -6.f) {
					
					characterEntity.lock()->m_characterComponent->bodyComp->SetVelocityX(0);
					characterEntity.lock()->m_characterComponent->bEnableInput = false;
					Ref<DialogFrame> _dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", color, "Press and hold on the right side of the screen to move to the right!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
					_dialog->SetStickyFrame(true);
					//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f, 0.f });
					//FPSuiComponent->SetPosition(tutorialCover, { .5f, 0 });
					Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
					spriteAnim->SetFrameAnimation(30, 15, false, [s](int frame) {
						//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame * (.9f / 15.f) });
						//FPSuiComponent->SetScale(tutorialCover, { (float)frame * (1.f / 15.f),(float)frame * (2.f / 15.f),.0f });
						});
					s.lock()->AddEntity(_dialog);
					dialog->Destroy();
					_dialog->AddComponent(spriteAnim);
					spriteAnim->Start();
					characterEntity.lock()->m_characterComponent->bEnableInput = true;
					characterEntity.lock()->m_characterComponent->SetInputFilterFunction([s,characterEntity, _dialog](const GEngine::FTouchInfo& touch) {
						// ensure we are moving left!
						if ((touch.state == 0 || touch.state == 1) && touch.x <= (float)Application::GetWidth() / 2.f) {
							return false;
						}
						else {
							if (characterEntity.lock()->m_characterComponent->bodyComp->GetVelocity().x > 200.f || characterEntity.lock()->GetEntityPosition().x > -2.25f) {
								characterEntity.lock()->m_characterComponent->bodyComp->SetVelocityX(0);
								characterEntity.lock()->m_characterComponent->bEnableInput = false;
								characterEntity.lock()->m_characterComponent->bEnableWalk = false;
								Ref<DialogFrame> __dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", color, "There is a box in the way. Maybe there is a way to get around it?. \n(Tap here to continue...)", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
								s.lock()->AddEntity(__dialog);
								_dialog->Destroy();
								//FPSuiComponent->Remove(tutorialCover);
								__dialog->SetOnDialogComplete([s,characterEntity]() {
									characterEntity.lock()->m_characterComponent->bEnableInput = true;
									// Enable jump during jump 
									characterEntity.lock()->m_characterComponent->bEnableJump = true;
									// Set walk true after jump tutorial
									characterEntity.lock()->m_characterComponent->bEnableWalk = false;

									Ref<DialogFrame> ___dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", color, "Click and drag to jump.", false, true);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
									___dialog->SetStickyFrame(true);
									s.lock()->AddEntity(___dialog);
									characterEntity.lock()->m_characterComponent->bdrawTrajectory = true;
									characterEntity.lock()->m_characterComponent->trajectoryColor = { .2,.2,.2,1.f };
									characterEntity.lock()->m_characterComponent->trajectoryOffset = characterEntity.lock()->m_characterComponent->bodyComp->m_quadCollider->GetQuadCollider(characterEntity.lock()->m_characterComponent->bodyComp->circleColliderID).scale.x;
									characterEntity.lock()->m_characterComponent->SetInputFilterFunction([s, characterEntity, ___dialog](const GEngine::FTouchInfo& touch) {
										// ensure that we jumped
										if (touch.state == 2 && characterEntity.lock()->m_characterComponent->GetIsJumping() ) {
											
											if (characterEntity.lock()->m_characterComponent->GetPredictedPosition().x < 0.81f) {
												characterEntity.lock()->m_characterComponent->ResetInput();
												return false;
											}
											___dialog->Destroy();
											Ref<DialogFrame> ____dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard",  "Content/Textures/wiz10_face.png", color, "(Tap to Continue...)", false, true);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
											characterEntity.lock()->m_characterComponent->bEnableJump = false;
											____dialog->SetOnDialogComplete([s, characterEntity]() {
												characterEntity.lock()->m_characterComponent->trajectoryOffset = 0.f;
												characterEntity.lock()->m_characterComponent->bEnableWalk = true;
												characterEntity.lock()->m_characterComponent->bEnableJump = true;
												characterEntity.lock()->m_characterComponent->ClearTrajectory();
												characterEntity.lock()->m_characterComponent->bdrawTrajectory = false;
												characterEntity.lock()->m_characterComponent->SetInputFilterFunction(nullptr);
											});
											s.lock()->AddEntity(____dialog);
										}
										else {
											//GE_CORE_DEBUG("Predicted Pos: {0}",characterEntity.lock()->m_characterComponent->GetPredictedPosition().x);
											if (characterEntity.lock()->m_characterComponent->GetPredictedPosition().x < 0.81f) {
												characterEntity.lock()->m_characterComponent->trajectoryColor = { .2,.2,.2,1.f };
											}
											else {
												characterEntity.lock()->m_characterComponent->trajectoryColor = { 1,1,1,1.f };
											}
										}

										return true;
										});

									});
								characterEntity.lock()->m_characterComponent->SetInputFilterFunction(nullptr);

							}

						}


						return true;
						});
					
				}

			}

			return true;
			});

		});
	s.lock()->AddEntity(dialog);
}
