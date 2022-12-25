#include <advent.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
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
	
	int max_y = 0;
	for(const auto& line: lines)
		for(const auto& p: line.points)
			max_y = std::max(p.y,max_y);
		
	std::unordered_set<point2d> grid;
	for(const auto& line: lines)
	{
		for(std::size_t i=0;i+1<line.points.size();++i)
		{
			point2d p = line.points[i];
			
			const point2d t = line.points[i+1];
			vec2d dir{0,0};
			dir.x = p.x<t.x?1:p.x>t.x?-1:0;
			dir.y = p.y<t.y?1:p.y>t.y?-1:0;
			for(;p!=t;p+=dir)
				grid.insert(p);
			
			grid.insert(t);
		}
	}
	
	const auto drop = [&](point2d p)
	{
		while(p.y+1<max_y+2)
		{
			if(!grid.count({p.x,p.y+1}))
				p+=vec2d{0,1};
			else if(!grid.count({p.x-1,p.y+1}))
				p+=vec2d{-1,1};
			else if(!grid.count({p.x+1,p.y+1}))
				p+=vec2d{1,1};
			else
				break;
		}
		return p;
	};
	
	const point2d spawn_point{500,0};
	int count = 0;
	for(;;++count)
	{
		point2d new_sand = drop(spawn_point);
		grid.insert({new_sand.x,new_sand.y});
		if(new_sand==spawn_point) break;
	}
	
	std::cout<<count+1;
}
