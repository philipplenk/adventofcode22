#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

enum class value_kind
{
	value, placeholder, equation
};

struct operation
{
	std::string arg0, arg1;
	char op;
};

struct value_type
{
	value_kind type;
	long long value;
};

std::istream& operator>>(std::istream& in, operation& op)
{
	return in>>op.arg0>>op.op>>op.arg1;
}

struct monkey
{
	std::string name;
	std::optional<value_type> value;
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
		m.value = value_type{};
		m.value->type = value_kind::value;
		m.value->value = val;
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
	
	monkeys["root"].op->op='-';
	monkeys["humn"].value = value_type{};
	monkeys["humn"].value->type = value_kind::placeholder;
	
	
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
					if(lhs->type!=value_kind::value || rhs->type!=value_kind::value)
						m.value = value_type{value_kind::equation,0};
					else
					{
						m.value = value_type{value_kind::value,0};
						switch(m.op->op)
						{
							case '+': m.value->value = lhs->value + rhs->value; break;
							case '-': m.value->value = lhs->value - rhs->value; break;
							case '*': m.value->value = lhs->value * rhs->value; break;
							case '/': m.value->value = lhs->value / rhs->value; break;
							
						}
					}
				}
				else
					done = false;
			}
		}
	}
	while(!done);
	
	
	const auto try_propagate = [&](std::string var, long long value, auto rec)
	{
		if(monkeys[var].value->type==value_kind::value) return;
		if(monkeys[var].value->type==value_kind::placeholder)
		{
			std::cout<<value;
			return;
		}
		
		if(monkeys[monkeys[var].op->arg0].value->type==value_kind::value)
		{
			switch(monkeys[var].op->op)
			{
				case '+': value-=monkeys[monkeys[var].op->arg0].value->value; break;
				case '-': value=monkeys[monkeys[var].op->arg0].value->value-value; break;
				case '*': value/=monkeys[monkeys[var].op->arg0].value->value; break;
				case '/': value=monkeys[monkeys[var].op->arg0].value->value/value; break;
			}
			rec(monkeys[var].op->arg1,value,rec);
		}
		else if(monkeys[monkeys[var].op->arg1].value->type==value_kind::value)
		{
			switch(monkeys[var].op->op)
			{
				case '+': value-=monkeys[monkeys[var].op->arg1].value->value; break;
				case '-': value=monkeys[monkeys[var].op->arg1].value->value+value; break;
				case '*': value/=monkeys[monkeys[var].op->arg1].value->value; break;
				case '/': value=monkeys[monkeys[var].op->arg1].value->value*value; break;
			}
			rec(monkeys[var].op->arg0,value,rec);
		}
		else
			std::cerr<<"This will prove difficult\n";
	};
			
	try_propagate("root",0,try_propagate);
	return 0;
}
