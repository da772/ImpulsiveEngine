
#include "gepch.h"

#include "Public/Core/Util/Utility.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Controller/CameraController.h"
#include <glm/gtc/matrix_transform.hpp>

#ifdef GE_PLATFORM_WINDOWS

#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#endif

#ifdef GE_PLATFORM_ANDROID
#include "Public/Core/Platform/Window/Android/android_util.h"
#include <arpa/inet.h>
#endif

#ifdef GE_PLATFORM_IOS
#include <CoreFoundation/CoreFoundation.h>
#include <arpa/inet.h>
#endif

#ifdef GE_PLATFORM_MACOSX
#include <arpa/inet.h>
#endif


namespace GEngine {

	std::string Utility::IPV4ToString(uint32_t ip)
	{
#ifdef GE_PLATFORM_WINDOWS
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#else 
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#endif
	}
	

	glm::vec3 Utility::ScreenPosToWorldPos(glm::vec2 pos)
	{
		glm::vec3 position = { pos.x,  pos.y, 1 };
		int width, height;
		GEngine::Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);
		//width = GEngine::Application::GetApp()->GetWindow()->GetWidth();
		//height = GEngine::Application::GetApp()->GetWindow()->GetHeight();
		position = glm::unProject(position, Application::GetApp()->GetTargetCamera()->GetViewMatrix(),
			Application::GetApp()->GetTargetCamera()->GetProjectionMatrix(), glm::vec4(0, height, width, -height));
		return position;
	}

	const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string Utility::GenerateHash(const int len)
	{
		std::string s;
		for (int i = 0; i < len; ++i) {
			s += alphanum[std::rand() % (sizeof(alphanum) - 1)];
		}
		return s;
	}

	void Utility::GenerateHash(char* str, const int len)
	{
		for (int i = 0; i < len; ++i) {
			str[i] = alphanum[std::rand() % (sizeof(alphanum) - 1)];
		}
	}

}
