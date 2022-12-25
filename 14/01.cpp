#include <advent.hpp>

#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

#include <cstddef>

using namespace advent;

struct path
{
	std::vector<point2d> points;
};

std::istream& operator>>(std::istream& in, path& p)
{
	p.points.clear();
	std::string line;
	std::getline(in,line);
	std::stringstream strm{line};
	
	while(strm)
	{
		point2d next;
		char skip;
		strm>>next.x>>skip>>next.y>>skip>>skip;
		p.points.push_back(next);
	}
	return in;
}

int main(int argc, char* argv[])
{
	std::vector<path> lines(std::istream_iterator<path>{std::cin},{});
	
	int min_x = std::numeric_limits<int>::max(), min_y = 0, max_x = 0, max_y = 0;
	for(const auto& line: lines)
	{
		for(const auto& p: line.points)
		{
			min_x = std::min(p.x,min_x);
			min_y = std::min(p.y,min_y);
			max_x = std::max(p.x,max_x);
			max_y = std::max(p.y,max_y);
		}
	}
	
	const unsigned width = max_x-min_x+2;
	const unsigned height = max_y-min_y+2;
	const auto offset_x = min_x-1;
	const auto offset_y = 0;
	
	std::vector<bool> grid(height*width,false);
	
	for(const auto& line: lines)
	{
		for(std::size_t i=0;i+1<line.points.size();++i)
		{
			point2d p = line.points[i];
			point2d t = line.points[i+1];
			vec2d dir{0,0};
			dir.x = p.x<t.x?1:p.x>t.x?-1:0;
			dir.y = p.y<t.y?1:p.y>t.y?-1:0;
			while(p!=t)
			{
				grid[(p.y-offset_y)*width+(p.x-offset_x)] = true;
				p+=dir;
			}
			grid[(p.y-offset_y)*width+(p.x-offset_x)] = true;
		}
	}
		
	point2d spawn_point{500,min_y};
	
	int count = 0;
	bool rest = true;
	for(;rest;++count)
	{
		rest = false;
		point2d new_sand = spawn_point;
		while(new_sand.y+1<max_y+1)
		{
			if(!grid[(new_sand.y+1-offset_y)*width+(new_sand.x-offset_x)])
				++new_sand.y;
			else if(!grid[(new_sand.y+1-offset_y)*width+(new_sand.x-1-offset_x)])
				++new_sand.y,--new_sand.x;
			else if(!grid[(new_sand.y+1-offset_y)*width+(new_sand.x+1-offset_x)])
				++new_sand.y,++new_sand.x;
			else
			{
				grid[(new_sand.y-offset_y)*width+(new_sand.x-offset_x)] = true;
				rest = true;
				break;
			}
		}
	}

	std::cout<<count-1;
}
