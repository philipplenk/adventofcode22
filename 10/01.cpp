#include <iostream>
#include <string>

enum class opcode
{
	noop,
	addx
};

struct instruction
{
	opcode op;
	int arg;
};

std::istream& operator>>(std::istream& in, instruction& ins)
{
	std::string name;
	in>>name;
	if(name=="noop")
		ins.op=opcode::noop;
	else
	{
		ins.op=opcode::addx;
		in>>ins.arg;
	}
	return in;
}

struct machine_state
{
	int cycle = 1;
	int x = 1;
};

int main(int argc, char* argv[])
{
	int sum = 0;
	const auto cycle_action = [&](const machine_state& state)
	{
		if(state.cycle%40==20)
			sum+=state.cycle*state.x;
	};
	
	const auto wait_cycles = [&](machine_state& state, int cycles)
	{
		while(cycles--)
		{
			cycle_action(state);
			++state.cycle;
		}
	};
	
	instruction ins;
	
	machine_state state;
	
	while(std::cin>>ins)
	{	
		switch(ins.op)
		{
			case opcode::noop:
			{
				wait_cycles(state,1);
				break;
			}
			case opcode::addx: 
			{
				wait_cycles(state,2);
				state.x+=ins.arg;
				break;
			}
		}
	}
	std::cout<<sum;
	return 0;
}
