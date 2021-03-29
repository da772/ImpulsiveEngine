#include "gepch.h"
#include "Public/Platform/Audio/Server/Server_AudioContext.h"
#include "Public/Platform/Audio/Server/Server_AudioSource.h"

namespace GEngine {

    Server_AudioContext::Server_AudioContext() {
        s_source = make_shared<Server_AudioSource>();
    }

}