#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include <cstddef>
#include <cstdint>

using namespace advent;

struct sensor_data
{
	point2d pos, beacon_pos;
};

std::istream& operator>>(std::istream& in, sensor_data& data)
{
	std::string skip;
	char skip_c;
	in>>skip>>skip>>skip_c>>skip_c>>data.pos.x>>skip_c>>skip_c>>skip_c>>data.pos.y>>skip_c;
	in>>skip>>skip>>skip>>skip>>skip_c>>skip_c>>data.beacon_pos.x>>skip_c>>skip_c>>skip_c>>data.beacon_pos.y;
	return in;
}

int main(int argc, char* argv[])
{
	const auto y = 2000000;
	
	std::unordered_set<point2d> seen, beacons;
	
	sensor_data data;
	while(std::cin>>data)
	{
		const auto max_dist = std::abs(data.pos.y-data.beacon_pos.y) + std::abs(data.pos.x-data.beacon_pos.x);
		const auto dist_y = std::abs(data.pos.y-y);
		const auto remaining_dist = max_dist-dist_y;
		if(remaining_dist>=0)
		{
			for(int i=0;i<=remaining_dist;++i)
			{
				seen.insert({data.pos.x-i,y});
				seen.insert({data.pos.x+i,y});
			}
		}
		if(data.beacon_pos.y==y)
			beacons.insert(data.beacon_pos);
	}
	std::cout<<seen.size()-beacons.size();;
	return 0;
}
