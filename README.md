# Impluse Engine
[![Windows Release Build](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Windows_Editor_Release.yml/badge.svg?branch=master)](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Windows_Editor_Release.yml) [![MacOS Release Build](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Macos_Editor_Release.yml/badge.svg?branch=master)](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Macos_Editor_Release.yml) [![Linux Release Build](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Linux_Editor_Release.yml/badge.svg?branch=master)](https://github.com/ImpulsiveGames/ImpulsiveEngine/actions/workflows/Linux_Editor_Release.yml)


C++ Game Engine

![Untitled](https://user-images.githubusercontent.com/8605626/153977632-d30f3169-2bd1-4d46-b325-ef20fe0bbc62.png)

> ImpulsiveEngine is a game engine created to learn about cross platform game engine development. ImpulsiveEngine currently supports iOS, Android, MacOS, Linux, and Windows.

## Features (Currently working on all platforms)
- 3D/2D rendering
  - Sprite batching
  - Transparency ordering
  - Text rendering (UTF8-UTF32)
- Asset packing
  - Compress asset files into larger packed file that is streamed into memory
- Networking
  - Programatic port forwarding
  - Sends reliable UDP packets to clients
- 3D audio
  - Stream in ogg files then play them in a 3d world
- Editor
  - Game editor allows for putting together scenes, entities, and components to create a game
  - Content browser for navigating and creating assets in project
  - Serialize and Deserialize entities and components
- Rewarded Video Advertisements
  - Ads can be run on mobile devices via Firebase
- C++ scripting
  - C++ scripting that supports hot reloading and variable inspection via reflection

## 3rd Party Libraries
- OpenGL
- Freetype
- GLFW
- ImGui
- ENet
- Box2d
- Miniupnpc
- OpenAL
- Vorbis
- zip
- zlib
- premake5
- Firebase
