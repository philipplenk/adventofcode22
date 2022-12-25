#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});

	const int width = grid[0].size();
	const int height = grid.size();
		
	std::vector<std::vector<bool>> seen(height,std::vector<bool>(width,false));
	
	int visible = 0, max_height = -1;
	
	const auto check_height = [&](int x, int y)
	{
		const auto h = grid[y][x]-'0';
		if(h>max_height)
		{
			max_height = h;
			if(!seen[y][x]) ++visible;
			seen[y][x] = true;  
		}
	};
	
	for(int y=0;y<height;++y)
	{
		max_height = -1;
		for(int x=0;x<width;++x)
			check_height(x,y);
		
		max_height = -1;
		for(int x=width-1;x>=0;--x)
			check_height(x,y);
	}
	
	for(int x=0;x<width;++x)
	{
		max_height = -1;
		
		for(int y=0;y<height;++y)
			check_height(x,y);
			
		max_height = -1;
		for(int y=height-1;y>=0;--y)
			check_height(x,y);
	}

	std::cout<<visible;
	
	return 0;
}
