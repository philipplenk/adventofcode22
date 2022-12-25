#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{
	int count = 0;
	std::string line;
	while(std::getline(std::cin,line))
	{
		std::stringstream strm{line};
		std::string s0, e0, s1, e1;
		std::getline(strm,s0,'-');
		std::getline(strm,e0,',');
		std::getline(strm,s1,'-');
		std::getline(strm,e1);
		
		int start0 = std::stoi(s0), end0 = std::stoi(e0), start1 = std::stoi(s1), end1 = std::stoi(e1);
		
		if(start0>start1 || (start0==start1 && end0<end1))
		{
			std::swap(start0,start1);
			std::swap(end0,end1);
		}
		
		if(start1<=end0)
			++count;
	}
	std::cout<<count;
	
	return 0;
}
