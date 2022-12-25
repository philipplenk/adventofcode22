#include <advent.hpp>

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int width = grid[0].size();
	const int height = grid.size();
	
	struct blizzard
	{
		point2d pos;
		vec2d dir;
	};
	std::vector<blizzard> blizzards;
	
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			switch(grid[y][x])
			{
				case '<':
				{
					blizzards.push_back({{x,y},to_vec(direction::left)});
					break;
				}
				case '>':
				{
					blizzards.push_back({{x,y},to_vec(direction::right)});
					break;
				}
				case '^':
				{
					blizzards.push_back({{x,y},to_vec(direction::up)});
					break;
				}
				case 'v':
				{
					blizzards.push_back({{x,y},to_vec(direction::down)});
					break;
				}
				default: break;
			}
		}
	}
	
	using map_type = std::vector<std::vector<bool>>;
	std::vector<map_type> unreachable;
	const auto move_blizzards = [&]()
	{
		map_type new_unreachable(height,std::vector<bool>(width,false));
		for(auto& b: blizzards)
		{
			b.pos+=b.dir;
			if(b.pos.x<=0) b.pos.x=width-2;
			if(b.pos.y<=0) b.pos.y=height-2;
			if(b.pos.x>width-2) b.pos.x=1;
			if(b.pos.y>height-2) b.pos.y=1;
			new_unreachable[b.pos.y][b.pos.x] = true;
		}
		unreachable.push_back(std::move(new_unreachable));
	};
	
	struct waypoint
	{
		point2d pos;
		int steps = 0;
	};
	
	const point2d start{1,0};
	const point2d end{width-2,height-1};
	
	const auto manhatten_dist = [](const point2d& lhs, const point2d& rhs)
	{
		return std::abs(lhs.x-rhs.x) + std::abs(lhs.y-rhs.y);
	};
	
	const auto compare = [&](const waypoint& lhs, const waypoint& rhs)
	{
		return rhs.steps+manhatten_dist(rhs.pos,end)<lhs.steps+manhatten_dist(lhs.pos,end) ||
		(rhs.steps+manhatten_dist(rhs.pos,end)==lhs.steps+manhatten_dist(lhs.pos,end) && manhatten_dist(rhs.pos,end)<manhatten_dist(lhs.pos,end));
	};
	
	std::unordered_map<point2d,std::unordered_set<int>> seen;
	
	std::priority_queue<waypoint,std::vector<waypoint>,decltype(compare)> to_visit(compare);
	to_visit.push({start,0});
	
	const auto valid = [&](point2d pos,int step)
	{
		if(pos.x<0) return false;
		if(pos.y<0) return false;
		if(pos.x>=width) return false;
		if(pos.y>=height) return false;
		if(grid[pos.y][pos.x]=='#') return false;
		if(unreachable[step][pos.y][pos.x]) return false;
		return true;
	};
	
	while(!to_visit.empty())
	{
		const auto [pos,steps] = to_visit.top();
		to_visit.pop();
		
		if(seen[pos].count(steps)) continue;
		seen[pos].insert(steps);
		
		if(pos==end)
		{
			std::cout<<steps;
			break;
		}
		
		if(steps>=static_cast<int>(unreachable.size()))
			move_blizzards();
		
		
		for(auto dir: {vec2d{1,0},vec2d{-1,0},vec2d{0,1},vec2d{0,-1},vec2d{0,0}})
		{
			if(valid(pos+dir,steps))
				to_visit.push({pos+dir,steps+1});
		}
	}

	return 0;
}
