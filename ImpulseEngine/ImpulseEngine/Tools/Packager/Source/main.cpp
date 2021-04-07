#include <GEngine.h>


static uint8_t isCommand(const char* c);
static void pakDir(int* p, char*, int argc, char** argv);
static std::string help();

using cmd_func = std::unordered_map<std::string, std::function<void(int*, char*, int, char**)>>;
using cmd_help = std::unordered_map<std::string, std::function<std::string()>>;


static cmd_func commands =
{
	{"help", [](int* p, char*, int argc,char** argv) {
		std::cout << help() << std::endl;
	}},
	{"pak", pakDir}
};

static cmd_help commands_help = {
	{"help", help},
	{"pak", []() {
		return "-pak: {directory to pack} {output file}.pak";
	}}
};

int main(int argc, char** argv) {
	
	size_t argS = 0;
	if (argc <= 2) {
		std::cout << commands_help["help"]() << std::endl;
		return 0;
	}

	for (int i = 1; i < argc; i++) {

		char* arg = argv[i];
		argS = strlen(arg);

		// Command
		if (int dCount = isCommand(arg)) {
			if (i == argc - 1) { 
				cmd_help::iterator cmd = commands_help.find(&arg[dCount]);
				if (cmd != commands_help.end()) {
					cmd->second();
				}
				break;
			}
			cmd_func::iterator cmd = commands.find(&arg[dCount]);
			if (cmd != commands.end()) {
				std::cout << "RUNNING COMMAND: " << cmd->first << std::endl;
				cmd->second(&i, &arg[dCount], argc, argv);
			}
		}
	}
	return 0;

}

void pakDir(int* p, char* cmd, int argc, char** argv) {
	int c = *p;
	*p = *p + 1;
	char* dir[2] = { 0,0 };
	int counter = 0;
	for (; *p < argc;) {
		if (!isCommand(argv[*p]) && counter < 2) {
			dir[counter++] = argv[*p];
			*p = *p + 1;
			continue;
		} 
		break;
	}
	if (counter == 2) {
		std::cout << dir[0] << " : " << dir[1] << std::endl;
		GEngine::FileSystem::PakDirectory(dir[0], dir[1], false);
	}
	else {
		std::cout << commands_help[cmd]();
		return;
	}
}

uint8_t isCommand(const char* c) {
	if (strlen(c) > 2) {
		if (c[0] == '-' && c[1] == '-') {
			return 2;
		}
	} 
	if (strlen(c) > 1) {
		if (c[0] == '-') {
			return 1;
		}
	}
	return 0;
}

std::string help() {
	std::string str = "Help:\nList of commands:";
	for (auto& s : commands) {
		str += " " + s.first;
	}
	return str;
}
