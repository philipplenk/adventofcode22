#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

struct operation
{
	std::string arg0, arg1;
	char op;
};

std::istream& operator>>(std::istream& in, operation& op)
{
	return in>>op.arg0>>op.op>>op.arg1;
}

struct monkey
{
	std::string name;
	std::optional<long long> value;
	std::optional<operation> op;
};

std::istream& operator>>(std::istream& in, monkey& m)
{
	in>>m.name;
	m.name.pop_back();
	in>>std::ws;
	
	m.value.reset();
	m.op.reset();
	
	if(std::isdigit(in.peek()))
	{
		long long val;
		in>>val;
		m.value = val;
	}
	else
	{
		operation op;
		in>>op;
		m.op = op;
	}
	
	return in;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, monkey> monkeys;
	
	monkey m;
	while(std::cin>>m)
		monkeys[m.name] = m;
	
	bool done = false;
	do
	{
		done = true;
		for(auto& [_,m]: monkeys)
		{
			if(!m.value)
			{
				const auto lhs = monkeys.find(m.op->arg0)->second.value;
				const auto rhs = monkeys.find(m.op->arg1)->second.value;
				if(lhs && rhs)
				{
					switch(m.op->op)
					{
						case '+': m.value = *lhs + *rhs; break;
						case '-': m.value = *lhs - *rhs; break;
						case '*': m.value = *lhs * *rhs; break;
						case '/': m.value = *lhs / *rhs; break;						
					}
				}
				else
					done = false;
			}
		}
	}
	while(!done);
			
	std::cout<<*monkeys["root"].value;

	return 0;
}
