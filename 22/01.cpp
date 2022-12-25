#include <advent.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace advent;

direction rotate_left(direction dir)
{
	switch(dir)
	{
		case direction::left: return direction::down;
		case direction::right: return direction::up;
		case direction::up: return direction::left;
		case direction::down: return direction::right;
	}
	return direction::up;
}
	
direction rotate_right(direction dir)
{
	switch(dir)
	{
		case direction::left: return direction::up;
		case direction::right: return direction::down;
		case direction::up: return direction::right;
		case direction::down: return direction::left;
	}
	return direction::up;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> grid;
	std::string line;
	std::size_t max = 0;
	while(std::getline(std::cin,line))
	{
		if(line.empty()) break;
		grid.push_back(line);
		max = std::max(line.size(),max);
	}
	
	for(auto& row: grid)
		row.resize(max,' ');
	
	std::string instructions;
	std::cin>>std::ws>>instructions;
	
	point2d pos{0,0};
	direction dir = direction::right;
	
	const auto to_value = [](direction dir)
	{
		switch(dir)
		{
			case direction::left: return 2; break;
			case direction::right: return 0; break;
			case direction::up: return 3; break;
			case direction::down: return 1; break;
		}
		return 0;
	};
	
	while(grid[pos.y][pos.x]==' ') ++pos.x;
	
	for(std::size_t input_pos = 0;input_pos<instructions.size();)
	{
		if(std::isdigit(instructions[input_pos]))
		{
			int steps = 0;
			while(std::isdigit(instructions[input_pos]))
			{
				steps = steps*10+instructions[input_pos]-'0';
				++input_pos;
			}
			
			auto vec = to_vec(dir);
			for(int i=0;i<steps;++i)
			{
				auto next = pos+vec;
				if(next.x<0) next.x+=grid[pos.y].size();
				if(next.y<0) next.y+=grid.size();
				next.x%=grid[pos.y].size();
				next.y%=grid.size();
			
				while(grid[next.y][next.x]==' ')
				{
					next+=vec;
					if(next.x<0) next.x+=grid[pos.y].size();
					if(next.y<0) next.y+=grid.size();
					next.x%=grid[pos.y].size();
					next.y%=grid.size();
				}
			
				if(grid[next.y][next.x]=='#') break;
				pos = next;
			}
		}
		else
		{
			switch(instructions[input_pos++])
			{
				case 'L': dir = rotate_left(dir); break;
				case 'R': dir = rotate_right(dir); break;
			}
		}
	}
	
	std::cout<<1000*(pos.y+1)+(4*(pos.x+1))+to_value(dir);
	return 0;
}
