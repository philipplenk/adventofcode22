#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

struct material_type
{
	static constexpr int ore = 0, clay=1, obsidian=2, geode=3;
	static constexpr int size = 4;
};

struct production_costs
{
	std::array<int,material_type::size> costs{};
};

struct blueprint
{
	std::array<production_costs,material_type::size> robot_costs{};
	std::array<int,material_type::size> max_worth{};
};

std::istream& operator>>(std::istream& in, blueprint& b)
{
	std::string skip;
	in>>skip>>skip;
	in>>skip>>skip>>skip>>skip>>b.robot_costs[material_type::ore].costs[material_type::ore]>>skip;
	in>>skip>>skip>>skip>>skip>>b.robot_costs[material_type::clay].costs[material_type::ore]>>skip;
	in>>skip>>skip>>skip>>skip>>b.robot_costs[material_type::obsidian].costs[material_type::ore]>>skip>>skip>>b.robot_costs[material_type::obsidian].costs[material_type::clay]>>skip;
	in>>skip>>skip>>skip>>skip>>b.robot_costs[material_type::geode].costs[material_type::ore]>>skip>>skip>>b.robot_costs[material_type::geode].costs[material_type::obsidian]>>skip;
	
	b.max_worth ={};
	for(const auto robot: b.robot_costs)
	{
		for(int i=0;i<material_type::size;++i)
			b.max_worth[i] = std::max(b.max_worth[i],robot.costs[i]);
	}
			
	return in;
}

struct state
{
	std::array<std::uint8_t,material_type::size> robots{}, material{};
};

bool operator<(const state& lhs, const state& rhs)
{
	return std::tie(lhs.robots,lhs.material)<std::tie(rhs.robots,rhs.material);
}

bool operator==(const state& lhs, const state& rhs)
{
	return std::tie(lhs.robots,lhs.material)==std::tie(rhs.robots,rhs.material);
}

std::vector<state> select_best(std::vector<state> next)
{	
	// If a state is smaller in each component(so less of every material type and less of every robot), it is strictly worse and can be skipped
	
	std::vector<state> result;
	
	std::sort(next.begin(),next.end(),[](const auto& lhs, const auto& rhs)
	{
		bool all_smaller = true, all_larger = true;;
		for(int i=0;i<material_type::size;++i)
		{
			all_smaller&=lhs.robots[i]<rhs.robots[i];
			all_smaller&=lhs.material[i]<rhs.material[i];
			
			all_larger&=rhs.robots[i]<lhs.robots[i];
			all_larger&=rhs.material[i]<lhs.material[i];
		}
		
		if(all_smaller) return false;
		if(all_larger) return true;
		
		return std::tie(lhs.robots,lhs.material)>std::tie(rhs.robots,rhs.material);
	});
	
	state last{};
	const auto smallereq = [](const auto& lhs, const auto& rhs)
	{
		bool all = true;
		for(int i=0;i<material_type::size;++i)
		{
			all&=lhs.robots[i]<=rhs.robots[i];
			all&=lhs.material[i]<=rhs.material[i];
		}
		return all;
	};
	
	for(const auto& w: next)
	{
		if(smallereq(w,last))
			continue;
		
		last = w;
		result.push_back(w);
	}
	
	return result;
}

int maximize(blueprint b, int depth)
{	
	std::array<std::uint8_t,material_type::size> init_robots{}, init_material{};
	init_robots[material_type::ore] = 1;
	
	std::vector<state> to_visit;
	to_visit.push_back({init_robots,init_material});
	
	for(int i=0;i<depth;++i)
	{
		std::vector<state> next;
		
		for(auto [robots, material]: to_visit)
		{
			const auto could_build = [&,old_mat = material](int type)
			{
				for(int i=0;i<material_type::size;++i)
					if(old_mat[i]<b.robot_costs[type].costs[i])
						return false;
				
				return true;
			};
			
			const auto build = [&,&robots=robots,&material=material](int type)
			{
				auto new_robots = robots;
				++new_robots[type];
				
				auto new_material = material;
				for(int i=0;i<material_type::size;++i)
					new_material[i]-=b.robot_costs[type].costs[i];
					
				return std::make_pair(new_robots, new_material);
			};
			
			// If we have enough of a material to pay the most expensive price now and earn at least as much each turn, we can never exhaust that material
			// So throw away the excess to keep states small
			for(int i=0;i<material_type::size-1;++i)
				if(robots[i]>=b.max_worth[i] && material[i]>=b.max_worth[i])
					material[i] = b.max_worth[i];
			
			for(int i=0;i<material_type::size;++i)
				material[i]+=robots[i];
			
			if(could_build(material_type::geode)) // Always build a geode robot if we can
			{
				const auto [new_robots, new_material] = build(3);
				next.push_back({new_robots,new_material});
			}
			else
			{	
				for(int i=0;i<material_type::size-1;++i)
				{
					if(robots[i]>=b.max_worth[i]) continue; // Never build a robot if we already earn enough each turn to pay the most expensive price of that type

					if(could_build(i))
					{
						const auto [new_robots, new_material] = build(i);
						next.push_back({new_robots,new_material});
					}
				}
				next.push_back({robots,material});

			}
		}
		
		std::sort(next.begin(),next.end());
		next.erase(std::unique(next.begin(),next.end()),next.end());
		to_visit = select_best(std::move(next)); 
	}
	
	return std::max_element(to_visit.begin(),to_visit.end(),[](const auto& lhs, const auto& rhs)
	{
		return lhs.material[material_type::geode]<rhs.material[material_type::geode];
	})->material[material_type::geode];
}

int main(int argc, char* argv[])
{
	std::vector<blueprint> blueprints(std::istream_iterator<blueprint>{std::cin},{});
	
	std::cout<<std::transform_reduce(std::execution::par_unseq,blueprints.begin(),blueprints.begin()+3,1,std::multiplies<>{},[](const auto& b)
	{
		return maximize(b,32);
	});

	return 0;
}
