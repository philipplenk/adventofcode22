#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct move
{
	int n, from, to;
};

std::istream& operator>>(std::istream& in, move& m)
{
	std::string skip;
	in>>skip>>m.n>>skip>>m.from>>skip>>m.to;
	--m.from;
	--m.to;
	return in;
}

int main(int argc, char* argv[])
{
	std::vector<std::vector<char>> stacks;
	
	for(std::string line;std::getline(std::cin,line);)
	{
		if(line.front()==' ') break;
		for(unsigned i=0, stack_id=0;i<line.size();i+=4,++stack_id)
		{
			if(stacks.size()<=stack_id)
				stacks.push_back({});
			
			if(line[i+1]!=' ')
				stacks[stack_id].push_back(line[i+1]);
		}
	}
	
	for(auto& s: stacks)
		std::reverse(s.begin(),s.end());
	
	for(move m;std::cin>>m;)
	{
		while(m.n--)
		{
			stacks[m.to].push_back(stacks[m.from].back());
			stacks[m.from].pop_back();
		}
	}
	
	for(const auto& s: stacks)
		if(!s.empty()) std::cout<<s.back();
	
	return 0;
}
