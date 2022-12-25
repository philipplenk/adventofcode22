#include <advent.hpp>

#include <iostream>
#include <string>
#include <vector>

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

struct interval
{
	long long begin, end;
};

struct area
{
	std::vector<interval> intervals;
};

void remove(area& a, interval v)
{
	std::vector<interval> new_intervals;
	for(auto i: a.intervals)
	{
		if(i.end<v.begin)
			new_intervals.push_back(i);
		else if(i.begin>v.end)
			new_intervals.push_back(i);
		else
		{
			interval before{i.begin,v.begin-1};
			interval after{v.end+1,i.end};
			
			if(before.begin<=before.end)
				new_intervals.push_back(before);
			if(after.begin<=after.end)
				new_intervals.push_back(after);
		}
			
	}
	a.intervals = std::move(new_intervals);
}

int main(int argc, char* argv[])
{
	const auto max = 4000000;
	
	std::vector<area> lines;
	for(int i=0;i<=max;++i)
	{
		lines.push_back({{interval{0,max}}});
	}
	
	sensor_data data;
	while(std::cin>>data)
	{
		for(int i=0;i<=max;++i)
		{
			const auto max_dist = std::abs(data.pos.y-data.beacon_pos.y) + std::abs(data.pos.x-data.beacon_pos.x);
			const auto dist_y = std::abs(data.pos.y-i);
			const auto remaining_dist = max_dist-dist_y;
			
			if(remaining_dist>=0)
				remove(lines[i],{data.pos.x-remaining_dist,data.pos.x+remaining_dist});
		}
	}
	long long y = 0, result = 0;
	for(const auto& line: lines)
	{
		if(!line.intervals.empty())
		{
			result = (line.intervals.front().begin*4000000)+y;
			std::cerr<<line.intervals.front().begin<<' '<<y<<'\n';
		}
		++y;
	}
	std::cout<<result;
	return 0;
}
