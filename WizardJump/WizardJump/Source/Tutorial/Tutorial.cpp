#include "Tutorial/Tutorial.hpp"
#include "UI/DialogFrame.hpp"
#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"

void Tutorial::CreateMainTutorial(Ref<Scene> s, Ref<CharacterEntity> characterEntity)
{
	//AddEntity(CreateGameObject<CloudGenerator>(Vector3f(20, 0, 14), Vector3f(-20, 0, 14), Vector2f(-.5f, .5f), 1.f, 10));

	//tutorialCover = FPSuiComponent->CreateQuad({ -.5f,0,0 }, 0, { 0.f,0,1 }, { .25f,.25f,.25f,0 });

	characterEntity->m_characterComponent->bEnableInput = false;
	characterEntity->m_characterComponent->bEnableJump = false;
	Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "This is a big mountain. I wonder how I got up here. Maybe I should search that tower for clues so I can get out of here! \n(Tap here to continue...)", true, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));


	dialog->SetOnDialogComplete([s, characterEntity]() {

		Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "Press and hold on the right side of the screen to move to the right!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
		dialog->SetStickyFrame(true);
		s->AddEntity(dialog);
		Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
		dialog->AddComponent(spriteAnim);
		spriteAnim->SetFrameAnimation(30, 15, false, [s](int frame) {
			GE_LOG_DEBUG("FRAME ALPHA: {0}", (float)frame * (.9f / 15.f));
			//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame * (.9f / 15.f) });
			//FPSuiComponent->SetScale(tutorialCover, { (float)frame * (1.f / 15.f),(float)frame * (2.f / 15.f),.0f });
			});
		spriteAnim->Start();

		characterEntity->m_characterComponent->bEnableInput = true;

		characterEntity->m_characterComponent->SetInputFilterFunction([s, characterEntity, dialog](const GEngine::FTouchInfo& touch) {
			// ensure we are moving right!
			if ((touch.state == 0 || touch.state == 1) && touch.x <= (float)Application::GetWidth() / 2.f) {
				return false;
			}
			else {
				if (characterEntity->m_characterComponent->bodyComp->GetVelocity().x > 2.f || characterEntity->GetEntityPosition().x > -8.f) {
					dialog->Destroy();
					characterEntity->m_characterComponent->bodyComp->SetVelocityX(0);
					characterEntity->m_characterComponent->bEnableInput = false;
					Ref<DialogFrame> _dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "Press and hold on the left side of the screen to move to the left!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
					_dialog->SetStickyFrame(true);
					//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f, 0.f });
					//FPSuiComponent->SetPosition(tutorialCover, { .5f, 0 });
					Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
					spriteAnim->SetFrameAnimation(30, 15, false, [s](int frame) {
						//FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame * (.9f / 15.f) });
						//FPSuiComponent->SetScale(tutorialCover, { (float)frame * (1.f / 15.f),(float)frame * (2.f / 15.f),.0f });
						});
					s->AddEntity(_dialog);
					_dialog->AddComponent(spriteAnim);
					spriteAnim->Start();
					characterEntity->m_characterComponent->bEnableInput = true;
					characterEntity->m_characterComponent->SetInputFilterFunction([characterEntity, _dialog,s](const GEngine::FTouchInfo& touch) {
						// ensure we are moving left!
						if ((touch.state == 0 || touch.state == 1) && touch.x >= (float)Application::GetWidth() / 2.f) {
							return false;
						}
						else {
							if (characterEntity->m_characterComponent->bodyComp->GetVelocity().x < -2.f || characterEntity->GetEntityPosition().x < -10.0f) {
								_dialog->Destroy();
								characterEntity->m_characterComponent->bodyComp->SetVelocityX(0);
								characterEntity->m_characterComponent->bEnableInput = false;
								Ref<DialogFrame> __dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "I should keep walking to the right. \n(Tap here to continue...)", false, true);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
								s->AddEntity(__dialog);
								//FPSuiComponent->Remove(tutorialCover);
								__dialog->SetOnDialogComplete([characterEntity]() {
									characterEntity->m_characterComponent->bEnableInput = true;
									// Enable jump during jump tutorial
									characterEntity->m_characterComponent->bEnableJump = true;
									});
								characterEntity->m_characterComponent->SetInputFilterFunction(nullptr);

							}

						}


						return true;
						});
					
				}

			}

			return true;
			});

		});
	s->AddEntity(dialog);
}
