#include <iostream>
#include <fstream>
#include <vector>

#ifdef linux
#include <stdlib.h>
#endif

namespace lox {
	enum FISH_STATE {
		FISH_INIT,
		FISH_COMMAND,
		FISH_TIME,
		FISH_PATH
	};

	FISH_STATE fish_state = FISH_INIT;
	struct fishCommand
	{
		std::string when;
		std::string command;
		std::vector<std::string> paths;	
	};

	inline void transition(FISH_STATE state) {
		fish_state = state;
	}
}


std::string trim(std::string);
std::ostream& operator<<(std::ostream&, const lox::fishCommand&);

int main(int argc, char const *argv[])
{
    // Check flags
    bool DISPLAY_TIME = false;
    bool DISPLAY_NUMBER = false;

    #define arg_strcmp(FLAG, VAR) if(strcmp(argv[argc], FLAG) == 0) \
    	{ VAR |= true; continue; }  
    	
    while(argc--) 
    {
        arg_strcmp("-t", DISPLAY_TIME);
        arg_strcmp("-n", DISPLAY_NUMBER);
    }

	std::string fishHistoryFilePath = "";
	fishHistoryFilePath  += getenv("HOME");
	fishHistoryFilePath  += "/.config/fish/fish_history";

	std::fstream fishHistoryFile;
	fishHistoryFile.open(fishHistoryFilePath.c_str());
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
	#define nextline() running = getline(fishHistoryFile, line); \
                       if (line.length() == 0) break;
	#define current_history() fishList[idx]

	while(running) 
	{
		switch(lox::fish_state) 
		{
			case lox::FISH_INIT:
				nextline();
				if(line.find("- cmd:") == 0) 
				{
					lox::transition(lox::FISH_COMMAND);	
				}
				break;
			case lox::FISH_COMMAND:
				idx++;
				fishList.push_back(lox::fishCommand());
				current_history().command = trim(line.replace(0, 6, ""));				

				nextline();
				if(line.find("when:")) 
				{
					lox::transition(lox::FISH_TIME);
				}
				break;
			case lox::FISH_TIME:		
				current_history().when = trim(line.replace(0, 8, ""));
				nextline();
				if(line.find("paths:")) 
				{
					lox::transition(lox::FISH_PATH);
				}

				if(line.find("- cmd:") == 0) 
				{
					lox::transition(lox::FISH_COMMAND);
				}

				break;
			case lox::FISH_PATH:
				nextline();
				if(line.find("- cmd:") == 0) 
				{
					lox::transition(lox::FISH_COMMAND);
				} else {
					current_history().paths.push_back(trim(line.replace(0, 5, "")));
					lox::transition(lox::FISH_PATH);					
				}
				break;
		}
	}


    char buff[20];
    time_t timestamp;
	for (int i = 0; i < fishList.size(); ++i)
	{                                   
        if(DISPLAY_NUMBER)
        {
        	printf("\x1b[36;1m%-3d\x1b[39;49m ", i );
        }
        
        if(DISPLAY_TIME) 
        {
        	timestamp = atoi(fishList[i].when.c_str());
			strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));	
        	printf("\x1b[32;1m%-20s\x1b[39;49m", buff);
        }
        
      	printf("%s\n", fishList[i].command.c_str() );
	}
	
	return 0;
}

std::string trim(std::string subject) {
	std::string out; 
	int front_idx = 0;
	int back_idx  = subject.length() - 1;
	char c;

	// Trim the front
	while(true) 
	{
		c = subject.at(front_idx);
		if(isspace(c)) {
			front_idx++;
		} else {
			break;
		}
	}	
	// Trim the back
	while(true) 
	{
		c = subject.at(back_idx);
		if(isspace(c)) {
			back_idx--;
		} else {
			break;
		}
	}

	return subject.substr(front_idx, back_idx);
}

std::ostream& operator<<(std::ostream& os, const lox::fishCommand& fish)
{
    os << "(Command): " << fish.command << " (When): " << fish.when << " \n";
    for (int i = 0; i < fish.paths.size(); ++i)
    {
    	os << fish.paths[i] << "\n";
    }
    return os;
}
