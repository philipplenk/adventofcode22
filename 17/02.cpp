#include <advent.hpp>

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstddef>

using namespace advent;

struct shape
{
	std::vector<std::string> parts;
};

static const std::array<shape,5> shapes
{{
	{{"####"}},
	{{".#.","###",".#."}},
	{{"..#","..#","###"}},
	{{"#","#","#","#"}},
	{{"##","##"}}
}};

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

void drop_and_fix(const std::string& input, std::size_t& input_pos, std::vector<std::string>& grid, int shape_id)
{	
	auto prev_size = grid.size();
	
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

int compute_remainder(const std::string& input, std::size_t input_pos, int offset_id, std::vector<std::string> grid, int steps)
{
	for(int i=0;i<steps;++i)
	{
		const auto shape_id = (i+offset_id)%shapes.size();
		drop_and_fix(input,input_pos,grid,shape_id);
	}
	
	return grid.size();
}

int main(int argc, char* argv[])
{	
	std::vector<std::string> grid;
	
	std::string input;
	std::cin>>input;
	std::size_t input_pos = 0;
	
	int rep_start, rep_end, rep_size;
	std::array<std::unordered_map<std::string,int>,5> seen;
	std::array<std::unordered_map<std::string,int>,5> heights;
	
	const auto max_drops = 1000000000000;
	for(int i=0;i<max_drops;++i)
	{
		const auto shape_id = i%shapes.size();
		drop_and_fix(input,input_pos,grid,shape_id);
		if(grid.size()>4)
		{
			std::string combined = grid.back();
			for(int j=1;j<4;++j)
			{
				for(int i=0;i<7;++i)
					combined[i] = combined[i]=='#'?'#':grid[grid.size()-1-j][i];
			}
				
			if(combined=="#######")
			{
				const std::string foursome = std::accumulate(grid.rbegin(),grid.rbegin()+4,std::string{});
				const auto key = std::to_string(input_pos)+foursome;
				if(seen[shape_id].count(key))
				{
					std::cerr<<"Repetition after "<<i<<" steps\n";
					rep_start = seen[shape_id][key];
					rep_end = i;
					rep_size = grid.size() - heights[shape_id][key];
					break;
				}
				else
				{
					seen[shape_id][key] = i;
					heights[shape_id][key] = grid.size();
				}
			}
		}
	}

	std::cerr<<"Rep started at "<<rep_start<<" and had a size of "<<rep_size<<'\n';
	
	const auto init_size = grid.size();
	std::cerr<<"Starting size was "<<init_size<<'\n';
	const auto remaining_drops = max_drops - rep_end - 1;
	std::cerr<<"Still have to deal with "<<remaining_drops<<'\n';
	const auto repetitions = remaining_drops/(rep_end-rep_start);
	std::cerr<<"That is "<<repetitions<<" repetitions\n";
	const auto remainder = remaining_drops%(rep_end-rep_start);
	std::cerr<<"And requires a fixup of a remainder of "<<remainder<<'\n';
	
	
	std::vector<std::string> remainder_grid;
	for(int i=0;i<4;++i)
		remainder_grid.push_back(grid[grid.size()-4+i]);
	
	const auto remaining_height = compute_remainder(input,input_pos,rep_end+1,remainder_grid,remainder)-4;
	std::cerr<<"Remainder turns out to be a height of "<<remaining_height<<'\n';
	
	unsigned long long total = remaining_height;
	total+=init_size;
	total+=repetitions*static_cast<unsigned long long>(rep_size);
	std::cout<<total;

	return 0;
}
