#include <algorithm>
#include <iostream>
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
	return in;
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

int main(int argc, char* argv[])
{
	packet a, b;
	
	int idx = 1, sum = 0;;
	while(std::cin>>a>>b>>std::ws)
	{
		sum+=idx*!!(a<b);
		++idx;
	}
	std::cout<<sum;
}
