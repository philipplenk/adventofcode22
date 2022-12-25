#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
	std::unordered_map<std::string,int> sizes;
	
	std::string ignore, cmd;
	std::vector<std::string> current_directory;
	while(std::cin>>ignore>>cmd)
	{
		if(cmd == "cd")
		{	
			std::string target;
			std::cin>>target;
			
			if(target=="/")
			{
				current_directory.clear();
				current_directory.push_back("");
			}
			else if (target=="..")
			{
				if(!current_directory.empty())
					current_directory.pop_back();
					
				if(current_directory.empty())
					current_directory.push_back("");
			}
			else
				current_directory.push_back(target);
		}
		else if(cmd== "ls")
		{
			std::cin>>std::ws;
	
			while(std::cin && std::cin.peek()!='$')
			{
				int size = 0;
				if(std::isdigit(std::cin.peek()))
					std::cin>>size;
				
				std::getline(std::cin,ignore);
				
				std::string path = "";
				for(const auto& part: current_directory)
				{
					path+=part+"/";
					sizes[path]+=size;
				}
			}
		}
	}
	
	int total = 0;
	for(const auto& [name, size]: sizes)
		if(size<=100000)
			total+=size;
	
	std::cout<<total;
	
	return 0;
}
