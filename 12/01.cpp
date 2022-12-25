#include <advent.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <queue>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
	const int width = grid[0].size();
	const int height = grid.size();
	
	struct waypoint
	{
		point2d pos;
		int steps;
	};
	
	std::queue<waypoint> to_visit;
	
	for(int y=0;y<static_cast<int>(grid.size());++y)
	{
		for(int x=0;x<static_cast<int>(grid[y].size());++x)
		{
			if(grid[y][x]=='S')
			{
				to_visit.push({{x,y},0});
				break;
			}
		}
	}
	
	std::vector<std::vector<bool>> visited(height,std::vector<bool>(width,false));
	
	while(!to_visit.empty())
	{
		const auto [next,steps] = to_visit.front();
		to_visit.pop();

		if(grid[next.y][next.x]=='E')
		{
			std::cout<<steps;
			break;
		}
		
		if(visited[next.y][next.x]) continue;
		visited[next.y][next.x] = true;
		
		const int current_height = grid[next.y][next.x]=='S'?'a':grid[next.y][next.x];
		const auto ok = [&](int x, int y)
		{
			const int target_height = grid[y][x]=='E'?'z':grid[y][x];
			return target_height-current_height<2;
		};
		
		if(next.x>0 && ok(next.x-1,next.y)) to_visit.push({{next.x-1,next.y},steps+1});
		if(next.x+1<width && ok(next.x+1,next.y)) to_visit.push({{next.x+1,next.y},steps+1});
		if(next.y>0 && ok(next.x,next.y-1)) to_visit.push({{next.x,next.y-1},steps+1});
		if(next.y+1<height && ok(next.x,next.y+1)) to_visit.push({{next.x,next.y+1},steps+1});
	}
	
	return 0;
}
