#include <advent.hpp>

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

using namespace advent;

struct shape
{
	std::vector<std::string> parts;
};

bool can_move(const shape& s, point2d pos, vec2d dir, const std::vector<std::string>& grid)
{
	if(pos.x+dir.x<0 || pos.x+s.parts[0].size()-1+dir.x>=grid[0].size() || pos.y+s.parts.size()-1+dir.y>=grid.size())
		return false;
		
	for(std::size_t y=0;y<s.parts.size();++y)
	{
		for(std::size_t x=0;x<s.parts[y].size();++x)
		{
			if(grid[grid.size()-1-(pos.y+y+dir.y)][pos.x+x+dir.x]=='#' && s.parts[y][x]=='#')
				return false;
		}
	}
	return true;
}

point2d drop(const shape& s, point2d pos, const std::vector<std::string>& grid, const std::string& input, std::size_t& input_pos)
{	
	for(;;)
	{		
		char movement = input[input_pos];
		input_pos = (input_pos+1)%input.size();
		
		if(movement=='<' && can_move(s,pos,{-1,0},grid))
			--pos.x;
			
		if(movement=='>' && can_move(s,pos,{+1,0},grid))
				++pos.x;
		
		if(can_move(s,pos,{0,+1},grid))
			++pos.y;
		else
			break;
	}
	return pos;
}

int main(int argc, char* argv[])
{
	const std::array<shape,5> shapes
	{{
		{{"####"}},
		{{".#.","###",".#."}},
		{{"..#","..#","###"}},
		{{"#","#","#","#"}},
		{{"##","##"}}
	}};
	
	std::vector<std::string> grid;
	
	std::string input;
	std::cin>>input;
	std::size_t input_pos = 0;
	
	for(int i=0;i<2022;++i)
	{
		auto prev_size = grid.size();
		
		const auto shape_id = i%shapes.size();
		grid.resize(grid.size()+3+shapes[shape_id].parts.size(),".......");

		const point2d pos = drop(shapes[shape_id],{2,0},grid,input,input_pos);
		
		for(std::size_t y=0;y<shapes[shape_id].parts.size();++y)
		{
			for(std::size_t x=0;x<shapes[shape_id].parts[y].size();++x)
			{
				if(shapes[shape_id].parts[y][x]=='#')
					grid[grid.size()-1-(pos.y+y)][pos.x+x] = '#';
			}
		}
		
		if(prev_size<grid.size()-pos.y)
			grid.resize(grid.size()-pos.y);
		else
			grid.resize(prev_size);
	}
	
	std::cout<<grid.size();
	
	return 0;
}
