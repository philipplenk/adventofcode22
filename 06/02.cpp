#include <array>
#include <iostream>

int main(int argc, char* argv[])
{
	std::array<int,256> seen{};
	std::array<char,14> window{};
	
	unsigned pos = 0;
	for(int idx = 0, common = 0; pos<window.size() || common>0; ++pos, idx=(idx+1)%window.size())
	{
		common-=--seen[window[idx]]>=1;
		std::cin>>window[idx];
		common+=++seen[window[idx]]>1;
	}
	
	std::cout<<pos;
	
	return 0;
}
