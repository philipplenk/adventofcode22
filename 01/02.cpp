#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::string line;
	int max = 0, sum = 0;
	std::vector<int> sums;
	while(std::getline(std::cin,line))
	{
		if(line.empty())
			sums.push_back(sum),sum = 0;
		else
			sum+=std::stoi(line);
		
		max = std::max(max,sum);
	}
	sums.push_back(sum);
	std::nth_element(sums.begin(),sums.begin()+3,sums.end(),std::greater<>{});
	
	std::cout<<std::accumulate(sums.begin(),sums.begin()+3,0);
}
