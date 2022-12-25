#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <cstdint>

struct valve
{
	std::string name;
	int flow_rate;
	std::vector<std::string> connections;
};

std::istream& operator>>(std::istream& in, valve& v)
{
	v.connections.clear();
	std::string skip;
	char skip_c;
	
	in>>skip>>v.name>>skip>>skip;
	for(int i=0;i<5;++i)
		in>>skip_c;
	
	in>>v.flow_rate>>skip_c;
	for(int i=0;i<4;++i)
		in>>skip;
	
	in>>std::ws;
	std::string line, target;
	std::getline(in,line);
	std::stringstream strm{line};
	while(std::getline(strm,target,','))
	{
		v.connections.push_back(target);
		strm>>std::ws;
	}
	
	
	return in;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string,valve> valves;
	std::unordered_map<std::string,std::unordered_map<std::string,int>> connections;
	
	std::vector<std::string> names;
	std::unordered_map<std::string,std::uint64_t> bitmap;
	
	valve c;
	while(std::cin>>c)
	{
		bitmap[c.name] = std::uint64_t{1}<<names.size();
		names.push_back(c.name);
		
		valves[c.name] = c;
		for(const auto& next: c.connections)
			connections[c.name][next] = 1;
		connections[c.name][c.name] = 0;
	}
	
	for(std::size_t i=0;i<valves.size();++i)
	{
		for(const auto& [name,v]: valves)
		{
			auto& own_connections = connections[name];
			
			for(const auto& other_name: names)
			{
				if(other_name!=name && own_connections.count(other_name)>0)
				{
					int dist = own_connections[other_name];
					auto& other_connections = connections[other_name];
					for(const auto& next_name: names)
					{
						if(other_connections.count(next_name)>0)
						{
							if(own_connections.count(next_name))
								own_connections[next_name] = std::min(dist+other_connections[next_name],own_connections[next_name]);
							else
								own_connections[next_name] = dist + other_connections[next_name];
						}
					}
				}
			}
		}
	}
	
	for(const auto& [_,v]: valves)
	{
		if(v.flow_rate==0)
		{
			for(const auto& n: names)
				connections[n].erase(v.name);
		}
		connections[v.name].erase(v.name);
	}
	
	std::unordered_map<std::uint64_t,int> best_values;
	const auto collect_options = [&](std::string current, int day, std::uint64_t seen, int total_so_far, auto rec)
	{
		if(day<=0)
		{
			seen = seen  & ~bitmap["AA"];
			best_values[seen] = std::max(best_values[seen],total_so_far);
			return;
		}
		
		seen|=bitmap[current];
		
		const auto& current_v = valves[current];
		int init = current_v.flow_rate*day;
		
		for(const auto& [next, dist]: connections[current])
		{
			if(seen&bitmap[next]) continue;
			rec(next,day-dist-1,seen,total_so_far+init,rec);
		}
	};
	
	collect_options("AA",26,0,0,collect_options);
	
	std::vector<std::uint64_t> options;
	std::transform(best_values.begin(),best_values.end(),std::back_inserter(options),[&](const auto& p)
	{
		return p.first;
	});
	
	std::sort(options.begin(),options.end(),[&](const auto& lhs, const auto& rhs)
	{
		return best_values[lhs]>best_values[rhs];
	});
	
	int max = 0;
	for(std::size_t i=0;i<options.size();++i)
	{
		for(std::size_t j=i+1;j<options.size();++j)
		{
			const auto sum = best_values[options[i]]+best_values[options[j]];
			if(sum<max)
				break;
			if(sum>max && (options[i]&options[j])==0)
				max = sum;
		}
	}
	std::cout<<max;
	return 0;
}
