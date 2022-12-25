#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>

int main(int argc, char* argv[])
{
	std::array<std::string,3> lines;
	
	int sum = 0;
	while(std::cin)
	{
		for(auto& line: lines)
			std::getline(std::cin,line);
		
		if(!std::cin) break;
		
		for(auto& line: lines)
			std::sort(line.begin(),line.end());
		
		std::string inter = lines.front();
		for(const auto& line: lines)
		{
			std::string result;
			std::set_intersection(inter.begin(),inter.end(),line.begin(),line.end(),std::back_inserter(result));
			inter = result;
		}
		
		inter.erase(std::unique(inter.begin(),inter.end()),inter.end());
		
		sum+=std::transform_reduce(inter.begin(),inter.end(),0,std::plus<>{},[](auto c)
		{
			return std::islower(c)?(c-'a'+1):(c-'A'+27);
		});
	}
	std::cout<<sum;
	return 0;
}
