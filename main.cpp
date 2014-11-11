#include <iostream>
#include <fstream>
#include <vector>

namespace lox {
	const int FISH_INIT = 0x00;
	const int FISH_COMMAND = 0x10;
	const int FISH_TIME = 0x20;
	int fish_state = FISH_INIT;
	struct fishCommand
	{
		std::string when;
		std::string command;
		std::string paths;	
	};

	inline void transition(int state) {
		fish_state = state;
	}
}

std::ostream& operator<<(std::ostream& os, const lox::fishCommand& fish)
{
    os << fish.command << "/" << fish.when << "/" << fish.paths; 
    return os;
}


int main(int argc, char const *argv[])
{
	std::fstream fishHistoryFile;
	fishHistoryFile.open("/Users/davidnuon/.config/fish/fish_history");
	std::string line;
	std::string buffy;

	std::vector<lox::fishCommand> fishList;
	int idx = -1;

	if(!fishHistoryFile.is_open()) 
	{
		std::cout << "Unable to open file!";
		exit(1);
	}

	bool running = true;
	#define next_line running = getline(fishHistoryFile, line)
	while(running) {
		next_line;
		idx++;
		fishList.push_back(lox::fishCommand());
		fishList[idx].command = line;
	}
	for (int i = 0; i < fishList.size(); ++i)
	{
		std::cout << fishList[i] << std::endl;
	}
	
	return 0;
}