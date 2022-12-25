#include <advent.hpp>

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <cctype>
#include <cstddef>

using namespace advent;

struct edge
{
	point2d side;
	direction dir;
};

direction invert(direction dir)
{
	switch(dir)
	{
		case direction::left: return direction::right;
		case direction::right: return direction::left;
		case direction::up: return direction::down;
		case direction::down: return direction::up;
	}
	
	return {};
}

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

vec2d edge_offset(direction dir, int sidelength)
{
	switch(dir)
	{
		case direction::left: return {0,0};
		case direction::right: return {sidelength-1,0};
		case direction::up: return {0,0};
		case direction::down: return {0,sidelength-1};
	}
	
	return {};
}

int offset_on_edge(point2d pos, direction dir)
{
	if(dir==direction::left || dir==direction::right)
		return pos.y;
	
	return pos.x;
}

vec2d to_edge_offset(direction dir, int offset)
{
	if(dir==direction::left || dir==direction::right)
		return {0,offset};
	
	return {offset,0};
}

int main(int argc, char* argv[])
{
	struct connection
	{
		edge other_edge;
		bool inverted;
	};
	
	std::unordered_map<point2d,std::array<connection,4>> connections;
	const auto link = [&](const edge& lhs, const edge& rhs, bool inverted)
	{
		connections[lhs.side][static_cast<int>(lhs.dir)] = {rhs,inverted};
		connections[rhs.side][static_cast<int>(rhs.dir)] = {lhs,inverted};
	};
	
	link({{1,0},direction::up},{{0,3},direction::left},false);
	link({{1,0},direction::left},{{0,2},direction::left},true);
	link({{2,0},direction::up},{{0,3},direction::down},false);
	link({{2,0},direction::right},{{1,2},direction::right},true);
	link({{2,0},direction::down},{{1,1},direction::right},false);
	
	link({{1,1},direction::left},{{0,2},direction::up},false);
	link({{0,2},direction::left},{{1,0},direction::left},true);
	
	link({{1,2},direction::down},{{0,3},direction::right},false);
	
	
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
		
	const int cube_sidelength = grid.size()/4;
	const int width = grid[0].size();
	const int height = grid.size();
	
	const auto is_invalid = [&](point2d pos)
	{
		if(pos.x<0 || pos.x>=width) return true;
		if(pos.y<0 || pos.y>=height) return true;
		return grid[pos.y][pos.x]==' ';
	};
	
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
	
	const auto wrap = [&](point2d pos, direction& dir)
	{
		auto offset_pos = pos;
		offset_pos.x%=cube_sidelength;
		offset_pos.y%=cube_sidelength;
		
		const auto reverse = [&](int v) { return cube_sidelength-1-v; };
		const auto cube_coordinates = [&](point2d grid_coords) { return point2d{grid_coords.x/cube_sidelength,grid_coords.y/cube_sidelength}; };
		const auto cube_offset = [&](point2d cube_coords) { return point2d{cube_coords.x*cube_sidelength, cube_coords.y*cube_sidelength}; };
		
		const auto from = cube_coordinates(pos);
		const auto target = connections[from][static_cast<int>(dir)];
		
		const auto offset = cube_offset(target.other_edge.side) + edge_offset(target.other_edge.dir,cube_sidelength);
		const auto target_direction = invert(target.other_edge.dir);
		
		auto relevant_coord = offset_on_edge(offset_pos,dir);
		if(target.inverted) relevant_coord = reverse(relevant_coord);
		const auto new_edge_offset = to_edge_offset(target.other_edge.dir,relevant_coord);

		dir = target_direction;
		return offset+new_edge_offset;
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
				auto old_dir = dir;
				
				if(is_invalid(next))
					next = wrap(pos,dir);

				vec = to_vec(dir);

				if(grid[next.y][next.x]=='#')
				{
					dir = old_dir;
					break;
				}
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
