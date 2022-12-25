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
	
	std::vector<std::vector<int>> result(height,std::vector<int>(width,1));
	
	std::vector<int> previous_tops;
	const auto closest_top = [&](int idx, int default_value, auto get_height)
	{
		const auto h = get_height(idx);
		
		while(!previous_tops.empty() && get_height(previous_tops.back())<h)
			previous_tops.pop_back();
			
		const auto result = previous_tops.empty()?std::abs(default_value-idx):std::abs(idx-previous_tops.back());
		previous_tops.push_back(idx);
		
		return result;
	};
	
	for(int y=0;y<height;++y)
	{
		const auto get_height = [&](int x){ return grid[y][x]; };
		
		previous_tops.clear();
		for(int x=0;x<width;++x)
			result[y][x] *= closest_top(x,0,get_height);
		
		previous_tops.clear();
		for(int x=width-1;x>=0;--x)
			result[y][x] *= closest_top(x,width-1,get_height);
	}
	
	for(int x=0;x<width;++x)
	{
		const auto get_height = [&](int y){ return grid[y][x]; };
		
		previous_tops.clear();
		for(int y=0;y<height;++y)
			result[y][x] *= closest_top(y,0,get_height);
		
		previous_tops.clear();
		for(int y=height-1;y>=0;--y)
			result[y][x] *= closest_top(y,height-1,get_height);
	}

	int max = 0;
	for(int y=0;y<height;++y)
		for(int x=0;x<width;++x)
			max = std::max(max,result[y][x]);
	
	std::cout<<max;
	
	return 0;
}
