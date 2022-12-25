#include <iostream>
#include <tuple>
#include <unordered_set>

struct point3d
{
	int x, y, z;
};

bool operator==(const point3d& lhs, const point3d& rhs)
{
	return std::tie(lhs.x,lhs.y,lhs.z)==std::tie(rhs.x,rhs.y,rhs.z);
}

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

int main(int argc, char* argv[])
{
	std::unordered_set<point3d> cubes;
	point3d p;
	while(std::cin>>p)
		cubes.insert(p);
	
	int count = 0;
	for(const auto& p: cubes)
	{
		for(int dx: {-1,1})
		{
			point3d n = p;
			n.x+=dx;
			count+=1-cubes.count(n);
		}
		for(int dy: {-1,1})
		{
			point3d n = p;
			n.y+=dy;
			count+=1-cubes.count(n);
		}
		for(int dz: {-1,1})
		{
			point3d n = p;
			n.z+=dz;
			count+=1-cubes.count(n);
		}
	}
	
	std::cout<<count;
	
	return 0;
}
