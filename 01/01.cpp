#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::string line;
	int max = 0, sum = 0;
	while(std::getline(std::cin,line))
	{
		if(line.empty())
			sum = 0;
		else
			sum+=std::stoi(line);
		
		max = std::max(max,sum);
	}
	std::cout<<max;
}
