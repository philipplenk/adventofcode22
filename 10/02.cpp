#include <array>
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

struct screen
{
	public:
	void update(const machine_state& state)
	{
		pixels_[(state.cycle-1)/40][(state.cycle-1)%40] = std::abs((state.cycle-1)%40-state.x)<2?true:false;
	}
	
	private:
	std::array<std::array<bool,40>,6> pixels_{};
	
	friend std::ostream& operator<<(std::ostream& out, const screen& scn)
	{
		for(auto& row: scn.pixels_)
		{
			for(auto v: row)
				out<<(v?'#':'.');
			out<<'\n';
		}
		return out;
	}
};

int main(int argc, char* argv[])
{
	screen crt;
	
	const auto cycle_action = [&](auto state)
	{
		crt.update(state);
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
	
	std::cout<<crt<<'\n';
	return 0;
}
