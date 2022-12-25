#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>


struct monkey
{
	int divisor;
	std::string arg0, arg1;
	char op;
	std::array<int,2> targets;
	
	std::vector<int> items;
};

std::istream& operator>>(std::istream& in, monkey& m)
{
	m.items.clear();
	
	std::string skip;
	in>>skip>>skip>>skip>>skip;

	std::string items,item;
	std::getline(in,items);
	std::stringstream strm{items};
	while(std::getline(strm,item,','))
		m.items.push_back(std::stoi(item));
	
	in>>skip>>skip>>skip;
	
	in>>m.arg0>>m.op>>m.arg1;
	
	in>>skip>>skip>>skip>>m.divisor;
	in>>skip>>skip>>skip>>skip>>skip>>m.targets[1];
	in>>skip>>skip>>skip>>skip>>skip>>m.targets[0];
	
	return in;
}

std::ostream& operator<<(std::ostream& out, const monkey& m)
{
	for(auto v: m.items) out<<v<<' ';
	out<<'\n';
	out<<m.arg0<<m.op<<m.arg1<<'\n';
	out<<m.targets[0]<<' '<<m.targets[1]<<'\n';
	out<<m.divisor<<'\n';
	return out;
}


int main(int argc, char* argv[])
{
	std::vector<monkey> monkeys(std::istream_iterator<monkey>{std::cin},{});
	std::vector<int> inspections(monkeys.size(),0);
	
	const auto fetch = [](auto type, int old)
	{
		if(type=="old")
			return old;
			
		return std::stoi(type);
	};
	
	const auto eval = [&](const auto& m, int value)
	{
		const auto lhs = fetch(m.arg0,value);
		const auto rhs = fetch(m.arg1,value);
		if(m.op=='+') return lhs+rhs;
		return lhs*rhs;
	};
	
	for(int i=0;i<20;++i)
	{
		for(unsigned m_idx = 0;m_idx<monkeys.size();++m_idx)
		{
			auto& m = monkeys[m_idx];
			for(auto item: m.items)
			{
				++inspections[m_idx];
				item = eval(m,item)/3;
				monkeys[m.targets[item%m.divisor==0]].items.push_back(item);
			}
			m.items.clear();
		}
	}
	
	std::sort(inspections.begin(),inspections.end(),std::greater<>{});
	std::cout<<inspections[0]*inspections[1];
	return 0;
}
