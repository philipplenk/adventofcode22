#include <advent.hpp>

#include <iostream>
#include <limits>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace advent;

namespace std
{
	
	template <>
	struct hash<::point3d>
	{
		std::size_t operator()(const ::point3d& p) const noexcept
		{
			return p.x^p.y^p.z;
		}
	};
}

std::istream& operator>>(std::istream& in, point3d& p)
{
	char skip;
	return in>>p.x>>skip>>p.y>>skip>>p.z;
}

struct minmax
{
	int min = std::numeric_limits<int>::max(), max = std::numeric_limits<int>::min();
	
	void update(int v)
	{
		min = std::min(v,min);
		max = std::max(v,max);
	}
};

int main(int argc, char* argv[])
{
	std::unordered_set<point3d> cubes;
	std::unordered_map<point2d,minmax> xy, yz, xz; 
	
	point3d p;
	while(std::cin>>p)
	{
		cubes.insert(p);
		xy[{p.x,p.y}].update(p.z);
		yz[{p.y,p.z}].update(p.x);
		xz[{p.x,p.z}].update(p.y);
	}
	
	const auto exists_smaller = [&](point2d p, auto& s, int v)
	{
		return s[p].min<v;
	};
	
	const auto exists_larger = [&](point2d p, auto& s, int v)
	{
		return s[p].max>v;
	};
	
	const auto trapped = [&](point3d p)
	{
		if(cubes.count(p)) return true;
		
		if(!exists_smaller({p.x,p.y},xy,p.z)) return false;
		if(!exists_larger({p.x,p.y},xy,p.z)) return false;
		if(!exists_smaller({p.x,p.z},xz,p.y)) return false;
		if(!exists_larger({p.x,p.z},xz,p.y)) return false;
		if(!exists_smaller({p.y,p.z},yz,p.x)) return false;
		if(!exists_larger({p.y,p.z},yz,p.x)) return false;
		
		return true;
	};
	
	
	int count = 0;
	for(const auto& p: cubes)
	{
		count+=1-trapped({p.x-1,p.y,p.z});
		count+=1-trapped({p.x+1,p.y,p.z});
		count+=1-trapped({p.x,p.y-1,p.z});
		count+=1-trapped({p.x,p.y+1,p.z});
		count+=1-trapped({p.x,p.y,p.z-1});
		count+=1-trapped({p.x,p.y,p.z+1});
	}
	
	std::cout<<count;	

	return 0;
}
