#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	int width = grid[0].size();
	int height = grid.size();
	
	const auto is_in_grid = [&](point2d pos)
	{
		if(pos.x<0 || pos.y<0 || pos.x>=width || pos.y>=height)
			return false;
		return true;
	};
	
	std::array<std::array<vec2d,3>,4> to_check;
	to_check[static_cast<int>(direction::up)] = {vec2d{-1,-1},vec2d{0,-1},vec2d{1,-1}};
	to_check[static_cast<int>(direction::down)] = {vec2d{-1,1},vec2d{0,1},vec2d{1,1}};
	to_check[static_cast<int>(direction::left)] = {vec2d{-1,-1},vec2d{-1,0},vec2d{-1,1}};
	to_check[static_cast<int>(direction::right)] = {vec2d{1,-1},vec2d{1,0},vec2d{1,1}};
	
	const auto count_neighbors = [&](const auto& grid, point2d pos)
	{
		int count = 0;
		for(int dx: {-1,0,1})
		{
			for(int dy: {-1,0,1})
			{
				const point2d npos{pos.x+dx,pos.y+dy};
				if(npos==pos) continue;
				if(!is_in_grid(npos)) continue;
				count+=grid[npos.y][npos.x]=='#';
			}
		}
		return count;
	};
	
	std::array<direction,4> proposal_order{{direction::up,direction::down,direction::left,direction::right}};
	
	const auto collect_proposals = [&](const auto& grid)
	{		
		std::unordered_map<point2d,int> proposal_counts;
		std::vector<std::vector<std::optional<direction>>> proposals(height,std::vector<std::optional<direction>>(width));
		
		for(int y=0;y<height;++y)
		{
			for(int x=0;x<width;++x)
			{
				const point2d pos{x,y};
				if(grid[y][x]!='#' || count_neighbors(grid,pos)==0)
					continue;
				
				for(auto d: proposal_order)
				{
					bool ok = true;
					point2d dest;
					for(auto v: to_check[static_cast<int>(d)])
					{
						dest = pos+v;
						ok&=!is_in_grid(dest) || grid[dest.y][dest.x]!='#';
					}
					if(ok)
					{
						proposals[pos.y][pos.x] = d;
						++proposal_counts[pos+to_vec(d)];
						break;
					}
				}
			}
		}
		return std::make_pair(proposals,proposal_counts);
	};
	
	const auto enlarged_copy = [&](const auto& grid)
	{
		auto cpy = grid;
		for(auto& row: cpy)
			row = '.' + row + '.';
		
		cpy.insert(cpy.begin(),std::string(width+2,'.'));
		cpy.push_back(std::string(width+2,'.'));
		
		return cpy;
	};
	
	const auto step = [&]()
	{
		const auto [proposals, counts] = collect_proposals(grid);
		
		auto next = enlarged_copy(grid);
		vec2d enlarged_offset{1,1};
	
		for(int y = 0;y<height;++y)
		{
			for(int x=0;x<width;++x)
			{
				const auto apos = point2d{x,y}+enlarged_offset;
				if(proposals[y][x])
				{
					const auto dest = point2d{x,y} + to_vec(*proposals[y][x]);
					const auto adest = dest + enlarged_offset;
					if(counts.find(dest)->second == 1)
					{ 
						next[adest.y][adest.x] = '#';
						next[apos.y][apos.x] = '.';
					}
				}
			}
		}
			
		grid = std::move(next);
		width+=2;
		height+=2;
		std::rotate(proposal_order.begin(),proposal_order.begin()+1,proposal_order.end());
	};
	
	for(int i=0;i<10;++i)
		step();
	
	int min_x=width, max_x=0, min_y= height, max_y = 0;
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			if(grid[y][x]=='#')
			{
				min_x = std::min(x,min_x);
				min_y = std::min(y,min_y);
				max_x = std::max(x,max_x);
				max_y = std::max(y,max_y);
			}
		}
	}
	
	int count = 0;
	for(int y=min_y;y<=max_y;++y)
	{
		for(int x=min_x;x<=max_x;++x)
		{
			count+=grid[y][x]=='.';
		}
	}
	std::cout<<count;
	
	return 0;
}
