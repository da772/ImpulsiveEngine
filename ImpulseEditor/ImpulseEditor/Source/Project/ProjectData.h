#pragma once
#include <GEngine.h>


namespace Project {

	enum class ProjectDataLanguages : uint32_t {
		NONE = 0, NATIVE = 0x1, SCRIPTING = 0x02
	};

	struct ProjectData {
		std::string name;
		GEngine::Ref<GEngine::Texture2D> thumbNail;
		std::string path;
		std::string time;
		uint64_t lastModified;
		uint32_t languages;


		inline bool isValid() const {
			return name.size() > 0 && path.size() > 0;
		}

		inline bool isNative() const {
			return languages & (uint32_t)ProjectDataLanguages::NATIVE;
		}


		friend std::ostream& operator << (std::ostream& out, const ProjectData& c) {
			char name[64] = { 0 };
			memcpy(name, c.name.data(), c.name.size());
			out.write(name, 64 * sizeof(char));
			char path[1024] = { 0 };
			memcpy(path, c.path.data(), c.path.size());
			out.write(path, sizeof(char) * 1024);
			char thumbnailPath[1024] = { 0 };
			if (c.thumbNail)
				memcpy(thumbnailPath, c.thumbNail->GetName().data(), c.thumbNail->GetName().size());
			out.write(thumbnailPath, 1024 * sizeof(char));
			char time[256] = { 0 };
			memcpy(time, c.time.data(), c.time.size());
			out.write(time, sizeof(time));
			out.write((char*)&c.lastModified, sizeof(uint64_t));
			out.write((char*)&c.languages, sizeof(uint32_t));
			return out;
		};

		friend std::istream& operator>>(std::istream& in, ProjectData& c) {
			char name[64] = { 0 };
			in.read(name, 64 * sizeof(char));
			char path[1024] = { 0 };
			in.read(path, 1024 * sizeof(char));
			char texture[1024] = { 0 };
			in.read(texture, 1024 * sizeof(char));
			char time[256] = { 0 };
			in.read(time, 256 * sizeof(char));
			uint64_t lastModified = 0;
			in.read((char*)&lastModified, sizeof(uint64_t));
			uint32_t langauges = 0;
			in.read((char*)&langauges, sizeof(uint32_t));
			if (strlen(texture) > 0)
				c.thumbNail = GEngine::Texture2D::Create(texture);
			c.name = name;
			c.path = path;
			c.time = time;
			c.lastModified = lastModified;
			c.languages = langauges;
			return in;
		}

		inline bool operator==(const ProjectData& other) {
			return other.path == this->path && other.name == this->name;
		}

	};

}