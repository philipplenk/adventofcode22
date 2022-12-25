#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <variant>
#include <vector>

#include <cstddef>

struct packet
{
	std::vector<std::variant<int,packet>> parts;
};

packet parse(const std::string& s, std::size_t& idx)
{
	packet p;
	int val = 0;
	for(;idx<s.size();++idx)
	{
		if(s[idx]==']')
		{
			p.parts.push_back(val);
			break;
		}
		if(std::isdigit(s[idx]))
			val = val*10+s[idx]-'0';
		else if(s[idx]==',')
		{
			p.parts.push_back(val);
			val = 0;
		}
		else
		{
			++idx;
			p.parts.push_back(parse(s,idx));
			++idx;
		}
	}
	return p;
}

std::istream& operator>>(std::istream& in, packet& p)
{
	p.parts.clear();
	std::string line;
	std::getline(in,line);
	std::size_t idx = 0;
	p = parse(line,idx);
	return in>>std::ws;
}

void print(std::ostream& out, const packet& p)
{
	out<<'[';
	for(const auto& part: p.parts)
	{
		std::visit([&](const auto& val)
		{
			out<<val<<',';
		},part);
	}
	out<<']';
}

std::ostream& operator<<(std::ostream& out, const packet& p)
{
	print(out,p);
	return out;
}

const packet& as_packet(const packet& p)
{
	return p;
}

packet as_packet(int i)
{	
	packet p;
	p.parts.push_back(i);
	return p;
}

bool operator<(const packet& lhs, const packet& rhs)
{
	return std::lexicographical_compare(lhs.parts.begin(),lhs.parts.end(),rhs.parts.begin(),rhs.parts.end(),[](const auto& lhs, const auto& rhs)
	{
		return std::visit([](const auto& lhs, const auto& rhs)
		{
			if constexpr(std::is_same_v<decltype(lhs),decltype(rhs)>)
				return lhs<rhs;
			else
				return as_packet(lhs)<as_packet(rhs);
		},lhs,rhs);
	});
}


bool operator==(const packet& lhs, const packet& rhs)
{
	return !(lhs<rhs) && !(rhs<lhs);
}

int main(int argc, char* argv[])
{
	std::vector<packet> packets(std::istream_iterator<packet>{std::cin},{});
	
	std::size_t idx = 0;
	auto div0 = parse("[[2]]",idx);
	idx = 0;
	auto div1 = parse("[[6]]",idx);
	packets.push_back(div0);
	packets.push_back(div1);

	std::sort(packets.begin(),packets.end());
	
	int result = 1;
	for(idx = 0;idx<packets.size();++idx)
	{
		if(packets[idx]==div0 || packets[idx]==div1)
			result*=idx+1;
	}
	std::cout<<result;
}
