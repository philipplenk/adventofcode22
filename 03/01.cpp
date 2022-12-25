#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>


int main(int argc, char* argv[])
{
	std::string line;
	
	int sum = 0;
	while(std::getline(std::cin,line))
	{
		std::string lhs = line.substr(0,line.size()/2);
		std::string rhs = line.substr(line.size()/2);
		std::sort(lhs.begin(),lhs.end());
		std::sort(rhs.begin(),rhs.end());
		
		std::string inter;
		std::set_intersection(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),std::back_inserter(inter));
		
		inter.erase(std::unique(inter.begin(),inter.end()),inter.end());
		
		sum+=std::transform_reduce(inter.begin(),inter.end(),0,std::plus<>{},[](auto c)
		{
			return std::islower(c)?(c-'a'+1):(c-'A'+27);
		});
	}
	std::cout<<sum;
	return 0;
}
